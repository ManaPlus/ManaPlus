#!/bin/bash

export CC=gcc-4.5
export CXX=g++-4.5
export LOGFILE=gcc45.log

source ./tools/ci/scripts/init.sh

do_init
run_configure --enable-werror
run_make

source ./tools/ci/scripts/exit.sh

exit 0
