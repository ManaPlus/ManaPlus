#!/bin/bash
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.


if [ "${NACL_SHARED}" = "1" ]; then
  EXECUTABLES="rdjpgcom${NACL_EXEEXT} wrjpgcom${NACL_EXEEXT} \
      .libs/cjpeg${NACL_EXEEXT} .libs/djpeg${NACL_EXEEXT} \
      .libs/jpegtran${NACL_EXEEXT}"
else
  EXECUTABLES="rdjpgcom${NACL_EXEEXT} wrjpgcom${NACL_EXEEXT} \
      cjpeg${NACL_EXEEXT} djpeg${NACL_EXEEXT} jpegtran${NACL_EXEEXT}"
fi

TestStep() {
#  export SEL_LDR_LIB_PATH=$PWD/.libs
#  if [ "${NACL_ARCH}" = "pnacl" ]; then
#    for arch in x86-32 x86-64; do
#      for exe in ${EXECUTABLES}; do
#        local exe_noext=${exe%.*}
#        WriteSelLdrScriptForPNaCl ${exe_noext} ${exe_noext}.${arch}.nexe ${arch}
#      done
#      make test
#    done
#  else
#    if [ "${NACL_SHARED}" = "1" ]; then
#      for exe in ${EXECUTABLES}; do
#       local script=$(basename ${exe%.*})
#        WriteSelLdrScript ${script} ${exe}
#      done
#    fi
#    make test
#  fi
  return 0
}
