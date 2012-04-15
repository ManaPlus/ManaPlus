/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "inputevent.h"
#include "keydata.h"

#include "gui/sdlinput.h"

#include <string>
#include <map>

#define KeyFunctionSize 3

struct KeyData;

class Setup_Input;

enum KeyTypes
{
    INPUT_UNKNOWN = 0,
    INPUT_KEYBOARD = 1,
    INPUT_MOUSE = 2,
    INPUT_JOYSTICK = 3
};

struct KeyItem
{
    KeyItem() : type(-1), value(-1)
    { }

    KeyItem(int type0, int value0) : type(type0), value(value0)
    { }

    int type;

    int value;
};

struct KeyFunction
{
    KeyItem values[KeyFunctionSize];
};

enum KeyCondition
{
    COND_DEFAULT = 1,          // default condition
    COND_ENABLED = 2,          // keyboard must be enabled
    COND_NOINPUT = 4,          // input items must be unfocused
    COND_NOAWAY = 8,           // player not in away mode
    COND_NOSETUP = 16,         // setup window is hidde
    COND_VALIDSPEED = 32,      // valid speed
    COND_NOMODAL = 64,         // modal windows inactive
    COND_NONPCINPUT = 128,     // npc input field inactive
    COND_EMODS = 256,          // game modifiers enabled
    COND_NOTARGET = 512,       // no target/untarget keys pressed
    COND_NOFOLLOW = 1024,      // follow mode disabled
    COND_SHORTCUT = 2 + 4 + 16 + 512, // flags for shortcut keys
    COND_GAME = 2 + 4 + 8 + 16 + 64, // main game key
    COND_GAME2 = 2 + 8 + 16 + 64
};

class InputManager
{
    public:
        InputManager();

        void init();

        bool handleEvent(const SDL_Event &event);

        int getInputConditionMask();

        bool checkKey(const KeyData *key, int mask);

        void retrieve();

        void store();

        void resetKeys();

        void makeDefault(int i);

        bool hasConflicts(int &key1, int &key2);

        void callbackNewKey();

        KeyFunction &getKey(int index);

        std::string getKeyValueString(int index) const;

        std::string getKeyStringLong(int index) const;

        void addActionKey(int action, int type, int val);

        void setNewKey(const SDL_Event &event, int type);

        void unassignKey();

        bool isActionActive(int index) const;

        /**
         * Set the index of the new key to be assigned.
         */
        void setNewKeyIndex(int value)
        { mNewKeyIndex = value; }

        /**
         * Set a reference to the key setup window.
         */
        void setSetupInput(Setup_Input *setupInput)
        { mSetupInput = setupInput; }

        /**
         * Get the index of the new key to be assigned.
         */
        int getNewKeyIndex() const
        { return mNewKeyIndex; }

        void updateKeyActionMap(KeyToActionMap &actionMap, int type);

        bool invokeKey(const KeyData *key, int keyNum, int mask);

        bool handleAssignKey(const SDL_Event &event, int type);

        bool triggerAction(const KeysVector *ptrs);

        int getKeyIndex(int value, int grp, int type) const;

        void update();

    protected:
        Setup_Input *mSetupInput;      /**< Reference to setup window */

        int mNewKeyIndex;              /**< Index of new key to be assigned */

        KeyFunction mKey[Input::KEY_TOTAL]; /**< Pointer to all the key data */
};

extern InputManager inputManager;

#endif
