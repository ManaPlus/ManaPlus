/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;
extern int packetVersionZero;

namespace EAthena
{

SkillHandler::SkillHandler() :
    Ea::SkillHandler()
{
    skillHandler = this;
}

SkillHandler::~SkillHandler()
{
    skillHandler = nullptr;
}

void SkillHandler::useBeing(const int id, const int level,
                            const BeingId beingId) const
{
    createOutPacket(CMSG_SKILL_USE_BEING);
    outMsg.writeInt16(CAST_S16(level), "skill level");
    outMsg.writeInt16(CAST_S16(id), "skill id");
    outMsg.writeInt32(toInt(beingId, int), "target id");
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION);
    if (packetVersion >= 20080827 && packetVersion < 20101124)
    {
        outMsg.writeInt32(0, "unused");
        outMsg.writeInt32(0, "unused");
        outMsg.writeInt16(CAST_S16(level), "skill level");
        outMsg.writeInt16(0, "unused");
        outMsg.writeInt16(CAST_S16(id), "skill id");
        outMsg.writeInt16(0, "unused");
        outMsg.writeInt16(CAST_S16(x), "x");
        outMsg.writeInt16(0, "unused");
        outMsg.writeInt8(0, "unused");
        outMsg.writeInt16(CAST_S16(y), "y");
    }
    else
    {
        outMsg.writeInt16(CAST_S16(level), "skill level");
        outMsg.writeInt16(CAST_S16(id), "skill id");
        outMsg.writeInt16(CAST_S16(x), "x");
        outMsg.writeInt16(CAST_S16(y), "y");
/*
        // disabled due priority from shuffle packets over normal.
        if (packetVersionZero >= 20180131 ||
            packetVersion >= 20180207)
        {
            outMsg.writeInt8(0, "unknown");
        }
*/
    }
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y,
                          const std::string &text) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION_MORE);
    outMsg.writeInt16(CAST_S16(level), "level");
    outMsg.writeInt16(CAST_S16(id), "id");
    outMsg.writeInt16(CAST_S16(x), "x");
    outMsg.writeInt16(CAST_S16(y), "y");
    outMsg.writeString(text, 80, "text");
}

void SkillHandler::useMap(const int id, const std::string &map) const
{
    createOutPacket(CMSG_SKILL_USE_MAP);
    outMsg.writeInt16(CAST_S16(id), "skill id");
    outMsg.writeString(map, 16, "map name");
}

void SkillHandler::getAlchemistRanks() const
{
    if (packetVersion >= 20041108)
    {
        createOutPacket(CMSG_ALCHEMIST_RANKS);
    }
}

void SkillHandler::getBlacksmithRanks() const
{
    if (packetVersion >= 20041108)
    {
        createOutPacket(CMSG_BLACKSMITH_RANKS);
    }
}

void SkillHandler::getPkRanks() const
{
    if (packetVersion >= 20050530)
    {
        createOutPacket(CMSG_PK_RANKS);
    }
}

void SkillHandler::getTaekwonRanks() const
{
    if (packetVersion >= 20050328)
    {
        createOutPacket(CMSG_TAEKWON_RANKS);
    }
}

void SkillHandler::feelSaveOk(const int which) const
{
    if (packetVersion >= 20050817)
    {
        createOutPacket(CMSG_SKILL_FEEL_SAVE_OK);
        outMsg.writeInt8(CAST_S8(which), "which");
        menu = MenuType::Unknown;
    }
}

void SkillHandler::lessEffects(const bool isLess) const
{
    if (packetVersion >= 20041115)
    {
        createOutPacket(CMSG_PLAYER_LESS_EFFECTS);
        outMsg.writeInt32(isLess ? 1 : 0, "state");
    }
}

}  // namespace EAthena
