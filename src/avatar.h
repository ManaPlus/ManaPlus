/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "localconsts.h"

#include <string>

enum AvatarType
{
    AVATAR_PLAYER = 0
};

class Avatar
{
public:
    explicit Avatar(const std::string &name = "");

    A_DELETE_COPY(Avatar)

    virtual ~Avatar()
    { }

    /**
     * Returns the avatar's name.
     */
    std::string getName() const A_WARN_UNUSED
    { return mName; }

    /**
     * Set the avatar's name.
     */
    void setName(const std::string &name)
    { mName = name; }

    /**
     * Returns the avatar's original name.
     */
    std::string getOriginalName() const A_WARN_UNUSED
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
    bool getOnline() const A_WARN_UNUSED
    { return mOnline; }

    /**
     * Set the avatar's online status.
     */
    void setOnline(const bool online)
    { mOnline = online; }

    int getHp() const A_WARN_UNUSED
    { return mHp; }

    void setHp(const int hp)
    { mHp = hp; }

    int getMaxHp() const A_WARN_UNUSED
    { return mMaxHp; }

    void setMaxHp(const int maxHp)
    { mMaxHp = maxHp; }

    int getDamageHp() const A_WARN_UNUSED
    { return mDamageHp; }

    void setDamageHp(const int damageHp)
    { mDamageHp = damageHp; }

    bool getDisplayBold() const A_WARN_UNUSED
    { return mDisplayBold; }

    void setDisplayBold(const bool displayBold)
    { mDisplayBold = displayBold; }

    int getLevel() const A_WARN_UNUSED
    { return mLevel; }

    void setLevel(const int level)
    { mLevel = level; }

    std::string getMap() const A_WARN_UNUSED
    { return mMap; }

    void setMap(std::string map)
    { mMap = map; }

    int getX() const A_WARN_UNUSED
    { return mX; }

    void setX(const int x)
    { mX = x; }

    int getY() const A_WARN_UNUSED
    { return mY; }

    void setY(const int y)
    { mY = y; }

    int getType() const A_WARN_UNUSED
    { return mType; }

    void setType(const int n)
    { mType = n; }

    int getExp() const A_WARN_UNUSED
    { return mExp; }

    void setExp(const int n)
    { mExp = n; }

    int getID() const A_WARN_UNUSED
    { return mId; }

    void setID(const int id)
    { mId = id; }

    int getCharId() const A_WARN_UNUSED
    { return mCharId; }

    void setCharId(const int id)
    { mCharId = id; }

    int getGender() const A_WARN_UNUSED
    { return mGender; }

    void setGender(const int g)
    { mGender = g; }

    int getRace() const A_WARN_UNUSED
    { return mRace; }

    void setRace(const int r)
    { mRace = r; }

    const std::string &getIp() const A_WARN_UNUSED
    { return mIp; }

    void setIp(std::string ip)
    { mIp = ip; }

    bool getPoison() const A_WARN_UNUSED
    { return mPoison; }

    void setPoison(const bool b)
    { mPoison = b; }

protected:
    int mId;
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
    int mGender;
    int mRace;
    std::string mIp;
    bool mOnline;
    bool mDisplayBold;
    bool mPoison;
};

#endif  // AVATAR_H
