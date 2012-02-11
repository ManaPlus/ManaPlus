#!/bin/sh

dir=`pwd`
export LANG=C

cd ..
#--enable-applebuild=yes
#--without-librt

echo start >build/make.log

autoreconf -i
echo opengl+debug+guichan >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=yes \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no
make -j5 2>>build/make.log

autoreconf -i
echo opengl+guichan >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no
make -j5 2>>build/make.log

autoreconf -i
echo opengl >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=no \
--enable-tcmalloc=no \
--enable-googleprofiler=no
make -j5 2>>build/make.log

autoreconf -i
echo opengl+manaserv >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=no \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-manaserv=yes
make -j5 2>>build/make.log

autoreconf -i
echo opengl+guichan+manaserv >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-manaserv=yes
make -j5 2>>build/make.log

autoreconf -i
echo guichan >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--without-opengl
make -j5 2>>build/make.log

autoreconf -i
echo "-" >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=no \
--with-internalguichan=no \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--without-opengl
make -j5 2>>build/make.log

autoreconf -i
echo guichan+manaserv >>build/make.log
make clean 2>>build/make.log
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
make -j5 2>>build/make.log

autoreconf -i
echo opengl+debug+guichan+portable >>build/make.log
make clean 2>>build/make.log
./configure --prefix=$dir/run \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-memdebug=yes \
--with-internalguichan=yes \
--enable-tcmalloc=no \
--enable-googleprofiler=no \
--enable-portable=yes
make -j5 2>>build/make.log

autoreconf -i
echo opengl+guichan+portable+manaserv >>build/make.log
make clean 2>>build/make.log
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
make -j5 2>>build/make.log
