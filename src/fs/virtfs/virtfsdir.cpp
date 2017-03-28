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

#include "fs/virtfs/virtfsdir.h"

#include "fs/files.h"
#include "fs/mkdir.h"
#include "fs/paths.h"

#include "fs/virtfs/virtdirentry.h"
#include "fs/virtfs/virtfile.h"
#include "fs/virtfs/virtfileprivate.h"
#include "fs/virtfsfuncs.h"
#include "fs/virtlist.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include <dirent.h>
#include <unistd.h>

#ifdef USE_FILE_FOPEN
#include <stdio.h>
#endif  // USE_FILE_FOPEN

#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

extern const char *dirSeparator;

namespace
{
    std::string mWriteDir;
    std::string mBaseDir;
    std::string mUserDir;
    bool mPermitLinks = false;
    VirtFsFuncs funcs;
}  // namespace

namespace VirtFsDir
{
    VirtFile *openInternal(VirtFsEntry *restrict const entry,
                           const std::string &filename,
                           const FILEMTYPE mode)
    {
        const std::string path = entry->root + filename;
        if (Files::existsLocal(path) == false)
            return nullptr;
        FILEHTYPE fd = FILEOPEN(path.c_str(),
            mode);
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFs::open file open error: %s",
                filename.c_str());
            return nullptr;
        }
        VirtFile *restrict const file = new VirtFile(&funcs);
        file->mPrivate = new VirtFilePrivate(fd);

        return file;
    }

    VirtFile *openRead(VirtFsEntry *restrict const entry,
                       const std::string &filename)
    {
        return openInternal(entry, filename, FILEOPEN_FLAG_READ);
    }

    VirtFile *openWrite(VirtFsEntry *restrict const entry,
                        const std::string &filename)
    {
        return openInternal(entry, filename, FILEOPEN_FLAG_WRITE);
    }

    VirtFile *openAppend(VirtFsEntry *restrict const entry,
                         const std::string &filename)
    {
        return openInternal(entry, filename, FILEOPEN_FLAG_APPEND);
    }

    void deinit()
    {
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
        ptr->exists = &VirtFsDir::exists;
        ptr->getRealDir = &VirtFsDir::getRealDir;
        ptr->enumerate = &VirtFsDir::enumerate;
        ptr->isDirectory = &VirtFsDir::isDirectory;
        ptr->openRead = &VirtFsDir::openRead;
        ptr->openWrite = &VirtFsDir::openWrite;
        ptr->openAppend = &VirtFsDir::openAppend;
    }

    VirtFsFuncs *getFuncs()
    {
        return &funcs;
    }

    const char *getBaseDir()
    {
        return mBaseDir.c_str();
    }

    const char *getUserDir()
    {
        return mUserDir.c_str();
    }

    bool getRealDir(VirtFsEntry *restrict const entry,
                    const std::string &filename,
                    const std::string &dirName A_UNUSED,
                    std::string &realDir)
    {
        VirtDirEntry *const dirEntry = static_cast<VirtDirEntry*>(entry);
        if (Files::existsLocal(dirEntry->root + filename))
        {
            realDir = dirEntry->userDir;
            return true;
        }
        return false;
    }

    bool exists(VirtFsEntry *restrict const entry,
                const std::string &fileName,
                const std::string &dirName A_UNUSED)
    {
        return Files::existsLocal(entry->root + fileName);
    }

    void enumerate(VirtFsEntry *restrict const entry,
                   const std::string &dirName,
                   StringVect &names)
    {
        const std::string path = entry->root + dirName;
        const struct dirent *next_file = nullptr;
        DIR *const dir = opendir(path.c_str());
        if (dir)
        {
            while ((next_file = readdir(dir)))
            {
                const std::string file = next_file->d_name;
                if (file == "." || file == "..")
                    continue;
#ifndef WIN32
                if (mPermitLinks == false)
                {
                    struct stat statbuf;
                    if (lstat(path.c_str(), &statbuf) == 0 &&
                        S_ISLNK(statbuf.st_mode) != 0)
                    {
                        continue;
                    }
                }
#endif  // WIN32

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

    bool isDirectory(VirtFsEntry *restrict const entry,
                     const std::string &dirName,
                     bool &isDirFlag)
    {
        std::string path = entry->root + dirName;

        struct stat statbuf;
        if (stat(path.c_str(), &statbuf) == 0)
        {
            isDirFlag = (S_ISDIR(statbuf.st_mode) != 0);
            return true;
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
#ifndef WIN32
        if (mPermitLinks == false)
            return false;

        struct stat statbuf;
        return lstat(name.c_str(), &statbuf) == 0 &&
            S_ISLNK(statbuf.st_mode) != 0;
#else
        return false;
#endif  // WIN32
    }

    void freeList(VirtList *restrict const handle)
    {
        delete handle;
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
        FILEHTYPE fd = file->mPrivate->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFsDir::read file not opened.");
            return 0;
        }
#ifdef USE_FILE_FOPEN
        return fread(buffer, objSize, objCount, fd);
#else  // USE_FILE_FOPEN
        int max = objSize * objCount;
        int cnt = ::read(fd, buffer, max);
        if (cnt <= 0)
            return cnt;
        return cnt / objSize;
#endif  // USE_FILE_FOPEN
    }

    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount)
    {
        if (file == nullptr)
            return 0;
        FILEHTYPE fd = file->mPrivate->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFsDir::write file not opened.");
            return 0;
        }
#ifdef USE_FILE_FOPEN
        return fwrite(buffer, objSize, objCount, fd);
#else  // USE_FILE_FOPEN
        int max = objSize * objCount;
        int cnt = ::write(fd, buffer, max);
        if (cnt <= 0)
            return cnt;
        return cnt / objSize;
#endif  // USE_FILE_FOPEN
    }

    int64_t fileLength(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;
        FILEHTYPE fd = file->mPrivate->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFsDir::fileLength file not opened.");
            return 0;
        }
#ifdef USE_FILE_FOPEN
        const long pos = ftell(fd);
        fseek(fd, 0, SEEK_END);
        const long sz = ftell(fd);
        fseek(fd, pos, SEEK_SET);
        return sz;
#else  // USE_FILE_FOPEN
        struct stat statbuf;
        if (fstat(fd, &statbuf) == -1)
        {
            reportAlways("VirtFsDir::fileLength error.");
            return -1;
        }
        return static_cast<int64_t>(statbuf.st_size);
#endif  // USE_FILE_FOPEN
    }

    int64_t tell(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        FILEHTYPE fd = file->mPrivate->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFsDir::tell file not opened.");
            return 0;
        }
