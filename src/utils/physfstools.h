/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef UTILS_PHYSFSTOOLS_H
#define UTILS_PHYSFSTOOLS_H

#include <physfs.h>

namespace PhysFs
{
    void init(const char *const name);
    void updateDirSeparator();
    const char *getDirSeparator();
    const char *getBaseDir();
    const char *getUserDir();
    bool exists(const char *const fname);
    char **enumerateFiles(const char *const dir);
    bool isDirectory(const char *const fname);
    void freeList(void *const listVar);
    PHYSFS_file *openRead(const char *const filename);
    PHYSFS_file *openWrite(const char *const filename);
    PHYSFS_file *openAppend(const char *const filename);
    bool setWriteDir(const char *const newDir);
    bool addToSearchPath(const char *const newDir, const int appendToPath);
    bool removeFromSearchPath(const char *const oldDir);
    const char *getRealDir(const char *const filename);
    bool mkdir(const char *const dirName);
}  // namespace PhysFs

extern const char *dirSeparator;

#endif  // UTILS_PHYSFSTOOLS_H
