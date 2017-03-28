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

#include "fs/virtfsrwops.h"

#include "logger.h"

#include "fs/virtfs.h"

#ifdef DEBUG_VIRTFS
#include "utils/debugmemoryobject.h"

#include <map>
#endif  // DEBUG_VIRTFS

#include "utils/fuzzer.h"

#include "debug.h"

#ifdef USE_SDL2
#define RWOPSINT int64_t
#define RWOPSSIZE size_t
#else  // USE_SDL2
#define RWOPSINT int32_t
#define RWOPSSIZE int
#endif  // USE_SDL2

#ifdef DUMP_LEAKED_RESOURCES
static int openedRWops = 0;
#endif  // DUMP_LEAKED_RESOURCES

#ifdef DEBUG_VIRTFS
namespace
{
    std::map<void*, VirtFs::DebugMemoryObject*> mRWops;
}  // namespace

static SDL_RWops *addDebugRWops(SDL_RWops *restrict const rwops,
                                const char *restrict const name,
                                const char *restrict const file,
                                const unsigned line)
{
    if (!rwops)
        return nullptr;

    mRWops[rwops] = new VirtFs::DebugMemoryObject(name, file, line);
    return rwops;
}

static void deleteDebugRWops(SDL_RWops *const rwops)
{
    if (!rwops)
        return;

    std::map<void*, VirtFs::DebugMemoryObject*>::iterator it =
        mRWops.find(rwops);
    if (it == mRWops.end())
    {
        logger->log("bad RWops delete: %p", static_cast<void*>(rwops));
    }
    else
    {
        VirtFs::DebugMemoryObject *const obj = (*it).second;
        if (obj)
        {
            mRWops.erase(rwops);
            delete obj;
        }
    }
}

void VirtFs::reportLeaks()
{
    if (!mRWops.empty())
    {
        logger->log("RWops leaks detected");
        std::map<void*, VirtFs::DebugMemoryObject*>::iterator it =
            mRWops.begin();
        const std::map<void*, VirtFs::DebugMemoryObject*>::iterator
            it_end = mRWops.end();
        for (; it != it_end; ++it)
        {
            VirtFs::DebugMemoryObject *obj = (*it).second;
            if (obj)
            {
                logger->log("file: %s at %s", obj->mName.c_str(),
                    obj->mAddFile.c_str());
                delete obj;
            }
        }
        mRWops.clear();
    }
}
#endif  // DEBUG_VIRTFS

static RWOPSINT virtfsrwops_seek(SDL_RWops *const rw,
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
                "virtfsrwops_seek: Can't find position in file.");
            return -1;
        } /* if */

        pos = CAST_S32(current);
        if (static_cast<int64_t>(pos) != current)
        {
            logger->assertLog("virtfsrwops_seek: "
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
            logger->assertLog("virtfsrwops_seek:Can't find end of file.");
            return -1;
        } /* if */

        pos = static_cast<RWOPSINT>(len);
        if (static_cast<int64_t>(pos) != len)
        {
            logger->assertLog("virtfsrwops_seek: "
                "Can't fit end-of-file position in an int!");
            return -1;
        } /* if */

        pos += offset;
    } /* else if */
    else
    {
        logger->assertLog("virtfsrwops_seek: Invalid 'whence' parameter.");
        return -1;
    } /* else */

    if (pos < 0)
    {
        logger->assertLog("virtfsrwops_seek: "
            "Attempt to seek past start of file.");
        return -1;
    } /* if */

    if (!VirtFs::seek(handle, static_cast<uint64_t>(pos)))
    {
        logger->assertLog("virtfsrwops_seek: seek error.");
        return -1;
    } /* if */

    return pos;
} /* virtfsrwops_seek */

static RWOPSSIZE virtfsrwops_read(SDL_RWops *const rw,
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
            logger->assertLog("virtfsrwops_seek: read error.");
        }
    } /* if */

    return CAST_S32(rc);
} /* virtfsrwops_read */

static RWOPSSIZE virtfsrwops_write(SDL_RWops *const rw,
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
        logger->assertLog("virtfsrwops_seek: write error.");
    }

    return CAST_S32(rc);
} /* virtfsrwops_write */

