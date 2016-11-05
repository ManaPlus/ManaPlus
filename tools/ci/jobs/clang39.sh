#!/bin/bash

export CC=clang-3.9
export CXX=clang++-3.9
export LOGFILE=clang39.log

source ./tools/ci/scripts/init.sh

aptget_install clang-3.9 libc++-dev libc++abi-dev \
    make autoconf automake autopoint gettext libphysfs-dev \
    libxml2-dev libcurl4-gnutls-dev libpng-dev \
    libsdl-gfx1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev libsdl-ttf2.0-dev

export CXXFLAGS="-Weverything -Wno-documentation -Wno-padded -Wno-sign-conversion \
-Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables \
-Wno-overloaded-virtual -Wno-covered-switch-default -Wno-float-equal -Wno-conversion \
-Wno-shorten-64-to-32 -Wno-missing-variable-declarations \
-Wno-disabled-macro-expansion -Wno-format-nonliteral -stdlib=libc++ \
-Wno-reserved-id-macro -Wno-packed -Wno-documentation-unknown-command \
-Wno-variadic-macros -Wno-double-promotion"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
