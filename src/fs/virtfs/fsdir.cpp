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

#include "fs/virtfs/fsdir.h"

#include "fs/files.h"
#include "fs/mkdir.h"
#include "fs/paths.h"

#include "fs/virtfs/direntry.h"
#include "fs/virtfs/file.h"
#include "fs/virtfs/fsdirrwops.h"
#include "fs/virtfs/fsfuncs.h"
#include "fs/virtfs/list.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/stdmove.h"
#include "utils/stringutils.h"

#include <dirent.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

extern const char *dirSeparator;

namespace VirtFs
{

namespace
{
    std::string mWriteDir;
    std::string mBaseDir;
    std::string mUserDir;
    bool mPermitLinks = false;
    FsFuncs funcs;
}  // namespace

namespace FsDir
{
    File *openInternal(FsEntry *restrict const entry,
                       const std::string &filename,
                       const FILEMTYPE mode)
    {
        const std::string path = static_cast<DirEntry*>(entry)->rootSubDir +
            filename;
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
        File *restrict const file = new File(&funcs, fd);
        return file;
    }

    File *openRead(FsEntry *restrict const entry,
                   std::string filename)
    {
        return openInternal(entry, filename, FILEOPEN_FLAG_READ);
    }

    File *openWrite(FsEntry *restrict const entry,
                    const std::string &filename)
    {
        return openInternal(entry, filename, FILEOPEN_FLAG_WRITE);
    }

