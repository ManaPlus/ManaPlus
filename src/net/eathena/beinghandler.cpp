/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/eathena/beinghandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"
#include "net/eathena/sprite.h"

#include "debug.h"

extern int packetVersion;
extern int serverVersion;

namespace EAthena
{

BeingHandler::BeingHandler(const bool enableSync) :
    Ea::BeingHandler(enableSync)
{
    beingHandler = this;
}

BeingHandler::~BeingHandler()
{
    beingHandler = nullptr;
}

void BeingHandler::requestNameById(const BeingId id) const
{
    createOutPacket(CMSG_NAME_REQUEST);
    if (packetVersion >= 20080827 && packetVersion < 20101124)
    {
        outMsg.writeInt32(0, "unused");
        outMsg.writeInt32(0, "unused");
    }
    outMsg.writeBeingId(id, "being id");
}

void BeingHandler::undress(Being *const being) const
{
    if (being == nullptr)
        return;
    being->unSetSprite(SPRITE_WEAPON);
    being->unSetSprite(SPRITE_HEAD_BOTTOM);
    being->unSetSprite(SPRITE_HEAD_TOP);
    being->unSetSprite(SPRITE_HEAD_MID);
    being->unSetSprite(SPRITE_CLOTHES_COLOR);
    being->unSetSprite(SPRITE_SHIELD);
    being->unSetSprite(SPRITE_FLOOR);
    being->unSetSprite(SPRITE_ROBE);
    being->unSetSprite(SPRITE_EVOL2);
    being->unSetSprite(SPRITE_EVOL3);
    being->unSetSprite(SPRITE_EVOL4);
    being->unSetSprite(SPRITE_EVOL5);
    being->unSetSprite(SPRITE_EVOL6);
    being->unSetSprite(SPRITE_HAIR);
    being->unSetSprite(SPRITE_SHOES);
}

void BeingHandler::requestRanks(const RankT rank) const
{
    if (packetVersion < 20130605)
        return;

    createOutPacket(CMSG_REQUEST_RANKS);
    outMsg.writeInt16(CAST_S16(rank), "type");
}

void BeingHandler::viewPlayerEquipment(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_PLAYER_VIEW_EQUIPMENT);
    outMsg.writeBeingId(being->getId(), "account id");
}

void BeingHandler::requestNameByCharId(const int id) const
{
    createOutPacket(CMSG_SOLVE_CHAR_NAME);
    if (packetVersion >= 20080827 && packetVersion < 20101124)
    {
        outMsg.writeInt32(9, "unused");
        outMsg.writeInt32(9, "unused");
    }
    outMsg.writeInt32(id, "character id");
}

}  // namespace EAthena
