/*
 *  The ManaPlus Client
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

SetupActionData setupActionDataCraft[] =
{
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 1),
        InputAction::CRAFT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 2),
        InputAction::CRAFT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 3),
        InputAction::CRAFT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 4),
        InputAction::CRAFT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 5),
        InputAction::CRAFT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 6),
        InputAction::CRAFT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 7),
        InputAction::CRAFT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 8),
        InputAction::CRAFT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Craft shortcut %d"), 9),
        InputAction::CRAFT_9,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    },
};
