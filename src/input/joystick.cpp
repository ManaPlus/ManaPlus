/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "input/joystick.h"

#include "configuration.h"
#include "logger.h"
#include "settings.h"
#include "sdlshared.h"

#include "input/inputmanager.h"

#include "utils/foreach.h"
#include "utils/timer.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

Joystick *joystick = nullptr;
int Joystick::joystickCount = 0;
bool Joystick::mEnabled = false;

Joystick::Joystick(const int no) :
    mDirection(0),
    mJoystick(nullptr),
    mAxisThreshold(0f),
    mUpTolerance(0),
    mDownTolerance(0),
    mLeftTolerance(0),
    mRightTolerance(0),
    mCalibrating(false),
    mNumber(no >= joystickCount ? joystickCount : no),
    mCalibrated(false),
    mButtonsNumber(MAX_BUTTONS),
    mUseInactive(false),
    mHaveHats(false),
    mKeyToAction(),
    mKeyToId(),
    mKeyTimeMap()
{
    for (int i = 0; i < MAX_BUTTONS; i++)
        mActiveButtons[i] = false;
}

Joystick::~Joystick()
{
    close();
}

void Joystick::init()
{
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    // +++ possible to use SDL_EventState with different joystick features.
    SDL_JoystickEventState(SDL_ENABLE);
    joystickCount = SDL_NumJoysticks();
    logger->log("%i joysticks/gamepads found", joystickCount);
    for (int i = 0; i < joystickCount; i++)
        logger->log("- %s", SDL_JoystickNameForIndex(i));

    mEnabled = config.getBoolValue("joystickEnabled");

    if (joystickCount > 0)
    {
        joystick = new Joystick(config.getIntValue("selectedJoystick"));
        if (mEnabled)
            joystick->open();
    }
}

bool Joystick::open()
{
    if (mNumber >= joystickCount)
        mNumber = joystickCount - 1;
    if (mNumber < 0)
    {
        logger->log1("error: incorrect joystick selection");
        return false;
    }
    logger->log("open joystick %d", mNumber);

    mJoystick = SDL_JoystickOpen(mNumber);

    if (mJoystick == nullptr)
    {
        logger->log("Couldn't open joystick: %s", SDL_GetError());
        return false;
    }

    mButtonsNumber = SDL_JoystickNumButtons(mJoystick);
    logger->log("Joystick: %i ", mNumber);
#ifdef USE_SDL2
    logger->log("Name: %s", SDL_JoystickName(mJoystick));
    SDL_JoystickGUID guid = SDL_JoystickGetGUID(mJoystick);
    std::string guidStr;
    for (int f = 0; f < 16; f ++)
        guidStr.append(strprintf("%02x", CAST_U32(guid.data[f])));
    logger->log("Guid: %s", guidStr.c_str());
#if SDL_VERSION_ATLEAST(2, 0, 6)
    logger->log("Device id: %u:%u.%u",
        CAST_U32(SDL_JoystickGetVendor(mJoystick)),
        CAST_U32(SDL_JoystickGetProduct(mJoystick)),
        CAST_U32(SDL_JoystickGetProductVersion(mJoystick)));

    SDL_JoystickType type = SDL_JoystickGetType(mJoystick);
    std::string typeStr;
    switch (type)
    {
        default:
        case SDL_JOYSTICK_TYPE_UNKNOWN:
            typeStr = "unknown";
            break;
        case SDL_JOYSTICK_TYPE_GAMECONTROLLER:
            typeStr = "game controller";
            break;
        case SDL_JOYSTICK_TYPE_WHEEL:
            typeStr = "wheel";
            break;
        case SDL_JOYSTICK_TYPE_ARCADE_STICK:
            typeStr = "arcade stick";
            break;
        case SDL_JOYSTICK_TYPE_FLIGHT_STICK:
            typeStr = "flight stick";
            break;
        case SDL_JOYSTICK_TYPE_DANCE_PAD:
            typeStr = "dance pad";
            break;
        case SDL_JOYSTICK_TYPE_GUITAR:
            typeStr = "guitar";
            break;
        case SDL_JOYSTICK_TYPE_DRUM_KIT:
            typeStr = "drum kit";
            break;
        case SDL_JOYSTICK_TYPE_ARCADE_PAD:
            typeStr = "arcade pad";
            break;
        case SDL_JOYSTICK_TYPE_THROTTLE:
            typeStr = "throttle";
            break;
    }
    logger->log("Type: " + typeStr);
#endif  // SDL_VERSION_ATLEAST(2, 0, 6)
    // probably need aslo dump SDL_JoystickCurrentPowerLevel
#else  // USE_SDL2

    logger->log("Name: %s", SDL_JoystickName(mNumber));
#endif  // USE_SDL2

    logger->log("Axes: %i ", SDL_JoystickNumAxes(mJoystick));
    logger->log("Balls: %i", SDL_JoystickNumBalls(mJoystick));
    logger->log("Hats: %i", SDL_JoystickNumHats(mJoystick));
    logger->log("Buttons: %i", mButtonsNumber);

    mHaveHats = (SDL_JoystickNumHats(mJoystick) > 0);

    if (mButtonsNumber > MAX_BUTTONS)
        mButtonsNumber = MAX_BUTTONS;

#ifdef __SWITCH__
    config.setValue("joystick" + toString(mNumber) + "calibrated", true);
    config.setValue("leftTolerance" + toString(mNumber), -10000);
    config.setValue("rightTolerance" + toString(mNumber), 10000);
    config.setValue("upTolerance" + toString(mNumber), -10000);
    config.setValue("downTolerance" + toString(mNumber), 10000);
#endif
    mCalibrated = config.getValueBool("joystick"
        + toString(mNumber) + "calibrated", false);

    mAxisThreshold = config.getFloatValue("axisThreshold");
    mUpTolerance = config.getIntValue("upTolerance" + toString(mNumber));
    mDownTolerance = config.getIntValue("downTolerance" + toString(mNumber));
    mLeftTolerance = config.getIntValue("leftTolerance" + toString(mNumber));
    mRightTolerance = config.getIntValue("rightTolerance" + toString(mNumber));
    mUseInactive = config.getBoolValue("useInactiveJoystick");

    return true;
}

