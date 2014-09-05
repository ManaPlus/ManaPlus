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

#include "net/tmwa/skillhandler.h"

#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "debug.h"

extern Net::SkillHandler *skillHandler;

namespace TmwAthena
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
        0
    };
    handledMessages = _messages;
    skillHandler = this;
}

void SkillHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("SkillHandler::handleMessage")
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

        default:
            break;
    }
    BLOCK_END("SkillHandler::handleMessage")
}

void SkillHandler::useBeing(const int id, const int level,
                            const int beingId) const
{
    MessageOut outMsg(CMSG_SKILL_USE_BEING);
    outMsg.writeInt16(static_cast<int16_t>(id));
    outMsg.writeInt16(static_cast<int16_t>(level));
    outMsg.writeInt32(beingId);
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y) const
{
    MessageOut outMsg(CMSG_SKILL_USE_POSITION);
    outMsg.writeInt16(static_cast<int16_t>(level));
    outMsg.writeInt16(static_cast<int16_t>(id));
    outMsg.writeInt16(static_cast<int16_t>(x));
    outMsg.writeInt16(static_cast<int16_t>(y));
}

void SkillHandler::useMap(const int id, const std::string &map) const
{
    MessageOut outMsg(CMSG_SKILL_USE_MAP);
    outMsg.writeInt16(static_cast<int16_t>(id));
    outMsg.writeString(map, 16);
}

void SkillHandler::processPlayerSkills(Net::MessageIn &msg)
{
    msg.readInt16();  // length
    const int skillCount = (msg.getLength() - 4) / 37;
    int updateSkill = 0;

    for (int k = 0; k < skillCount; k++)
    {
        const int skillId = msg.readInt16();
        const int inf = msg.readInt16();
        msg.skip(2);  // skill pool flags
        const int level = msg.readInt16();
        msg.readInt16();  // sp
        const int range = msg.readInt16();
        msg.skip(24);  // 0 unused
        const int up = msg.readUInt8();
        const int oldLevel = PlayerInfo::getSkillLevel(skillId);
        if (oldLevel && oldLevel != level)
            updateSkill = skillId;
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf))
                skillDialog->addSkill(skillId, "", level, range, up, inf);
        }
    }
    if (updateSkill && skillDialog)
        skillDialog->playUpdateEffect(updateSkill);
}

}  // namespace TmwAthena
