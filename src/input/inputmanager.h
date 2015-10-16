/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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

#include "input/inputfunction.h"

#include "events/inputevent.h"

#include "utils/stringmap.h"

#include <list>

#include <SDL_events.h>

class Setup_Input;

struct InputActionData;

typedef std::map<std::string, InputActionT> StringInpActionMap;
typedef StringInpActionMap::const_iterator StringInpActionMapCIter;

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

        void makeDefault(const InputActionT i);

        bool hasConflicts(InputActionT &restrict key1,
                          InputActionT &restrict key2) const A_WARN_UNUSED;

        void callbackNewKey();

        InputFunction &getKey(InputActionT index) A_WARN_UNUSED;

        std::string getKeyValueString(const InputActionT index)
                                      const A_WARN_UNUSED;

        std::string getKeyStringLong(const InputActionT index)
                                     const A_WARN_UNUSED;

        std::string getKeyValueByName(const std::string &keyName);

        std::string getKeyValueByNameLong(const std::string &keyName);

        void addActionKey(const InputActionT action,
                          const int type,
                          const int val);

        void setNewKey(const SDL_Event &event, const int type);

        void unassignKey();

        bool isActionActive(const InputActionT index) const A_WARN_UNUSED;

        /**
         * Set the index of the new key to be assigned.
         */
        void setNewKeyIndex(const InputActionT value)
        { mNewKeyIndex = value; }

        /**
         * Set a reference to the key setup window.
         */
        void setSetupInput(Setup_Input *const setupInput) A_NONNULL(2)
        { mSetupInput = setupInput; }

        /**
         * Get the index of the new key to be assigned.
         */
        InputActionT getNewKeyIndex() const A_WARN_UNUSED
        { return mNewKeyIndex; }

        void updateKeyActionMap(KeyToActionMap &actionMap,
                                KeyToIdMap &idMap, KeyTimeMap &keyTimeMap,
                                const int type) const;

        bool invokeKey(const InputActionData *const key,
                       const InputActionT keyNum);

        bool handleAssignKey(const SDL_Event &event, const int type);

        static void handleRepeat();

        bool triggerAction(const KeysVector *const ptrs);

        InputActionT getKeyIndex(const int value,
                                 const int grp,
                                 const int type) const A_WARN_UNUSED;

        static void update();

        void updateConditionMask();

        InputActionT getActionByKey(const SDL_Event &event)
                                    const A_WARN_UNUSED;

        void executeAction(const InputActionT keyNum);

        bool executeChatCommand(const std::string &cmd,
                                const std::string &args,
                                ChatTab *const tab);

        bool executeRemoteChatCommand(const std::string &cmd,
                                      const std::string &args,
                                      ChatTab *const tab);

        bool executeChatCommand(const InputActionT keyNum,
                                const std::string &args,
                                ChatTab *const tab);

        void addChatCommands(std::list<std::string> &arr);

    protected:
        void resetKey(const InputActionT i);

        static bool isActionActive0(const InputActionT index) A_WARN_UNUSED;

        /** Reference to setup window */
        Setup_Input *mSetupInput A_NONNULLPOINTER;
        /** Index of new key to be assigned */
        InputActionT mNewKeyIndex;

        int mMask;

        StringInpActionMap mNameMap;
        StringIntMap mChatMap;

        InputFunction mKey[static_cast<size_t>(InputAction::TOTAL)];
};

extern InputManager inputManager;

#endif  // INPUT_INPUTMANAGER_H
