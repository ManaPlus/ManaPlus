#!/bin/bash

export CC=gcc-4.7
export CXX=g++-4.7
export LOGFILE=gcc47.log

source ./tools/ci/scripts/init.sh

aptget_install gcc-4.7 g++-4.7 \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

run_configure
run_make
run_check_warnings

source ./tools/ci/scripts/exit.sh

exit 0
