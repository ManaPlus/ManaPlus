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

#include "utils/virtfs.h"

#include "logger.h"

#include "utils/virtfile.h"
#include "utils/virtfileprivate.h"

#include <iostream>
#include <unistd.h>

#ifdef ANDROID
#include "utils/paths.h"
#endif  // ANDROID

#include "debug.h"

const char *dirSeparator = nullptr;

namespace VirtFs
{
#if defined(__native_client__)
    void init(const char *const name A_UNUSED)
    {
        if (!PHYSFS_init("/fakebinary"))
#elif defined(ANDROID)
    void init(const char *const name A_UNUSED)
    {
        if (!PHYSFS_init((getRealPath(".").append("/fakebinary")).c_str()))
#else  // defined(__native_client__)

    void init(const char *const name)
    {
        if (!PHYSFS_init(name))
#endif  // defined(__native_client__)
        {
            std::cout << "Error while initializing PhysFS: "
                << VirtFs::getLastError() << std::endl;
            _exit(1);
        }
        updateDirSeparator();
        atexit(reinterpret_cast<void(*)()>(PHYSFS_deinit));
    }

    void updateDirSeparator()
    {
        dirSeparator = PHYSFS_getDirSeparator();
    }

    const char *getDirSeparator()
    {
        return dirSeparator;
    }

    const char *getBaseDir()
    {
        return PHYSFS_getBaseDir();
    }

    const char *getUserDir()
    {
        return PHYSFS_getUserDir();
    }

    bool exists(const char *const fname)
    {
        return PHYSFS_exists(fname);
    }

    char **enumerateFiles(const char *const dir)
    {
        return PHYSFS_enumerateFiles(dir);
    }

    bool isDirectory(const char *const fname)
    {
        return PHYSFS_isDirectory(fname);
    }

    void freeList(void *const listVar)
    {
        PHYSFS_freeList(listVar);
    }

    VirtFile *openRead(const char *const filename)
    {
        PHYSFS_file *const handle = PHYSFS_openRead(filename);
        if (!handle)
            return nullptr;
        VirtFile *const file = new VirtFile;
        file->mPrivate = new VirtFilePrivate(handle);
        return file;
    }

    VirtFile *openWrite(const char *const filename)
    {
        PHYSFS_file *const handle = PHYSFS_openWrite(filename);
        if (!handle)
            return nullptr;
        VirtFile *const file = new VirtFile;
        file->mPrivate = new VirtFilePrivate(handle);
        return file;
    }

    VirtFile *openAppend(const char *const filename)
    {
        PHYSFS_file *const handle = PHYSFS_openAppend(filename);
        if (!handle)
            return nullptr;
        VirtFile *const file = new VirtFile;
        file->mPrivate = new VirtFilePrivate(handle);
        return file;
    }

    bool setWriteDir(const char *const newDir)
    {
        return PHYSFS_setWriteDir(newDir);
    }

    bool addDirToSearchPath(const std::string &newDir,
                            const Append append)
    {
        return PHYSFS_addToSearchPath(newDir.c_str(),
            append == Append_true ? 1 : 0);
    }

    bool removeDirFromSearchPath(const std::string &oldDir)
    {
        return PHYSFS_removeFromSearchPath(oldDir.c_str());
    }

    bool addZipToSearchPath(const std::string &newDir,
                            const Append append)
    {
        return PHYSFS_addToSearchPath(newDir.c_str(),
            append == Append_true ? 1 : 0);
    }

    bool removeZipFromSearchPath(const std::string &oldDir)
    {
        return PHYSFS_removeFromSearchPath(oldDir.c_str());
    }

    const char *getRealDir(const char *const filename)
    {
        return PHYSFS_getRealDir(filename);
    }

    bool mkdir(const char *const dirname)
    {
        return PHYSFS_mkdir(dirname);
    }

    void *loadFile(const std::string &fileName, int &fileSize)
    {
        // Attempt to open the specified file using PhysicsFS
        VirtFile *const file = VirtFs::openRead(fileName.c_str());

        if (!file)
        {
            logger->log("Warning: Failed to load %s: %s",
                fileName.c_str(),
                VirtFs::getLastError());
            return nullptr;
        }

        logger->log("Loaded %s/%s",
            VirtFs::getRealDir(fileName.c_str()),
            fileName.c_str());

        fileSize = CAST_S32(VirtFs::fileLength(file));
        // Allocate memory and load the file
        void *const buffer = calloc(fileSize, 1);
        VirtFs::read(file, buffer, 1, fileSize);
        VirtFs::close(file);

        return buffer;
    }

    bool deinit()
    {
        if (PHYSFS_deinit() != 0)
        {
            logger->log("Physfs deinit error: %s",
                VirtFs::getLastError());
            return false;
        }
        return true;
    }

    void permitLinks(const bool val)
    {
        PHYSFS_permitSymbolicLinks(val ? 1 : 0);
    }

    const char *getLastError()
    {
        return PHYSFS_getLastError();
    }

    int close(VirtFile *const file)
    {
        if (file == nullptr)
            return 0;
        delete file;
        return 1;
    }

    int64_t read(VirtFile *const file,
                 void *const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount)
    {
        if (file == nullptr)
            return 0;
        return PHYSFS_read(file->mPrivate->mFile,
            buffer,
            objSize,
            objCount);
    }

    int64_t write(VirtFile *const file,
                  const void *const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount)
    {
        if (file == nullptr)
            return 0;
        return PHYSFS_write(file->mPrivate->mFile,
            buffer,
            objSize,
            objCount);
    }

    int64_t fileLength(VirtFile *const file)
    {
        if (file == nullptr)
            return -1;
        return PHYSFS_fileLength(file->mPrivate->mFile);
    }

    int64_t tell(VirtFile *const file)
    {
        if (file == nullptr)
            return -1;
        return PHYSFS_tell(file->mPrivate->mFile);
    }

    int seek(VirtFile *const file,
             const uint64_t pos)
    {
        return PHYSFS_seek(file->mPrivate->mFile,
            pos);
    }

    int eof(VirtFile *const file)
    {
        return PHYSFS_eof(file->mPrivate->mFile);
    }
}  // namespace PhysFs
