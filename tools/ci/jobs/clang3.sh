#!/bin/bash

export CC=clang
export CXX=clang++
export LOGFILE=clang3.log

source ./tools/ci/scripts/init.sh

aptget_install clang libc++-dev \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

run_configure
run_make
run_check_warnings

source ./tools/ci/scripts/exit.sh

exit 0
