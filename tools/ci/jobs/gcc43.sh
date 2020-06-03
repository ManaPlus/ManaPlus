#!/usr/bin/env bash

export CC=gcc-4.3.6-r1
export CXX=g++-4.3.6-r1

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc43.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

do_init
run_configure $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
