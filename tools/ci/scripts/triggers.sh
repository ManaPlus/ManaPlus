#!/bin/bash

function run {
    echo "running pipeline for $1"
    curl --request POST --form "token=$BUILD_TRIGGER" --form ref=$1 https://gitlab.com/api/v4/projects/180398/trigger/pipeline || exit 1
}

run "ci_deheader"
run "ci_gcc4x"
sleep 15s
run "ci_gcc49"
run "ci_clang3x"
sleep 15s
run "ci_customnls"
run "ci_clang-tidy"
sleep 15s
run "ci_h"
run "ci_gcc6_tests"
sleep 15s
run "ci_gcc6"
run "ci_malloc"
sleep 15s
run "ci_clang6"
run "ci_gcc5"
sleep 15s
run "ci_clang4_5"
run "ci_xmllibs"
sleep 15s
run "ci_fedora"
run "ci_gcc-snapshot"
sleep 15s
run "ci_mxe"
run "ci_mse"
sleep 15s
run "ci_glibcdebug"
run "ci_checks"
sleep 15s
run "ci_gcc7_tests"
run "ci_gcc3"
sleep 15s
run "ci_site"
