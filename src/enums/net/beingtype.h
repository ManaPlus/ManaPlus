/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef ENUMS_NET_BEINGTYPE_H
#define ENUMS_NET_BEINGTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(BeingType)
{
    PC        = 0,
    NPC       = 1,
    ITEM      = 2,
    SKILL     = 3,
    CHAT      = 4,
    MONSTER   = 5,
    NPC_EVENT = 6,
    PET       = 7,
    HOMUN     = 8,
    MERSOL    = 9,
    ELEMENTAL = 10
}
enumEnd(BeingType);

#endif  // ENUMS_NET_BEINGTYPE_H
