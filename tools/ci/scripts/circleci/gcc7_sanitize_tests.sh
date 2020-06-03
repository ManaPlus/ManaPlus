#!/usr/bin/env bash

echo "CI nodes: ${CIRCLE_NODE_TOTAL}"
echo "Current node: ${CIRCLE_NODE_INDEX}"

case $CIRCLE_NODE_INDEX in
    0)
        echo nothing
    ;;
    1)
        export IMAGE=debian:buster
        export COMMAND="./tools/ci/jobs/gcc7_sanitize_tests.sh --enable-unittests=yes --without-dyecmd --without-manaplusgame --without-opengl"
        export PACKAGES="gcc-7 g++-7 ${LIBS_PACKAGES} ${SDL1} valgrind"
        ./tools/ci/scripts/docker_run.sh
    ;;
    2)
        echo nothing
    ;;
    3)
        export IMAGE=debian:buster
        export COMMAND="./tools/ci/jobs/gcc7_sanitize_tests.sh --enable-unittests=yes --without-dyecmd --without-manaplusgame --without-opengl --with-sdl2"
        export PACKAGES="gcc-7 g++-7 ${LIBS_PACKAGES} ${SDL2} valgrind"
        ./tools/ci/scripts/docker_run.sh
    ;;
    *)
        echo unknown node
        exit 1
    ;;
esac
