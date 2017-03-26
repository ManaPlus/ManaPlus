#!/bin/bash

export CC=gcc-4.1
export CXX=g++-4.1

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc41.log

source ./tools/ci/scripts/init.sh

do_init
run_configure $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
