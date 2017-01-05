#!/bin/bash

n=0

while true; do
    $*
    if [ "$?" == 0 ]; then
        exit 0
    fi
    if [[ $n -ge 10 ]]; then
        exit -1
    fi
    sleep 5s
    n=$((n+1))
done
