AC_PREREQ(2.60)
AC_INIT([ManaPlus], [2.1.3.17], [akaras@inbox.ru], [manaplus])
AM_INIT_AUTOMAKE([1.9])
AC_LANG_CPLUSPLUS

AC_DEFUN([AC_CHECK_CXX_FLAG],
    [
        AC_MSG_CHECKING([whether $CC supports $1])
        OLD_CXXFLAGS="$CXXFLAGS"
        CXXFLAGS="$CXXFLAGS -Werror $1"
        $2=no
        AC_COMPILE_IFELSE(
            [AC_LANG_SOURCE([int foo;])],
            [
                AC_MSG_RESULT([yes])
                $2=yes
            ],
            [
                AC_MSG_RESULT([no])
                $2=no
            ]
        )
        CXXFLAGS="$OLD_CXXFLAGS"
    ]
)

AC_DEFUN([AC_CHECK_LD_FLAG],
    [
        AC_MSG_CHECKING([whether $LD supports $1])
        OLD_CXXFLAGS="$CXXFLAGS"
        OLD_LDFLAGS="$LDFLAGS"
        CXXFLAGS=""
        LDFLAGS="$LDFLAGS $1"
        $2=no
        AC_LINK_IFELSE(
            [AC_LANG_SOURCE([
                int main(int argc, char **argv)
                {
                    return 0;
                }
            ])],
            [
                AC_MSG_RESULT([yes])
                $2=yes
            ],
            [
                AC_MSG_RESULT([no])
                $2=no
            ]
        )
        CXXFLAGS="$OLD_CXXFLAGS"
        LDFLAGS="$OLD_LDFLAGS"
    ]
)

AC_DEFUN([AC_CHECK_GLXEXT],
    [
        AC_MSG_CHECKING([whether have glext.h])
        OLD_CXXFLAGS="$CXXFLAGS"
        CXXFLAGS=""
        $1=no
        AC_LINK_IFELSE(
            [AC_LANG_SOURCE([
                #include <GL/gl.h>
                #include <GL/glext.h>
                #ifndef __gl_glext_h_
                #error __gl_glext_h_ not defined
                #endif
                int main(int argc, char **argv)
                {
                    return 0;
                }
            ])],
            [
                AC_MSG_RESULT([yes])
                $1=yes
            ],
            [
                AC_MSG_RESULT([no])
                $1=no
            ]
        )
        CXXFLAGS="$OLD_CXXFLAGS"
    ]
)

# Enable -Werror
AC_ARG_ENABLE(werror,
[  --enable-werror    Fail build if warning present],
[case "${enableval}" in
  yes) werror_enabled=true
 ;;
  no)  werror_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-werror) ;;
esac],[werror_enabled=false])

AM_CONDITIONAL(ENABLE_WERROR, test x$werror_enabled = xtrue)

# Enable asserts
AC_ARG_ENABLE(asserts,
[  --enable-asserts    Enable asserts in log],
[case "${enableval}" in
  yes) asserts_enabled=true
 ;;
  no)  asserts_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-asserts) ;;
esac],[asserts_enabled=true])

AM_CONDITIONAL(ENABLE_ASSERTS, test x$asserts_enabled = xtrue)

# Enable -D_GLIBCXX_DEBUG
AC_ARG_ENABLE(glibcdebug,
[  --enable-glibcdebug    Enable glibc++ debug mode],
[case "${enableval}" in
  yes) glibcdebug_enabled=true
 ;;
  no)  glibcdebug_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-glibcdebug) ;;
esac],[glibcdebug_enabled=false])

AM_CONDITIONAL(ENABLE_GLIBCDEBUG, test x$glibcdebug_enabled = xtrue)

# Enable mse classes replacements for stl
AC_ARG_ENABLE(stldebug,
[  --enable-stldebug    Enable mse libs],
[case "${enableval}" in
  yes) stldebug_enabled=true
 ;;
  no)  stldebug_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-stldebug) ;;
esac],[stldebug_enabled=false])

AM_CONDITIONAL(ENABLE_STLDEBUG, test x$stldebug_enabled = xtrue)

# Enable OpenGL error reporting
AC_ARG_ENABLE(openglerrors,
[  --enable-openglerrors    Enable OpenGL errors reporting],
[case "${enableval}" in
  yes) openglerrors_enabled=true
 ;;
  no)  openglerrors_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-openglerrors) ;;
