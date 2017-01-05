#!/bin/bash

export LOGFILE2=logs/deheader2.log

source ./tools/ci/scripts/init.sh

aptget_install python wget \
    gcc-5 g++-5 \
    libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl2-gfx-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev

export dir=$(pwd)

rm deheader
tools/ci/scripts/retry.sh wget "https://gitlab.com/4144/deheader/raw/master/deheader"
chmod +x deheader
