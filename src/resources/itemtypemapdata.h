/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#ifndef RESOURCES_ITEMTYPEMAPDATA_H
#define RESOURCES_ITEMTYPEMAPDATA_H

#include "localconsts.h"

#include "resources/itemtypemap.h"

ItemTypeMap itemTypeMap[] =
{
    {"generic", ItemType::UNUSABLE},
    {"other", ItemType::UNUSABLE},
    {"usable", ItemType::USABLE},
    {"equip-1hand", ItemType::EQUIPMENT_ONE_HAND_WEAPON},
    {"equip-2hand", ItemType::EQUIPMENT_TWO_HANDS_WEAPON},
    {"equip-torso", ItemType::EQUIPMENT_TORSO},
    {"equip-arms", ItemType::EQUIPMENT_ARMS},
    {"equip-head", ItemType::EQUIPMENT_HEAD},
    {"equip-legs", ItemType::EQUIPMENT_LEGS},
    {"equip-shield", ItemType::EQUIPMENT_SHIELD},
    {"equip-ring", ItemType::EQUIPMENT_RING},
    {"equip-charm", ItemType::EQUIPMENT_CHARM},
    {"equip-necklace", ItemType::EQUIPMENT_NECKLACE},
    {"equip-neck", ItemType::EQUIPMENT_NECKLACE},
    {"equip-feet", ItemType::EQUIPMENT_FEET},
    {"equip-ammo", ItemType::EQUIPMENT_AMMO},
    {"racesprite", ItemType::SPRITE_RACE},
    {"hairsprite", ItemType::SPRITE_HAIR},
};

#endif  // RESOURCES_ITEMTYPEMAPDATA_H
