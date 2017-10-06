#!/bin/bash

export LOGFILE2=logs/deheader2.log

export DATA=$(cat ${LOGFILE})
if [[ -z "${DATA}" ]]; then
    echo "Empty output. Probably wrong search flags."
    exit 1
fi

export DATA=$(grep "error:" ${LOGFILE})
if [[ -n "${DATA}" ]]; then
    echo "Compilation error found"
    exit 1
fi

grep "deheader:" ${LOGFILE} \
| grep -v "deheader: remove <climits> from ./src/being/localplayer.cpp" \
| grep -v "deheader: remove <fstream> from ./src/utils/files.cpp" \
| grep -v "deheader: remove <climits> from ./src/resources/map/map.cpp" \
| grep -v "deheader: remove <sys/time.h> from ./src/resources/wallpaper.cpp" \
| grep -v "deheader: remove <linux/tcp.h> from ./src/net/sdltcpnet.cpp" \
| grep -v "deheader: remove <netdb.h> from ./src/net/sdltcpnet.cpp" \
| grep -v "deheader: remove <netinet/in.h> from ./src/net/sdltcpnet.cpp" \
| grep -v "deheader: remove <climits> from ./src/resources/map/map.cpp" \
| grep -v "deheader: remove <vector> from ./src/utils/vector.h" \
| grep -v "has more than one inclusion of \"resources/dye/dyepalette_replace" \
| grep -v "/sdl2gfx/" \
| grep -v "deheader: remove <climits> from ./src/units.cpp" > ${LOGFILE2}

export DATA=$(cat ${LOGFILE2})
if [[ -n "${DATA}" ]]; then
    echo "Wrong include files found"
    echo ${DATA}
    exit 1
fi
