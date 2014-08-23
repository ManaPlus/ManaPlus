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

#include "input/inputaction.h"
#include "input/inputfunction.h"

#include "events/inputevent.h"

#include <string>
#include <map>

#include <SDL_events.h>

class Setup_Input;

struct InputActionData;

class InputManager final
{
    public:
        InputManager();

        A_DELETE_COPY(InputManager)

        void init();

        bool handleEvent(const SDL_Event &event);

        bool checkKey(const InputActionData *const key) const A_WARN_UNUSED;

        void retrieve();

        void store() const;

        void resetKeys();

        void makeDefault(const int i);

        bool hasConflicts(int &restrict key1,
                          int &restrict key2) const A_WARN_UNUSED;

        void callbackNewKey();

        InputFunction &getKey(int index) A_WARN_UNUSED;

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

        bool invokeKey(const InputActionData *const key, const int keyNum);

        bool handleAssignKey(const SDL_Event &event, const int type);

        static void handleRepeat();

        bool triggerAction(const KeysVector *const ptrs);

        int getKeyIndex(const int value, const int grp,
                        const int type) const A_WARN_UNUSED;

        static void update();

        void updateConditionMask();

        int getActionByKey(const SDL_Event &event) const A_WARN_UNUSED;

        void executeAction(const int keyNum);

        bool executeChatCommand(const std::string &cmd,
                                const std::string &args,
                                ChatTab *const tab);

    protected:
        void resetKey(const int i);

        static bool isActionActive0(const int index) A_WARN_UNUSED;

        Setup_Input *mSetupInput;      /**< Reference to setup window */

        int mNewKeyIndex;              /**< Index of new key to be assigned */

        int mMask;

        std::map<std::string, int> mNameMap;
        std::map<std::string, int> mChatMap;

        InputFunction mKey[InputAction::TOTAL];
};

extern InputManager inputManager;

#endif  // INPUT_INPUTMANAGER_H
