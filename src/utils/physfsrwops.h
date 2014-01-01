/*
 * This code provides a glue layer between PhysicsFS and Simple Directmedia
 *  Layer's (SDL) RWops i/o abstraction.
 *
 * License: this code is public domain. I make no warranty that it is useful,
 *  correct, harmless, or environmentally safe.
 *
 * This particular file may be used however you like, including copying it
 *  verbatim into a closed-source project, exploiting it commercially, and
 *  removing any trace of my name from the source (although I hope you won't
 *  do that). I welcome enhancements and corrections to this file, but I do
 *  not require you to send me patches if you make changes. This code has
 *  NO WARRANTY.
 *
 * Unless otherwise stated, the rest of PhysicsFS falls under the zlib license.
 *  Please see LICENSE.txt in the root of the source tree.
 *
 * SDL falls under the LGPL license. You can get SDL at http://www.libsdl.org/
 *
 *  This file was written by Ryan C. Gordon. (icculus@icculus.org).
 *
 *  Copyright (C) 2012-2014  The ManaPlus Developers
 */

#ifndef UTILS_PHYSFSRWOPS_H
#define UTILS_PHYSFSRWOPS_H

#include <SDL_rwops.h>

#include "utils/physfstools.h"

#include "localconsts.h"

/**
 * Open a platform-independent filename for reading, and make it accessible
 *  via an SDL_RWops structure. The file will be closed in PhysicsFS when the
 *  RWops is closed. PhysicsFS should be configured to your liking before
 *  opening files through this method.
 *
 *   @param filename File to open in platform-independent notation.
 *  @return A valid SDL_RWops structure on success, NULL on error. Specifics
 *           of the error can be gleaned from PHYSFS_getLastError().
 */
SDL_RWops *PHYSFSRWOPS_openRead(const char *const fname) A_WARN_UNUSED;

/**
 * Open a platform-independent filename for writing, and make it accessible
 *  via an SDL_RWops structure. The file will be closed in PhysicsFS when the
 *  RWops is closed. PhysicsFS should be configured to your liking before
 *  opening files through this method.
 *
 *   @param filename File to open in platform-independent notation.
 *  @return A valid SDL_RWops structure on success, NULL on error. Specifics
 *           of the error can be gleaned from PHYSFS_getLastError().
 */
SDL_RWops *PHYSFSRWOPS_openWrite(const char *const fname) A_WARN_UNUSED;

/**
 * Open a platform-independent filename for appending, and make it accessible
 *  via an SDL_RWops structure. The file will be closed in PhysicsFS when the
 *  RWops is closed. PhysicsFS should be configured to your liking before
 *  opening files through this method.
 *
 *   @param filename File to open in platform-independent notation.
 *  @return A valid SDL_RWops structure on success, NULL on error. Specifics
 *           of the error can be gleaned from PHYSFS_getLastError().
 */
SDL_RWops *PHYSFSRWOPS_openAppend(const char *const fname) A_WARN_UNUSED;

/**
 * Make a SDL_RWops from an existing PhysicsFS file handle. You should
 *  dispose of any references to the handle after successful creation of
 *  the RWops. The actual PhysicsFS handle will be destroyed when the
 *  RWops is closed.
 *
 *   @param handle a valid PhysicsFS file handle.
 *  @return A valid SDL_RWops structure on success, NULL on error. Specifics
 *           of the error can be gleaned from PHYSFS_getLastError().
 */
SDL_RWops *PHYSFSRWOPS_makeRWops(PHYSFS_file *const handle) A_WARN_UNUSED;

#ifdef DUMP_LEAKED_RESOURCES
void reportRWops();
#endif

#endif  // UTILS_PHYSFSRWOPS_H
