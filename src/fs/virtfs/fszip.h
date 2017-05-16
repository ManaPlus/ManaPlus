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

namespace VirtFs
{

struct File;
struct List;
struct FsFuncs;
struct FsEntry;

namespace FsZip
{
    FsFuncs *getFuncs();
    void init();
    void initFuncs(FsFuncs *restrict const ptr);
    void deinit();
    bool exists(FsEntry *restrict const entry,
                std::string filename,
                std::string dirName);
    void enumerate(FsEntry *restrict const entry,
                   std::string dirName,
                   StringVect &names);
    void getFiles(FsEntry *restrict const entry,
                  const std::string &dirName,
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
    void freeList(List *restrict const handle);
    File *openRead(FsEntry *restrict const entry,
                   std::string filename);
    File *openWrite(FsEntry *restrict const entry,
                    const std::string &filename);
    File *openAppend(FsEntry *restrict const entry,
                     const std::string &filename);
    File *openReadInternal(const std::string &filename);
    bool getRealDir(FsEntry *restrict const entry,
                    std::string filename,
                    std::string dirName,
                    std::string &realDir);
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
}  // namespace FsZip

}  // namespace VirtFs

#endif  // UTILS_VIRTFSZIP_H
