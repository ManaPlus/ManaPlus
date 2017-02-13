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

#ifndef UTILS_VIRTFS_H
#define UTILS_VIRTFS_H

#include "enums/simpletypes/append.h"

#include "localconsts.h"

#include <string>

struct VirtFile;

namespace VirtFs
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
    VirtFile *openRead(const char *const filename);
    VirtFile *openWrite(const char *const filename);
    VirtFile *openAppend(const char *const filename);
    bool setWriteDir(const std::string &newDir);
    bool addDirToSearchPath(const std::string &newDir,
                            const Append append);
    bool removeDirFromSearchPath(const std::string &oldDir);
    bool addZipToSearchPath(const std::string &newDir,
                            const Append append);
    bool removeZipFromSearchPath(const std::string &oldDir);
    const char *getRealDir(const char *const filename);
    bool mkdir(const char *const dirName);
    bool deinit();
    void permitLinks(const bool val);
    const char *getLastError();
    int64_t read(VirtFile *const handle,
                 void *const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount);
    int64_t write(VirtFile *const file,
                  const void *const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount);
    int close(VirtFile *const file);
    int64_t fileLength(VirtFile *const file);
    int64_t tell(VirtFile *const file);
    int seek(VirtFile *const file,
             const uint64_t pos);
    int eof(VirtFile *const file);
}  // namespace VirtFs

extern const char *dirSeparator;

#endif  // UTILS_VIRTFS_H
