#!/bin/bash

export LIST=./logs/testslist.txt

./tools/ci/scripts/gettests.py >${LIST} || exit 1

IFS='
'

for str in $(cat ${LIST})
do
    unset IFS
    declare -a "arr=($str)"
    echo ./src/manaplustests --test-case=\"${arr[0]}\" --subcase=\"${arr[1]}\"
    ./src/manaplustests --test-case="${arr[0]}" --subcase="${arr[1]}" || exit 1
done

