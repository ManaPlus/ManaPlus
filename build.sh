#!/bin/bash

dir=`pwd`

autoreconf -i
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man

cd po
make update-gmo
cd ..
make
if [ ! -d "run" ]; then
    mkdir run
fi
make install
