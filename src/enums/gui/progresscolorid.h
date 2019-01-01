/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef ENUMS_GUI_PROGRESSCOLORID_H
#define ENUMS_GUI_PROGRESSCOLORID_H

#include "enums/simpletypes/enumdefines.h"

enumStart(ProgressColorId)
{
    PROG_HP = 0,
    PROG_HP_POISON,
    PROG_MP,
    PROG_NO_MP,
    PROG_EXP,
    PROG_INVY_SLOTS,
    PROG_WEIGHT,
    PROG_JOB,
    PROG_UPDATE,
    PROG_MONEY,
    PROG_ARROWS,
    PROG_STATUS,
    THEME_PROG_END
}
enumEnd(ProgressColorId);

#endif  // ENUMS_GUI_PROGRESSCOLORID_H
