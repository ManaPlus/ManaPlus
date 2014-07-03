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

#include "utils/gettext.h"

#include "resources/itemtypemap.h"

ItemTypeMap itemTypeMap[] =
{
    // TRANSLATORS: inventory button
    {"generic", ItemType::UNUSABLE, N_("Use")},
    // TRANSLATORS: inventory button
    {"other", ItemType::UNUSABLE, N_("Use")},
    // TRANSLATORS: inventory button
    {"usable", ItemType::USABLE, N_("Use")},
    // TRANSLATORS: inventory button
    {"equip-1hand", ItemType::EQUIPMENT_ONE_HAND_WEAPON, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-2hand", ItemType::EQUIPMENT_TWO_HANDS_WEAPON, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-torso", ItemType::EQUIPMENT_TORSO, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-arms", ItemType::EQUIPMENT_ARMS, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-head", ItemType::EQUIPMENT_HEAD, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-legs", ItemType::EQUIPMENT_LEGS, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-shield", ItemType::EQUIPMENT_SHIELD, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-ring", ItemType::EQUIPMENT_RING, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-charm", ItemType::EQUIPMENT_CHARM, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-necklace", ItemType::EQUIPMENT_NECKLACE, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-neck", ItemType::EQUIPMENT_NECKLACE, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-feet", ItemType::EQUIPMENT_FEET, N_("Equip")},
    // TRANSLATORS: inventory button
    {"equip-ammo", ItemType::EQUIPMENT_AMMO, N_("Equip")},
    // TRANSLATORS: inventory button
    {"racesprite", ItemType::SPRITE_RACE, N_("Use")},
    // TRANSLATORS: inventory button
    {"hairsprite", ItemType::SPRITE_HAIR, N_("Use")},
};

#endif  // RESOURCES_ITEMTYPEMAPDATA_H
