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

#include "net/eathena/guildhandler.h"

#include "actormanager.h"
#include "configuration.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/guildtab.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::GuildHandler *guildHandler;

namespace EAthena
{

GuildHandler::GuildHandler() :
    Ea::GuildHandler(),
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_GUILD_CREATE_RESPONSE,
        SMSG_GUILD_POSITION_INFO,
        SMSG_GUILD_MEMBER_LOGIN,
        SMSG_GUILD_MASTER_OR_MEMBER,
        SMSG_GUILD_BASIC_INFO,
        SMSG_GUILD_ALIANCE_INFO,
        SMSG_GUILD_MEMBER_LIST,
        SMSG_GUILD_POS_NAME_LIST,
        SMSG_GUILD_POS_INFO_LIST,
        SMSG_GUILD_POSITION_CHANGED,
        SMSG_GUILD_MEMBER_POS_CHANGE,
        SMSG_GUILD_EMBLEM_DATA,
        SMSG_GUILD_SKILL_INFO,
        SMSG_GUILD_NOTICE,
        SMSG_GUILD_INVITE,
        SMSG_GUILD_INVITE_ACK,
        SMSG_GUILD_LEAVE,
        SMSG_GUILD_EXPULSION,
        SMSG_GUILD_EXPULSION_LIST,
        SMSG_GUILD_MESSAGE,
        SMSG_GUILD_SKILL_UP,
        SMSG_GUILD_REQ_ALLIANCE,
        SMSG_GUILD_REQ_ALLIANCE_ACK,
        SMSG_GUILD_DEL_ALLIANCE,
        SMSG_GUILD_OPPOSITION_ACK,
        SMSG_GUILD_BROKEN,
        SMSG_GUILD_UPDATE_COORDS,
        SMSG_GUILD_EMBLEM,
        0
    };
    handledMessages = _messages;

    guildHandler = this;
}

GuildHandler::~GuildHandler()
{
}

void GuildHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_GUILD_CREATE_RESPONSE:
            processGuildCreateResponse(msg);
            break;

        case SMSG_GUILD_POSITION_INFO:
            processGuildPositionInfo(msg);
            break;

        case SMSG_GUILD_MEMBER_LOGIN:
            processGuildMemberLogin(msg);
            break;

        case SMSG_GUILD_MASTER_OR_MEMBER:
            processGuildMasterOrMember(msg);
            break;

        case SMSG_GUILD_BASIC_INFO:
            processGuildBasicInfo(msg);
            break;

        case SMSG_GUILD_ALIANCE_INFO:
            processGuildAlianceInfo(msg);
            break;

        case SMSG_GUILD_MEMBER_LIST:
            processGuildMemberList(msg);
            break;

        case SMSG_GUILD_POS_NAME_LIST:
            processGuildPosNameList(msg);
            break;

        case SMSG_GUILD_POS_INFO_LIST:
            processGuildPosInfoList(msg);
            break;

        case SMSG_GUILD_POSITION_CHANGED:
            processGuildPositionChanged(msg);
            break;

        case SMSG_GUILD_MEMBER_POS_CHANGE:
            processGuildMemberPosChange(msg);
            break;

        case SMSG_GUILD_EMBLEM_DATA:
            processGuildEmblemData(msg);
            break;

        case SMSG_GUILD_SKILL_INFO:
            processGuildSkillInfo(msg);
            break;

        case SMSG_GUILD_NOTICE:
            processGuildNotice(msg);
            break;

        case SMSG_GUILD_INVITE:
            processGuildInvite(msg);
            break;

        case SMSG_GUILD_INVITE_ACK:
            processGuildInviteAck(msg);
            break;

        case SMSG_GUILD_LEAVE:
            processGuildLeave(msg);
            break;

        case SMSG_GUILD_EXPULSION:
            processGuildExpulsion(msg);
            break;

        case SMSG_GUILD_EXPULSION_LIST:
            processGuildExpulsionList(msg);
            break;

        case SMSG_GUILD_MESSAGE:
            processGuildMessage(msg);
            break;

        case SMSG_GUILD_SKILL_UP:
            processGuildSkillUp(msg);
            break;

        case SMSG_GUILD_REQ_ALLIANCE:
            processGuildReqAlliance(msg);
            break;

        case SMSG_GUILD_REQ_ALLIANCE_ACK:
            processGuildReqAllianceAck(msg);
            break;

        case SMSG_GUILD_DEL_ALLIANCE:
            processGuildDelAlliance(msg);
            break;

        case SMSG_GUILD_OPPOSITION_ACK:
            processGuildOppositionAck(msg);
            break;

        case SMSG_GUILD_BROKEN:
            processGuildBroken(msg);
            break;

        case SMSG_GUILD_UPDATE_COORDS:
            processGuildUpdateCoords(msg);
            break;

        case SMSG_GUILD_EMBLEM:
            processGuildEmblem(msg);
            break;

        default:
            break;
    }
}

