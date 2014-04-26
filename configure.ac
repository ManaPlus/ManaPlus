AC_PREREQ(2.60)
AC_INIT([ManaPlus], [1.4.4.26], [akaras@inbox.ru], [manaplus])
AM_INIT_AUTOMAKE([1.9])
AC_CONFIG_HEADERS([config.h:config.h.in])
AC_LANG_CPLUSPLUS

# Checks for android.
AC_ARG_ENABLE(androidbuild,
[  --enable-androidbuild    Turn on android building],
[case "${enableval}" in
  yes) androidbuild_enabled=true
 ;;
  no)  androidbuild_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-androidbuild) ;;
esac],[androidbuild_enabled=false])

# Enable nacl build
AC_ARG_ENABLE(naclbuild,
[  --enable-naclbuild    Turn on nacl building],
[case "${enableval}" in
  yes) naclbuild_enabled=true
 ;;
  no)  naclbuild_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-naclbuild) ;;
esac],[naclbuild_enabled=false])

# Checks for programs.
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

if test "x$androidbuild_enabled" == "xfalse"; then
    if test "x$naclbuild_enabled" == "xfalse"; then
        AC_FUNC_MALLOC
        AC_FUNC_REALLOC
    fi
fi

AC_C_INLINE
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_FUNC_FORK
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit floor getcwd gethostbyname memset mkdir select socket])
AC_CHECK_FUNCS([clock_gettime dup2 gettimeofday memchr memmove pow putenv])
AC_CHECK_FUNCS([realpath setenv setlocale sqrt strchr munmap])
#AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([libintl.h limits.h sys/param.h sys/time.h wchar.h])

AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_SSIZE_T

# Checks for internationalization support
AM_GNU_GETTEXT([external],[need-ngettext])
AM_GNU_GETTEXT_VERSION([0.16.1])

