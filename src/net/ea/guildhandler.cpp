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

#include "net/ea/guildhandler.h"

#include "actormanager.h"
#include "configuration.h"
#include "notifications.h"
#include "notifymanager.h"

#include "being/localplayer.h"

#include "gui/windows/socialwindow.h"

#include "net/messagein.h"

#include "net/ea/gui/guildtab.h"

#include "debug.h"

namespace Ea
{
GuildTab *guildTab = nullptr;
Guild *taGuild = nullptr;

GuildHandler::GuildHandler() :
    showBasicInfo(false)
{
}

GuildHandler::~GuildHandler()
{
    delete guildTab;
    guildTab = nullptr;
}

void GuildHandler::requestAlliance(const int guildId A_UNUSED,
                                   const int otherGuildId A_UNUSED) const
{
}

void GuildHandler::requestAllianceResponse(const int guildId A_UNUSED,
                                           const int otherGuildId A_UNUSED,
                                           const bool response A_UNUSED) const
{
}

void GuildHandler::endAlliance(const int guildId A_UNUSED,
                               const int otherGuildId A_UNUSED) const
{
}

void GuildHandler::processGuildCreateResponse(Net::MessageIn &msg) const
{
    const int flag = msg.readInt8();

    switch (flag)
    {
        case 0:
            // Success
            NotifyManager::notify(NotifyManager::GUILD_CREATED);
            break;

        case 1:
            // Already in a guild
            NotifyManager::notify(NotifyManager::GUILD_ALREADY);
            break;

        case 2:
            // Unable to make (likely name already in use)
            NotifyManager::notify(NotifyManager::GUILD_ALREADY);
            break;

        case 3:
            // Emperium check failed
            NotifyManager::notify(NotifyManager::GUILD_EMPERIUM_CHECK_FAILED);
            break;

        default:
            // Unknown response
            NotifyManager::notify(NotifyManager::GUILD_ERROR);
            break;
    }
}

void GuildHandler::processGuildPositionInfo(Net::MessageIn &msg) const
{
    const int guildId =  msg.readInt32();
    const int emblem =  msg.readInt32();
    const int posMode =  msg.readInt32();
    msg.readInt32();  // Unused
    msg.readInt8();  // Unused
    std::string guildName = msg.readString(24);

    Guild *const g = Guild::getGuild(static_cast<int16_t>(guildId));
    if (!g)
        return;

    g->setName(guildName);
    g->setEmblemId(emblem);
    if (!taGuild)
        taGuild = g;
    if (!guildTab && chatWindow)
    {
        guildTab = new GuildTab(chatWindow);
        if (config.getBoolValue("showChatHistory"))
            guildTab->loadFromLogFile("#Guild");
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

void GuildHandler::processGuildMemberLogin(Net::MessageIn &msg) const
{
    const int accountId = msg.readInt32();  // Account ID
    const int charId = msg.readInt32();     // Char ID
    const int online = msg.readInt32();     // Flag
    if (taGuild)
    {
        GuildMember *const m = taGuild->getMember(accountId, charId);
        if (m)
        {
            m->setOnline(online);
            if (guildTab)
                guildTab->showOnline(m->getName(), online);
            if (socialWindow)
                socialWindow->updateGuildCounter();
        }
    }
}

void GuildHandler::processGuildMasterOrMember(Net::MessageIn &msg) const
{
    msg.readInt32();  // Type (0x57 for member, 0xd7 for master)
}

void GuildHandler::processGuildBasicInfo(Net::MessageIn &msg)
{
    const int guildId = msg.readInt32();      // Guild ID
    const int level = msg.readInt32();        // Guild level
    const int members = msg.readInt32();      // 'Connect member'
    const int maxMembers = msg.readInt32();   // 'Max member'
    const int avgLevel = msg.readInt32();     // Average level
    const int exp = msg.readInt32();          // Exp
    const int nextExp = msg.readInt32();      // Next exp
    msg.skip(16);                             // 0 unused
    std::string name = msg.readString(24);    // Name
    std::string master = msg.readString(24);  // Master's name
    std::string castle = msg.readString(20);  // Castles
                            // (ie: "Six Castles" or "None Taken")

    if (guildTab && showBasicInfo)
    {
        showBasicInfo = false;
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild name: %s"),
            name.c_str()), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild master: %s"),
            master.c_str()), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild level: %d"), level), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Online members: %d"),
            members), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Max members: %d"),
            maxMembers), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Average level: %d"),
            avgLevel), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild exp: %d"), exp), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild next exp: %d"),
            nextExp), BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild castle: %s"),
            castle.c_str()), BY_SERVER);
    }

    Guild *const g = Guild::getGuild(static_cast<int16_t>(guildId));
    if (!g)
        return;
    g->setName(name);
}

void GuildHandler::processGuildAlianceInfo(Net::MessageIn &msg) const
{
    const int length = msg.readInt16();
    if (length < 4)
        return;
    const int count = (length - 4) / 32;

    for (int i = 0; i < count; i++)
    {
        msg.readInt32();     // 'Opposition'
        msg.readInt32();     // Other guild ID
        msg.readString(24);  // Other guild name
    }
}

