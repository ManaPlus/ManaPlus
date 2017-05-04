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

#ifndef UTILS_VIRTFSFUNCS_H
#define UTILS_VIRTFSFUNCS_H

#include "fs/virtfs/rwopstypes.h"

#include "utils/stringvector.h"

#include "localconsts.h"

struct SDL_RWops;

namespace VirtFs
{

struct File;
struct FsEntry;

struct FsFuncs final
{
    FsFuncs() :
        close(nullptr),
        read(nullptr),
        write(nullptr),
        fileLength(nullptr),
        tell(nullptr),
        seek(nullptr),
        exists(nullptr),
        getRealDir(nullptr),
        enumerate(nullptr),
        getFiles(nullptr),
        getFilesWithDir(nullptr),
        getDirs(nullptr),
        isDirectory(nullptr),
        openRead(nullptr),
        openWrite(nullptr),
        openAppend(nullptr),
        eof(nullptr),
        loadFile(nullptr),
        rwops_seek(nullptr),
        rwops_read(nullptr),
        rwops_write(nullptr),
#ifdef USE_SDL2
        rwops_size(nullptr),
#endif  // USE_SDL2
        rwops_close(nullptr)
    {
    }

    A_DELETE_COPY(FsFuncs)

    int (*close) (File *restrict const file);
    int64_t (*read) (File *restrict const file,
                     void *restrict const buffer,
                     const uint32_t objSize,
                     const uint32_t objCount);
    int64_t (*write) (File *restrict const file,
                      const void *restrict const buffer,
                      const uint32_t objSize,
                      const uint32_t objCount);
    int64_t (*fileLength) (File *restrict const file);
    int64_t (*tell) (File *restrict const file);
    int (*seek) (File *restrict const file,
                 const uint64_t pos);
    bool (*exists) (FsEntry *restrict const entry,
                    const std::string &filename,
                    const std::string &dirName);
    bool (*getRealDir) (FsEntry *restrict const entry,
                        const std::string &filename,
                        const std::string &dirName,
                        std::string &realDir);
    void (*enumerate) (FsEntry *restrict const entry,
                       const std::string &dirName,
                       StringVect &names);
    void (*getFiles) (FsEntry *restrict const entry,
                      const std::string &dirName,
                      StringVect &names);
    void (*getFilesWithDir) (FsEntry *restrict const entry,
                             const std::string &dirName,
                             StringVect &names);
    void (*getDirs) (FsEntry *restrict const entry,
                     const std::string &dirName,
                     StringVect &names);
    bool (*isDirectory) (FsEntry *restrict const entry,
                         const std::string &dirName,
                         bool &isDirFlag);
    File *(*openRead) (FsEntry *restrict const entry,
                       const std::string &filename);
    File *(*openWrite) (FsEntry *restrict const entry,
                        const std::string &filename);
    File *(*openAppend) (FsEntry *restrict const entry,
                         const std::string &filename);
    int (*eof) (File *restrict const file);
    const char *(*loadFile) (FsEntry *restrict const entry,
                             const std::string &restrict fileName,
                             int &restrict fileSize);

    RWOPSINT (*rwops_seek) (SDL_RWops *const rw,
                            const RWOPSINT offset,
                            const int whence);
    RWOPSSIZE (*rwops_read) (SDL_RWops *const rw,
                             void *const ptr,
                             const RWOPSSIZE size,
                             const RWOPSSIZE maxnum);
    RWOPSSIZE (*rwops_write) (SDL_RWops *const rw,
                              const void *const ptr,
                              const RWOPSSIZE size,
                              const RWOPSSIZE num);
#ifdef USE_SDL2
    RWOPSINT (*rwops_size) (SDL_RWops *const rw);
#endif  // USE_SDL2
    int (*rwops_close) (SDL_RWops *const rw);
};

}  // namespace VirtFs

#endif  // UTILS_VIRTFSFUNCS_H
