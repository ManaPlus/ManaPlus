#!/bin/bash

dir=`pwd`

clang-tidy -checks="$1" "$3" -- -std=c++11 -x c++ -DENABLE_LIBXML -DTMWA_SUPPORT -DDEBUGBUILD $2 -I$dir -I/usr/include -I/usr/include/SDL -I/usr/include/libxml2
