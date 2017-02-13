#!/bin/bash

export CC=gcc-4.7
export CXX=g++-4.7
export LOGFILE=gcc47.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-ggdb3 -O2 -pipe -ffast-math \
-funswitch-loops \
-std=c++0x"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
