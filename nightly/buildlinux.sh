#!/bin/bash

cd ..

DIR=`pwd`
DSTDIR=packaging/build/linux

autoreconf -i
#make clean
./configure
make

if [ -e src/manaplus ];
then
    strip src/manaplus
    rm -rf $DSTDIR
    mkdir -p $DSTDIR
    cp -r data $DSTDIR
    mkdir -p $DSTDIR/bin
    cp src/manaplus $DSTDIR/bin
    cp nightly/runme.sh $DSTDIR
fi
