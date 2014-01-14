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

#include "input/inputmanager.h"

#include "configuration.h"
#include "game.h"
#include "touchmanager.h"

#include "input/joystick.h"
#include "input/keyboardconfig.h"
#include "input/keyboarddata.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#ifdef USE_SDL2
#include "input/multitouchmanager.h"
#endif

#include "gui/gui.h"
#include "gui/sdlinput.h"

#include "gui/widgets/tabs/setup_input.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/npcpostdialog.h"
#include "gui/windows/selldialog.h"
#include "gui/windows/setup.h"
#include "gui/windows/textdialog.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/quitdialog.h"

#include "utils/timer.h"

#include <guichan/exception.hpp>
#include <guichan/focushandler.hpp>

#include <algorithm>

#include "debug.h"

InputManager inputManager;

extern QuitDialog *quitDialog;

static class KeyFunctor final
{
    public:
        bool operator() (const int key1, const int key2) const
        {
            return keys[key1].priority >= keys[key2].priority;
        }

        const KeyData *keys;
} keyDataSorter;


InputManager::InputManager() :
    mSetupInput(nullptr),
    mNewKeyIndex(Input::KEY_NO_VALUE),
    mMask(1),
    mNameMap()
{
}

void InputManager::init()
{
    for (unsigned int i = 0; i < Input::KEY_TOTAL; i++)
    {
        KeyFunction &kf = mKey[i];
        for (unsigned int f = 0; f < KeyFunctionSize; f ++)
        {
            KeyItem &ki = kf.values[f];
            ki.type = INPUT_UNKNOWN;
            ki.value = -1;
        }
    }

    mNewKeyIndex = Input::KEY_NO_VALUE;

    resetKeys();
    retrieve();
    update();
}

void InputManager::update() const
{
    keyboard.update();
    if (joystick)
        joystick->update();
}

void InputManager::retrieve()
{
    for (int i = 0; i < Input::KEY_TOTAL; i++)
    {
#ifdef USE_SDL2
        const std::string cf = std::string("sdl2") + keyData[i].configField;
#else
        const std::string cf = keyData[i].configField;
#endif
        if (!cf.empty())
        {
            mNameMap[cf] = i;
            KeyFunction &kf = mKey[i];
            const std::string keyStr = config.getValue(cf, "");
            const int keyStrSize = keyStr.size();
            if (keyStr.empty())
                continue;

            StringVect keys;
            splitToStringVector(keys, keyStr, ',');
            unsigned int i2 = 0;
            for (StringVectCIter it = keys.begin(), it_end = keys.end();
                 it != it_end && i2 < KeyFunctionSize; ++ it)
            {
                std::string keyStr2 = *it;
                if (keyStrSize < 2)
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
                const int key = atoi(keyStr2.c_str());
                if (key >= -255 && key < SDLK_LAST)
                {
                    kf.values[i2] = KeyItem(type, key);
                    i2 ++;
                }
            }
        }
    }
}

void InputManager::store() const
{
    for (int i = 0; i < Input::KEY_TOTAL; i++)
    {
#ifdef USE_SDL2
        const std::string cf = std::string("sdl2") + keyData[i].configField;
#else
        const std::string cf = keyData[i].configField;
#endif
        if (!cf.empty())
        {
            std::string keyStr;
            const KeyFunction &kf = mKey[i];

            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                const KeyItem &key = kf.values[i2];
                if (key.type != INPUT_UNKNOWN)
                {
                    std::string tmp("k");
                    switch (key.type)
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
                    if (key.value != -1)
                    {
                        if (keyStr.empty())
                        {
                            keyStr.append(tmp).append(toString(key.value));
                        }
                        else
                        {
                            keyStr.append(strprintf(",%s%d",
                                tmp.c_str(), key.value));
                        }
                    }
                }
            }
            if (keyStr.empty())
                keyStr = "-1";

            config.setValue(cf, keyStr);
        }
    }
}

