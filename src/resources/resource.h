/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "localconsts.h"

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
        Resource() :
            mTimeStamp(0),
            mProtected(false),
#ifdef DEBUG_DUMP_LEAKS
            mRefCount(0),
            mDumped(false)
#else
            mRefCount(0)
#endif
        {
        }

        A_DELETE_COPY(Resource)

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

        /**
         * Return the path identifying this resource.
         */
        const std::string &getIdPath() const A_WARN_UNUSED
        { return mIdPath; }

        /**
         * Return refCount for this resource.
         */
        unsigned getRefCount() const A_WARN_UNUSED
        { return mRefCount; }

        void setSource(const std::string &str)
        { mSource = str; }

        std::string getSource() const A_WARN_UNUSED
        { return mSource; }

        void setProtected(bool b)
        { mProtected = b; }

        bool isProtected() const
        { return mProtected; }

#ifdef DEBUG_DUMP_LEAKS
        bool getDumped() const A_WARN_UNUSED
        { return mDumped; }

        void setDumped(bool n)
        { mDumped = n; }
#endif

    protected:
        /**
         * Destructor.
         */
        virtual ~Resource();

        std::string mIdPath; /**< Path identifying this resource. */
        std::string mSource;

    private:
        time_t mTimeStamp;   /**< Time at which the resource was orphaned. */
        bool mProtected;
        unsigned mRefCount;  /**< Reference count. */
#ifdef DEBUG_DUMP_LEAKS
        bool mDumped;
#endif
};

#endif
