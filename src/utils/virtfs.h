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
struct VirtList;

namespace VirtFs
{
    void init(const std::string &restrict name);
    void updateDirSeparator();
    const char *getDirSeparator();
    const char *getBaseDir();
    const char *getUserDir();
    bool exists(const std::string &restrict name);
    VirtList *enumerateFiles(const std::string &restrict dir) RETURNS_NONNULL;
    bool isDirectory(const std::string &restrict name);
    void freeList(VirtList *restrict const handle);
    VirtFile *openRead(const std::string &restrict filename);
    VirtFile *openWrite(const std::string &restrict filename);
    VirtFile *openAppend(const std::string &restrict filename);
    bool setWriteDir(const std::string &restrict newDir);
    bool addDirToSearchPath(const std::string &restrict newDir,
                            const Append append);
    bool removeDirFromSearchPath(const std::string &restrict oldDir);
    bool addZipToSearchPath(const std::string &restrict newDir,
                            const Append append);
    bool removeZipFromSearchPath(const std::string &restrict oldDir);
    std::string getRealDir(const std::string &restrict filename);
    bool mkdir(const std::string &restrict dirName);
    bool remove(const std::string &restrict filename);
    bool deinit();
    void permitLinks(const bool val);
    const char *getLastError();
    int64_t read(VirtFile *restrict const handle,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount);
    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount);
    int close(VirtFile *restrict const file);
    int64_t fileLength(VirtFile *restrict const file);
    int64_t tell(VirtFile *restrict const file);
    int seek(VirtFile *restrict const file,
             const uint64_t pos);
    int eof(VirtFile *restrict const file);
}  // namespace VirtFs

extern const char *dirSeparator;

#endif  // UTILS_VIRTFS_H
