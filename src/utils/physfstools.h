/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef UTILS_PHYSFS_H
#define UTILS_PHYSFS_H

#include <physfs.h>

namespace PhysFs
{
    void updateDirSeparator();
    const char *getDirSeparator();
    const char *getBaseDir();
    const char *getUserDir();
    bool exists(const char *fname);
    char **enumerateFiles(const char *dir);
    bool isDirectory(const char *fname);
    void freeList(void *listVar);
    PHYSFS_file *openRead(const char *filename);
    PHYSFS_file *openWrite(const char *filename);
    PHYSFS_file *openAppend(const char *filename);
    bool setWriteDir(const char *newDir);
    bool addToSearchPath(const char *newDir, int appendToPath);
    bool removeFromSearchPath(const char *oldDir);
    const char *getRealDir(const char *filename);
    bool mkdir(const char *dirName);
}  // namespace PhysFs

extern const char *dirSeparator;

#endif  // UTILS_PHYSFS_H
