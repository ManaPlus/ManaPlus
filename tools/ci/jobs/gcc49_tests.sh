#!/usr/bin/env bash

export CC=gcc-4.9
export CXX=g++-4.9

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc49_tests.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-ggdb3 -O2 -pipe -ffast-math \
-fno-omit-frame-pointer \
-D_FORTIFY_SOURCE=2 -std=gnu++1y"

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
