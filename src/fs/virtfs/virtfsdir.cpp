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

#ifndef USE_PHYSFS

#include "fs/virtfs/virtfsdir.h"

#include "fs/files.h"
#include "fs/mkdir.h"
#include "fs/paths.h"
#include "fs/virtfs.h"
#include "fs/virtfile.h"
#include "fs/virtfsfuncs.h"
#include "fs/virtlist.h"

#include "fs/virtfs/virtdirentry.h"
#include "fs/virtfs/virtfileprivate.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/stringutils.h"

#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

extern const char *dirSeparator;

namespace
{
    std::vector<VirtDirEntry*> mEntries;
    std::string mWriteDir;
    std::string mBaseDir;
    std::string mUserDir;
    bool mPermitLinks = false;
    VirtFsFuncs funcs;
}  // namespace

namespace VirtFsDir
{
    namespace
    {
        static VirtFile *openFile(std::string filename,
                                  const int mode)
        {
            prepareFsPath(filename);
            if (checkPath(filename) == false)
            {
                reportAlways("VirtFsDir::openFile invalid path: %s",
                    filename.c_str());
                return nullptr;
            }
            VirtDirEntry *const entry = searchEntryByPath(filename);
            if (entry == nullptr)
                return nullptr;

            const std::string path = entry->root + filename;
            const int fd = open(path.c_str(),
                mode,
                S_IRUSR | S_IWUSR);
            if (fd == -1)
            {
                reportAlways("VirtFsDir::openFile file open error: %s",
                    filename.c_str());
                return nullptr;
            }
            VirtFile *restrict const file = new VirtFile(&funcs);
            file->mPrivate = new VirtFilePrivate(fd);

            return file;
        }
    }  // namespace

    VirtFile *openReadDirEntry(VirtDirEntry *const entry,
                               const std::string &filename)
    {
        const std::string path = entry->root + filename;
        const int fd = open(path.c_str(),
            O_RDONLY,
            S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            reportAlways("VirtFsDir::openReadDirEntry file open error: %s",
                filename.c_str());
            return nullptr;
        }
        VirtFile *restrict const file = new VirtFile(&funcs);
        file->mPrivate = new VirtFilePrivate(fd);

        return file;
    }

    VirtDirEntry *searchEntryByRoot(const std::string &restrict root)
    {
        FOR_EACH (std::vector<VirtDirEntry*>::const_iterator, it, mEntries)
        {
            if ((*it)->root == root)
                return *it;
        }
        return nullptr;
    }

    VirtDirEntry *searchEntryByPath(const std::string &restrict path)
    {
        FOR_EACH (std::vector<VirtDirEntry*>::const_iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            if (Files::existsLocal(entry->root + path))
                return entry;
        }
        return nullptr;
    }

    bool addToSearchPathSilent(std::string newDir,
                               const Append append,
                               const SkipError skipError)
    {
        prepareFsPath(newDir);
        if (skipError == SkipError_false &&
            Files::existsLocal(newDir) == false)
        {
            logger->log("VirtFsDir::addToSearchPath directory not exists: %s",
                newDir.c_str());
            return false;
        }
        if (newDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called VirtFsDir::addToSearchPath with zip archive");
            return false;
        }
        std::string rootDir = newDir;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;
        VirtDirEntry *const entry = VirtFsDir::searchEntryByRoot(rootDir);
        if (entry != nullptr)
        {
            reportAlways("VirtFsDir::addToSearchPath already exists: %s",
                newDir.c_str());
            return false;
        }
        logger->log("Add virtual directory: " + newDir);
        if (append == Append_true)
        {
            mEntries.push_back(new VirtDirEntry(newDir,
                rootDir));
        }
        else
        {
            mEntries.insert(mEntries.begin(),
                new VirtDirEntry(newDir,
                rootDir));
        }
        return true;
    }

    bool addToSearchPath(std::string newDir,
                         const Append append)
    {
        prepareFsPath(newDir);
        if (Files::existsLocal(newDir) == false)
        {
            reportAlways("VirtFsDir::addToSearchPath directory not exists: %s",
                newDir.c_str());
            return false;
        }
        if (newDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called VirtFsDir::addToSearchPath with zip archive");
            return false;
        }
        std::string rootDir = newDir;
        if (findLast(rootDir, std::string(dirSeparator)) == false)
            rootDir += dirSeparator;
        VirtDirEntry *const entry = VirtFsDir::searchEntryByRoot(rootDir);
        if (entry != nullptr)
        {
            reportAlways("VirtFsDir::addToSearchPath already exists: %s",
                newDir.c_str());
            return false;
        }
        logger->log("Add virtual directory: " + newDir);
        if (append == Append_true)
        {
            mEntries.push_back(new VirtDirEntry(newDir,
                rootDir));
        }
        else
        {
            mEntries.insert(mEntries.begin(),
                new VirtDirEntry(newDir,
                rootDir));
        }
        return true;
    }

