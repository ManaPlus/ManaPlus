#!/bin/bash

cd ..

FINKDIR=/sw
GCCVER=4.0

. ${FINKDIR}/bin/init.sh
export CC=gcc-${GCCVER}
export CXX=g++-${GCCVER}
export CPP=cpp-${GCCVER}
export CXXCPP=cpp-${GCCVER}
export CPPFLAGS="-I/sw/include"
export LDFLAGS="-framework OpenGL"
dir=`pwd`/run
#dir=${FINKDIR}

autoreconf -i 2>build/fink1.log

# with prefix
#./configure --enable-applebuild=yes \
#--prefix=${dir} \
#--datadir=${dir}/share/games \
#--bindir=${dir}/bin \
#--mandir=${dir}/share/man 2>fink2.log 1>fink2_ok.log

# without prefix
./configure --enable-applebuild=yes \
--datarootdir=${dir}/share/games \
--bindir=${dir}/bin \
--sbindir=${dir}/bin \
--mandir=${dir}/share/man 2>build/fink2.log

# simple
#./configure --enable-applebuild=yes 2>fink2.log 1>fink2_ok.log
make 2>build/fink3.log