#ifdef USE_FILE_FOPEN
        const int64_t pos = ftell(fd);
#else  // USE_FILE_FOPEN
        const int64_t pos = lseek(fd, 0, SEEK_CUR);
#endif  // USE_FILE_FOPEN
        return pos;
    }

    int seek(VirtFile *restrict const file,
             const uint64_t pos)
    {
        if (file == nullptr)
            return 0;

        FILEHTYPE fd = file->mPrivate->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFsDir::seek file not opened.");
            return 0;
        }
        const int64_t res = FILESEEK(fd, pos, SEEK_SET);
        if (res == -1)
            return 0;
        return 1;
    }

    int eof(VirtFile *restrict const file)
    {
        if (file == nullptr)
            return -1;

        FILEHTYPE fd = file->mPrivate->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFsDir::eof file not opened.");
            return 0;
        }
#ifdef USE_FILE_FOPEN
        const int flag = feof(fd);
        if (flag != 0)
            return 1;
        const int64_t pos = ftell(fd);
        const int64_t len = fileLength(file);
#else  // USE_FILE_FOPEN
        const int64_t pos = lseek(fd, 0, SEEK_CUR);
        struct stat statbuf;
        if (fstat(fd, &statbuf) == -1)
        {
            reportAlways("VirtFsDir::fileLength error.");
            return -1;
        }
        const int64_t len = static_cast<int64_t>(statbuf.st_size);
#endif  // USE_FILE_FOPEN
        return pos < 0 || len < 0 || pos >= len;
    }
}  // namespace VirtFs
