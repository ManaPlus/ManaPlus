/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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
#include "settings.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "input/inputactionmap.h"
#include "input/inputactionsortfunctor.h"
#include "input/joystick.h"
#include "input/keyboardconfig.h"
#ifdef USE_SDL2
#include "input/touch/multitouchmanager.h"
#endif  // USE_SDL2

#include "input/touch/touchmanager.h"

#include "gui/gui.h"
#include "gui/sdlinput.h"

#include "gui/widgets/selldialog.h"
#include "gui/widgets/textfield.h"

#include "gui/widgets/tabs/setup_input.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/textdialog.h"
#include "gui/windows/tradewindow.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/stdmove.h"
#include "utils/timer.h"

#include "gui/focushandler.h"

#include <algorithm>

#include "debug.h"

InputManager inputManager;

class QuitDialog;

extern QuitDialog *quitDialog;

namespace
{
    InputActionSortFunctor inputActionDataSorter;
}  // namespace

InputManager::InputManager() :
    mSetupInput(nullptr),
    mNewKeyIndex(InputAction::NO_VALUE),
    mMask(1),
    mNameMap(),
    mChatMap(),
    mKey()
{
}

void InputManager::init() restrict2
{
#ifdef __SWITCH__
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
#endif
    for (size_t i = 0;
         i < CAST_SIZE(InputAction::TOTAL);
         i ++)
    {
        InputFunction &kf = mKey[i];
        mKeyStr[i].clear();
        for (size_t f = 0; f < inputFunctionSize; f ++)
        {
            InputItem &ki = kf.values[f];
            ki.type = InputType::UNKNOWN;
            ki.value = -1;
        }
    }

    mNewKeyIndex = InputAction::NO_VALUE;

    resetKeys();
    retrieve();
    update();
}

void InputManager::update()
{
    keyboard.update();
    if (joystick != nullptr)
        joystick->update();
}

void InputManager::retrieve() restrict2
{
    for (int i = 0; i < CAST_S32(InputAction::TOTAL); i ++)
    {
        const std::string &restrict cmd = inputActionData[i].chatCommand;
        if (!cmd.empty())
        {
            StringVect tokens;
            splitToStringVector(tokens, cmd, '|');
            FOR_EACH (StringVectCIter, it, tokens)
                mChatMap[*it] = i;
        }
#ifdef USE_SDL2
        const std::string cf = std::string("sdl2")
            + inputActionData[i].configField;
#else  // USE_SDL2

        const std::string cf = inputActionData[i].configField;
#endif  // USE_SDL2

        InputFunction &restrict kf = mKey[i];
        if (!cf.empty())
        {
            mNameMap[cf] = static_cast<InputActionT>(i);
            const std::string keyStr = config.getValue(cf, "");
            const size_t keyStrSize = keyStr.size();
            if (keyStr.empty())
            {
                updateKeyString(kf, i);
                continue;
            }

            StringVect keys;
            splitToStringVector(keys, keyStr, ',');
            unsigned int i2 = 0;
            for (StringVectCIter it = keys.begin(), it_end = keys.end();
                 it != it_end && i2 < inputFunctionSize; ++ it)
            {
                std::string keyStr2 = *it;
                if (keyStrSize < 2)
                    continue;
                InputTypeT type = InputType::KEYBOARD;
                if ((keyStr2[0] < '0' || keyStr2[0] > '9')
                    && keyStr2[0] != '-')
                {
                    switch (keyStr2[0])
                    {
                        case 'm':
                            type = InputType::MOUSE;
                            break;
                        case 'j':
                            type = InputType::JOYSTICK;
                            break;
                        default:
                            break;
                    }
                    keyStr2 = keyStr2.substr(1);
                }
                const int key = atoi(keyStr2.c_str());
                if (key >= -255 && key < SDLK_LAST)
                {
                    kf.values[i2] = InputItem(type, key);
                    i2 ++;
                }
            }
            for (; i2 < inputFunctionSize; i2 ++)
                kf.values[i2] = InputItem();
        }
        updateKeyString(kf, i);
    }
}

