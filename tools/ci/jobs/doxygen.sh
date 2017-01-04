#!/bin/bash

export LOGFILE=doxygen.log

source ./tools/ci/scripts/init.sh

aptget_install doxygen graphviz \
    libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

doxygen tools/doxygen/doxygen.conf
check_error $?
