/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_CHARACTER_H
#define NET_CHARACTER_H

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "utils/delete2.h"

#include "localconsts.h"

namespace Net
{

/**
 * A structure to hold information about a character.
 */
struct Character final
{
    Character() :
        dummy(nullptr),
        data(),
        slot(0U)
    {
    }

    A_DELETE_COPY(Character)

    ~Character()
    {
        delete2(dummy)
    }

    LocalPlayer *dummy;  /**< A dummy representing this character */
    PlayerInfoBackend data;
    uint16_t slot;       /**< The index in the list of characters */
};

typedef std::list<Character*> Characters;

}  // namespace Net

#endif  // NET_CHARACTER_H
