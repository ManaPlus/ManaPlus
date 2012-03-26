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

        KeyFunction *keys;

} keySorter;

void KeyboardConfig::init()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        mKey[i].configField = keyData[i].configField;
        mKey[i].defaultValue = keyData[i].defaultValue;
        mKey[i].caption = gettext(keyData[i].caption.c_str());
        mKey[i].value = KEY_NO_VALUE;
        mKey[i].grp = keyData[i].grp;
        mKey[i].action = keyData[i].action;
        mKey[i].modKeyIndex = keyData[i].modKeyIndex;
        mKey[i].priority = keyData[i].priority;
        mKey[i].condition = keyData[i].condition;
    }
    for (int i = KEY_EMOTE_1; i <= KEY_EMOTE_48; i ++)
    {
        mKey[i].caption = strprintf(
            _("Emote Shortcut %d"), i - KEY_EMOTE_1 + 1);
    }
    for (int i = KEY_SHORTCUT_1; i <= KEY_SHORTCUT_20; i ++)
    {
        mKey[i].caption = strprintf(
            _("Item Shortcut %d"), i - KEY_SHORTCUT_1 + 1);
    }

    mNewKeyIndex = KEY_NO_VALUE;
    mEnabled = true;

    retrieve();
    updateKeyActionMap();
}

void KeyboardConfig::retrieve()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (*mKey[i].configField)
        {
            mKey[i].value = static_cast<int>(config.getValue(
                mKey[i].configField, mKey[i].defaultValue));
            if (mKey[i].value < -255 || mKey[i].value >= SDLK_LAST)
                mKey[i].value = KEY_NO_VALUE;
        }
    }
}

void KeyboardConfig::store()
{
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (*mKey[i].configField)
            config.setValue(mKey[i].configField, mKey[i].value);
    }
}

void KeyboardConfig::makeDefault()
{
    for (int i = 0; i < KEY_TOTAL; i++)
        mKey[i].value = mKey[i].defaultValue;
}

bool KeyboardConfig::hasConflicts()
{
    int i, j;
    /**
     * No need to parse the square matrix: only check one triangle
     * that's enough to detect conflicts
     */
    for (i = 0; i < KEY_TOTAL; i++)
    {
        if (mKey[i].value == KEY_NO_VALUE || !*mKey[i].configField)
            continue;

        for (j = i, j++; j < KEY_TOTAL; j++)
        {
            // Allow for item shortcut and emote keys to overlap
            // as well as emote and ignore keys, but no other keys
            if (mKey[j].value != KEY_NO_VALUE &&
                mKey[i].value == mKey[j].value &&
                ((mKey[i].grp & mKey[j].grp) != 0 &&
                *mKey[i].configField)
               )
            {
                mBindError = strprintf(_("Conflict \"%s\" and \"%s\" keys. "
                                       "Resolve them, or gameplay may result"
                                       " in strange behaviour."),
                                       mKey[i].caption.c_str(),
                                       mKey[j].caption.c_str());
                return true;
            }
        }
    }
    mBindError = "";
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
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (keyValue == mKey[i].value &&
            (grp & mKey[i].grp) != 0)
        {
            return i;
        }
    }
    return KEY_NO_VALUE;
}

bool KeyboardConfig::isActionActive(int index) const
{
    if (!mActiveKeys)
        return false;
    const int value = mKey[index].value;
    if (value >= 0)
        return mActiveKeys[value];
    else
        return false;   // scan codes active state now not implimented
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyStringLong(int index) const
{
    const int keyValue = getKeyValue(index);
    if (keyValue >= 0)
        return SDL_GetKeyName(static_cast<SDLKey>(keyValue));
    else if (keyValue < -1)
        return strprintf(_("key_%d"), -keyValue);
    else
        return _("unknown key");
}

std::string KeyboardConfig::getKeyValueString(int index) const
{
    const int keyValue = getKeyValue(index);
    if (keyValue >= 0)
    {
        std::string key = SDL_GetKeyName(static_cast<SDLKey>(keyValue));
        return getKeyShortString(key);
    }
    else if (keyValue < -1)
    {
        return strprintf("#%d", -keyValue);
    }
    else
    {
        // TRANSLATORS: Unknown key short string. This string must be maximum 5 chars
        return _("u key");
    }
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
    mKey[mNewKeyIndex].value = getKeyValueFromEvent(event);
    updateKeyActionMap();
}

void KeyboardConfig::unassignKey()
{
    mKey[mNewKeyIndex].value = KEY_NO_VALUE;
    updateKeyActionMap();
}

void KeyboardConfig::updateKeyActionMap()
{
    mKeyToAction.clear();

    for (int i = 0; i < KEY_TOTAL; i++)
    {
        if (mKey[i].value != KEY_NO_VALUE && mKey[i].action)
            mKeyToAction[mKey[i].value].push_back(i);
    }

    keySorter.keys = &mKey[0];
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

            if (inputManager.checkKey(&mKey[keyNum], mask))
            {
                InputEvent evt(keyNum, mask);
                if ((*(mKey[keyNum].action))(evt))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
