/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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
#include "configuration.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "net/serverfeatures.h"

#include "net/ea/gui/guildtab.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "resources/notifytypes.h"

#include "utils/delete2.h"

#include "debug.h"

extern Net::GuildHandler *guildHandler;

namespace TmwAthena
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
        SMSG_GUILD_EMBLEM,
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
    BLOCK_START("GuildHandler::handleMessage")
    if (!serverFeatures->haveNativeGuilds())
        return;

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

        case SMSG_GUILD_EMBLEM:
            processGuildEmblem(msg);
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

        default:
            break;
    }
    BLOCK_END("GuildHandler::handleMessage")
}

void GuildHandler::create(const std::string &name) const
{
    MessageOut msg(CMSG_GUILD_CREATE);
    msg.writeInt32(0);  // unused
    msg.writeString(name, 24);
}

void GuildHandler::invite(const int guildId A_UNUSED,
                          const std::string &name) const
{
    if (!actorManager)
        return;

    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being)
    {
        MessageOut msg(CMSG_GUILD_INVITE);
        msg.writeInt32(being->getId());
        msg.writeInt32(0);  // unused
        msg.writeInt32(0);  // unused
    }
}

void GuildHandler::invite(const int guildId A_UNUSED,
                          const Being *const being) const
{
    if (!being)
        return;

    MessageOut msg(CMSG_GUILD_INVITE);
    msg.writeInt32(being->getId());
    msg.writeInt32(0);  // unused
    msg.writeInt32(0);  // unused
}

void GuildHandler::inviteResponse(const int guildId, const bool response) const
{
    MessageOut msg(CMSG_GUILD_INVITE_REPLY);
    msg.writeInt32(guildId);
    msg.writeInt8(response);
    msg.writeInt8(0);   // unused
    msg.writeInt16(0);  // unused
}

void GuildHandler::leave(const int guildId) const
{
    if (!localPlayer)
        return;

    MessageOut msg(CMSG_GUILD_LEAVE);
    msg.writeInt32(guildId);
    msg.writeInt32(localPlayer->getId());     // Account ID
    msg.writeInt32(PlayerInfo::getCharId());  // Char ID
    msg.writeString("", 40);                  // Message
}

void GuildHandler::kick(const GuildMember *restrict const member,
                        const std::string &restrict reason) const
{
    if (!member || !member->getGuild())
        return;

    MessageOut msg(CMSG_GUILD_EXPULSION);
    msg.writeInt32(member->getGuild()->getId());
    msg.writeInt32(member->getID());      // Account ID
    msg.writeInt32(member->getCharId());  // Char ID
    msg.writeString(reason, 40);          // Message
}

void GuildHandler::chat(const int guildId A_UNUSED,
                        const std::string &text) const
{
    if (!localPlayer)
        return;

    const std::string str = std::string(localPlayer->getName()).append(
        " : ").append(text);
    MessageOut msg(CMSG_GUILD_MESSAGE);
    msg.writeInt16(static_cast<uint16_t>(str.size() + 4));
    msg.writeString(str, static_cast<int>(str.length()));
}

void GuildHandler::memberList(const int guildId A_UNUSED) const
{
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    MessageOut msg(CMSG_GUILD_REQUEST_INFO);
    msg.writeInt32(1);  // Request member list
}

void GuildHandler::info(const int guildId A_UNUSED)
{
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    showBasicInfo = true;
    MessageOut msg(CMSG_GUILD_REQUEST_INFO);
    msg.writeInt32(0);  // Request basic info
}

void GuildHandler::changeMemberPostion(const GuildMember *const member,
                                       const int level) const
{
    if (!member || !member->getGuild())
        return;

    MessageOut msg(CMSG_GUILD_CHANGE_MEMBER_POS);
    msg.writeInt16(16);                   // size less then 16 <= 4 + 12
    msg.writeInt32(member->getID());      // Account ID
    msg.writeInt32(member->getCharId());  // Char ID
    msg.writeInt32(level);                // pos
}

void GuildHandler::changeNotice(const int guildId,
                                const std::string &restrict msg1,
                                const std::string &restrict msg2) const
{
    MessageOut msg(CMSG_GUILD_CHANGE_NOTICE);
    msg.writeInt32(guildId);
    msg.writeString(msg1, 60);   // msg1
    msg.writeString(msg2, 120);  // msg2
}

void GuildHandler::checkMaster() const
{
    MessageOut msg(CMSG_GUILD_CHECK_MASTER);
}

void GuildHandler::processGuildPositionInfo(Net::MessageIn &msg) const
{
    const int guildId =  msg.readInt32("guild id");
    const int emblem =  msg.readInt32("emblem");
    const int posMode =  msg.readInt32("position");
    msg.readInt32("unused");
    msg.readUInt8("usused");
    std::string guildName = msg.readString(24, "guild name");

    PlayerInfo::setGuildPositionFlags(GuildPositionFlags::Invite);

    Guild *const g = Guild::getGuild(static_cast<int16_t>(guildId));
    if (!g)
        return;

    g->setName(guildName);
    g->setEmblemId(emblem);
    if (!Ea::taGuild)
        Ea::taGuild = g;
    if (!Ea::guildTab && chatWindow)
    {
        Ea::guildTab = new Ea::GuildTab(chatWindow);
        if (config.getBoolValue("showChatHistory"))
            Ea::guildTab->loadFromLogFile("#Guild");
        if (localPlayer)
            localPlayer->addGuild(Ea::taGuild);
        memberList(guildId);
    }

    if (localPlayer)
    {
        localPlayer->setGuild(g);
        localPlayer->setGuildName(g->getName());
    }

    logger->log("Guild position info: %d %d %d %s\n", guildId,
                emblem, posMode, guildName.c_str());
}

void GuildHandler::processGuildMemberLogin(Net::MessageIn &msg) const
{
    const int accountId = msg.readInt32("account id");
    const int charId = msg.readInt32("char id");
    const int online = msg.readInt32("flag");
    if (Ea::taGuild)
    {
        GuildMember *const m = Ea::taGuild->getMember(accountId, charId);
        if (m)
        {
            m->setOnline(online);
            if (Ea::guildTab)
                Ea::guildTab->showOnline(m->getName(), online);
            if (socialWindow)
                socialWindow->updateGuildCounter();
        }
    }
}

void GuildHandler::processGuildExpulsion(Net::MessageIn &msg) const
{
    msg.skip(2, "len?");
    const std::string nick = msg.readString(24, "name?");
    msg.skip(24, "player name");
    msg.readString(44, "message");  // Message

    processGuildExpulsionContinue(nick);
}

void GuildHandler::processGuildExpulsionList(Net::MessageIn &msg) const
{
    const int length = msg.readInt16("len");
    if (length < 4)
        return;

    const int count = (length - 4) / 88;

    for (int i = 0; i < count; i++)
    {
        msg.readString(24, "name of expulsed");
        msg.readString(24, "name of expluser");
        msg.readString(24, "message");
    }
}

}  // namespace TmwAthena
