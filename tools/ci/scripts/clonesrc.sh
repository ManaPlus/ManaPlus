#!/usr/bin/env bash

rm -rf manaplus || true
./tools/ci/scripts/retry.sh "git clone --branch master --depth ${GIT_DEPTH} https://gitlab.com/manaplus/manaplus.git manaplus" || exit 1
mkdir manaplus/logs || exit 1
