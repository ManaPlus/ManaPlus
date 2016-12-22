/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAIN_H
#define MAIN_H

/**
 * \mainpage
 *
 * \section General General information
 *
 * During the game, the current Map is displayed by the main Viewport, which
 * is the bottom-most widget in the WindowContainer. Aside the viewport, the
 * window container keeps track of all the \link Window Windows\endlink
 * displayed during the game. It is the <i>top</i> widget for Guichan.
 *
 * A Map is composed of several layers of \link Image Images\endlink (tiles),
 * a layer with collision information and \link Sprite Sprites\endlink. The
 * sprites define the visible part of \link Being Beings\endlink and
 * \link FloorItem FloorItems\endlink, they are drawn from top to bottom
 * by the map, interleaved with the tiles in the fringe layer.
 *
 * The client supports two servers, \link EAthena eAthena\endlink (the TMW
 * version) and the \link EvolServ server\endlink. To achieve this, the
 * \link Net network communication layer\endlink is abstracted in many
 * different interfaces, which have different implementations for each server.
 */

#define SMALL_VERSION "1.6.11.5"
#define CHECK_VERSION "01.06.11.05"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#elif defined WIN32
#include "winver.h"
#else  // HAVE_CONFIG_H
#ifdef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif  // PACKAGE_VERSION
#define PACKAGE_VERSION SMALL_VERSION;
#endif  // HAVE_CONFIG_H

#include "localconsts.h"

#if defined __APPLE__
#define PACKAGE_OS "Mac OS X"
#elif defined(__ANDROID__) || defined(ANDROID)
#define PACKAGE_OS "Android"
#elif defined __FreeBSD__ || defined __DragonFly__
#define PACKAGE_OS "FreeBSD"
#elif defined(__NetBSD__) || defined(__NetBSD)
#define PACKAGE_OS "NetBSD"
#elif defined(__OpenBSD__) || defined(__OPENBSD)
#define PACKAGE_OS "OpenBSD"
#elif defined __linux__ || defined __linux
#define PACKAGE_OS "Linux"
#elif defined __CYGWIN__
#define PACKAGE_OS "Cygwin"
#elif defined(__sun) || defined(sun)
#define PACKAGE_OS "SunOS"
#elif defined(_AIX) || defined(__AIX) || defined(__AIX__) \
    || defined(__aix) || defined(__aix__)
#define PACKAGE_OS "AIX"
#elif defined(__sgi) || defined(__sgi__) || defined(_SGI)
#define PACKAGE_OS "IRIX"
#elif defined(__hpux) || defined(__hpux__)
#define PACKAGE_OS "HP-UX"
#elif defined(__BeOS) || defined(__BEOS__) || defined(_BEOS)
#define PACKAGE_OS "BeOS"
#elif defined(__QNX__) || defined(__QNXNTO__)
#define PACKAGE_OS "QNX"
#elif defined(__tru64) || defined(_tru64) || defined(__TRU64__)
#define PACKAGE_OS "Tru64"
#elif defined(__riscos) || defined(__riscos__)
#define PACKAGE_OS "RISCos"
#elif defined(__sinix) || defined(__sinix__) || defined(__SINIX__)
#define PACKAGE_OS "SINIX"
#elif defined(__UNIX_SV__)
#define PACKAGE_OS "UNIX_SV"
#elif defined(__bsdos__)
#define PACKAGE_OS "BSDOS"
#elif defined(_MPRAS) || defined(MPRAS)
#define PACKAGE_OS "MP-RAS"
#elif defined(__osf) || defined(__osf__)
#define PACKAGE_OS "OSF1"
#elif defined(_SCO_SV) || defined(SCO_SV) || defined(sco_sv)
#define PACKAGE_OS "SCO_SV"
#elif defined(__ultrix) || defined(__ultrix__) || defined(_ULTRIX)
#define PACKAGE_OS "ULTRIX"
#elif defined(__XENIX__) || defined(_XENIX) || defined(XENIX)
#define PACKAGE_OS "Xenix"
#elif defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
#define PACKAGE_OS "BSDI"
#elif defined(_arch_dreamcast)
#define PACKAGE_OS "Dreamcast"
#elif defined(__OS2__)
#define PACKAGE_OS "OS/2"
#elif defined(osf) || defined(__osf) || defined(__osf__)
#define PACKAGE_OS "OSF"
#elif defined(riscos) || defined(__riscos) || defined(__riscos__)
#define PACKAGE_OS "RISC OS"
#elif defined(__SVR4)
#define PACKAGE_OS "Solaris"
#elif defined __GNU__
#define PACKAGE_OS "GNU Hurd"
#elif defined __HAIKU__
#define PACKAGE_OS "Haiku"
#elif defined WIN32 || defined _WIN32 || defined __WIN32__ || defined __NT__ \
    || defined WIN64 || defined _WIN64 || defined __WIN64__ \
    || defined __MINGW32__ || defined _MSC_VER
#define PACKAGE_OS "Windows"
#elif defined __native_client__
#define PACKAGE_OS "nacl"
#else  // OTHER
#define PACKAGE_OS "Other"
#endif  // defined __APPLE__

// define DEBUG_LEAKS 1
// define DEBUG_CONFIG 1
// define DEBUG_FONT 1
// define DEBUG_FONT_COUNTERS 1
// define DEBUG_ALPHA_CACHE 1
// define DEBUG_OPENGL_LEAKS 1

#ifdef USE_SDL2
#define SDL_NAME "SDL2"
#else  // USE_SDL2
#define SDL_NAME "SDL1.2"
#endif  // USE_SDL2
#if defined(ENABLE_PUGIXML)
#define XML_NAME "pugixml"
#elif defined(ENABLE_LIBXML)
#define XML_NAME "libxml2"
#else  // defined(ENABLE_PUGIXML)
#define XML_NAME "unknown"
#endif  // defined(ENABLE_PUGIXML)

#ifdef SIMD_SUPPORTED
#define SIMD_NAME ", SIMD"
#else  // SIMD_SUPPORTED
#define SIMD_NAME ""
#endif  // SIMD_SUPPORTED

#define PACKAGE_EXTENDED_VERSION "ManaPlus (" PACKAGE_OS \
"; %s; " SDL_NAME ", " XML_NAME SIMD_NAME "; 4144 v" SMALL_VERSION ")"
#define PACKAGE_VERSION_4144 "ManaPlus 4144-" SMALL_VERSION ""

#define FULL_VERSION "ManaPlus " SMALL_VERSION " " PACKAGE_OS ", " \
    SDL_NAME ", " XML_NAME SIMD_NAME

#ifdef ANDROID
#ifdef PKG_DATADIR
#undef PKG_DATADIR
#endif  // PKG_DATADIR
#ifdef LOCALEDIR
#undef LOCALEDIR
#endif  // LOCALEDIR
#define LOCALEDIR "locale"
#endif  // ANDROID

#ifndef PKG_DATADIR
#define PKG_DATADIR ""
#endif  // PKG_DATADIR

#endif  // MAIN_H
