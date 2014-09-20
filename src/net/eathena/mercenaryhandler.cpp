/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "actormanager.h"
#include "logger.h"

#include "being/being.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::MercenaryHandler *mercenaryHandler;

namespace EAthena
{

MercenaryHandler::MercenaryHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_MERCENARY_UPDATE,
        SMSG_MERCENARY_INFO,
        SMSG_MERCENARY_SKILLS,
        0
    };
    handledMessages = _messages;
    mercenaryHandler = this;
}

void MercenaryHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_MERCENARY_UPDATE:
            processMercenaryUpdate(msg);
            break;

        case SMSG_MERCENARY_INFO:
            processMercenaryInfo(msg);
            break;

        case SMSG_MERCENARY_SKILLS:
            processMercenarySkills(msg);
            break;

        default:
            break;
    }
}

void MercenaryHandler::processMercenaryUpdate(Net::MessageIn &msg)
{
    // +++ need create if need mercenary being and update stats
    msg.readInt16("type");
    msg.readInt32("value");
}

void MercenaryHandler::processMercenaryInfo(Net::MessageIn &msg)
{
    // +++ need create if need mercenary being and update stats
    Being *const dstBeing = actorManager->findBeing(msg.readInt32("being id"));
    msg.readInt16("atk");
    msg.readInt16("matk");
    msg.readInt16("hit");
    msg.readInt16("crit/10");
    msg.readInt16("def");
    msg.readInt16("mdef");
    msg.readInt16("flee");
    msg.readInt16("attack speed");
    const std::string name = msg.readString(24, "name");
    const int level = msg.readInt16("level");
    msg.readInt32("hp");
    msg.readInt32("max hp");
    msg.readInt32("sp");
    msg.readInt32("max sp");
    msg.readInt32("expire time");
    msg.readInt16("faith");
    msg.readInt32("calls");
    msg.readInt32("kills");
    const int range = msg.readInt16("attack range");
    if (dstBeing)
    {
        dstBeing->setName(name);
        dstBeing->setLevel(level);
        dstBeing->setAttackRange(range);
    }
}

void MercenaryHandler::processMercenarySkills(Net::MessageIn &msg)
{
    // +++ need create if need mercenary being and update stats
    const int count = (msg.readInt16("len") - 4) / 37;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("skill id");
        msg.readInt32("inf");
        msg.readInt16("level");
        msg.readInt16("sp");
        msg.readInt16("attack range");
        msg.readString(24, "name");
        msg.readUInt8("upgradable");
    }
}

}  // namespace EAthena
