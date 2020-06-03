#!/usr/bin/env bash

export LOGFILE=gitstats.log

source ./tools/ci/scripts/init.sh

tools/ci/scripts/retry.sh git clone https://github.com/hoxu/gitstats.git gitstats
gitstats/gitstats . stats
check_error $?
