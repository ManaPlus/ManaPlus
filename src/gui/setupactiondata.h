/*
 *  Setup action data configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_SETUPACTIONDATA_H
#define GUI_SETUPACTIONDATA_H

#include "input/keydata.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <string>

struct SetupActionData final
{
    std::string name;
    const int actionId;
    std::string text;
};

static SetupActionData setupActionData0[] =
{
    {
        // TRANSLATORS: input action name
        N_("Target and attack keys"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Attack"),
        Input::KEY_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target & Attack"),
        Input::KEY_TARGET_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to Target"),
        Input::KEY_MOVE_TO_TARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Move to Target type"),
        Input::KEY_CHANGE_MOVE_TO_TARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to Home location"),
        Input::KEY_MOVE_TO_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Set home location"),
        Input::KEY_SET_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to navigation point"),
        Input::KEY_MOVE_TO_POINT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Talk"),
        Input::KEY_TALK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Stop Attack / Modifier key"),
        Input::KEY_STOP_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Untarget"),
        Input::KEY_UNTARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target monster"),
        Input::KEY_TARGET_MONSTER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target NPC"),
        Input::KEY_TARGET_NPC,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target Player"),
        Input::KEY_TARGET_PLAYER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Other Keys"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pickup"),
        Input::KEY_PICKUP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Pickup Type"),
        Input::KEY_CHANGE_PICKUP_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Sit"),
        Input::KEY_SIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Screenshot"),
        Input::KEY_SCREENSHOT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Enable/Disable Trading"),
        Input::KEY_TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Open trade window"),
        Input::KEY_OPEN_TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Map View Mode"),
        Input::KEY_PATHFIND,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select OK"),
        Input::KEY_OK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quit"),
        Input::KEY_QUIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Stop or sit"),
        Input::KEY_STOP_SIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Return to safe video mode"),
        Input::KEY_SAFE_VIDEO,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData1[] =
{
    {
        // TRANSLATORS: input action name
        N_("Shortcuts modifiers keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Item Shortcuts Key"),
        Input::KEY_SHORTCUTS_KEY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Shortcuts keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 1),
        Input::KEY_SHORTCUT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 2),
        Input::KEY_SHORTCUT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 3),
        Input::KEY_SHORTCUT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 4),
        Input::KEY_SHORTCUT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 5),
        Input::KEY_SHORTCUT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 6),
        Input::KEY_SHORTCUT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 7),
        Input::KEY_SHORTCUT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 8),
        Input::KEY_SHORTCUT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 9),
        Input::KEY_SHORTCUT_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 10),
        Input::KEY_SHORTCUT_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 11),
        Input::KEY_SHORTCUT_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 12),
        Input::KEY_SHORTCUT_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 13),
        Input::KEY_SHORTCUT_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 14),
        Input::KEY_SHORTCUT_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 15),
        Input::KEY_SHORTCUT_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 16),
        Input::KEY_SHORTCUT_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 17),
        Input::KEY_SHORTCUT_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 18),
        Input::KEY_SHORTCUT_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 19),
        Input::KEY_SHORTCUT_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Item Shortcut %d"), 20),
        Input::KEY_SHORTCUT_20,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData2[] =
{
    {
        // TRANSLATORS: input action name
        N_("Show Windows Menu"),
        Input::KEY_SHOW_WINDOWS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Hide Windows"),
        Input::KEY_HIDE_WINDOWS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Help Window"),
        Input::KEY_WINDOW_HELP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Status Window"),
        Input::KEY_WINDOW_STATUS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Inventory Window"),
        Input::KEY_WINDOW_INVENTORY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Equipment Window"),
        Input::KEY_WINDOW_EQUIPMENT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Skill Window"),
        Input::KEY_WINDOW_SKILL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Minimap Window"),
        Input::KEY_WINDOW_MINIMAP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Chat Window"),
        Input::KEY_WINDOW_CHAT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Item Shortcut Window"),
        Input::KEY_WINDOW_SHORTCUT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Setup Window"),
        Input::KEY_WINDOW_SETUP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Debug Window"),
        Input::KEY_WINDOW_DEBUG,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Social Window"),
        Input::KEY_WINDOW_SOCIAL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emote Shortcut Window"),
        Input::KEY_WINDOW_EMOTE_SHORTCUT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Outfits Window"),
        Input::KEY_WINDOW_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Shop Window"),
        Input::KEY_WINDOW_SHOP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick drop Window"),
        Input::KEY_WINDOW_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Kill Stats Window"),
        Input::KEY_WINDOW_KILLS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Commands Window"),
        Input::KEY_WINDOW_SPELLS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Bot Checker Window"),
        Input::KEY_WINDOW_BOT_CHECKER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Who Is Online Window"),
        Input::KEY_WINDOW_ONLINE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Did you know Window"),
        Input::KEY_WINDOW_DIDYOUKNOW,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quests Window"),
        Input::KEY_WINDOW_QUESTS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Social Tab"),
        Input::KEY_PREV_SOCIAL_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Social Tab"),
        Input::KEY_NEXT_SOCIAL_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Shortcuts tab"),
        Input::KEY_PREV_SHORTCUTS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Shortcuts tab"),
        Input::KEY_NEXT_SHORTCUTS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Commands tab"),
        Input::KEY_PREV_COMMANDS_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Commands tab"),
        Input::KEY_NEXT_COMMANDS_TAB,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData3[] =
{
    {
        // TRANSLATORS: input action name
        N_("Emote modifiers keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Emote modifier key"),
        Input::KEY_EMOTE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emote shortcuts"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 1),
        Input::KEY_EMOTE_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 2),
        Input::KEY_EMOTE_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 3),
        Input::KEY_EMOTE_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 4),
        Input::KEY_EMOTE_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 5),
        Input::KEY_EMOTE_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 6),
        Input::KEY_EMOTE_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 7),
        Input::KEY_EMOTE_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 8),
        Input::KEY_EMOTE_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 9),
        Input::KEY_EMOTE_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 10),
        Input::KEY_EMOTE_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 11),
        Input::KEY_EMOTE_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 12),
        Input::KEY_EMOTE_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 13),
        Input::KEY_EMOTE_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 14),
        Input::KEY_EMOTE_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 15),
        Input::KEY_EMOTE_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 16),
        Input::KEY_EMOTE_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 17),
        Input::KEY_EMOTE_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 18),
        Input::KEY_EMOTE_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 19),
        Input::KEY_EMOTE_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 20),
        Input::KEY_EMOTE_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 21),
        Input::KEY_EMOTE_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 22),
        Input::KEY_EMOTE_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 23),
        Input::KEY_EMOTE_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 24),
        Input::KEY_EMOTE_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 25),
        Input::KEY_EMOTE_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 26),
        Input::KEY_EMOTE_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 27),
        Input::KEY_EMOTE_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 28),
        Input::KEY_EMOTE_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 29),
        Input::KEY_EMOTE_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 30),
        Input::KEY_EMOTE_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 31),
        Input::KEY_EMOTE_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 32),
        Input::KEY_EMOTE_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 33),
        Input::KEY_EMOTE_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 34),
        Input::KEY_EMOTE_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 35),
        Input::KEY_EMOTE_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 36),
        Input::KEY_EMOTE_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 37),
        Input::KEY_EMOTE_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 38),
        Input::KEY_EMOTE_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 39),
        Input::KEY_EMOTE_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 40),
        Input::KEY_EMOTE_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 41),
        Input::KEY_EMOTE_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 42),
        Input::KEY_EMOTE_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 43),
        Input::KEY_EMOTE_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 44),
        Input::KEY_EMOTE_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 45),
        Input::KEY_EMOTE_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 46),
        Input::KEY_EMOTE_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 47),
        Input::KEY_EMOTE_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Emote Shortcut %d"), 48),
        Input::KEY_EMOTE_48,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData4[] =
{
    {
        // TRANSLATORS: input action name
        N_("Outfits keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Wear Outfit"),
        Input::KEY_WEAR_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy Outfit"),
        Input::KEY_COPY_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy equipped to Outfit"),
        Input::KEY_COPY_EQUIPED_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Outfits shortcuts"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 1),
        Input::KEY_OUTFIT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 2),
        Input::KEY_OUTFIT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 3),
        Input::KEY_OUTFIT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 4),
        Input::KEY_OUTFIT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 5),
        Input::KEY_OUTFIT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 6),
        Input::KEY_OUTFIT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 7),
        Input::KEY_OUTFIT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 8),
        Input::KEY_OUTFIT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 9),
        Input::KEY_OUTFIT_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 10),
        Input::KEY_OUTFIT_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 11),
        Input::KEY_OUTFIT_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 12),
        Input::KEY_OUTFIT_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 13),
        Input::KEY_OUTFIT_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 14),
        Input::KEY_OUTFIT_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 15),
        Input::KEY_OUTFIT_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 16),
        Input::KEY_OUTFIT_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 17),
        Input::KEY_OUTFIT_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 18),
        Input::KEY_OUTFIT_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 19),
        Input::KEY_OUTFIT_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 20),
        Input::KEY_OUTFIT_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 21),
        Input::KEY_OUTFIT_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 22),
        Input::KEY_OUTFIT_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 23),
        Input::KEY_OUTFIT_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 24),
        Input::KEY_OUTFIT_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 25),
        Input::KEY_OUTFIT_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 26),
        Input::KEY_OUTFIT_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 27),
        Input::KEY_OUTFIT_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 28),
        Input::KEY_OUTFIT_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 29),
        Input::KEY_OUTFIT_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 30),
        Input::KEY_OUTFIT_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 31),
        Input::KEY_OUTFIT_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 32),
        Input::KEY_OUTFIT_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 33),
        Input::KEY_OUTFIT_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 34),
        Input::KEY_OUTFIT_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 35),
        Input::KEY_OUTFIT_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 36),
        Input::KEY_OUTFIT_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 37),
        Input::KEY_OUTFIT_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 38),
        Input::KEY_OUTFIT_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 39),
        Input::KEY_OUTFIT_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 40),
        Input::KEY_OUTFIT_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 41),
        Input::KEY_OUTFIT_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 42),
        Input::KEY_OUTFIT_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 43),
        Input::KEY_OUTFIT_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 44),
        Input::KEY_OUTFIT_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 45),
        Input::KEY_OUTFIT_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 46),
        Input::KEY_OUTFIT_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 47),
        Input::KEY_OUTFIT_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Outfit Shortcut %d"), 48),
        Input::KEY_OUTFIT_48,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData5[] =
{
    {
        // TRANSLATORS: input action name
        N_("Toggle Chat"),
        Input::KEY_TOGGLE_CHAT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Scroll Chat Up"),
        Input::KEY_SCROLL_CHAT_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Scroll Chat Down"),
        Input::KEY_SCROLL_CHAT_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous Chat Tab"),
        Input::KEY_PREV_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next Chat Tab"),
        Input::KEY_NEXT_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Close current Chat Tab"),
        Input::KEY_CLOSE_CHAT_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Previous chat line"),
        Input::KEY_CHAT_PREV_HISTORY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Next chat line"),
        Input::KEY_CHAT_NEXT_HISTORY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Chat modifier key"),
        Input::KEY_CHAT_MOD,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData6[] =
{
    {
        // TRANSLATORS: input action name
        N_("Ignore input 1"),
        Input::KEY_IGNORE_INPUT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ignore input 2"),
        Input::KEY_IGNORE_INPUT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Up"),
        Input::KEY_DIRECT_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Down"),
        Input::KEY_DIRECT_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Left"),
        Input::KEY_DIRECT_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Right"),
        Input::KEY_DIRECT_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Crazy moves"),
        Input::KEY_CRAZY_MOVES,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Crazy Move mode"),
        Input::KEY_CHANGE_CRAZY_MOVES_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick Drop N Items from 0 slot"),
        Input::KEY_QUICK_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick Drop N Items"),
        Input::KEY_QUICK_DROPN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch Quick Drop Counter"),
        Input::KEY_SWITCH_QUICK_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick heal target or self"),
        Input::KEY_MAGIC_INMA1,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Use #itenplz spell"),
        Input::KEY_MAGIC_ITENPLZ,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Use magic attack"),
        Input::KEY_MAGIC_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch magic attack"),
        Input::KEY_SWITCH_MAGIC_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch pvp attack"),
        Input::KEY_SWITCH_PVP_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change move type"),
        Input::KEY_INVERT_DIRECTION,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Attack Weapon Type"),
        Input::KEY_CHANGE_ATTACK_WEAPON_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Attack Type"),
        Input::KEY_CHANGE_ATTACK_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Follow mode"),
        Input::KEY_CHANGE_FOLLOW_MODE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Imitation mode"),
        Input::KEY_CHANGE_IMITATION_MODE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Disable / Enable Game modifier keys"),
        Input::KEY_DISABLE_GAME_MODIFIERS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("On / Off audio"),
        Input::KEY_CHANGE_AUDIO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Enable / Disable away mode"),
        Input::KEY_AWAY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emulate right click from keyboard"),
        Input::KEY_RIGHT_CLICK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Toggle camera mode"),
        Input::KEY_CAMERA,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show onscreen keyboard"),
        Input::KEY_SHOW_KEYBOARD,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData7[] =
{
    {
        // TRANSLATORS: input action name
        N_("Move Keys"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Up"),
        Input::KEY_MOVE_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Down"),
        Input::KEY_MOVE_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Left"),
        Input::KEY_MOVE_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Right"),
        Input::KEY_MOVE_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Forward"),
        Input::KEY_MOVE_FORWARD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to navigation point shortcuts"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 1),
        Input::KEY_MOVE_TO_POINT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 2),
        Input::KEY_MOVE_TO_POINT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 3),
        Input::KEY_MOVE_TO_POINT_3,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 4),
        Input::KEY_MOVE_TO_POINT_4,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 5),
        Input::KEY_MOVE_TO_POINT_5,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 6),
        Input::KEY_MOVE_TO_POINT_6,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 7),
        Input::KEY_MOVE_TO_POINT_7,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 8),
        Input::KEY_MOVE_TO_POINT_8,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 9),
        Input::KEY_MOVE_TO_POINT_9,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 10),
        Input::KEY_MOVE_TO_POINT_10,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 11),
        Input::KEY_MOVE_TO_POINT_11,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 12),
        Input::KEY_MOVE_TO_POINT_12,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 13),
        Input::KEY_MOVE_TO_POINT_13,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 14),
        Input::KEY_MOVE_TO_POINT_14,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 15),
        Input::KEY_MOVE_TO_POINT_15,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 16),
        Input::KEY_MOVE_TO_POINT_16,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 17),
        Input::KEY_MOVE_TO_POINT_17,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 18),
        Input::KEY_MOVE_TO_POINT_18,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 19),
        Input::KEY_MOVE_TO_POINT_19,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 20),
        Input::KEY_MOVE_TO_POINT_20,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 21),
        Input::KEY_MOVE_TO_POINT_21,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 22),
        Input::KEY_MOVE_TO_POINT_22,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 23),
        Input::KEY_MOVE_TO_POINT_23,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 24),
        Input::KEY_MOVE_TO_POINT_24,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 25),
        Input::KEY_MOVE_TO_POINT_25,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 26),
        Input::KEY_MOVE_TO_POINT_26,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 27),
        Input::KEY_MOVE_TO_POINT_27,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 28),
        Input::KEY_MOVE_TO_POINT_28,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 29),
        Input::KEY_MOVE_TO_POINT_29,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 30),
        Input::KEY_MOVE_TO_POINT_30,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 31),
        Input::KEY_MOVE_TO_POINT_31,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 32),
        Input::KEY_MOVE_TO_POINT_32,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 33),
        Input::KEY_MOVE_TO_POINT_33,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 34),
        Input::KEY_MOVE_TO_POINT_34,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 35),
        Input::KEY_MOVE_TO_POINT_35,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 36),
        Input::KEY_MOVE_TO_POINT_36,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 37),
        Input::KEY_MOVE_TO_POINT_37,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 38),
        Input::KEY_MOVE_TO_POINT_38,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 39),
        Input::KEY_MOVE_TO_POINT_39,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 40),
        Input::KEY_MOVE_TO_POINT_40,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 41),
        Input::KEY_MOVE_TO_POINT_41,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 42),
        Input::KEY_MOVE_TO_POINT_42,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 43),
        Input::KEY_MOVE_TO_POINT_43,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 44),
        Input::KEY_MOVE_TO_POINT_44,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 45),
        Input::KEY_MOVE_TO_POINT_45,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 46),
        Input::KEY_MOVE_TO_POINT_46,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 47),
        Input::KEY_MOVE_TO_POINT_47,
        "",
    },
    {
        // TRANSLATORS: input action name
        strprintf(N_("Move to point Shortcut %d"), 48),
        Input::KEY_MOVE_TO_POINT_48,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData8[] =
{
    {
        // TRANSLATORS: input action name
        N_("Move & selection"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Up"),
        Input::KEY_GUI_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Down"),
        Input::KEY_GUI_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Left"),
        Input::KEY_GUI_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Right"),
        Input::KEY_GUI_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Home"),
        Input::KEY_GUI_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move End"),
        Input::KEY_GUI_END,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Page up"),
        Input::KEY_GUI_PAGE_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Page down"),
        Input::KEY_GUI_PAGE_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Other"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select"),
        Input::KEY_GUI_SELECT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select2"),
        Input::KEY_GUI_SELECT2,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Cancel"),
        Input::KEY_GUI_CANCEL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Delete"),
        Input::KEY_GUI_DELETE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Backspace"),
        Input::KEY_GUI_BACKSPACE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Insert"),
        Input::KEY_GUI_INSERT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Tab"),
        Input::KEY_GUI_TAB,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Mod"),
        Input::KEY_GUI_MOD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ctrl"),
        Input::KEY_GUI_CTRL,
        "",
    },
    {
        "B",
        Input::KEY_GUI_B,
        "",
    },
    {
        "C",
        Input::KEY_GUI_C,
        "",
    },
    {
        "D",
        Input::KEY_GUI_D,
        "",
    },
    {
        "E",
        Input::KEY_GUI_E,
        "",
    },
    {
        "F",
        Input::KEY_GUI_F,
        "",
    },
    {
        "H",
        Input::KEY_GUI_H,
        "",
    },
    {
        "K",
        Input::KEY_GUI_K,
        "",
    },
    {
        "U",
        Input::KEY_GUI_U,
        "",
    },
    {
        "V",
        Input::KEY_GUI_V,
        "",
    },
    {
        "W",
        Input::KEY_GUI_W,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData *const setupActionData[] =
{
    setupActionData0,
    setupActionData7,
    setupActionData1,
    setupActionData2,
    setupActionData3,
    setupActionData4,
    setupActionData5,
    setupActionData6,
    setupActionData8
};

static const char *const pages[] =
{
    // TRANSLATORS: input tab sub tab name
    N_("Basic"),
    // TRANSLATORS: input tab sub tab name
    N_("Move"),
    // TRANSLATORS: input tab sub tab name
    N_("Shortcuts"),
    // TRANSLATORS: input tab sub tab name
    N_("Windows"),
    // TRANSLATORS: input tab sub tab name
    N_("Emotes"),
    // TRANSLATORS: input tab sub tab name
    N_("Outfits"),
    // TRANSLATORS: input tab sub tab name
    N_("Chat"),
    // TRANSLATORS: input tab sub tab name
    N_("Other"),
    // TRANSLATORS: input tab sub tab name
    N_("Gui"),
    nullptr
};

const int touchActionDataSize = 4;

static SetupActionData *const touchActionData[] =
{
    setupActionData0,
    setupActionData2,
    setupActionData5,
    setupActionData6
};

#endif  // GUI_SETUPACTIONDATA_H
