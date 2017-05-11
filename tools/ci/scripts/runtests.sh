#!/bin/bash

export SDL_VIDEODRIVER="dummy"
ulimit -c unlimited -S
ulimit -c unlimited
rm -rf core*

export HOME="logs/home"

echo "clean config runs"
n=0
while true; do
    echo "run test ${n}"
    rm "${HOME}/.config/mana/mana/config.xml"
    rm "/root/.config/mana/mana/config.xml"
    ./tools/ci/scripts/runtest.sh
    if [ "$?" != 0 ]; then
        exit 1
    fi
    if [[ $n -ge 5 ]]; then
        break
    fi
    sleep 5s
    n=$((n+1))
done

echo "existing config runs"
n=0
while true; do
    echo "run test ${n}"
    ./tools/ci/scripts/runtest.sh
    if [ "$?" != 0 ]; then
        exit 1
    fi
    if [[ $n -ge 5 ]]; then
        exit 0
    fi
    sleep 5s
    n=$((n+1))
done
