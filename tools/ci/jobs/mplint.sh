#!/bin/bash

export CC=gcc
export CXX=g++

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=mplint_po.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-std=gnu++11"

do_init

rm -rf mplint || true
gitclone https://gitlab.com/manaplus mplint.git
cd mplint
run_configure_simple
run_make
cd ..
echo " " >config.h
run_mplint $*

source ./tools/ci/scripts/exit.sh

exit 0
