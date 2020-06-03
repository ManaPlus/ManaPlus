#!/usr/bin/env bash

export zipname="lib.zip"
export libname="$1_$2"

mkdir libdownload
cd libdownload
rm "${zipname}"
../tools/ci/scripts/retry.sh wget -O "${zipname}" "https://gitlab.com/simplepackagemanager/spm/builds/artifacts/$1/download?job=${libname}"
unzip "${zipname}"
cd ..

rm -rf /usr/local/spm/bin/${libname}
mkdir -p /usr/local/spm/bin

cp -r libdownload/bin/${libname} /usr/local/spm/bin/
ls /usr/local/spm/bin/${libname}
if [ "$?" != 0 ]; then
    echo "Library $1 $2 unpack failed"
    exit 1
fi