esac],[openglerrors_enabled=false])

AM_CONDITIONAL(ENABLE_OPENGLERRORS, test x$openglerrors_enabled = xtrue)

# Enable password from command line
AC_ARG_ENABLE(commandlinepassword,
[  --enable-commandlinepassword    Enable command line option for set password],
[case "${enableval}" in
  yes) commandlinepassword_enabled=true
 ;;
  no)  commandlinepassword_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-commandlinepassword) ;;
esac],[commandlinepassword_enabled=false])

AM_CONDITIONAL(ENABLE_COMMANDLINEPASSWORD, test x$commandlinepassword_enabled = xtrue)

# Checks for android.
AC_ARG_ENABLE(androidbuild,
[  --enable-androidbuild    Turn on android building],
[case "${enableval}" in
  yes) androidbuild_enabled=true
       dyecmd_enabled=false
 ;;
  no)  androidbuild_enabled=false
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-androidbuild) ;;
esac],[androidbuild_enabled=false])

# Enable for dyecmd
AC_ARG_WITH(dyecmd,[  --with-dyecmd        build dyecmd tool ] )
if test "x$with_dyecmd" == "xno"; then
    with_dyecmd=no
else
    if test "x$androidbuild_enabled" == "xfalse"; then
        with_dyecmd=yes
    else
        with_dyecmd=no
    fi
fi

AM_CONDITIONAL(ENABLE_DYECMD, test x$with_dyecmd = xyes)

# Option to enable ManaPlus game
AC_ARG_WITH(manaplusgame,[  --without-manaplusgame        don't build ManaPlus game ] )
if test "x$with_manaplusgame" == "xno"; then
    with_manaplusgame=no
else
    with_manaplusgame=yes
fi
AM_CONDITIONAL(ENABLE_MANAPLUSGAME, test x$with_manaplusgame = xyes)

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

# disabled because look like build issue with some libc versions with asan
#if test "x$androidbuild_enabled" == "xfalse"; then
#    if test "x$naclbuild_enabled" == "xfalse"; then
#        AC_FUNC_MALLOC
#        AC_FUNC_REALLOC
#    fi
#fi

AC_C_INLINE
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_FUNC_FORK
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit floor getcwd gethostbyname memset mkdir select socket])
AC_CHECK_FUNCS([clock_gettime dup2 gettimeofday memchr memmove pow putenv])
AC_CHECK_FUNCS([realpath setenv setlocale sqrt strchr munmap])
AC_CHECK_FUNCS([malloc_trim])
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

# Search for *-config
PKG_PROG_PKG_CONFIG
AC_PATH_PROG(CURL_CONFIG, curl-config)
AC_PATH_PROG(ICU_CONFIG, icu-config)

LIBS="$LIBS `$PKG_CONFIG --libs zlib`"
CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags zlib`"
LIBS="$LIBS `$PKG_CONFIG --libs libpng`"
CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags libpng`"

# Checks for internationalization support
AM_GNU_GETTEXT([external],[need-ngettext])
AM_GNU_GETTEXT_VERSION([0.16.1])

case $host in 
  (*mingw*)
    skip_check_lib="yes"
    mingw=true
    LIBS="$LIBS -lintl"
    ;;
  (*)
    skip_check_lib="no"
    mingw=false
    ;;
esac

AM_CONDITIONAL(MINGW, test x$mingw = xtrue)


