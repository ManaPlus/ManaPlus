/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/tmwa/guildhandler.h"

#include "actormanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "net/serverfeatures.h"

#include "net/ea/guildrecv.h"

#include "net/tmwa/guildrecv.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "debug.h"

extern Net::GuildHandler *guildHandler;

namespace TmwAthena
{

GuildHandler::GuildHandler() :
    Ea::GuildHandler()
{
    guildHandler = this;
}

GuildHandler::~GuildHandler()
{
}

void GuildHandler::create(const std::string &name) const
{
    createOutPacket(CMSG_GUILD_CREATE);
    outMsg.writeInt32(0, "unused");
    outMsg.writeString(name, 24, "guild name");
}

void GuildHandler::invite(const std::string &name) const
{
    if (!actorManager)
        return;

    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being)
    {
        createOutPacket(CMSG_GUILD_INVITE);
        outMsg.writeBeingId(being->getId(), "account id");
        outMsg.writeInt32(0, "unused");
        outMsg.writeInt32(0, "unused");
    }
}

void GuildHandler::invite(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_GUILD_INVITE);
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
}

void GuildHandler::inviteResponse(const int guildId, const bool response) const
{
    createOutPacket(CMSG_GUILD_INVITE_REPLY);
    outMsg.writeInt32(guildId, "guild id");
    outMsg.writeInt8(response, "response");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt16(0, "unused");
}

void GuildHandler::leave(const int guildId) const
{
    if (!localPlayer)
        return;

    createOutPacket(CMSG_GUILD_LEAVE);
    outMsg.writeInt32(guildId, "guild id");
    outMsg.writeBeingId(localPlayer->getId(), "account id");
    outMsg.writeInt32(PlayerInfo::getCharId(), "char id");
    outMsg.writeString("", 40, "message");
}

void GuildHandler::kick(const GuildMember *restrict const member,
                        const std::string &restrict reason) const
{
    if (!member || !member->getGuild())
        return;

    createOutPacket(CMSG_GUILD_EXPULSION);
    outMsg.writeInt32(member->getGuild()->getId(), "guild id");
    outMsg.writeBeingId(member->getID(), "account id");
    outMsg.writeInt32(member->getCharId(), "char id");
    outMsg.writeString(reason, 40, "message");
}

void GuildHandler::chat(const std::string &text) const
{
    if (!localPlayer)
        return;

    const std::string str = std::string(localPlayer->getName()).append(
        " : ").append(text);
    createOutPacket(CMSG_GUILD_MESSAGE);
    outMsg.writeInt16(static_cast<uint16_t>(str.size() + 4), "len");
    outMsg.writeString(str, static_cast<int>(str.length()), "message");
}

void GuildHandler::memberList() const
{
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    createOutPacket(CMSG_GUILD_REQUEST_INFO);
    outMsg.writeInt32(1, "action");  // Request member list
}

void GuildHandler::info()
{
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    Ea::GuildRecv::showBasicInfo = true;
    createOutPacket(CMSG_GUILD_REQUEST_INFO);
    outMsg.writeInt32(0, "action");  // Request basic info
}

void GuildHandler::changeMemberPostion(const GuildMember *const member,
                                       const int level) const
{
    if (!member || !member->getGuild())
        return;

    createOutPacket(CMSG_GUILD_CHANGE_MEMBER_POS);
    outMsg.writeInt16(16, "len");
    outMsg.writeBeingId(member->getID(), "account id");
    outMsg.writeInt32(member->getCharId(), "char id");
    outMsg.writeInt32(level, "position");
}

void GuildHandler::changeNotice(const int guildId,
                                const std::string &restrict msg1,
                                const std::string &restrict msg2) const
{
    createOutPacket(CMSG_GUILD_CHANGE_NOTICE);
    outMsg.writeInt32(guildId, "guild id");
    outMsg.writeString(msg1, 60, "msg1");
    outMsg.writeString(msg2, 120, "msg2");
}

void GuildHandler::checkMaster() const
{
    createOutPacket(CMSG_GUILD_CHECK_MASTER);
}

void GuildHandler::requestAlliance(const Being *const being A_UNUSED) const
{
}

void GuildHandler::requestAllianceResponse(const int beingId A_UNUSED,
                                           const bool accept A_UNUSED) const
{
}

void GuildHandler::endAlliance(const int guildId A_UNUSED,
                               const int flag A_UNUSED) const
{
}

void GuildHandler::changePostionInfo(const int posId A_UNUSED,
                                     const int mode A_UNUSED,
                                     const int ranking A_UNUSED,
                                     const int payRate A_UNUSED,
                                     const std::string &name A_UNUSED) const
{
}

void GuildHandler::requestOpposition(const Being *const being A_UNUSED) const
{
}

void GuildHandler::breakGuild(const std::string &name A_UNUSED) const
{
}

void GuildHandler::changeEmblem(std::string emblem A_UNUSED) const
{
}

void GuildHandler::requestEmblem(const int guildId A_UNUSED) const
{
}

}  // namespace TmwAthena