void InputManager::resetKeys()
{
    for (int i = 0; i < Input::KEY_TOTAL; i++)
    {
        KeyFunction &key = mKey[i];
        for (size_t i2 = 1; i2 < KeyFunctionSize; i2 ++)
        {
            KeyItem &ki2 = key.values[i2];
            ki2.type = INPUT_UNKNOWN;
            ki2.value = -1;
        }
        const KeyData &kd = keyData[i];
        KeyItem &val0 = key.values[0];
        val0.type = kd.defaultType1;
        KeyItem &val1 = key.values[1];
        val1.type = kd.defaultType2;
#ifdef USE_SDL2
        if (kd.defaultType1 == INPUT_KEYBOARD)
            val0.value = SDL_GetScancodeFromKey(kd.defaultValue1);
        else
            val0.value = kd.defaultValue1;
        if (kd.defaultType2 == INPUT_KEYBOARD)
            val1.value = SDL_GetScancodeFromKey(kd.defaultValue2);
        else
            val1.value = kd.defaultValue2;
#else
        val0.value = kd.defaultValue1;
        val1.value = kd.defaultValue2;
#endif
    }
}

void InputManager::makeDefault(const int i)
{
    if (i >= 0 && i < Input::KEY_TOTAL)
    {
        KeyFunction &key = mKey[i];
        for (size_t i2 = 1; i2 < KeyFunctionSize; i2 ++)
        {
            KeyItem &ki2 = key.values[i2];
            ki2.type = INPUT_UNKNOWN;
            ki2.value = -1;
        }
        const KeyData &kd = keyData[i];
        KeyItem &val0 = key.values[0];
        val0.type = kd.defaultType1;
        KeyItem &val1 = key.values[1];
        val1.type = kd.defaultType2;

#ifdef USE_SDL2
        if (kd.defaultType1 == INPUT_KEYBOARD)
            val0.value = SDL_GetScancodeFromKey(kd.defaultValue1);
        else
            val0.value = kd.defaultValue1;
        if (kd.defaultType2 == INPUT_KEYBOARD)
            val1.value = SDL_GetScancodeFromKey(kd.defaultValue2);
        else
            val1.value = kd.defaultValue2;
#else
        val0.value = kd.defaultValue1;
        val1.value = kd.defaultValue2;
#endif

        update();
    }
}

