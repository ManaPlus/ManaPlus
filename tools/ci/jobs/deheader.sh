#!/usr/bin/env bash

export LOGFILE2=logs/deheader2.log

source ./tools/ci/scripts/init.sh

export dir=$(pwd)

rm deheader
tools/ci/scripts/retry.sh wget "https://gitlab.com/4144/deheader/raw/master/deheader"
chmod +x deheader
