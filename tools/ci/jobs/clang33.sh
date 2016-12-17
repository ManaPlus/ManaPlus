#!/bin/bash

export CC=clang-3.3
export CXX=clang++-3.3
export LOGFILE=clang33.log

source ./tools/ci/scripts/init.sh

aptget_install clang-3.3 libc++-dev \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

export CXXFLAGS="-stdlib=libc++"

do_init
run_configure --enable-werror
run_make

source ./tools/ci/scripts/exit.sh

exit 0
