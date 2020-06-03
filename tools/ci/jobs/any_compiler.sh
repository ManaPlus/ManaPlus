#!/usr/bin/env bash

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

source ./tools/ci/scripts/init.sh

do_init
run_configure $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
