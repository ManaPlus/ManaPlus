#!/usr/bin/env bash

export CC=gcc-7
export CXX=g++-7

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc7.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

source ./tools/ci/flags/gcc7.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure --enable-unittests=yes --with-sdl2 --with-gcov $*
export SDL_VIDEODRIVER=dummy
run_make_check
run_gcov gcov-7 gcc-7_SDL2

source ./tools/ci/scripts/exit.sh

exit 0