bool InputManager::hasConflicts(int &restrict key1, int &restrict key2) const
{
    /**
     * No need to parse the square matrix: only check one triangle
     * that's enough to detect conflicts
     */
    for (int i = 0; i < Input::KEY_TOTAL; i++)
    {
        const KeyData &kdi = keyData[i];
        if (!*kdi.configField)
            continue;

        const KeyFunction &ki = mKey[i];
        for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
        {
            const KeyItem &vali2 = ki.values[i2];
            if (vali2.value == Input::KEY_NO_VALUE)
                continue;

            size_t j;
            for (j = i, j++; j < Input::KEY_TOTAL; j++)
            {
                if ((kdi.grp & keyData[j].grp) == 0 || !*kdi.configField)
                    continue;

                for (size_t j2 = 0; j2 < KeyFunctionSize; j2 ++)
                {
                    const KeyItem &valj2 = mKey[j].values[j2];
                    // Allow for item shortcut and emote keys to overlap
                    // as well as emote and ignore keys, but no other keys
                    if (valj2.type != INPUT_UNKNOWN
                        && vali2.value == valj2.value
                        && vali2.type == valj2.type)
                    {
                        key1 = static_cast<int>(i);
                        key2 = static_cast<int>(j);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void InputManager::callbackNewKey()
{
    mSetupInput->newKeyCallback(mNewKeyIndex);
}

bool InputManager::isActionActive(const int index) const
{
    if (!isActionActive0(index))
        return false;

    const KeyData &key = keyData[index];
//    logger->log("isActionActive mask=%d, condition=%d, index=%d",
//        mMask, key.condition, index);
    if ((key.condition & mMask) != key.condition)
        return false;
    return true;
}

bool InputManager::isActionActive0(const int index) const
{
    if (keyboard.isActionActive(index))
        return true;
    if (joystick && joystick->isActionActive(index))
        return true;
    return touchManager.isActionActive(index);
}

KeyFunction &InputManager::getKey(int index)
{
    if (index < 0 || index >= Input::KEY_TOTAL)
        index = 0;
    return mKey[index];
}

std::string InputManager::getKeyStringLong(const int index) const
{
    std::string keyStr;
    const KeyFunction &ki = mKey[index];

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const KeyItem &key = ki.values[i];
        std::string str;
        if (key.type == INPUT_KEYBOARD)
        {
            if (key.value >= 0)
            {
                str = keyboard.getKeyName(key.value);
            }
            else if (key.value < -1)
            {
                // TRANSLATORS: long key name. must be short.
                str = strprintf(_("key_%d"), -key.value);
            }
        }
        else if (key.type == INPUT_JOYSTICK)
        {
            // TRANSLATORS: long joystick button name. must be short.
            str = strprintf(_("JButton%d"), key.value + 1);
        }
        if (!str.empty())
        {
            if (keyStr.empty())
                keyStr = str;
            else
                keyStr.append(", ").append(str);
        }
    }

    if (keyStr.empty())
    {
        // TRANSLATORS: unknown long key type
        return _("unknown key");
    }
    return keyStr;
}

std::string InputManager::getKeyValueString(const int index) const
{
    std::string keyStr;
    const KeyFunction &ki = mKey[index];

    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const KeyItem &key = ki.values[i];
        std::string str;
        if (key.type == INPUT_KEYBOARD)
        {
            if (key.value >= 0)
            {
                str = keyboard.getKeyShortString(
                    keyboard.getKeyName(key.value));
            }
            else if (key.value < -1)
            {
                // TRANSLATORS: short key name. must be very short.
                str = strprintf(_("key_%d"), -key.value);
            }
        }
        else if (key.type == INPUT_JOYSTICK)
        {
            // TRANSLATORS: short joystick button name. muse be very short
            str = strprintf(_("JB%d"), key.value + 1);
        }
        if (!str.empty())
        {
            if (keyStr.empty())
                keyStr = str;
            else
                keyStr.append(" ").append(str);
        }
    }

    if (keyStr.empty())
    {
        // TRANSLATORS: unknown short key type. must be short
        return _("u key");
    }
    return keyStr;
}

std::string InputManager::getKeyValueByName(const std::string &keyName)
{
    const std::map<std::string, int>::const_iterator
        it = mNameMap.find(keyName);

    if (it == mNameMap.end())
        return std::string();
    return getKeyValueString((*it).second);
}

void InputManager::addActionKey(const int action, const int type,
                                const int val)
{
    if (action < 0 || action >= Input::KEY_TOTAL)
        return;

    int idx = -1;
    KeyFunction &key = mKey[action];
    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        const KeyItem &val2 = key.values[i];
        if (val2.type == INPUT_UNKNOWN || (val2.type == type
            && val2.value == val))
        {
            idx = static_cast<int>(i);
            break;
        }
    }
    if (idx == -1)
    {
        for (size_t i = 1; i < KeyFunctionSize; i ++)
        {
            KeyItem &val1 = key.values[i - 1];
            KeyItem &val2 = key.values[i];
            val1.type = val2.type;
            val1.value = val2.value;
        }
        idx = KeyFunctionSize - 1;
    }

    key.values[idx] = KeyItem(type, val);
}

void InputManager::setNewKey(const SDL_Event &event, const int type)
{
    int val = -1;
    if (type == INPUT_KEYBOARD)
        val = keyboard.getKeyValueFromEvent(event);
    else if (type == INPUT_JOYSTICK && joystick)
        val = joystick->getButtonFromEvent(event);

    if (val != -1)
    {
        addActionKey(mNewKeyIndex, type, val);
        update();
    }
}

void InputManager::unassignKey()
{
    KeyFunction &key = mKey[mNewKeyIndex];
    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        KeyItem &val = key.values[i];
        val.type = INPUT_UNKNOWN;
        val.value = -1;
    }
    update();
}

bool InputManager::handleAssignKey(const SDL_Event &event, const int type)
{
    if (setupWindow && setupWindow->isWindowVisible() &&
        getNewKeyIndex() > Input::KEY_NO_VALUE)
    {
        setNewKey(event, type);
        callbackNewKey();
        setNewKeyIndex(Input::KEY_NO_VALUE);
        return true;
    }
    return false;
}

bool InputManager::handleEvent(const SDL_Event &event)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
        {
            keyboard.refreshActiveKeys();
            updateConditionMask();
            if (handleAssignKey(event, INPUT_KEYBOARD))
                return true;

            keyboard.handleActicateKey(event);
            // send straight to gui for certain windows
            if (quitDialog || TextDialog::isActive() ||
                NpcPostDialog::isActive())
            {
                try
                {
                    if (guiInput)
                        guiInput->pushInput(event);
                    if (gui)
                        gui->handleInput();
                }
                catch(const gcn::Exception &e)
                {
                    const char *const err = e.getMessage().c_str();
                    logger->log("Warning: guichan input exception: %s", err);
                }
                return true;
            }
            break;
        }
        case SDL_KEYUP:
        {
            keyboard.refreshActiveKeys();
            updateConditionMask();
            keyboard.handleDeActicateKey(event);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        {
            updateConditionMask();
//            joystick.handleActicateButton(event);
            if (handleAssignKey(event, INPUT_JOYSTICK))
                return true;
            break;
        }
        case SDL_JOYBUTTONUP:
        {
            updateConditionMask();
//            joystick.handleDeActicateButton(event);
            break;
        }
#ifdef USE_SDL2
        case SDL_FINGERDOWN:
            multiTouchManager.handleFingerDown(event);
            break;
        case SDL_FINGERUP:
            multiTouchManager.handleFingerUp(event);
            break;
#else
#ifdef ANDROID
        case SDL_ACCELEROMETER:
        {
            break;
        }
#endif
#endif
        default:
            break;
    }

    try
    {
        if (guiInput)
            guiInput->pushInput(event);
    }
    catch(const gcn::Exception &e)
    {
        const char *const err = e.getMessage().c_str();
        logger->log("Warning: guichan input exception: %s", err);
    }
    if (gui)
    {
        const bool res = gui->handleInput();
        if (res && event.type == SDL_KEYDOWN)
            return true;
    }

    switch (event.type)
    {
        case SDL_KEYDOWN:
            if (triggerAction(keyboard.getActionVector(event)))
                return true;
            break;

        case SDL_JOYBUTTONDOWN:
            if (joystick && joystick->validate())
            {
                if (triggerAction(joystick->getActionVector(event)))
                    return true;
            }
            break;
#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
        {
            break;
        }
#endif
#endif
        default:
            break;
    }

    return false;
}

void InputManager::handleRepeat() const
{
    const int time = tick_time;
    keyboard.handleRepeat(time);
    if (joystick)
        joystick->handleRepeat(time);
}

void InputManager::updateConditionMask()
{
    mMask = 1;
    if (keyboard.isEnabled())
        mMask |= COND_ENABLED;
    if ((!chatWindow || !chatWindow->isInputFocused()) &&
        !NpcDialog::isAnyInputFocused() &&
        !InventoryWindow::isAnyInputFocused() &&
        (!tradeWindow || !tradeWindow->isInpupFocused()))
    {
        mMask |= COND_NOINPUT;
    }

    if (!BuyDialog::isActive() && !SellDialog::isActive())
        mMask |= COND_NOBUYSELL;

    if (!player_node || !player_node->getAway())
        mMask |= COND_NOAWAY;

    if (!setupWindow || !setupWindow->isWindowVisible())
        mMask |= COND_NOSETUP;

    if (Game::instance() && Game::instance()->getValidSpeed())
        mMask |= COND_VALIDSPEED;

    if (gui && !gui->getFocusHandler()->getModalFocused())
        mMask |= COND_NOMODAL;

    const NpcDialog *const dialog = NpcDialog::getActive();
    if (!dialog || !dialog->isTextInputFocused())
        mMask |= COND_NONPCINPUT;
    if (!dialog || dialog->isCloseState())
    {
        mMask |= COND_NONPCDIALOG;
        if (!InventoryWindow::isStorageActive())
            mMask |= COND_NOTALKING;
    }

    if (!player_node || !player_node->getDisableGameModifiers())
        mMask |= COND_EMODS;

    if (!isActionActive0(Input::KEY_STOP_ATTACK)
        && !isActionActive0(Input::KEY_UNTARGET))
    {
        mMask |= COND_NOTARGET;
    }
    if (Game::instance())
        mMask |= COND_INGAME;

    if (!player_node || player_node->getFollow().empty())
        mMask |= COND_NOFOLLOW;
}

bool InputManager::checkKey(const KeyData *const key) const
{
    // logger->log("checkKey mask=%d, condition=%d", mMask, key->condition);
    if (!key || (key->condition & mMask) != key->condition)
        return false;

    return (key->modKeyIndex == Input::KEY_NO_VALUE
        || isActionActive0(key->modKeyIndex));
}

bool InputManager::invokeKey(const KeyData *const key, const int keyNum)
{
    // no validation to keyNum because it validated in caller

    if (checkKey(key))
    {
        InputEvent evt(keyNum, mMask);
        ActionFuncPtr func = *(keyData[keyNum].action);
        if (func && func(evt))
            return true;
    }
    return false;
}

void InputManager::executeAction(const int keyNum)
{
    if (keyNum < 0 || keyNum >= Input::KEY_TOTAL)
        return;

    InputEvent evt(keyNum, mMask);
    ActionFuncPtr func = *(keyData[keyNum].action);
    if (func)
        func(evt);
}

void InputManager::updateKeyActionMap(KeyToActionMap &actionMap,
                                      KeyToIdMap &idMap,
                                      KeyTimeMap &keyTimeMap,
                                      const int type) const
{
    actionMap.clear();
    keyTimeMap.clear();

    for (size_t i = 0; i < Input::KEY_TOTAL; i ++)
    {
        const KeyFunction &key = mKey[i];
        const KeyData &kd = keyData[i];
        if (kd.action)
        {
            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                const KeyItem &ki = key.values[i2];
                if (ki.type == type && ki.value != -1)
                    actionMap[ki.value].push_back(static_cast<int>(i));
            }
        }
        if (kd.configField && (kd.grp & Input::GRP_GUICHAN))
        {
            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                const KeyItem &ki = key.values[i2];
                if (ki.type == type && ki.value != -1)
                    idMap[ki.value] = static_cast<int>(i);
            }
        }
        if (kd.configField && (kd.grp & Input::GRP_REPEAT))
        {
            for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
            {
                const KeyItem &ki = key.values[i2];
                if (ki.type == type && ki.value != -1)
                    keyTimeMap[ki.value] = 0;
            }
        }
    }

    keyDataSorter.keys = &keyData[0];
    FOR_EACH (KeyToActionMapIter, it, actionMap)
    {
        KeysVector *const keys = &it->second;
        if (keys && keys->size() > 1)
            std::sort(keys->begin(), keys->end(), keyDataSorter);
    }
}

