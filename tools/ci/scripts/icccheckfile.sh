#!/usr/bin/env bash

identify -verbose $1 | egrep -i "profile|iCCP" >/dev/null

if [ "$?" == 0 ]; then
    echo "ICC or iCCP profile found for image $1"
fi
