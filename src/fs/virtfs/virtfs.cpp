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

#include "fs/paths.h"
#include "fs/virtfile.h"
#include "fs/virtfsfuncs.h"
#include "fs/virtlist.h"

#include "fs/virtfs/virtdirentry.h"
#include "fs/virtfs/virtfsdir.h"
#include "fs/virtfs/virtfszip.h"

#include "utils/checkutils.h"

#include "debug.h"

const char *dirSeparator = nullptr;

namespace VirtFs
{
    void init(const std::string &restrict name)
    {
        VirtFsDir::init(name);
        updateDirSeparator();
    }

    void updateDirSeparator()
    {
#ifdef WIN32
        dirSeparator = "\\";
#else  // WIN32
        dirSeparator = "/";
#endif  // WIN32
    }

    const char *getDirSeparator()
    {
        return dirSeparator;
    }

    const char *getBaseDir()
    {
        return VirtFsDir::getBaseDir();
    }

    const char *getUserDir()
    {
        return VirtFsDir::getUserDir();
    }

    bool exists(const std::string &restrict name)
    {
        return VirtFsDir::exists(name) || VirtFsZip::exists(name);
    }

    VirtList *enumerateFiles(std::string dirName)
    {
        VirtList *const list = new VirtList;
        prepareFsPath(dirName);
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFs::enumerateFiles invalid path: %s",
                dirName.c_str());
            return list;
        }

        VirtFsDir::enumerateFiles(dirName, list);
        VirtFsZip::enumerateFiles(dirName, list);
        return list;
    }

    bool isDirectory(std::string name)
    {
        prepareFsPath(name);
        if (checkPath(name) == false)
        {
            reportAlways("VirtFs::isDirectory invalid path: %s",
                name.c_str());
            return false;
        }
        return VirtFsDir::isDirectoryInternal(name) ||
            VirtFsZip::isDirectoryInternal(name);
    }

    bool isSymbolicLink(const std::string &restrict name)
    {
        return VirtFsDir::isSymbolicLink(name);
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
    }

    VirtFile *openRead(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::openRead invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        VirtDirEntry *const entry = VirtFsDir::searchEntryByPath(filename);
        if (entry == nullptr)
            return VirtFsZip::openReadInternal(filename);
        return VirtFsDir::openReadDirEntry(entry, filename);
    }

    VirtFile *openWrite(const std::string &restrict filename)
    {
        return VirtFsDir::openWrite(filename);
    }

    VirtFile *openAppend(const std::string &restrict filename)
    {
        return VirtFsDir::openAppend(filename);
    }

    bool setWriteDir(const std::string &restrict newDir)
    {
        return VirtFsDir::setWriteDir(newDir);
    }

    bool addDirToSearchPath(const std::string &restrict newDir,
                            const Append append)
    {
        return VirtFsDir::addToSearchPath(newDir, append);
    }

    bool removeDirFromSearchPath(const std::string &restrict oldDir)
    {
        return VirtFsDir::removeFromSearchPath(oldDir);
    }

    bool addZipToSearchPath(const std::string &restrict newDir,
                            const Append append)
    {
        return VirtFsZip::addToSearchPath(newDir, append);
    }

    bool removeZipFromSearchPath(const std::string &restrict oldDir)
    {
        return VirtFsZip::removeFromSearchPath(oldDir);
    }

    std::string getRealDir(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::getRealDir invalid path: %s",
                filename.c_str());
            return std::string();
        }
        VirtDirEntry *const entry = VirtFsDir::searchEntryByPath(filename);
        if (entry == nullptr)
            return VirtFsZip::getRealDirInternal(filename);
        return entry->mUserDir;
    }

    bool mkdir(const std::string &restrict dirname)
    {
        return VirtFsDir::mkdir(dirname);
    }

    bool remove(const std::string &restrict filename)
    {
        return VirtFsDir::remove(filename);
    }

    bool deinit()
    {
        VirtFsDir::deinit();
        return true;
    }

    void permitLinks(const bool val)
    {
        VirtFsDir::permitLinks(val);
    }

    const char *getLastError()
    {
        return "";
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
