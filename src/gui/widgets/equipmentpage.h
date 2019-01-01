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

#ifndef GUI_WIDGETS_EQUIPMENTPAGE_H
#define GUI_WIDGETS_EQUIPMENTPAGE_H

#include "utils/vector.h"

#include "localconsts.h"

class PlayerBox;

struct EquipmentBox;

struct EquipmentPage final
{
    EquipmentPage() :
        boxes(),
        x(0),
        y(0),
        width(0),
        height(0),
        showPlayerBox(true)
    { }

    A_DELETE_COPY(EquipmentPage)

    STD_VECTOR<EquipmentBox*> boxes;
    int x;
    int y;
    int width;
    int height;
    bool showPlayerBox;
};

#endif  // GUI_WIDGETS_EQUIPMENTPAGE_H
