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

#include "utils/physfstools.h"

#include "logger.h"

#include <iostream>
#include <unistd.h>

#ifdef ANDROID
#include "utils/paths.h"
#endif  // ANDROID

#include "debug.h"

const char *dirSeparator = nullptr;

namespace PhysFs
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
                << PHYSFS_getLastError() << std::endl;
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

    PHYSFS_file *openRead(const char *const filename)
    {
        return PHYSFS_openRead(filename);
    }

    PHYSFS_file *openWrite(const char *const filename)
    {
        return PHYSFS_openWrite(filename);
    }

    PHYSFS_file *openAppend(const char *const filename)
    {
        return PHYSFS_openAppend(filename);
    }

    bool setWriteDir(const char *const newDir)
    {
        return PHYSFS_setWriteDir(newDir);
    }

    bool addToSearchPath(const char *const newDir, const int appendToPath)
    {
        return PHYSFS_addToSearchPath(newDir, appendToPath);
    }

    bool removeFromSearchPath(const char *const oldDir)
    {
        return PHYSFS_removeFromSearchPath(oldDir);
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
        PHYSFS_file *const file = PhysFs::openRead(fileName.c_str());

        if (!file)
        {
            logger->log("Warning: Failed to load %s: %s",
                        fileName.c_str(), PHYSFS_getLastError());
            return nullptr;
        }

        logger->log("Loaded %s/%s", PhysFs::getRealDir(fileName.c_str()),
            fileName.c_str());

        fileSize = CAST_S32(PHYSFS_fileLength(file));
        // Allocate memory and load the file
        void *const buffer = calloc(fileSize, 1);
        PHYSFS_read(file, buffer, 1, fileSize);
        PHYSFS_close(file);

        return buffer;
    }
}  // namespace PhysFs