    bool removeFromSearchPathSilent(std::string oldDir)
    {
        prepareFsPath(oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called removeFromSearchPath with zip archive");
            return false;
        }
        if (findLast(oldDir, std::string(dirSeparator)) == false)
            oldDir += dirSeparator;
        FOR_EACH (std::vector<VirtDirEntry*>::iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            if (entry->root == oldDir)
            {
                logger->log("Remove virtual directory: " + oldDir);
                mEntries.erase(it);
                delete entry;
                return true;
            }
        }

        logger->log("VirtFsDir::removeFromSearchPath not exists: %s",
            oldDir.c_str());
        return false;
    }

    bool removeFromSearchPath(std::string oldDir)
    {
        prepareFsPath(oldDir);
        if (oldDir.find(".zip") != std::string::npos)
        {
            reportAlways("Called removeFromSearchPath with zip archive");
            return false;
        }
        if (findLast(oldDir, std::string(dirSeparator)) == false)
            oldDir += dirSeparator;
        FOR_EACH (std::vector<VirtDirEntry*>::iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            if (entry->root == oldDir)
            {
                logger->log("Remove virtual directory: " + oldDir);
                mEntries.erase(it);
                delete entry;
                return true;
            }
        }

        reportAlways("VirtFsDir::removeFromSearchPath not exists: %s",
            oldDir.c_str());
        return false;
    }

    std::vector<VirtDirEntry*> &getEntries()
    {
        return mEntries;
    }

