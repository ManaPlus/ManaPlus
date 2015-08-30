/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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

#include "gui/windows/skilldialog.h"
#include "gui/windows/questswindow.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/questrecv.h"

#include "resources/skillconsts.h"

#include "debug.h"

namespace EAthena
{

QuestHandler::QuestHandler() :
    MessageHandler(),
    Net::QuestHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_QUEST_ADD,
        SMSG_QUEST_LIST,
        SMSG_QUEST_LIST_OBJECTIVES,
        SMSG_QUEST_UPDATE_OBJECTIVES,
        SMSG_QUEST_REMOVE,
        SMSG_QUEST_ACTIVATE,
        SMSG_QUEST_NPC_EFFECT,
        0
    };
    handledMessages = _messages;
    questHandler = this;
}

void QuestHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("QuestHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_QUEST_ADD:
            QuestRecv::processAddQuest(msg);
            break;

        case SMSG_QUEST_LIST:
            QuestRecv::processAddQuests(msg);
            break;

        case SMSG_QUEST_LIST_OBJECTIVES:
            QuestRecv::processAddQuestsObjectives(msg);
            break;

        case SMSG_QUEST_UPDATE_OBJECTIVES:
            QuestRecv::processUpdateQuestsObjectives(msg);
            break;

        case SMSG_QUEST_REMOVE:
            QuestRecv::processRemoveQuest(msg);
            break;

        case SMSG_QUEST_ACTIVATE:
            QuestRecv::processActivateQuest(msg);
            break;

        case SMSG_QUEST_NPC_EFFECT:
            QuestRecv::processNpcQuestEffect(msg);
            break;

        default:
            break;
    }
    BLOCK_END("QuestHandler::handleMessage")
}

void QuestHandler::setQeustActiveState(const int questId,
                                       const bool active) const
{
    createOutPacket(CMSG_QUEST_ACTIVATE);
    outMsg.writeInt32(questId, "quest id");
    outMsg.writeInt8(static_cast<int8_t>(active ? 1 : 0), "activate");
}

}  // namespace EAthena
