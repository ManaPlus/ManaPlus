#!/bin/bash

export CC=gcc-4.9
export CXX=g++-4.9
export LOGFILE=gcc49.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-fno-var-tracking"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
