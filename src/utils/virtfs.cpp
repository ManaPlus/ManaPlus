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

#include "utils/checkutils.h"
#include "utils/virtfile.h"
#include "utils/virtfileprivate.h"
#include "utils/virtlist.h"

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
    void init(const std::string &restrict name A_UNUSED)
    {
        if (!PHYSFS_init("/fakebinary"))
#elif defined(ANDROID)
    void init(const std::string &restrict name A_UNUSED)
    {
        if (!PHYSFS_init((getRealPath(".").append("/fakebinary")).c_str()))
#else  // defined(__native_client__)

    void init(const std::string &restrict name)
    {
        if (!PHYSFS_init(name.c_str()))
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

    bool exists(const std::string &restrict name)
    {
        return PHYSFS_exists(name.c_str());
    }

    VirtList *enumerateFiles(const std::string &restrict dir)
    {
        char** handle = PHYSFS_enumerateFiles(dir.c_str());
        VirtList *const files = new VirtList;
        if (handle == nullptr)
            return files;
        for (char **i = handle; *i; i++)
        {
            std::string str = *i;
            files->names.push_back(str);
        }
        PHYSFS_freeList(handle);
        return files;
    }

    bool isDirectory(const std::string &restrict name)
    {
        return PHYSFS_isDirectory(name.c_str());
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
    }

    VirtFile *openRead(const std::string &restrict filename)
    {
        PHYSFS_file *restrict const handle = PHYSFS_openRead(
            filename.c_str());
        if (!handle)
            return nullptr;
        VirtFile *restrict const file = new VirtFile;
        file->mPrivate = new VirtFilePrivate(handle);
        return file;
    }

    VirtFile *openWrite(const std::string &restrict filename)
    {
        PHYSFS_file *restrict const handle = PHYSFS_openWrite(
            filename.c_str());
        if (!handle)
            return nullptr;
        VirtFile *restrict const file = new VirtFile;
        file->mPrivate = new VirtFilePrivate(handle);
        return file;
    }

    VirtFile *openAppend(const std::string &restrict filename)
    {
        PHYSFS_file *restrict const handle = PHYSFS_openAppend(
            filename.c_str());
        if (!handle)
            return nullptr;
        VirtFile *restrict const file = new VirtFile;
        file->mPrivate = new VirtFilePrivate(handle);
        return file;
    }

    bool setWriteDir(const std::string &restrict newDir)
    {
        return PHYSFS_setWriteDir(newDir.c_str());
    }

    bool addDirToSearchPath(const std::string &restrict newDir,
                            const Append append)
    {
        logger->log("Add virtual directory: " + newDir);
        if (newDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called addDirToSearchPath with zip archive");
            return false;
        }
        return PHYSFS_addToSearchPath(newDir.c_str(),
            append == Append_true ? 1 : 0);
    }

    bool removeDirFromSearchPath(const std::string &restrict oldDir)
    {
        logger->log("Remove virtual directory: " + oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called removeDirFromSearchPath with zip archive");
            return false;
        }
        return PHYSFS_removeFromSearchPath(oldDir.c_str());
    }

    bool addZipToSearchPath(const std::string &restrict newDir,
                            const Append append)
    {
        logger->log("Add virtual zip: " + newDir);
        if (newDir.find(".zip") == std::string::npos)
        {
            reportAlways("Called addZipToSearchPath without zip archive");
            return false;
        }
        return PHYSFS_addToSearchPath(newDir.c_str(),
            append == Append_true ? 1 : 0);
    }

    bool removeZipFromSearchPath(const std::string &restrict oldDir)
    {
        logger->log("Remove virtual zip: " + oldDir);
        if (oldDir.find(".zip") == std::string::npos)
        {
            reportAlways("Called removeZipFromSearchPath without zip archive");
            return false;
        }
        return PHYSFS_removeFromSearchPath(oldDir.c_str());
    }

    std::string getRealDir(const std::string &restrict filename)
    {
        const char *const str = PHYSFS_getRealDir(filename.c_str());
        if (str == nullptr)
            return std::string();
        return str;
    }

    bool mkdir(const std::string &restrict dirname)
    {
        return PHYSFS_mkdir(dirname.c_str());
    }

    bool remove(const std::string &restrict filename)
    {
        return PHYSFS_delete(filename.c_str());
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

    int close(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return 0;
        delete file;
        return 1;
    }

    int64_t read(VirtFile *restrict const file,
                 void *restrict const buffer,
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

    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
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

    int64_t fileLength(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;
        return PHYSFS_fileLength(file->mPrivate->mFile);
    }

    int64_t tell(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;
        return PHYSFS_tell(file->mPrivate->mFile);
    }

    int seek(VirtFile *restrict const file,
             const uint64_t pos)
    {
        return PHYSFS_seek(file->mPrivate->mFile,
            pos);
    }

    int eof(VirtFile *restrict const file)
    {
        return PHYSFS_eof(file->mPrivate->mFile);
    }
}  // namespace VirtFs
