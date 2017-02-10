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
 *  Copyright (C) 2012-2017  The ManaPlus Developers
 */

#ifndef UTILS_PHYSFSRWOPS_H
#define UTILS_PHYSFSRWOPS_H

#include "localconsts.h"

#include <SDL_rwops.h>

#include "utils/virtfs.h"

namespace VirtFs
{
#ifdef DEBUG_VIRTFS
    SDL_RWops *RWopsOpenRead(const char *const fname,
                             const char *restrict const file,
                             const unsigned line);
    void reportLeaks();
#else  // DEBUG_VIRTFS
    SDL_RWops *RWopsOpenRead(const char *const fname);
#endif  // DEBUG_VIRTFS

    SDL_RWops *RWopsOpenWrite(const char *const fname) A_WARN_UNUSED;
    SDL_RWops *RWopsOpenAppend(const char *const fname) A_WARN_UNUSED;
    SDL_RWops *MakeRWops(PHYSFS_file *const handle) A_WARN_UNUSED;
#ifdef DUMP_LEAKED_RESOURCES
    void reportRWops();
#endif  // DUMP_LEAKED_RESOURCES
}  // namespace VirtFs

#endif  // UTILS_PHYSFSRWOPS_H
