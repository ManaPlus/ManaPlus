#!/usr/bin/env bash

export CC=gcc-5
export CXX=g++-5

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc5_h.log

source ./tools/ci/scripts/init.sh

do_init

cd src
export dir=$(pwd)
export includes="-I$dir -I/usr/include -I/usr/include/libxml2"

run_h $*

cd ..

source ./tools/ci/scripts/exit.sh

exit 0
