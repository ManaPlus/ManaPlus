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

#include "net/eathena/clanhandler.h"

#include "being/localplayer.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

ClanHandler::ClanHandler() :
    Net::ClanHandler()
{
    clanHandler = this;
}

ClanHandler::~ClanHandler()
{
    clanHandler = nullptr;
}

void ClanHandler::chat(const std::string &restrict text) const
{
    if (localPlayer == nullptr)
        return;

    if (packetVersion < 20120716)
        return;

    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    createOutPacket(CMSG_CLAN_MESSAGE);
    if (packetVersion >= 20151001)
    {
        outMsg.writeInt16(CAST_S16(mes.length() + 4), "len");
        outMsg.writeString(mes, CAST_S32(mes.length()), "message");
    }
    else
    {
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(CAST_S16(mes.length() + 4 + 1), "len");
        outMsg.writeString(mes, CAST_S32(mes.length() + 1), "message");
    }
}

}  // namespace EAthena
