#!/usr/bin/env bash

export LIST=./logs/testslist.txt

./tools/ci/scripts/gettests.py >${LIST} || exit 1

IFS='
'

export HOME="logs/home"

for str in $(cat ${LIST})
do
    unset IFS
    declare -a "arr=($str)"
    rm -rf "${HOME}"
    mkdir "${HOME}"
    echo ./src/manaplustests --test-case=\"${arr[0]}\" --subcase=\"${arr[1]}\"
    ./src/manaplustests --test-case="${arr[0]}" --subcase="${arr[1]}" || exit 1
done

