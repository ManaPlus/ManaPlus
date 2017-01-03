#!/bin/bash

export CC=gcc-6
export CXX=g++-6
export LOGFILE=gcc6.log

source ./tools/ci/scripts/init.sh

aptget_install gcc-6 g++-6 \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

export CXXFLAGS="-ggdb3 -pipe -ffast-math -O9 -flto -fwhole-program \
-fno-omit-frame-pointer -funswitch-loops -D_FORTIFY_SOURCE=2 -std=gnu++1z \
-Wno-attributes"

source ./tools/ci/flags/gcc6.sh

do_init
run_configure --enable-werror
run_make

source ./tools/ci/scripts/exit.sh

exit 0
