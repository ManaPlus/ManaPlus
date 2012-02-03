/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef RESOURCE_H
#define RESOURCE_H

#include "main.h"

#include <ctime>
#include <string>

/**
 * A generic reference counted resource object.
 */
class Resource
{
    friend class ResourceManager;

    public:
        /**
         * Constructor
         */
        Resource(): mRefCount(0)
        {
#ifdef DEBUG_DUMP_LEAKS
            mDumped = false;
#endif
        }

        /**
         * Increments the internal reference count.
         */
        void incRef();

        /**
         * Decrements the reference count and deletes the object
         * if no references are left.
         *
         * @return <code>true</code> if the object was deleted
         *         <code>false</code> otherwise.
         */
        void decRef();

        /**
         * Return the path identifying this resource.
         */
        const std::string &getIdPath() const
        { return mIdPath; }

        /**
         * Return refCount for this resource.
         */
        unsigned getRefCount() const
        { return mRefCount; }

#ifdef DEBUG_DUMP_LEAKS
        bool getDumped() const
        { return mDumped; }

        void setDumped(bool n)
        { mDumped = n; }
#endif

    protected:
        /**
         * Destructor.
         */
        virtual ~Resource();

    private:
        std::string mIdPath; /**< Path identifying this resource. */
        time_t mTimeStamp;   /**< Time at which the resource was orphaned. */
        unsigned mRefCount;  /**< Reference count. */
        std::string mName;
#ifdef DEBUG_DUMP_LEAKS
        bool mDumped;
#endif
};

#endif
