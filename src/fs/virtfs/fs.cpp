/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2018  The ManaPlus Developers
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

#include "fs/virtfs/fs.h"

#include "fs/files.h"
#include "fs/paths.h"

#include "fs/virtfs/direntry.h"
#include "fs/virtfs/file.h"
#include "fs/virtfs/fsdir.h"
#include "fs/virtfs/fsfuncs.h"
#include "fs/virtfs/fszip.h"
#include "fs/virtfs/list.h"
#include "fs/virtfs/zipentry.h"
#include "fs/virtfs/zipreader.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/stdmove.h"
#include "utils/stringutils.h"

#include "debug.h"

const char *dirSeparator = nullptr;

#ifdef UNITTESTS
#define reportNonTests logger->log
#else  // UNITTESTS
#define reportNonTests reportAlways
#endif  // UNITTESTS

namespace VirtFs
{
    namespace
    {
        STD_VECTOR<FsEntry*> mEntries;
    }  // namespace

    void init(const std::string &restrict name)
    {
        updateDirSeparator();
        FsDir::init(name);
        FsZip::init();
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
        return FsDir::getBaseDir();
    }

    const char *getUserDir()
    {
        return FsDir::getUserDir();
    }

    STD_VECTOR<FsEntry*> &getEntries()
    {
        return mEntries;
    }

    FsEntry *searchByRootInternal(const std::string &restrict root,
                                  const std::string &restrict subDir)
    {
        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            const FsEntry *const entry = *it;
            if (entry->root == root &&
                entry->subDir == subDir)
            {
                return *it;
            }
        }
        return nullptr;
    }

    FsEntry *searchByTypeInternal(const std::string &restrict root,
                                  const FsEntryTypeT type)
    {
        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            const FsEntry *const entry = *it;
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
            reportAlways("FsDir::exists invalid path: %s",
                name.c_str());
            return false;
        }

        std::string rootDir = name;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            if (entry->funcs->exists(entry, name, rootDir) == true)
                return true;
        }
        return false;
    }

    List *enumerateFiles(std::string dirName)
    {
        List *const list = new List;
        prepareFsPath(dirName);
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFs::enumerateFiles invalid path: %s",
                dirName.c_str());
            return list;
        }

        std::string rootDir = STD_MOVE(dirName);
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;
        StringVect &names = list->names;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            entry->funcs->enumerate(entry, rootDir, names);
        }

        return list;
    }

    void getFiles(std::string dirName,
                  StringVect &list)
    {
        prepareFsPath(dirName);
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFs::enumerateFiles invalid path: %s",
                dirName.c_str());
            return;
        }

        std::string rootDir = STD_MOVE(dirName);
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            entry->funcs->getFiles(entry, rootDir, list);
        }
    }

    void getFilesWithDir(std::string dirName,
                         StringVect &list)
    {
        prepareFsPath(dirName);
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFs::enumerateFiles invalid path: %s",
                dirName.c_str());
            return;
        }

        std::string rootDir = STD_MOVE(dirName);
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            entry->funcs->getFilesWithDir(entry, rootDir, list);
        }
    }


    void getDirs(std::string dirName,
                 StringVect &list)
    {
        prepareFsPath(dirName);
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFs::enumerateFiles invalid path: %s",
                dirName.c_str());
            return;
        }

        std::string rootDir = STD_MOVE(dirName);
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            entry->funcs->getDirs(entry, rootDir, list);
        }
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
        std::string dirName = STD_MOVE(name);
        if (findLast(dirName, std::string(dirSeparator)) == false)
            dirName += dirSeparator;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
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
        return FsDir::isSymbolicLink(name);
    }

    void freeList(List *restrict const handle)
    {
        delete handle;
    }

    File *openRead(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::openRead invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            File *const file = entry->funcs->openRead(entry, filename);
            if (file != nullptr)
                return file;
        }
        return nullptr;
    }

    File *openWrite(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::openWrite invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            File *const file = entry->funcs->openWrite(entry, filename);
            if (file != nullptr)
                return file;
        }
        return nullptr;
    }

    File *openAppend(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::openAppend invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            File *const file = entry->funcs->openAppend(entry, filename);
            if (file != nullptr)
                return file;
        }
        return nullptr;
    }

    bool setWriteDir(const std::string &restrict newDir)
    {
        return FsDir::setWriteDir(newDir);
    }

    void addEntry(FsEntry *const entry,
                  const Append append)
    {
        if (append == Append_true)
            mEntries.push_back(entry);
        else
            mEntries.insert(mEntries.begin(), entry);
    }

    bool mountDirInternal(const std::string &restrict newDir,
                          std::string subDir,
                          const Append append)
    {
        if (newDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called FsDir::mount with zip archive");
            return false;
        }
        std::string rootDir = newDir;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;
        if (subDir == dirSeparator)
        {
            subDir.clear();
        }
        else if (!subDir.empty() &&
                 findLast(subDir, std::string(dirSeparator)) == false)
        {
            subDir += dirSeparator;
        }
        const FsEntry *const entry = searchByRootInternal(rootDir, subDir);
        if (entry != nullptr)
        {
            reportAlways("VirtFs::mount already exists: %s",
                newDir.c_str());
            return false;
        }
        if (subDir.empty())
        {
            logger->log("Add virtual directory: " + newDir);
        }
        else
        {
            logger->log("Add virtual directory: %s with dir %s",
                newDir.c_str(),
                subDir.c_str());
        }

        addEntry(new DirEntry(newDir,
            rootDir,
            subDir,
            rootDir + subDir,
            FsDir::getFuncs()),
            append);
        return true;
    }

    bool mountDir(std::string newDir,
                  const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            reportNonTests("VirtFs::mount directory not exists: %s",
                newDir.c_str());
            return false;
        }
        return mountDirInternal(newDir, dirSeparator, append);
    }

    bool mountDir2(std::string newDir,
                   std::string subDir,
                   const Append append)
    {
        prepareFsPath(newDir);
        prepareFsPath(subDir);
        if (Files::existsLocal(newDir) == false)
        {
            reportNonTests("VirtFs::mount directory not exists: %s",
                newDir.c_str());
            return false;
        }
        return mountDirInternal(newDir, subDir, append);
    }

    bool mountDirSilent(std::string newDir,
                        const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFs::mount directory not exists: %s",
                newDir.c_str());
            return false;
        }
        return mountDirInternal(newDir, std::string(), append);
    }

    bool mountDirSilent2(std::string newDir,
                         std::string subDir,
                         const Append append)
    {
        prepareFsPath(newDir);
        prepareFsPath(subDir);
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFs::mount directory not exists: %s",
                newDir.c_str());
            return false;
        }
        return mountDirInternal(newDir, subDir, append);
    }

