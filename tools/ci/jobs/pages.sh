#!/bin/bash

export LOGFILE=pages.log

source ./tools/ci/scripts/init.sh

aptget_install git-core

tools/ci/scripts/retry.sh git clone https://gitlab.com/4144/pagesindexgen.git pagesindexgen

mkdir -p public/docs
mkdir -p public/stats

cd pagesindexgen
./pagesindexgen.py ../public

cd ..
cp -r doxygen/html/* public/docs
cp -r stats/* public/stats

