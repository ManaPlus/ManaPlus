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

#ifndef BEING_BEINGFLAG_H
#define BEING_BEINGFLAG_H

namespace BeingFlag
{
    enum
    {
        SHOP = 1,
        AWAY = 2,
        INACTIVE = 4,
        GM = 64,
        GENDER_MALE = 128,
        SPECIAL = 128 + 64
    };
}  // namespace BeingFlag
#endif  // BEING_BEINGFLAG_H
