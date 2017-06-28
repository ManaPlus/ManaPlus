#!/bin/bash

function check_is_run {
    kill -0 ${PID}
    if [ "$?" != 0 ]; then
        echo "Error: process look like crashed"
        cat logs/run.log
        echo "Run with gdb"
        cp ./src/manaplus ./logs/
        cp -r core* ./logs/
        sleep 10s
        COREFILE=$(find . -maxdepth 1 -name "core*" | head -n 1)
        if [[ -f "$COREFILE" ]]; then
            gdb -c "$COREFILE" ./src/manaplus -ex "thread apply all bt" -ex "set pagination 0" -batch
        fi
        exit 1
    fi
}

function check_assert {
    grep "Assert:" "${HOME}/.local/share/mana/manaplus.log"
    if [ "$?" == 0 ]; then
        echo "Assert found in log"
        exit 1
    fi
}

function wait_for_servers_list {
    n=0
    while true; do
        echo "wait for servers list ${n}"
        check_is_run
        # check here
        grep "Skipping servers list update" "${HOME}/.local/share/mana/manaplus.log" && return
        grep "Servers list updated" "${HOME}/.local/share/mana/manaplus.log" && return
        grep "Error: servers list updating error" "${HOME}/.local/share/mana/manaplus.log"
        if [ "$?" == 0 ]; then
            echo "Servers list downloading error"
            exit 1
        fi

        if [[ $n -ge 150 ]]; then
            break
        fi
        sleep 5s
        n=$((n+1))
    done
    echo "Waiting time for servers list update is up"
    exit 1
}

function run {
    ./src/manaplus --default-cursor --enable-ipc --renderer=0 >logs/run.log 2>&1 &
    export PID=$!
    sleep 20s
    echo "pause after run"
    wait_for_servers_list
    check_assert
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
    check_assert
}

function check_exists {
    if [ ! -f "logs/home/Desktop/ManaPlus/$1" ]; then
        sleep 10s
        if [ ! -f "logs/home/Desktop/ManaPlus/$1" ]; then
            echo "Error: image $1 not exists"
            exit 1
        fi
    fi
    if [ ! -f "logs/home/Desktop/ManaPlus/$2" ]; then
        sleep 10s
        if [ ! -f "logs/home/Desktop/ManaPlus/$2" ]; then
            echo "Error: image $2 not exists"
            exit 1
        fi
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

# set focus to servers window
send_command "/sendmousekey 400 300 1"
send_command "/screenshot center_click.png"
imagesdiff run4.png center_click.png

send_command "/guikey -989 keyGUIHome"
send_command "/screenshot run5.png"
imagesdiff run4.png run5.png

# open add server dialog
send_command "/guikey -990 keyGUIInsert"
send_command "/screenshot add_server_empty.png"
imagesdiff run5.png add_server_empty.png

send_command "/sendchars local"
send_command "/screenshot add_server_name.png"
imagesdiff add_server_empty.png add_server_name.png

send_command "/guikey 9 keyGUITab"
send_command "/screenshot add_server_adress1.png"
imagesdiff add_server_name.png add_server_adress1.png

send_command "/sendchars 127.0.0.1"
send_command "/screenshot add_server_adress2.png"
imagesdiff add_server_adress1.png add_server_adress2.png

send_command "/guikey 9 keyGUITab"
send_command "/screenshot add_server_port.png"
imagesdiff add_server_adress2.png add_server_port.png

send_command "/guikey 9 keyGUITab"
send_command "/screenshot add_server_type.png"
imagesdiff add_server_port.png add_server_type.png

send_command "/guikey 9 keyGUITab"
send_command "/screenshot add_server_description1.png"
imagesdiff add_server_type.png add_server_description1.png

send_command "/sendchars local server"
send_command "/screenshot add_server_description2.png"
imagesdiff add_server_description1.png add_server_description2.png

send_command "/guikey 9 keyGUITab"
send_command "/screenshot add_server_url1.png"
imagesdiff add_server_description2.png add_server_url1.png

send_command "/sendchars http://localhost/"
send_command "/screenshot add_server_url2.png"
imagesdiff add_server_url1.png add_server_url2.png

send_command "/guikey 9 keyGUITab"
send_command "/guikey 9 keyGUITab"
send_command "/guikey 9 keyGUITab"
send_command "/guikey 9 keyGUITab"

# add new server
send_command "/guikey 13 keyGUISelect2"
send_command "/screenshot new_server.png"
imagesdiff add_server_url2.png new_server.png

# other comparisions
imagesdiff downkey.png help.png
imagesdiff help.png setup.png
imagesdiff run5.png add_server_name.png
imagesdiff run5.png add_server_adress1.png
imagesdiff run5.png add_server_adress2.png
imagesdiff run5.png add_server_port.png
imagesdiff run5.png add_server_type.png
imagesdiff run5.png add_server_description1.png
imagesdiff run5.png add_server_description2.png
imagesdiff run5.png add_server_url1.png
imagesdiff run5.png add_server_url2.png
imagesdiff run5.png new_server.png

imagesdiff run4.png add_server_name.png
imagesdiff run4.png add_server_adress1.png
imagesdiff run4.png add_server_adress2.png
imagesdiff run4.png add_server_port.png
imagesdiff run4.png add_server_type.png
imagesdiff run4.png add_server_description1.png
imagesdiff run4.png add_server_description2.png
imagesdiff run4.png add_server_url1.png
imagesdiff run4.png add_server_url2.png
imagesdiff run4.png new_server.png

# final delay
sleep 5s

kill_app
final_log
check_assert

exit 0
