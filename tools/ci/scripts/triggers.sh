#!/bin/bash

function run {
    echo "running pipeline for $1"
    curl --request POST --form "token=$BUILD_TRIGGER" --form ref=$1 https://gitlab.com/api/v4/projects/180398/trigger/pipeline || exit 1
}

run "ci_deheader"
