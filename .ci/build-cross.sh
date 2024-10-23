#!/bin/bash

set -euxo pipefail

_CLEAN=0
_DEBUG=0  # dummy
_PROFILE=""
while [[ $# -ge 1 ]]; do
  case "$1" in
    -c|--clean)
      _CLEAN=1
      shift
      ;;
    -p|--profile)
      _PROFILE="$2"
      shift
      shift
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

case "$_PROFILE" in
  64-mcf|64-ucrt|64-msvcrt)
    _ARCH=x86_64
    _EH=seh
    ;;
  32-mcf|32-ucrt|32-msvcrt)
    _ARCH=i686
    _EH=dwarf
    ;;
  *)
    echo "Unknown profile: $_PROFILE"
    exit 1
    ;;
esac
_TARGET="$_ARCH-w64-mingw32"

. $(dirname "$0")/common.sh

function clean_cross_toolchain() {
  if [[ "$_CLEAN" -eq 1 && -d "$_MINGW_LITE_X_DIR" ]]; then
    rm -rf "$_MINGW_LITE_X_DIR"
  fi
}

function setup_host_toolchain() {
  apt update
  env DEBIAN_FRONTEND=noninteractive \
    apt install -y --no-install-recommends \
      cmake g++ gcc ninja-build \
      curl ca-certificates git zstd
}

function setup_cross_toolchain() {
  if [[ ! -d "$_MINGW_LITE_X_DIR" ]]; then
    if [[ ! -f "assets/$_MINGW_LITE_X_ARCHIVE" ]]; then
      curl -L -o "assets/$_MINGW_LITE_X_ARCHIVE" "$_MINGW_LITE_X_URL"
    fi
    zstdcat "assets/$_MINGW_LITE_X_ARCHIVE" | tar -x -C /
  fi
}

function prepare_source() {
  mkdir -p "$_BUILD_DIR"/{qtbase-host,qtbase-target,qtsvg,qttools-host,qttools-target,qttranslations}

  if [[ ! -d "assets/qtsvg-$_QT_BRANCH" ]]; then
    git clone --branch "$_QT_BRANCH" --depth 1 https://github.com/qt/qtsvg.git "assets/qtsvg-$_QT_BRANCH"
  fi

  if [[ ! -d "assets/qttools-$_QT_BRANCH" ]]; then
    git clone --branch "$_QT_BRANCH" --depth 1 https://github.com/qt/qttools.git "assets/qttools-$_QT_BRANCH"
  fi

  if [[ ! -d "assets/qttranslations-$_QT_BRANCH" ]]; then
    git clone --branch "$_QT_BRANCH" --depth 1 https://github.com/qt/qttranslations.git "assets/qttranslations-$_QT_BRANCH"
  fi
}

function build_host_qtbase() {
  build_dir="$_BUILD_DIR/qtbase-host"
  pushd "$build_dir"
  "$_PROJECT_ROOT/configure" \
    -prefix "$_MINGW_LITE_X_DIR" \
    $( true === build options === ) \
    -cmake-generator Ninja \
    -release \
    -optimize-size \
    -static \
    -platform linux-g++ \
    -unity-build \
    $( true === component selection === ) \
    -nomake examples \
    -nomake tests \
    -nomake tools \
    $( true === core options === ) \
    -qt-doubleconversion \
    -no-icu \
    -qt-pcre \
    -qt-zlib \
    $( true === network options === ) \
    -no-ssl \
    $( true === gui, printing, widget options === ) \
    -no-gui \
    $( true === database options === ) \
    -sql-sqlite \
    -qt-sqlite
  cmake --build . --parallel
  cmake --install .
  popd
}

function build_host_qttools() {
  build_dir="$_BUILD_DIR/qttools-host"
  pushd "$build_dir"
  qt-configure-module "$_PROJECT_ROOT/assets/qttools-$_QT_BRANCH"
  cmake --build . --parallel
  cmake --install .
  popd
}

function build_target_qtbase() {
  build_dir="$_BUILD_DIR/qtbase-target"
  pushd "$build_dir"
  "$_PROJECT_ROOT/configure" \
    -prefix "$_MINGW_LITE_X_DIR/$_TARGET" \
    $( true === configure meta === ) \
    -no-feature-style-windowsvista \
    $( true === build options === ) \
    -cmake-generator "Ninja" \
    -debug \
    -optimize-debug \
    -static \
    -platform linux-g++ \
    -xplatform win32-g++ \
    -device-option CROSS_COMPILE="$_TARGET-" \
    -static-runtime \
    $( true === component selection === ) \
    -nomake examples \
    -nomake tests \
    -nomake tools \
    $( true === core options === ) \
    -qt-doubleconversion \
    -no-icu \
    -qt-pcre \
    -qt-zlib \
    $( true === network options === ) \
    -no-ssl \
    -no-openssl \
    -no-schannel \
    $( true === gui, printing, widget options === ) \
    -qt-freetype \
    -qt-harfbuzz \
    -opengl dynamic \
    $( true === gui, printing, widget options === ) \
    -qt-libpng \
    -qt-libjpeg \
    $( true === database options === ) \
    -sql-sqlite \
    -qt-sqlite \
    CMAKE_TOOLCHAIN_FILE="$_PROJECT_ROOT/.ci/cmake/$_TARGET.cmake" \
    QT_HOST_PATH=$_MINGW_LITE_X_DIR
  cmake --build . --parallel
  cmake --install .
  popd
}

function build_target_qtsvg() {
  build_dir="$_BUILD_DIR/qtsvg"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qt-configure-module "$_PROJECT_ROOT/assets/qtsvg-$_QT_BRANCH"
  cmake --build . --parallel
  cmake --install .
  popd
}

function build_target_qttools() {
  build_dir="$_BUILD_DIR/qttools-target"
  pushd "$build_dir"
  bash
  qt-configure-module "$_PROJECT_ROOT/assets/qttools-$_QT_BRANCH" \
    -no-feature-assistant
  cmake --build . --parallel
  cmake --install .
  popd
}

function build_target_qttranslations() {
  build_dir="$_BUILD_DIR/qttranslations"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qt-configure-module "$_PROJECT_ROOT/assets/qttranslations-$_QT_BRANCH"
  cmake --build . --parallel
  cmake --install .
  popd
}

function package() {
  tar -c "$_MINGW_LITE_X_DIR" | zstd -9 --force -o "build/$_REDPANDA_QT_X_ARCHIVE"
}

clean_build
clean_cross_toolchain
prepare_dirs
setup_host_toolchain
setup_cross_toolchain
prepare_source
export PATH="$_MINGW_LITE_X_DIR/bin:$PATH"
build_host_qtbase
build_host_qttools
export PATH="$_MINGW_LITE_X_DIR/$_TARGET/bin:$PATH"
build_target_qtbase
build_target_qtsvg
build_target_qttools
build_target_qttranslations
package
