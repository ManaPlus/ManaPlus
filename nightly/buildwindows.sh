#!/bin/bash

dir=`pwd`

cd ..
export PATH=/usr/i586-mingw32msvc/bin:$PATH
CXXFLAGS="-Wall -g0 -O3" cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain.nightly .
result=$?
if [ "$result" != 0 ]; then
    exit $result
fi

make -j5 2>make.log
result=$?
if [ "$result" != 0 ]; then
    exit $result
fi

cd ./packaging/windows
./make-translations.sh
result=$?
if [ "$result" != 0 ]; then
    exit $result
fi

makensis -DDLLDIR=$dir/../packaging/windows/libs/dll/ \
    -DPRODUCT_VERSION="nightly-$1" \
    -DEXESUFFIX=/src \
    -DUPX=true \
    setup.nsi

