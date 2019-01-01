/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2019  The ManaPlus Developers
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

#ifndef ITEMCOLORMANAGER_H
#define ITEMCOLORMANAGER_H

#include "enums/simpletypes/itemcolor.h"

#include "localconsts.h"

struct CardsList;

class ItemColorManager final
{
    public:
        A_DELETE_COPY(ItemColorManager)

        static ItemColor getColorFromCards(const int *const cards)
                                           A_WARN_UNUSED;

        static ItemColor getColorFromCards(const CardsList &cards)
                                           A_WARN_UNUSED;
};

#endif  // ITEMCOLORMANAGER_H
