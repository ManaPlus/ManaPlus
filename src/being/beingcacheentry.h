/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef BEING_BEINGCACHEENTRY_H
#define BEING_BEINGCACHEENTRY_H

#include "enums/simpletypes/beingid.h"

#include "localconsts.h"

#include <string>

class BeingCacheEntry final
{
    public:
        explicit BeingCacheEntry(const BeingId id) :
            mName(),
            mPartyName(),
            mGuildName(),
            mIp(),
            mId(id),
            mLevel(0),
            mPvpRank(0),
            mPvpChannel(0),
            mTime(0),
            mFlags(0),
            mIsAdvanced(false)
        {
        }

        A_DELETE_COPY(BeingCacheEntry)

        BeingId getId() const
        { return mId; }

        /**
         * Returns the name of the being.
         */
        const std::string &getName() const
        { return mName; }

        /**
         * Sets the name for the being.
         *
         * @param name The name that should appear.
         */
        void setName(const std::string &name)
        { mName = name; }

        /**
         * Following are set from the server (mainly for players)
         */
        void setPartyName(const std::string &name)
        { mPartyName = name; }

        void setGuildName(const std::string &name)
        { mGuildName = name; }

        const std::string &getPartyName() const
        { return mPartyName; }

        const std::string &getGuildName() const
        { return mGuildName; }

        void setLevel(const int n)
        { mLevel = n; }

        int getLevel() const
        { return mLevel; }

        void setTime(const int n)
        { mTime = n; }

        int getTime() const
        { return mTime; }

        unsigned getPvpRank() const
        { return mPvpRank; }

        void setPvpRank(const int r)
        { mPvpRank = r; }

        unsigned getPvpChannel() const
        { return mPvpChannel; }

        void setPvpChannel(const int r)
        { mPvpChannel = r; }

        std::string getIp() const
        { return mIp; }

        void setIp(const std::string &ip)
        { mIp = ip; }

        bool isAdvanced() const
        { return mIsAdvanced; }

        void setAdvanced(const bool a)
        { mIsAdvanced = a; }

        int getFlags() const
        { return mFlags; }

        void setFlags(const int flags)
        { mFlags = flags; }

    protected:
        std::string mName;        /**< Name of character */
        std::string mPartyName;
        std::string mGuildName;
        std::string mIp;
        BeingId mId;              /**< Unique sprite id */
        int mLevel;
        unsigned int mPvpRank;
        unsigned int mPvpChannel;
        int mTime;
        int mFlags;
        bool mIsAdvanced;
};

#endif  // BEING_BEINGCACHEENTRY_H
