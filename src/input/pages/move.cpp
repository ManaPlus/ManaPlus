/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

SetupActionData setupActionDataMove[] =
{
    {
        // TRANSLATORS: input action name
        N_("Move Keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move player up"),
        InputAction::MOVE_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move player down"),
        InputAction::MOVE_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move player left"),
        InputAction::MOVE_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move player right"),
        InputAction::MOVE_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move player forward"),
        InputAction::MOVE_FORWARD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move pet up"),
        InputAction::PET_MOVE_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move pet down"),
        InputAction::PET_MOVE_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move pet left"),
        InputAction::PET_MOVE_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move pet right"),
        InputAction::PET_MOVE_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move mercenary to master"),
        InputAction::MERCENARY_TO_MASTER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to navigation point shortcuts"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 1),
        InputAction::MOVE_TO_POINT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 2),
        InputAction::MOVE_TO_POINT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 3),
        InputAction::MOVE_TO_POINT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 4),
        InputAction::MOVE_TO_POINT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 5),
        InputAction::MOVE_TO_POINT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 6),
        InputAction::MOVE_TO_POINT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 7),
        InputAction::MOVE_TO_POINT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 8),
        InputAction::MOVE_TO_POINT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 9),
        InputAction::MOVE_TO_POINT_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 10),
        InputAction::MOVE_TO_POINT_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 11),
        InputAction::MOVE_TO_POINT_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 12),
        InputAction::MOVE_TO_POINT_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 13),
        InputAction::MOVE_TO_POINT_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 14),
        InputAction::MOVE_TO_POINT_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 15),
        InputAction::MOVE_TO_POINT_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 16),
        InputAction::MOVE_TO_POINT_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 17),
        InputAction::MOVE_TO_POINT_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 18),
        InputAction::MOVE_TO_POINT_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 19),
        InputAction::MOVE_TO_POINT_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 20),
        InputAction::MOVE_TO_POINT_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 21),
        InputAction::MOVE_TO_POINT_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 22),
        InputAction::MOVE_TO_POINT_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 23),
        InputAction::MOVE_TO_POINT_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 24),
        InputAction::MOVE_TO_POINT_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 25),
        InputAction::MOVE_TO_POINT_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 26),
        InputAction::MOVE_TO_POINT_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 27),
        InputAction::MOVE_TO_POINT_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 28),
        InputAction::MOVE_TO_POINT_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 29),
        InputAction::MOVE_TO_POINT_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 30),
        InputAction::MOVE_TO_POINT_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 31),
        InputAction::MOVE_TO_POINT_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 32),
        InputAction::MOVE_TO_POINT_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 33),
        InputAction::MOVE_TO_POINT_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 34),
        InputAction::MOVE_TO_POINT_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 35),
        InputAction::MOVE_TO_POINT_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 36),
        InputAction::MOVE_TO_POINT_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 37),
        InputAction::MOVE_TO_POINT_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 38),
        InputAction::MOVE_TO_POINT_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 39),
        InputAction::MOVE_TO_POINT_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 40),
        InputAction::MOVE_TO_POINT_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 41),
        InputAction::MOVE_TO_POINT_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 42),
        InputAction::MOVE_TO_POINT_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 43),
        InputAction::MOVE_TO_POINT_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 44),
        InputAction::MOVE_TO_POINT_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 45),
        InputAction::MOVE_TO_POINT_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 46),
        InputAction::MOVE_TO_POINT_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 47),
        InputAction::MOVE_TO_POINT_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 48),
        InputAction::MOVE_TO_POINT_48,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
