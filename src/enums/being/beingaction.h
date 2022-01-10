/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef ENUMS_BEING_BEINGACTION_H
#define ENUMS_BEING_BEINGACTION_H

#include "enums/simpletypes/enumdefines.h"

enumStart(BeingAction)
{
    STAND    = 0,
    MOVE     = 1,
    ATTACK   = 2,
    CAST     = 3,
    SIT      = 4,
    DEAD     = 5,
    HURT     = 6,
    SPAWN    = 7,
    PRESTAND = 8
}
enumEnd(BeingAction);

#endif  // ENUMS_BEING_BEINGACTION_H
