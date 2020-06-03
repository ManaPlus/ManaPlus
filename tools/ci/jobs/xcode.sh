#!/usr/bin/env bash

uname -a
sudo xcode-select -switch /Applications/${xcodename} || exit 1
cd packaging/OSX || exit 1
./gethttps.sh || exit 1
cd ../../Xcode || exit 1
./build.sh || exit 1
./pack.sh || exit 1
ls -la build || exit 1
cp build/manaplus.dmg "${artifacts}/" || exit 1
