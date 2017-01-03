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

SetupActionData setupActionDataShortcuts[] =
{
    {
        // TRANSLATORS: input action name
        N_("Shortcuts modifiers keys"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Item Shortcuts Key"),
        InputAction::SHORTCUTS_KEY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Shortcuts keys"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 1),
        InputAction::SHORTCUT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 2),
        InputAction::SHORTCUT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 3),
        InputAction::SHORTCUT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 4),
        InputAction::SHORTCUT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 5),
        InputAction::SHORTCUT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 6),
        InputAction::SHORTCUT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 7),
        InputAction::SHORTCUT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 8),
        InputAction::SHORTCUT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 9),
        InputAction::SHORTCUT_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 10),
        InputAction::SHORTCUT_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 11),
        InputAction::SHORTCUT_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 12),
        InputAction::SHORTCUT_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 13),
        InputAction::SHORTCUT_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 14),
        InputAction::SHORTCUT_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 15),
        InputAction::SHORTCUT_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 16),
        InputAction::SHORTCUT_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 17),
        InputAction::SHORTCUT_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 18),
        InputAction::SHORTCUT_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 19),
        InputAction::SHORTCUT_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 20),
        InputAction::SHORTCUT_20,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
