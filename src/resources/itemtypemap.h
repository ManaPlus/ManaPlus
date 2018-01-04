/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2018  The ManaPlus Developers
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

#ifndef RESOURCES_ITEMTYPEMAP_H
#define RESOURCES_ITEMTYPEMAP_H

#include "enums/resources/item/itemdbtype.h"

#include <string>

#include "localconsts.h"

struct ItemTypeMap final
{
    A_DEFAULT_COPY(ItemTypeMap)

    std::string name;
    ItemDbTypeT type;
    std::string useButton;
    std::string useButton2;
};

#endif  // RESOURCES_ITEMTYPEMAP_H
