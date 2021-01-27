/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef RESOURCES_HORSEINFO_H
#define RESOURCES_HORSEINFO_H

#include "resources/sprite/spritereference.h"

#include "resources/horseoffset.h"

#include "localconsts.h"

struct HorseInfo final
{
    HorseInfo() :
        downSprites(),
        upSprites(),
        offsets()
    {
        for (int f = 0; f < 10; f ++)
        {
            offsets[f].upOffsetX = 0;
            offsets[f].upOffsetY = 0;
            offsets[f].downOffsetX = 0;
            offsets[f].downOffsetY = 0;
            offsets[f].riderOffsetX = 0;
            offsets[f].riderOffsetY = 0;
        }
    }

    A_DELETE_COPY(HorseInfo)

    STD_VECTOR<SpriteReference*> downSprites;
    STD_VECTOR<SpriteReference*> upSprites;
    HorseOffset offsets[10];  // by direction
};

#endif  // RESOURCES_HORSEINFO_H
