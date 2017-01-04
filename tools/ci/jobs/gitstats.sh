#!/bin/bash

export LOGFILE=gitstats.log

source ./tools/ci/scripts/init.sh

aptget_install python zlib1g git-core gnuplot

tools/ci/scripts/retry.sh git clone https://github.com/hoxu/gitstats.git gitstats
gitstats/gitstats . stats
check_error $?
