/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef ENUMS_STATE_H
#define ENUMS_STATE_H

#include "enums/simpletypes/enumdefines.h"

#ifdef WIN32
#undef ERROR
#endif  // WIN32

enumStart(State)
{
    ERROR = -1,
    START,
    CHOOSE_SERVER,
    CONNECT_SERVER,
    PRE_LOGIN,
    LOGIN,
    LOGIN_ATTEMPT,
    WORLD_SELECT,            // 5
    WORLD_SELECT_ATTEMPT,
    UPDATE,
    LOAD_DATA,
    GET_CHARACTERS,
    CHAR_SELECT,             // 10
    CONNECT_GAME,
    GAME,
    CHANGE_MAP,              // Switch map-server/gameserver
    LOGIN_ERROR,
    ACCOUNTCHANGE_ERROR,     // 15
    REGISTER_PREP,
    REGISTER,
    REGISTER_ATTEMPT,
    CHANGEPASSWORD,
    CHANGEPASSWORD_ATTEMPT,  // 20
    CHANGEPASSWORD_SUCCESS,
    CHANGEEMAIL,
    CHANGEEMAIL_ATTEMPT,
    CHANGEEMAIL_SUCCESS,
    SWITCH_SERVER,
    SWITCH_LOGIN,
    SWITCH_CHARACTER,        // 30
    LOGOUT_ATTEMPT,
    WAIT,
    EXIT,
    FORCE_QUIT,
    AUTORECONNECT_SERVER = 1000
}
enumEnd(State);

#endif  // ENUMS_STATE_H
