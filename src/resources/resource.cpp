/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "resources/resource.h"

#include "logger.h"

#include "utils/cast.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

Resource::~Resource()
{
}

void Resource::incRef()
{
#ifdef DEBUG_IMAGES
    logger->log("before incRef for: %p", static_cast<void*>(this));
    mRefCount++;
    logger->log("after incRef: %p, %u", static_cast<void*>(this), mRefCount);
#else  // DEBUG_IMAGES

    mRefCount++;
#endif  // DEBUG_IMAGES
}

void Resource::decRef()
{
#ifdef DEBUG_IMAGES
    logger->log("before decRef for: %p", static_cast<void*>(this));
#endif  // DEBUG_IMAGES

    // Reference may not already have reached zero
    if (mRefCount == 0)
    {
        logger->log("Warning: mRefCount already zero for %s", mIdPath.c_str());
        return;
    }

    mRefCount--;

#ifdef DEBUG_IMAGES
    logger->log("after decRef: %p, %u", static_cast<void*>(this), mRefCount);
#endif  // DEBUG_IMAGES

    if (mRefCount == 0 && !mNotCount)
    {
        // Warn the manager that this resource is no longer used.
        ResourceManager::release(this);
    }
}

int Resource::calcMemoryLocal() const
{
    return CAST_S32(sizeof(Resource)) +
        CAST_S32(mIdPath.size()) +
        CAST_S32(mSource.size());
}
