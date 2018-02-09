/*
 *  The ManaPlus Client
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

#ifndef ENUMS_GUI_CHATTABTYPE_H
#define ENUMS_GUI_CHATTABTYPE_H

#include "enums/simpletypes/enumdefines.h"

#ifdef INPUT
#undef INPUT
#endif  // INPUT

PRAGMAMINGW(GCC diagnostic push)
PRAGMAMINGW(GCC diagnostic ignored "-Wshadow")

enumStart(ChatTabType)
{
    UNKNOWN = 0,
    INPUT,
    WHISPER,
    PARTY,
    GUILD,
    DEBUG,
    TRADE,
    BATTLE,
    LANG,
    GM,
    CHANNEL,
    CLAN
}
enumEnd(ChatTabType);

PRAGMAMINGW(GCC diagnostic pop)

#endif  // ENUMS_GUI_CHATTABTYPE_H