void InputManager::store() const restrict2
{
    for (int i = 0; i < CAST_S32(InputAction::TOTAL); i ++)
    {
#ifdef USE_SDL2
        const std::string cf = std::string("sdl2")
            + inputActionData[i].configField;
#else  // USE_SDL2

        const std::string cf = inputActionData[i].configField;
#endif  // USE_SDL2

        if (!cf.empty())
        {
            std::string keyStr;
            const InputFunction &restrict kf = mKey[i];

            for (size_t i2 = 0; i2 < inputFunctionSize; i2 ++)
            {
                const InputItem &restrict key = kf.values[i2];
                if (key.type != InputType::UNKNOWN)
                {
                    std::string tmp("k");
                    switch (key.type)
                    {
                        case InputType::MOUSE:
                            tmp = "m";
                            break;
                        case InputType::JOYSTICK:
                            tmp = "j";
                            break;
                        case InputType::KEYBOARD:
                        case InputType::UNKNOWN:
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

void InputManager::resetKey(const InputActionT i) restrict2
{
    InputFunction &restrict key = mKey[CAST_SIZE(i)];
    for (size_t i2 = 1; i2 < inputFunctionSize; i2 ++)
    {
        InputItem &restrict ki2 = key.values[i2];
        ki2.type = InputType::UNKNOWN;
        ki2.value = -1;
    }
    const InputActionData &restrict kd =
        inputActionData[CAST_SIZE(i)];
    InputItem &restrict val0 = key.values[0];
    val0.type = kd.defaultType1;
    InputItem &restrict val1 = key.values[1];
    val1.type = kd.defaultType2;
#ifdef USE_SDL2
    if (kd.defaultType1 == InputType::KEYBOARD)
    {
        val0.value = SDL_GetScancodeFromKey(kd.defaultValue1);
        if (val0.value == SDL_SCANCODE_UNKNOWN)
            val0.value = -1;
    }
    else
        val0.value = kd.defaultValue1;
    if (kd.defaultType2 == InputType::KEYBOARD)
    {
        val1.value = SDL_GetScancodeFromKey(kd.defaultValue2);
        if (val1.value == SDL_SCANCODE_UNKNOWN)
            val1.value = -1;
    }
    else
        val1.value = kd.defaultValue2;
#else  // USE_SDL2

    val0.value = kd.defaultValue1;
    val1.value = kd.defaultValue2;
#endif  // USE_SDL2

    updateKeyString(key, CAST_SIZE(i));
}

void InputManager::resetKeys() restrict2
{
    for (int i = 0; i < CAST_S32(InputAction::TOTAL); i++)
        resetKey(static_cast<InputActionT>(i));
}

void InputManager::makeDefault(const InputActionT i) restrict2
{
    if (i > InputAction::NO_VALUE && i < InputAction::TOTAL)
    {
        resetKey(i);
        update();
    }
}

bool InputManager::hasConflicts(InputActionT &restrict key1,
                                InputActionT &restrict key2) const restrict2
{
    /**
     * No need to parse the square matrix: only check one triangle
     * that's enough to detect conflicts
     */
    for (int i = 0; i < CAST_S32(InputAction::TOTAL); i++)
    {
        const InputActionData &restrict kdi = inputActionData[i];
        if (*kdi.configField == 0)
            continue;

        const InputFunction &restrict ki = mKey[i];
        for (size_t i2 = 0; i2 < inputFunctionSize; i2 ++)
        {
            const InputItem &restrict vali2 = ki.values[i2];
            if (vali2.value == -1)
                continue;

            size_t j;
            for (j = i, j++; j < CAST_S32(InputAction::TOTAL); j++)
            {
                if ((kdi.grp & inputActionData[j].grp) == 0
                    || (*kdi.configField == 0))
                {
                    continue;
                }

                for (size_t j2 = 0; j2 < inputFunctionSize; j2 ++)
                {
                    const InputItem &restrict valj2 = mKey[j].values[j2];
                    // Allow for item shortcut and emote keys to overlap
                    // as well as emote and ignore keys, but no other keys
                    if (valj2.type != InputType::UNKNOWN
                        && vali2.value == valj2.value
                        && vali2.type == valj2.type)
                    {
                        key1 = static_cast<InputActionT>(i);
                        key2 = static_cast<InputActionT>(j);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void InputManager::callbackNewKey() restrict2
{
#ifndef DYECMD
    mSetupInput->newKeyCallback(mNewKeyIndex);
#endif  // DYECMD
}

bool InputManager::isActionActive(const InputActionT index) const restrict2
{
    if (!isActionActive0(index))
        return false;

    const InputActionData &restrict key =
        inputActionData[CAST_SIZE(index)];
//    logger->log("isActionActive mask=%d, condition=%d, index=%d",
//        mMask, key.condition, index);
    if ((key.condition & mMask) != key.condition)
        return false;
    return true;
}

bool InputManager::isActionActive0(const InputActionT index)
{
    if (keyboard.isActionActive(index))
        return true;
    if ((joystick != nullptr) && joystick->isActionActive(index))
        return true;
    return touchManager.isActionActive(index);
}

InputFunction &InputManager::getKey(InputActionT index) restrict2
{
    if (CAST_S32(index) < 0 || index >= InputAction::TOTAL)
        index = InputAction::MOVE_UP;
    return mKey[CAST_SIZE(index)];
}

std::string InputManager::getKeyStringLong(const InputActionT index) const
                                           restrict2
{
    std::string keyStr;
    const InputFunction &restrict ki = mKey[CAST_SIZE(index)];

    for (size_t i = 0; i < inputFunctionSize; i ++)
    {
        const InputItem &restrict key = ki.values[i];
        std::string str;
        if (key.type == InputType::KEYBOARD)
        {
            if (key.value >= 0)
            {
                str = KeyboardConfig::getKeyName(key.value);
            }
            else if (key.value < -1)
            {
                // TRANSLATORS: long key name. must be short.
                str = strprintf(_("key_%d"), -key.value);
            }
        }
        else if (key.type == InputType::JOYSTICK)
        {
            // TRANSLATORS: long joystick button name. must be short.
            if (key.value < Joystick::MAX_BUTTONS)
                str = strprintf(_("JButton%d"), key.value + 1);
            if (key.value == Joystick::KEY_UP)
                str = _("JUp");
            if (key.value == Joystick::KEY_DOWN)
                str = _("JDown");
            if (key.value == Joystick::KEY_LEFT)
                str = _("JLeft");
            if (key.value == Joystick::KEY_RIGHT)
                str = _("JRight");
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

void InputManager::updateKeyString(const InputFunction &ki,
                                   const size_t actionIdx) restrict2
{
    std::string keyStr;
    for (size_t i = 0; i < inputFunctionSize; i ++)
    {
        const InputItem &restrict key = ki.values[i];
        std::string str;
        if (key.type == InputType::KEYBOARD)
        {
            if (key.value >= 0)
            {
                str = KeyboardConfig::getKeyShortString(
                    KeyboardConfig::getKeyName(key.value));
            }
            else if (key.value < -1)
            {
                // TRANSLATORS: short key name. must be very short.
                str = strprintf(_("key_%d"), -key.value);
            }
        }
        else if (key.type == InputType::JOYSTICK)
        {
            // TRANSLATORS: short joystick button name. muse be very short
            if (key.value < Joystick::MAX_BUTTONS)
                str = strprintf(_("JB%d"), key.value + 1);
            if (key.value == Joystick::KEY_UP)
                str = _("JUp");
            if (key.value == Joystick::KEY_DOWN)
                str = _("JDown");
            if (key.value == Joystick::KEY_LEFT)
                str = _("JLeft");
            if (key.value == Joystick::KEY_RIGHT)
                str = _("JRight");
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
        // TRANSLATORS: unknown short key type. must be short
        mKeyStr[actionIdx] = _("u key");
    }
    else
    {
        mKeyStr[actionIdx] = STD_MOVE(keyStr);
    }
}

std::string InputManager::getKeyValueString(const InputActionT index) const
                                            restrict2
{
    return mKeyStr[CAST_SIZE(index)];
}

std::string InputManager::getKeyValueByName(const std::string &restrict
                                            keyName) restrict2
{
    const StringInpActionMapCIter it = mNameMap.find(keyName);

    if (it == mNameMap.end())
        return std::string();
    return getKeyValueString((*it).second);
}

std::string InputManager::getKeyValueByNameLong(const std::string &restrict
                                                keyName) restrict2
{
    const StringInpActionMapCIter it = mNameMap.find(keyName);

    if (it == mNameMap.end())
        return std::string();
    return getKeyStringLong((*it).second);
}

void InputManager::addActionKey(const InputActionT action,
                                const InputTypeT type,
                                const int val) restrict2
{
    if (CAST_S32(action) < 0 || action >= InputAction::TOTAL)
        return;

    int idx = -1;
    InputFunction &restrict key = mKey[CAST_SIZE(action)];
    for (size_t i = 0; i < inputFunctionSize; i ++)
    {
        const InputItem &restrict val2 = key.values[i];
        if (val2.type == InputType::UNKNOWN ||
            (val2.type == type && val2.value == val))
        {
            idx = CAST_S32(i);
            break;
        }
    }
    if (idx == -1)
    {
        for (size_t i = 1; i < inputFunctionSize; i ++)
        {
            InputItem &restrict val1 = key.values[i - 1];
            InputItem &restrict val2 = key.values[i];
            val1.type = val2.type;
            val1.value = val2.value;
        }
        idx = inputFunctionSize - 1;
    }

    key.values[idx] = InputItem(type, val);
    updateKeyString(key, CAST_SIZE(action));
}

void InputManager::setNewKey(const SDL_Event &event,
                             const InputTypeT type) restrict2
{
    int val = -1;
    if (type == InputType::KEYBOARD)
        val = KeyboardConfig::getKeyValueFromEvent(event);
    else if (type == InputType::JOYSTICK && (joystick != nullptr))
        val = joystick->getButtonFromEvent(event);

    if (val != -1)
    {
        addActionKey(mNewKeyIndex, type, val);
        update();
    }
}

void InputManager::unassignKey() restrict2
{
    InputFunction &restrict key = mKey[CAST_SIZE(mNewKeyIndex)];
    for (size_t i = 0; i < inputFunctionSize; i ++)
    {
        InputItem &restrict val = key.values[i];
        val.type = InputType::UNKNOWN;
        val.value = -1;
    }
    updateKeyString(key, CAST_SIZE(mNewKeyIndex));
    update();
}

#ifndef DYECMD
bool InputManager::handleAssignKey(const SDL_Event &restrict event,
                                   const InputTypeT type) restrict2
{
    if ((setupWindow != nullptr) && setupWindow->isWindowVisible() &&
        getNewKeyIndex() > InputAction::NO_VALUE)
    {
        setNewKey(event, type);
        callbackNewKey();
        setNewKeyIndex(InputAction::NO_VALUE);
        return true;
    }
    return false;
}
#else  // DYECMD

bool InputManager::handleAssignKey(const SDL_Event &restrict event A_UNUSED,
                                   const InputTypeT type A_UNUSED) restrict2
{
    return false;
}
#endif  // DYECMD

bool InputManager::handleEvent(const SDL_Event &restrict event) restrict2
{
    BLOCK_START("InputManager::handleEvent")
    switch (event.type)
    {
        case SDL_KEYDOWN:
        {
#ifdef USE_SDL2
            if (keyboard.ignoreKey(event))
            {
                BLOCK_END("InputManager::handleEvent")
                return true;
            }
#endif  // USE_SDL2

            keyboard.refreshActiveKeys();
            updateConditionMask(true);
            if (handleAssignKey(event, InputType::KEYBOARD))
            {
                BLOCK_END("InputManager::handleEvent")
                return true;
            }

            keyboard.handleActivateKey(event);
            // send straight to gui for certain windows
#ifndef DYECMD
            if ((quitDialog != nullptr) || TextDialog::isActive())
            {
                if (guiInput != nullptr)
                    guiInput->pushInput(event);
                if (gui != nullptr)
                    gui->handleInput();
                BLOCK_END("InputManager::handleEvent")
                return true;
            }
#endif  // DYECMD

            break;
        }
        case SDL_KEYUP:
        {
#ifdef USE_SDL2
            if (keyboard.ignoreKey(event))
            {
                BLOCK_END("InputManager::handleEvent")
                return true;
            }
#endif  // USE_SDL2

            keyboard.refreshActiveKeys();
            updateConditionMask(false);
            keyboard.handleDeActicateKey(event);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYHATMOTION:
        {
            updateConditionMask(true);
//            joystick.handleActicateButton(event);
            if (handleAssignKey(event, InputType::JOYSTICK))
            {
                BLOCK_END("InputManager::handleEvent")
                return true;
            }
            break;
        }
        case SDL_JOYBUTTONUP:
        {
            updateConditionMask(false);
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
#else  // USE_SDL2
#ifdef ANDROID
        case SDL_ACCELEROMETER:
        {
            break;
        }
#endif  // ANDROID
#endif  // USE_SDL2

        default:
            break;
    }

    if (guiInput != nullptr)
        guiInput->pushInput(event);
    if (gui != nullptr)
    {
        const bool res = gui->handleInput();
        if (res && event.type == SDL_KEYDOWN)
        {
            BLOCK_END("InputManager::handleEvent")
            return true;
        }
    }

    switch (event.type)
    {
        case SDL_KEYDOWN:
            if (isActionActive(InputAction::IGNORE_INPUT_1) ||
                isActionActive(InputAction::IGNORE_INPUT_2))
            {
                BLOCK_END("InputManager::handleEvent")
                return true;
            }
            if (triggerAction(keyboard.getActionVector(event)))
            {
                BLOCK_END("InputManager::handleEvent")
                return true;
            }
            break;

// disabled temporary
//        case SDL_KEYUP:
//            if (triggerAction(keyboard.getActionVector(event)))
//            {
//                BLOCK_END("InputManager::handleEvent")
//                return true;
//            }
//            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYHATMOTION:
            if ((joystick != nullptr) && joystick->validate())
            {
                if (triggerAction(joystick->getActionVector(event)))
                {
                    BLOCK_END("InputManager::handleEvent")
                    return true;
                }
            }
            break;
#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
        {
            break;
        }
#endif  // USE_SDL2
#endif  // ANDROID

        default:
            break;
    }

    BLOCK_END("InputManager::handleEvent")
    return false;
}

void InputManager::handleRepeat()
{
    const int time = tick_time;
    keyboard.handleRepeat(time);
    if (joystick != nullptr)
        joystick->handleRepeat(time);
}

void InputManager::updateConditionMask(const bool pressed A_UNUSED) restrict2
{
    mMask = 1;
    if (keyboard.isEnabled())
        mMask |= InputCondition::ENABLED;
#ifndef DYECMD
    if (((chatWindow == nullptr) || !chatWindow->isInputFocused()) &&
        !NpcDialog::isAnyInputFocused() &&
        !InventoryWindow::isAnyInputFocused() &&
        ((tradeWindow == nullptr) || !tradeWindow->isInpupFocused()))
    {
        if (gui != nullptr)
        {
            FocusHandler *restrict const focus = gui->getFocusHandler();
            if (focus != nullptr)
            {
                if (dynamic_cast<TextField*>(focus->getFocused()) == nullptr)
                    mMask |= InputCondition::NOINPUT;
            }
            else
            {
                mMask |= InputCondition::NOINPUT;
            }
        }
        else
        {
            mMask |= InputCondition::NOINPUT;
        }
    }

    if (!BuyDialog::isActive() && !SellDialog::isActive())
        mMask |= InputCondition::NOBUYSELL;

    if (!PlayerInfo::isVending())
        mMask |= InputCondition::NOVENDING;
    if (!PlayerInfo::isInRoom())
        mMask |= InputCondition::NOROOM;

    const NpcDialog *restrict const dialog = NpcDialog::getActive();
    if ((dialog == nullptr) || !dialog->isTextInputFocused())
        mMask |= InputCondition::NONPCINPUT;
    if ((dialog == nullptr) || (dialog->isCloseState() != 0))
    {
        mMask |= InputCondition::NONPCDIALOG;
        if (!InventoryWindow::isStorageActive())
            mMask |= InputCondition::NOTALKING;
    }
    if ((setupWindow == nullptr) || !setupWindow->isWindowVisible())
        mMask |= InputCondition::NOSETUP;

    if ((Game::instance() != nullptr) && Game::instance()->getValidSpeed())
        mMask |= InputCondition::VALIDSPEED;

    if (Game::instance() != nullptr)
        mMask |= InputCondition::INGAME;

    if (localPlayer != nullptr)
    {
        if (localPlayer->getFollow().empty())
            mMask |= InputCondition::NOFOLLOW;

        if (!localPlayer->isTrickDead())
            mMask |= InputCondition::NOBLOCK;

        if (localPlayer->isAlive())
            mMask |= InputCondition::ALIVE;
    }
    else
    {
        mMask |= InputCondition::NOFOLLOW;
        mMask |= InputCondition::NOBLOCK;
    }
#endif  // DYECMD

    if (!settings.awayMode)
        mMask |= InputCondition::NOAWAY;

    if (gui != nullptr && gui->getFocusHandler()->getModalFocused() == nullptr)
        mMask |= InputCondition::NOMODAL;

    if (!settings.disableGameModifiers)
        mMask |= InputCondition::EMODS;

    if (!isActionActive0(InputAction::STOP_ATTACK)
        && !isActionActive0(InputAction::UNTARGET))
    {
        mMask |= InputCondition::NOTARGET;
    }
    // enable it temporary
    mMask |= InputCondition::KEY_DOWN;
//    if (pressed == true)
//        mMask |= InputCondition::KEY_DOWN;
//    else
//        mMask |= InputCondition::KEY_UP;
}

bool InputManager::checkKey(const InputActionData *restrict const key) const
                            restrict2
{
    // logger->log("checkKey mask=%d, condition=%d", mMask, key->condition);
    if ((key == nullptr) || (key->condition & mMask) != key->condition)
        return false;

    return key->modKeyIndex == InputAction::NO_VALUE
        || isActionActive0(key->modKeyIndex);
}

bool InputManager::invokeKey(const InputActionData *restrict const key,
                             const InputActionT keyNum) restrict2
{
    // no validation to keyNum because it validated in caller

    if (checkKey(key))
    {
        InputEvent evt(keyNum, mMask);
        ActionFuncPtr func = *(inputActionData[
            CAST_SIZE(keyNum)].action);
        if ((func != nullptr) && func(evt))
            return true;
    }
    return false;
}

void InputManager::executeAction(const InputActionT keyNum) restrict2
{
    if (keyNum < InputAction::MOVE_UP || keyNum >= InputAction::TOTAL)
        return;

    ActionFuncPtr func = *(inputActionData[CAST_SIZE(
        keyNum)].action);
    if (func != nullptr)
    {
        InputEvent evt(keyNum, mMask);
        func(evt);
    }
}

bool InputManager::executeChatCommand(const std::string &restrict cmd,
                                      const std::string &restrict args,
                                      ChatTab *restrict const tab) restrict2
{
    const StringIntMapCIter it = mChatMap.find(cmd);
    if (it != mChatMap.end())
    {
        ActionFuncPtr func = *(inputActionData[(*it).second].action);
        if (func != nullptr)
        {
            InputEvent evt(args, tab, mMask);
            func(evt);
            return true;
        }
    }
    else
    {
        reportAlways("Unknown chat command: /%s %s",
            cmd.c_str(),
            args.c_str())
    }
    return false;
}

bool InputManager::executeRemoteChatCommand(const std::string &restrict cmd,
                                            const std::string &restrict args,
                                            ChatTab *restrict const tab)
                                            restrict2
{
    const StringIntMapCIter it = mChatMap.find(cmd);
    if (it != mChatMap.end())
    {
        const InputActionData &restrict data = inputActionData[(*it).second];
        if (data.isProtected == Protected_true)
            return false;
        ActionFuncPtr func = *(data.action);
        if (func != nullptr)
        {
            InputEvent evt(args, tab, mMask);
            func(evt);
            return true;
        }
    }
    return false;
}

bool InputManager::executeChatCommand(const InputActionT keyNum,
                                      const std::string &restrict args,
                                      ChatTab *restrict const tab) restrict2
{
    if (CAST_S32(keyNum) < 0 || keyNum >= InputAction::TOTAL)
        return false;
    ActionFuncPtr func = *(inputActionData[CAST_SIZE(
        keyNum)].action);
    if (func != nullptr)
    {
        InputEvent evt(args, tab, mMask);
        func(evt);
        return true;
    }
    return false;
}

void InputManager::updateKeyActionMap(KeyToActionMap &restrict actionMap,
                                      KeyToIdMap &restrict idMap,
                                      KeyTimeMap &restrict keyTimeMap,
                                      const InputTypeT type) const restrict2
{
    actionMap.clear();
    keyTimeMap.clear();

    for (size_t i = 0; i < CAST_SIZE(InputAction::TOTAL); i ++)
    {
        const InputFunction &restrict key = mKey[i];
        const InputActionData &restrict kd = inputActionData[i];
        if (kd.action != nullptr)
        {
            for (size_t i2 = 0; i2 < inputFunctionSize; i2 ++)
            {
                const InputItem &restrict ki = key.values[i2];
                if (ki.type == type && ki.value != -1)
                {
                    actionMap[ki.value].push_back(
                        static_cast<InputActionT>(i));
                }
            }
        }
        if (kd.configField != nullptr && (kd.grp & Input::GRP_GUICHAN) != 0)
        {
            for (size_t i2 = 0; i2 < inputFunctionSize; i2 ++)
            {
                const InputItem &restrict ki = key.values[i2];
                if (ki.type == type && ki.value != -1)
                    idMap[ki.value] = static_cast<InputActionT>(i);
            }
        }
        if (kd.configField != nullptr && (kd.grp & Input::GRP_REPEAT) != 0)
        {
            for (size_t i2 = 0; i2 < inputFunctionSize; i2 ++)
            {
                const InputItem &restrict ki = key.values[i2];
                if (ki.type == type && ki.value != -1)
                    keyTimeMap[ki.value] = 0;
            }
        }
    }

    inputActionDataSorter.keys = &inputActionData[0];
    FOR_EACH (KeyToActionMapIter, it, actionMap)
    {
        KeysVector *const keys = &it->second;
        if (keys->size() > 1)
            std::sort(keys->begin(), keys->end(), inputActionDataSorter);
    }
}

bool InputManager::triggerAction(const KeysVector *restrict const ptrs)
                                 restrict2
{
    if (ptrs == nullptr)
        return false;

//    logger->log("ptrs: %d", (int)ptrs.size());

    FOR_EACHP (KeysVectorCIter, it, ptrs)
    {
        const InputActionT keyNum = *it;
        if (CAST_S32(keyNum) < 0 || keyNum >= InputAction::TOTAL)
            continue;

        if (invokeKey(&inputActionData[CAST_SIZE(keyNum)], keyNum))
            return true;
    }
    return false;
}

InputActionT InputManager::getKeyIndex(const int value,
                                       const int grp,
                                       const InputTypeT type) const restrict2
{
    for (size_t i = 0; i < CAST_SIZE(InputAction::TOTAL); i++)
    {
        const InputFunction &restrict key = mKey[i];
        const InputActionData &restrict kd = inputActionData[i];
        for (size_t i2 = 0; i2 < inputFunctionSize; i2 ++)
        {
            const InputItem &restrict vali2 = key.values[i2];
            if (value == vali2.value &&
                (grp & kd.grp) != 0 && vali2.type == type)
            {
                return static_cast<InputActionT>(i);
            }
        }
    }
    return InputAction::NO_VALUE;
}

InputActionT InputManager::getActionByKey(const SDL_Event &restrict event)
                                          const restrict2
{
    // for now support only keyboard events
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        const InputActionT idx = keyboard.getActionId(event);
        if (CAST_S32(idx) >= 0 &&
            checkKey(&inputActionData[CAST_SIZE(idx)]))
        {
            return idx;
        }
    }
    return InputAction::NO_VALUE;
}

InputActionT InputManager::getActionByConfigField(const std::string &field)
{
    for (int i = 0; i < CAST_S32(InputAction::TOTAL); i ++)
    {
        const std::string cf = inputActionData[i].configField;
        if (field == cf)
            return static_cast<InputActionT>(i);
    }
    return InputAction::NO_VALUE;
}

void InputManager::addChatCommands(std::list<std::string> &restrict arr)
{
    const int sz = CAST_S32(InputAction::TOTAL);
    for (int i = 0; i < sz; i++)
    {
        const InputActionData &restrict ad = inputActionData[i];
        std::string cmd = ad.chatCommand;
        if (!cmd.empty())
        {
            StringVect tokens;
            splitToStringVector(tokens, cmd, '|');
            FOR_EACH (StringVectCIter, it, tokens)
            {
                cmd = std::string("/").append(*it);
                if (ad.useArgs == UseArgs_true)
                    cmd.append(" ");
                arr.push_back(cmd);
            }
        }
    }
}
