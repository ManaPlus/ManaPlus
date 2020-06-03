#!/usr/bin/env bash

source ./tools/ci/scripts/winvars.sh || exit 1
source ./tools/ci/flags/mingw6.sh || exit 1

$CC --version
$CXX --version

export MXE="/mxe/usr/${CROSS}/bin/"
export DSTDIR="$1"

function mxefile {
    cp ${MXE}${1} ${DSTDIR} || exit 1
}

function mxefile1 {
    cp ${MXE}${1} ${DSTDIR} || true
}

mxefile gdb.exe
mxefile libcurl-4.dll
mxefile libdl.dll
mxefile libexpat-1.dll
mxefile libfreetype-6.dll
mxefile libiconv-2.dll
mxefile libintl-8.dll
mxefile libjpeg-9.dll
mxefile libogg-0.dll
mxefile libpng16-16.dll
mxefile libreadline6.dll
mxefile libSDL_gfx-15.dll
mxefile libstdc++-6.dll
mxefile libtermcap.dll
mxefile libvorbis-0.dll
mxefile libvorbisfile-3.dll
mxefile libxml2-2.dll
mxefile libwinpthread-1.dll
mxefile SDL.dll
mxefile SDL_image.dll
mxefile SDL_net.dll
mxefile SDL_ttf.dll
mxefile zlib1.dll
mxefile1 libgcc_s_sjlj-1.dll
mxefile1 libgcc_s_seh-1.dll
