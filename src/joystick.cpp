/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "joystick.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "debug.h"

int Joystick::joystickCount = 0;
bool Joystick::mEnabled = false;

void Joystick::init()
{
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    joystickCount = SDL_NumJoysticks();
    logger->log("%i joysticks/gamepads found", joystickCount);
    for (int i = 0; i < joystickCount; i++)
        logger->log("- %s", SDL_JoystickName(i));

    mEnabled = config.getBoolValue("joystickEnabled");

    if (Joystick::getNumberOfJoysticks() > 0)
    {
        joystick = new Joystick(config.getIntValue("selectedJoystick"));
        if (mEnabled)
            joystick->open();
    }
}

Joystick::Joystick(int no):
    mDirection(0),
    mJoystick(nullptr),
    mUpTolerance(0),
    mDownTolerance(0),
    mLeftTolerance(0),
    mRightTolerance(0),
    mCalibrating(false),
    mCalibrated(false),
    mButtonsNumber(MAX_BUTTONS),
    mUseInactive(false),
    mHaveHats(false)
{
    if (no >= joystickCount)
        no = joystickCount;

    mNumber = no;

    for (int i = 0; i < MAX_BUTTONS; i++)
        mButtons[i] = false;
}

Joystick::~Joystick()
{
    close();
}

bool Joystick::open()
{
    logger->log("open joystick %d", mNumber);

    mJoystick = SDL_JoystickOpen(mNumber);

    // TODO Bail out!
    if (!mJoystick)
    {
        logger->log("Couldn't open joystick: %s", SDL_GetError());
        return false;
    }

    mButtonsNumber = SDL_JoystickNumButtons(mJoystick);
    logger->log("Joystick: %i ", mNumber);
    logger->log("Axes: %i ", SDL_JoystickNumAxes(mJoystick));
    logger->log("Balls: %i", SDL_JoystickNumBalls(mJoystick));
    logger->log("Hats: %i", SDL_JoystickNumHats(mJoystick));
    logger->log("Buttons: %i", mButtonsNumber);

    mHaveHats = (SDL_JoystickNumHats(mJoystick) > 0);

    if (mButtonsNumber > MAX_BUTTONS)
        mButtonsNumber = MAX_BUTTONS;

    mCalibrated = config.getValueBool("joystick"
        + toString(mNumber) + "calibrated", false);
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
    if (mJoystick)
    {
        SDL_JoystickClose(mJoystick);
        mJoystick = nullptr;
    }
}

void Joystick::setNumber(int n)
{
    if (mJoystick)
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

void Joystick::update()
{
    mDirection = 0;

    // When calibrating, don't bother the outside with our state
    if (mCalibrating)
    {
        doCalibration();
        return;
    };

    if (!mEnabled || !mCalibrated)
        return;

    if (mUseInactive || Client::getInputFocused())
    {
        // X-Axis
        int position = SDL_JoystickGetAxis(mJoystick, 0);
        if (position >= mRightTolerance)
            mDirection |= RIGHT;
        else if (position <= mLeftTolerance)
            mDirection |= LEFT;

        // Y-Axis
        position = SDL_JoystickGetAxis(mJoystick, 1);
        if (position <= mUpTolerance)
            mDirection |= UP;
        else if (position >= mDownTolerance)
            mDirection |= DOWN;

#ifdef DEBUG_JOYSTICK
        if (SDL_JoystickGetAxis(mJoystick, 2))
            logger->log("axis 2 pos: %d", SDL_JoystickGetAxis(mJoystick, 2));
        if (SDL_JoystickGetAxis(mJoystick, 3))
            logger->log("axis 3 pos: %d", SDL_JoystickGetAxis(mJoystick, 3));
        if (SDL_JoystickGetAxis(mJoystick, 4))
            logger->log("axis 4 pos: %d", SDL_JoystickGetAxis(mJoystick, 4));
#endif

        if (!mDirection && mHaveHats)
        {
            // reading only hat 0
            Uint8 hat = SDL_JoystickGetHat(mJoystick, 0);
            if (hat & SDL_HAT_RIGHT)
                mDirection |= RIGHT;
            else if (hat & SDL_HAT_LEFT)
                mDirection |= LEFT;
            if (hat & SDL_HAT_UP)
                mDirection |= UP;
            else if (hat & SDL_HAT_DOWN)
                mDirection |= DOWN;
        }

        // Buttons
        for (int i = 0; i < mButtonsNumber; i++)
        {
            mButtons[i] = (SDL_JoystickGetButton(mJoystick, i) == 1);
#ifdef DEBUG_JOYSTICK
            if (mButtons[i])
                logger->log("button: %d", i);
#endif
        }
    }
    else
    {
        for (int i = 0; i < mButtonsNumber; i++)
            mButtons[i] = false;
    }
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

bool Joystick::buttonPressed(unsigned char no) const
{
    return (mEnabled && no < MAX_BUTTONS) ? mButtons[no] : false;
}

void Joystick::getNames(std::vector <std::string> &names)
{
    names.clear();
    for (int i = 0; i < joystickCount; i++)
        names.push_back(SDL_JoystickName(i));
}
