#!/bin/bash

export CC=gcc-3.4.6
export CXX=g++-3.4.6

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc3.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

do_init
run_configure $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
