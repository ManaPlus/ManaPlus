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

#include "fs/virtfs/virtfsziprwops.h"

#include "logger.h"

#include "fs/virtfs/virtfile.h"
#include "fs/virtfs/virtfs.h"

#include <SDL_rwops.h>

#include "debug.h"

namespace VirtFsZip
{
    RWOPSINT rwops_seek(SDL_RWops *const rw,
                        const RWOPSINT offset,
                        const int whence)
    {
        if (!rw)
            return -1;
        VirtFile *const handle = static_cast<VirtFile *const>(
            rw->hidden.unknown.data1);
//        const uint8_t *mBuf = handle->mBuf;
        size_t mPos = handle->mPos;
        size_t mSize = handle->mSize;

        RWOPSINT pos = 0;

        if (whence == SEEK_SET)
        {
            pos = offset;
        }
        else if (whence == SEEK_CUR)
        {
            const int64_t current = mPos;
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
            pos = mSize + offset;
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

        handle->mPos = pos;

        if (pos > static_cast<RWOPSINT>(mSize))
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
        VirtFile *const handle = static_cast<VirtFile *const>(
            rw->hidden.unknown.data1);
        const int64_t rc = VirtFs::read(handle, ptr,
            CAST_U32(size),
            CAST_U32(maxnum));
        if (rc != static_cast<int64_t>(maxnum))
        {
            if (!VirtFs::eof(handle)) /* not EOF? Must be an error. */
            {
                logger->assertLog("VirtFs::rwops_seek: read error.");
            }
        }

        return CAST_S32(rc);
    }

    RWOPSSIZE rwops_write(SDL_RWops *const rw,
                          const void *const ptr,
                          const RWOPSSIZE size,
                          const RWOPSSIZE num)
    {
        if (!rw)
            return 0;
        VirtFile *const handle = static_cast<VirtFile *const>(
            rw->hidden.unknown.data1);
        const int64_t rc = VirtFs::write(handle, ptr,
            CAST_U32(size),
            CAST_U32(num));
        if (rc != static_cast<int64_t>(num))
        {
            logger->assertLog("VirtFs::rwops_seek: write error.");
        }

        return CAST_S32(rc);
    }

    int rwops_close(SDL_RWops *const rw)
    {
        if (!rw)
            return 0;
        VirtFile *const handle = static_cast<VirtFile*>(
            rw->hidden.unknown.data1);
        if (!VirtFs::close(handle))
        {
            logger->assertLog("VirtFs::rwops_seek: close error.");
            return -1;
        }

        SDL_FreeRW(rw);
        return 0;
    }

#ifdef USE_SDL2
    RWOPSINT rwops_size(SDL_RWops *const rw)
    {
        VirtFile *const handle = static_cast<VirtFile *const>(
            rw->hidden.unknown.data1);
        return VirtFs::fileLength(handle);
    }
#endif  // USE_SDL2

}  // namespace VirtFsZip
