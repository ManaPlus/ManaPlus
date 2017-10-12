#!/bin/bash

function run {
    echo "running pipeline for $1"
    curl --request POST --form "token=$BUILD_TRIGGER" --form ref=$1 https://gitlab.com/api/v4/projects/180398/trigger/pipeline || exit 1
}

run "ci_deheader"
sleep 15s
run "ci_gcc4x"
sleep 15s
run "ci_gcc49"
sleep 15s
run "ci_clang3x"
sleep 15s
run "ci_customnls"
sleep 15s
run "ci_clang-tidy"
sleep 15s
run "ci_h"
sleep 15s
run "ci_gcc6_tests"
sleep 15s
run "ci_gcc6"
sleep 15s
run "ci_malloc"
sleep 15s
run "ci_clang6"
sleep 15s
run "ci_gcc5"
sleep 15s
run "ci_clang4_5"
sleep 15s
run "ci_xmllibs"
sleep 15s
run "ci_fedora"
sleep 15s
run "ci_gcc-snapshot"
sleep 15s
run "ci_mxe"
sleep 15s
run "ci_mse"
