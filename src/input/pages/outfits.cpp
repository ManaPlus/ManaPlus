/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/setupactiondata.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

SetupActionData setupActionDataOutfits[] =
{
    {
        // TRANSLATORS: input action name
        N_("Outfits keys"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Wear outfit"),
        InputAction::WEAR_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Clear outfit"),
        InputAction::CLEAR_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy Outfit to other outfit"),
        InputAction::COPY_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy outfit to chat"),
        InputAction::COPY_OUTFIT_TO_CHAT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy equipped to outfit"),
        InputAction::COPY_EQUIPED_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show hat in chat"),
        InputAction::HAT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Outfits shortcuts"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 1),
        InputAction::OUTFIT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 2),
        InputAction::OUTFIT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 3),
        InputAction::OUTFIT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 4),
        InputAction::OUTFIT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 5),
        InputAction::OUTFIT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 6),
        InputAction::OUTFIT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 7),
        InputAction::OUTFIT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 8),
        InputAction::OUTFIT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 9),
        InputAction::OUTFIT_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 10),
        InputAction::OUTFIT_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 11),
        InputAction::OUTFIT_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 12),
        InputAction::OUTFIT_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 13),
        InputAction::OUTFIT_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 14),
        InputAction::OUTFIT_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 15),
        InputAction::OUTFIT_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 16),
        InputAction::OUTFIT_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 17),
        InputAction::OUTFIT_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 18),
        InputAction::OUTFIT_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 19),
        InputAction::OUTFIT_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 20),
        InputAction::OUTFIT_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 21),
        InputAction::OUTFIT_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 22),
        InputAction::OUTFIT_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 23),
        InputAction::OUTFIT_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 24),
        InputAction::OUTFIT_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 25),
        InputAction::OUTFIT_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 26),
        InputAction::OUTFIT_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 27),
        InputAction::OUTFIT_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 28),
        InputAction::OUTFIT_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 29),
        InputAction::OUTFIT_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 30),
        InputAction::OUTFIT_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 31),
        InputAction::OUTFIT_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 32),
        InputAction::OUTFIT_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 33),
        InputAction::OUTFIT_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 34),
        InputAction::OUTFIT_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 35),
        InputAction::OUTFIT_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 36),
        InputAction::OUTFIT_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 37),
        InputAction::OUTFIT_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 38),
        InputAction::OUTFIT_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 39),
        InputAction::OUTFIT_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 40),
        InputAction::OUTFIT_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 41),
        InputAction::OUTFIT_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 42),
        InputAction::OUTFIT_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 43),
        InputAction::OUTFIT_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 44),
        InputAction::OUTFIT_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 45),
        InputAction::OUTFIT_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 46),
        InputAction::OUTFIT_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 47),
        InputAction::OUTFIT_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 48),
        InputAction::OUTFIT_48,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
