/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/eathena/guildhandler.h"

#include "actormanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/widgets/tabs/chat/guildtab.h"

#include "net/eathena/guildrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "utils/delete2.h"

#include "debug.h"

GuildTab *guildTab = nullptr;

extern int packetVersion;

namespace EAthena
{

GuildHandler::GuildHandler() :
    Net::GuildHandler()
{
    guildHandler = this;
    GuildRecv::showBasicInfo = false;
}

GuildHandler::~GuildHandler()
{
    delete2(guildTab)
    guildHandler = nullptr;
    taGuild = nullptr;
}

void GuildHandler::clear() const
{
    taGuild = nullptr;
}

ChatTab *GuildHandler::getTab() const
{
    return guildTab;
}

void GuildHandler::create(const std::string &name) const
{
    createOutPacket(CMSG_GUILD_CREATE);
    outMsg.writeInt32(0, "unused");
    outMsg.writeString(name, 24, "guild name");
}

void GuildHandler::invite(const std::string &name) const
{
    if (actorManager == nullptr)
        return;

    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being != nullptr)
    {
        createOutPacket(CMSG_GUILD_INVITE);
        outMsg.writeBeingId(being->getId(), "account id");
        outMsg.writeInt32(0, "unused");
        outMsg.writeInt32(0, "unused");
    }
    else if (packetVersion >= 20120418)
    {
        createOutPacket(CMSG_GUILD_INVITE2);
        outMsg.writeString(name, 24, "name");
    }
}

void GuildHandler::invite(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_GUILD_INVITE);
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
}

void GuildHandler::inviteResponse(const int guildId,
                                  const bool response) const
{
    createOutPacket(CMSG_GUILD_INVITE_REPLY);
    outMsg.writeInt32(guildId, "guild id");
    outMsg.writeInt8(static_cast<int8_t>(response), "response");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt16(0, "unused");
}

void GuildHandler::leave(const int guildId) const
{
    if (localPlayer == nullptr)
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
    if ((member == nullptr) || (member->getGuild() == nullptr))
        return;

    createOutPacket(CMSG_GUILD_EXPULSION);
    outMsg.writeInt32(member->getGuild()->getId(), "guild id");
    outMsg.writeBeingId(member->getID(), "account id");
    outMsg.writeInt32(member->getCharId(), "char id");
    outMsg.writeString(reason, 40, "message");
}

void GuildHandler::chat(const std::string &text) const
{
    if (localPlayer == nullptr)
        return;

    const std::string str = std::string(localPlayer->getName()).append(
        " : ").append(text);
    createOutPacket(CMSG_GUILD_MESSAGE);
    if (packetVersion >= 20151001)
    {
        outMsg.writeInt16(CAST_U16(str.size() + 4), "len");
        outMsg.writeString(str, CAST_S32(str.length()), "message");
    }
    else
    {
        outMsg.writeInt16(CAST_U16(str.size() + 4 + 1), "len");
        outMsg.writeString(str, CAST_S32(str.length()), "message");
        outMsg.writeInt8(0, "zero byte");
    }
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

void GuildHandler::info() const
{
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    GuildRecv::showBasicInfo = true;
    createOutPacket(CMSG_GUILD_REQUEST_INFO);
    outMsg.writeInt32(0, "action");  // Request basic info
}

void GuildHandler::changeMemberPostion(const GuildMember *const member,
                                       const int level) const
{
    if ((member == nullptr) || (member->getGuild() == nullptr))
        return;

    createOutPacket(CMSG_GUILD_CHANGE_MEMBER_POS);
    outMsg.writeInt16(16, "len");
    outMsg.writeBeingId(member->getID(), "account id");
    outMsg.writeInt32(member->getCharId(), "char id");
    outMsg.writeInt32(level, "pos");
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

void GuildHandler::requestAlliance(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_GUILD_ALLIANCE_REQUEST);
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt32(0, "inviter account id");
    outMsg.writeInt32(0, "inviter char id");
}

void GuildHandler::requestAllianceResponse(const int beingId,
                                           const bool accept) const
{
    createOutPacket(CMSG_GUILD_ALLIANCE_REPLY);
    outMsg.writeInt32(beingId, "account id");
    outMsg.writeInt32(static_cast<int32_t>(accept), "accept flag");
}

void GuildHandler::endAlliance(const int guildId,
                               const int flag) const
{
    createOutPacket(CMSG_GUILD_ALLIANCE_DELETE);
    outMsg.writeInt32(guildId, "guild id");
    outMsg.writeInt32(flag, "flag");
}

void GuildHandler::changePostionInfo(const int posId,
                                     const int mode,
                                     const int ranking,
                                     const int payRate,
                                     const std::string &name) const
{
    createOutPacket(CMSG_GUILD_CHANGE_POS_INFO);
    outMsg.writeInt16(44, "len");
    outMsg.writeInt32(posId, "position id");
    outMsg.writeInt32(mode, "mode");
    outMsg.writeInt32(ranking, "ranking");
    outMsg.writeInt32(payRate, "pay rate");
    outMsg.writeString(name, 24, "name");
}

void GuildHandler::requestOpposition(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_GUILD_OPPOSITION);
    outMsg.writeBeingId(being->getId(), "account id");
}

void GuildHandler::breakGuild(const std::string &name) const
{
    createOutPacket(CMSG_GUILD_BREAK);
    outMsg.writeString(name, 40, "name");
}

void GuildHandler::changeEmblem(std::string emblem) const
{
    createOutPacket(CMSG_GUILD_CHANGE_EMBLEM);
    if (emblem.size() > 200)
        emblem = emblem.substr(0, 200);
    const int sz = CAST_S32(emblem.size());
    outMsg.writeInt16(CAST_S16(sz + 4), "len");
    outMsg.writeString(emblem, sz, "emblem");
}

void GuildHandler::requestEmblem(const int guildId) const
{
    createOutPacket(CMSG_GUILD_REQUEST_EMBLEM);
    outMsg.writeInt32(guildId, "guild id");
}

}  // namespace EAthena
