#!/usr/bin/env bash

export LOGFILE=pages.log

source ./tools/ci/scripts/init.sh

tools/ci/scripts/retry.sh git clone https://gitlab.com/4144/pagesindexgen.git pagesindexgen

mkdir -p ../public/docs
mkdir -p ../public/stats
mkdir -p ../public/stats2
mkdir -p ../public/gcov
mkdir -p ../public/version

cp ./logs/gcc-?.* ../public/gcov || exit 1
cp ./logs/gcc-?_SDL2.* ../public/gcov || exit 1
date >../public/version/commit.txt
echo "Url: $CI_PROJECT_URL" >>../public/version/commit.txt
echo "Ref name: $CI_BUILD_REF_NAME" >>../public/version/commit.txt
echo "Ref: $CI_BUILD_REF" >>../public/version/commit.txt

cd pagesindexgen
./pagesindexgen.py ../../public
check_error $?

cd ..
cp -r ./doxygen/html/* ../public/docs || exit 1
cp -r ./stats/* ../public/stats || exit 1
cp -r ./stats2/* ../public/stats2 || exit 1
