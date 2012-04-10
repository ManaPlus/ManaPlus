/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef KEYDATA_H
#define KEYDATA_H

#include "actionmanager.h"

#include <string>

typedef bool (*ActionFuncPtr) (InputEvent &event);

struct KeyData
{
    const char *configField;
    int defaultType1;
    int defaultValue1;
    int defaultType2;
    int defaultValue2;
    int grp;
    ActionFuncPtr action;
    int modKeyIndex;
    int priority;
    int condition;
};

namespace Input
{
    enum KeyGroup
    {
        GRP_DEFAULT = 1,           // default game key
        GRP_CHAT = 2,              // chat key
        GRP_EMOTION = 4,           // emotions key
        GRP_OUTFIT = 8,            // outfit key
        GRP_GUI = 16               // gui key
    };
  
    /**
      * All the key functions.
      * KEY_NO_VALUE is used in initialization, and should be unchanged.
      * KEY_TOTAL should always be last (used as a conditional in loops).
      * The key assignment view gets arranged according to the order of
      * these values.
      */
    enum KeyAction
    {
        KEY_NO_VALUE = -1,
        KEY_MOVE_UP,
        KEY_MOVE_DOWN,
        KEY_MOVE_LEFT,
        KEY_MOVE_RIGHT,
        KEY_ATTACK,
        KEY_TARGET_ATTACK,
        KEY_MOVE_TO_TARGET,
        KEY_CHANGE_MOVE_TO_TARGET,
        KEY_MOVE_TO_HOME,
        KEY_SET_HOME,
        KEY_MOVE_TO_POINT,
        KEY_TALK,
        KEY_STOP_ATTACK,
        KEY_UNTARGET,
        KEY_TARGET_MONSTER,
        KEY_TARGET_NPC,
        KEY_TARGET_PLAYER,
        KEY_PICKUP,
        KEY_CHANGE_PICKUP_TYPE,
        KEY_HIDE_WINDOWS,
        KEY_SIT,
        KEY_SCREENSHOT,
        KEY_TRADE,
        KEY_PATHFIND,
        KEY_OK,
        KEY_QUIT,
        KEY_SHORTCUTS_KEY,
        KEY_SHORTCUT_1,
        KEY_SHORTCUT_2,
        KEY_SHORTCUT_3,
        KEY_SHORTCUT_4,
        KEY_SHORTCUT_5,
        KEY_SHORTCUT_6,
        KEY_SHORTCUT_7,
        KEY_SHORTCUT_8,
        KEY_SHORTCUT_9,
        KEY_SHORTCUT_10,
        KEY_SHORTCUT_11,
        KEY_SHORTCUT_12,
        KEY_SHORTCUT_13,
        KEY_SHORTCUT_14,
        KEY_SHORTCUT_15,
        KEY_SHORTCUT_16,
        KEY_SHORTCUT_17,
        KEY_SHORTCUT_18,
        KEY_SHORTCUT_19,
        KEY_SHORTCUT_20,
        KEY_WINDOW_HELP,
        KEY_WINDOW_STATUS,
        KEY_WINDOW_INVENTORY,
        KEY_WINDOW_EQUIPMENT,
        KEY_WINDOW_SKILL,
        KEY_WINDOW_MINIMAP,
        KEY_WINDOW_CHAT,
        KEY_WINDOW_SHORTCUT,
        KEY_WINDOW_SETUP,
        KEY_WINDOW_DEBUG,
        KEY_WINDOW_SOCIAL,
        KEY_WINDOW_EMOTE_SHORTCUT,
        KEY_WINDOW_OUTFIT,
        KEY_WINDOW_SHOP,
        KEY_WINDOW_DROP,
        KEY_WINDOW_KILLS,
        KEY_WINDOW_SPELLS,
        KEY_WINDOW_BOT_CHECKER,
        KEY_WINDOW_ONLINE,
        KEY_WINDOW_DIDYOUKNOW,
        KEY_PREV_SOCIAL_TAB,
        KEY_NEXT_SOCIAL_TAB,
        KEY_EMOTE,
        KEY_EMOTE_1,
        KEY_EMOTE_2,
        KEY_EMOTE_3,
        KEY_EMOTE_4,
        KEY_EMOTE_5,
        KEY_EMOTE_6,
        KEY_EMOTE_7,
        KEY_EMOTE_8,
        KEY_EMOTE_9,
        KEY_EMOTE_10,
        KEY_EMOTE_11,
        KEY_EMOTE_12,
        KEY_EMOTE_13,
        KEY_EMOTE_14,
        KEY_EMOTE_15,
        KEY_EMOTE_16,
        KEY_EMOTE_17,
        KEY_EMOTE_18,
        KEY_EMOTE_19,
        KEY_EMOTE_20,
        KEY_EMOTE_21,
        KEY_EMOTE_22,
        KEY_EMOTE_23,
        KEY_EMOTE_24,
        KEY_EMOTE_25,
        KEY_EMOTE_26,
        KEY_EMOTE_27,
        KEY_EMOTE_28,
        KEY_EMOTE_29,
        KEY_EMOTE_30,
        KEY_EMOTE_31,
        KEY_EMOTE_32,
        KEY_EMOTE_33,
        KEY_EMOTE_34,
        KEY_EMOTE_35,
        KEY_EMOTE_36,
        KEY_EMOTE_37,
        KEY_EMOTE_38,
        KEY_EMOTE_39,
        KEY_EMOTE_40,
        KEY_EMOTE_41,
        KEY_EMOTE_42,
        KEY_EMOTE_43,
        KEY_EMOTE_44,
        KEY_EMOTE_45,
        KEY_EMOTE_46,
        KEY_EMOTE_47,
        KEY_EMOTE_48,
        KEY_WEAR_OUTFIT,
        KEY_COPY_OUTFIT,
        KEY_COPY_EQUIPED_OUTFIT,
        KEY_TOGGLE_CHAT,
        KEY_SCROLL_CHAT_UP,
        KEY_SCROLL_CHAT_DOWN,
        KEY_PREV_CHAT_TAB,
        KEY_NEXT_CHAT_TAB,
        KEY_CLOSE_CHAT_TAB,
        KEY_CHAT_PREV_HISTORY,
        KEY_CHAT_NEXT_HISTORY,
        KEY_AUTOCOMPLETE_CHAT,
        KEY_DEACTIVATE_CHAT,
        KEY_IGNORE_INPUT_1,
        KEY_IGNORE_INPUT_2,
        KEY_DIRECT_UP,
        KEY_DIRECT_DOWN,
        KEY_DIRECT_LEFT,
        KEY_DIRECT_RIGHT,
        KEY_CRAZY_MOVES,
        KEY_CHANGE_CRAZY_MOVES_TYPE,
        KEY_QUICK_DROP,
        KEY_QUICK_DROPN,
        KEY_SWITCH_QUICK_DROP,
        KEY_MAGIC_INMA1,
        KEY_MAGIC_ITENPLZ,
        KEY_MAGIC_ATTACK,
        KEY_SWITCH_MAGIC_ATTACK,
        KEY_SWITCH_PVP_ATTACK,
        KEY_INVERT_DIRECTION,
        KEY_CHANGE_ATTACK_WEAPON_TYPE,
        KEY_CHANGE_ATTACK_TYPE,
        KEY_CHANGE_FOLLOW_MODE,
        KEY_CHANGE_IMITATION_MODE,
        KEY_DISABLE_GAME_MODIFIERS,
        KEY_CHANGE_AUDIO,
        KEY_AWAY,
        KEY_RIGHT_CLICK,
        KEY_CAMERA,
        KEY_MOD,
        KEY_TOTAL
    };
}

#endif
