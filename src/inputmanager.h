/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "gui/sdlinput.h"

#include <string>
#include <map>

struct KeyData;
//struct KeyFunction;

enum KeyCondition
{
    COND_DEFAULT = 1,          // default condition
    COND_ENABLED = 2,          // keyboard must be enabled
    COND_NOINPUT = 4,          // input items must be unfocused
    COND_NOAWAY = 8,           // player not in away mode
    COND_NOSETUP = 16,         // setup window is hidde
    COND_VALIDSPEED = 32,      // valid speed
    COND_NOMODAL = 64,         // modal windows inactive
    COND_NONPCINPUT = 128,     // npc input field inactive
    COND_EMODS = 256,          // game modifiers enabled
    COND_NOTARGET = 512,       // no target/untarget keys pressed
    COND_SHORTCUT = 2 + 4 + 16 + 512, // flags for shortcut keys
    COND_GAME = 2 + 4 + 8 + 16 + 64, // main game key
    COND_GAME2 = 2 + 8 + 16 + 64
};

class InputManager
{
    public:
        InputManager();

        bool handleEvent(const SDL_Event &event);

        bool handleKeyEvent(const SDL_Event &event);

        int getInputConditionMask();

        bool checkKey(const KeyData *key, int mask);
};

extern InputManager inputManager;

#endif
