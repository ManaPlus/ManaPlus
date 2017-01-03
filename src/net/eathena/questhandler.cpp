/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "net/eathena/questhandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

namespace EAthena
{

QuestHandler::QuestHandler() :
    Net::QuestHandler()
{
    questHandler = this;
}

void QuestHandler::setQeustActiveState(const int questId,
                                       const bool active) const
{
    createOutPacket(CMSG_QUEST_ACTIVATE);
    outMsg.writeInt32(questId, "quest id");
    outMsg.writeInt8(CAST_S8(active ? 1 : 0), "activate");
}

}  // namespace EAthena
