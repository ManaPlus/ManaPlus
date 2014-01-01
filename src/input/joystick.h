/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef INPUT_JOYSTICK_H
#define INPUT_JOYSTICK_H

#include "input/inputevent.h"

#include <SDL_events.h>

#include <string>
#include <vector>

class Joystick final
{
    public:
        /**
         * Number of buttons we can handle.
         */
        enum
        {
            MAX_BUTTONS = 64
        };

        /**
         * Directions, to be used as bitmask values.
         */
        enum
        {
            UP    = 1,
            DOWN  = 2,
            LEFT  = 4,
            RIGHT = 8
        };

        /**
         * Initializes the joystick subsystem.
         */
        static void init();

        /**
         * Returns the number of available joysticks.
         */
        static int getNumberOfJoysticks() A_WARN_UNUSED
        { return joystickCount; }

        /**
         * Constructor, pass the number of the joystick the new object
         * should access.
         */
        explicit Joystick(const int no);

        A_DELETE_COPY(Joystick)

        ~Joystick();

        bool open();

        void close();

        bool isEnabled() const A_WARN_UNUSED
        { return mEnabled; }

        void setNumber(const int n);

        static void setEnabled(const bool enabled)
        { mEnabled = enabled; }

        static void getNames(std::vector <std::string> &names);

        /**
         * Updates the direction and button information.
         */
        void logic();

        void startCalibration();

        void finishCalibration();

        bool isCalibrating() const A_WARN_UNUSED
        { return mCalibrating; }

        bool buttonPressed(const unsigned char no) const A_WARN_UNUSED;

        bool isUp() const A_WARN_UNUSED
        { return mEnabled && (mDirection & UP); }

        bool isDown() const A_WARN_UNUSED
        { return mEnabled && (mDirection & DOWN); }

        bool isLeft() const A_WARN_UNUSED
        { return mEnabled && (mDirection & LEFT); }

        bool isRight() const A_WARN_UNUSED
        { return mEnabled && (mDirection & RIGHT); }

        int getNumber() const A_WARN_UNUSED
        { return mNumber; }

        void setUseInactive(const bool b)
        { mUseInactive = b; }

        void update();

        KeysVector *getActionVector(const SDL_Event &event) A_WARN_UNUSED;

        KeysVector *getActionVectorByKey(const int i) A_WARN_UNUSED;

        int getButtonFromEvent(const SDL_Event &event) const A_WARN_UNUSED;

        bool isActionActive(const int index) const A_WARN_UNUSED;

        bool validate() const A_WARN_UNUSED;

        void handleRepeat(const int time);

        void resetRepeat(const int key);

        void reload();

    protected:
        unsigned char mDirection;

        bool mActiveButtons[MAX_BUTTONS];

        SDL_Joystick *mJoystick;

        int mUpTolerance;
        int mDownTolerance;
        int mLeftTolerance;
        int mRightTolerance;
        bool mCalibrating;
        int mNumber;
        bool mCalibrated;
        int mButtonsNumber;
        bool mUseInactive;
        bool mHaveHats;

        KeyToActionMap mKeyToAction;

        KeyToIdMap mKeyToId;

        KeyTimeMap mKeyTimeMap;

        /**
         * Is joystick support enabled.
         */
        static bool mEnabled;
        static int joystickCount;

        void doCalibration();
};

extern Joystick *joystick;

#endif  // INPUT_JOYSTICK_H
