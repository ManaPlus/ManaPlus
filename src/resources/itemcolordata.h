/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Aethyra Development Team
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

#ifndef RESOURCES_ITEMCOLORDATA_H
#define RESOURCES_ITEMCOLORDATA_H

#include "enums/simpletypes/itemcolor.h"

#include <string>

#include "localconsts.h"

class ItemColorData final
{
    public:
        ItemColorData() :
            id(ItemColor_zero),
            name(),
            color()
        { }

        ItemColorData(const ItemColor id0,
                      const std::string &name0,
                      const std::string &color0) :
            id(id0),
            name(name0),
            color(color0)
        {
        }

        A_DEFAULT_COPY(ItemColorData)

        ItemColor id;
        std::string name;
        std::string color;
};

#endif  // RESOURCES_ITEMCOLORDATA_H
