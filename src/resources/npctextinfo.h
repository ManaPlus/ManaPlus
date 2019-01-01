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

#ifndef RESOURCES_NPCTEXTINFO_H
#define RESOURCES_NPCTEXTINFO_H

#include <string>

#include "localconsts.h"

struct NpcTextInfo final
{
    NpcTextInfo() :
        text(),
        x(0),
        y(0),
        width(32),
        height(32)
    {
    }

    A_DELETE_COPY(NpcTextInfo)

    std::string text;
    int x;
    int y;
    int width;
    int height;
};

#endif  // RESOURCES_NPCTEXTINFO_H
