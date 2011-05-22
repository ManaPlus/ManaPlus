/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "guild.h"
#include "event.h"
#include "localplayer.h"
#include "log.h"
#include "playerinfo.h"

#include "gui/socialwindow.h"

#include "net/tmwa/messagein.h"
#include "net/tmwa/protocol.h"

#include "net/tmwa/gui/guildtab.h"

#include "utils/gettext.h"

extern Net::GuildHandler *guildHandler;

namespace TmwAthena
{

GuildTab *guildTab = 0;
Guild *taGuild;
bool showBasicInfo(false);

GuildHandler::GuildHandler()
{
    static const Uint16 _messages[] =
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
    delete guildTab;
    guildTab = 0;
}

void GuildHandler::handleMessage(Net::MessageIn &msg)
{
    DEBUGLOG("guild message");

    switch (msg.getId())
    {
        case SMSG_GUILD_CREATE_RESPONSE:
        {
            int flag = msg.readInt8();

            if (flag == 0)
            {
                // Success
                SERVER_NOTICE(_("Guild created."))
            }
            else if (flag == 1)
            {
                // Already in a guild
                SERVER_NOTICE(_("You are already in guild."))
            }
            else if (flag == 2)
            {
                // Unable to make (likely name already in use)
                SERVER_NOTICE(_("You are already in guild."))
            }
            else if (flag == 3)
            {
                // Emperium check failed
                SERVER_NOTICE(_("Emperium check failed."))
            }
            else
            {
                // Unknown response
                SERVER_NOTICE(_("Unknown server response."))
            }
        }
        break;

        case SMSG_GUILD_POSITION_INFO:
        {
            int guildId =  msg.readInt32();
            int emblem =  msg.readInt32();
            int posMode =  msg.readInt32();
            msg.readInt32(); // Unused
            msg.readInt8(); // Unused
            std::string guildName = msg.readString(24);

            Guild *g = Guild::getGuild(static_cast<short int>(guildId));
            if (!g)
                break;

            g->setName(guildName);
            g->setEmblemId(emblem);
            if (!taGuild)
                taGuild = g;
            if (!guildTab && chatWindow)
            {
                guildTab = new GuildTab();
                if (player_node)
                    player_node->addGuild(taGuild);
                memberList(guildId);
            }

            if (player_node)
            {
                player_node->setGuild(g);
                player_node->setGuildName(g->getName());
            }

            logger->log("Guild position info: %d %d %d %s\n", guildId,
                        emblem, posMode, guildName.c_str());
        }
        break;

        case SMSG_GUILD_MEMBER_LOGIN:
        {
            int accountId = msg.readInt32(); // Account ID
            int charId = msg.readInt32(); // Char ID
            int online = msg.readInt32(); // Flag
            if (taGuild)
            {
                GuildMember *m = taGuild->getMember(accountId, charId);
                if (m)
                    m->setOnline(online);
            }
            break;
        }

        case SMSG_GUILD_MASTER_OR_MEMBER:
            msg.readInt32(); // Type (0x57 for member, 0xd7 for master)
            break;

        case SMSG_GUILD_BASIC_INFO:
        {
            int guildId = msg.readInt32(); // Guild ID
            int level = msg.readInt32(); // Guild level
            int members = msg.readInt32(); // 'Connect member'
            int maxMembers = msg.readInt32(); // 'Max member'
            int avgLevel = msg.readInt32(); // Average level
            int exp = msg.readInt32(); // Exp
            int nextExp = msg.readInt32(); // Next exp
            msg.skip(16); // unused
            std::string name = msg.readString(24); // Name
            std::string master = msg.readString(24); // Master's name
            std::string castle = msg.readString(20); // Castles
                                    // (ie: "Six Castles" or "None Taken")

            if (guildTab && showBasicInfo)
            {
                showBasicInfo = false;
                guildTab->chatLog(strprintf(
                    _("Guild name: %s"), name.c_str()), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Guild master: %s"), master.c_str()), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Guild level: %d"), level), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Online members: %d"), members), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Max members: %d"), maxMembers), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Average level: %d"), avgLevel), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Guild exp: %d"), exp), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Guild next exp: %d"), nextExp), BY_SERVER);
                guildTab->chatLog(strprintf(
                    _("Guild castle: %s"), castle.c_str()), BY_SERVER);
            }

            Guild *g = Guild::getGuild(static_cast<short int>(guildId));
            if (!g)
                break;
            g->setName(name);
        }
        break;

        case SMSG_GUILD_ALIANCE_INFO:
        {
            int length = msg.readInt16();
            int count = (length - 4) / 32;

            for (int i = 0; i < count; i++)
            {
                msg.readInt32(); // 'Opposition'
                msg.readInt32(); // Other guild ID
                msg.readString(24); // Other guild name
            }
        }
        break;

        case SMSG_GUILD_MEMBER_LIST:
        {
            int length = msg.readInt16();
            int count = (length - 4) / 104;
            if (!taGuild)
            {
                logger->log1("!taGuild");
                break;
            }

            taGuild->clearMembers();

            for (int i = 0; i < count; i++)
            {
                int id = msg.readInt32(); // Account ID
                int charId = msg.readInt32(); // Char ID
                msg.readInt16(); // Hair
                msg.readInt16(); // Hair color
                int gender = msg.readInt16(); // Gender
                int race = msg.readInt16(); // Class
                int level = msg.readInt16(); // Level
                int exp = msg.readInt32(); // Exp
                int online = msg.readInt32(); // Online
                int pos = msg.readInt32(); // Position
                msg.skip(50); // unused
                std::string name = msg.readString(24); // Name

                GuildMember *m = taGuild->addMember(id, charId, name);
                if (m)
                {
                    m->setOnline(online);
                    m->setID(id);
                    m->setCharId(charId);
                    if (!gender)
                        m->setGender(GENDER_FEMALE);
                    else if (gender == 1)
                        m->setGender(GENDER_MALE);
                    else
                        m->setGender(GENDER_UNSPECIFIED);

                    m->setLevel(level);
                    m->setExp(exp);
                    m->setPos(pos);
                    m->setRace(race);
//                    m->setDisplayBold(!pos);
                    if (actorSpriteManager)
                    {
                        Being *being = actorSpriteManager->findBeingByName(
                            name, Being::PLAYER);
                        if (being)
                        {
                            being->setGuildName(taGuild->getName());
                            if (being->getLevel() != level)
                            {
                                being->setLevel(level);
                                being->updateName();
                            }
                        }
                    }
                }
            }
            taGuild->sort();
            if (actorSpriteManager)
            {
                actorSpriteManager->updatePlayerGuild();
                actorSpriteManager->updatePlayerColors();
            }
        }
        break;

        case SMSG_GUILD_POS_NAME_LIST:
        {
            if (!taGuild)
            {
                logger->log1("!taGuild");
                break;
            }

            int length = msg.readInt16();
            int count = (length - 4) / 28;

            for (int i = 0; i < count; i++)
            {
                int id = msg.readInt32(); // ID
                std::string name = msg.readString(24); // Position name
                taGuild->addPos(id, name);
            }
        }
        break;

        case SMSG_GUILD_POS_INFO_LIST:
        {
            int length = msg.readInt16();
            int count = (length - 4) / 16;

            for (int i = 0; i < count; i++)
            {
                msg.readInt32(); // ID
                msg.readInt32(); // Mode
                msg.readInt32(); // Same ID
                msg.readInt32(); // Exp mode
            }
        }
        break;

        case SMSG_GUILD_POSITION_CHANGED:
            msg.readInt16(); // Always 44
            msg.readInt32(); // ID
            msg.readInt32(); // Mode
            msg.readInt32(); // Same ID
            msg.readInt32(); // Exp mode
            msg.readString(24); // Name
            break;

        case SMSG_GUILD_MEMBER_POS_CHANGE:
        {
            msg.readInt16(); // Always 16
            int accountId = msg.readInt32(); // Account ID
            int charId = msg.readInt32(); // Char ID
            int pos = msg.readInt32(); // Position
            if (taGuild)
            {
                GuildMember *m = taGuild->getMember(accountId, charId);
                if (m)
                    m->setPos(pos);
            }
            break;
        }

        case SMSG_GUILD_EMBLEM:
        {
            int length = msg.readInt16();

            msg.readInt32(); // Guild ID
            msg.readInt32(); // Emblem ID
            msg.skip(length - 12); // Emblem data (unknown format)
        }
        break;

        case SMSG_GUILD_SKILL_INFO:
        {
            int length = msg.readInt16();
            int count = (length - 6) / 37;

            msg.readInt16(); // 'Skill point'

            for (int i = 0; i < count; i++)
            {
                msg.readInt16(); // ID
                msg.readInt16(); // 'Info' (unknown atm)
                msg.readInt16(); // unused
                msg.readInt16(); // Level
                msg.readInt16(); // SP
                msg.readInt16(); // 'Range'
                msg.skip(24); // unused
                msg.readInt8(); // Can be increased
            }
        }
        break;

        case SMSG_GUILD_NOTICE:
        {
            std::string msg1 = msg.readString(60); // Mes1
            std::string msg2 = msg.readString(120); // Mes2
            if (guildTab)
            {
                guildTab->chatLog(msg1, BY_SERVER);
                guildTab->chatLog(msg2, BY_SERVER);
            }
            break;
        }

        case SMSG_GUILD_INVITE:
        {
            int guildId = msg.readInt32();
            std::string guildName = msg.readString(24);

            if (socialWindow)
                socialWindow->showGuildInvite(guildName, guildId, "");
            break;
        }

        case SMSG_GUILD_INVITE_ACK:
        {
            int flag = msg.readInt8();
            if (!guildTab)
                break;

            switch (flag)
            {
                case 0:
                    guildTab->chatLog(_("Could not inivte user to guild."),
                                      BY_SERVER);
                    break;

                case 1:
                    guildTab->chatLog(_("User rejected guild invite."),
                                      BY_SERVER);
                    break;

                case 2:
                    guildTab->chatLog(_("User is now part of your guild."),
                                      BY_SERVER);
                    break;

                case 3:
                    guildTab->chatLog(_("Your guild is full."),
                                      BY_SERVER);
                    break;

                default:
                    guildTab->chatLog(_("Unknown guild invite response."),
                                      BY_SERVER);
                    break;
            }
        }
        break;

        case SMSG_GUILD_LEAVE:
        {
            std::string nick = msg.readString(24); // Name
            std::string message = msg.readString(40); // Message

            if (taGuild)
                taGuild->removeMember(nick);

            if (player_node && nick == player_node->getName())
            {
                if (taGuild)
                {
                    taGuild->removeFromMembers();
                    taGuild->clearMembers();
                }
                SERVER_NOTICE(_("You have left the guild."))
                delete guildTab;
                guildTab = 0;

                if (socialWindow && taGuild)
                    socialWindow->removeTab(taGuild);
                if (actorSpriteManager)
                    actorSpriteManager->updatePlayerColors();
            }
            else
            {
                if (guildTab)
                {
                    guildTab->chatLog(strprintf(
                        _("%s has left your guild."),
                        nick.c_str()), BY_SERVER);
                }
                if (actorSpriteManager)
                {
                    Being *b = actorSpriteManager->findBeingByName(
                        nick, Being::PLAYER);

                    if (b)
                        b->clearGuilds();
                    if (taGuild)
                        taGuild->removeMember(nick);
                }
            }
            break;
        }

        case SMSG_GUILD_EXPULSION:
        {
            std::string nick = msg.readString(24); // Name (of expulsed?)
            std::string message = msg.readString(40); // Message
            msg.skip(24); // unused ("dummy")
            if (taGuild)
                taGuild->removeMember(nick);

            if (player_node && nick == player_node->getName())
            {
                if (taGuild)
                {
                    taGuild->removeFromMembers();
                    taGuild->clearMembers();
                }
                SERVER_NOTICE(_("You was kicked from guild."));
                delete guildTab;
                guildTab = 0;

                if (socialWindow && taGuild)
                    socialWindow->removeTab(taGuild);
                if (actorSpriteManager)
                    actorSpriteManager->updatePlayerColors();
            }
            else
            {
                if (guildTab)
                {
                    guildTab->chatLog(strprintf(
                        _("%s has kicked from your guild."),
                        nick.c_str()), BY_SERVER);
                }

                if (actorSpriteManager)
                {
                    Being *b = actorSpriteManager->findBeingByName(
                        nick, Being::PLAYER);

                    if (b)
                        b->clearGuilds();
                    if (taGuild)
                        taGuild->removeMember(nick);
                }
            }
            break;
        }

        case SMSG_GUILD_EXPULSION_LIST:
        {
            int length = msg.readInt16();
            int count = (length - 4) / 88;

            for (int i = 0; i < count; i++)
            {
                msg.readString(24); // Name (of expulsed?)
                msg.readString(24); // 'Acc' (name of expulser?)
                msg.readString(24); // Message
            }
        }
        break;

        case SMSG_GUILD_MESSAGE:
        {
            int msgLength = msg.readInt16() - 4;

            if (msgLength <= 0)
                return;
            if (guildTab)
            {
                std::string chatMsg = msg.readString(msgLength);

                std::string::size_type pos = chatMsg.find(" : ", 0);
                if (pos != std::string::npos)
                {
                    std::string sender_name = ((pos == std::string::npos)
                        ? "" : chatMsg.substr(0, pos));

                        chatMsg.erase(0, pos + 3);

                    trim(chatMsg);
                    guildTab->chatLog(sender_name, chatMsg);
                }
                else
                {
                    guildTab->chatLog(chatMsg);
                }
            }
        }
        break;

        case SMSG_GUILD_SKILL_UP:
            msg.readInt16(); // Skill ID
            msg.readInt16(); // Level
            msg.readInt16(); // SP
            msg.readInt16(); // 'Range'
            msg.readInt8(); // unused? (always 1)
            break;

        case SMSG_GUILD_REQ_ALLIANCE:
            msg.readInt32(); // Account ID
            msg.readString(24); // Name
            break;

        case SMSG_GUILD_REQ_ALLIANCE_ACK:
            msg.readInt32(); // Flag
            break;

        case SMSG_GUILD_DEL_ALLIANCE:
            msg.readInt32(); // Guild ID
            msg.readInt32(); // Flag
            break;

        case SMSG_GUILD_OPPOSITION_ACK:
            msg.readInt8(); // Flag
            break;

        case SMSG_GUILD_BROKEN:
            msg.readInt32(); // Flag
            break;

        default:
            break;
    }
}

