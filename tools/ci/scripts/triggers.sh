#!/usr/bin/env bash

function run {
    echo "running pipeline for $1"
    curl --request POST --form "token=$BUILD_TRIGGER" --form ref=$1 https://gitlab.com/api/v4/projects/180398/trigger/pipeline || exit 1
}

run "ci_main1"
run "ci_deheader"
sleep 15s
run "ci_gcc4x"
run "ci_gcc49"
sleep 15s
run "ci_clang3x"
run "ci_customnls"
sleep 15s
run "ci_clang-tidy"
run "ci_h"
sleep 15s
run "ci_gcc6_tests"
run "ci_gcc6"
sleep 15s
run "ci_malloc"
run "ci_clang6"
sleep 15s
run "ci_gcc5"
run "ci_clang4_5"
sleep 15s
run "ci_xmllibs"
run "ci_fedora"
sleep 15s
run "ci_gcc-snapshot"
run "ci_mxe"
sleep 15s
run "ci_mse"
run "ci_glibcdebug"
sleep 15s
run "ci_checks"
run "ci_gcc7_tests"
sleep 15s
run "ci_gcc8_tests"
run "ci_gcc7"
sleep 15s
run "ci_gcc3"
run "ci_gcc10_tests"
sleep 15s
run "ci_gcc8"
run "ci_gcc9_tests"
sleep 15s
run "ci_gcc9"
run "ci_gcc10"
sleep 15s
run "ci_clang7"
sleep 5s
run "ci_site"
