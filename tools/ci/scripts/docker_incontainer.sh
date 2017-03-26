#!/bin/bash -x

mkdir logs
echo ${PACKAGES}
.tools/ci/scripts/retry.sh apt-get update
.tools/ci/scripts/retry.sh apt-get install -y -qq ${PACKAGES}
echo ${COMMAND}
${COMMAND}
