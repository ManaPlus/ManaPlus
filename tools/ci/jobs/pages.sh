#!/bin/bash

export LOGFILE=pages.log

source ./tools/ci/scripts/init.sh

aptget_install git-core python

tools/ci/scripts/retry.sh git clone https://gitlab.com/4144/pagesindexgen.git pagesindexgen

mkdir -p public/docs
mkdir -p public/stats
mkdir -p public/gcov

cp logs/gcc-6.* public/gcov
cp logs/gcc-6_SDL2.* public/gcov

cd pagesindexgen
./pagesindexgen.py ../public
check_error $?

cd ..
cp -r doxygen/html/* public/docs
cp -r stats/* public/stats

