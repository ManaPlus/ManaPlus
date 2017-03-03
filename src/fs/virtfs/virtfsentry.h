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

#ifndef UTILS_VIRTFSENTRY_H
#define UTILS_VIRTFSENTRY_H
#ifndef USE_PHYSFS

#include "enums/fs/fsentrytype.h"

#include <string>

#include "localconsts.h"

struct VirtFsFuncs;

struct VirtFsEntry notfinal
{
    VirtFsEntry(const FsEntryType &type0,
                VirtFsFuncs *restrict const funcs);

    A_DELETE_COPY(VirtFsEntry)

    ~VirtFsEntry();

    std::string root;

    FsEntryType type;

    VirtFsFuncs *funcs;
};

#endif  // USE_PHYSFS
#endif  // UTILS_VIRTFSENTRY_H
