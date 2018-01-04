/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef BEING_PLAYERIGNORESTRATEGY_H
#define BEING_PLAYERIGNORESTRATEGY_H

#include <string>

#include "localconsts.h"

class Being;

/**
 * Ignore strategy: describes how we should handle ignores.
 */
class PlayerIgnoreStrategy notfinal
{
    public:
        std::string mDescription;
        std::string mShortName;

        A_DELETE_COPY(PlayerIgnoreStrategy)

        virtual ~PlayerIgnoreStrategy()
        { }

        /**
         * Handle the ignoring of the indicated action by the indicated player.
         */
        virtual void ignore(Being *const being,
                            const unsigned int flags) const = 0;
    protected:
        PlayerIgnoreStrategy() :
            mDescription(),
            mShortName()
        {
        }
};
#endif  // BEING_PLAYERIGNORESTRATEGY_H
