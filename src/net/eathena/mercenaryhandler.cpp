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

#include "net/eathena/mercenaryhandler.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/being.h"
#include "being/localplayer.h"
#include "being/mercenaryinfo.h"
#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"

#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

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
    if (dstBeing && localPlayer)
    {
        MercenaryInfo *const mercenary = new MercenaryInfo;
        mercenary->id = dstBeing->getId();
        mercenary->name = name;
        mercenary->level = level;
        mercenary->range = range;
        PlayerInfo::setMercenary(mercenary);
        PlayerInfo::setMercenaryBeing(dstBeing);
    }
}

void MercenaryHandler::processMercenarySkills(Net::MessageIn &msg)
{
    if (skillDialog)
        skillDialog->hideSkills(SkillOwner::Mercenary);
    const int count = (msg.readInt16("len") - 4) / 37;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt32("inf"));
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const int up = msg.readUInt8("up flag");
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Mercenary,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog)
        skillDialog->updateModels();
}

void MercenaryHandler::handleMercenaryMessage(const int cmd)
{
    PlayerInfo::setMercenary(nullptr);
    if (skillDialog)
    {
        skillDialog->hideSkills(SkillOwner::Mercenary);
        skillDialog->updateModels();
    }

    switch (cmd)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MERCENARY_EXPIRED);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MERCENARY_KILLED);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::MERCENARY_FIRED);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::MERCENARY_RUN);
            break;
        default:
            NotifyManager::notify(NotifyTypes::MERCENARY_UNKNOWN);
            break;
    }
}

void MercenaryHandler::fire()
{
    createOutPacket(CMSG_MERCENARY_ACTION);
    outMsg.writeInt8(2, "action");  // delete
}

void MercenaryHandler::moveToMaster() const
{
    const int id = PlayerInfo::getMercenaryId();
    if (!id)
        return;
    createOutPacket(CMSG_HOMMERC_MOVE_TO_MASTER);
    outMsg.writeInt32(id, "mercenary id");
}

void MercenaryHandler::move(const int x, const int y) const
{
    const int id = PlayerInfo::getMercenaryId();
    if (!id)
        return;
    createOutPacket(CMSG_HOMMERC_MOVE_TO);
    outMsg.writeInt32(id, "mercenary id");
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        0U, "position");
}

void MercenaryHandler::attack(const int targetId, const bool keep) const
{
    const int id = PlayerInfo::getMercenaryId();
    if (!id)
        return;
    createOutPacket(CMSG_HOMMERC_ATTACK);
    outMsg.writeInt32(id, "mercenary id");
    outMsg.writeInt32(targetId, "target id");
    outMsg.writeInt8(static_cast<int8_t>(keep ? 1 : 0), "keep");
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
    outMsg.writeString(msg, sz, "message");
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