bool InputManager::triggerAction(const KeysVector *const ptrs)
{
    if (!ptrs)
        return false;

//    logger->log("ptrs: %d", (int)ptrs.size());

    FOR_EACHP (KeysVectorCIter, it, ptrs)
    {
        const int keyNum = *it;
        if (keyNum < 0 || keyNum >= Input::KEY_TOTAL)
            continue;

        if (invokeKey(&keyData[keyNum], keyNum))
            return true;
    }
    return false;
}

int InputManager::getKeyIndex(const int value, const int grp,
                              const int type) const
{
    for (size_t i = 0; i < Input::KEY_TOTAL; i++)
    {
        const KeyFunction &key = mKey[i];
        const KeyData &kd = keyData[i];
        for (size_t i2 = 0; i2 < KeyFunctionSize; i2 ++)
        {
            const KeyItem &vali2 = key.values[i2];
            if (value == vali2.value && (grp & kd.grp) != 0
                && vali2.type == type)
            {
                return static_cast<int>(i);
            }
        }
    }
    return Input::KEY_NO_VALUE;
}

int InputManager::getActionByKey(const SDL_Event &event) const
{
    // for now support only keyboard events
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        const int idx = keyboard.getActionId(event);
        if (idx >= 0 && checkKey(&keyData[idx]))
            return idx;
    }
    return Input::KEY_NO_VALUE;
}
