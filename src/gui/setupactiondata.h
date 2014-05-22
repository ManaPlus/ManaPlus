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

#include "input/inputaction.h"
#include "input/keydata.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <string>

struct SetupActionData final
{
#ifdef ADVGCC
    SetupActionData(const std::string &name0,
                    const int actionId0,
                    const std::string &text0) :
        name(name0),
        actionId(actionId0),
        text(text0)
    { }

    A_DELETE_COPY(SetupActionData)
#endif

    std::string name;
    const int actionId;
    std::string text;
};

static SetupActionData setupActionData0[] =
{
    {
        // TRANSLATORS: input action name
        N_("Target and attack keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Attack"),
        InputAction::ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target & Attack"),
        InputAction::TARGET_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to Target"),
        InputAction::MOVE_TO_TARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Move to Target type"),
        InputAction::CHANGE_MOVE_TO_TARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to Home location"),
        InputAction::MOVE_TO_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Set home location"),
        InputAction::SET_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to navigation point"),
        InputAction::MOVE_TO_POINT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Talk"),
        InputAction::TALK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Stop Attack / Modifier key"),
        InputAction::STOP_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Untarget"),
        InputAction::UNTARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target monster"),
        InputAction::TARGET_MONSTER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target NPC"),
        InputAction::TARGET_NPC,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target Player"),
        InputAction::TARGET_PLAYER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Other Keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pickup"),
        InputAction::PICKUP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Pickup Type"),
        InputAction::CHANGE_PICKUP_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Sit"),
        InputAction::SIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Screenshot"),
        InputAction::SCREENSHOT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Enable/Disable Trading"),
        InputAction::TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Open trade window"),
        InputAction::OPEN_TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Map View Mode"),
        InputAction::PATHFIND,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select OK"),
        InputAction::OK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quit"),
        InputAction::QUIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Stop or sit"),
        InputAction::STOP_SIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Return to safe video mode"),
        InputAction::SAFE_VIDEO,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData1[] =
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

static SetupActionData setupActionData2[] =
{
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
        N_("Help Window"),
        InputAction::WINDOW_HELP,
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
        N_("Bot Checker Window"),
        InputAction::WINDOW_BOT_CHECKER,
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
        "",
        InputAction::NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData3[] =
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
    }
};

static SetupActionData setupActionData4[] =
{
    {
        // TRANSLATORS: input action name
        N_("Outfits keys"),
        InputAction::NO_VALUE,
        ""
    },
    {
        // TRANSLATORS: input action name
        N_("Wear Outfit"),
        InputAction::WEAR_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy Outfit"),
        InputAction::COPY_OUTFIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Copy equipped to Outfit"),
        InputAction::COPY_EQUIPED_OUTFIT,
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

static SetupActionData setupActionData5[] =
{
    {
        // TRANSLATORS: input action name
        N_("Toggle Chat"),
        InputAction::TOGGLE_CHAT,
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
        N_("Close current Chat Tab"),
        InputAction::CLOSE_CHAT_TAB,
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
        // TRANSLATORS: input action name
        N_("Chat modifier key"),
        InputAction::CHAT_MOD,
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

static SetupActionData setupActionData6[] =
{
    {
        // TRANSLATORS: input action name
        N_("Ignore input 1"),
        InputAction::IGNORE_INPUT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ignore input 2"),
        InputAction::IGNORE_INPUT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Up"),
        InputAction::DIRECT_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Down"),
        InputAction::DIRECT_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Left"),
        InputAction::DIRECT_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direct Right"),
        InputAction::DIRECT_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Crazy moves"),
        InputAction::CRAZY_MOVES,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Crazy Move mode"),
        InputAction::CHANGE_CRAZY_MOVES_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick Drop N Items from 0 slot"),
        InputAction::QUICK_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick Drop N Items"),
        InputAction::QUICK_DROPN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch Quick Drop Counter"),
        InputAction::SWITCH_QUICK_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick heal target or self"),
        InputAction::MAGIC_INMA1,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Use #itenplz spell"),
        InputAction::MAGIC_ITENPLZ,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Use magic attack"),
        InputAction::MAGIC_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch magic attack"),
        InputAction::SWITCH_MAGIC_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch pvp attack"),
        InputAction::SWITCH_PVP_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change move type"),
        InputAction::INVERT_DIRECTION,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Attack Weapon Type"),
        InputAction::CHANGE_ATTACK_WEAPON_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Attack Type"),
        InputAction::CHANGE_ATTACK_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Follow mode"),
        InputAction::CHANGE_FOLLOW_MODE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Imitation mode"),
        InputAction::CHANGE_IMITATION_MODE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Disable / Enable Game modifier keys"),
        InputAction::DISABLE_GAME_MODIFIERS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("On / Off audio"),
        InputAction::CHANGE_AUDIO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Enable / Disable away mode"),
        InputAction::AWAY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emulate right click from keyboard"),
        InputAction::RIGHT_CLICK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Toggle camera mode"),
        InputAction::CAMERA,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show onscreen keyboard"),
        InputAction::SHOW_KEYBOARD,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData7[] =
{
    {
        // TRANSLATORS: input action name
        N_("Move Keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Up"),
        InputAction::MOVE_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Down"),
        InputAction::MOVE_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Left"),
        InputAction::MOVE_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Right"),
        InputAction::MOVE_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move Forward"),
        InputAction::MOVE_FORWARD,
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

static SetupActionData setupActionData8[] =
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
