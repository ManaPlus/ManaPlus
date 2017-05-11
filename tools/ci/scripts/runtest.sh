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
    sleep 5s
    check_is_run
}

function check_exists {
    if [ ! -f "logs/home/Desktop/ManaPlus/$1" ]; then
        echo "Error: image $1 not exists"
        exit 1
    fi
    if [ ! -f "logs/home/Desktop/ManaPlus/$2" ]; then
        echo "Error: image $2 not exists"
        exit 1
    fi
}

function imagesdiff {
    check_exists "$1" "$2"
    diff "logs/home/Desktop/ManaPlus/$1" "logs/home/Desktop/ManaPlus/$2"
    if [ "$?" == 0 ]; then
        echo "Error: images '$1' and '$2' is same."
        exit 1
    fi
}

function imagessame {
    check_exists "$1" "$2"
    diff "logs/home/Desktop/ManaPlus/$1" "logs/home/Desktop/ManaPlus/$2"
    if [ "$?" != 0 ]; then
        echo "Error: images '$1' and '$2' is different."
        exit 1
    fi
}

run
check_is_run

send_command "/screenshot run.png"

# send down key
send_command "/guikey -960 keyGUIDown"
send_command "/screenshot downkey.png"
imagesdiff run.png downkey.png

# send up key
send_command "/guikey -961 keyGUIUp"
send_command "/screenshot run2.png"
imagessame run.png run2.png

# open help
send_command "/help"
send_command "/screenshot help.png"
imagesdiff run2.png help.png
send_command "/help"
send_command "/screenshot run3.png"
imagessame run2.png run3.png

# open settings
send_command "/setup"
send_command "/screenshot setup.png"
imagesdiff run3.png setup.png
send_command "/setup"
send_command "/screenshot run4.png"
imagessame run3.png run4.png

# final delay
sleep 5s

kill_app
final_log

exit 0
