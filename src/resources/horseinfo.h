/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "localconsts.h"

class AnimatedSprite;

struct HorseInfo final
{
    HorseInfo() :
        downSprite(nullptr),
        upSprite(nullptr),
        downOffsetX(0),
        downOffsetY(0),
        upOffsetX(0),
        upOffsetY(0)
    { }

    A_DELETE_COPY(HorseInfo)

    AnimatedSprite *downSprite;
    AnimatedSprite *upSprite;
    int downOffsetX;
    int downOffsetY;
    int upOffsetX;
    int upOffsetY;
};

#endif  // RESOURCES_HORSEINFO_H
