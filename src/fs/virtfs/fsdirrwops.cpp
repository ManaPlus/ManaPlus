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

#include "fs/virtfs/fsdirrwops.h"

#include "logger.h"

#include "fs/virtfs/file.h"

#include <SDL_rwops.h>

#include "debug.h"

namespace VirtFs
{

namespace FsDir
{
    RWOPSINT rwops_seek(SDL_RWops *const rw,
                        const RWOPSINT offset,
                        const int whence)
    {
        if (!rw)
            return -1;
        File *const handle = static_cast<File *const>(
            rw->hidden.unknown.data1);
        FILEHTYPE fd = handle->mFd;
        RWOPSINT pos = 0;

        if (whence == SEEK_SET)
        {
            pos = offset;
        }
        else if (whence == SEEK_CUR)
        {
#ifdef USE_FILE_FOPEN
            const int64_t current = ftell(fd);
#else  // USE_FILE_FOPEN
            const int64_t current = lseek(fd, 0, SEEK_CUR);
#endif  // USE_FILE_FOPEN

            if (current == -1)
            {
                logger->assertLog(
                    "VirtFs::rwops_seek: Can't find position in file.");
                return -1;
            }

            pos = CAST_S32(current);
            if (static_cast<int64_t>(pos) != current)
            {
                logger->assertLog("VirtFs::rwops_seek: "
                    "Can't fit current file position in an int!");
                return -1;
            }

            if (offset == 0)  /* this is a "tell" call. We're done. */
                return pos;

            pos += offset;
        }
        else if (whence == SEEK_END)
        {
            int64_t len = 0;
#ifdef USE_FILE_FOPEN
            const long curpos = ftell(fd);
            fseek(fd, 0, SEEK_END);
            len = ftell(fd);
//            fseek(fd, curpos, SEEK_SET);
#else  // USE_FILE_FOPEN
            struct stat statbuf;
            if (fstat(fd, &statbuf) == -1)
            {
                reportAlways("FsDir::fileLength error.");
                len = -1;
            }
            else
            {
                len = static_cast<int64_t>(statbuf.st_size);
            }
#endif  // USE_FILE_FOPEN

            if (len == -1)
            {
#ifdef USE_FILE_FOPEN
                fseek(fd, curpos, SEEK_SET);
#endif  // USE_FILE_FOPEN
                logger->assertLog(
                    "VirtFs::rwops_seek:Can't find end of file.");
                return -1;
            }

            pos = static_cast<RWOPSINT>(len);
            if (static_cast<int64_t>(pos) != len)
            {
#ifdef USE_FILE_FOPEN
                fseek(fd, curpos, SEEK_SET);
#endif  // USE_FILE_FOPEN
                logger->assertLog("VirtFs::rwops_seek: "
                    "Can't fit end-of-file position in an int!");
                return -1;
            }

            pos += offset;
        }
        else
        {
            logger->assertLog(
                "VirtFs::rwops_seek: Invalid 'whence' parameter.");
            return -1;
        }

        if (pos < 0)
        {
            logger->assertLog("VirtFs::rwops_seek: "
                "Attempt to seek past start of file.");
            return -1;
        }

        const int64_t res = FILESEEK(fd, pos, SEEK_SET);
        if (res == -1)
        {
            logger->assertLog("VirtFs::rwops_seek: seek error.");
            return -1;
        }

        return pos;
    }

    RWOPSSIZE rwops_read(SDL_RWops *const rw,
                         void *const ptr,
                         const RWOPSSIZE size,
                         const RWOPSSIZE maxnum)
    {
        if (!rw)
            return 0;
        File *const handle = static_cast<File *const>(
            rw->hidden.unknown.data1);
        FILEHTYPE fd = handle->mFd;

#ifdef USE_FILE_FOPEN
        const int64_t rc = fread(ptr, size, maxnum, fd);
#else  // USE_FILE_FOPEN
        int max = size * maxnum;
        int cnt = ::read(fd, ptr, max);
        if (cnt <= 0)
            return cnt;
        const int64_t rc = cnt / size;
#endif  // USE_FILE_FOPEN

        if (rc != static_cast<int64_t>(maxnum))
        {
#ifndef USE_FILE_FOPEN
            const int64_t pos = lseek(fd, 0, SEEK_CUR);
            struct stat statbuf;
            if (fstat(fd, &statbuf) == -1)
            {
                reportAlways("FsDir::fileLength error.");
                return CAST_S32(rc);
            }
#endif  // USE_FILE_FOPEN
        }
        return CAST_S32(rc);
    }

    RWOPSSIZE rwops_write(SDL_RWops *const rw,
                          const void *const ptr,
                          const RWOPSSIZE size,
                          const RWOPSSIZE maxnum)
    {
        if (!rw)
            return 0;
        File *const handle = static_cast<File *const>(
            rw->hidden.unknown.data1);
        FILEHTYPE fd = handle->mFd;

#ifdef USE_FILE_FOPEN
        const int64_t rc = fwrite(ptr, size, maxnum, fd);
#else  // USE_FILE_FOPEN
        int max = size * maxnum;
        int cnt = ::write(fd, ptr, max);
        if (cnt <= 0)
            return cnt;
        const int64_t rc = cnt / size;
#endif  // USE_FILE_FOPEN

        if (rc != static_cast<int64_t>(maxnum))
        {
#ifndef USE_FILE_FOPEN
            const int64_t pos = lseek(fd, 0, SEEK_CUR);
            struct stat statbuf;
            if (fstat(fd, &statbuf) == -1)
            {
                reportAlways("FsDir::fileLength error.");
                return CAST_S32(rc);
            }
#endif  // USE_FILE_FOPEN
        }
        return CAST_S32(rc);
    }

    int rwops_close(SDL_RWops *const rw)
    {
        if (!rw)
            return 0;
        File *const handle = static_cast<File*>(
            rw->hidden.unknown.data1);
        delete handle;
        SDL_FreeRW(rw);
        return 0;
    }

#ifdef USE_SDL2
    RWOPSINT rwops_size(SDL_RWops *const rw)
    {
        File *const handle = static_cast<File *const>(
            rw->hidden.unknown.data1);
        FILEHTYPE fd = handle->mFd;
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
#endif  // USE_SDL2

}  // namespace FsDir

}  // namespace VirtFs
