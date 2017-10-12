#!/bin/bash

function run {
    echo "running pipeline for $1"
    curl --request POST --form "token=$BUILD_TRIGGER" --form ref=$1 https://gitlab.com/api/v4/projects/180398/trigger/pipeline || exit 1
}

run "ci_deheader"
run "ci_gcc4x"
run "ci_gcc49"
run "ci_clang3x"
