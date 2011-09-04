#!/bin/sh

dir=`pwd`
export LANG=C

#--enable-applebuild=yes
#--without-librt

echo start >make2.log

autoreconf -i
echo opengl+debug+guichan >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=yes \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no
make -j5 2>>make2.log

autoreconf -i
echo opengl+guichan >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no
make -j5 2>>make2.log

autoreconf -i
echo opengl >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=no \
--enable-tcmalloc=no \
--enable-googleprofiler=no
make -j5 2>>make2.log

autoreconf -i
echo opengl+manaserv >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=no \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-manaserv=yes
make -j5 2>>make2.log

autoreconf -i
echo opengl+guichan+manaserv >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-manaserv=yes
make -j5 2>>make2.log

autoreconf -i
echo guichan >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--without-opengl
make -j5 2>>make2.log

autoreconf -i
echo "-" >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=no \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--without-opengl
make -j5 2>>make2.log

autoreconf -i
echo guichan+manaserv >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-manaserv=yes \
--without-opengl
make -j5 2>>make2.log

autoreconf -i
echo opengl+debug+guichan+portable >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=yes \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-portable=yes
make -j5 2>>make2.log

autoreconf -i
echo opengl+guichan+portable+manaserv >>make2.log
make clean 2>>make2.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-manaserv=yes \
--enable-portable=yes
make -j5 2>>make2.log
