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
  64-mcf|64-ucrt|64-msvcrt|64)
    _ARCH=x86_64
    _EH=seh
    ;;
  32-mcf|32-ucrt|32-msvcrt|32)
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
      build-essential qttools5-dev-tools \
      curl git zstd
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
  mkdir -p "$_BUILD_DIR"/{qtbase,qtsvg,qttranslations}

  # in-tree build required, or `sed` will fail due to misgenerated paths
  git archive --format=tar HEAD | tar -x -C "$_BUILD_DIR/qtbase"

  if [[ ! -d "assets/qtsvg-$_QT_BRANCH" ]]; then
    git clone --branch "$_QT_BRANCH" --depth 1 https://github.com/qt/qtsvg.git "assets/qtsvg-$_QT_BRANCH"
  fi

  if [[ ! -d "assets/qttranslations-$_QT_BRANCH" ]]; then
    git clone --branch "$_QT_BRANCH" --depth 1 https://github.com/qt/qttranslations.git "assets/qttranslations-$_QT_BRANCH"
  fi
}

function build_qtbase() {
  build_dir="$_BUILD_DIR/qtbase"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  ./bin/syncqt.pl -version "$QT_VERSION"
  ./configure \
    -prefix "$_MINGW_LITE_X_DIR/$_TARGET" \
    -hostprefix "$_MINGW_LITE_X_DIR" \
    -opensource -confirm-license \
    -release -optimize-size -static -static-runtime -platform linux-g++ \
    -xplatform win32-g++ -device-option CROSS_COMPILE="$_TARGET-" \
    -opengl desktop -no-angle -no-icu -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -no-fontconfig -qt-harfbuzz -no-ssl -no-openssl \
    -no-style-windowsvista -no-direct2d -no-directwrite -no-feature-directwrite2 \
    -nomake examples -nomake tests -nomake tools
  make -j$(nproc)
  make install
  popd
}

function build_qtsvg() {
  build_dir="$_BUILD_DIR/qtsvg"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qmake "$_PROJECT_ROOT/assets/qtsvg-$_QT_BRANCH"
  make -j$(nproc)
  make install
  popd
}

function dummy_qttools() {
  ln -sf /usr/bin/lrelease "$_MINGW_LITE_X_DIR/bin/lrelease"
}

function build_qttranslations() {
  build_dir="$_BUILD_DIR/qttranslations"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qmake "$_PROJECT_ROOT/assets/qttranslations-$_QT_BRANCH"
  make -j$(nproc)
  make install
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
build_qtbase
build_qtsvg
dummy_qttools
build_qttranslations
package
