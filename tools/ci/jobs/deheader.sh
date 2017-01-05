#!/bin/bash

export LOGFILE2=logs/deheader2.log

source ./tools/ci/scripts/init.sh

aptget_install python wget \
    gcc-5 g++-5 \
    libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

export dir=$(pwd)

rm deheader
tools/ci/scripts/retry.sh wget "https://gitlab.com/4144/deheader/raw/master/deheader"
chmod +x deheader
