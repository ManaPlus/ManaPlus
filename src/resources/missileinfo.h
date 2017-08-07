/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RESOURCES_MISSILEINFO_H
#define RESOURCES_MISSILEINFO_H

#include <string>

#include "localconsts.h"

struct MissileInfo final
{
    MissileInfo() :
        particle(),
        z(32.0F),
        speed(7.0F),
        dieDistance(8.0F),
        lifeTime(3000)
    { }

    A_DELETE_COPY(MissileInfo)

    std::string particle;
    float z;
    float speed;
    float dieDistance;
    int lifeTime;
};

#endif  // RESOURCES_MISSILEINFO_H
