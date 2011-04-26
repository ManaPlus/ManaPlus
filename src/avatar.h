/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include <string>

enum AvatarType
{
    AVATAR_PLAYER = 0
};

class Avatar
{
public:
    Avatar(const std::string &name = "");

    /**
     * Returns the avatar's name.
     */
    std::string getName() const
    { return mName; }

    /**
     * Set the avatar's name.
     */
    void setName(const std::string &name)
    { mName = name; }

    /**
     * Returns the avatar's original name.
     */
    std::string getOriginalName() const
    { return mOriginalName; }

    std::string getComplexName() const;

    virtual std::string getAdditionString() const;

    /**
     * Set the avatar's original name.
     */
    void setOriginalName(const std::string &name)
    { mOriginalName = name; }

    /**
     * Returns the avatar's online status.
     */
    bool getOnline() const
    { return mOnline; }

    /**
     * Set the avatar's online status.
     */
    void setOnline(bool online)
    { mOnline = online; }

    int getHp() const
    { return mHp; }

    void setHp(int hp)
    { mHp = hp; }

    int getMaxHp() const
    { return mMaxHp; }

    void setMaxHp(int maxHp)
    { mMaxHp = maxHp; }

    int getDamageHp() const
    { return mDamageHp; }

    void setDamageHp(int damageHp)
    { mDamageHp = damageHp; }

    bool getDisplayBold() const
    { return mDisplayBold; }

    void setDisplayBold(bool displayBold)
    { mDisplayBold = displayBold; }

    int getLevel() const
    { return mLevel; }

    void setLevel(int level)
    { mLevel = level; }

    std::string getMap() const
    { return mMap; }

    void setMap(std::string map)
    { mMap = map; }

    int getX() const
    { return mX; }

    void setX(int x)
    { mX = x; }

    int getY() const
    { return mY; }

    void setY(int y)
    { mY = y; }

    int getType() const
    { return mType; }

    void setType(int n)
    { mType = n; }

    int getExp() const
    { return mExp; }

    void setExp(int n)
    { mExp = n; }

    int getID() const
    { return mId; }

    void setID(int id)
    { mId = id; }

    int getCharId() const
    { return mCharId; }

    void setCharId(int id)
    { mCharId = id; }

    int getGender() const
    { return mGender; }

    void setGender(int g)
    { mGender = g; }

    int getRace() const
    { return mRace; }

    void setRace(int r)
    { mRace = r; }

    const std::string &getIp() const
    { return mIp; }

    void setIp(std::string ip)
    { mIp = ip; }

protected:
    int mId;
    int mCharId;
    std::string mName;
    std::string mOriginalName;
    int mHp;
    int mMaxHp;
    int mDamageHp;
    int mLevel;
    bool mOnline;
    bool mDisplayBold;
    std::string mMap;
    int mX;
    int mY;
    int mType;
    int mExp;
    int mGender;
    int mRace;
    std::string mIp;
};

#endif // AVATAR_H
