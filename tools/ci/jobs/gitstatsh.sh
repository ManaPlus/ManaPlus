#!/usr/bin/env bash

export LOGFILE=gitstatsh.log

source ./tools/ci/scripts/init.sh

tools/ci/scripts/retry.sh pip install pandas
check_error $?
tools/ci/scripts/retry.sh pip install yattag
check_error $?
tools/ci/scripts/retry.sh git clone https://github.com/gzc/gitstats.git gitstatsh
check_error $?
git log --stat --no-merges --summary --decorate=false >repo_log
check_error $?
cd gitstatsh/v1
python main.py ManaPlus ../../repo_log
check_error $?
mv web ../../stats2
check_error $?
cd ../..
check_error $?
ls -la
