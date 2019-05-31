#!/bin/bash

dir=`pwd`

cd ..
export PATH=/usr/i586-mingw32msvc/bin:$PATH
export CXXFLAGS="-Wall -g1 -O3"
cmake -DCMAKE_TOOLCHAIN_FILE=./build/toolchain.nightly .
result=$?
if [ "$result" != 0 ]; then
    exit $result
fi

make -j16 2>build/make.log
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

cp ../../nightly/setup_finish.bmp .
cp ../../nightly/setup_welcome.bmp .

makensis -DDLLDIR=$dir/../packaging/windows/libs/dll/ \
    -DPRODUCT_VERSION="nightly-$1" \
    -DEXESUFFIX=/src \
    -DUPX=true \
    evol.nsi
