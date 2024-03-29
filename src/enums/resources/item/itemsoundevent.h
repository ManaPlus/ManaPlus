/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_ITEM_ITEMSOUNDEVENT_H
#define ENUMS_RESOURCES_ITEM_ITEMSOUNDEVENT_H

namespace ItemSoundEvent
{
    enum Type
    {
        HIT = 0,
        MISS,
        HURT,
        DIE,
        MOVE,
        SIT,
        SITTOP,
        SPAWN,
        DROP,
        PICKUP,
        TAKE,   // take from container
        PUT,    // put into container
        EQUIP,
        UNEQUIP,
        USE,
        USECARD
    };
}  // namespace ItemSoundEvent

#endif  // ENUMS_RESOURCES_ITEM_ITEMSOUNDEVENT_H
