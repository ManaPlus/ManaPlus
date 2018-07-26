/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2018  The ManaPlus Developers
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

#include "net/eathena/pethandler.h"

#include "being/playerinfo.h"

#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

PetHandler::PetHandler()
{
    petHandler = this;
}

PetHandler::~PetHandler()
{
    petHandler = nullptr;
}

void PetHandler::move(const int x,
                      const int y) const
{
    if (!serverFeatures->haveMovePet())
        return;
    createOutPacket(CMSG_PET_MOVE_TO);
    outMsg.writeInt32(0, "pet id");
    outMsg.writeInt16(CAST_S16(x), "x");
    outMsg.writeInt16(CAST_S16(y), "y");
}

void PetHandler::emote(const uint8_t emoteId)
{
    createOutPacket(CMSG_PET_EMOTE);
    outMsg.writeInt8(emoteId, "emote id");
}

void PetHandler::catchPet(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_PET_CATCH);
    outMsg.writeBeingId(being->getId(), "monster id");
}

void PetHandler::sendPetMessage(const int data) const
{
    createOutPacket(CMSG_PET_SEND_MESSAGE);
    outMsg.writeInt32(data, "param");
}

void PetHandler::setName(const std::string &name) const
{
    createOutPacket(CMSG_PET_SET_NAME);
    outMsg.writeString(name, 24, "name");
}

void PetHandler::requestStatus() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(0, "action");
}

void PetHandler::feed() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(1, "action");
}

void PetHandler::dropLoot() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(2, "action");  // performance
}

void PetHandler::returnToEgg() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(3, "action");
    PlayerInfo::setPet(nullptr);
}

void PetHandler::unequip() const
{
    createOutPacket(CMSG_PET_MENU_ACTION);
    outMsg.writeInt8(4, "action");
}

void PetHandler::setDirection(const unsigned char type) const
{
    if (!serverFeatures->haveMovePet())
        return;
    createOutPacket(CMSG_PET_DIRECTION);
    outMsg.writeInt32(0, "pet id");
    outMsg.writeInt8(0, "head direction");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(MessageOut::toServerDirection(type),
        "pet direction");
}

void PetHandler::evolution(const Item *const item) const
{
    if (packetVersion < 20140122 ||
        item == nullptr)
    {
        return;
    }
    createOutPacket(CMSG_PET_EVOLUTION);
    outMsg.writeItemId(item->getId(), "egg id");
}

}  // namespace EAthena
