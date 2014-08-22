/*
 *  The ManaPlus Client
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

#include "gui/setupactiondata.h"

#include "input/inputaction.h"
#include "input/inputactiondata.h"

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

SetupActionData setupActionDataChat[] =
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

SetupActionData setupActionDataOther[] =
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
