#!/usr/bin/env bash

dir=`pwd`

clang-tidy -checks="$1" "$3" -- -std=c++11 -x c++ $2 -I$dir ${INCLUDES}
