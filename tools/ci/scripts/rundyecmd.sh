#!/usr/bin/env bash

function testdye {
    rm test1.png || true
    echo ./src/dyecmd "data/icons/manaplus.png|$1" test1.png
    ./src/dyecmd "data/icons/manaplus.png|$1" test1.png || exit 1
    ls test1.png || exit 1
    rm test1.png || true
}

export SDL_VIDEODRIVER="dummy"
testdye "S:#3c3c3c,40332d,4d4d4d,5e4a3d,686868,705740,919191,a1825d,b6b6b6,b59767,dfdfdf,dbbf88"
testdye "A:#3c3c3c00,40332d50"
testdye "W:#743e66,955372,bd6f8c,e68e9a,f0b49c,efe1c7"
