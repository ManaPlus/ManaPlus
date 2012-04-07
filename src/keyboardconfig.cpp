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
#include "gui/setup_keyboard.h"

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
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        for (int f = 0; f < KeyFunctionSize; f ++)
        {
            mKey[i].values[f].type = INPUT_UNKNOWN;
            mKey[i].values[f].value = -1;
        }
    }

    mNewKeyIndex = KEY_NO_VALUE;
    mEnabled = true;

    makeDefault();
    retrieve();
    updateKeyActionMap();
}

void KeyboardConfig::retrieve()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (*keyData[i].configField)
        {
            KeyFunction &kf = mKey[i];
            std::string keyStr = config.getValue(keyData[i].configField,
                toString(keyData[i].defaultValue));
            StringVect keys;
            splitToStringVector(keys, keyStr, ',');
            StringVectCIter it = keys.begin();
            StringVectCIter it_end = keys.end();
            int i2 = 0;
            for (; it != it_end && i2 < KeyFunctionSize; ++ it)
            {
                std::string keyStr2 = *it;
                if (keyStr.size() < 2)
                    continue;
                int type = INPUT_KEYBOARD;
                if ((keyStr2[0] < '0' || keyStr2[0] > '9')
                    && keyStr2[0] != '-')
                {
                    switch (keyStr2[0])
                    {
                        case 'm':
                            type = INPUT_MOUSE;
                            break;
                        case 'j':
                            type = INPUT_JOYSTICK;
                            break;
                        default:
                            break;
                    }
                    keyStr2 = keyStr2.substr(1);
                }
                int key = atoi(keyStr2.c_str());
                if (key >= -255 && key < SDLK_LAST)
                {
                    kf.values[i2] = KeyItem(type, key);
                    i2 ++;
                }
            }
        }
    }
}

void KeyboardConfig::store()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (*keyData[i].configField)
        {
            std::string keyStr;

            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                const KeyItem &key = mKey[i].values[i2];
                if (key.type != INPUT_UNKNOWN)
                {
                    std::string tmp = "k";
                    switch(key.type)
                    {
                        case INPUT_MOUSE:
                            tmp = "m";
                            break;
                        case INPUT_JOYSTICK:
                            tmp = "j";
                            break;
                        default:
                            break;
                    }
                    if (key.value >= 0)
                    {
                        if (keyStr.empty())
                        {
                            keyStr = tmp + toString(key.value);
                        }
                        else
                        {
                            keyStr += strprintf(",%s%d", tmp.c_str(), key.value);
                        }
                    }
                }
            }
            if (keyStr.empty())
                keyStr = "-1";

            config.setValue(keyData[i].configField, keyStr);
        }
    }
}

void KeyboardConfig::makeDefault()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        for (size_t i2 = 1; i2 < KeyFunctionSize; i2 ++)
        {
            mKey[i].values[i2].type = INPUT_UNKNOWN;
            mKey[i].values[i2].value = -1;
        }
        mKey[i].values[0].type = INPUT_KEYBOARD;
        mKey[i].values[0].value = keyData[i].defaultValue;
    }
}

bool KeyboardConfig::hasConflicts(int &key1, int &key2)
{
    size_t i;
    size_t j;
    /**
     * No need to parse the square matrix: only check one triangle
     * that's enough to detect conflicts
     */
    for (i = 0; i < KEY_TOTAL; i++)
    {
        if (!*keyData[i].configField)
            continue;

        for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
        {
            if (mKey[i].values[i2].value == KEY_NO_VALUE)
                continue;

            for (j = i, j++; j < KEY_TOTAL; j++)
            {

                if ((keyData[i].grp & keyData[j].grp) == 0 ||
                    !*keyData[i].configField)
                {
                    continue;
                }

                for (size_t j2 = 0; j2 < KeyFunctionSize; j2 ++)
                {
                    // Allow for item shortcut and emote keys to overlap
                    // as well as emote and ignore keys, but no other keys
                    if (mKey[j].values[j2].type != INPUT_UNKNOWN
                        && mKey[i].values[i2].value == mKey[j].values[j2].value
                        && mKey[i].values[i2].type == mKey[j].values[j2].type)
                    {
                        key1 = i;
                        key2 = j;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void KeyboardConfig::callbackNewKey()
{
    mSetupKey->newKeyCallback(mNewKeyIndex);
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
    for (size_t i = 0; i < KEY_TOTAL; i++)
    {
        for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
        {
            if (keyValue == mKey[i].values[i2].value
                && (grp & keyData[i].grp) != 0
                && mKey[i].values[i2].type == INPUT_KEYBOARD)
            {
                return i;
            }
        }
    }
    return KEY_NO_VALUE;
}

bool KeyboardConfig::isActionActive(int index) const
{
    if (!mActiveKeys)
        return false;

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const int value = mKey[index].values[i].value;
        if (value >= 0 && mActiveKeys[value])
            return true;
    }
    return false;
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyStringLong(int index) const
{
    std::string keyStr;

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const KeyItem &key = mKey[index].values[i];
        if (key.type == INPUT_KEYBOARD)
        {
            if (key.value >= 0)
            {
                if (keyStr.empty())
                {
                    keyStr = SDL_GetKeyName(static_cast<SDLKey>(key.value));
                }
                else
                {
                    keyStr += std::string(" ") + SDL_GetKeyName(
                        static_cast<SDLKey>(key.value));
                }
            }
        }
    }

    if (keyStr.empty())
        return _("unknown key");
    return keyStr;
}

std::string KeyboardConfig::getKeyValueString(int index) const
{
    std::string keyStr;

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const KeyItem &key = mKey[index].values[i];
        if (key.type == INPUT_KEYBOARD)
        {
            if (key.value >= 0)
            {
                if (keyStr.empty())
                {
                    keyStr = getKeyShortString(SDL_GetKeyName(
                        static_cast<SDLKey>(key.value)));
                }
                else
                {
                    keyStr += " " + getKeyShortString(SDL_GetKeyName(
                        static_cast<SDLKey>(key.value)));
                }
            }
        }
    }

    if (keyStr.empty())
        return _("u key");
    return keyStr;
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

void KeyboardConfig::setNewKey(const SDL_Event &event)
{
    int idx = -1;
    KeyFunction &key = mKey[mNewKeyIndex];
    int val = getKeyValueFromEvent(event);

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        if (key.values[i].type == INPUT_UNKNOWN
            || (key.values[i].type == INPUT_KEYBOARD
            && key.values[i].value == val))
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        for (size_t i = 1; i < KeyFunctionSize; i ++)
        {
            key.values[i - 1].type = key.values[i].type;
            key.values[i - 1].value = key.values[i].value;
        }
        idx = KeyFunctionSize - 1;
    }

    key.values[idx] = KeyItem(INPUT_KEYBOARD, val);
    updateKeyActionMap();
}

void KeyboardConfig::unassignKey()
{
    KeyFunction &key = mKey[mNewKeyIndex];
    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        key.values[i].type = INPUT_UNKNOWN;
        key.values[i].value = -1;
    }
    updateKeyActionMap();
}

void KeyboardConfig::updateKeyActionMap()
{
    mKeyToAction.clear();

    for (size_t i = 0; i < KEY_TOTAL; i++)
    {
        if (keyData[i].action)
        {
            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                if (mKey[i].values[i2].type == INPUT_KEYBOARD)
                    mKeyToAction[mKey[i].values[i2].value].push_back(i);
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
            if (keyNum < 0 || keyNum >= KEY_TOTAL)
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

int KeyboardConfig::getKeyValue(int index) const
{
    return mKey[index].values[0].value;
}
