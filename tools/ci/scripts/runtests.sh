#!/usr/bin/env bash

if [[ ${CI_RUNNER_TAGS} != *"docker"* ]]; then
    echo "Running from shell. Skipping run tests $*"
    exit 0
fi

export SDL_VIDEODRIVER="dummy"
ulimit -c unlimited -S
ulimit -c unlimited
rm -rf core*
sysctl -w kernel.core_pattern=core

export HOME="logs/home"
rm -rf $HOME

export ASAN_OPTIONS=detect_stack_use_after_return=true:strict_init_order=true

echo "clean config runs"
n=0
while true; do
    echo "run test ${n}"
    rm "${HOME}/.config/mana/mana/config.xml"
    rm "/root/.config/mana/mana/config.xml"
    rm "${HOME}/.config/mana/mana/serverlistplus.xml"
    rm "/root/.config/mana/mana/serverlistplus.xml"
    rm "${HOME}/.local/share/mana/manaplus.log"
    rm "/root/.local/share/mana/manaplus.log"
    ./tools/ci/scripts/runtest.sh
    if [ "$?" != 0 ]; then
        exit 1
    fi
    if [[ $n -ge 3 ]]; then
        break
    fi
    sleep 5s
    n=$((n+1))
done

exit 0

echo "existing config runs"
n=0
while true; do
    echo "run test ${n}"
    rm "${HOME}/.local/share/mana/manaplus.log"
    rm "/root/.local/share/mana/manaplus.log"
    ./tools/ci/scripts/runtest.sh
    if [ "$?" != 0 ]; then
        exit 1
    fi
    if [[ $n -ge 1 ]]; then
        exit 0
    fi
    sleep 5s
    n=$((n+1))
done
