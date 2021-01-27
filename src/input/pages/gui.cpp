/*
 *  The ManaPlus Client
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

#include "debug.h"

SetupActionData setupActionDataGui[] =
{
    {
        // TRANSLATORS: input action name
        N_("Move & selection"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Up"),
        InputAction::GUI_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Down"),
        InputAction::GUI_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Left"),
        InputAction::GUI_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Right"),
        InputAction::GUI_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Home"),
        InputAction::GUI_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move End"),
        InputAction::GUI_END,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Page up"),
        InputAction::GUI_PAGE_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Page down"),
        InputAction::GUI_PAGE_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Other"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select"),
        InputAction::GUI_SELECT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select2"),
        InputAction::GUI_SELECT2,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Cancel"),
        InputAction::GUI_CANCEL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Delete"),
        InputAction::GUI_DELETE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Backspace"),
        InputAction::GUI_BACKSPACE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Insert"),
        InputAction::GUI_INSERT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Tab"),
        InputAction::GUI_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Mod"),
        InputAction::GUI_MOD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ctrl"),
        InputAction::GUI_CTRL,
        "",
    },
    {
        "B",
        InputAction::GUI_B,
        "",
    },
    {
        "C",
        InputAction::GUI_C,
        "",
    },
    {
        "D",
        InputAction::GUI_D,
        "",
    },
    {
        "E",
        InputAction::GUI_E,
        "",
    },
    {
        "F",
        InputAction::GUI_F,
        "",
    },
    {
        "H",
        InputAction::GUI_H,
        "",
    },
    {
        "K",
        InputAction::GUI_K,
        "",
    },
    {
        "U",
        InputAction::GUI_U,
        "",
    },
    {
        "V",
        InputAction::GUI_V,
        "",
    },
    {
        "W",
        InputAction::GUI_W,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