    void deinit()
    {
        delete_all(mEntries);
        mEntries.clear();
    }

#if defined(__native_client__)
    void init(const std::string &restrict name A_UNUSED)
    {
        mBaseDir = "/";
#elif defined(ANDROID)
    void init(const std::string &restrict name A_UNUSED)
    {
        mBaseDir = getRealPath(".");
#else  // defined(__native_client__)

    void init(const std::string &restrict name)
    {
        mBaseDir = getRealPath(getFileDir(name));
#endif  // defined(__native_client__)

        prepareFsPath(mBaseDir);
        mUserDir = getHomePath();
        prepareFsPath(mUserDir);
        initFuncs(&funcs);
    }

    void initFuncs(VirtFsFuncs *restrict const ptr)
    {
        ptr->close = &VirtFsDir::close;
        ptr->read = &VirtFsDir::read;
        ptr->write = &VirtFsDir::write;
        ptr->fileLength = &VirtFsDir::fileLength;
        ptr->tell = &VirtFsDir::tell;
        ptr->seek = &VirtFsDir::seek;
        ptr->eof = &VirtFsDir::eof;
    }

    const char *getBaseDir()
    {
        return mBaseDir.c_str();
    }

    const char *getUserDir()
    {
        return mUserDir.c_str();
    }

    std::string getRealDir(std::string filename)
    {
        prepareFsPath(filename);
        if (checkPath(filename) == false)
        {
            reportAlways("VirtFsDir::exists invalid path: %s",
                filename.c_str());
            return std::string();
        }
        FOR_EACH (std::vector<VirtDirEntry*>::iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            const std::string path = entry->root + filename;
            if (Files::existsLocal(path))
                return entry->mUserDir;
        }
        return std::string();
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
        FOR_EACH (std::vector<VirtDirEntry*>::iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            if (Files::existsLocal(entry->root + name))
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
            reportAlways("VirtFsDir::enumerateFiles invalid path: %s",
                dirName.c_str());
            return list;
        }
        return enumerateFiles(dirName, list);
    }

    VirtList *enumerateFiles(const std::string &restrict dirName,
                             VirtList *restrict const list)
    {
        StringVect &names = list->names;
        FOR_EACH (std::vector<VirtDirEntry*>::iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            StringVect files;
            std::string path = entry->root + dirName;
            if (findLast(path, std::string(dirSeparator)) == false)
                path += dirSeparator;
            const struct dirent *next_file = nullptr;
            DIR *const dir = opendir(path.c_str());
            if (dir)
            {
                while ((next_file = readdir(dir)))
                {
                    const std::string file = next_file->d_name;
                    if (file == "." || file == "..")
                        continue;
                    if (mPermitLinks == false)
                    {
                        struct stat statbuf;
                        if (lstat(path.c_str(), &statbuf) == 0 &&
                            S_ISLNK(statbuf.st_mode) != 0)
                        {
                            continue;
                        }
                    }
                    bool found(false);
                    FOR_EACH (StringVectCIter, itn, names)
                    {
                        if (*itn == file)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found == false)
                        names.push_back(file);
                }
                closedir(dir);
            }
        }
        return list;
    }

    bool isDirectory(std::string dirName)
    {
        prepareFsPath(dirName);
        if (checkPath(dirName) == false)
        {
            reportAlways("VirtFsDir::isDirectory invalid path: %s",
                dirName.c_str());
            return false;
        }
        return isDirectoryInternal(dirName);
    }

    bool isDirectoryInternal(const std::string &restrict dirName)
    {
        FOR_EACH (std::vector<VirtDirEntry*>::iterator, it, mEntries)
        {
            VirtDirEntry *const entry = *it;
            std::string path = entry->root + dirName;
            if (findLast(path, std::string(dirSeparator)) == false)
                path += dirSeparator;

            struct stat statbuf;
            if (stat(path.c_str(), &statbuf) == 0 &&
                S_ISDIR(statbuf.st_mode) != 0)
            {
                return true;
            }
        }
        return false;
    }

    bool isSymbolicLink(std::string name)
    {
        prepareFsPath(name);
        if (checkPath(name) == false)
        {
            reportAlways("VirtFsDir::isSymbolicLink invalid path: %s",
                name.c_str());
            return false;
        }
        if (mPermitLinks == false)
            return false;

        struct stat statbuf;
        return lstat(name.c_str(), &statbuf) == 0 &&
            S_ISLNK(statbuf.st_mode) != 0;
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
    }

    VirtFile *openRead(const std::string &restrict filename)
    {
        return openFile(filename, O_RDONLY);
    }

    VirtFile *openWrite(const std::string &restrict filename)
    {
        return openFile(filename, O_WRONLY | O_CREAT | O_TRUNC);
    }

    VirtFile *openAppend(const std::string &restrict filename)
    {
        return openFile(filename, O_WRONLY | O_CREAT | O_APPEND);
    }

    bool setWriteDir(std::string newDir)
    {
        prepareFsPath(newDir);
        mWriteDir = newDir;
        if (findLast(mWriteDir, std::string(dirSeparator)) == false)
            mWriteDir += dirSeparator;
        return true;
    }

    bool mkdir(std::string dirname)
    {
        prepareFsPath(dirname);
        if (mWriteDir.empty())
        {
            reportAlways("VirtFsDir::mkdir write dir is empty");
            return false;
        }
        return mkdir_r((mWriteDir + dirname).c_str()) != -1;
    }

    bool remove(std::string filename)
    {
        prepareFsPath(filename);
        if (mWriteDir.empty())
        {
            reportAlways("VirtFsDir::remove write dir is empty");
            return false;
        }
        return ::remove((mWriteDir + filename).c_str()) != 0;
    }

    void permitLinks(const bool val)
    {
        mPermitLinks = val;
    }

    const char *getLastError()
    {
        return nullptr;
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
        const int fd = file->mPrivate->mFd;
        if (fd == -1)
        {
            reportAlways("VirtFsDir::read file not opened.");
            return 0;
        }
        int max = objSize * objCount;
        int cnt = ::read(fd, buffer, max);
        if (cnt <= 0)
            return cnt;
        return cnt / objSize;
    }

    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount)
    {
        if (file == nullptr)
            return 0;
        const int fd = file->mPrivate->mFd;
        if (fd == -1)
        {
            reportAlways("VirtFsDir::write file not opened.");
            return 0;
        }
        int max = objSize * objCount;
        int cnt = ::write(fd, buffer, max);
        if (cnt <= 0)
            return cnt;
        return cnt / objSize;
    }

    int64_t fileLength(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;
        const int fd = file->mPrivate->mFd;
        if (fd == -1)
        {
            reportAlways("VirtFsDir::fileLength file not opened.");
            return 0;
        }
        struct stat statbuf;
        if (fstat(fd, &statbuf) == -1)
        {
            reportAlways("VirtFsDir::fileLength error.");
            return -1;
        }
        return static_cast<int64_t>(statbuf.st_size);
    }

    int64_t tell(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        const int fd = file->mPrivate->mFd;
        if (fd == -1)
        {
            reportAlways("VirtFsDir::tell file not opened.");
            return 0;
        }
        const int64_t pos = lseek(fd, 0, SEEK_CUR);
        return pos;
    }

    int seek(VirtFile *restrict const file,
             const uint64_t pos)
    {
        if (file == nullptr)
            return 0;

        const int fd = file->mPrivate->mFd;
        if (fd == -1)
        {
            reportAlways("VirtFsDir::seek file not opened.");
            return 0;
        }
        const int64_t res = lseek(fd, pos, SEEK_SET);
        if (res == -1)
            return 0;
        return 1;
    }

    int eof(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        const int fd = file->mPrivate->mFd;
        if (fd == -1)
        {
            reportAlways("VirtFsDir::eof file not opened.");
            return 0;
        }
        const int64_t pos = lseek(fd, 0, SEEK_CUR);
        struct stat statbuf;
        if (fstat(fd, &statbuf) == -1)
        {
            reportAlways("VirtFsDir::fileLength error.");
            return -1;
        }
        const int64_t len = static_cast<int64_t>(statbuf.st_size);
        return pos < 0 || len < 0 || pos >= len;
    }
}  // namespace VirtFs
#endif  // USE_PHYSFS