    File *openAppend(FsEntry *restrict const entry,
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

    void initFuncs(FsFuncs *restrict const ptr)
    {
        ptr->close = &FsDir::close;
        ptr->read = &FsDir::read;
        ptr->write = &FsDir::write;
        ptr->fileLength = &FsDir::fileLength;
        ptr->tell = &FsDir::tell;
        ptr->seek = &FsDir::seek;
        ptr->eof = &FsDir::eof;
        ptr->exists = &FsDir::exists;
        ptr->getRealDir = &FsDir::getRealDir;
        ptr->enumerate = &FsDir::enumerate;
        ptr->isDirectory = &FsDir::isDirectory;
        ptr->openRead = &FsDir::openRead;
        ptr->openWrite = &FsDir::openWrite;
        ptr->openAppend = &FsDir::openAppend;
        ptr->loadFile = &FsDir::loadFile;
        ptr->getFiles = &FsDir::getFiles;
        ptr->getFilesWithDir = &FsDir::getFilesWithDir;
        ptr->getDirs = &FsDir::getDirs;
        ptr->rwops_seek = &FsDir::rwops_seek;
        ptr->rwops_read = &FsDir::rwops_read;
        ptr->rwops_write = &FsDir::rwops_write;
        ptr->rwops_close = &FsDir::rwops_close;
#ifdef USE_SDL2
        ptr->rwops_size = &FsDir::rwops_size;
#endif  // USE_SDL2
    }

    FsFuncs *getFuncs()
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

    bool getRealDir(FsEntry *restrict const entry,
                    std::string filename,
                    std::string dirName A_UNUSED,
                    std::string &realDir)
    {
        const DirEntry *const dirEntry = static_cast<const DirEntry*>(entry);
        if (Files::existsLocal(dirEntry->rootSubDir + filename))
        {
            realDir = dirEntry->userDir;
            return true;
        }
        return false;
    }

    bool exists(FsEntry *restrict const entry,
                std::string fileName,
                std::string dirName A_UNUSED)
    {
        return Files::existsLocal(static_cast<DirEntry*>(entry)->rootSubDir +
            fileName);
    }

    void enumerate(FsEntry *restrict const entry,
                   std::string dirName,
                   StringVect &names)
    {
        const std::string path = static_cast<DirEntry*>(entry)->rootSubDir +
            dirName;
        const struct dirent *next_file = nullptr;
        DIR *const dir = opendir(path.c_str());
        if (dir != nullptr)
        {
            while ((next_file = readdir(dir)) != nullptr)
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

    bool isDirectory(FsEntry *restrict const entry,
                     std::string dirName,
                     bool &isDirFlag)
    {
        std::string path = static_cast<DirEntry*>(entry)->rootSubDir + dirName;

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
            reportAlways("FsDir::isSymbolicLink invalid path: %s",
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

    void freeList(List *restrict const handle)
    {
        delete handle;
    }

    bool setWriteDir(std::string newDir)
    {
        prepareFsPath(newDir);
        mWriteDir = STD_MOVE(newDir);
        if (findLast(mWriteDir, std::string(dirSeparator)) == false)
            mWriteDir += dirSeparator;
        return true;
    }

    bool mkdir(std::string dirname)
    {
        prepareFsPath(dirname);
        if (mWriteDir.empty())
        {
            reportAlways("FsDir::mkdir write dir is empty");
            return false;
        }
        return mkdir_r((mWriteDir + dirname).c_str()) != -1;
    }

    bool remove(std::string filename)
    {
        prepareFsPath(filename);
        if (mWriteDir.empty())
        {
            reportAlways("FsDir::remove write dir is empty");
            return false;
        }
        return ::remove((mWriteDir + filename).c_str()) != 0;
    }

    void permitLinks(const bool val)
    {
        mPermitLinks = val;
    }

    int close(File *restrict const file)
    {
        if (file == nullptr)
            return 0;
        delete file;
        return 1;
    }

    int64_t read(File *restrict const file,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount)
    {
        if (file == nullptr)
            return 0;
        FILEHTYPE fd = file->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("FsDir::read file not opened.");
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

    int64_t write(File *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount)
    {
        if (file == nullptr)
            return 0;
        FILEHTYPE fd = file->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("FsDir::write file not opened.");
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

    int64_t fileLength(File *restrict const file)
    {
        if (file == nullptr)
            return -1;
        FILEHTYPE fd = file->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("FsDir::fileLength file not opened.");
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
            reportAlways("FsDir::fileLength error.");
            return -1;
        }
        return static_cast<int64_t>(statbuf.st_size);
#endif  // USE_FILE_FOPEN
    }

    int64_t tell(File *restrict const file)
    {
        if (file == nullptr)
            return -1;

        FILEHTYPE fd = file->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("FsDir::tell file not opened.");
            return 0;
        }
#ifdef USE_FILE_FOPEN
        const int64_t pos = ftell(fd);
#else  // USE_FILE_FOPEN
        const int64_t pos = lseek(fd, 0, SEEK_CUR);
#endif  // USE_FILE_FOPEN
        return pos;
    }

    int seek(File *restrict const file,
             const uint64_t pos)
    {
        if (file == nullptr)
            return 0;

        FILEHTYPE fd = file->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("FsDir::seek file not opened.");
            return 0;
        }
        const int64_t res = FILESEEK(fd, pos, SEEK_SET);
        if (res == -1)
            return 0;
        return 1;
    }

    int eof(File *restrict const file)
    {
        if (file == nullptr)
            return -1;

        FILEHTYPE fd = file->mFd;
        if (fd == FILEHDEFAULT)
        {
            reportAlways("FsDir::eof file not opened.");
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
            reportAlways("FsDir::fileLength error.");
            return -1;
        }
        const int64_t len = static_cast<int64_t>(statbuf.st_size);
#endif  // USE_FILE_FOPEN
        return static_cast<int>(pos < 0 || len < 0 || pos >= len);
    }

    const char *loadFile(FsEntry *restrict const entry,
                         std::string filename,
                         int &restrict fileSize)
    {
        const DirEntry *const dirEntry = static_cast<DirEntry*>(entry);
        const std::string path = dirEntry->rootSubDir + filename;
        if (Files::existsLocal(path) == false)
            return nullptr;
        FILEHTYPE fd = FILEOPEN(path.c_str(),
            FILEOPEN_FLAG_READ);
        if (fd == FILEHDEFAULT)
        {
            reportAlways("VirtFs::loadFile file open error: %s",
                filename.c_str());
            return nullptr;
        }

        logger->log("Loaded %s/%s",
            dirEntry->userDir.c_str(),
            filename.c_str());

#ifdef USE_FILE_FOPEN
        fseek(fd, 0, SEEK_END);
        const long sz = ftell(fd);
        fseek(fd, 0, SEEK_SET);
        fileSize = static_cast<int>(sz);
#else  // USE_FILE_FOPEN
        struct stat statbuf;
        if (fstat(fd, &statbuf) == -1)
        {
            reportAlways("FsDir::fileLength error.");
            if (fd != FILEHDEFAULT)
                FILECLOSE(fd);
            return nullptr;
        }
        fileSize = static_cast<int>(statbuf.st_size);
#endif  // USE_FILE_FOPEN

        // Allocate memory and load the file
        char *restrict const buffer = new char[CAST_SIZE(fileSize)];
        if (fileSize > 0)
            buffer[fileSize - 1] = 0;

#ifdef USE_FILE_FOPEN
        const int cnt = CAST_S32(fread(buffer, 1, fileSize, fd));
#else  // USE_FILE_FOPEN
        const int cnt = ::read(fd, buffer, fileSize);
#endif  // USE_FILE_FOPEN

        if (cnt <= 0)
        {
            delete [] buffer;
            if (fd != FILEHDEFAULT)
                FILECLOSE(fd);
            return nullptr;
        }

        if (fd != FILEHDEFAULT)
            FILECLOSE(fd);

        return buffer;
    }

    void getFiles(FsEntry *restrict const entry,
                  std::string dirName,
                  StringVect &names)
    {
        const std::string path = static_cast<DirEntry*>(entry)->rootSubDir +
            dirName;
        const struct dirent *next_file = nullptr;
        DIR *const dir = opendir(path.c_str());
        if (dir != nullptr)
        {
            while ((next_file = readdir(dir)) != nullptr)
            {
                struct stat statbuf;
                const std::string file = next_file->d_name;
                if (file == "." || file == "..")
                    continue;
#ifndef WIN32
                if (mPermitLinks == false)
                {
                    if (lstat(path.c_str(), &statbuf) == 0 &&
                        S_ISLNK(statbuf.st_mode) != 0)
                    {
                        continue;
                    }
                }
#endif  // WIN32

                const std::string filePath = pathJoin(path, file);
                if (stat(filePath.c_str(), &statbuf) == 0)
                {
                    if (S_ISDIR(statbuf.st_mode) != 0)
                        continue;
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

    void getFilesWithDir(FsEntry *restrict const entry,
                         const std::string &dirName,
                         StringVect &names)
    {
        const std::string path = static_cast<DirEntry*>(entry)->rootSubDir +
            dirName;
        const struct dirent *next_file = nullptr;
        DIR *const dir = opendir(path.c_str());
        if (dir != nullptr)
        {
            while ((next_file = readdir(dir)) != nullptr)
            {
                struct stat statbuf;
                const std::string file = next_file->d_name;
                if (file == "." || file == "..")
                    continue;
#ifndef WIN32
                if (mPermitLinks == false)
                {
                    if (lstat(path.c_str(), &statbuf) == 0 &&
                        S_ISLNK(statbuf.st_mode) != 0)
                    {
                        continue;
                    }
                }
#endif  // WIN32

                const std::string filePath = pathJoin(path, file);
                if (stat(filePath.c_str(), &statbuf) == 0)
                {
                    if (S_ISDIR(statbuf.st_mode) != 0)
                        continue;
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
                    names.push_back(pathJoin(dirName, file));
            }
            closedir(dir);
        }
    }

    void getDirs(FsEntry *restrict const entry,
                 std::string dirName,
                 StringVect &names)
    {
        const std::string path = static_cast<DirEntry*>(entry)->rootSubDir +
            dirName;
        const struct dirent *next_file = nullptr;
        DIR *const dir = opendir(path.c_str());
        if (dir != nullptr)
        {
            while ((next_file = readdir(dir)) != nullptr)
            {
                struct stat statbuf;
                const std::string file = next_file->d_name;
                if (file == "." || file == "..")
                    continue;
#ifndef WIN32
                if (mPermitLinks == false)
                {
                    if (lstat(path.c_str(), &statbuf) == 0 &&
                        S_ISLNK(statbuf.st_mode) != 0)
                    {
                        continue;
                    }
                }
#endif  // WIN32

                const std::string filePath = pathJoin(path, file);
                if (stat(filePath.c_str(), &statbuf) == 0)
                {
                    if (S_ISDIR(statbuf.st_mode) == 0)
                        continue;
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
}  // namespace FsDir

}  // namespace VirtFs
