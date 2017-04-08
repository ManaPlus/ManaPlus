/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RESOURCES_RESOURCE_H
#define RESOURCES_RESOURCE_H

#include "resources/memorycounter.h"

#include "localconsts.h"

/**
 * A generic reference counted resource object.
 */
class Resource notfinal : public MemoryCounter
{
    public:
        /**
         * Constructor
         */
        Resource() :
            MemoryCounter(),
            mTimeStamp(0),
            mIdPath(),
            mSource(),
            mRefCount(0),
            mProtected(false),
#ifdef DEBUG_DUMP_LEAKS
            mNotCount(false),
            mDumped(false)
#else  // DEBUG_DUMP_LEAKS
            mNotCount(false)
#endif  // DEBUG_DUMP_LEAKS
        {
        }

        A_DELETE_COPY(Resource)

        /**
         * Destructor.
         */
        virtual ~Resource();

        /**
         * Increments the internal reference count.
         */
        virtual void incRef();

        /**
         * Decrements the reference count and deletes the object
         * if no references are left.
         *
         * @return <code>true</code> if the object was deleted
         *         <code>false</code> otherwise.
         */
        virtual void decRef();

        int calcMemoryLocal() const override;

        std::string getCounterName() const override
        { return mIdPath + "-" + mSource; }

        time_t mTimeStamp;   /**< Time at which the resource was orphaned. */

        std::string mIdPath; /**< Path identifying this resource. */
        std::string mSource;

        unsigned int mRefCount;  /**< Reference count. */
        bool mProtected;
        bool mNotCount;

#ifdef DEBUG_DUMP_LEAKS
        bool mDumped;
#endif  // DEBUG_DUMP_LEAKS
};

#endif  // RESOURCES_RESOURCE_H
