#!/bin/bash
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# zlib doesn't support custom build directories so we have
# to build directly in the source dir.
BUILD_DIR=${SRC_DIR}
EXECUTABLES="minigzip${NACL_EXEEXT} example${NACL_EXEEXT}"
if [ "${NACL_SHARED}" = "1" ]; then
  EXECUTABLES+=" libz.so.1"
fi

ConfigureStep() {
  LogExecute rm -f libz.*
  SetupCrossEnvironment
  CHOST=${NACL_CROSS_PREFIX} LogExecute ./configure --prefix=${PREFIX}
}


RunMinigzip() {
#  export LD_LIBRARY_PATH=.
#  if echo "hello world" | ./minigzip | ./minigzip -d; then
#    echo '  *** minigzip test OK ***'
#  else
#    echo '  *** minigzip test FAILED ***'
#    exit 1
#  fi
#  unset LD_LIBRARY_PATH
    return 0
}


RunExample() {
  export LD_LIBRARY_PATH=.
  # This second test does not yet work on nacl (gzopen fails)
  #if ./example; then \
    #echo '  *** zlib test OK ***'; \
  #else \
    #echo '  *** zlib test FAILED ***'; \
    #exit 1
  #fi
  unset LD_LIBRARY_PATH
}


TestStep() {
  if [ "${NACL_LIBC}" = "glibc" ]; then
    # Tests do not currently run on GLIBC due to fdopen() not working
    # TODO(sbc): Remove this once glibc is fixed:
    # https://code.google.com/p/nativeclient/issues/detail?id=3362
    return
  fi

  if [ "${NACL_ARCH}" = "pnacl" ]; then
    local minigzip_pexe="minigzip${NACL_EXEEXT}"
    local example_pexe="example${NACL_EXEEXT}"
    local minigzip_script="minigzip"
    local example_script="example"
    TranslateAndWriteSelLdrScript ${minigzip_pexe} x86-32 \
      minigzip.x86-32.nexe ${minigzip_script}
    RunMinigzip
    TranslateAndWriteSelLdrScript ${minigzip_pexe} x86-64 \
      minigzip.x86-64.nexe ${minigzip_script}
    RunMinigzip
    TranslateAndWriteSelLdrScript ${example_pexe} x86-32 \
      example.x86-32.nexe ${example_script}
    RunExample
    TranslateAndWriteSelLdrScript ${example_pexe} x86-64 \
      example.x86-64.nexe ${example_script}
    RunExample
  else
    RunMinigzip
    RunExample
  fi
}
