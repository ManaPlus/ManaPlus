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

#include "fs/virtfs.h"

#ifdef USE_PHYSFS
#include "fs/virtfsphys.h"
#else  // USE_PHYSFS
#include "fs/virtfsdir.h"
#endif  // USE_PHYSFS
#include "fs/virtfile.h"
#include "fs/virtfsfuncs.h"
#include "fs/virtlist.h"

#include "debug.h"

const char *dirSeparator = nullptr;

namespace VirtFs
{
    void init(const std::string &restrict name)
    {
#ifdef USE_PHYSFS
        VirtFsPhys::init(name);
#else  // USE_PHYSFS
        VirtFsDir::init(name);
#endif  // USE_PHYSFS
        updateDirSeparator();
    }

    void updateDirSeparator()
    {
#ifdef USE_PHYSFS
        dirSeparator = VirtFsPhys::getDirSeparator();
#else  // USE_PHYSFS
#ifdef WIN32
        dirSeparator = "\\";
#else  // WIN32
        dirSeparator = "/";
#endif  // WIN32
#endif  // USE_PHYSFS
    }

    const char *getDirSeparator()
    {
#ifdef USE_PHYSFS
        return VirtFsPhys::getDirSeparator();
#else  // USE_PHYSFS
        return dirSeparator;
#endif  // USE_PHYSFS
    }

    const char *getBaseDir()
    {
#ifdef USE_PHYSFS
        return VirtFsPhys::getBaseDir();
#else  // USE_PHYSFS
        return VirtFsDir::getBaseDir();
#endif  // USE_PHYSFS
    }

    const char *getUserDir()
    {
#ifdef USE_PHYSFS
        return VirtFsPhys::getUserDir();
#else  // USE_PHYSFS
        return VirtFsDir::getUserDir();
#endif  // USE_PHYSFS
    }

    bool exists(const std::string &restrict name)
    {
        return VirtFsPhys::exists(name);
    }

    VirtList *enumerateFiles(const std::string &restrict dir)
    {
        return VirtFsPhys::enumerateFiles(dir);
    }

    bool isDirectory(const std::string &restrict name)
    {
        return VirtFsPhys::isDirectory(name);
    }

    bool isSymbolicLink(const std::string &restrict name)
    {
        return VirtFsPhys::isSymbolicLink(name);
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
    }

    VirtFile *openRead(const std::string &restrict filename)
    {
        return VirtFsPhys::openRead(filename);
    }

    VirtFile *openWrite(const std::string &restrict filename)
    {
        return VirtFsPhys::openWrite(filename);
    }

    VirtFile *openAppend(const std::string &restrict filename)
    {
        return VirtFsPhys::openAppend(filename);
    }

    bool setWriteDir(const std::string &restrict newDir)
    {
        return VirtFsPhys::setWriteDir(newDir);
    }

    bool addDirToSearchPath(const std::string &restrict newDir,
                            const Append append)
    {
        return VirtFsPhys::addDirToSearchPath(newDir, append);
    }

    bool removeDirFromSearchPath(const std::string &restrict oldDir)
    {
        return VirtFsPhys::removeDirFromSearchPath(oldDir);
    }

    bool addZipToSearchPath(const std::string &restrict newDir,
                            const Append append)
    {
        return VirtFsPhys::addZipToSearchPath(newDir, append);
    }

    bool removeZipFromSearchPath(const std::string &restrict oldDir)
    {
        return VirtFsPhys::removeZipFromSearchPath(oldDir);
    }

    std::string getRealDir(const std::string &restrict filename)
    {
        return VirtFsPhys::getRealDir(filename);
    }

    bool mkdir(const std::string &restrict dirname)
    {
        return VirtFsPhys::mkdir(dirname);
    }

    bool remove(const std::string &restrict filename)
    {
        return VirtFsPhys::remove(filename);
    }

    bool deinit()
    {
#ifdef USE_PHYSFS
        return VirtFsPhys::deinit();
#else  // USE_PHYSFS
        VirtFsDir::deinit();
        return true;
#endif  // USE_PHYSFS
    }

    void permitLinks(const bool val)
    {
        VirtFsPhys::permitLinks(val);
    }

    const char *getLastError()
    {
        return VirtFsPhys::getLastError();
    }

    int close(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return 0;
        return file->funcs->close(file);
    }

    int64_t read(VirtFile *restrict const file,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount)
    {
        return file->funcs->read(file,
            buffer,
            objSize,
            objCount);
    }

    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount)
    {
        return file->funcs->write(file,
            buffer,
            objSize,
            objCount);
    }

    int64_t fileLength(VirtFile *restrict const file)
    {
        return file->funcs->fileLength(file);
    }

    int64_t tell(VirtFile *restrict const file)
    {
        return file->funcs->tell(file);
    }

    int seek(VirtFile *restrict const file,
             const uint64_t pos)
    {
        return file->funcs->seek(file,
            pos);
    }

    int eof(VirtFile *restrict const file)
    {
        return file->funcs->eof(file);
    }
}  // namespace VirtFs
