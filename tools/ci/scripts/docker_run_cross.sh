#!/bin/bash -x

echo "IMAGE: ${IMAGE}"
echo "PACKAGES: ${PACKAGES}"
echo "CROSS: ${CROSS}"
echo "COMMAND: ${COMMAND}"
echo "JOBS: ${JOBS}"

docker pull ${IMAGE} || exit 1
docker run \
    --volume=$(pwd):/build:rw \
    -e PACKAGES="${PACKAGES}" \
    -e COMMAND="${COMMAND}" \
    -e JOBS="${JOBS}" \
    -e CROSS="${CROSS}" \
    ${IMAGE} \
    /bin/sh -x -c "cd /build; pwd; ./tools/ci/scripts/docker_incontainer.sh"
