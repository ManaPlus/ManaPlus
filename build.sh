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
mkdir run
make install