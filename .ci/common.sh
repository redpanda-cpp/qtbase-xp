QT_VERSION=6.8.0
LOCAL_VERSION=0

MINGW_LITE_VERSION=14.2.0-r3

MINGW_BUILDS_VERSION=11.5.0-rt_v9-rev0

## internal variables

_MINGW_LITE_BRANCH="${MINGW_LITE_VERSION%%.*}"
_MINGW_LITE_DIR="mingw$_PROFILE-$_MINGW_LITE_BRANCH"
_MINGW_LITE_ARCHIVE="mingw$_PROFILE-$MINGW_LITE_VERSION.7z"
_MINGW_LITE_URL="https://github.com/redpanda-cpp/mingw-lite/releases/download/$MINGW_LITE_VERSION/$_MINGW_LITE_ARCHIVE"
_MINGW_LITE_X_DIR="/opt/x-mingw$_PROFILE-$_MINGW_LITE_BRANCH"
_MINGW_LITE_X_ARCHIVE="x-mingw$_PROFILE-$MINGW_LITE_VERSION.tar.zst"
_MINGW_LITE_X_URL="https://github.com/redpanda-cpp/mingw-lite/releases/download/$MINGW_LITE_VERSION/$_MINGW_LITE_X_ARCHIVE"

_MINGW_BUILDS_GCC="${MINGW_BUILDS_VERSION%%-*}"
_MINGW_BUILDS_RT="${MINGW_BUILDS_VERSION#*-}"
_MINGW_BUILDS_DIR="mingw$_PROFILE"
_MINGW_BUILDS_ARCHIVE="$_ARCH-$_MINGW_BUILDS_GCC-release-posix-$_EH-$_MINGW_BUILDS_RT.7z"
_MINGW_BUILDS_URL="https://github.com/redpanda-cpp/mingw-builds/releases/download/$MINGW_BUILDS_VERSION/$_MINGW_BUILDS_ARCHIVE"

_QT_BRANCH="v$QT_VERSION"
_REDPANDA_QT_RELEASE="$QT_VERSION+redpanda$LOCAL_VERSION"
if [[ "$_DEBUG" -eq 0 ]]; then
  _REDPANDA_QT_ARCHIVE="qt-$_REDPANDA_QT_RELEASE-$_PROFILE.tar.zst"
else
  _REDPANDA_QT_ARCHIVE="qt-$_REDPANDA_QT_RELEASE-${_PROFILE}d.tar.zst"
fi
_REDPANDA_QT_X_ARCHIVE="x-qt-$_REDPANDA_QT_RELEASE-$_PROFILE.tar.zst"

_PROJECT_ROOT="$PWD"
_BUILD_DIR="build/$_PROFILE"

## internal functions

function clean_build() {
  if [[ "$_CLEAN" -eq 1 && -d "$_BUILD_DIR" ]]; then
    rm -rf "$_BUILD_DIR"
  fi
}

function prepare_dirs() {
  mkdir -p "$_BUILD_DIR" assets dist
}
