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

# remove -flto -fwhole-program because gcc bug with lto + target attribute

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O5 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-ffast-math -funswitch-loops \
-funsafe-loop-optimizations \
-march=native -fsched-pressure
-std=gnu++2a -Wformat=1 \
-fno-var-tracking -Wno-attributes"

source ./tools/ci/flags/gcc9.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
