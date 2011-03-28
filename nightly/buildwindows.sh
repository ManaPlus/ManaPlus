#!/bin/bash

dir=`pwd`

cd ..
export PATH=/usr/i586-mingw32msvc/bin:$PATH
CXXFLAGS="-Wall -g0 -O2" cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain.nightly .

make -j5 2>make.log

cd ./packaging/windows
./make-translations.sh
makensis -DDLLDIR=$dir/../packaging/windows/libs/dll/ \
    -DPRODUCT_VERSION="nightly-$1" \
    -DEXESUFFIX=/src \
    -DUPX=true \
    setup.nsi

