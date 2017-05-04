/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#ifndef UTILS_VIRTFILE_H
#define UTILS_VIRTFILE_H

#include "localconsts.h"

#include "fs/virtfs/fileapi.h"

namespace VirtFs
{

struct VirtFsFuncs;

struct VirtFile final
{
    VirtFile(const VirtFsFuncs *restrict const funcs0,
             const uint8_t *restrict const buf,
             const size_t sz);

    VirtFile(const VirtFsFuncs *restrict const funcs0,
             FILEHTYPE fd);

    A_DELETE_COPY(VirtFile)

    ~VirtFile();

    const VirtFsFuncs *funcs;

    // zipfs fields
    const uint8_t *mBuf;

    // zipfs fields
    size_t mPos;
    size_t mSize;

    // dirfs fields
    FILEHTYPE mFd;
};

}  // namespace VirtFs

#endif  // UTILS_VIRTFILE_H
