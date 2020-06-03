#!/usr/bin/env bash

export LOGFILE=mxe_gcc6.log

source ./tools/ci/scripts/winvars.sh || exit 1

$CC --version
$CXX --version

autoreconf -i || exit 1

./configure \
--host=${CROSS} \
--enable-unittestsbin \
$* || (cp config.log logs || exit 1)

make -j2 V=0
