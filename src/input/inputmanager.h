/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef INPUT_INPUTMANAGER_H
#define INPUT_INPUTMANAGER_H

#include "input/inputevent.h"
#include "input/keydata.h"

#include <string>
#include <map>

#include <SDL_events.h>

const unsigned int KeyFunctionSize = 3;

// hack to avoid conflicts with windows headers.
#ifdef INPUT_KEYBOARD
#undef INPUT_KEYBOARD
#endif
#ifdef INPUT_MOUSE
#undef INPUT_MOUSE
#endif

class Setup_Input;

enum KeyTypes
{
    INPUT_UNKNOWN = 0,
    INPUT_KEYBOARD = 1,
    INPUT_MOUSE = 2,
    INPUT_JOYSTICK = 3
};

struct KeyItem final
{
    KeyItem() :
        type(-1), value(-1)
    { }

    KeyItem(const int type0, const int value0) :
        type(type0), value(value0)
    { }

    int type;

    int value;
};

struct KeyFunction final
{
    KeyItem values[KeyFunctionSize];
};

enum KeyCondition
{
    COND_DEFAULT = 1,                         // default condition
    COND_ENABLED = 2,                         // keyboard must be enabled
    COND_NOINPUT = 4,                         // input items must be unfocused
    COND_NOAWAY = 8,                          // player not in away mode
    COND_NOSETUP = 16,                        // setup window is hidde
    COND_VALIDSPEED = 32,                     // valid speed
    COND_NOMODAL = 64,                        // modal windows inactive
    COND_NONPCINPUT = 128,                    // npc input field inactive
    COND_EMODS = 256,                         // game modifiers enabled
    COND_NOTARGET = 512,                      // no target/untarget keys
                                              // pressed
    COND_NOFOLLOW = 1024,                     // follow mode disabled
    COND_INGAME = 2048,                       // game must be started
    COND_NOBUYSELL = 4096,                    // no active buy or sell dialogs
    COND_NONPCDIALOG = 8192,                  // no active npc dialog or
                                              // dialog almost closed
    COND_NOTALKING = 16384,                   // player have no opened
                                              // dialogs what prevent moving
    COND_SHORTCUT = 2 + 4 + 16 + 512 + 2048,  // flags for shortcut keys
    COND_SHORTCUT0 = 2 + 4 + 16 + 512,        // flags for shortcut keys
    COND_GAME = 2 + 4 + 8 + 16 + 64 + 2048,   // main game key
    COND_GAME2 = 2 + 8 + 16 + 64 + 2048,
    COND_ARROWKEYS = 2 + 8 + 16 + 64 + 2048 + 4096 + 16384
};

class InputManager final
{
    public:
        InputManager();

        A_DELETE_COPY(InputManager)

        void init();

        bool handleEvent(const SDL_Event &event);

        bool checkKey(const KeyData *const key) const A_WARN_UNUSED;

        void retrieve();

        void store() const;

        void resetKeys();

        void makeDefault(const int i);

        bool hasConflicts(int &restrict key1,
                          int &restrict key2) const A_WARN_UNUSED;

        void callbackNewKey();

        KeyFunction &getKey(int index) A_WARN_UNUSED;

        std::string getKeyValueString(const int index) const A_WARN_UNUSED;

        std::string getKeyStringLong(const int index) const A_WARN_UNUSED;

        std::string getKeyValueByName(const std::string &keyName);

        void addActionKey(const int action, const int type, const int val);

        void setNewKey(const SDL_Event &event, const int type);

        void unassignKey();

        bool isActionActive(const int index) const A_WARN_UNUSED;

        /**
         * Set the index of the new key to be assigned.
         */
        void setNewKeyIndex(const int value)
        { mNewKeyIndex = value; }

        /**
         * Set a reference to the key setup window.
         */
        void setSetupInput(Setup_Input *const setupInput)
        { mSetupInput = setupInput; }

        /**
         * Get the index of the new key to be assigned.
         */
        int getNewKeyIndex() const A_WARN_UNUSED
        { return mNewKeyIndex; }

        void updateKeyActionMap(KeyToActionMap &actionMap,
                                KeyToIdMap &idMap, KeyTimeMap &keyTimeMap,
                                const int type) const;

        bool invokeKey(const KeyData *const key, const int keyNum);

        bool handleAssignKey(const SDL_Event &event, const int type);

        void handleRepeat() const;

        bool triggerAction(const KeysVector *const ptrs);

        int getKeyIndex(const int value, const int grp,
                        const int type) const A_WARN_UNUSED;

        void update() const;

        void updateConditionMask();

        int getActionByKey(const SDL_Event &event) const A_WARN_UNUSED;

        void executeAction(const int keyNum);

    protected:
        bool isActionActive0(const int index) const A_WARN_UNUSED;

        Setup_Input *mSetupInput;      /**< Reference to setup window */

        int mNewKeyIndex;              /**< Index of new key to be assigned */

        int mMask;

        std::map<std::string, int> mNameMap;

        KeyFunction mKey[Input::KEY_TOTAL]; /**< Pointer to all the key data */
};

extern InputManager inputManager;

#endif  // INPUT_INPUTMANAGER_H
