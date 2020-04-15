#!/bin/bash

export PATH=/opt/mxe/usr/i686-w64-mingw32.shared/include:/opt/mxe/usr/bin:/opt/mxe/usr/x86_64-pc-linux-gnu/bin:/opt/mxe/usr/${CROSS}/bin:$PATH

export AR=${CROSS}-ar
export AS=${CROSS}-as
export LD=${CROSS}-ld
export NM=${CROSS}-nm
export RC=${CROSS}-windres
export WINDRES=${CROSS}-windres
export ADD2LINE=${CROSS}-addr2line
export STRIP=${CROSS}-strip
export CC=${CROSS}-gcc
export CXX=${CROSS}-g++
export CXXCPP=${CROSS}-cpp
export CPP=${CROSS}-cpp
export RANLIB=${CROSS}-ranlib

export PKG_CONFIG="/opt/mxe/usr/bin/${CROSS}-pkg-config"
export CXXFLAGS="-Wall -g1 -O5 -ffast-math -std=gnu++1z -funswitch-loops -funsafe-loop-optimizations -pipe"
