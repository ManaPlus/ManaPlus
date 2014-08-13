#!/bin/bash
# Copyright (c) 2012 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# The openssl build can fail when build with -jN.
# TODO(sbc): Remove this if/when openssl is upgraded to a version that supports
# parallel make.
OS_JOBS=1
BUILD_DIR=${SRC_DIR}
INSTALL_TARGETS="install_sw INSTALL_PREFIX=${DESTDIR}"

ConfigureStep() {
  if [ "${NACL_SHARED}" = "1" ] ; then
    local EXTRA_ARGS="shared"
  else
    local EXTRA_ARGS="no-dso"
  fi

  if [ "${NACL_LIBC}" = "newlib" ] ; then
    EXTRA_ARGS+=" -I${NACLPORTS_INCLUDE}/glibc-compat"
    # The default from MACHINE=i686 is linux-elf, which links things
    # with -ldl. However, newlib does not have -ldl. In that case,
    # make a fake machine where the build rule does not use -ldl.
    local machine="le32newlib"
  else
    local machine="i686"
  fi

  MACHINE=${machine} CC=${NACLCC} AR=${NACLAR} RANLIB=${NACLRANLIB} \
    LogExecute ./config \
    --prefix=${PREFIX} no-asm no-hw no-krb5 ${EXTRA_ARGS} -D_GNU_SOURCE

  HackStepForNewlib
}


HackStepForNewlib() {
  if [ "${NACL_SHARED}" = "1" ]; then
    git checkout apps/Makefile
    git checkout test/Makefile
    return
  fi

  # apps/Makefile links programs that require socket(), etc.
  # Stub it out until we link against nacl_io or something.
  echo "all clean install: " > apps/Makefile
  # test/Makefile is similar -- stub out, but keep the original for testing.
  git checkout test/Makefile
  mv test/Makefile test/Makefile.orig
  echo "all clean install: " > test/Makefile
}


BuildStep() {
  LogExecute make clean
  DefaultBuildStep
}


InstallStep() {
  DefaultInstallStep
  # openssl (for some reason) installs shared libraries with 555 (i.e.
  # not writable.  This causes issues when create_nmf copies the libraries
  # and then tries to overwrite them later.
  if [ "${NACL_SHARED}" = "1" ] ; then
    LogExecute chmod 644 ${DESTDIR_LIB}/libssl.so.*
    LogExecute chmod 644 ${DESTDIR_LIB}/libcrypto.so.*
  fi
}


TestStep() {
  return 0
}
