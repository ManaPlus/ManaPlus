/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2018  The ManaPlus Developers
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

#ifndef UTILS_VIRTFS_VIRTFSZIPRWOPS_H
#define UTILS_VIRTFS_VIRTFSZIPRWOPS_H

#include "fs/virtfs/rwopstypes.h"

#include "localconsts.h"

struct SDL_RWops;

namespace VirtFs
{

namespace FsZip
{
    RWOPSINT rwops_seek(SDL_RWops *const rw,
                        const RWOPSINT offset,
                        const int whence);
    RWOPSSIZE rwops_read(SDL_RWops *const rw,
                         void *const ptr,
                         const RWOPSSIZE size,
                         const RWOPSSIZE maxnum);
    RWOPSSIZE rwops_write(SDL_RWops *const rw,
                          const void *const ptr,
                          const RWOPSSIZE size,
                          const RWOPSSIZE num);
    int rwops_close(SDL_RWops *const rw);
#ifdef USE_SDL2
    RWOPSINT rwops_size(SDL_RWops *const rw);
#endif  // USE_SDL2

}  // namespace FsZip

}  // namespace VirtFs

#endif  // UTILS_VIRTFS_VIRTFSZIPRWOPS_H
