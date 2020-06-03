#!/usr/bin/env bash

export CC=gcc-9
export CXX=g++-9

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc9.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++2a -Wformat=1 \
-Wno-attributes"

source ./tools/ci/flags/gcc9.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure --enable-unittests=yes --with-gcov $*
export SDL_VIDEODRIVER=dummy
run_make_check
run_gcov gcov-9 gcc-9

source ./tools/ci/scripts/exit.sh

exit 0
