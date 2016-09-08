/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/eathena/guildrecv.h"

#include "configuration.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/guildtab.h"

#include "net/ea/guildrecv.h"

#include "net/messagein.h"

#include "net/eathena/guildhandler.h"

#include "debug.h"

namespace EAthena
{

void GuildRecv::processGuildUpdateCoords(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    if (Ea::taGuild)
    {
        GuildMember *const m = Ea::taGuild->getMember(id);
        if (m)
        {
            m->setX(x);
            m->setY(y);
        }
    }
}

void GuildRecv::processGuildPositionInfo(Net::MessageIn &msg)
{
    const int guildId =  msg.readInt32("guild id");
    const int emblem =  msg.readInt32("elblem id");
    PlayerInfo::setGuildPositionFlags(
        static_cast<GuildPositionFlags::Type>(msg.readInt32("mode")));
    msg.readUInt8("guild master");
    msg.readInt32("unused");
    std::string guildName = msg.readString(24, "guild name");

    Guild *const g = Guild::getGuild(CAST_S16(guildId));
    if (!g)
        return;

    g->setName(guildName);
    g->setEmblemId(emblem);
    if (!Ea::taGuild)
        Ea::taGuild = g;
    if (!guildTab && chatWindow)
    {
        guildTab = new GuildTab(chatWindow);
        if (config.getBoolValue("showChatHistory"))
            guildTab->loadFromLogFile("#Guild");
        if (localPlayer)
            localPlayer->addGuild(Ea::taGuild);
        guildHandler->memberList();
    }

    if (localPlayer)
    {
        localPlayer->setGuild(g);
        localPlayer->setGuildName(g->getName());
    }
}

void GuildRecv::processGuildMemberLogin(Net::MessageIn &msg)
{
    const BeingId accountId = msg.readBeingId("account id");
    const int charId = msg.readInt32("char id");
    const int online = msg.readInt32("flag");
    const GenderT gender = Being::intToGender(CAST_U8(
        msg.readInt16("sex")));
    msg.readInt16("hair");
    msg.readInt16("hair color");
    if (Ea::taGuild)
    {
        GuildMember *const m = Ea::taGuild->getMember(accountId, charId);
        if (m)
        {
            m->setOnline(online);
            if (online)
                m->setGender(gender);
            if (guildTab)
                guildTab->showOnline(m->getName(), fromBool(online, Online));
            if (socialWindow)
                socialWindow->updateGuildCounter();
        }
    }
}

void GuildRecv::processGuildExpulsion(Net::MessageIn &msg)
{
    const std::string nick = msg.readString(24, "name");
    msg.readString(40, "message");

    Ea::GuildRecv::processGuildExpulsionContinue(nick);
}

void GuildRecv::processGuildExpulsionList(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");
    if (length < 4)
        return;

    int count;
    if (msg.getVersion() < 20100803)
    {
        count = (length - 4) / 64;
        for (int i = 0; i < count; i++)
        {
            msg.readString(24, "name");
            msg.readString(40, "message");
        }
    }
    else
    {
        count = (length - 4) / 40;
        for (int i = 0; i < count; i++)
            msg.readString(40, "message");
    }
}

void GuildRecv::processGuildEmblem(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readBeingId("being id");
    msg.readInt32("guild id");
    msg.readInt16("emblem id");
}

void GuildRecv::processOnlineInfo(Net::MessageIn &msg)
{
    // look like unused packet
    UNIMPLIMENTEDPACKET;
    msg.readBeingId("being id");
    msg.readInt32("char id");
    msg.readInt32("online");
}

}  // namespace EAthena
