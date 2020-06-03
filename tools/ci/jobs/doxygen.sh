#!/usr/bin/env bash

export LOGFILE=doxygen.log

source ./tools/ci/scripts/init.sh

doxygen tools/doxygen/doxygen.conf
check_error $?
