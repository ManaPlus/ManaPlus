#!/bin/bash

export path=bot
export server=irc.freenode.net
export C="\0003"
export R="\x0f"

function bot_connect {
    mkdir ${path}
    ii -s $server -i $path -n $1 -f $1 &
    sleep 7s
}

function bot_join {
    echo "/j $1" > "$path/$server/in"
    sleep 7s
}

function bot_message {
    echo -e $2 >bot/irc.freenode.net/${1}/in
    sleep 1s
}

function bot_exit {
    sleep 5s
    killall ii
}
