/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_MAP_MAPITEMTYPE_H
#define ENUMS_RESOURCES_MAP_MAPITEMTYPE_H

namespace MapItemType
{
    enum Type
    {
        EMPTY = 0,
        HOME = 1,
        ROAD = 2,
        CROSS = 3,
        ARROW_UP = 4,
        ARROW_DOWN = 5,
        ARROW_LEFT = 6,
        ARROW_RIGHT = 7,
        PORTAL = 8,
        MUSIC = 9,
        ATTACK = 10,
        PRIORITY = 11,
        IGNORE_ = 12,
        PICKUP = 13,
        NOPICKUP = 14,
        SEPARATOR = 15
    };
}  // namespace MapItemType

#endif  // ENUMS_RESOURCES_MAP_MAPITEMTYPE_H
