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

#ifndef KEYBOARDCONFIG_H
#define KEYBOARDCONFIG_H

#include <SDL_types.h>
#include <SDL_keyboard.h>

#include "inputevent.h"
#include "keydata.h"

#include <map>
#include <string>
#include <vector>

union SDL_Event;

class KeyboardConfig
{
    public:
        KeyboardConfig();

        /**
         * Initializes the keyboard config explicitly.
         */
        void init();

        /**
         * Get the enable flag, which will stop the user from doing actions.
         */
        bool isEnabled() const
        { return mEnabled; }

        /**
         * Get the key function index by providing the keys value.
         */
        int getKeyIndex(const SDL_Event &event, int grp = 1) const;

        /**
         * Set the enable flag, which will stop the user from doing actions.
         */
        void setEnabled(bool flag)
        { mEnabled = flag; }

        /**
         * Takes a snapshot of all the active keys.
         */
        void refreshActiveKeys();

        std::string getKeyShortString(const std::string &key) const;

        SDLKey getKeyFromEvent(const SDL_Event &event) const;

        int getKeyValueFromEvent(const SDL_Event &event) const;

        KeysVector *getActionVector(const SDL_Event &event);

        std::string getKeyName(int key);

        bool isActionActive(int index) const;

        void update();

        void handleActicateKey(const SDL_Event &event);

        void handleDeActicateKey(const SDL_Event &event);

        int getActionId(const SDL_Event &event);

    private:
        bool mEnabled;                 /**< Flag to respond to key input */

        Uint8 *mActiveKeys;            /**< Stores a list of all the keys */

        Uint8 *mActiveKeys2;           /**< Stores a list of all the keys */

        KeyToActionMap mKeyToAction;

        KeyToIdMap mKeyToId;
};

extern KeyboardConfig keyboard;

#endif
