#!/bin/bash

export CC=clang-3.4
export CXX=clang++-3.4
export LOGFILE=clang34.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-stdlib=libc++"

do_init
run_configure --enable-werror
run_make

source ./tools/ci/scripts/exit.sh

exit 0
