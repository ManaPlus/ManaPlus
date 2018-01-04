/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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
#ifdef USE_SDL2
#include "settings.h"
#endif  // USE_SDL2

#include "input/inputmanager.h"

#include "utils/foreach.h"
#include "utils/gettext.h"

#include "debug.h"

extern volatile int tick_time;

KeyboardConfig keyboard;

KeyboardConfig::KeyboardConfig() :
    mEnabled(true),
    mActiveKeys(nullptr),
    mActiveKeys2(nullptr),
    mRepeatTime(0),
    mKeyToAction(),
    mKeyToId(),
    mKeyTimeMap(),
    mBlockAltTab(true)
{
}

void KeyboardConfig::init()
{
    mEnabled = true;
    delete [] mActiveKeys2;
    mActiveKeys2 = new uint8_t[500];
    mRepeatTime = config.getIntValue("repeateInterval2") / 10;
    mBlockAltTab = config.getBoolValue("blockAltTab");
}

void KeyboardConfig::deinit()
{
    delete [] mActiveKeys2;
    mActiveKeys2 = nullptr;
}

InputActionT KeyboardConfig::getKeyIndex(const SDL_Event &event, const int grp)
{
    const int keyValue = getKeyValueFromEvent(event);
    return inputManager.getKeyIndex(keyValue, grp, InputType::KEYBOARD);
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyName(const int key)
{
    if (key == -1)
        return "";
    if (key > -1)
    {
#ifdef USE_SDL2
        return SDL_GetScancodeName(
            static_cast<SDL_Scancode>(key));
#else  // USE_SDL2

        return SDL_GetKeyName(static_cast<SDLKey>(key));
#endif  // USE_SDL2
    }

    // TRANSLATORS: long key name, should be short
    return strprintf(_("key_%d"), CAST_S32(key));
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

InputActionT KeyboardConfig::getActionId(const SDL_Event &event)
{
    const int i = getKeyValueFromEvent(event);
//    logger->log("getActionId: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToId.find(i) != mKeyToId.end())
        return mKeyToId[i];
    return InputAction::NO_VALUE;
}

bool KeyboardConfig::isActionActive(const InputActionT index) const
{
    if ((mActiveKeys == nullptr) || (mActiveKeys2 == nullptr))
        return false;

    const InputFunction &key = inputManager.getKey(index);
    for (size_t i = 0; i < inputFunctionSize; i ++)
    {
        const InputItem &val = key.values[i];
        if (val.type != InputType::KEYBOARD)
            continue;

        const int value = val.value;
        if (value >= 0)
        {
            if (mActiveKeys[value] != 0u)
                return true;
        }
        else if (value < -1 && value > -500)
        {
            if (mActiveKeys2[-value] != 0u)
                return true;
        }
    }
    return false;
}

void KeyboardConfig::update()
{
    inputManager.updateKeyActionMap(mKeyToAction, mKeyToId,
        mKeyTimeMap, InputType::KEYBOARD);
}

void KeyboardConfig::handleActivateKey(const SDL_Event &event)
{
    if (mActiveKeys2 == nullptr)
        return;
    const int key = getKeyValueFromEvent(event);
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 1;
    resetRepeat(key);
}

void KeyboardConfig::handleActivateKey(const int key)
{
    if (mActiveKeys2 == nullptr)
        return;
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 1;
    resetRepeat(key);
}

void KeyboardConfig::handleDeActicateKey(const SDL_Event &event)
{
    if (mActiveKeys2 == nullptr)
        return;
    const int key = getKeyValueFromEvent(event);
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 0;
    resetRepeat(key);
}

void KeyboardConfig::handleDeActicateKey(const int key)
{
    if (mActiveKeys2 == nullptr)
        return;
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 0;
    resetRepeat(key);
}

void KeyboardConfig::handleRepeat(const int time)
{
    BLOCK_START("KeyboardConfig::handleRepeat")
    FOR_EACH (KeyTimeMapIter, it, mKeyTimeMap)
    {
        bool repeat(false);
        const int key = (*it).first;
        if (key >= 0)
        {
            if ((mActiveKeys != nullptr) && (mActiveKeys[key] != 0u))
                repeat = true;
        }
        else if (key < -1 && key > -500)
        {
            if ((mActiveKeys2 != nullptr) && (mActiveKeys2[-key] != 0u))
                repeat = true;
        }
        if (repeat)
        {
            int &keyTime = (*it).second;
            if (time > keyTime && abs(time - keyTime)
                > CAST_S32(mRepeatTime))
            {
                keyTime = time;
                inputManager.triggerAction(getActionVectorByKey(key));
            }
        }
    }
    BLOCK_END("KeyboardConfig::handleRepeat")
}

void KeyboardConfig::resetRepeat(const int key)
{
    const KeyTimeMapIter it = mKeyTimeMap.find(key);
    if (it != mKeyTimeMap.end())
        (*it).second = tick_time;
}

#ifdef USE_SDL2

bool KeyboardConfig::ignoreKey(const SDL_Event &restrict event)
{
    if (!mBlockAltTab ||
        mActiveKeys == nullptr)
    {
        return false;
    }
    const int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_TAB)
    {
#if SDL_VERSION_ATLEAST(2, 0, 5)
        // SDL_WINDOWEVENT_TAKE_FOCUS not triggered after focus restored
        if (settings.inputFocused != KeyboardFocus::Focused2)
            return true;
#endif  // SDL_VERSION_ATLEAST(2, 0, 5)

        if (mActiveKeys[SDL_SCANCODE_LALT] != 0)
            return true;
    }
    else if (key == SDL_SCANCODE_LALT)
    {
        if (mActiveKeys[SDL_SCANCODE_TAB] != 0)
            return true;
    }
    return false;
}
#endif  // USE_SDL2
