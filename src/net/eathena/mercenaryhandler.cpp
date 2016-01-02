/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/eathena/mercenaryhandler.h"

#include "being/playerinfo.h"

#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern Net::MercenaryHandler *mercenaryHandler;

namespace EAthena
{

MercenaryHandler::MercenaryHandler()
{
    mercenaryHandler = this;
}

void MercenaryHandler::fire()
{
    createOutPacket(CMSG_MERCENARY_ACTION);
    outMsg.writeInt8(2, "action");  // delete
}

void MercenaryHandler::moveToMaster() const
{
    const BeingId id = PlayerInfo::getMercenaryId();
    if (id == BeingId_zero)
        return;
    createOutPacket(CMSG_HOMMERC_MOVE_TO_MASTER);
    outMsg.writeBeingId(id, "mercenary id");
}

void MercenaryHandler::move(const int x, const int y) const
{
    const BeingId id = PlayerInfo::getMercenaryId();
    if (id == BeingId_zero)
        return;
    createOutPacket(CMSG_HOMMERC_MOVE_TO);
    outMsg.writeBeingId(id, "mercenary id");
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        0U, "position");
}

void MercenaryHandler::attack(const BeingId targetId,
                              const Keep keep) const
{
    const BeingId id = PlayerInfo::getMercenaryId();
    if (id == BeingId_zero)
        return;
    createOutPacket(CMSG_HOMMERC_ATTACK);
    outMsg.writeBeingId(id, "mercenary id");
    outMsg.writeBeingId(targetId, "target id");
    outMsg.writeInt8(static_cast<int8_t>(keep == Keep_true ? 1 : 0), "keep");
}

void MercenaryHandler::talk(const std::string &restrict text) const
{
    if (!serverFeatures->haveMovePet())
        return;
    if (text.empty())
        return;
    std::string msg = text;
    if (msg.size() > 500)
        msg = msg.substr(0, 500);
    const size_t sz = msg.size();

    createOutPacket(CMSG_HOMMERC_TALK);
    outMsg.writeInt16(static_cast<int16_t>(sz + 4 + 1), "len");
    outMsg.writeString(msg, static_cast<int>(sz), "message");
    outMsg.writeInt8(0, "zero byte");
}

void MercenaryHandler::emote(const uint8_t emoteId) const
{
    if (!serverFeatures->haveMovePet())
        return;
    createOutPacket(CMSG_HOMMERC_EMOTE);
    outMsg.writeInt8(emoteId, "emote id");
}

void MercenaryHandler::setDirection(const unsigned char type) const
{
    if (!serverFeatures->haveMovePet())
        return;
    createOutPacket(CMSG_HOMMERC_DIRECTION);
    outMsg.writeInt32(0, "pet id");
    outMsg.writeInt8(0, "head direction");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(MessageOut::toServerDirection(type),
        "pet direction");
}

}  // namespace EAthena
