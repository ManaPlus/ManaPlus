/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_ONLINEPLAYER_H
#define GUI_ONLINEPLAYER_H

#include "enums/being/gender.h"

#include <string>

#include "localconsts.h"

class OnlinePlayer final
{
    public:
        OnlinePlayer(const std::string &nick,
                     const unsigned char status,
                     const signed char level,
                     const GenderT gender,
                     const signed char version,
                     const unsigned char group) :
            mNick(nick),
            mText(),
            mStatus(status),
            mLevel(level),
            mGroup(group),
            mVersion(version),
            mGender(gender),
            mIsGM(false)
        {
        }

        A_DELETE_COPY(OnlinePlayer)

        const std::string getNick() const noexcept2 A_WARN_UNUSED
        { return mNick; }

        unsigned char getStaus() const noexcept2 A_WARN_UNUSED
        { return mStatus; }

        void setIsGM(const bool b)
        { mIsGM = b; }

        char getVersion() const noexcept2 A_WARN_UNUSED
        { return mVersion; }

        char getLevel() const noexcept2 A_WARN_UNUSED
        { return mLevel; }

        const std::string getText() const noexcept2 A_WARN_UNUSED
        { return mText; }

        void setText(std::string str);

        void setLevel(const char level) noexcept2
        { mLevel = level; }

    private:
        std::string mNick;

        std::string mText;

        unsigned char mStatus;

        signed char mLevel;

        unsigned char mGroup;

        signed char mVersion;

        GenderT mGender;

        bool mIsGM;
};

#endif  // GUI_ONLINEPLAYER_H
