#!/bin/bash
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#source pkg_info
#source ../../build_tools/common.sh

ConfigureStep() {
  Banner "Configuring ${PACKAGE_NAME}"

  export SDL_CONFIG=${NACLPORTS_LIBDIR}/../bin/sdl-config
  SDL_CFLAGS=`$SDL_CONFIG --cflags`
  SDL_LIBS=`$SDL_CONFIG --libs`

  # export the nacl tools
  export CC=${NACLCC}
  export CXX=${NACLCXX}
  export AR=${NACLAR}
  export RANLIB=${NACLRANLIB}
  export PKG_CONFIG_PATH=${NACLPORTS_LIBDIR}/pkgconfig
  export PKG_CONFIG_LIBDIR=${NACLPORTS_LIBDIR}
  export CFLAGS="${NACLPORTS_CFLAGS} $SDL_CFLAGS"
  export CXXFLAGS="${NACLPORTS_CXXFLAGS} $SDL_CFLAGS"
  export LDFLAGS=${NACLPORTS_LDFLAGS}
  export PATH=${NACL_BIN_PATH}:${PATH};
  export LIBS="-L${NACLPORTS_LIBDIR} -lppapi -lppapi_gles2 -lppapi_cpp -lnacl_io -lpthread -lstdc++ -lm -lnosys $SDL_LIBS"
  ${SRC_DIR}/autogen.sh
  ${SRC_DIR}/configure \
    --enable-mmx=no \
    --host=nacl \
    --prefix=${PREFIX}
}

PackageInstall
exit 0
