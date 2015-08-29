/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/eathena/familyhandler.h"

#include "being/being.h"

#include "net/eathena/familyrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::FamilyHandler *familyHandler;

namespace EAthena
{

FamilyHandler::FamilyHandler() :
    MessageHandler()
{
    FamilyRecv::mParent1 = 0;
    FamilyRecv::mParent2 = 0;

    static const uint16_t _messages[] =
    {
        SMSG_FAMILY_ASK_FOR_CHILD,
        SMSG_FAMILY_CALL_PARTNER,
        SMSG_FAMILY_DIVORCED,
        SMSG_FAMILY_ASK_FOR_CHILD_REPLY,
        0
    };
    handledMessages = _messages;
    familyHandler = this;
}

void FamilyHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_FAMILY_ASK_FOR_CHILD:
            FamilyRecv::processAskForChild(msg);
            break;

        case SMSG_FAMILY_CALL_PARTNER:
            FamilyRecv::processCallPartner(msg);
            break;

        case SMSG_FAMILY_DIVORCED:
            FamilyRecv::processDivorced(msg);
            break;

        case SMSG_FAMILY_ASK_FOR_CHILD_REPLY:
            FamilyRecv::processAskForChildReply(msg);
            break;

        default:
            break;
    }
}

void FamilyHandler::askForChild(const Being *const being)
{
    if (!being)
        return;

    createOutPacket(CMSG_FAMILY_ASK_FOR_CHILD);
    outMsg.writeBeingId(being->getId(), "account id");
}

void FamilyHandler::askForChildReply(const bool accept)
{
    createOutPacket(CMSG_FAMILY_ASK_FOR_CHILD_REPLY);
    outMsg.writeInt32(FamilyRecv::mParent1, "parent1");
    outMsg.writeInt32(FamilyRecv::mParent2, "parent2");
    outMsg.writeInt32(accept ? 0: 1, "result");
}

}  // namespace EAthena
