/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "actorspritemanager.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "net/tmwa/messagein.h"
#include "net/tmwa/protocol.h"

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

void GuildHandler::create(const std::string &name)
{
    MessageOut msg(CMSG_GUILD_CREATE);
    msg.writeInt32(0); // Unused
    msg.writeString(name, 24);
}

void GuildHandler::invite(int guildId A_UNUSED,
                          const std::string &name A_UNUSED)
{
    if (!actorSpriteManager)
        return;

    const Being *const being = actorSpriteManager->findBeingByName(
        name, Being::PLAYER);
    if (being)
    {
        MessageOut msg(CMSG_GUILD_INVITE);
        msg.writeInt32(being->getId());
        msg.writeInt32(0); // Unused
        msg.writeInt32(0); // Unused
    }
}

void GuildHandler::invite(int guildId A_UNUSED, const Being *const being)
{
    if (!being)
        return;

    MessageOut msg(CMSG_GUILD_INVITE);
    msg.writeInt32(being->getId());
    msg.writeInt32(0); // Unused
    msg.writeInt32(0); // Unused
}

void GuildHandler::inviteResponse(int guildId, bool response)
{
    MessageOut msg(CMSG_GUILD_INVITE_REPLY);
    msg.writeInt32(guildId);
    msg.writeInt8(response);
    msg.writeInt8(0); // Unused
    msg.writeInt16(0); // Unused
}

void GuildHandler::leave(int guildId)
{
    if (!player_node)
        return;

    MessageOut msg(CMSG_GUILD_LEAVE);
    msg.writeInt32(guildId);
    msg.writeInt32(player_node->getId()); // Account ID
    msg.writeInt32(PlayerInfo::getCharId()); // Char ID
    msg.writeString("", 40); // Message
}

void GuildHandler::kick(GuildMember *member, std::string reason)
{
    if (!member || !member->getGuild())
        return;

    MessageOut msg(CMSG_GUILD_EXPULSION);
    msg.writeInt32(member->getGuild()->getId());
    msg.writeInt32(member->getID()); // Account ID
    msg.writeInt32(member->getCharId()); // Char ID
    msg.writeString(reason, 40); // Message
}

void GuildHandler::chat(int guildId A_UNUSED, const std::string &text)
{
    if (!player_node)
        return;

    std::string str = player_node->getName() + " : " + text;
    MessageOut msg(CMSG_GUILD_MESSAGE);
    msg.writeInt16(static_cast<uint16_t>(str.size() + 4));
    msg.writeString(str, static_cast<int>(str.length()));
}

void GuildHandler::memberList(int guildId A_UNUSED)
{
    // TODO four types of info requests:
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    MessageOut msg(CMSG_GUILD_REQUEST_INFO);
    msg.writeInt32(1); // Request member list
}

void GuildHandler::info(int guildId A_UNUSED)
{
    // TODO four types of info requests:
    // 0 = basic info + alliance info
    // 1 = position name list + member list
    // 2 = position name list + position info list
    // 3 = skill info
    // 4 = expulsion list

    showBasicInfo = true;
    MessageOut msg(CMSG_GUILD_REQUEST_INFO);
    msg.writeInt32(0); // Request basic info
}

void GuildHandler::changeMemberPostion(GuildMember *member, int level)
{
    if (!member || !member->getGuild())
        return;

    MessageOut msg(CMSG_GUILD_CHANGE_MEMBER_POS);
    msg.writeInt16(16); // size less then 16 <= 4 + 12
    msg.writeInt32(member->getID()); // Account ID
    msg.writeInt32(member->getCharId()); // Char ID
    msg.writeInt32(level); // pos
}

void GuildHandler::changeNotice(int guildId, std::string msg1,
                                std::string msg2)
{
    MessageOut msg(CMSG_GUILD_CHANGE_NOTICE);
    msg.writeInt32(guildId);
    msg.writeString(msg1, 60); // msg1
    msg.writeString(msg2, 120); // msg2
}

} // namespace TmwAthena
