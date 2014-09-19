/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "notifymanager.h"

#include "gui/chatconsts.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::PetHandler *petHandler;

namespace EAthena
{

PetHandler::PetHandler() :
    MessageHandler(),
    mRandCounter(1000)
{
    static const uint16_t _messages[] =
    {
        SMSG_PET_MESSAGE,
        SMSG_PET_ROULETTE,
        0
    };
    handledMessages = _messages;
    petHandler = this;
}

void PetHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("PetHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_PET_MESSAGE:
            processPetMessage(msg);
            break;

        case SMSG_PET_ROULETTE:
            processPetRoulette(msg);
            break;

        default:
            break;
    }
    BLOCK_END("PetHandler::handleMessage")
}

void PetHandler::move(const Being *const being A_UNUSED,
                      const int petId A_UNUSED,
                      const int x1 A_UNUSED, const int y1 A_UNUSED,
                      const int x2 A_UNUSED, const int y2 A_UNUSED) const
{
}

void PetHandler::spawn(const Being *const being A_UNUSED,
                       const int petId A_UNUSED,
                       const int x A_UNUSED, const int y A_UNUSED) const
{
}

void PetHandler::emote(const uint8_t emoteId, const int petId A_UNUSED)
{
    mRandCounter ++;
    if (mRandCounter > 10000)
        mRandCounter = 1000;

    chatHandler->talk(strprintf("\302\202\302e%dz%d",
        static_cast<int>(emoteId), mRandCounter), GENERAL_CHANNEL);
}

void PetHandler::catchPet(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_PET_CATCH);
    outMsg.writeInt32(being->getId(), "monster id");
}

void PetHandler::requestPetState(const int data) const
{
    createOutPacket(CMSG_PET_REQUEST_STATE);
    outMsg.writeInt32(data, "param");
}

void PetHandler::setName(const std::string &name) const
{
    createOutPacket(CMSG_PET_SET_NAME);
    outMsg.writeString(name, 24, "name");
}

void PetHandler::processPetMessage(Net::MessageIn &msg)
{
    msg.readInt32("pet id");
    msg.readInt32("param");
}

void PetHandler::processPetRoulette(Net::MessageIn &msg)
{
    const uint8_t data = msg.readUInt8("data");
    switch (data)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::PET_CATCH_FAILED);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::PET_CATCH_SUCCESS);
            break;
        default:
            NotifyManager::notify(NotifyTypes::PET_CATCH_UNKNOWN, data);
            break;
    }
}

}  // namespace EAthena
