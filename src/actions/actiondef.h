/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#ifndef ACTIONS_ACTIONDEF_H
#define ACTIONS_ACTIONDEF_H

#include "gamemodifiers.h"
#include "settings.h"

#include "input/inputmanager.h"

#define impHandler(name) bool name(InputEvent &event)

#ifdef EATHENA_SUPPORT
#define impHandlerE(name) bool name(InputEvent &event)
#else  // EATHENA_SUPPORT
#define impHandlerE(name) bool name(InputEvent &event A_UNUSED)
#endif  // EATHENA_SUPPORT

#define impHandler0(name) bool name(InputEvent &event A_UNUSED)
#define impHandlerVoid(name) bool name(InputEvent &event A_UNUSED) \
    { \
        return false; \
    }

#define callYellowBar(name) \
    GameModifiers::name(!inputManager.isActionActive( \
        InputAction::STOP_ATTACK)); \
    return true;

#define callYellowBarCond(name) \
    if (!settings.disableGameModifiers) \
    { \
        GameModifiers::name(!inputManager.isActionActive( \
            InputAction::STOP_ATTACK)); \
        return true; \
    } \
    return false;

#endif  // ACTIONS_ACTIONDEF_H
