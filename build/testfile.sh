#!/bin/bash

cd ../src

export includes="-I. -I/usr/include -I/usr/include/SDL -I/usr/include/libxml2"
export LOGFILE="../build/testfile.log"

function run {
    export CXX="g++"

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
        -i "\"$name\"" \
        -c "$CXX" \
        -d "$1" \
        -s "-std=gnu++1z" \
        -m "-c -Werror -Wall -Wextra -Wundef -Wmissing-declarations -DLOCALEDIR=\\\"\\\" $2" $3 \
        | grep -v "portability requires" >> ${LOGFILE}

    echo ---------------------------- >>${LOGFILE}
}

echo >${LOGFILE}

run "-DENABLE_LIBXML" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_LIBXML -DENABLE_NLS" "$includes" "$1"
run "-DENABLE_LIBXML -DTMWA_SUPPORT" "$includes" "$1"
run "-DENABLE_LIBXML -DTMWA_SUPPORT -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_LIBXML -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11" "$includes" "$1"
run "-DENABLE_LIBXML -DTMWA_SUPPORT -DUSE_X11" "$includes" "$1"

run "-DENABLE_PUGIXML" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_PUGIXML -DENABLE_NLS" "$includes" "$1"
run "-DENABLE_PUGIXML -DTMWA_SUPPORT" "$includes" "$1"
run "-DENABLE_PUGIXML -DTMWA_SUPPORT -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_PUGIXML -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11" "$includes" "$1"
run "-DENABLE_PUGIXML -DTMWA_SUPPORT -DUSE_X11" "$includes" "$1"

export includes="-I. -I/usr/include -I/usr/include/SDL2 -I/usr/include/libxml2"

run "-DENABLE_LIBXML -DUSE_SDL2" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_SDL2 -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_SDL2 -DENABLE_NLS" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_SDL2 -DTMWA_SUPPORT" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11" "$includes" "$1"
run "-DENABLE_LIBXML -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_X11" "$includes" "$1"

run "-DENABLE_PUGIXML -DUSE_SDL2" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_SDL2 -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_SDL2 -DENABLE_NLS" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_SDL2 -DTMWA_SUPPORT" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_OPENGL -DUSE_X11" "$includes" "$1"
run "-DENABLE_PUGIXML -DUSE_SDL2 -DTMWA_SUPPORT -DUSE_X11" "$includes" "$1"
