/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "debug.h"



namespace TmwAthena
{

SkillHandler::SkillHandler() :
    Ea::SkillHandler()
{
    skillHandler = this;
}

void SkillHandler::useBeing(const int id, const int level,
                            const BeingId beingId) const
{
    createOutPacket(CMSG_SKILL_USE_BEING);
    outMsg.writeInt16(CAST_S16(id), "skill id");
    outMsg.writeInt16(CAST_S16(level), "level");
    outMsg.writeBeingId(beingId, "target id");
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION);
    outMsg.writeInt16(CAST_S16(level), "skill level");
    outMsg.writeInt16(CAST_S16(id), "skill id");
    outMsg.writeInt16(CAST_S16(x), "x");
    outMsg.writeInt16(CAST_S16(y), "y");
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y,
                          const std::string &text) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION_MORE);
    outMsg.writeInt16(CAST_S16(level), "skill level");
    outMsg.writeInt16(CAST_S16(id), "skill id");
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
}

void SkillHandler::getBlacksmithRanks() const
{
}

void SkillHandler::getPkRanks() const
{
}

void SkillHandler::getTaekwonRanks() const
{
}

void SkillHandler::feelSaveOk(const int which A_UNUSED) const
{
}

void SkillHandler::lessEffects(const bool isLess A_UNUSED) const
{
}

}  // namespace TmwAthena
