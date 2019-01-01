/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

SetupActionData setupActionDataChat[] =
{
    {
        // TRANSLATORS: input action group
        N_("Main"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Toggle Chat"),
        InputAction::TOGGLE_CHAT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Chat modifier key"),
        InputAction::CHAT_MOD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Scroll Chat Up"),
        InputAction::SCROLL_CHAT_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Scroll Chat Down"),
        InputAction::SCROLL_CHAT_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action group
        N_("Tabs"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Chat Tab"),
        InputAction::PREV_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Chat Tab"),
        InputAction::NEXT_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Clear current chat tab"),
        InputAction::CLEAR_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Close current Chat Tab"),
        InputAction::CLOSE_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Close all chat tabs"),
        InputAction::CLOSE_ALL_CHAT_TABS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select general tab"),
        InputAction::CHAT_GENERAL_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select debug tab"),
        InputAction::CHAT_DEBUG_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select trade tab"),
        InputAction::CHAT_TRADE_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select battle tab"),
        InputAction::CHAT_BATTLE_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select gm tab"),
        InputAction::CHAT_GM_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select lang tab"),
        InputAction::CHAT_LANG_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select party tab"),
        InputAction::CHAT_PARTY_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select guild tab"),
        InputAction::CHAT_GUILD_TAB,
        "",
    },
    {
        // TRANSLATORS: input action group
        N_("Other"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ignore all whispers"),
        InputAction::IGNORE_ALL_WHISPERS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ignore all whispers on server side"),
        InputAction::SERVER_IGNORE_ALL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Unignore all whispers on server side"),
        InputAction::SERVER_UNIGNORE_ALL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous chat line"),
        InputAction::CHAT_PREV_HISTORY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next chat line"),
        InputAction::CHAT_NEXT_HISTORY,
        "",
    },
    {
        // TRANSLATORS: input action group
        N_("Smiles"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show smiles"),
        InputAction::GUI_F1,
        "",
    },
    {
        "F2",
        InputAction::GUI_F2,
        "",
    },
    {
        "F3",
        InputAction::GUI_F3,
        "",
    },
    {
        "F4",
        InputAction::GUI_F4,
        "",
    },
    {
        "F5",
        InputAction::GUI_F5,
        "",
    },
    {
        "F6",
        InputAction::GUI_F6,
        "",
    },
    {
        "F7",
        InputAction::GUI_F7,
        "",
    },
    {
        "F8",
        InputAction::GUI_F8,
        "",
    },
    {
        "F9",
        InputAction::GUI_F9,
        "",
    },
    {
        "F10",
        InputAction::GUI_F10,
        "",
    },
    {
        "F11",
        InputAction::GUI_F11,
        "",
    },
    {
        "F12",
        InputAction::GUI_F12,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
