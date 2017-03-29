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

/*
 * This code provides a glue layer between PhysicsFS and Simple Directmedia
 *  Layer's (SDL) RWops i/o abstraction.
 *
 * License: this code is public domain. I make no warranty that it is useful,
 *  correct, harmless, or environmentally safe.
 *
 * This particular file may be used however you like, including copying it
 *  verbatim into a closed-source project, exploiting it commercially, and
 *  removing any trace of my name from the source (although I hope you won't
 *  do that). I welcome enhancements and corrections to this file, but I do
 *  not require you to send me patches if you make changes. This code has
 *  NO WARRANTY.
 *
 * Unless otherwise stated, the rest of PhysicsFS falls under the zlib license.
 *  Please see LICENSE.txt in the root of the source tree.
 *
 * SDL falls under the LGPL license. You can get SDL at http://www.libsdl.org/
 *
 *  This file was written by Ryan C. Gordon. (icculus@icculus.org).
 *
 *  Copyright (C) 2012-2017  The ManaPlus Developers
 */

#include "fs/virtfs/virtfsrwops.h"

#include "logger.h"

#include "fs/virtfs/virtfs.h"

#include "utils/fuzzer.h"

#include "debug.h"

RWOPSINT VirtFs::rwops_seek(SDL_RWops *const rw,
                            const RWOPSINT offset,
                            const int whence)
{
    if (!rw)
        return -1;
    VirtFile *const handle = static_cast<VirtFile *const>(
        rw->hidden.unknown.data1);
    RWOPSINT pos = 0;

    if (whence == SEEK_SET)
    {
        pos = offset;
    } /* if */
    else if (whence == SEEK_CUR)
    {
        const int64_t current = VirtFs::tell(handle);
        if (current == -1)
        {
            logger->assertLog(
                "VirtFs::rwops_seek: Can't find position in file.");
            return -1;
        } /* if */

        pos = CAST_S32(current);
        if (static_cast<int64_t>(pos) != current)
        {
            logger->assertLog("VirtFs::rwops_seek: "
                "Can't fit current file position in an int!");
            return -1;
        } /* if */

        if (offset == 0)  /* this is a "tell" call. We're done. */
            return pos;

        pos += offset;
    } /* else if */
    else if (whence == SEEK_END)
    {
        const int64_t len = VirtFs::fileLength(handle);
        if (len == -1)
        {
            logger->assertLog("VirtFs::rwops_seek:Can't find end of file.");
            return -1;
        } /* if */

        pos = static_cast<RWOPSINT>(len);
        if (static_cast<int64_t>(pos) != len)
        {
            logger->assertLog("VirtFs::rwops_seek: "
                "Can't fit end-of-file position in an int!");
            return -1;
        } /* if */

        pos += offset;
    } /* else if */
    else
    {
        logger->assertLog("VirtFs::rwops_seek: Invalid 'whence' parameter.");
        return -1;
    } /* else */

    if (pos < 0)
    {
        logger->assertLog("VirtFs::rwops_seek: "
            "Attempt to seek past start of file.");
        return -1;
    } /* if */

    if (!VirtFs::seek(handle, static_cast<uint64_t>(pos)))
    {
        logger->assertLog("VirtFs::rwops_seek: seek error.");
        return -1;
    } /* if */

    return pos;
} /* VirtFs::rwops_seek */

RWOPSSIZE VirtFs::rwops_read(SDL_RWops *const rw,
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
    } /* if */

    return CAST_S32(rc);
} /* VirtFs::rwops_read */

RWOPSSIZE VirtFs::rwops_write(SDL_RWops *const rw,
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
} /* VirtFs::rwops_write */

int VirtFs::rwops_close(SDL_RWops *const rw)
{
    if (!rw)
        return 0;
    VirtFile *const handle = static_cast<VirtFile*>(
        rw->hidden.unknown.data1);
    if (!VirtFs::close(handle))
    {
        logger->assertLog("VirtFs::rwops_seek: close error.");
        return -1;
    } /* if */

    SDL_FreeRW(rw);
    return 0;
} /* VirtFs::rwops_close */

#ifdef USE_SDL2
RWOPSINT VirtFs::rwops_size(SDL_RWops *const rw)
{
    VirtFile *const handle = static_cast<VirtFile *const>(
        rw->hidden.unknown.data1);
    return VirtFs::fileLength(handle);
} /* VirtFs::rwops_size */
#endif  // USE_SDL2

SDL_RWops *VirtFs::create_rwops(VirtFile *const file)
{
    SDL_RWops *retval = nullptr;

    if (!file)
    {
        logger->assertLog("VirtFs::rwops_seek: create rwops error.");
    }
    else
    {
        retval = SDL_AllocRW();
        if (retval)
        {
#ifdef USE_SDL2
            retval->size  = &VirtFs::rwops_size;
#endif  // USE_SDL2

            retval->seek  = &VirtFs::rwops_seek;
            retval->read  = &VirtFs::rwops_read;
            retval->write = &VirtFs::rwops_write;
            retval->close = &VirtFs::rwops_close;
            retval->hidden.unknown.data1 = file;
        } /* if */
    } /* else */

    return retval;
} /* VirtFs::create_rwops */

SDL_RWops *VirtFs::makeRwops(VirtFile *const handle)
{
    SDL_RWops *retval = nullptr;
    if (!handle)
    {
        logger->assertLog("VirtFs::rwops_seek: NULL pointer passed to "
            "RWopsmakeRWops().");
    }
    else
    {
        retval = VirtFs::create_rwops(handle);
    }

    return retval;
} /* RWopsmakeRWops */

#ifdef __APPLE__
static bool checkFilePath(const std::string &restrict fname)
{
    if (fname.empty())
        return false;
    if (!VirtFs::exists(fname) || VirtFs::isDirectory(fname))
        return false;
    return true;
}
#endif  // __APPLE__

SDL_RWops *VirtFs::rwopsOpenRead(const std::string &restrict fname)
{
    BLOCK_START("RWopsopenRead")
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__
#ifdef USE_FUZZER
    if (Fuzzer::conditionTerminate(fname))
        return nullptr;
#endif  // USE_FUZZER
#ifdef USE_PROFILER

    SDL_RWops *const ret = VirtFs::create_rwops(VirtFs::openRead(fname));

    BLOCK_END("RWopsopenRead")
    return ret;
#else  // USE_PROFILER

    return VirtFs::create_rwops(VirtFs::openRead(fname));
#endif  // USE_PROFILER
} /* RWopsopenRead */

SDL_RWops *VirtFs::rwopsOpenWrite(const std::string &restrict fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return VirtFs::create_rwops(VirtFs::openWrite(fname));
} /* RWopsopenWrite */

SDL_RWops *VirtFs::rwopsOpenAppend(const std::string &restrict fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return VirtFs::create_rwops(VirtFs::openAppend(fname));
} /* RWopsopenAppend */

/* end of virtfsrwops.c ... */
