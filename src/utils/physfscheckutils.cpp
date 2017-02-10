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

#include "utils/physfscheckutils.h"

#ifdef DEBUG_PHYSFS

#include "logger.h"

#include "utils/stringutils.h"

#include "utils/physfsmemoryobject.h"
#include "utils/virtfsrwops.h"

#include <map>

#include "debug.h"

namespace
{
    std::map<void*, PHYSFSMemoryObject*> mRWops;
}  // namespace

static SDL_RWops *addRWops(SDL_RWops *restrict const rwops,
                           const char *restrict const name,
                           const char *restrict const file,
                           const unsigned line)
{
    if (!rwops)
        return nullptr;

    mRWops[rwops] = new PHYSFSMemoryObject(name, file, line);
    return rwops;
}

static void deleteRWops(SDL_RWops *const rwops)
{
    if (!rwops)
        return;

    std::map<void*, PHYSFSMemoryObject*>::iterator it = mRWops.find(rwops);
    if (it == mRWops.end())
    {
        logger->log("bad RWops delete: %p", static_cast<void*>(rwops));
    }
    else
    {
        PHYSFSMemoryObject *const obj = (*it).second;
        if (obj)
        {
            mRWops.erase(rwops);
            delete obj;
        }
    }
}

SDL_RWops *FakePHYSFSRWOPS_openRead(const char *restrict const name,
                                    const char *restrict const file,
                                    const unsigned line)
{
    return addRWops(VirtFs::RWopsOpenRead(name), name, file, line);
}

void FakePhysFSClose(SDL_RWops *const rwops)
{
    deleteRWops(rwops);
}

void reportPhysfsLeaks()
{
    if (!mRWops.empty())
    {
        logger->log("RWops leaks detected");
        std::map<void*, PHYSFSMemoryObject*>::iterator it = mRWops.begin();
        const std::map<void*, PHYSFSMemoryObject*>::iterator
            it_end = mRWops.end();
        for (; it != it_end; ++it)
        {
            PHYSFSMemoryObject *obj = (*it).second;
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

#endif  // DEBUG_PHYSFS