static int virtfsrwops_close(SDL_RWops *const rw)
{
    if (!rw)
        return 0;
    VirtFile *const handle = static_cast<VirtFile*>(
        rw->hidden.unknown.data1);
    if (!VirtFs::close(handle))
    {
        logger->assertLog("virtfsrwops_seek: close error.");
        return -1;
    } /* if */

    SDL_FreeRW(rw);
#ifdef DUMP_LEAKED_RESOURCES
    if (openedRWops <= 0)
        logger->assertLog("virtfsrwops_seek: closing already closed RWops");
    openedRWops --;
#endif  // DUMP_LEAKED_RESOURCES
#ifdef DEBUG_VIRTFS
    deleteDebugRWops(rw);
#endif  // DEBUG_VIRTFS

    return 0;
} /* virtfsrwops_close */

#ifdef USE_SDL2
static RWOPSINT virtfsrwops_size(SDL_RWops *const rw)
{
    VirtFile *const handle = static_cast<VirtFile *const>(
        rw->hidden.unknown.data1);
    return VirtFs::fileLength(handle);
} /* virtfsrwops_size */
#endif  // USE_SDL2

static SDL_RWops *create_rwops(VirtFile *const file)
{
    SDL_RWops *retval = nullptr;

    if (!file)
    {
        logger->assertLog("virtfsrwops_seek: create rwops error.");
    }
    else
    {
        retval = SDL_AllocRW();
        if (retval)
        {
#ifdef USE_SDL2
            retval->size  = &virtfsrwops_size;
#endif  // USE_SDL2

            retval->seek  = &virtfsrwops_seek;
            retval->read  = &virtfsrwops_read;
            retval->write = &virtfsrwops_write;
            retval->close = &virtfsrwops_close;
            retval->hidden.unknown.data1 = file;
        } /* if */
#ifdef DUMP_LEAKED_RESOURCES
        openedRWops ++;
#endif  // DUMP_LEAKED_RESOURCES
    } /* else */

    return retval;
} /* create_rwops */

SDL_RWops *VirtFs::MakeRWops(VirtFile *const handle)
{
    SDL_RWops *retval = nullptr;
    if (!handle)
    {
        logger->assertLog("virtfsrwops_seek: NULL pointer passed to "
            "RWopsmakeRWops().");
    }
    else
    {
        retval = create_rwops(handle);
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

#ifdef DEBUG_VIRTFS
#undef RWopsOpenRead
SDL_RWops *VirtFs::RWopsOpenRead(const std::string &restrict fname,
                                 const char *restrict const file,
                                 const unsigned line)
#else  // DEBUG_VIRTFS
SDL_RWops *VirtFs::RWopsOpenRead(const std::string &restrict fname)
#endif  // DEBUG_VIRTFS
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

#ifdef DEBUG_VIRTFS
    SDL_RWops *const ret = addDebugRWops(
        create_rwops(VirtFs::openRead(fname)),
        fname,
        file,
        line);
#else  // DEBUG_VIRTFS
    SDL_RWops *const ret = create_rwops(VirtFs::openRead(fname));
#endif  // DEBUG_VIRTFS

    BLOCK_END("RWopsopenRead")
    return ret;
#else  // USE_PROFILER

#ifdef DEBUG_VIRTFS
    return addDebugRWops(
        create_rwops(VirtFs::openRead(fname)),
        fname,
        file,
        line);
#else  // DEBUG_VIRTFS
    return create_rwops(VirtFs::openRead(fname));
#endif  // DEBUG_VIRTFS
#endif  // USE_PROFILER
} /* RWopsopenRead */

SDL_RWops *VirtFs::RWopsOpenWrite(const std::string &restrict fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(VirtFs::openWrite(fname));
} /* RWopsopenWrite */

SDL_RWops *VirtFs::RWopsOpenAppend(const std::string &restrict fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(VirtFs::openAppend(fname));
} /* RWopsopenAppend */

#ifdef DUMP_LEAKED_RESOURCES
void VirtFs::reportRWops()
{
    if (openedRWops)
    {
        logger->assertLog("virtfsrwops_seek: leaking RWops: %d",
            openedRWops);
    }
}
#endif  // DUMP_LEAKED_RESOURCES

/* end of virtfsrwops.c ... */