# Enable sdl2
AC_ARG_WITH(sdl2,[  --with-sdl2        enable SDL 2 support] )
if test "x$with_sdl2" == "xyes"; then
    AC_PATH_PROG(SDL_CONFIG, sdl2-config)
    if test -n "$SDL_CONFIG"; then
        LIBS="$LIBS `$SDL_CONFIG --libs`"
        CPPFLAGS="$CPPFLAGS `$SDL_CONFIG --cflags`"
    fi
    AC_CHECK_LIB([SDL2], [SDL_Init], ,
    AC_MSG_ERROR([ *** Unable to find SDL2 library (http://www.libsdl.org/)]))

    AC_CHECK_LIB(SDL2_image, IMG_LoadPNG_RW, ,
    AC_MSG_ERROR([ *** Unable to find SDL2_image library with PNG support
    (http://www.libsdl.org/projects/SDL_image/)]))

    AC_CHECK_LIB(SDL2_ttf, TTF_Quit, ,
    AC_MSG_ERROR([ *** Unable to find SDL2_ttf library (http://www.libsdl.org/projects/SDL_ttf/)]))

    AC_CHECK_LIB([SDL2_mixer], [Mix_OpenAudio], ,
    AC_MSG_ERROR([ *** Unable to find SDL2_mixer library (http://www.libsdl.org/projects/SDL_mixer/)]))



    AC_CHECK_LIB(SDL2_net, SDLNet_Init, ,
    AC_MSG_ERROR([ *** Unable to find SDL2_net library]))

    with_sdl2=yes
else
    AC_PATH_PROG(SDL_CONFIG, sdl-config)
    if test -n "$SDL_CONFIG"; then
        LIBS="$LIBS `$SDL_CONFIG --libs`"
        CPPFLAGS="$CPPFLAGS `$SDL_CONFIG --cflags`"
    fi

    if test "x$naclbuild_enabled" == "xfalse"; then
        AC_CHECK_LIB([SDL], [SDL_Init], ,
        AC_MSG_ERROR([ *** Unable to find SDL library (http://www.libsdl.org/)]))


        AC_CHECK_LIB(SDL_image, IMG_LoadPNG_RW, ,
        AC_MSG_ERROR([ *** Unable to find SDL_image library with PNG support
        (http://www.libsdl.org/projects/SDL_image/)]))

        AC_CHECK_LIB(SDL_ttf, TTF_Quit, ,
        AC_MSG_ERROR([ *** Unable to find SDL_ttf library (http://www.libsdl.org/projects/SDL_ttf/)]))

        AC_CHECK_LIB([SDL_mixer], [Mix_OpenAudio], ,
        AC_MSG_ERROR([ *** Unable to find SDL_mixer library (http://www.libsdl.org/projects/SDL_mixer/)]))


        AC_CHECK_LIB(SDL_gfx, rotozoomSurfaceXY, ,
        AC_MSG_ERROR([ *** Unable to find SDL_gfx library (http://www.ferzkopp.net/joomla/software-mainmenu-14/4-ferzkopps-linux-software/19-sdlgfx)]))
        AC_CHECK_HEADERS(SDL_rotozoom.h, ,)

        AC_CHECK_LIB(SDL_net, SDLNet_Init, ,
        AC_MSG_ERROR([ *** Unable to find SDL_net library]))
    fi

    with_sdl2=no
fi

AM_CONDITIONAL(USE_SDL2, test x$with_sdl2 = xyes)

# Option to enable internal sdl-gfx for SDL2 (for now it enabled by default)
AC_ARG_WITH(internalsdlgfx,[  --without-internalsdlgfx        don't use internal sdlgfx (for SDL2 only) ] )
if test "x$with_internalsdlgfx" == "xno"; then
    with_internalsdlgfx=no

    AC_CHECK_LIB(SDL2_gfx, rotozoomSurfaceXY, ,
    AC_MSG_ERROR([ *** Unable to find SDL2_gfx library (http://www.ferzkopp.net/joomla/software-mainmenu-14/4-ferzkopps-linux-software/19-sdlgfx)]))
    AC_CHECK_HEADERS(SDL2_rotozoom.h, ,)
else
    with_internalsdlgfx=yes
    AC_DEFINE(USE_INTERNALSDLGFX, 1, [Defines if ManaPlus should use internal sdlgfx2])
fi
AM_CONDITIONAL(USE_INTERNALSDLGFX, test x$with_internalsdlgfx = xyes)

# Search for *-config
AC_PATH_PROG(PKG_CONFIG, pkg-config)
AC_PATH_PROG(CURL_CONFIG, curl-config)

# Checks for libraries
AC_ARG_WITH(pthread,[  --without-pthread        don't check for pthread ] )
if test "x$with_pthread" == "xno"; then
    without_pthread=yes
else
    if test "x$androidbuild_enabled" == "xfalse"; then
        AC_CHECK_LIB([pthread], [pthread_create], ,
        AC_MSG_ERROR([ *** Unable to find pthread library]))
    fi
    withoud_pthread=no
fi


AC_CHECK_HEADERS([SDL.h], ,
AC_MSG_ERROR([ *** SDL library found but cannot find headers (http://www.libsdl.org/)]))

AC_CHECK_LIB([z], [inflate], ,
AC_MSG_ERROR([ *** Unable to find zlib (http://www.gzip.org/zlib/)]))

AC_CHECK_LIB([physfs], [PHYSFS_init], ,
AC_MSG_ERROR([ *** Unable to find PhysFS library (http://icculus.org/physfs/)]))
AC_CHECK_HEADERS([physfs.h], ,
AC_MSG_ERROR([ *** PhysFS library found but cannot find headers (http://icculus.org/physfs/)]))

if test -n "$CURL_CONFIG"; then
    LIBS="$LIBS `$CURL_CONFIG --libs`"
    CPPFLAGS="$CPPFLAGS `$CURL_CONFIG --cflags`"
fi
AC_CHECK_LIB([curl], [curl_global_init], ,
AC_MSG_ERROR([ *** Unable to find CURL library (http://curl.haxx.se/)]))
AC_CHECK_HEADERS([curl/curl.h], ,
AC_MSG_ERROR([ *** CURL library found but cannot find headers (http://curl.haxx.se/)]))

if test -n "$PKG_CONFIG"; then
    LIBS="$LIBS `$PKG_CONFIG --libs libxml-2.0`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags libxml-2.0`"
fi
AC_CHECK_LIB([xml2], [xmlInitParser], ,
AC_MSG_ERROR([ *** Unable to find libxml2 library (http://xmlsoft.org/)]))
AC_CHECK_HEADERS([libxml/xmlreader.h], ,
AC_MSG_ERROR([ *** libxml2 library found but cannot find headers (http://xmlsoft.org/)]))

AC_CHECK_LIB(png, png_write_info, ,
AC_MSG_ERROR([ *** Unable to find png library]))


# === Check for X11 (check borrowed from Wormux) ========================
# Deactivated on purpose under OSX (in case X11 SDK is installed)
if test "x$OSX" != "xyes" ; then
  AC_CHECK_HEADER(X11/Xlib.h, check_x11="yes",check_x11="no")
  if test x${check_x11} = xno ; then
    AC_CHECK_HEADER(X11R6/Xlib.h,
                    [ check_x11="yes"
                      LDFLAGS="-L/usr/X11R6/include $CFLAGS"],
                    check_x11="no")
  fi
  if test x${check_x11} = xyes ; then
    AC_CHECK_LIB(X11, XOpenDisplay,
                 [ LIBS="$LIBS -lX11"
                   AC_DEFINE(USE_X11, 1, [Define to use X11 copy'n'paste]) ],
                 [])
  fi
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h malloc.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Enable mac build
AC_ARG_ENABLE(applebuild,
[  --enable-applebuild    Turn on apple building],
[case "${enableval}" in
  yes) applebuild_enabled=true
 ;;
  no)  applebuild_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-applebuild) ;;
esac],[applebuild_enabled=false])


# Option to enable OpenGL
AC_ARG_WITH(opengl,[  --without-opengl        don't use OpenGL ] )
if test "x$with_opengl" == "xno"; then
    with_opengl=no
else
    with_opengl=yes
    if test "x$applebuild_enabled" == "xfalse"; then

        if test "x$androidbuild_enabled" == "xfalse"; then
            AC_CHECK_LIB([GL], [glBegin], ,
            AC_MSG_ERROR([ *** Unable to find OpenGL library]))
        else
            LDFLAGS="$LDFLAGS -lGLESv1_CM -lEGL"
        fi
    else
        LDFLAGS="$LDFLAGS -framework OpenGL"
    fi
fi
AM_CONDITIONAL(USE_OPENGL, test x$with_opengl = xyes)

# Option to enable mumble support
AC_ARG_WITH(mumble,[  --without-mumble        don't use mumble integration ] )
if test "x$with_mumble" == "xno"; then
    with_mumble=no
else
    if test "x$androidbuild_enabled" == "xfalse"; then
        with_mumble=yes
    else
        with_mumble=no
    fi
fi
AM_CONDITIONAL(USE_MUMBLE, test x$with_mumble = xyes)


AC_ARG_WITH(librt,[  --without-librt        don't link to librt ] )
if test "x$with_librt" == "xno"; then
    without_librt=yes
else
    if test "x$applebuild_enabled" == "xfalse"; then
        if test "x$androidbuild_enabled" == "xfalse"; then
            AC_CHECK_LIB(rt, shm_open, ,
            AC_MSG_ERROR([ *** Unable to find librt library]))
        fi
    fi
    without_librt=no
fi

# Enable eAthena
AC_ARG_ENABLE(eathena,
[  --enable-eathena    Turn on eAthena support],
[case "${enableval}" in
  yes) with_eathena=true ;;
  no)  with_eathena=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-eathena) ;;
esac],[with_eathena=false])

AM_CONDITIONAL(ENABLE_EATHENA, test x$with_eathena = xtrue)

# Enable checks
AC_ARG_ENABLE(checks,
[  --enable-checks    Turn on internal checks (can be slow)],
[case "${enableval}" in
  yes) with_checks=true ;;
  no)  with_checks=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-checks) ;;
esac],[with_checks=false])

AM_CONDITIONAL(ENABLE_CHECKS, test x$with_checks = xtrue)

# Enable portable
AC_ARG_ENABLE(portable,
[  --enable-portable    Turn on portable mode for linux],
[case "${enableval}" in
  yes) portable_enabled=true ;;
  no)  portable_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-portable) ;;
esac],[portable_enabled=false])

AM_CONDITIONAL(ENABLE_PORTABLE, test x$portable_enabled = xtrue)

# Enable debug
AC_ARG_ENABLE(memdebug,
[  --enable-memdebug    Turn on memory debug mode],
[case "${enableval}" in
  yes) memdebug_enabled=true ;;
  no)  memdebug_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-memdebug) ;;
esac],[memdebug_enabled=false])

AM_CONDITIONAL(ENABLE_MEM_DEBUG, test x$memdebug_enabled = xtrue)

# Enable unit tests
AC_ARG_ENABLE(unittests,
[  --enable-unittests    Turn on unit tests],
[case "${enableval}" in
  yes) unittests_enabled=true
LIBS="$LIBS -lgtest"
 ;;
  no)  unittests_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-tests) ;;
esac],[unittests_enabled=false])

AM_CONDITIONAL(ENABLE_UNITTESTS, test x$unittests_enabled = xtrue)

# Enable tcmalloc
AC_ARG_ENABLE(tcmalloc,
[  --enable-tcmalloc    Turn on tcmalloc],
[case "${enableval}" in
  yes) tcmalloc_enabled=true
LIBS="$LIBS -ltcmalloc"
 ;;
  no)  tcmalloc_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-tcmalloc) ;;
esac],[tcmalloc_enabled=false])

# Enable google profiler
AC_ARG_ENABLE(googleprofiler,
[  --enable-googleprofiler    Turn on google profiler],
[case "${enableval}" in
  yes) googleprofiler_enabled=true
LIBS="$LIBS -lprofiler"
 ;;
  no)  googleprofiler_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-googleprofiler) ;;
esac],[googleprofiler_enabled=false])

AM_CONDITIONAL(ENABLE_GOOGLE_PROFILER, test x$googleprofiler_enabled = xtrue)

AC_CONFIG_FILES([
manaplus.spec
Makefile
PKGBUILD
src/Makefile
data/Makefile
data/evol/evol.desktop
data/evol/Makefile
data/fonts/Makefile
data/graphics/Makefile
data/graphics/flags/Makefile
data/graphics/gui/Makefile
data/graphics/images/Makefile
data/graphics/sprites/Makefile
data/sfx/Makefile
data/sfx/system/Makefile
data/themes/Makefile
data/themes/blacknblack/Makefile
data/themes/blackwood/Makefile
data/themes/classic/Makefile
data/themes/enchilado/Makefile
data/themes/golden-delicious/Makefile
data/themes/jewelry/Makefile
data/themes/mana/Makefile
data/themes/pink/Makefile
data/themes/unity/Makefile
data/themes/wood/Makefile
data/tmw/tmw.desktop
data/tmw/Makefile
data/help/Makefile
data/help/tips/Makefile
data/icons/Makefile
data/perserver/Makefile
data/perserver/default/Makefile
data/translations/Makefile
data/translations/help/Makefile
docs/Makefile
po/Makefile.in
])

AC_OUTPUT

echo
echo "Build with OpenGL: $with_opengl"
echo
echo "configure complete, now type \"make\""
echo
