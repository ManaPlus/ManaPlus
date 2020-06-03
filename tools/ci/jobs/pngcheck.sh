#!/usr/bin/env bash

source ./tools/ci/scripts/init.sh

export LOG1="pngcheck.log"
export LOG2="pngcheck2.log"

rm ${LOG1}
rm ${LOG2}

find -H . -type f -name "*.png" -exec pngcheck {} \; >${LOG1}

grep -v "32-bit RGB+alpha, non-interlaced, " ${LOG1} >${LOG2}
export DATA=$(cat pngcheck2.log)
if [[ -n "${DATA}" ]]; then
    echo "Images must be in 32 bit RGBA and non-interlanced"
    echo "Wrong images format found:"
    cat ${LOG2}
    exit 1
fi
