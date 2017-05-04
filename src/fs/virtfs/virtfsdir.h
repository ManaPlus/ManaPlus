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

#ifndef UTILS_VIRTFSDIR_H
#define UTILS_VIRTFSDIR_H

#include "fs/virtfs/fileapi.h"

#include "utils/stringvector.h"

#include "localconsts.h"

namespace VirtFs
{

struct VirtFile;
struct VirtFsEntry;
struct VirtFsFuncs;
struct VirtList;

namespace VirtFsDir
{
    VirtFile *openInternal(VirtFsEntry *restrict const entry,
                           const std::string &filename,
                           const FILEMTYPE mode);
    VirtFile *openRead(VirtFsEntry *restrict const entry,
                       const std::string &filename);
    VirtFile *openWrite(VirtFsEntry *restrict const entry,
                        const std::string &filename);
    VirtFile *openAppend(VirtFsEntry *restrict const entry,
                         const std::string &filename);
    const char *getBaseDir();
    const char *getUserDir();
    VirtFsFuncs *getFuncs();
    void init(const std::string &restrict name);
    void initFuncs(VirtFsFuncs *restrict const ptr);
    void deinit();
    bool exists(VirtFsEntry *restrict const entry,
                const std::string &fileName,
                const std::string &dirName);
    void enumerate(VirtFsEntry *restrict const entry,
                   const std::string &dirName,
                   StringVect &names);
    void getFiles(VirtFsEntry *restrict const entry,
                  const std::string &dirName,
                  StringVect &names);
    void getFilesWithDir(VirtFsEntry *restrict const entry,
                         const std::string &dirName,
                         StringVect &names);
    void getDirs(VirtFsEntry *restrict const entry,
                 const std::string &dirName,
                 StringVect &names);
    bool isDirectory(VirtFsEntry *restrict const entry,
                     const std::string &dirName,
                     bool &isDirFlag);
    bool isSymbolicLink(std::string name);
    void freeList(VirtList *restrict const handle);
    bool setWriteDir(std::string newDir);
    bool getRealDir(VirtFsEntry *restrict const entry,
                    const std::string &filename,
                    const std::string &dirName,
                    std::string &realDir);
    bool mkdir(std::string dirName);
    bool remove(std::string filename);
    void permitLinks(const bool val);
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
}  // namespace VirtFsDir

}  // namespace VirtFs

#endif  // UTILS_VIRTFSDIR_H