void GuildHandler::processGuildMemberList(Net::MessageIn &msg) const
{
    const int length = msg.readInt16();
    if (length < 4)
        return;
    const int count = (length - 4) / 104;
    if (!taGuild)
    {
        logger->log1("!taGuild");
        return;
    }

    taGuild->clearMembers();

    int onlineNum = 0;
    int totalNum = 0;
    for (int i = 0; i < count; i++)
    {
        const int id = msg.readInt32();      // Account ID
        const int charId = msg.readInt32();  // Char ID
        msg.readInt16();                     // Hair
        msg.readInt16();                     // Hair color
        const int gender = msg.readInt16();  // Gender
        const int race = msg.readInt16();    // Class
        const int level = msg.readInt16();   // Level
        const int exp = msg.readInt32();     // Exp
        const int online = msg.readInt32();  // Online
        const int pos = msg.readInt32();     // Position
        msg.skip(50);                        // 0 unused
        std::string name = msg.readString(24);  // Name

        GuildMember *const m = taGuild->addMember(id, charId, name);
        if (m)
        {
            m->setOnline(online);
            m->setID(id);
            m->setCharId(charId);
            m->setGender(Being::intToGender(static_cast<uint8_t>(gender)));
            m->setLevel(level);
            m->setExp(exp);
            m->setPos(pos);
            m->setRace(race);
            if (actorManager)
            {
                Being *const being = actorManager->findBeingByName(
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
            if (online)
                onlineNum ++;
            totalNum ++;
        }
    }
    taGuild->sort();
    if (actorManager)
    {
        actorManager->updatePlayerGuild();
        actorManager->updatePlayerColors();
    }
    if (socialWindow)
        socialWindow->updateGuildCounter(onlineNum, totalNum);
}

void GuildHandler::processGuildPosNameList(Net::MessageIn &msg) const
{
    if (!taGuild)
    {
        logger->log1("!taGuild");
        return;
    }

    const int length = msg.readInt16();
    if (length < 4)
        return;
    const int count = (length - 4) / 28;

    for (int i = 0; i < count; i++)
    {
        const int id = msg.readInt32();               // ID
        const std::string name = msg.readString(24);  // Position name
        taGuild->addPos(id, name);
    }
}

void GuildHandler::processGuildPosInfoList(Net::MessageIn &msg) const
{
    const int length = msg.readInt16();
    if (length < 4)
        return;
    const int count = (length - 4) / 16;

    for (int i = 0; i < count; i++)
    {
        msg.readInt32();  // ID
        msg.readInt32();  // Mode
        msg.readInt32();  // Same ID
        msg.readInt32();  // Exp mode
    }
}

void GuildHandler::processGuildPositionChanged(Net::MessageIn &msg) const
{
    msg.readInt16();     // Always 44
    msg.readInt32();     // ID
    msg.readInt32();     // Mode
    msg.readInt32();     // Same ID
    msg.readInt32();     // Exp mode
    msg.readString(24);  // Name
}

void GuildHandler::processGuildMemberPosChange(Net::MessageIn &msg) const
{
    msg.readInt16();                        // Always 16
    const int accountId = msg.readInt32();  // Account ID
    const int charId = msg.readInt32();     // Char ID
    const int pos = msg.readInt32();        // Position
    if (taGuild)
    {
        GuildMember *const m = taGuild->getMember(accountId, charId);
        if (m)
            m->setPos(pos);
    }
}

void GuildHandler::processGuildEmblem(Net::MessageIn &msg) const
{
    const int length = msg.readInt16();

    msg.readInt32();  // Guild ID
    msg.readInt32();  // Emblem ID
    if (length < 12)
        return;
    msg.skip(length - 12);  // Emblem data (unknown format)
}

void GuildHandler::processGuildSkillInfo(Net::MessageIn &msg) const
{
    const int length = msg.readInt16();
    const int count = (length - 6) / 37;

    msg.readInt16();  // 'Skill point'

    if (length < 6)
        return;
    for (int i = 0; i < count; i++)
    {
        msg.readInt16();  // ID
        msg.readInt16();  // 'Info' (unknown atm)
        msg.readInt16();  // 0 unused
        msg.readInt16();  // Level
        msg.readInt16();  // SP
        msg.readInt16();  // 'Range'
        msg.skip(24);     // 0 unused
        msg.readInt8();   // Can be increased
    }
}

void GuildHandler::processGuildNotice(Net::MessageIn &msg) const
{
    const std::string msg1 = msg.readString(60);   // Mes1
    const std::string msg2 = msg.readString(120);  // Mes2
    if (guildTab)
    {
        guildTab->chatLog(msg1, BY_SERVER);
        guildTab->chatLog(msg2, BY_SERVER);
    }
}

void GuildHandler::processGuildInvite(Net::MessageIn &msg) const
{
    const int guildId = msg.readInt32();
    const std::string guildName = msg.readString(24);

    if (socialWindow)
        socialWindow->showGuildInvite(guildName, guildId, "");
}

void GuildHandler::processGuildInviteAck(Net::MessageIn &msg) const
{
    const int flag = msg.readInt8();
    if (!guildTab)
        return;

    switch (flag)
    {
        case 0:
            NotifyManager::notify(NotifyManager::GUILD_INVITE_FAILED);
            break;

        case 1:
            NotifyManager::notify(NotifyManager::GUILD_INVITE_REJECTED);
            break;

        case 2:
            NotifyManager::notify(NotifyManager::GUILD_INVITE_JOINED);
            break;

        case 3:
            NotifyManager::notify(NotifyManager::GUILD_INVITE_FULL);
            break;

        default:
            NotifyManager::notify(NotifyManager::GUILD_INVITE_ERROR);
            break;
    }
}

void GuildHandler::processGuildLeave(Net::MessageIn &msg) const
{
    const std::string nick = msg.readString(24);  // Name
    msg.readString(40);                           // Message

    if (taGuild)
        taGuild->removeMember(nick);

    if (!player_node)
        return;

    if (nick == player_node->getName())
    {
        if (taGuild)
        {
            taGuild->removeFromMembers();
            taGuild->clearMembers();
        }
        NotifyManager::notify(NotifyManager::GUILD_LEFT);
        delete guildTab;
        guildTab = nullptr;

        if (socialWindow && taGuild)
            socialWindow->removeTab(taGuild);
        if (actorManager)
            actorManager->updatePlayerColors();
    }
    else
    {
        NotifyManager::notify(NotifyManager::GUILD_USER_LEFT, nick);
        if (actorManager)
        {
            Being *const b = actorManager->findBeingByName(
                nick, Being::PLAYER);

            if (b)
                b->clearGuilds();
            if (taGuild)
                taGuild->removeMember(nick);
        }
    }
}

void GuildHandler::processGuildExpulsion(Net::MessageIn &msg) const
{
    msg.skip(2);    // size (can be many explusions in list)
    const std::string nick = msg.readString(24);  // Name (of expulsed?)
    msg.skip(24);        // acc
    msg.readString(44);  // Message
    if (taGuild)
        taGuild->removeMember(nick);

    if (!player_node)
        return;

    if (nick == player_node->getName())
    {
        if (taGuild)
        {
            taGuild->removeFromMembers();
            taGuild->clearMembers();
        }
        NotifyManager::notify(NotifyManager::GUILD_KICKED);
        delete guildTab;
        guildTab = nullptr;

        if (socialWindow && taGuild)
            socialWindow->removeTab(taGuild);
        if (actorManager)
            actorManager->updatePlayerColors();
    }
    else
    {
        NotifyManager::notify(NotifyManager::GUILD_USER_KICKED, nick);
        if (actorManager)
        {
            Being *const b = actorManager->findBeingByName(
                nick, Being::PLAYER);

            if (b)
                b->clearGuilds();
            if (taGuild)
                taGuild->removeMember(nick);
        }
    }
}

void GuildHandler::processGuildExpulsionList(Net::MessageIn &msg) const
{
    const int length = msg.readInt16();
    if (length < 4)
        return;

    const int count = (length - 4) / 88;

    for (int i = 0; i < count; i++)
    {
        msg.readString(24);  // Name (of expulsed?)
        msg.readString(24);  // 'Acc' (name of expulser?)
        msg.readString(24);  // Message
    }
}

void GuildHandler::processGuildMessage(Net::MessageIn &msg) const
{
    const int msgLength = msg.readInt16() - 4;

    if (msgLength <= 0)
        return;
    if (guildTab)
    {
        std::string chatMsg = msg.readString(msgLength);
        const size_t pos = chatMsg.find(" : ", 0);
        if (pos != std::string::npos)
        {
            const std::string sender_name = chatMsg.substr(0, pos);
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

void GuildHandler::processGuildSkillUp(Net::MessageIn &msg) const
{
    msg.readInt16();  // Skill ID
    msg.readInt16();  // Level
    msg.readInt16();  // SP
    msg.readInt16();  // 'Range'
    msg.readInt8();   // unused? (always 1)
}

void GuildHandler::processGuildReqAlliance(Net::MessageIn &msg) const
{
    msg.readInt32();     // Account ID
    msg.readString(24);  // Name
}

void GuildHandler::processGuildReqAllianceAck(Net::MessageIn &msg) const
{
    msg.readInt32();  // Flag
}

void GuildHandler::processGuildDelAlliance(Net::MessageIn &msg) const
{
    msg.readInt32();  // Guild ID
    msg.readInt32();  // Flag
}

void GuildHandler::processGuildOppositionAck(Net::MessageIn &msg) const
{
    msg.readInt8();  // Flag
}

void GuildHandler::processGuildBroken(Net::MessageIn &msg) const
{
    msg.readInt32();  // Flag
}

void GuildHandler::clear() const
{
    taGuild = nullptr;
}

ChatTab *GuildHandler::getTab() const
{
    return guildTab;
}

}  // namespace Ea
