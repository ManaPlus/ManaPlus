/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "utils/cast.h"
#include "utils/stringmap.h"

#include <list>

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
PRAGMA48(GCC diagnostic pop)

class Setup_Input;

struct InputActionData;

typedef std::map<std::string, InputActionT> StringInpActionMap;
typedef StringInpActionMap::const_iterator StringInpActionMapCIter;

class InputManager final
{
    public:
        InputManager();

        A_DELETE_COPY(InputManager)

        void init() restrict2;

        bool handleEvent(const SDL_Event &restrict event) restrict2;

        bool checkKey(const InputActionData *restrict const key) const
                      restrict2 A_WARN_UNUSED;

        void retrieve() restrict2;

        void store() const restrict2;

        void resetKeys() restrict2;

        void makeDefault(const InputActionT i) restrict2;

        bool hasConflicts(InputActionT &restrict key1,
                          InputActionT &restrict key2) const
                          restrict2 A_WARN_UNUSED;

        void callbackNewKey() restrict2;

        InputFunction &getKey(InputActionT index)
                              restrict2 A_CONST A_WARN_UNUSED;

        std::string getKeyValueString(const InputActionT index)
                                      const restrict2 A_WARN_UNUSED;

        std::string getKeyStringLong(const InputActionT index)
                                     const restrict2 A_WARN_UNUSED;

        std::string getKeyValueByName(const std::string &restrict keyName)
                                      restrict2;

        std::string getKeyValueByNameLong(const std::string &restrict keyName)
                                          restrict2;

        void addActionKey(const InputActionT action,
                          const InputTypeT type,
                          const int val) restrict2;

        void setNewKey(const SDL_Event &event,
                       const InputTypeT type) restrict2;

        void unassignKey() restrict2;

        bool isActionActive(const InputActionT index) const
                            restrict2 A_WARN_UNUSED;

        /**
         * Set the index of the new key to be assigned.
         */
        void setNewKeyIndex(const InputActionT value) restrict2 noexcept2
        { mNewKeyIndex = value; }

        /**
         * Set a reference to the key setup window.
         */
        void setSetupInput(Setup_Input *restrict const setupInput)
                           restrict2 noexcept2 A_NONNULL(2)
        { mSetupInput = setupInput; }

        /**
         * Get the index of the new key to be assigned.
         */
        InputActionT getNewKeyIndex() const restrict2 noexcept2 A_WARN_UNUSED
        { return mNewKeyIndex; }

        void updateKeyActionMap(KeyToActionMap &restrict actionMap,
                                KeyToIdMap &restrict idMap,
                                KeyTimeMap &restrict keyTimeMap,
                                const InputTypeT type) const restrict2;

        bool invokeKey(const InputActionData *restrict const key,
                       const InputActionT keyNum) restrict2;

        bool handleAssignKey(const SDL_Event &restrict event,
                             const InputTypeT type) restrict2;

        static void handleRepeat();

        bool triggerAction(const KeysVector *restrict const ptrs) restrict2;

        InputActionT getKeyIndex(const int value,
                                 const int grp,
                                 const InputTypeT type) const
                                 restrict2 A_WARN_UNUSED;

        static void update();

        void updateConditionMask(const bool pressed) restrict2;

        InputActionT getActionByKey(const SDL_Event &restrict event)
                                    const restrict2 A_WARN_UNUSED;

        InputActionT getActionByConfigField(const std::string &field)
                                            const restrict2 A_WARN_UNUSED;

        void executeAction(const InputActionT keyNum) restrict2;

        bool executeChatCommand(const std::string &restrict cmd,
                                const std::string &restrict args,
                                ChatTab *restrict const tab) restrict2;

        bool executeRemoteChatCommand(const std::string &restrict cmd,
                                      const std::string &restrict args,
                                      ChatTab *restrict const tab) restrict2;

        bool executeChatCommand(const InputActionT keyNum,
                                const std::string &restrict args,
                                ChatTab *restrict const tab) restrict2;

        void addChatCommands(std::list<std::string> &restrict arr) restrict;

    protected:
        void resetKey(const InputActionT i) restrict2;

        static bool isActionActive0(const InputActionT index) A_WARN_UNUSED;

        void updateKeyString(const InputFunction &ki,
                             const size_t actionIdx) restrict2;

        /** Reference to setup window */
        Setup_Input *mSetupInput A_NONNULLPOINTER;
        /** Index of new key to be assigned */
        InputActionT mNewKeyIndex;

        int mMask;

        StringInpActionMap mNameMap;
        StringIntMap mChatMap;

        InputFunction mKey[CAST_SIZE(InputAction::TOTAL)];
        std::string mKeyStr[CAST_SIZE(InputAction::TOTAL)];
};

extern InputManager inputManager;

#endif  // INPUT_INPUTMANAGER_H
