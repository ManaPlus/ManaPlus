#!/bin/bash

dir=`pwd`

autoreconf -i
./configure --prefix=$dir/run \
--enable-commandlinepassword \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-portable=yes

cd po
make update-gmo
cd ..
make -j4

if [ ! -d "run" ]; then
    mkdir run
fi

make install
