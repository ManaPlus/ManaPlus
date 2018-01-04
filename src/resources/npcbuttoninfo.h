/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef RESOURCES_NPCBUTTONINFO_H
#define RESOURCES_NPCBUTTONINFO_H

#include <string>

#include "localconsts.h"

struct NpcButtonInfo final
{
    NpcButtonInfo() :
        name(),
        value(),
        image(),
        x(0),
        y(0),
        imageWidth(16),
        imageHeight(16)
    {
    }

    A_DELETE_COPY(NpcButtonInfo)

    std::string name;
    std::string value;
    std::string image;
    int x;
    int y;
    int imageWidth;
    int imageHeight;
};

#endif  // RESOURCES_NPCBUTTONINFO_H