# Option to enable internal sdl-gfx for SDL2 (for now it enabled by default)
AC_ARG_WITH(internalsdlgfx,[  --without-internalsdlgfx        don't use internal sdlgfx (for SDL2 only) ] )

AC_DEFUN([AC_CHECK_SDL],
[
# Enable sdl2
AC_ARG_WITH(sdl2,[  --with-sdl2        enable SDL 2 support] )
if test "x$with_sdl2" == "xyes"; then
    AC_PATH_PROG(SDL_CONFIG, sdl2-config)
    if test -n "$SDL_CONFIG"; then
        LIBS="$LIBS `$SDL_CONFIG --libs`"
        CPPFLAGS="$CPPFLAGS `$SDL_CONFIG --cflags`"
    fi
    if test "x$with_internalsdlgfx" == "xno"; then
        LIBS="$LIBS `$PKG_CONFIG --libs SDL2_gfx`"
        CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL2_gfx`"
    fi
    LIBS="$LIBS `$PKG_CONFIG --libs SDL2_image`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL2_image`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL2_mixer`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL2_mixer`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL2_net`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL2_net`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL2_ttf`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL2_ttf`"

    if test "$skip_check_lib" == "no"; then
        AC_CHECK_LIB([SDL2], [SDL_Init], ,
        AC_MSG_ERROR([ *** Unable to find SDL2 library (http://www.libsdl.org/)]))

        AC_CHECK_LIB(SDL2_image, IMG_LoadPNG_RW, ,
        AC_MSG_ERROR([ *** Unable to find SDL2_image library with PNG support (http://www.libsdl.org/projects/SDL_image/)]))

        AC_CHECK_LIB(SDL2_ttf, TTF_Quit, ,
        AC_MSG_ERROR([ *** Unable to find SDL2_ttf library (http://www.libsdl.org/projects/SDL_ttf/)]))

        AC_CHECK_LIB([SDL2_mixer], [Mix_OpenAudio], ,
        AC_MSG_ERROR([ *** Unable to find SDL2_mixer library (http://www.libsdl.org/projects/SDL_mixer/)]))

        AC_CHECK_LIB(SDL2_net, SDLNet_Init, ,
        AC_MSG_ERROR([ *** Unable to find SDL2_net library]))
    fi
    with_sdl2=yes
else
    AC_PATH_PROG(SDL_CONFIG, sdl-config)
    if test -n "$SDL_CONFIG"; then
        LIBS="$LIBS `$SDL_CONFIG --libs`"
        CPPFLAGS="$CPPFLAGS `$SDL_CONFIG --cflags`"
    fi
    LIBS="$LIBS `$PKG_CONFIG --libs SDL_gfx`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL_gfx`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL_image`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL_image`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL_mixer`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL_mixer`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL_net`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL_net`"
    LIBS="$LIBS `$PKG_CONFIG --libs SDL_ttf`"
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags SDL_ttf`"

    if test "x$naclbuild_enabled" == "xfalse"; then
        if test "$skip_check_lib" == "no"; then
            AC_CHECK_LIB([SDL], [SDL_Init], ,
            AC_MSG_ERROR([ *** Unable to find SDL library (http://www.libsdl.org/)]))

            AC_CHECK_LIB(SDL_image, IMG_LoadPNG_RW, ,
            AC_MSG_ERROR([ *** Unable to find SDL_image library with PNG support (http://www.libsdl.org/projects/SDL_image/)]))

            AC_CHECK_LIB(SDL_ttf, TTF_Quit, ,
            AC_MSG_ERROR([ *** Unable to find SDL_ttf library (http://www.libsdl.org/projects/SDL_ttf/)]))

            AC_CHECK_LIB([SDL_mixer], [Mix_OpenAudio], ,
            AC_MSG_ERROR([ *** Unable to find SDL_mixer library (http://www.libsdl.org/projects/SDL_mixer/)]))

            AC_CHECK_LIB(SDL_gfx, rotozoomSurfaceXY, ,
            AC_MSG_ERROR([ *** Unable to find SDL_gfx library (http://www.ferzkopp.net/joomla/software-mainmenu-14/4-ferzkopps-linux-software/19-sdlgfx)]))

            AC_CHECK_LIB(SDL_net, SDLNet_Init, ,
            AC_MSG_ERROR([ *** Unable to find SDL_net library]))
        fi
        AC_CHECK_HEADERS(SDL_rotozoom.h, ,)
    fi

    with_sdl2=no
fi

AM_CONDITIONAL(USE_SDL2, test x$with_sdl2 = xyes)

AC_CHECK_GLXEXT(have_glext)
AM_CONDITIONAL(HAVE_GLEXT, test x$have_glext = xyes)

# Option to enable internal sdl-gfx for SDL2 (for now it enabled by default)
if test "x$with_internalsdlgfx" == "xno"; then
    with_internalsdlgfx=no

    if test "$skip_check_lib" == "no"; then
        AC_CHECK_LIB(SDL2_gfx, rotozoomSurfaceXY, ,
        AC_MSG_ERROR([ *** Unable to find SDL2_gfx library (http://www.ferzkopp.net/joomla/software-mainmenu-14/4-ferzkopps-linux-software/19-sdlgfx)]))
    fi
    AC_CHECK_HEADERS(SDL2_rotozoom.h, ,)
else
    with_internalsdlgfx=yes
    AC_DEFINE(USE_INTERNALSDLGFX, 1, [Defines if ManaPlus should use internal sdlgfx2])
fi
AM_CONDITIONAL(USE_INTERNALSDLGFX, test x$with_internalsdlgfx = xyes)

AC_CHECK_HEADERS([SDL.h], ,
AC_MSG_ERROR([ *** SDL library found but cannot find headers (http://www.libsdl.org/)]))
])

if test "x$naclbuild_enabled" == "xfalse"; then
    AC_CHECK_SDL()
fi

# Checks for libraries
AC_ARG_WITH(pthread,[  --without-pthread        don't check for pthread ] )
if test "x$with_pthread" == "xno"; then
    without_pthread=yes
else
    if test "x$androidbuild_enabled" == "xfalse"; then
        if test "$skip_check_lib" == "no"; then
            AC_CHECK_LIB([pthread], [pthread_create], ,
            AC_MSG_ERROR([ *** Unable to find pthread library]))
        fi
    fi
    withoud_pthread=no
fi

if test "$skip_check_lib" == "no"; then
    AC_CHECK_LIB([z], [inflate], ,
    AC_MSG_ERROR([ *** Unable to find zlib (http://www.gzip.org/zlib/)]))
fi

if test -n "$CURL_CONFIG"; then
    LIBS="$LIBS `$CURL_CONFIG --libs`"
    CPPFLAGS="$CPPFLAGS `$CURL_CONFIG --cflags`"
fi
if test "$skip_check_lib" == "no"; then
    AC_CHECK_LIB([curl], [curl_global_init], ,
    AC_MSG_ERROR([ *** Unable to find CURL library (http://curl.haxx.se/)]))
fi
AC_CHECK_HEADERS([curl/curl.h], ,
AC_MSG_ERROR([ *** CURL library found but cannot find headers (http://curl.haxx.se/)]))


# select xml lib
AC_ARG_ENABLE(libxml,
    AC_HELP_STRING([--enable-libxml=ARG],
        [xml libs: libxml (default), pugixml (experimental)]),
    [
        xmllib="${enableval}"
    ],
    [
        xmllib="libxml"
    ]
)

case $xmllib in
    "libxml")
        AM_CONDITIONAL(ENABLE_LIBXML, true)
        AM_CONDITIONAL(ENABLE_PUGIXML, false)
        AM_CONDITIONAL(ENABLE_TINYXML2, false)
        ;;
    "pugixml")
        AM_CONDITIONAL(ENABLE_LIBXML, false)
        AM_CONDITIONAL(ENABLE_PUGIXML, true)
        AM_CONDITIONAL(ENABLE_TINYXML2, false)
        ;;
    "tinyxml2")
        AM_CONDITIONAL(ENABLE_LIBXML, false)
        AM_CONDITIONAL(ENABLE_PUGIXML, false)
        AM_CONDITIONAL(ENABLE_TINYXML2, true)
        ;;
    *)
        AC_MSG_ERROR([[Wrong xml lib name]])
        ;;
esac

if test "$xmllib" == "libxml"; then
    if test -n "$PKG_CONFIG"; then
        LIBS="$LIBS `$PKG_CONFIG --libs libxml-2.0`"
        CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags libxml-2.0`"
    fi
    if test "$skip_check_lib" == "no"; then
        AC_CHECK_LIB([xml2], [xmlInitParser], ,
        AC_MSG_ERROR([ *** Unable to find libxml2 library (http://xmlsoft.org/)]))
    fi
    AC_CHECK_HEADERS(
        [libxml/xmlreader.h],
        ,
        # workaround for icu-59 issue.
        CPPFLAGS="$CPPFLAGS --std=c++0x"
        AC_MSG_RESULT([no])
        AC_MSG_CHECKING([Checking libxml2 headers with icu bug fix])
        icu_bug=yes
        $as_unset ac_cv_header_libxml_xmlreader_h
    )
    if test "$icu_bug" == "yes"; then
        AC_CHECK_HEADERS(
            [libxml/xmlreader.h],
            ,
            AC_MSG_ERROR([ *** libxml2 library found but cannot find headers (http://xmlsoft.org/)])
        )
    fi
fi

if test "$xmllib" == "pugixml"; then
    if test "$skip_check_lib" == "no"; then
        AC_CHECK_LIB([pugixml], [main], ,
        AC_MSG_ERROR([ *** Unable to find pugixml library (http://pugixml.org/)]))
    fi
    AC_CHECK_HEADERS([pugixml.hpp], ,
    AC_MSG_ERROR([ *** pugixml library found but cannot find headers (http://pugixml.org/)]))
fi

# hack for support unversioned api change in tinyxml2 (master branch)
tinyxml2_old=yes
if test "$xmllib" == "tinyxml2"; then
    if test "$skip_check_lib" == "no"; then
        AC_CHECK_LIB([tinyxml2], [main], ,
        AC_MSG_ERROR([ *** Unable to find tinyxml2 library (http://grinninglizard.com/tinyxml2/)]))
        AC_MSG_CHECKING([whether tinyxml supports old error api])
        AC_COMPILE_IFELSE(
            [AC_LANG_SOURCE([
                #ifndef GCC_VERSION
                #define GCC_VERSION (__GNUC__ * 10000 \
                    + __GNUC_MINOR__ * 100 \
                    + __GNUC_PATCHLEVEL__)
                #endif  // GCC_VERSION
                #ifdef __clang__
                #ifndef CLANG_VERSION
                #define CLANG_VERSION (__clang_major__ * 10000 \
                    + __clang_minor__ * 100 \
                    + __clang_patchlevel__)
                #endif  // CLANG_VERSION
                #endif  // __clang__
                #if GCC_VERSION >= 49000
                #define PRAGMA49(str) _Pragma(#str)
                #else  // GCC_VERSION > 49000
                #define PRAGMA49(str)
                #endif  // GCC_VERSION > 49000
                #if GCC_VERSION >= 40600
                #define PRAGMACLANG6GCC(str) _Pragma(#str)
                #elif defined(__clang__) && CLANG_VERSION >= 30800
                #define PRAGMACLANG6GCC(str) _Pragma(#str)
                #else  // __clang__
                #define PRAGMACLANG6GCC(str)
                #endif  // __clang__

                PRAGMA49(GCC diagnostic push)
                PRAGMA49(GCC diagnostic ignored "-Wzero-as-null-pointer-constant")
                PRAGMA49(GCC diagnostic ignored "-Wsuggest-override")
                PRAGMACLANG6GCC(GCC diagnostic push)
                PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
                #include <tinyxml2.h>
                PRAGMACLANG6GCC(GCC diagnostic pop)
                PRAGMA49(GCC diagnostic pop)

                int main(int, char **)
                {
                    tinyxml2::XMLDocument doc;
                    const char *str = doc.ErrorStr();
                    return !str;
                }
            ])],
            [
                AC_MSG_RESULT([no])
                tinyxml2_old=no
            ],
            [
                AC_MSG_RESULT([yes])
                tinyxml2_old=yes
            ]
        )
    fi
    AC_CHECK_HEADERS([tinyxml2.h], ,
    AC_MSG_ERROR([ *** tinyxml2 library found but cannot find headers (http://grinninglizard.com/tinyxml2/)]))
fi

AM_CONDITIONAL(USE_TINYXML_OLD, test x$tinyxml2_old = xyes)

if test "$skip_check_lib" == "no"; then
    AC_CHECK_LIB(png, png_write_info, ,
    AC_MSG_ERROR([ *** Unable to find png library]))
fi

use_x11=no
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
                   AC_DEFINE(USE_X11, 1, [Define to use X11 copy'n'paste])
                   use_x11=yes
                 ],
                 [])
  fi
fi

AM_CONDITIONAL(USE_X11, test x$use_x11 = xyes)

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


# Option to enable gcov
AC_ARG_WITH(gcov,[  --with-gcov        use gcov ] )
if test "x$with_gcov" == "xyes"; then
    with_gcov=yes
    CPPFLAGS="$CPPFLAGS -coverage"
    LDFLAGS="$LDFLAGS -coverage"
else
    with_gcov=no
fi
AM_CONDITIONAL(USE_GCOV, test x$with_gcov = xyes)

# Option to enable OpenGL
AC_ARG_WITH(opengl,[  --without-opengl        don't use OpenGL ] )
if test "x$with_opengl" == "xno"; then
    with_opengl=no
else
    with_opengl=yes
    if test "x$applebuild_enabled" == "xfalse"; then
        if test "x$androidbuild_enabled" == "xfalse"; then
            if test "x$naclbuild_enabled" == "xfalse"; then
                if test "$skip_check_lib" == "no"; then
                    AC_CHECK_LIB([GL], [glBegin], ,
                    AC_MSG_ERROR([ *** Unable to find OpenGL library]))
                fi
            fi
        else
            LDFLAGS="$LDFLAGS -lGLESv1_CM -lEGL"
        fi
    else
        LDFLAGS="$LDFLAGS -framework OpenGL"
    fi
case $host in 
  (*mingw*) LIBS="$LIBS `$PKG_CONFIG --libs gl`"
esac

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
            AC_CHECK_LD_FLAG(-lrt, use_librt)
            if test "x$use_librt" == "xyes"; then
                AC_CHECK_LIB(rt, shm_open, ,
                AC_MSG_ERROR([ *** Unable to find librt library]))
            fi
        fi
    fi
    without_librt=no
fi

AM_CONDITIONAL(USE_LIBRT, test x$use_librt = xyes)

# Enable tmwA
AC_ARG_ENABLE(tmwa,
[  --enable-tmwa    Turn on tmwA support],
[case "${enableval}" in
  yes) with_tmwa=true ;;
  no)  with_tmwa=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-tmwa) ;;
esac],[with_tmwa=true])

AM_CONDITIONAL(ENABLE_TMWA, test x$with_tmwa = xtrue)

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
  yes)
       unittests_enabled=true
       unittests_catch=true
       unittests_doctest=false
       unittests_embed=true
 ;;
  catch)
       unittests_enabled=true
       unittests_catch=true
       unittests_doctest=false
       unittests_embed=true
 ;;
  doctest)
       unittests_enabled=true
       unittests_catch=false
       unittests_doctest=true
       unittests_embed=true
 ;;
  systemcatch)
       unittests_enabled=true
       unittests_catch=true
       unittests_doctest=false
       unittests_embed=false
 ;;
  systemdoctest)
       unittests_enabled=true
       unittests_catch=false
       unittests_doctest=true
       unittests_embed=false
 ;;
  no)
       unittests_enabled=false
       unittests_embed=true
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-unittests) ;;
esac],[unittests_enabled=false])

AM_CONDITIONAL(ENABLE_UNITTESTS, test x$unittests_enabled = xtrue)
AM_CONDITIONAL(ENABLE_UNITTESTS_CATCH, test x$unittests_catch = xtrue)
AM_CONDITIONAL(ENABLE_UNITTESTS_DOCTEST, test x$unittests_doctest = xtrue)
AM_CONDITIONAL(ENABLE_UNITTESTS_EMBED, test x$unittests_embed = xtrue)

# Enable unit tests binaries only
AC_ARG_ENABLE(unittestsbin,
[  --enable-unittestsbin    Turn on unit tests binary compilation only],
[case "${enableval}" in
  yes)
    unittestsbin_enabled=true
    unittestsbin_catch=true
    unittestsbin_doctest=false
    unittestsbin_embed=true
 ;;
  catch)
    unittestsbin_enabled=true
    unittestsbin_catch=true
    unittestsbin_doctest=false
    unittestsbin_embed=true
 ;;
  doctest)
    unittestsbin_enabled=true
    unittestsbin_catch=false
    unittestsbin_doctest=true
    unittestsbin_embed=true
 ;;
  systemcatch)
    unittestsbin_enabled=true
    unittestsbin_catch=true
    unittestsbin_doctest=false
    unittestsbin_embed=false
 ;;
  systemdoctest)
    unittestsbin_enabled=true
    unittestsbin_catch=false
    unittestsbin_doctest=true
    unittestsbin_embed=false
 ;;
  no)
    unittestsbin_enabled=false
    unittestsbin_embed=true
 ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-unittestsbin) ;;
esac],[unittestsbin_enabled=false])

AM_CONDITIONAL(ENABLE_UNITTESTSBIN, test x$unittestsbin_enabled = xtrue)
AM_CONDITIONAL(ENABLE_UNITTESTSBIN_CATCH, test x$unittestsbin_catch = xtrue)
AM_CONDITIONAL(ENABLE_UNITTESTSBIN_DOCTEST, test x$unittestsbin_doctest = xtrue)
AM_CONDITIONAL(ENABLE_UNITTESTSBIN_EMBED, test x$unittestsbin_embed = xtrue)

# Override home directory for unit tests
AC_ARG_WITH(unittestsdir,
[  --with-unittestsdir        override home directory for unit tests ],
[
    if test "x$withval" != "xno"; then
        with_unittestsdir=$withval
        CXXFLAGS="$CXXFLAGS -DUNITESTSDIR=\\\"$withval\\\""
    fi
])

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

# Enable gcc check plugin
AC_ARG_ENABLE(checkplugin,
[  --enable-checkplugin    Turn on gcc check plugin],
[case "${enableval}" in
  yes) checkplugin_enabled=true ;;
  no)  checkplugin_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-checkplugin) ;;
esac],[checkplugin_enabled=false])

AM_CONDITIONAL(ENABLE_CHECKPLUGIN, test x$checkplugin_enabled = xtrue)

# Enable custom NLS
AC_ARG_ENABLE(customnls,
[  --enable-customnls    Turn on build in translation system (NLS)],
[case "${enableval}" in
  yes) customnls_enabled=true ;;
  no)  customnls_enabled=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-customnls) ;;
esac],[customnls_enabled=false])

AM_CONDITIONAL(ENABLE_CUSTOMNLS, test x$customnls_enabled = xtrue)

if test "x$naclbuild_enabled" == "xtrue"; then
    AC_CHECK_SDL()
fi

AC_CHECK_LD_FLAG(-rdynamic, have_rdynamic)
AM_CONDITIONAL(HAVE_RDYNAMIC, test x$have_rdynamic = xyes)

AC_CHECK_HEADERS([execinfo.h],
    [
        AC_SEARCH_LIBS([backtrace],
            [execinfo],
            [
                have_execinfo=yes
            ],
            [
                have_execinfo=no
            ]
        )
    ],
    []
)
AM_CONDITIONAL(HAVE_EXECINFO, test x$have_execinfo = xyes)

AC_CONFIG_FILES([
manaplus.spec
Makefile
PKGBUILD
src/Makefile
data/Makefile
data/evol/evol.desktop
data/evol/Makefile
data/fonts/Makefile
data/fonts/src/Makefile
data/graphics/Makefile
data/graphics/badges/Makefile
data/graphics/flags/Makefile
data/graphics/gui/Makefile
data/graphics/images/Makefile
data/graphics/shaders/Makefile
data/graphics/sprites/Makefile
data/sfx/Makefile
data/sfx/system/Makefile
data/test/Makefile
data/test/dir1/Makefile
data/test/dir2/Makefile
data/themes/Makefile
data/themes/blacknblack/Makefile
data/themes/blackwood/Makefile
data/themes/classic/Makefile
data/themes/enchilado/Makefile
data/themes/golden-delicious/Makefile
data/themes/jewelry/Makefile
data/themes/jewelry-simple/Makefile
data/themes/mana/Makefile
data/themes/pink/Makefile
data/themes/unity/Makefile
data/themes/wood/Makefile
data/help/Makefile
data/help/idx/Makefile
data/help/tips/Makefile
data/icons/Makefile
data/music/Makefile
data/perserver/Makefile
data/perserver/default/Makefile
data/translations/Makefile
data/translations/help/Makefile
data/translations/test/Makefile
docs/Makefile
po/Makefile.in
])

AC_CONFIG_FILES([data/evol/evol], [chmod +x data/evol/evol])

AC_OUTPUT

echo
if test "$with_manaplusgame" == "yes"; then
    echo "Enabled building manaplus game."
fi
if test "$with_dyecmd" == "yes"; then
    echo "Enabled building dyecmd."
fi
if test "$unittests_enabled" == true; then
    echo "Enabled building unit tests."
fi

echo "Build with OpenGL: $with_opengl"
echo
echo "LIBS: $LIBS"
echo "CPPFLAGS: $CPPFLAGS"

echo
echo "configure complete, now type \"make\""
echo
