#!/bin/bash

export SDL_VIDEODRIVER="dummy"
ulimit -c unlimited -S
ulimit -c unlimited
rm -rf core*

./src/manaplus --renderer=0 >logs/run.log 2>&1 &
export PID=$!
sleep 12s

kill -0 ${PID}
if [ "$?" != 0 ]; then
    echo "Error: process look like crashed"
    cat logs/run.log
    echo "Run with gdb"
    cp ./src/manaplus ./logs/
    cp -r core* ./logs/
    COREFILE=$(find . -maxdepth 1 -name "core*" | head -n 1)
    if [[ -f "$COREFILE" ]]; then
        gdb -c "$COREFILE" ./src/manaplus -ex "thread apply all bt" -ex "set pagination 0" -batch
    fi
    exit 1
fi
kill -s SIGTERM ${PID}
export RET=$?

sleep 10s

if [ "${RET}" != 0 ]; then
    echo "Error: process not responsing to termination signal"
    kill -s SIGKILL ${PID}
    cat logs/run.log
    exit 1
fi

export DATA=$(cat logs/run.log)
if [[ -z "${DATA}" ]]; then
    echo "Error: no output"
    exit 1
fi

cat logs/run.log

export DATA=$(grep "[.]cpp" logs/run.log)
if [[ -n "${DATA}" ]]; then
    echo "Error: possible leak detected"
    echo "${DATA}"
    exit 1
fi
exit 0
