#!/bin/bash -x

mkdir logs
echo ${PACKAGES}
apt-get update
apt-get install -y -qq ${PACKAGES}
echo ${COMMAND}
${COMMAND}
