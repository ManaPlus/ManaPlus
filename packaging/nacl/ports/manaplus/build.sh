#!/bin/bash
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

GitCloneStep() {
  if CheckKeyStamp clone "$URL" ; then
    Banner "Skipping git clone step"
    return
  fi

  local GIT_URL=${URL%@*}
  rm -rf ${SRC_DIR}
  mkdir ${SRC_DIR}
  git clone ${GIT_URL} ${SRC_DIR}

  TouchKeyStamp clone "$URL"
}

ConfigureStep() {
  Banner "Configuring ${PACKAGE_NAME}"

  SetupCrossEnvironment
  export PKG_CONFIG_PATH=${NACLPORTS_LIBDIR}/pkgconfig
  export PKG_CONFIG_LIBDIR=${NACLPORTS_LIBDIR}
  export SDL_CONFIG=${NACLPORTS_LIBDIR}/../bin/sdl-config
  SDL_CFLAGS=`$SDL_CONFIG --cflags`
  SDL_LIBS="-lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lSDL_net `$SDL_CONFIG --libs` -lfreetype -ljpeg -lpng16 -lwebp -lvorbisfile -lvorbis -logg -lmikmod -lz -lbz2"
  export LIBS="-L${NACLPORTS_LIBDIR} ${SDL_LIBS} -lnacl_io -lppapi_cpp -lppapi -lpthread -lstdc++ -lRegal -lm -lc"
  autoreconf -i ${SRC_DIR}
  ${SRC_DIR}/configure \
    --enable-naclbuild \
    --disable-nls \
    --without-opengl \
    --without-librt \
    --without-mumble \
    --host=nacl \
    --prefix=${PREFIX}
}

DefaultPackageInstall
exit 0
