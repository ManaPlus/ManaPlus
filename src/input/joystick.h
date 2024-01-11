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

#ifndef INPUT_JOYSTICK_H
#define INPUT_JOYSTICK_H

#include "events/inputevent.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
PRAGMA48(GCC diagnostic pop)

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
         * Additional "buttons" for hat 0 (d-pad).
         */
        enum
        {
            KEY_UP = MAX_BUTTONS,
            KEY_DOWN,
            KEY_LEFT,
            KEY_RIGHT,
            KEY_LAST = KEY_RIGHT
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

        static const int AXIS_MIN = -32768;
        static const int AXIS_MAX = 32767;

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

        static bool isEnabled() A_WARN_UNUSED
        { return mEnabled; }

        void setNumber(const int n);

        constexpr2 static void setEnabled(const bool enabled) noexcept2
        { mEnabled = enabled; }

        static void getNames(STD_VECTOR <std::string> &names);

        /**
         * Updates the direction and button information.
         */
        void logic();

        bool buttonPressed(const int no) const A_WARN_UNUSED;

        bool isUp() const noexcept2 A_WARN_UNUSED
        { return mEnabled && ((mDirection & UP) != 0); }

        bool isDown() const noexcept2 A_WARN_UNUSED
        { return mEnabled && ((mDirection & DOWN) != 0); }

        bool isLeft() const noexcept2 A_WARN_UNUSED
        { return mEnabled && ((mDirection & LEFT) != 0); }

        bool isRight() const noexcept2 A_WARN_UNUSED
        { return mEnabled && ((mDirection & RIGHT) != 0); }

        int getNumber() const noexcept2 A_WARN_UNUSED
        { return mNumber; }

        void setTolerance(const float tolerance)
        { mTolerance = tolerance; }

        void setUseHatForMovement(const bool b)
        { mUseHatForMovement = b; }

        void setUseInactive(const bool b)
        { mUseInactive = b; }

        void update();

        KeysVector *getActionVector(const SDL_Event &event) A_WARN_UNUSED;

        KeysVector *getActionVectorByKey(const int i) A_WARN_UNUSED;

        InputActionT getActionId(const SDL_Event &event) A_WARN_UNUSED;

        int getButtonFromEvent(const SDL_Event &event) const A_WARN_UNUSED;

        bool isActionActive(const InputActionT index) const A_WARN_UNUSED;

        bool validate() const A_WARN_UNUSED;

        void handleRepeat(const int time);

        void resetRepeat(const int key);

        void reload();

    protected:
        unsigned char mDirection;

        unsigned char mHatPosition;
        bool mActiveButtons[MAX_BUTTONS];

        SDL_Joystick *mJoystick;

        float mTolerance;
        int mNumber;
        int mButtonsNumber;
        bool mUseHatForMovement;
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
};

extern Joystick *joystick;

#endif  // INPUT_JOYSTICK_H
