/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef ENUMS_NET_PACKETTYPE_H
#define ENUMS_NET_PACKETTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(PacketType)
{
    PACKET_CHAT       = 0,
    PACKET_PICKUP     = 1,
    PACKET_DROP       = 2,
    PACKET_NPC_NEXT   = 3,
    PACKET_NPC_TALK   = 4,
    PACKET_NPC_INPUT  = 5,
    PACKET_EMOTE      = 6,
    PACKET_SIT        = 7,
    PACKET_DIRECTION  = 8,
    PACKET_ATTACK     = 9,
    PACKET_STOPATTACK = 10,
    PACKET_ONLINELIST = 11,
    PACKET_WHISPER    = 12,
    PACKET_SIZE
}
enumEnd(PacketType);

#endif  // ENUMS_NET_PACKETTYPE_H
