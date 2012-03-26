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

#ifndef KEYBOARDCONFIG_H
#define KEYBOARDCONFIG_H

#include <SDL_types.h>
#include <SDL_keyboard.h>

#include "inputevent.h"

#include <map>
#include <string>
#include <vector>

union SDL_Event;

typedef std::vector<int> KeysVector;
typedef KeysVector::iterator KeysVectorIter;
typedef KeysVector::const_iterator KeysVectorCIter;

typedef std::map<int, KeysVector> KeyToActionMap;
typedef KeyToActionMap::iterator KeyToActionMapIter;

/**
 * Each key represents a key function. Such as 'Move up', 'Attack' etc.
 */
struct KeyFunction
{
    const char* configField;    /** Field index that is in the config file. */
    int defaultValue;           /** The default key value used. */
    std::string caption;        /** The caption value for the key function. */
    int value;                  /** The actual value that is used. */
    int grp;                    /** The key group */
    ActionFuncPtr action;       /** The key action function */
    int modKeyIndex;            /** Modifier what enable this key action */
    int priority;               /** Action priority */
    int condition;              /** Condition for allow use key */
};

class Setup_Keyboard;

class KeyboardConfig
{
    public:
        /**
         * Initializes the keyboard config explicitly.
         */
        void init();

        /**
         * Retrieve the key values from config file.
         */
        void retrieve();

        /**
         * Store the key values to config file.
         */
        void store();

        /**
         * Make the keys their default values.
         */
        void makeDefault();

        /**
         * Determines if any key assignments are the same as each other.
         */
        bool hasConflicts();

        /**
         * Calls a function back so the key re-assignment(s) can be seen.
         */
        void callbackNewKey();

        /**
         * Obtain the value stored in memory.
         */
        int getKeyValue(int index) const
        { return mKey[index].value; }

        bool isSeparator(int index) const
        { return !*mKey[index].configField; }

        /**
         * Get the index of the new key to be assigned.
         */
        int getNewKeyIndex() const
        { return mNewKeyIndex; }

        /**
         * Get the enable flag, which will stop the user from doing actions.
         */
        bool isEnabled() const
        { return mEnabled; }

        /**
         * Get the key caption, providing more meaning to the user.
         */
        const std::string &getKeyCaption(int index) const
        { return mKey[index].caption; }

        /**
         * Get the key function index by providing the keys value.
         */
        int getKeyIndex(const SDL_Event &event, int grp = 1) const;

        /**
         * Set the enable flag, which will stop the user from doing actions.
         */
        void setEnabled(bool flag)
        { mEnabled = flag; }

        /**
         * Set the index of the new key to be assigned.
         */
        void setNewKeyIndex(int value)
        { mNewKeyIndex = value; }

        /**
         * Set the value of the new key.
         */
        void setNewKey(const SDL_Event &event);

        /**
         * Set a reference to the key setup window.
         */
        void setSetupKeyboard(Setup_Keyboard *setupKey)
        { mSetupKey = setupKey; }

        /**
         * Checks if the key is active, by providing the key function index.
         */
        bool isActionActive(int index) const;

        /**
         * Takes a snapshot of all the active keys.
         */
        void refreshActiveKeys();

        std::string getKeyValueString(int index) const;

        std::string getKeyShortString(const std::string &key) const;

        std::string getKeyStringLong(int index) const;

        const std::string &getBindError() const
        { return mBindError; }

        SDLKey getKeyFromEvent(const SDL_Event &event) const;

        int getKeyValueFromEvent(const SDL_Event &event) const;

        void unassignKey();

        void updateKeyActionMap();

        bool triggerAction(const SDL_Event &event);

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
            KEY_SEPARATOR1,
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
            KEY_TARGET,
            KEY_UNTARGET,
            KEY_TARGET_CLOSEST,
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
            KEY_SEPARATOR2,
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
            KEY_SEPARATOR3,
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
            KEY_SEPARATOR4,
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
            KEY_SEPARATOR5,
            KEY_WEAR_OUTFIT,
            KEY_COPY_OUTFIT,
            KEY_COPY_EQUIPED_OUTFIT,
            KEY_SEPARATOR6,
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
            KEY_SEPARATOR7,
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

        enum KeyGroup
        {
            GRP_DEFAULT = 1,           // default game key
            GRP_CHAT = 2,              // chat key
            GRP_EMOTION = 4,           // emotions key
            GRP_OUTFIT = 8,            // outfit key
            GRP_GUI = 16               // gui key
        };

    private:
        int mNewKeyIndex;              /**< Index of new key to be assigned */
        bool mEnabled;                 /**< Flag to respond to key input */

        Setup_Keyboard *mSetupKey;     /**< Reference to setup window */

        KeyFunction mKey[KEY_TOTAL];   /**< Pointer to all the key data */

        Uint8 *mActiveKeys;            /**< Stores a list of all the keys */

        std::string mBindError;

        KeyToActionMap mKeyToAction;
};

extern KeyboardConfig keyboard;

#endif
