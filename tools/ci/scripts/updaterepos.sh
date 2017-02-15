#!/bin/bash

if [[ ${CI_RUNNER_TAGS} != *"docker"* ]]; then
    echo "Running from shell. Skipping update repos"
    exit 0
fi
export DATA=$(cat /etc/resolv.conf|grep "nameserver 1.10.100.101")
echo "${DATA}"
if [ "$DATA" != "" ]; then
    echo "Detected local runner"
    sed -i 's!http://httpredir.debian.org/debian!http://1.10.100.103/debian!' /etc/apt/sources.list || true
fi
