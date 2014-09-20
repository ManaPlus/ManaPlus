/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/skillhandler.h"

#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::SkillHandler *skillHandler;

namespace EAthena
{

SkillHandler::SkillHandler() :
    MessageHandler(),
    Ea::SkillHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_PLAYER_SKILLS,
        SMSG_SKILL_FAILED,
        SMSG_PLAYER_SKILL_UP,
        SMSG_PLAYER_SKILL_COOLDOWN,
        SMSG_PLAYER_SKILL_COOLDOWN_LIST,
        0
    };
    handledMessages = _messages;
    skillHandler = this;
}

void SkillHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_PLAYER_SKILLS:
            processPlayerSkills(msg);
            break;

        case SMSG_PLAYER_SKILL_UP:
            processPlayerSkillUp(msg);
            break;

        case SMSG_SKILL_FAILED:
            processSkillFailed(msg);
            break;

        case SMSG_PLAYER_SKILL_COOLDOWN:
            processSkillCoolDown(msg);
            break;

        case SMSG_PLAYER_SKILL_COOLDOWN_LIST:
            processSkillCoolDownList(msg);
            break;

        default:
            break;
    }
}

void SkillHandler::useBeing(const int id, const int level,
                            const int beingId) const
{
    createOutPacket(CMSG_SKILL_USE_BEING);
    outMsg.writeInt16(static_cast<int16_t>(id));
    outMsg.writeInt16(static_cast<int16_t>(level));
    outMsg.writeInt32(beingId);
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION);
    outMsg.writeInt16(static_cast<int16_t>(level));
    outMsg.writeInt16(static_cast<int16_t>(id));
    outMsg.writeInt16(static_cast<int16_t>(x));
    outMsg.writeInt16(static_cast<int16_t>(y));
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y,
                          const std::string &text) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION_MORE);
    outMsg.writeInt16(static_cast<int16_t>(level), "level");
    outMsg.writeInt16(static_cast<int16_t>(id), "id");
    outMsg.writeInt16(static_cast<int16_t>(x), "x");
    outMsg.writeInt16(static_cast<int16_t>(y), "y");
    outMsg.writeString(text, 80, "text");
}

void SkillHandler::useMap(const int id, const std::string &map) const
{
    createOutPacket(CMSG_SKILL_USE_MAP);
    outMsg.writeInt16(static_cast<int16_t>(id));
    outMsg.writeString(map, 16);
}

void SkillHandler::processPlayerSkills(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const int skillCount = (msg.getLength() - 4) / 37;
    int updateSkill = 0;

    if (skillDialog)
        skillDialog->hideSkills(SkillOwner::Player);
    for (int k = 0; k < skillCount; k++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt32("inf"));
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const int up = msg.readUInt8("up flag");
        const int oldLevel = PlayerInfo::getSkillLevel(skillId);
        if (oldLevel && oldLevel != level)
            updateSkill = skillId;
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Player,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (updateSkill && skillDialog)
        skillDialog->playUpdateEffect(updateSkill);
}

void SkillHandler::processSkillCoolDown(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int duration = msg.readInt32("duration");
    if (skillDialog)
        skillDialog->setSkillDuration(SkillOwner::Player, skillId, duration);
}

void SkillHandler::processSkillCoolDownList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 4) / 10;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        msg.readInt32("total");
        const int duration = msg.readInt32("duration");
        if (skillDialog)
        {
            skillDialog->setSkillDuration(SkillOwner::Player,
                skillId, duration);
        }
    }
}

}  // namespace EAthena
