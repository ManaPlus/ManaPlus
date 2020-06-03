#!/usr/bin/env bash

export libname="$1_$2"

rm -rf /usr/local/spm/bin/${libname}
mkdir -p /usr/local/spm/bin

cp -r ./bin /usr/local/spm/
ls /usr/local/spm/bin/${libname}
if [ "$?" != 0 ]; then
    echo "Library $1 $2 copy failed"
    exit 1
fi
