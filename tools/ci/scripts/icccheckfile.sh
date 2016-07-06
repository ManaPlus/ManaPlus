#!/bin/bash

identify -verbose $1 | grep profile >/dev/null

if [ "$?" == 0 ]; then
    echo "ICC profile found for image $1"
fi
