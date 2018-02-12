/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "net/eathena/clanrecv.h"

#include "configuration.h"

#include "being/localclan.h"

#include "gui/widgets/tabs/chat/clantab.h"

#include "gui/windows/chatwindow.h"

#include "net/messagein.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "resources/claninfo.h"

#include "resources/db/clandb.h"

#include "debug.h"

namespace EAthena
{

void ClanRecv::processClanInfo(Net::MessageIn &msg)
{
    msg.readInt16("len");
    localClan.id = msg.readInt32("clan id");
    localClan.name = msg.readString(24, "clan name");
    localClan.masterName = msg.readString(24, "master name");
    localClan.mapName = msg.readString(16, "map name");
    const int allyCount = msg.readUInt8("ally clans count");
    const int antagonistCount = msg.readUInt8("antagonist clans count");
    for (int f = 0; f < allyCount; f ++)
    {
        localClan.allyClans.push_back(
            msg.readString(24, "ally clan name"));
    }
    for (int f = 0; f < antagonistCount; f ++)
    {
        localClan.antagonistClans.push_back(
            msg.readString(24, "antagonist clan name"));
    }
    const ClanInfo *const info = ClanDb::get(localClan.id);
    if (info == nullptr)
    {
        reportAlways("missing clan %d in clandb",
            localClan.id);
    }
    else
    {
        localClan.stats = info->stats;
    }
    createTab();
}

void ClanRecv::processClanOnlineCount(Net::MessageIn &msg)
{
    localClan.onlineMembers = msg.readInt16("online members count");
    localClan.totalMembers = msg.readInt16("total members count");
}

void ClanRecv::processClanLeave(Net::MessageIn &msg A_UNUSED)
{
    delete2(clanTab);
}

void ClanRecv::processClanChat(Net::MessageIn &msg)
{
    const int chatMsgLength = msg.readInt16("len") - 4 - 24;
    if (chatMsgLength <= 0)
        return;
    msg.readString(24, "player name (unused)");
    std::string chatMsg = msg.readString(chatMsgLength, "message");
    if (clanTab == nullptr)
    {
        reportAlways("received clan chat messages without clan.");
        return;
    }
    const size_t pos = chatMsg.find(" : ", 0);
    if (pos != std::string::npos)
    {
        const std::string sender_name = chatMsg.substr(0, pos);
        chatMsg.erase(0, pos + 3);
        trim(chatMsg);
        clanTab->chatLog(sender_name, chatMsg);
    }
    else
    {
        clanTab->chatLog(chatMsg,
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }
}

void ClanRecv::createTab()
{
    if (clanTab != nullptr)
        return;
    clanTab = new ClanTab(chatWindow);
    if (config.getBoolValue("showChatHistory"))
        clanTab->loadFromLogFile("#Clan");
}

}  // namespace EAthena
