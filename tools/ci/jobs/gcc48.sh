#!/usr/bin/env bash

export CC=gcc-4.8
export CXX=g++-4.8

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc47.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
