/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef AVATAR_H
#define AVATAR_H

#include "enums/being/gender.h"

#include "enums/simpletypes/beingid.h"

#include <string>

#include "localconsts.h"

enum AvatarType
{
    AVATAR_PLAYER = 0
};

class Avatar notfinal
{
    public:
        explicit Avatar(const std::string &name);

        A_DELETE_COPY(Avatar)

        virtual ~Avatar()
        { }

        /**
         * Returns the avatar's name.
         */
        std::string getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        /**
         * Set the avatar's name.
         */
        void setName(const std::string &name)
        { mName = name; }

        /**
         * Returns the avatar's original name.
         */
        std::string getOriginalName() const noexcept2 A_WARN_UNUSED
        { return mOriginalName; }

        std::string getComplexName() const A_WARN_UNUSED;

        virtual std::string getAdditionString() const A_WARN_UNUSED;

        /**
         * Set the avatar's original name.
         */
        void setOriginalName(const std::string &name)
        { mOriginalName = name; }

        /**
         * Returns the avatar's online status.
         */
        bool getOnline() const noexcept2 A_WARN_UNUSED
        { return mOnline; }

        /**
         * Set the avatar's online status.
         */
        void setOnline(const bool online)
        { mOnline = online; }

        int getHp() const noexcept2 A_WARN_UNUSED
        { return mHp; }

        void setHp(const int hp)
        { mHp = hp; }

        int getMaxHp() const noexcept2 A_WARN_UNUSED
        { return mMaxHp; }

        void setMaxHp(const int maxHp)
        { mMaxHp = maxHp; }

        int getDamageHp() const noexcept2 A_WARN_UNUSED
        { return mDamageHp; }

        void setDamageHp(const int damageHp)
        { mDamageHp = damageHp; }

        bool getDisplayBold() const noexcept2 A_WARN_UNUSED
        { return mDisplayBold; }

        void setDisplayBold(const bool displayBold)
        { mDisplayBold = displayBold; }

        int getLevel() const noexcept2 A_WARN_UNUSED
        { return mLevel; }

        void setLevel(const int level)
        { mLevel = level; }

        std::string getMap() const noexcept2 A_WARN_UNUSED
        { return mMap; }

        void setMap(const std::string &map)
        { mMap = map; }

        int getX() const noexcept2 A_WARN_UNUSED
        { return mX; }

        void setX(const int x)
        { mX = x; }

        int getY() const noexcept2 A_WARN_UNUSED
        { return mY; }

        void setY(const int y)
        { mY = y; }

        int getType() const noexcept2 A_WARN_UNUSED
        { return mType; }

        void setType(const int n)
        { mType = n; }

        int getExp() const noexcept2 A_WARN_UNUSED
        { return mExp; }

        void setExp(const int n)
        { mExp = n; }

        BeingId getID() const noexcept2 A_WARN_UNUSED
        { return mId; }

        void setID(const BeingId id)
        { mId = id; }

        int getCharId() const noexcept2 A_WARN_UNUSED
        { return mCharId; }

        void setCharId(const int id)
        { mCharId = id; }

        GenderT getGender() const noexcept2 A_WARN_UNUSED
        { return mGender; }

        void setGender(const GenderT g)
        { mGender = g; }

        int getRace() const noexcept2 A_WARN_UNUSED
        { return mRace; }

        void setRace(const int r)
        { mRace = r; }

        const std::string &getIp() const noexcept2 A_WARN_UNUSED
        { return mIp; }

        void setIp(const std::string &ip)
        { mIp = ip; }

        bool getPoison() const noexcept2 A_WARN_UNUSED
        { return mPoison; }

        void setPoison(const bool b)
        { mPoison = b; }

    protected:
        BeingId mId;
        int mCharId;
        std::string mName;
        std::string mOriginalName;
        int mHp;
        int mMaxHp;
        int mDamageHp;
        int mLevel;
        std::string mMap;
        int mX;
        int mY;
        int mType;
        int mExp;
        GenderT mGender;
        int mRace;
        std::string mIp;
        bool mOnline;
        bool mDisplayBold;
        bool mPoison;
};

#endif  // AVATAR_H
