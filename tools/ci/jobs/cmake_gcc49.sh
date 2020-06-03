#!/usr/bin/env bash

export CC=gcc-4.9
export CXX=g++-4.9

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=cmake_gcc49.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

cp CMakeLists.txt.legacy CMakeLists.txt

do_init
run_cmake
run_make

source ./tools/ci/scripts/exit.sh

exit 0
