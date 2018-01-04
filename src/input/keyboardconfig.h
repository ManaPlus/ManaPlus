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

#ifndef INPUT_KEYBOARDCONFIG_H
#define INPUT_KEYBOARDCONFIG_H

#include "localconsts.h"

#ifndef USE_SDL2
#include "utils/cast.h"
#endif  // USE_SDL2

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
PRAGMA48(GCC diagnostic pop)

#include "sdlshared.h"

#include "events/inputevent.h"

class KeyboardConfig final
{
    public:
        KeyboardConfig();

        A_DELETE_COPY(KeyboardConfig)

        /**
         * Initializes the keyboard config explicitly.
         */
        void init();

        void deinit();

        /**
         * Get the enable flag, which will stop the user from doing actions.
         */
        bool isEnabled() const noexcept2 A_WARN_UNUSED
        { return mEnabled; }

        /**
         * Get the key function index by providing the keys value.
         */
        static InputActionT getKeyIndex(const SDL_Event &event,
                                        const int grp) A_WARN_UNUSED;

        /**
         * Set the enable flag, which will stop the user from doing actions.
         */
        void setEnabled(const bool flag)
        { mEnabled = flag; }

        /**
         * Takes a snapshot of all the active keys.
         */
        void refreshActiveKeys();

        static std::string getKeyShortString(const std::string &key)
                                             A_WARN_UNUSED;

        constexpr static SDLKey getKeyFromEvent(const SDL_Event &event)
                                                A_WARN_UNUSED
        {
#ifdef USE_SDL2
            return event.key.keysym.scancode;
#else  // USE_SDL2

            return event.key.keysym.sym;
#endif  // USE_SDL2
        }

        constexpr2 static int getKeyValueFromEvent(const SDL_Event &event)
                                                   A_WARN_UNUSED
        {
#ifdef USE_SDL2
            return event.key.keysym.scancode;
#else  // USE_SDL2

            if (event.key.keysym.sym != 0u)
                return CAST_S32(event.key.keysym.sym);
            else if (event.key.keysym.scancode > 1)
                return -event.key.keysym.scancode;
            return 0;
#endif  // USE_SDL2
        }


        KeysVector *getActionVector(const SDL_Event &event) A_WARN_UNUSED;

        KeysVector *getActionVectorByKey(const int i) A_WARN_UNUSED;

        static std::string getKeyName(const int key) A_WARN_UNUSED;

        bool isActionActive(const InputActionT index) const A_WARN_UNUSED;

        void update();

        void handleActivateKey(const SDL_Event &event);

        void handleActivateKey(const int key);

        void handleDeActicateKey(const SDL_Event &event);

        void handleDeActicateKey(const int key);

        InputActionT getActionId(const SDL_Event &event) A_WARN_UNUSED;

        void handleRepeat(const int time);

        void resetRepeat(const int key);

#ifdef USE_SDL2
        bool ignoreKey(const SDL_Event &restrict event) A_WARN_UNUSED;
#endif  // USE_SDL2

    private:
        bool mEnabled;                   /**< Flag to respond to key input */

        const uint8_t *mActiveKeys;      /**< Stores a list of all the keys */

        uint8_t *mActiveKeys2;           /**< Stores a list of all the keys */

        unsigned int mRepeatTime;

        KeyToActionMap mKeyToAction;

        KeyToIdMap mKeyToId;

        KeyTimeMap mKeyTimeMap;

        bool mBlockAltTab;
};

extern KeyboardConfig keyboard;

#endif  // INPUT_KEYBOARDCONFIG_H
