/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "net/eathena/protocol.h"

#include "resources/skillconsts.h"

#include "debug.h"

namespace EAthena
{

QuestHandler::QuestHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_QUEST_ADD,
        0
    };
    handledMessages = _messages;
}

void QuestHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("QuestHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_QUEST_ADD:
            processAddQuest(msg);
            break;

        default:
            break;
    }
    BLOCK_END("QuestHandler::handleMessage")
}

void QuestHandler::processAddQuest(Net::MessageIn &msg)
{
    const int var = msg.readInt32("quest id");
    const int val = msg.readUInt8("state");
    msg.readUInt8("time diff");
    msg.readInt32("time");
    const int num = msg.readInt16("objectives count");
    for (int f = 0; f < num; f ++)
    {
        msg.readInt32("monster id");
        msg.readInt16("count");
        msg.readString(24, "monster name");
    }

    if (questsWindow)
    {
        questsWindow->updateQuest(var, val);
        questsWindow->rebuild(true);
    }
    if (skillDialog)
    {
        skillDialog->updateQuest(var, val);
        skillDialog->playUpdateEffect(var + SKILL_VAR_MIN_ID);
    }
}

}  // namespace TmwAthena
