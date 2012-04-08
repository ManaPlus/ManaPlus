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

#include "keyboardconfig.h"

#include "configuration.h"
#include "inputevent.h"
#include "inputmanager.h"
#include "keyboarddata.h"
#include "logger.h"

#include "gui/sdlinput.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <SDL_events.h>

#include <algorithm>

#include "debug.h"

class KeyFunctor
{
    public:
        bool operator() (int key1, int key2)
        {
            return keys[key1].priority >= keys[key2].priority;
        }

        const KeyData *keys;

} keySorter;

void KeyboardConfig::init()
{
    mEnabled = true;

    updateKeyActionMap();
}

int KeyboardConfig::getKeyValueFromEvent(const SDL_Event &event) const
{
    if (event.key.keysym.sym)
        return event.key.keysym.sym;
    else if (event.key.keysym.scancode > 1)
        return -event.key.keysym.scancode;
    return 0;
}

int KeyboardConfig::getKeyIndex(const SDL_Event &event, int grp) const
{
    const int keyValue = getKeyValueFromEvent(event);
    for (size_t i = 0; i < Input::KEY_TOTAL; i++)
    {
        KeyFunction &key = inputManager.getKey(i);
        for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
        {
            if (keyValue == key.values[i2].value
                && (grp & keyData[i].grp) != 0
                && key.values[i2].type == INPUT_KEYBOARD)
            {
                return i;
            }
        }
    }
    return Input::KEY_NO_VALUE;
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyName(int key)
{
    if (key == Input::KEY_NO_VALUE)
        return "";
    if (key >= 0)
        return SDL_GetKeyName(static_cast<SDLKey>(key));

    return strprintf(_("key_%d"), key);
}

std::string KeyboardConfig::getKeyShortString(const std::string &key) const
{
    if (key == "backspace")
    {
        return "bksp";
    }
    else if (key == "unknown key")
    {
        // TRANSLATORS: Unknown key short string. This string must be maximum 5 chars
        return _("u key");
    }
    return key;
}

SDLKey KeyboardConfig::getKeyFromEvent(const SDL_Event &event) const
{
    return event.key.keysym.sym;
}

void KeyboardConfig::updateKeyActionMap()
{
    mKeyToAction.clear();

    for (size_t i = 0; i < Input::KEY_TOTAL; i ++)
    {
        if (keyData[i].action)
        {
            KeyFunction &key = inputManager.getKey(i);
            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                if (key.values[i2].type == INPUT_KEYBOARD)
                    mKeyToAction[key.values[i2].value].push_back(i);
            }
        }
    }

    keySorter.keys = &keyData[0];
    KeyToActionMapIter it = mKeyToAction.begin();
    KeyToActionMapIter it_end = mKeyToAction.end();
    for (; it != it_end; ++ it)
    {
        KeysVector *keys = &it->second;
        if (keys->size() > 1)
            sort(keys->begin(), keys->end(), keySorter);
    }
}

bool KeyboardConfig::triggerAction(const SDL_Event &event)
{
    const int i = getKeyValueFromEvent(event);
//    logger->log("key triggerAction: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToAction.find(i) != mKeyToAction.end())
    {
        const KeysVector &ptrs = mKeyToAction[i];
//        logger->log("ptrs: %d", (int)ptrs.size());
        KeysVectorCIter it = ptrs.begin();
        KeysVectorCIter it_end = ptrs.end();

        int mask = inputManager.getInputConditionMask();

        for (; it != it_end; ++ it)
        {
            const int keyNum = *it;
            if (keyNum < 0 || keyNum >= Input::KEY_TOTAL)
                continue;

            if (inputManager.checkKey(&keyData[keyNum], mask))
            {
                InputEvent evt(keyNum, mask);
                if ((*(keyData[keyNum].action))(evt))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool KeyboardConfig::isActionActive(int index) const
{
    if (!mActiveKeys)
        return false;

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const int value = inputManager.getKey(index).values[i].value;
        if (value >= 0 && mActiveKeys[value])
            return true;
    }
    return false;
}
