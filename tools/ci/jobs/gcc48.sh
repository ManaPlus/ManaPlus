#!/bin/bash

export CC=gcc-4.8
export CXX=g++-4.8
export LOGFILE=gcc47.log

source ./tools/ci/scripts/init.sh

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