void Joystick::close()
{
    logger->log("close joystick %d", mNumber);
    if (mJoystick != nullptr)
    {
        SDL_JoystickClose(mJoystick);
        mJoystick = nullptr;
    }
}

void Joystick::reload()
{
    joystickCount = SDL_NumJoysticks();
    setNumber(mNumber);
}

void Joystick::setNumber(const int n)
{
    if (mJoystick != nullptr)
    {
        SDL_JoystickClose(mJoystick);
        mNumber = n;
        open();
    }
    else
    {
        mNumber = n;
    }
}

void Joystick::logic()
{
    BLOCK_START("Joystick::logic")
    // When calibrating, don't bother the outside with our state
    if (mCalibrating)
    {
        doCalibration();
        BLOCK_END("Joystick::logic")
        return;
    }

    if (!mEnabled || !mCalibrated)
    {
        BLOCK_END("Joystick::logic")
        return;
    }

    mDirection = 0;

    if (mUseInactive ||
        settings.inputFocused != KeyboardFocus::Unfocused)
    {
        // X-Axis
        int position = SDL_JoystickGetAxis(mJoystick, 0);
        if (position >= mRightTolerance * mAxisThreshold)
            mDirection |= RIGHT;
        else if (position <= mLeftTolerance * mAxisThreshold)
            mDirection |= LEFT;

        // Y-Axis
        position = SDL_JoystickGetAxis(mJoystick, 1);
        if (position <= mUpTolerance * mAxisThreshold)
            mDirection |= UP;
        else if (position >= mDownTolerance * mAxisThreshold)
            mDirection |= DOWN;

#ifdef DEBUG_JOYSTICK
        if (SDL_JoystickGetAxis(mJoystick, 2))
            logger->log("axis 2 pos: %d", SDL_JoystickGetAxis(mJoystick, 2));
        if (SDL_JoystickGetAxis(mJoystick, 3))
            logger->log("axis 3 pos: %d", SDL_JoystickGetAxis(mJoystick, 3));
        if (SDL_JoystickGetAxis(mJoystick, 4))
            logger->log("axis 4 pos: %d", SDL_JoystickGetAxis(mJoystick, 4));
#endif  // DEBUG_JOYSTICK

        if ((mDirection == 0U) && mHaveHats)
        {
            // reading only hat 0
            const uint8_t hat = SDL_JoystickGetHat(mJoystick, 0);
            if ((hat & SDL_HAT_RIGHT) != 0)
                mDirection |= RIGHT;
            else if ((hat & SDL_HAT_LEFT) != 0)
                mDirection |= LEFT;
            if ((hat & SDL_HAT_UP) != 0)
                mDirection |= UP;
            else if ((hat & SDL_HAT_DOWN) != 0)
                mDirection |= DOWN;
        }

        // Buttons
        for (int i = 0; i < mButtonsNumber; i++)
        {
            const bool state = (SDL_JoystickGetButton(mJoystick, i) == 1);
            mActiveButtons[i] = state;
            if (!state)
                resetRepeat(i);
#ifdef DEBUG_JOYSTICK
            if (mActiveButtons[i])
                logger->log("button: %d", i);
#endif  // DEBUG_JOYSTICK
        }
    }
    else
    {
        for (int i = 0; i < mButtonsNumber; i++)
            mActiveButtons[i] = false;
    }
    BLOCK_END("Joystick::logic")
}

