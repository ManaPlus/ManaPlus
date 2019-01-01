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

#ifndef ENUMS_MAGICSCHOOL_H
#define ENUMS_MAGICSCHOOL_H

#ifdef TMWA_SUPPORT

#include "enums/simpletypes/enumdefines.h"

enumStart(MagicSchool)
{
    SkillMagic          = 340,
    SkillMagicLife      = 341,
    SkillMagicWar       = 342,
    SkillMagicTransmute = 343,
    SkillMagicNature    = 344,
    SkillMagicAstral    = 345
}
enumEnd(MagicSchool);

#endif  // TMWA_SUPPORT
#endif  // ENUMS_MAGICSCHOOL_H
