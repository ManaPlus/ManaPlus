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

#include "debug.h"

SetupActionData setupActionDataWindows[] =
{
    {
        // TRANSLATORS: input action label
        N_("Windows"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show Windows Menu"),
        InputAction::SHOW_WINDOWS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Hide Windows"),
        InputAction::HIDE_WINDOWS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("About Window"),
        InputAction::WINDOW_ABOUT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Bank Window"),
        InputAction::WINDOW_BANK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Help Window"),
        InputAction::WINDOW_HELP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Server Info Window"),
        InputAction::WINDOW_SERVER_INFO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Status Window"),
        InputAction::WINDOW_STATUS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Inventory Window"),
        InputAction::WINDOW_INVENTORY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Equipment Window"),
        InputAction::WINDOW_EQUIPMENT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Skill Window"),
        InputAction::WINDOW_SKILL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Minimap Window"),
        InputAction::WINDOW_MINIMAP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Chat Window"),
        InputAction::WINDOW_CHAT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Item Shortcut Window"),
        InputAction::WINDOW_SHORTCUT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Setup Window"),
        InputAction::WINDOW_SETUP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Debug Window"),
        InputAction::WINDOW_DEBUG,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Social Window"),
        InputAction::WINDOW_SOCIAL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emote Shortcut Window"),
        InputAction::WINDOW_EMOTE_SHORTCUT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Outfits Window"),
        InputAction::WINDOW_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Shop Window"),
        InputAction::WINDOW_SHOP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick drop Window"),
        InputAction::WINDOW_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Kill Stats Window"),
        InputAction::WINDOW_KILLS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Commands Window"),
        InputAction::WINDOW_SPELLS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Who Is Online Window"),
        InputAction::WINDOW_ONLINE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Did you know Window"),
        InputAction::WINDOW_DIDYOUKNOW,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quests Window"),
        InputAction::WINDOW_QUESTS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Updates Window"),
        InputAction::WINDOW_UPDATER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Cart window"),
        InputAction::WINDOW_CART,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick settings window"),
        InputAction::WINDOW_QUICK_SETTINGS,
        "",
    },
    {
        // TRANSLATORS: input action label
        N_("Tabs"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Social Tab"),
        InputAction::PREV_SOCIAL_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Social Tab"),
        InputAction::NEXT_SOCIAL_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Shortcuts tab"),
        InputAction::PREV_SHORTCUTS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Shortcuts tab"),
        InputAction::NEXT_SHORTCUTS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Commands tab"),
        InputAction::PREV_COMMANDS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Commands tab"),
        InputAction::NEXT_COMMANDS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous inventory tab"),
        InputAction::PREV_INV_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next inventory tab"),
        InputAction::NEXT_INV_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Clear drops window"),
        InputAction::DROP_CLEAR,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