#ifdef UNITTESTS
    bool mountDirSilentTest(std::string newDir,
                            const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFs::mount directory not exists: %s",
                newDir.c_str());
        }
        return mountDirInternal(newDir, std::string(), append);
    }

    bool mountDirSilentTest2(std::string newDir,
                             std::string subDir,
                             const Append append)
    {
        prepareFsPath(newDir);
        prepareFsPath(subDir);
        if (Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFs::mount directory not exists: %s",
                newDir.c_str());
        }
        return mountDirInternal(newDir, subDir, append);
    }
#endif  // UNITTESTS

    bool unmountDirInternal(std::string oldDir,
                            std::string subDir)
    {
        if (findLast(oldDir, std::string(dirSeparator)) == false)
            oldDir += dirSeparator;
        if (subDir == dirSeparator)
        {
            subDir.clear();
        }
        else if (!subDir.empty() &&
                 findLast(subDir, std::string(dirSeparator)) == false)
        {
            subDir += dirSeparator;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            if (entry->root == oldDir &&
                entry->type == FsEntryType::Dir &&
                entry->subDir == subDir)
            {
                DirEntry *const dirEntry = static_cast<DirEntry*>(
                    entry);
                if (subDir.empty())
                {
                    logger->log("Remove virtual directory: " + oldDir);
                }
                else
                {
                    logger->log("Remove virtual directory: %s with dir %s",
                        oldDir.c_str(),
                        subDir.c_str());
                }
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
            reportAlways("Called unmount with zip archive");
            return false;
        }
        if (unmountDirInternal(oldDir, std::string()) == false)
        {
            reportAlways("VirtFs::unmountDir not exists: %s",
                oldDir.c_str());
            return false;
        }
        return true;
    }

    bool unmountDir2(std::string oldDir,
                     std::string subDir)
    {
        prepareFsPath(oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called unmount with zip archive");
            return false;
        }
        prepareFsPath(subDir);
        if (unmountDirInternal(oldDir, subDir) == false)
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
            reportAlways("Called unmount with zip archive");
            return false;
        }
        if (unmountDirInternal(oldDir, std::string()) == false)
        {
            logger->log("VirtFs::unmountDir not exists: %s",
                oldDir.c_str());
            return false;
        }
        return true;
    }

    bool unmountDirSilent2(std::string oldDir,
                           std::string subDir)
    {
        prepareFsPath(oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called unmount with zip archive");
            return false;
        }
        prepareFsPath(subDir);
        if (unmountDirInternal(oldDir, subDir) == false)
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
            reportNonTests("FsZip::mount file not exists: %s",
                newDir.c_str());
            return false;
        }
        if (findLast(newDir, ".zip") == false)
        {
            reportAlways("Called VirtFs::mount without "
                "zip archive");
            return false;
        }
        if (searchByRootInternal(newDir, std::string()) != nullptr)
        {
            reportAlways("FsZip::mount already exists: %s",
                newDir.c_str());
            return false;
        }
        ZipEntry *const entry = new ZipEntry(newDir,
            std::string(),
            FsZip::getFuncs());
        if (ZipReader::readArchiveInfo(entry) == false)
        {
            delete entry;
            return false;
        }

        logger->log("Add virtual zip: " + newDir);
        addEntry(entry, append);
        return true;
    }

    bool mountZip2(std::string newDir,
                   std::string subDir,
                   const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            reportNonTests("FsZip::mount file not exists: %s",
                newDir.c_str());
            return false;
        }
        if (findLast(newDir, ".zip") == false)
        {
            reportAlways("Called VirtFs::mount without "
                "zip archive");
            return false;
        }
        prepareFsPath(subDir);
        if (subDir == dirSeparator)
        {
            subDir.clear();
        }
        else if (!subDir.empty() &&
                 findLast(subDir, std::string(dirSeparator)) == false)
        {
            subDir += dirSeparator;
        }
        if (searchByRootInternal(newDir, subDir) != nullptr)
        {
            reportAlways("FsZip::mount already exists: %s",
                newDir.c_str());
            return false;
        }
        ZipEntry *const entry = new ZipEntry(newDir,
            subDir,
            FsZip::getFuncs());
        if (ZipReader::readArchiveInfo(entry) == false)
        {
            delete entry;
            return false;
        }

        logger->log("Add virtual zip: %s with dir %s",
            newDir.c_str(),
            subDir.c_str());
        addEntry(entry, append);
        return true;
    }

    bool unmountZip(std::string oldDir)
    {
        prepareFsPath(oldDir);
        if (findLast(oldDir, ".zip") == false)
        {
            reportAlways("Called unmount without zip archive");
            return false;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            if (entry->root == oldDir &&
                entry->type == FsEntryType::Zip &&
                entry->subDir.empty())
            {
                ZipEntry *const zipEntry = static_cast<ZipEntry*>(
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

    bool unmountZip2(std::string oldDir,
                     std::string subDir)
    {
        prepareFsPath(oldDir);
        if (findLast(oldDir, ".zip") == false)
        {
            reportAlways("Called unmount without zip archive");
            return false;
        }
        prepareFsPath(subDir);
        if (subDir == dirSeparator)
        {
            subDir.clear();
        }
        else if (!subDir.empty() &&
                 findLast(subDir, std::string(dirSeparator)) == false)
        {
            subDir += dirSeparator;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            if (entry->root == oldDir &&
                entry->type == FsEntryType::Zip &&
                entry->subDir == subDir)
            {
                ZipEntry *const zipEntry = static_cast<ZipEntry*>(
                    entry);
                logger->log("Remove virtual zip: %s with dir %s",
                    oldDir.c_str(),
                    subDir.c_str());
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
            reportAlways("FsDir::getRealDir invalid path: %s",
                fileName.c_str());
            return std::string();
        }

        std::string rootDir = fileName;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;

        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
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
        return FsDir::mkdir(dirname);
    }

    bool remove(const std::string &restrict filename)
    {
        return FsDir::remove(filename);
    }

    bool deinit()
    {
        FsDir::deinit();
        FsZip::deinit();
        FOR_EACH (STD_VECTOR<FsEntry*>::iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            if (entry->type == FsEntryType::Dir)
                delete static_cast<DirEntry*>(entry);
            else if (entry->type == FsEntryType::Zip)
                delete static_cast<ZipEntry*>(entry);
            else
                delete entry;
        }
        mEntries.clear();
        return true;
    }

    void permitLinks(const bool val)
    {
        FsDir::permitLinks(val);
    }

    int close(File *restrict const file)
    {
        if (file == nullptr)
            return 0;
        return file->funcs->close(file);
    }

    int64_t read(File *restrict const file,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount)
    {
        return file->funcs->read(file,
            buffer,
            objSize,
            objCount);
    }

    int64_t write(File *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount)
    {
        return file->funcs->write(file,
            buffer,
            objSize,
            objCount);
    }

    int64_t fileLength(File *restrict const file)
    {
        return file->funcs->fileLength(file);
    }

    int64_t tell(File *restrict const file)
    {
        return file->funcs->tell(file);
    }

    int seek(File *restrict const file,
             const uint64_t pos)
    {
        return file->funcs->seek(file,
            pos);
    }

    int eof(File *restrict const file)
    {
        return file->funcs->eof(file);
    }

    const char *loadFile(std::string filename,
                         int &restrict fileSize)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFs::loadFile invalid path: %s",
                filename.c_str());
            return nullptr;
        }
        FOR_EACH (STD_VECTOR<FsEntry*>::const_iterator, it, mEntries)
        {
            FsEntry *const entry = *it;
            const char *const buf = entry->funcs->loadFile(entry,
                filename,
                fileSize);
            if (buf != nullptr)
                return buf;
        }
        return nullptr;
    }

}  // namespace VirtFs
