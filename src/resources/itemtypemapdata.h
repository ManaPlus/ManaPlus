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

#ifndef RESOURCES_ITEMTYPEMAPDATA_H
#define RESOURCES_ITEMTYPEMAPDATA_H

#include "utils/gettext.h"

#include "resources/itemtypemap.h"

#include "localconsts.h"

ItemTypeMap itemTypeMap[] =
{
    {"generic", ItemDbType::UNUSABLE,
        std::string(), std::string()
    },
    {"other", ItemDbType::UNUSABLE,
        // TRANSLATORS: inventory button
        N_("Use"), N_("Use")
    },
    {"usable", ItemDbType::USABLE,
        // TRANSLATORS: inventory button
        N_("Use"), N_("Use")
    },
    {"equip-1hand", ItemDbType::EQUIPMENT_ONE_HAND_WEAPON,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-2hand", ItemDbType::EQUIPMENT_TWO_HANDS_WEAPON,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-torso", ItemDbType::EQUIPMENT_TORSO,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-arms", ItemDbType::EQUIPMENT_ARMS,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-head", ItemDbType::EQUIPMENT_HEAD,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-legs", ItemDbType::EQUIPMENT_LEGS,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-shield", ItemDbType::EQUIPMENT_SHIELD,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-ring", ItemDbType::EQUIPMENT_RING,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-charm", ItemDbType::EQUIPMENT_CHARM,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-necklace", ItemDbType::EQUIPMENT_NECKLACE,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-neck", ItemDbType::EQUIPMENT_NECKLACE,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-feet", ItemDbType::EQUIPMENT_FEET,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"equip-ammo", ItemDbType::EQUIPMENT_AMMO,
        // TRANSLATORS: inventory button
        N_("Equip"), N_("Unequip")
    },
    {"card", ItemDbType::CARD,
        // TRANSLATORS: inventory button
        N_("Insert"), std::string()
    },
    {"racesprite", ItemDbType::SPRITE_RACE,
        std::string(), std::string()
    },
    {"hairsprite", ItemDbType::SPRITE_HAIR,
        std::string(), std::string()
    }
};

#endif  // RESOURCES_ITEMTYPEMAPDATA_H
