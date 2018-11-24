#!/bin/bash -x

mkdir logs
echo ${PACKAGES}
./tools/ci/scripts/retry.sh apt-get update
./tools/ci/scripts/retry.sh apt-get install -y -qq ${PACKAGES}
./tools/ci/scripts/patchsdl1.sh || true
echo ${COMMAND}
${COMMAND}
