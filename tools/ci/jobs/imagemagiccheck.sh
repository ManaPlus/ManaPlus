#!/usr/bin/env bash

source ./tools/ci/scripts/init.sh

export LOG="logs/icccheck.log"

mkdir logs
rm ${LOG}

find -H $DIR -type f -name "*.png" -exec ./tools/ci/scripts/icccheckfile.sh {} \; >${LOG}
check_error $?

export RES=$(cat ${LOG})
if [[ -n "${RES}" ]]; then
    echo "Detected icc profiles"
    cat ${LOG}
    exit 1
fi