void GuildHandler::processGuildUpdateCoords(Net::MessageIn &msg)
{
    const int id = msg.readInt32("account id");
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
        outMsg.writeInt32(being->getId(), "account id");
        outMsg.writeInt32(0, "unused");
        outMsg.writeInt32(0, "unused");
    }
}

void GuildHandler::invite(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_GUILD_INVITE);
    outMsg.writeInt32(being->getId(), "account id");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
}

void GuildHandler::inviteResponse(const int guildId,
                                  const bool response) const
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
    outMsg.writeInt32(localPlayer->getId(), "account id");
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
    outMsg.writeInt32(member->getID(), "account id");
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
    outMsg.writeInt16(static_cast<uint16_t>(str.size() + 4 + 1), "len");
    outMsg.writeString(str, static_cast<int>(str.length()), "message");
    outMsg.writeInt8(0, "zero byte");
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

    showBasicInfo = true;
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
    outMsg.writeInt32(member->getID(), "account id");
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

void GuildHandler::processGuildPositionInfo(Net::MessageIn &msg)
{
    const int guildId =  msg.readInt32("guild id");
    const int emblem =  msg.readInt32("elblem id");
    PlayerInfo::setGuildPositionFlags(
        static_cast<GuildPositionFlags::Type>(msg.readInt32("mode")));
    msg.readUInt8("guild master");
    msg.readInt32("unused");
    std::string guildName = msg.readString(24, "guild name");

    Guild *const g = Guild::getGuild(static_cast<int16_t>(guildId));
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

void GuildHandler::processGuildMemberLogin(Net::MessageIn &msg)
{
    const int accountId = msg.readInt32("account id");
    const int charId = msg.readInt32("char id");
    const int online = msg.readInt32("flag");
    const Gender::Type gender = Being::intToGender(static_cast<uint8_t>(
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
                guildTab->showOnline(m->getName(), online);
            if (socialWindow)
                socialWindow->updateGuildCounter();
        }
    }
}

void GuildHandler::processGuildExpulsion(Net::MessageIn &msg)
{
    const std::string nick = msg.readString(24, "name");
    msg.readString(40, "message");

    processGuildExpulsionContinue(nick);
}

void GuildHandler::processGuildExpulsionList(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");
    if (length < 4)
        return;

    const int count = (length - 4) / 64;

    for (int i = 0; i < count; i++)
    {
        msg.readString(24, "name");
        msg.readString(40, "message");
    }
}

void GuildHandler::processGuildEmblem(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("being id");
    msg.readInt32("guild id");
    msg.readInt16("emblem id");
}

void GuildHandler::requestAlliance(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_GUILD_ALLIANCE_REQUEST);
    outMsg.writeInt32(being->getId(), "account id");
    outMsg.writeInt32(0, "inviter account id");
    outMsg.writeInt32(0, "inviter char id");
}

void GuildHandler::requestAllianceResponse(const int beingId,
                                           const bool accept) const
{
    createOutPacket(CMSG_GUILD_ALLIANCE_REPLY);
    outMsg.writeInt32(beingId, "account id");
    outMsg.writeInt32(accept, "accept flag");
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
    if (!being)
        return;

    createOutPacket(CMSG_GUILD_OPPOSITION);
    outMsg.writeInt32(being->getId(), "account id");
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
    const int sz = emblem.size();
    outMsg.writeInt16(sz + 4, "len");
    outMsg.writeString(emblem, sz, "emblem");
}

}  // namespace EAthena
