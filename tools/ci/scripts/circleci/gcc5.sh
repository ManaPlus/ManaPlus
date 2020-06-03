#!/usr/bin/env bash

echo "CI nodes: ${CIRCLE_NODE_TOTAL}"
echo "Current node: ${CIRCLE_NODE_INDEX}"

case $CIRCLE_NODE_INDEX in
    0)
        export IMAGE=ubuntu:xenial
        export COMMAND="./tools/ci/jobs/gcc5.sh --enable-werror --without-opengl"
        export PACKAGES="gcc-5 g++-5 ${LIBS_PACKAGES} ${SDL1}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    1)
        export IMAGE=i386/ubuntu:xenial
        export COMMAND="./tools/ci/jobs/gcc5.sh --enable-werror --without-opengl"
        export PACKAGES="gcc-5 g++-5 ${LIBS_PACKAGES} ${SDL1}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    2)
        export IMAGE=ubuntu:xenial
        export COMMAND="./tools/ci/jobs/gcc5.sh --enable-werror --without-opengl --with-sdl2"
        export PACKAGES="gcc-5 g++-5 ${LIBS_PACKAGES} ${SDL2}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    3)
        export IMAGE=i386/ubuntu:xenial
        export COMMAND="./tools/ci/jobs/gcc5.sh --enable-werror --without-opengl --with-sdl2"
        export PACKAGES="gcc-5 g++-5 ${LIBS_PACKAGES} ${SDL2}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    *)
        echo unknown node
        exit 1
    ;;
esac
