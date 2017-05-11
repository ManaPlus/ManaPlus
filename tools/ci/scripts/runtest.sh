#!/bin/bash

function check_is_run {
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
}

function run {
    ./src/manaplus --enable-ipc --renderer=0 >logs/run.log 2>&1 &
    export PID=$!
    sleep 12s
}

function kill_app {
    kill -s SIGTERM ${PID}
    export RET=$?

    sleep 10s

    if [ "${RET}" != 0 ]; then
        echo "Error: process not responsing to termination signal"
        kill -s SIGKILL ${PID}
        cat logs/run.log
        exit 1
    fi
}

function final_log {
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
}

function send_command {
    echo -n "$1" | nc 127.0.0.1 44007
}

run
check_is_run

send_command "/screenshot 1_"
sleep 5s
check_is_run

# send down key
send_command "/guikey -960 keyGUIDown"
sleep 5s
check_is_run

send_command "/screenshot 2_"
sleep 5s
check_is_run

# final delay
sleep 5s

kill_app
final_log

exit 0
