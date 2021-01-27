/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/setupactiondata.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

SetupActionData setupActionDataEmotes[] =
{
    {
        // TRANSLATORS: input action name
        N_("Emote modifiers keys"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Emote modifier key"),
        InputAction::EMOTE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet emote modifier key"),
        InputAction::PET_EMOTE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Homunculus / mercenary emote modifier key"),
        InputAction::HOMUN_EMOTE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emote shortcuts"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 1),
        InputAction::EMOTE_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 2),
        InputAction::EMOTE_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 3),
        InputAction::EMOTE_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 4),
        InputAction::EMOTE_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 5),
        InputAction::EMOTE_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 6),
        InputAction::EMOTE_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 7),
        InputAction::EMOTE_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 8),
        InputAction::EMOTE_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 9),
        InputAction::EMOTE_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 10),
        InputAction::EMOTE_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 11),
        InputAction::EMOTE_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 12),
        InputAction::EMOTE_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 13),
        InputAction::EMOTE_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 14),
        InputAction::EMOTE_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 15),
        InputAction::EMOTE_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 16),
        InputAction::EMOTE_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 17),
        InputAction::EMOTE_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 18),
        InputAction::EMOTE_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 19),
        InputAction::EMOTE_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 20),
        InputAction::EMOTE_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 21),
        InputAction::EMOTE_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 22),
        InputAction::EMOTE_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 23),
        InputAction::EMOTE_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 24),
        InputAction::EMOTE_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 25),
        InputAction::EMOTE_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 26),
        InputAction::EMOTE_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 27),
        InputAction::EMOTE_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 28),
        InputAction::EMOTE_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 29),
        InputAction::EMOTE_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 30),
        InputAction::EMOTE_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 31),
        InputAction::EMOTE_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 32),
        InputAction::EMOTE_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 33),
        InputAction::EMOTE_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 34),
        InputAction::EMOTE_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 35),
        InputAction::EMOTE_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 36),
        InputAction::EMOTE_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 37),
        InputAction::EMOTE_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 38),
        InputAction::EMOTE_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 39),
        InputAction::EMOTE_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 40),
        InputAction::EMOTE_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 41),
        InputAction::EMOTE_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 42),
        InputAction::EMOTE_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 43),
        InputAction::EMOTE_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 44),
        InputAction::EMOTE_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 45),
        InputAction::EMOTE_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 46),
        InputAction::EMOTE_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 47),
        InputAction::EMOTE_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 48),
        InputAction::EMOTE_48,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Pet emote shortcuts"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 1),
        InputAction::PET_EMOTE_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 2),
        InputAction::PET_EMOTE_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 3),
        InputAction::PET_EMOTE_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 4),
        InputAction::PET_EMOTE_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 5),
        InputAction::PET_EMOTE_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 6),
        InputAction::PET_EMOTE_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 7),
        InputAction::PET_EMOTE_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 8),
        InputAction::PET_EMOTE_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 9),
        InputAction::PET_EMOTE_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 10),
        InputAction::PET_EMOTE_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 11),
        InputAction::PET_EMOTE_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 12),
        InputAction::PET_EMOTE_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 13),
        InputAction::PET_EMOTE_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 14),
        InputAction::PET_EMOTE_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 15),
        InputAction::PET_EMOTE_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 16),
        InputAction::PET_EMOTE_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 17),
        InputAction::PET_EMOTE_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 18),
        InputAction::PET_EMOTE_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 19),
        InputAction::PET_EMOTE_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 20),
        InputAction::PET_EMOTE_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 21),
        InputAction::PET_EMOTE_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 22),
        InputAction::PET_EMOTE_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 23),
        InputAction::PET_EMOTE_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 24),
        InputAction::PET_EMOTE_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 25),
        InputAction::PET_EMOTE_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 26),
        InputAction::PET_EMOTE_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 27),
        InputAction::PET_EMOTE_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 28),
        InputAction::PET_EMOTE_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 29),
        InputAction::PET_EMOTE_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 30),
        InputAction::PET_EMOTE_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 31),
        InputAction::PET_EMOTE_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 32),
        InputAction::PET_EMOTE_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 33),
        InputAction::PET_EMOTE_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 34),
        InputAction::PET_EMOTE_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 35),
        InputAction::PET_EMOTE_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 36),
        InputAction::PET_EMOTE_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 37),
        InputAction::PET_EMOTE_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 38),
        InputAction::PET_EMOTE_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 39),
        InputAction::PET_EMOTE_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 40),
        InputAction::PET_EMOTE_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 41),
        InputAction::PET_EMOTE_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 42),
        InputAction::PET_EMOTE_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 43),
        InputAction::PET_EMOTE_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 44),
        InputAction::PET_EMOTE_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 45),
        InputAction::PET_EMOTE_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 46),
        InputAction::PET_EMOTE_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 47),
        InputAction::PET_EMOTE_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Pet emote shortcut %d"), 48),
        InputAction::PET_EMOTE_48,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    },
};
