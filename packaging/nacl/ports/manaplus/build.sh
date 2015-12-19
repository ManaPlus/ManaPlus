#!/bin/bash
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

ConfigureStep() {
  Banner "Configuring ${PACKAGE_NAME}"

  SetupCrossEnvironment

  # export the nacl tools
  export PKG_CONFIG_PATH=${NACLPORTS_LIBDIR}/pkgconfig
  export PKG_CONFIG_LIBDIR=${NACLPORTS_LIBDIR}

  export SDL_CONFIG=${NACLPORTS_LIBDIR}/../bin/sdl-config
  SDL_CFLAGS=`$SDL_CONFIG --cflags`
  SDL_LIBS="-lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lSDL_net -lfreetype -ljpeg -lpng16 -lwebp -lvorbisfile -lvorbis -logg -lmikmod -lz -lbz2 -ltiff"
  export LIBS="-L${NACLPORTS_LIBDIR} ${SDL_LIBS} -lnacl_io -lpthread -lRegal -lm"
  export MANAPLUSDIR="${START_DIR}/src"

#  autoreconf -i ${SRC_DIR}
  autoreconf -i ${MANAPLUSDIR}
#  ${SRC_DIR}/configure
  ${MANAPLUSDIR}/configure \
    --enable-naclbuild \
    --disable-nls \
    --with-opengl \
    --without-librt \
    --without-mumble \
    --without-dyecmd \
    --host=nacl \
    --prefix=${PREFIX}
}
