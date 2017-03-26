#!/bin/bash -x

docker pull ${IMAGE}
docker run \
    --volume=$(pwd):/build:rw \
    -e PACKAGES="${PACKAGES}" \
    -e COMMAND="${COMMAND}" \
    ${IMAGE} \
    /bin/sh -x -c "cd /build; pwd; ./tools/ci/scripts/docker_incontainer.sh"
