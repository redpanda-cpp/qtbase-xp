#!/bin/bash

set -euxo pipefail

_CLEAN=0
_DEBUG=0
_PROFILE=""
_USE_MINGW_BUILDS=0
while [[ $# -ge 1 ]]; do
  case "$1" in
    -c|--clean)
      _CLEAN=1
      shift
      ;;
    -d|--debug)
      _DEBUG=1
      shift
      ;;
    -p|--profile)
      _PROFILE="$2"
      shift
      shift
      ;;
    --use-mingw-builds)
      _USE_MINGW_BUILDS=1
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

. $(dirname "$0")/common.sh

if [[ "$_DEBUG" -eq 0 ]]; then
  _REDPANDA_QT_DIR="/c/Qt/$_REDPANDA_QT_RELEASE/$_PROFILE"
else
  _REDPANDA_QT_DIR="/c/Qt/$_REDPANDA_QT_RELEASE/${_PROFILE}d"
fi
_REDPANDA_QT_PARENT="/c/Qt/$_REDPANDA_QT_RELEASE"

function clean_toolchain() {
  if [[ "$_CLEAN" -eq 1 && -d "$_REDPANDA_QT_DIR" ]]; then
    rm -rf "$_REDPANDA_QT_DIR"
  fi
}

function setup_toolchain() {
  if [[ ! -d "$_REDPANDA_QT_DIR" ]]; then
    if [[ "$_USE_MINGW_BUILDS" -eq 0 ]]; then
      if [[ ! -f "assets/$_MINGW_LITE_ARCHIVE" ]]; then
        curl -L -o "assets/$_MINGW_LITE_ARCHIVE" "$_MINGW_LITE_URL"
      fi
      mkdir -p "$_REDPANDA_QT_PARENT"
      pushd "$_REDPANDA_QT_PARENT"
      7z x "$_PROJECT_ROOT/assets/$_MINGW_LITE_ARCHIVE"
      mv "$_MINGW_LITE_DIR" "$_REDPANDA_QT_DIR"
      popd
    else
      if [[ ! -f "assets/$_MINGW_BUILDS_ARCHIVE" ]]; then
        curl -L -o "assets/$_MINGW_BUILDS_ARCHIVE" "$_MINGW_BUILDS_URL"
      fi
      mkdir -p "$_REDPANDA_QT_PARENT"
      pushd "$_REDPANDA_QT_PARENT"
      7z x "$_PROJECT_ROOT/assets/$_MINGW_BUILDS_ARCHIVE"
      mv "$_MINGW_BUILDS_DIR" "$_REDPANDA_QT_DIR"
      popd
    fi
  fi
}

function prepare_source() {
  mkdir -p "$_BUILD_DIR"/{qtbase,qtsvg,qttools,qttranslations}

  # in-tree build required, or `sed` will fail due to misgenerated paths
  git archive --format=tar HEAD | tar -x -C "$_BUILD_DIR/qtbase"

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

function build_qtbase() {
  if [[ "$_DEBUG" -eq 0 ]]; then
    mode_args="-release -optimize-size"
  else
    mode_args="-debug -optimize-debug -optimized-tools"
  fi

  build_dir="$_BUILD_DIR/qtbase"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  ./bin/syncqt.pl -version "$QT_VERSION"
  ./configure.bat \
    -prefix "$_REDPANDA_QT_DIR" \
    -opensource -confirm-license \
    $mode_args -static -static-runtime -platform win32-g++ \
    -opengl desktop -no-angle -no-icu -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -no-fontconfig -qt-harfbuzz -no-ssl -no-openssl \
    -no-style-windowsvista -no-direct2d -no-directwrite -no-feature-directwrite2 \
    -nomake examples -nomake tests -nomake tools
  mingw32-make -j$(nproc)
  mingw32-make install
  popd
}

function build_qtsvg() {
  build_dir="$_BUILD_DIR/qtsvg"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qmake "$_PROJECT_ROOT/assets/qtsvg-$_QT_BRANCH"
  mingw32-make -j$(nproc)
  mingw32-make install
  popd
}

function build_qttools() {
  build_dir="$_BUILD_DIR/qttools"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qmake "$_PROJECT_ROOT/assets/qttools-$_QT_BRANCH"
  mingw32-make -j$(nproc)
  mingw32-make install
  popd
}

function build_qttranslations() {
  build_dir="$_BUILD_DIR/qttranslations"
  mkdir -p "$build_dir"
  pushd "$build_dir"
  qmake "$_PROJECT_ROOT/assets/qttranslations-$_QT_BRANCH"
  mingw32-make -j$(nproc)
  mingw32-make install
  popd
}

function package() {
  if [[ "$_DEBUG" -eq 1 ]]; then
    find "$_REDPANDA_QT_DIR/bin" -name '*.exe' -not -name 'strip.exe' -exec strip '{}' ';'
  fi
  tar -c -C /c/Qt "${_REDPANDA_QT_DIR#/c/Qt/}" | zstd -9 --force -o "build/$_REDPANDA_QT_ARCHIVE"
}

clean_build
clean_toolchain
prepare_dirs
setup_toolchain
prepare_source
export PATH="$_REDPANDA_QT_DIR/bin:$PATH"
build_qtbase
build_qtsvg
build_qttools
build_qttranslations
package