void Joystick::startCalibration()
{
    mUpTolerance = 0;
    mDownTolerance = 0;
    mLeftTolerance = 0;
    mRightTolerance = 0;
    mCalibrating = true;
}

void Joystick::doCalibration()
{
    // X-Axis
    int position = SDL_JoystickGetAxis(mJoystick, 0);
    if (position > mRightTolerance)
        mRightTolerance = position;
    else if (position < mLeftTolerance)
        mLeftTolerance = position;

    // Y-Axis
    position = SDL_JoystickGetAxis(mJoystick, 1);
    if (position > mDownTolerance)
        mDownTolerance = position;
    else if (position < mUpTolerance)
        mUpTolerance = position;
}

void Joystick::finishCalibration()
{
    mCalibrated = true;
    mCalibrating = false;
    config.setValue("joystick" + toString(mNumber) + "calibrated", true);
    config.setValue("leftTolerance" + toString(mNumber), mLeftTolerance);
    config.setValue("rightTolerance" + toString(mNumber), mRightTolerance);
    config.setValue("upTolerance" + toString(mNumber), mUpTolerance);
    config.setValue("downTolerance" + toString(mNumber), mDownTolerance);
}

bool Joystick::buttonPressed(const unsigned char no) const
{
    return (mEnabled && no < MAX_BUTTONS) ? mActiveButtons[no] : false;
}

void Joystick::getNames(STD_VECTOR <std::string> &names)
{
    names.clear();
    for (int i = 0; i < joystickCount; i++)
        names.push_back(SDL_JoystickNameForIndex(i));
}

void Joystick::update()
{
    inputManager.updateKeyActionMap(mKeyToAction, mKeyToId,
        mKeyTimeMap, InputType::JOYSTICK);
}

KeysVector *Joystick::getActionVector(const SDL_Event &event)
{
    const int i = getButtonFromEvent(event);

    if (i < 0 || i >= mButtonsNumber)
        return nullptr;
//    logger->log("button triggerAction: %d", i);
    if (mKeyToAction.find(i) != mKeyToAction.end())
        return &mKeyToAction[i];
    return nullptr;
}

KeysVector *Joystick::getActionVectorByKey(const int i)
{
    if (i < 0 || i >= mButtonsNumber)
        return nullptr;
//    logger->log("button triggerAction: %d", i);
    if (mKeyToAction.find(i) != mKeyToAction.end())
        return &mKeyToAction[i];
    return nullptr;
}

int Joystick::getButtonFromEvent(const SDL_Event &event) const
{
    if (event.jbutton.which != mNumber)
        return -1;
    return event.jbutton.button;
}

bool Joystick::isActionActive(const InputActionT index) const
{
    if (!validate())
        return false;

    const InputFunction &key = inputManager.getKey(index);
    for (size_t i = 0; i < inputFunctionSize; i ++)
    {
        const InputItem &val = key.values[i];
        if (val.type != InputType::JOYSTICK)
            continue;
        const int value = val.value;
        if (value >= 0 && value < mButtonsNumber)
        {
            if (mActiveButtons[value])
                return true;
        }
    }
    return false;
}

bool Joystick::validate() const
{
    if (mCalibrating || !mEnabled || !mCalibrated)
        return false;

    return mUseInactive ||
        settings.inputFocused != KeyboardFocus::Unfocused;
}

void Joystick::handleRepeat(const int time)
{
    BLOCK_START("Joystick::handleRepeat")
    FOR_EACH (KeyTimeMapIter, it, mKeyTimeMap)
    {
        bool repeat(false);
        const int key = (*it).first;
        if (key >= 0 && key < mButtonsNumber)
        {
            if (mActiveButtons[key])
                repeat = true;
        }
        if (repeat)
        {
            int &keyTime = (*it).second;
            if (time > keyTime && abs(time - keyTime)
                > SDL_DEFAULT_REPEAT_DELAY * 10)
            {
                keyTime = time;
                inputManager.triggerAction(getActionVectorByKey(key));
            }
        }
    }
    BLOCK_END("Joystick::handleRepeat")
}

void Joystick::resetRepeat(const int key)
{
    const KeyTimeMapIter it = mKeyTimeMap.find(key);
    if (it != mKeyTimeMap.end())
        (*it).second = tick_time;
}
