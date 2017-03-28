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

#include "fs/files.h"
#include "fs/paths.h"
#include "fs/virtfile.h"
#include "fs/virtfsfuncs.h"
#include "fs/virtlist.h"

#include "fs/virtfs/virtdirentry.h"
#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/virtfsdir.h"
#include "fs/virtfs/virtfszip.h"
#include "fs/virtfs/virtzipentry.h"
#include "fs/virtfs/zip.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include "debug.h"

const char *dirSeparator = nullptr;

namespace
{
    std::vector<VirtFsEntry*> mEntries;
}  // namespace

#ifdef UNITTESTS
#define reportNonTests logger->log
#else  // UNITTESTS
#define reportNonTests reportAlways
#endif  // UNITTESTS

namespace VirtFs
{
    void init(const std::string &restrict name)
    {
        VirtFsDir::init(name);
        VirtFsZip::init();
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

    std::vector<VirtFsEntry*> &getEntries()
    {
        return mEntries;
    }

    VirtFsEntry *searchEntryByRootInternal(const std::string &restrict root)
    {
        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            if ((*it)->root == root)
                return *it;
        }
        return nullptr;
    }

    VirtFsEntry *searchEntryInternal(const std::string &restrict root,
                                     const FsEntryTypeT type)
    {
        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            const VirtFsEntry *const entry = *it;
            if (entry->root == root &&
                entry->type == type)
            {
                return *it;
            }
        }
        return nullptr;
    }

    bool exists(std::string name)
    {
        prepareFsPath(name);
        if (checkPath(name) == false)
        {
            reportAlways("VirtFsDir::exists invalid path: %s",
                name.c_str());
            return false;
        }

        std::string rootDir = name;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            if (entry->funcs->exists(entry, name, rootDir) == true)
                return true;
        }
        return false;
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

        std::string rootDir = dirName;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;
        StringVect &names = list->names;

        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            entry->funcs->enumerate(entry, rootDir, names);
        }

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
        std::string dirName = name;
        if (findLast(dirName, std::string(dirSeparator)) == false)
            dirName += dirSeparator;

        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            bool isDirFlag(false);
            if (entry->funcs->isDirectory(entry, dirName, isDirFlag) == true)
            {
                return isDirFlag;
            }
        }
        return false;
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
        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            VirtFile *const file = entry->funcs->openRead(entry, filename);
            if (file != nullptr)
                return file;
        }
        return nullptr;
    }

    VirtFile *openWrite(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::openWrite invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            VirtFile *const file = entry->funcs->openWrite(entry, filename);
            if (file != nullptr)
                return file;
        }
        return nullptr;
    }

    VirtFile *openAppend(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::openAppend invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            VirtFile *const file = entry->funcs->openAppend(entry, filename);
            if (file != nullptr)
                return file;
        }
        return nullptr;
    }

    bool setWriteDir(const std::string &restrict newDir)
    {
        return VirtFsDir::setWriteDir(newDir);
    }

    void addEntry(VirtFsEntry *const entry,
                  const Append append)
    {
        if (append == Append_true)
            mEntries.push_back(entry);
        else
            mEntries.insert(mEntries.begin(), entry);
    }

    bool mountDirInternal(const std::string &restrict newDir,
                          const Append append)
    {
        if (newDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called VirtFsDir::addToSearchPath with zip archive");
            return false;
        }
        std::string rootDir = newDir;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;
        const VirtFsEntry *const entry = searchEntryByRootInternal(rootDir);
        if (entry != nullptr)
        {
            reportAlways("VirtFs::addToSearchPath already exists: %s",
                newDir.c_str());
            return false;
        }
        logger->log("Add virtual directory: " + newDir);
        addEntry(new VirtDirEntry(newDir, rootDir, VirtFsDir::getFuncs()),
            append);
        return true;
    }

    bool mountDir(std::string newDir,
                  const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            reportNonTests("VirtFs::addToSearchPath directory not exists: %s",
                newDir.c_str());
            return false;
        }
        return mountDirInternal(newDir, append);
    }

    bool mountDirSilent(std::string newDir,
                        const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFs::addToSearchPath directory not exists: %s",
                newDir.c_str());
            return false;
        }
        return mountDirInternal(newDir, append);
    }

