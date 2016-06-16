#!/bin/bash

cd ../src

export includes="-I. -I/usr/include -I/usr/include/SDL -I/usr/include/libxml2"
export LOGFILE="../build/testfile.log"

function run {
    export CXX="g++-5"

    export CMD="$CXX -c $1 $2 $3"
    echo $CMD >>${LOGFILE}
    $CMD 2>>${LOGFILE}

    export CMD="$CXX -c -std=gnu++1z $1 $2 $3"
    $CMD 2>>${LOGFILE}

    ../../deheader/deheader -q -i "\"debug.h\"" \
        -i "\"utils/checkutils.h\"" \
        -i "\"utils/sdlcheckutils.h\"" \
        -i "\"sdlshared.h\"" \
        -i "\"localconsts.h\"" \
        -i "\"utils/fuzzer.h\"" \
        -i "\"utils/physfscheckutils.h\"" \
        -i "\"$name\"" \
        -c "$CXX" \
        -d "$1" \
        -s "-std=gnu++1z" \
        -m "-c -Werror -Wall -Wextra -Wundef -Wmissing-declarations -DLOCALEDIR=\\\"\\\" $2" $3 \
        | grep -v "portability requires" >> ${LOGFILE}

    echo ---------------------------- >>${LOGFILE}
}

echo >${LOGFILE}

run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DENABLE_NLS -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DTMWA_SUPPORT -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DTMWA_SUPPORT -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11 -DDYECMD" "$includes" "$1"

run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DENABLE_NLS -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DTMWA_SUPPORT -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DTMWA_SUPPORT -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11 -DDYECMD" "$includes" "$1"

export includes="-I. -I/usr/include -I/usr/include/SDL2 -I/usr/include/libxml2"

run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_SDL2 -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_SDL2 -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_SDL2 -DENABLE_NLS -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_SDL2 -DTMWA_SUPPORT -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_LIBXML -DEATHENA_SUPPORT -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11 -DDYECMD" "$includes" "$1"

run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_SDL2 -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_SDL2 -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_SDL2 -DENABLE_NLS -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_SDL2 -DTMWA_SUPPORT -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DDYECMD" "$includes" "$1"
run "-DENABLE_PUGIXML -DEATHENA_SUPPORT -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11 -DDYECMD" "$includes" "$1"
