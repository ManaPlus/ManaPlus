#!/bin/bash

export CC=clang
export CXX=clang++
export LOGFILE=$1.log

source ./tools/ci/scripts/init.sh

do_init
run_configure --enable-werror
run_make

source ./tools/ci/scripts/exit.sh

exit 0