#ifdef UNITTESTS
    bool mountDirSilent2(std::string newDir,
                         const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFs::addToSearchPath directory not exists: %s",
                newDir.c_str());
        }
        return mountDirInternal(newDir, append);
    }
#endif  // UNITTESTS

    bool unmountDirInternal(std::string oldDir)
    {
        if (findLast(oldDir, std::string(dirSeparator)) == false)
            oldDir += dirSeparator;
        FOR_EACH (std::vector<VirtFsEntry*>::iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            if (entry->root == oldDir &&
                entry->type == FsEntryType::Dir)
            {
                VirtDirEntry *const dirEntry = static_cast<VirtDirEntry*>(
                    entry);
                    logger->log("Remove virtual directory: " + oldDir);
                    mEntries.erase(it);
                    delete dirEntry;
                    return true;
            }
        }
        return false;
    }

    bool unmountDir(std::string oldDir)
    {
        prepareFsPath(oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called removeFromSearchPath with zip archive");
            return false;
        }
        if (unmountDirInternal(oldDir) == false)
        {
            reportAlways("VirtFs::unmountDir not exists: %s",
                oldDir.c_str());
            return false;
        }
        return true;
    }

    bool unmountDirSilent(std::string oldDir)
    {
        prepareFsPath(oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called removeFromSearchPath with zip archive");
            return false;
        }
        if (unmountDirInternal(oldDir) == false)
        {
            logger->log("VirtFs::unmountDir not exists: %s",
                oldDir.c_str());
            return false;
        }
        return true;
    }

    bool mountZip(std::string newDir,
                  const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            reportNonTests("VirtFsZip::addToSearchPath file not exists: %s",
                newDir.c_str());
            return false;
        }
        if (findLast(newDir, ".zip") == false)
        {
            reportAlways("Called VirtFs::addToSearchPath without "
                "zip archive");
            return false;
        }
        if (searchEntryByRootInternal(newDir) != nullptr)
        {
            reportAlways("VirtFsZip::addToSearchPath already exists: %s",
                newDir.c_str());
            return false;
        }
        VirtZipEntry *const entry = new VirtZipEntry(newDir,
            VirtFsZip::getFuncs());
        if (Zip::readArchiveInfo(entry) == false)
        {
            delete entry;
            return false;
        }

        logger->log("Add virtual zip: " + newDir);
        addEntry(entry, append);
        return true;
    }

    bool unmountZip(std::string oldDir)
    {
        prepareFsPath(oldDir);
        if (findLast(oldDir, ".zip") == false)
        {
            reportAlways("Called removeFromSearchPath without zip archive");
            return false;
        }
        FOR_EACH (std::vector<VirtFsEntry*>::iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            if (entry->root == oldDir &&
                entry->type == FsEntryType::Zip)
            {
                VirtZipEntry *const zipEntry = static_cast<VirtZipEntry*>(
                    entry);
                logger->log("Remove virtual zip: " + oldDir);
                mEntries.erase(it);
                delete zipEntry;
                return true;
            }
        }

        reportAlways("VirtFs::unmountZip not exists: %s",
            oldDir.c_str());
        return false;
    }

    std::string getRealDir(std::string fileName)
    {
        prepareFsPath(fileName);
        if (checkPath(fileName) == false)
        {
            reportAlways("VirtFsDir::getRealDir invalid path: %s",
                fileName.c_str());
            return std::string();
        }

        std::string rootDir = fileName;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (std::vector<VirtFsEntry*>::const_iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            std::string realDir;
            if (entry->funcs->getRealDir(entry,
                fileName,
                rootDir,
                realDir) == true)
            {
                return realDir;
            }
        }
        return std::string();
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
        VirtFsZip::deinit();
        FOR_EACH (std::vector<VirtFsEntry*>::iterator, it, mEntries)
        {
            VirtFsEntry *const entry = *it;
            if (entry->type == FsEntryType::Dir)
                delete static_cast<VirtDirEntry*>(entry);
            else if (entry->type == FsEntryType::Zip)
                delete static_cast<VirtZipEntry*>(entry);
            else
                delete entry;
        }
        mEntries.clear();
        return true;
    }

    void permitLinks(const bool val)
    {
        VirtFsDir::permitLinks(val);
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
