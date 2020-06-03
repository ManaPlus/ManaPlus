#!/usr/bin/env bash

export CC=gcc
export CXX=g++

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

if [ "$FLAGS" == "" ]; then
    export FLAGS="gccsnapshot.sh"
fi

export LOGFILE=gcc-snapshot.log
export PATH=/usr/lib/gcc-snapshot/bin:$PATH

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes \
-Wno-deprecated-copy"

source ./tools/ci/flags/${FLAGS}

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