void GuildHandler::create(const std::string &name)
{
    MessageOut msg(CMSG_GUILD_CREATE);
    msg.writeInt32(0); // Unused
    msg.writeString(name, 24);
}

void GuildHandler::invite(int guildId _UNUSED_,
                          const std::string &name _UNUSED_)
{
    if (!actorSpriteManager)
        return;

    Being* being = actorSpriteManager->findBeingByName(name, Being::PLAYER);
    if (being)
    {
        MessageOut msg(CMSG_GUILD_INVITE);
        msg.writeInt32(being->getId());
        msg.writeInt32(0); // Unused
        msg.writeInt32(0); // Unused
    }
}

void GuildHandler::invite(int guildId _UNUSED_, Being *being)
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

void GuildHandler::chat(int guildId _UNUSED_, const std::string &text)
{
    if (!player_node)
        return;

    std::string str = player_node->getName() + " : " + text;
    MessageOut msg(CMSG_GUILD_MESSAGE);
    msg.writeInt16(static_cast<Uint16>(str.size() + 4));
    msg.writeString(str, static_cast<int>(str.length()));
}

void GuildHandler::memberList(int guildId _UNUSED_)
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

void GuildHandler::info(int guildId _UNUSED_)
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

void GuildHandler::requestAlliance(int guildId _UNUSED_,
                                   int otherGuildId _UNUSED_)
{
    // TODO
}

void GuildHandler::requestAllianceResponse(int guildId _UNUSED_,
                                           int otherGuildId _UNUSED_,
                                           bool response _UNUSED_)
{
    // TODO
}

void GuildHandler::endAlliance(int guildId _UNUSED_, int otherGuildId _UNUSED_)
{
    // TODO
}

void GuildHandler::changeNotice(int guildId, std::string msg1,
                                std::string msg2)
{
    MessageOut msg(CMSG_GUILD_CHANGE_NOTICE);
    msg.writeInt32(guildId);
    msg.writeString(msg1, 60); // msg1
    msg.writeString(msg2, 120); // msg2
}

bool GuildHandler::isSupported()
{
    return true;
}

} // namespace TmwAthena
