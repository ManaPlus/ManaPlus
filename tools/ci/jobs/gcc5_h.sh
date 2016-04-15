#!/bin/bash

export CC=gcc-5
export CXX=g++-5
export LOGFILE=gcc5_h.log

source ./tools/ci/scripts/init.sh

aptget_install gcc-5 g++-5 \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

do_init

cd src
export dir=$(pwd)
export includes="-I$dir -I/usr/include -I/usr/include/SDL -I/usr/include/libxml2"

run_h $*

cd ..

source ./tools/ci/scripts/exit.sh

exit 0
