/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
 */

#ifndef SRC_FS_VIRTFSRWOPS_H
#define SRC_FS_VIRTFSRWOPS_H

#include "localconsts.h"

#include <string>

struct SDL_RWops;

namespace VirtFs
{
    struct File;

    SDL_RWops *create_rwops(File *const file,
                            const std::string &restrict fname);

    SDL_RWops *rwopsOpenRead(const std::string &restrict fname);
    SDL_RWops *rwopsOpenWrite(const std::string &restrict fname) A_WARN_UNUSED;
    SDL_RWops *rwopsOpenAppend(const std::string &restrict fname)
                               A_WARN_UNUSED;
}  // namespace VirtFs

#endif  // SRC_FS_VIRTFSRWOPS_H
