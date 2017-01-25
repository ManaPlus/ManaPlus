#!/bin/bash

export CC=gcc-4.9
export CXX=g++-4.9
export LOGFILE=cmake_gcc49.log

source ./tools/ci/scripts/init.sh

do_init
run_cmake
run_make

source ./tools/ci/scripts/exit.sh

exit 0
