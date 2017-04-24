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

#ifndef UTILS_VIRTFSZIP_H
#define UTILS_VIRTFSZIP_H

#include "utils/stringvector.h"

#include "localconsts.h"

struct VirtFile;
struct VirtList;
struct VirtFsFuncs;
struct VirtFsEntry;

namespace VirtFsZip
{
    VirtFsFuncs *getFuncs();
    void init();
    void initFuncs(VirtFsFuncs *restrict const ptr);
    void deinit();
    bool exists(VirtFsEntry *restrict const entry,
                const std::string &filename,
                const std::string &dirName);
    void enumerate(VirtFsEntry *restrict const entry,
                   const std::string &dirName,
                   StringVect &names);
    void getFiles(VirtFsEntry *restrict const entry,
                  const std::string &dirName,
                  StringVect &names);
    bool isDirectory(VirtFsEntry *restrict const entry,
                     const std::string &dirName,
                     bool &isDirFlag);
    void freeList(VirtList *restrict const handle);
    VirtFile *openRead(VirtFsEntry *restrict const entry,
                       const std::string &filename);
    VirtFile *openWrite(VirtFsEntry *restrict const entry,
                        const std::string &filename);
    VirtFile *openAppend(VirtFsEntry *restrict const entry,
                         const std::string &filename);
    VirtFile *openReadInternal(const std::string &filename);
    bool getRealDir(VirtFsEntry *restrict const entry,
                    const std::string &filename,
                    const std::string &dirName,
                    std::string &realDir);
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
    const char *loadFile(VirtFsEntry *restrict const entry,
                         const std::string &restrict fileName,
                         int &restrict fileSize);
}  // namespace VirtFsZip

#endif  // UTILS_VIRTFSZIP_H
