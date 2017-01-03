#!/bin/bash

export CC=gcc
export CXX=g++
export LOGFILE=gcc-snapshot.log
export PATH=/usr/lib/gcc-snapshot/bin:$PATH

source ./tools/ci/scripts/init.sh

aptget_install gcc-snapshot \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

export CXXFLAGS="-pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

source ./tools/ci/flags/gccsnapshot.sh

do_init
run_configure --enable-werror
run_make

source ./tools/ci/scripts/exit.sh

exit 0
