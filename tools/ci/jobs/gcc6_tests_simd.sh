#!/bin/bash

export CC=gcc-6
export CXX=g++-6
export LOGFILE=gcc6.log

source ./tools/ci/scripts/init.sh

aptget_install gcc-6 g++-6 \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev \
    valgrind

# remove -flto -fwhole-program because gcc bug with lto + target attribute

export CXXFLAGS="-pedantic -ggdb3 -O5 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-ffast-math -funswitch-loops \
-funsafe-loop-optimizations \
-march=native -fsched-pressure
-std=gnu++1z -Wformat=1 \
-fno-var-tracking -Wno-attributes"

source ./tools/ci/flags/gcc6.sh

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
