/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#ifndef UTILS_FS_VIRTFS_VIRTFS_H
#define UTILS_FS_VIRTFS_VIRTFS_H

#include "enums/simpletypes/append.h"

#include "localconsts.h"

#include <string>
#include <vector>

struct VirtFsEntry;

namespace VirtFs
{
    bool addDirToSearchPathInternal(const std::string &restrict newDir,
                                    const Append append);
    bool removeDirFromSearchPathInternal(std::string oldDir);
    std::vector<VirtFsEntry*> &getEntries();
    VirtFsEntry *searchEntryByRootInternal(const std::string &restrict
                                           root);
    VirtFsEntry *searchEntryInternal(const std::string &restrict root,
                                     const FsEntryType type);
    void addEntry(VirtFsEntry *const entry,
                  const Append append);
#ifdef UNITTESTS
    bool addDirToSearchPathSilent2(std::string newDir,
                                   const Append append);
#endif  // UNITTESTS
}  // namespace VirtFs

#endif  // UTILS_FS_VIRTFS_VIRTFS_H
