#!/usr/bin/env bash

export CC=gcc-4.9
export CXX=g++-4.9

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc49.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -fno-var-tracking -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
