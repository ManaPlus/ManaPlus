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

struct File;
struct FsEntry;
struct FsFuncs;
struct List;

namespace FsDir
{
    File *openInternal(FsEntry *restrict const entry,
                       const std::string &filename,
                       const FILEMTYPE mode);
    File *openRead(FsEntry *restrict const entry,
                   std::string filename);
    File *openWrite(FsEntry *restrict const entry,
                    const std::string &filename);
    File *openAppend(FsEntry *restrict const entry,
                     const std::string &filename);
    const char *getBaseDir();
    const char *getUserDir();
    FsFuncs *getFuncs();
    void init(const std::string &restrict name);
    void initFuncs(FsFuncs *restrict const ptr);
    void deinit();
    bool exists(FsEntry *restrict const entry,
                std::string fileName,
                std::string dirName);
    void enumerate(FsEntry *restrict const entry,
                   std::string dirName,
                   StringVect &names);
    void getFiles(FsEntry *restrict const entry,
                  std::string dirName,
                  StringVect &names);
    void getFilesWithDir(FsEntry *restrict const entry,
                         const std::string &dirName,
                         StringVect &names);
    void getDirs(FsEntry *restrict const entry,
                 const std::string &dirName,
                 StringVect &names);
    bool isDirectory(FsEntry *restrict const entry,
                     std::string dirName,
                     bool &isDirFlag);
    bool isSymbolicLink(std::string name);
    void freeList(List *restrict const handle);
    bool setWriteDir(std::string newDir);
    bool getRealDir(FsEntry *restrict const entry,
                    std::string filename,
                    std::string dirName,
                    std::string &realDir);
    bool mkdir(std::string dirName);
    bool remove(std::string filename);
    void permitLinks(const bool val);
    int64_t read(File *restrict const handle,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount);
    int64_t write(File *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount);
    int close(File *restrict const file);
    int64_t fileLength(File *restrict const file);
    int64_t tell(File *restrict const file);
    int seek(File *restrict const file,
             const uint64_t pos);
    int eof(File *restrict const file);
    const char *loadFile(FsEntry *restrict const entry,
                         std::string fileName,
                         int &restrict fileSize);
}  // namespace FsDir

}  // namespace VirtFs

#endif  // UTILS_VIRTFSDIR_H
