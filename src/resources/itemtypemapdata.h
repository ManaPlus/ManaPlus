/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2015  The ManaPlus Developers
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

#include "utils/gettext.h"

#include "resources/itemtypemap.h"

#include "localconsts.h"

ItemTypeMap itemTypeMap[] =
{
    {"generic", ItemType::UNUSABLE,
        std::string(), std::string()
    },
    {"other", ItemType::UNUSABLE,
        // TRANSLATORS: inventory button
        N_("Use"), N_("Use")
    },
    {"usable", ItemType::USABLE,
        // TRANSLATORS: inventory button
        N_("Use"), N_("Use")
    },
    {"equip-1hand", ItemType::EQUIPMENT_ONE_HAND_WEAPON,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-2hand", ItemType::EQUIPMENT_TWO_HANDS_WEAPON,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-torso", ItemType::EQUIPMENT_TORSO,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-arms", ItemType::EQUIPMENT_ARMS,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-head", ItemType::EQUIPMENT_HEAD,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-legs", ItemType::EQUIPMENT_LEGS,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-shield", ItemType::EQUIPMENT_SHIELD,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-ring", ItemType::EQUIPMENT_RING,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-charm", ItemType::EQUIPMENT_CHARM,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-necklace", ItemType::EQUIPMENT_NECKLACE,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-neck", ItemType::EQUIPMENT_NECKLACE,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-feet", ItemType::EQUIPMENT_FEET,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-ammo", ItemType::EQUIPMENT_AMMO,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"card", ItemType::CARD,
        // TRANSLATORS: inventory button
        N_("Insert"), std::string()
    },
    {"racesprite", ItemType::SPRITE_RACE,
        std::string(), std::string()
    },
    {"hairsprite", ItemType::SPRITE_HAIR,
        std::string(), std::string()
    }
};

#endif  // RESOURCES_ITEMTYPEMAPDATA_H
