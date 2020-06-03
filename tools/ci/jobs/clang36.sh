#!/usr/bin/env bash

export CC=clang-3.6
export CXX=clang++-3.6

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=clang36.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -stdlib=libc++ -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
