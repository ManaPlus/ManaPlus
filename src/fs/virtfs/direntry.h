/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef UTILS_VIRTDIRENTRY_H
#define UTILS_VIRTDIRENTRY_H

#include "fs/virtfs/fsentry.h"

#include "localconsts.h"

namespace VirtFs
{

struct DirEntry final : public FsEntry
{
    DirEntry(const std::string &userDir0,
             const std::string &rootDir,
             const std::string &subDir0,
             const std::string &rootSubDir0,
             FsFuncs *restrict const funcs0);

    A_DELETE_COPY(DirEntry)

    virtual ~DirEntry();

    std::string userDir;
    std::string rootSubDir;
};

}  // namespace VirtFs

#endif  // UTILS_VIRTDIRENTRY_H
