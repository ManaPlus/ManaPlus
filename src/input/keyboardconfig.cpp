/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "input/keyboardconfig.h"

#include "configuration.h"

#include "input/inputmanager.h"

#include "utils/gettext.h"

#include <SDL_events.h>

#include "debug.h"

extern volatile int tick_time;

KeyboardConfig::KeyboardConfig() :
    mEnabled(true),
    mActiveKeys(nullptr),
    mActiveKeys2(nullptr),
    mRepeatTime(0),
    mKeyToAction(),
    mKeyToId(),
    mKeyTimeMap()
{
}

void KeyboardConfig::init()
{
    mEnabled = true;
    delete [] mActiveKeys2;
    mActiveKeys2 = new uint8_t[500];
    mRepeatTime = config.getIntValue("repeateInterval2") / 10;
}

void KeyboardConfig::deinit()
{
    delete [] mActiveKeys2;
    mActiveKeys2 = nullptr;
}

int KeyboardConfig::getKeyValueFromEvent(const SDL_Event &event) const
{
#ifdef USE_SDL2
    return event.key.keysym.scancode;
#else
    if (event.key.keysym.sym)
        return event.key.keysym.sym;
    else if (event.key.keysym.scancode > 1)
        return -event.key.keysym.scancode;
#endif
    return 0;
}

int KeyboardConfig::getKeyIndex(const SDL_Event &event, const int grp) const
{
    const int keyValue = getKeyValueFromEvent(event);
    return inputManager.getKeyIndex(keyValue, grp, INPUT_KEYBOARD);
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyName(const int key)
{
    if (key == Input::KEY_NO_VALUE)
        return "";
    if (key >= 0)
    {
#ifdef USE_SDL2
        return SDL_GetKeyName(SDL_GetKeyFromScancode(
            static_cast<SDL_Scancode>(key)));
#else
        return SDL_GetKeyName(static_cast<SDLKey>(key));
#endif
    }

    // TRANSLATORS: long key name, should be short
    return strprintf(_("key_%d"), key);
}

std::string KeyboardConfig::getKeyShortString(const std::string &key)
{
    if (key == "backspace")
        return "bksp";
    else if (key == "numlock")
        return "numlock";
    else if (key == "caps lock")
        return "caplock";
    else if (key == "scroll lock")
        return "scrlock";
    else if (key == "right shift")
        return "rshift";
    else if (key == "left shift")
        return "lshift";
    else if (key == "right ctrl")
        return "rctrl";
    else if (key == "left ctrl")
        return "lctrl";
    else if (key == "right alt")
        return "ralt";
    else if (key == "left alt")
        return "lalt";
    else if (key == "right meta")
        return "rmeta";
    else if (key == "left meta")
        return "lmeta";
    else if (key == "right super")
        return "rsuper";
    else if (key == "left super")
        return "lsuper";
    else if (key == "print screen")
        return "print screen";
    else if (key == "page up")
        return "pg up";
    else if (key == "page down")
        return "pg down";

    if (key == "unknown key")
    {
        // TRANSLATORS: Unknown key short string.
        // TRANSLATORS: This string must be maximum 5 chars
        return _("u key");
    }
    return key;
}

SDLKey KeyboardConfig::getKeyFromEvent(const SDL_Event &event)
{
#ifdef USE_SDL2
    return event.key.keysym.scancode;
#else
    return event.key.keysym.sym;
#endif
}

KeysVector *KeyboardConfig::getActionVector(const SDL_Event &event)
{
    const int i = getKeyValueFromEvent(event);
//    logger->log("key triggerAction: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToAction.find(i) != mKeyToAction.end())
        return &mKeyToAction[i];
    return nullptr;
}

KeysVector *KeyboardConfig::getActionVectorByKey(const int i)
{
//    logger->log("key triggerAction: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToAction.find(i) != mKeyToAction.end())
        return &mKeyToAction[i];
    return nullptr;
}

int KeyboardConfig::getActionId(const SDL_Event &event)
{
    const int i = getKeyValueFromEvent(event);
//    logger->log("getActionId: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToId.find(i) != mKeyToId.end())
        return mKeyToId[i];
    return -1;
}

bool KeyboardConfig::isActionActive(const int index) const
{
    if (!mActiveKeys)
        return false;

    const KeyFunction &key = inputManager.getKey(index);
    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const KeyItem &val = key.values[i];
        if (val.type != INPUT_KEYBOARD)
            continue;

        const int value = val.value;
        if (value >= 0)
        {
            if (mActiveKeys[value])
                return true;
        }
        else if (value < -1 && value > -500)
        {
            if (mActiveKeys2[-value])
                return true;
        }
    }
    return false;
}

void KeyboardConfig::update()
{
    inputManager.updateKeyActionMap(mKeyToAction, mKeyToId,
        mKeyTimeMap, INPUT_KEYBOARD);
}

void KeyboardConfig::handleActicateKey(const SDL_Event &event)
{
    const int key = getKeyValueFromEvent(event);
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 1;
    resetRepeat(key);
}

void KeyboardConfig::handleActicateKey(const int key)
{
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 1;
    resetRepeat(key);
}

void KeyboardConfig::handleDeActicateKey(const SDL_Event &event)
{
    const int key = getKeyValueFromEvent(event);
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 0;
    resetRepeat(key);
}

void KeyboardConfig::handleDeActicateKey(const int key)
{
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 0;
    resetRepeat(key);
}

void KeyboardConfig::handleRepeat(const int time)
{
    FOR_EACH (KeyTimeMapIter, it, mKeyTimeMap)
    {
        bool repeat(false);
        const int key = (*it).first;
        if (key >= 0)
        {
            if (mActiveKeys && mActiveKeys[key])
                repeat = true;
        }
        else if (key < -1 && key > -500)
        {
            if (mActiveKeys2 && mActiveKeys2[-key])
                repeat = true;
        }
        if (repeat)
        {
            int &keyTime = (*it).second;
            if (time > keyTime && abs(time - keyTime)
                > static_cast<signed>(mRepeatTime))
            {
                keyTime = time;
                inputManager.triggerAction(getActionVectorByKey(key));
            }
        }
    }
}

void KeyboardConfig::resetRepeat(const int key)
{
    const KeyTimeMapIter it = mKeyTimeMap.find(key);
    if (it != mKeyTimeMap.end())
        (*it).second = tick_time;
}
