/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "net/tmwa/questhandler.h"

#include "localplayer.h"

#include "gui/skilldialog.h"
#include "gui/questswindow.h"

#include "net/tmwa/protocol.h"

#include "net/ea/eaprotocol.h"

#include "utils/langs.h"

#include "debug.h"

//extern Net::QuestHandler *questHandler;

namespace TmwAthena
{

QuestHandler::QuestHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        CMSG_QUEST_SET_VAR,
        CMSG_QUEST_PLAYER_VARS,
        0
    };
    handledMessages = _messages;
//    questHandler = this;
}

void QuestHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("QuestHandler::handleMessage")
    switch (msg.getId())
    {
        case CMSG_QUEST_SET_VAR:
            processSetQuestVar(msg);
            break;

        case CMSG_QUEST_PLAYER_VARS:
            processPlayerQuests(msg);
            break;

        default:
            break;
    }
    BLOCK_END("QuestHandler::handleMessage")
}

void QuestHandler::processSetQuestVar(Net::MessageIn &msg A_UNUSED)
{
    const int var = msg.readInt16();    // variable
    const int val = msg.readInt32();    // value
    if (questsWindow)
    {
        questsWindow->updateQuest(var, val);
        questsWindow->rebuild(true);
    }
    if (skillDialog)
        skillDialog->updateQuest(var, val);
}

void QuestHandler::processPlayerQuests(Net::MessageIn &msg A_UNUSED)
{
    const int count = (msg.readInt16() - 4) / 6;
    for (int f = 0; f < count; f ++)
    {
        const int var = msg.readInt16();    // variable
        const int val = msg.readInt32();    // value
        if (questsWindow)
            questsWindow->updateQuest(var, val);
        if (skillDialog)
            skillDialog->updateQuest(var, val);
    }
    if (questsWindow)
        questsWindow->rebuild(false);
}

} // namespace TmwAthena
