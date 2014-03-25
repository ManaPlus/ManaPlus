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

#include "avatar.h"

#include "being/being.h"

#include "debug.h"

Avatar::Avatar(const std::string &name):
    mId(0),
    mCharId(0),
    mName(name),
    mOriginalName(name),
    mHp(0),
    mMaxHp(0),
    mDamageHp(0),
    mLevel(1),
    mMap(),
    mX(-1),
    mY(-1),
    mType(AVATAR_PLAYER),
    mExp(0),
    mGender(GENDER_UNSPECIFIED),
    mRace(-1),
    mIp(),
    mOnline(false),
    mDisplayBold(false),
    mPoison(false)
{
}

std::string Avatar::getComplexName() const
{
    if (mName == mOriginalName || mOriginalName.empty())
    {
        return mName;
    }
    else
    {
        return std::string(mName).append("(").append(
            mOriginalName).append(")");
    }
}

std::string Avatar::getAdditionString() const
{
    if (!getIp().empty())
        return " - " + getIp();
    else
        return "";
}
