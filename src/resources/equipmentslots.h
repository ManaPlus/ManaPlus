/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef RESOURCES_EQUIPMENTSLOTS_H
#define RESOURCES_EQUIPMENTSLOTS_H

#include "localconsts.h"

struct EquipmentSlotMap final
{
    const char *const name;
    const int id;
};

static const EquipmentSlotMap equipmentSlots[] =
{
    {"topclothes",    0},
    {"top",           0},
    {"torso",         0},
    {"body",          0},
    {"slot0",         0},
    {"glove",         1},
    {"gloves",        1},
    {"slot1",         1},
    {"hat",           2},
    {"hats",          2},
    {"slot2",         2},
    {"bottomclothes", 3},
    {"bottom",        3},
    {"pants",         3},
    {"slot3",         3},
    {"shoes",         4},
    {"boot",          4},
    {"boots",         4},
    {"slot4",         4},
    {"misc1",         5},
    {"cape",          5},
    {"slot5",         5},
    {"wings",         6},
    {"slot6",         6},
    {"misc2",         7},
    {"scarf",         7},
    {"scarfs",        7},
    {"slot7",         7},
    {"weapon",        8},
    {"weapons",       8},
    {"slot8",         8},
    {"shield",        9},
    {"shields",       9},
    {"slot9",         9},
    {"arrow",         10},
    {"arrows",        10},
    {"ammo",          10},
    {"slot10",        10},
    {"amulet",        11},
    {"amulets",       11},
    {"slot11",        11},
    {"ring",          12},
    {"rings",         12},
    {"slot12",        12},
    {"slot13",        13},
    {"slot14",        14},
    {"slot15",        15},
    {"slot16",        16},
    {"slot17",        17},
    {"slot18",        18},
    {"slot19",        19},
    {"slot20",        20},
    {"slot21",        21},
    {"slot22",        22},
    {"slot23",        23},
    {"slot24",        24},
    {"slot25",        25},
    {"slot26",        26}
};

#endif  // RESOURCES_EQUIPMENTSLOTS_H
