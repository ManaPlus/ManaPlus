/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "actormanager.h"
#include "configuration.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/guildtab.h"

#include "net/messagein.h"

#include "net/eathena/guildhandler.h"

#include "utils/delete2.h"
#include "utils/checkutils.h"
#include "utils/gettext.h"

#include "debug.h"

namespace EAthena
{

Guild *taGuild = nullptr;

namespace GuildRecv
{
    bool showBasicInfo = false;
}  // namespace GuildRecv

void GuildRecv::processGuildCreateResponse(Net::MessageIn &msg)
{
    const uint8_t flag = msg.readUInt8("flag");

    switch (flag)
    {
        case 0:
            // Success
            NotifyManager::notify(NotifyTypes::GUILD_CREATED);
            break;

        case 1:
            // Already in a guild
            NotifyManager::notify(NotifyTypes::GUILD_ALREADY);
            break;

        case 2:
            // Unable to make (likely name already in use)
            NotifyManager::notify(NotifyTypes::GUILD_ALREADY);
            break;

        case 3:
            // Emperium check failed
            NotifyManager::notify(NotifyTypes::GUILD_EMPERIUM_CHECK_FAILED);
            break;

        default:
            // Unknown response
            NotifyManager::notify(NotifyTypes::GUILD_ERROR);
            break;
    }
}

void GuildRecv::processGuildMasterOrMember(Net::MessageIn &msg)
{
    msg.readInt32("type");  // Type (0x57 for member, 0xd7 for master)
}

void GuildRecv::processGuildBasicInfo(Net::MessageIn &msg)
{
    const int guildId = msg.readInt32("guild id");
    const int level = msg.readInt32("guild level");
    const int members = msg.readInt32("connect member");
    const int maxMembers = msg.readInt32("max member");
    const int avgLevel = msg.readInt32("average level");
    const int exp = msg.readInt32("exp");
    const int nextExp = msg.readInt32("next exp");
    msg.skip(12, "unused");
    const int emblem = msg.readInt32("emblem id");
    std::string name = msg.readString(24, "guild name");
    std::string castle;
    std::string master;
    if (msg.getVersion() >= 20160622)
    {
        castle = msg.readString(16, "castles");
        msg.readInt32("money, unused");
        msg.readBeingId("leader char id");
    }
    else
    {
        master = msg.readString(24, "master name");
        castle = msg.readString(16, "castles");
        msg.readInt32("money, unused");
    }

    if (guildTab != nullptr &&
        showBasicInfo)
    {
        showBasicInfo = false;
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild name: %s"),
            name.c_str()), ChatMsgType::BY_SERVER);
        if (!master.empty())
        {
            // TRANSLATORS: guild info message
            guildTab->chatLog(strprintf(_("Guild master: %s"),
                master.c_str()), ChatMsgType::BY_SERVER);
        }
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild level: %d"), level),
            ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Online members: %d"),
            members), ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Max members: %d"),
            maxMembers), ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Average level: %d"),
            avgLevel), ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild exp: %d"), exp),
            ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild next exp: %d"),
            nextExp), ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild castle: %s"),
            castle.c_str()), ChatMsgType::BY_SERVER);
    }

    Guild *const g = Guild::getGuild(CAST_S16(guildId));
    if (g == nullptr)
        return;
    g->setName(name);
    g->setEmblemId(emblem);
}

void GuildRecv::processGuildAlianceInfo(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");
    if (length < 4)
        return;
    const int count = (length - 4) / 32;

    for (int i = 0; i < count; i++)
    {
        msg.readInt32("opposition");
        msg.readInt32("guild id");
        msg.readString(24, "guild name");
    }
}

void GuildRecv::processGuildMemberList(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");
    if (length < 4)
        return;
    int guildSize = 0;
    if (msg.getVersion() >= 20161026)
    {
        guildSize = 34;
        reportAlways("missing guild member names");
    }
    else
    {
        guildSize = 104;
    }

    const int count = (length - 4) / guildSize;
    if (taGuild == nullptr)
    {
        logger->log1("!taGuild");
        return;
    }

    taGuild->clearMembers();

    int onlineNum = 0;
    int totalNum = 0;
    for (int i = 0; i < count; i++)
    {
        const BeingId id = msg.readBeingId("account id");
        const int charId = msg.readInt32("char id");
        msg.readInt16("hair");
        msg.readInt16("hair color");
        const int gender = msg.readInt16("gender");
        const int race = msg.readInt16("class");
        const int level = msg.readInt16("level");
        const int exp = msg.readInt32("exp");
        const int online = msg.readInt32("online");
        const int pos = msg.readInt32("position");
        std::string name;
        if (msg.getVersion() < 20161026)
        {
            msg.skip(50, "unused");
            name = msg.readString(24, "name");
        }
        else
        {
            continue;
        }

        GuildMember *const m = taGuild->addMember(id, charId, name);
        if (m != nullptr)
        {
            m->setOnline(online != 0);
            m->setID(id);
            m->setCharId(charId);
            m->setGender(Being::intToGender(CAST_U8(gender)));
            m->setLevel(level);
            m->setExp(exp);
            m->setPos(pos);
            m->setRace(race);
            if (actorManager != nullptr)
            {
                Being *const being = actorManager->findBeingByName(
                    name, ActorType::Player);
                if (being != nullptr)
                {
                    being->setGuildName(taGuild->getName());
                    if (being->getLevel() != level)
                    {
                        being->setLevel(level);
                        being->updateName();
                    }
                }
            }
            if (online != 0)
                onlineNum ++;
            totalNum ++;
        }
    }
    taGuild->sort();
    if (actorManager != nullptr)
    {
        actorManager->updatePlayerGuild();
        actorManager->updatePlayerColors();
    }
    if (socialWindow != nullptr)
        socialWindow->updateGuildCounter(onlineNum, totalNum);
}

void GuildRecv::processGuildPosNameList(Net::MessageIn &msg)
{
    if (taGuild == nullptr)
    {
        logger->log1("!taGuild");
        return;
    }

    const int length = msg.readInt16("len");
    if (length < 4)
        return;
    const int count = (length - 4) / 28;

    for (int i = 0; i < count; i++)
    {
        const int id = msg.readInt32("position id");
        const std::string name = msg.readString(24, "position name");
        taGuild->addPos(id, name);
    }
}

void GuildRecv::processGuildPosInfoList(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");
    if (length < 4)
        return;
    const int count = (length - 4) / 16;

    for (int i = 0; i < count; i++)
    {
        msg.readInt32("id");
        msg.readInt32("mode");
        msg.readInt32("same id");
        msg.readInt32("exp mode");
    }
}

void GuildRecv::processGuildPositionChanged(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("len");
    msg.readInt32("id");
    msg.readInt32("mode");
    msg.readInt32("same ip");
    msg.readInt32("exp mode");
    msg.readString(24, "name");
}

void GuildRecv::processGuildMemberPosChange(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const BeingId accountId = msg.readBeingId("account id");
    const int charId = msg.readInt32("char id");
    const int pos = msg.readInt32("position");
    if (taGuild != nullptr)
    {
        GuildMember *const m = taGuild->getMember(accountId, charId);
        if (m != nullptr)
            m->setPos(pos);
    }
}

void GuildRecv::processGuildEmblemData(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int length = msg.readInt16("len");

    msg.readInt32("guild id");
    msg.readInt32("emblem id");
    if (length < 12)
        return;
    msg.skip(length - 12, "emblem data");
}

void GuildRecv::processGuildSkillInfo(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 6) / 37;
    msg.readInt16("skill points");

    if (skillDialog != nullptr)
        skillDialog->hideSkills(SkillOwner::Guild);
    for (int i = 0; i < count; i++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt32("inf"));
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog != nullptr)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Guild,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog != nullptr)
        skillDialog->updateModels();
}

void GuildRecv::processGuildNotice(Net::MessageIn &msg)
{
    const std::string msg1 = msg.readString(60, "msg1");
    const std::string msg2 = msg.readString(120, "msg2");
    if (guildTab != nullptr)
    {
        guildTab->chatLog(msg1, ChatMsgType::BY_SERVER);
        guildTab->chatLog(msg2, ChatMsgType::BY_SERVER);
    }
}

void GuildRecv::processGuildInvite(Net::MessageIn &msg)
{
    const int guildId = msg.readInt32("guild id");
    const std::string guildName = msg.readString(24, "guild name");

    if (socialWindow != nullptr)
        socialWindow->showGuildInvite(guildName, guildId, "");
}

void GuildRecv::processGuildInviteAck(Net::MessageIn &msg)
{
    const uint8_t flag = msg.readUInt8("flag");
    if (guildTab == nullptr)
        return;

    switch (flag)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::GUILD_INVITE_FAILED);
            break;

        case 1:
            NotifyManager::notify(NotifyTypes::GUILD_INVITE_REJECTED);
            break;

        case 2:
            NotifyManager::notify(NotifyTypes::GUILD_INVITE_JOINED);
            break;

        case 3:
            NotifyManager::notify(NotifyTypes::GUILD_INVITE_FULL);
            break;

        default:
            NotifyManager::notify(NotifyTypes::GUILD_INVITE_ERROR);
            break;
    }
}

void GuildRecv::processGuildLeave(Net::MessageIn &msg)
{
    const std::string nick = msg.readString(24, "nick");
    msg.readString(40, "message");

    if (taGuild != nullptr)
        taGuild->removeMember(nick);

    if (localPlayer == nullptr)
        return;

    if (nick == localPlayer->getName())
    {
        if (taGuild != nullptr)
        {
            taGuild->removeFromMembers();
            taGuild->clearMembers();
            localPlayer->removeGuild(taGuild->getId());
        }
        NotifyManager::notify(NotifyTypes::GUILD_LEFT);
        delete2(guildTab)

        if ((socialWindow != nullptr) && (taGuild != nullptr))
            socialWindow->removeTab(taGuild);
        if (actorManager != nullptr)
            actorManager->updatePlayerColors();
    }
    else
    {
        NotifyManager::notify(NotifyTypes::GUILD_USER_LEFT, nick);
        if (actorManager != nullptr)
        {
            Being *const b = actorManager->findBeingByName(
                nick, ActorType::Player);

            if (b != nullptr)
                b->clearGuilds();
            if (taGuild != nullptr)
                taGuild->removeMember(nick);
        }
    }
}

void GuildRecv::processGuildMessage(Net::MessageIn &msg)
{
    const int msgLength = msg.readInt16("len") - 4;

    if (msgLength <= 0)
        return;
    if (guildTab != nullptr)
    {
        std::string chatMsg = msg.readString(msgLength, "message");
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
            guildTab->chatLog(chatMsg, ChatMsgType::BY_SERVER);
        }
    }
    else
    {
        DEBUGLOGSTR("invisible guild?");
        msg.readString(msgLength, "message");
    }
}

void GuildRecv::processGuildSkillUp(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("skill id");
    msg.readInt16("level");
    msg.readInt16("sp");
    msg.readInt16("range");
    msg.readUInt8("unused?");
}

void GuildRecv::processGuildReqAlliance(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("id");
    msg.readString(24, "name");
}

void GuildRecv::processGuildReqAllianceAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("flag");
}

void GuildRecv::processGuildDelAlliance(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("guild id");
    msg.readInt32("flag");
}

void GuildRecv::processGuildOppositionAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("flag");
}

void GuildRecv::processGuildBroken(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("flag");
}

void GuildRecv::processGuildExpulsionContinue(const std::string &nick)
{
    if (taGuild != nullptr)
        taGuild->removeMember(nick);

    if (localPlayer == nullptr)
        return;

    if (nick == localPlayer->getName())
    {
        if (taGuild != nullptr)
        {
            taGuild->removeFromMembers();
            taGuild->clearMembers();
        }
        NotifyManager::notify(NotifyTypes::GUILD_KICKED);
        delete2(guildTab)

        if ((socialWindow != nullptr) && (taGuild != nullptr))
            socialWindow->removeTab(taGuild);
        if (actorManager != nullptr)
            actorManager->updatePlayerColors();
    }
    else
    {
        NotifyManager::notify(NotifyTypes::GUILD_USER_KICKED, nick);
        if (actorManager != nullptr)
        {
            Being *const b = actorManager->findBeingByName(
                nick, ActorType::Player);

            if (b != nullptr)
                b->clearGuilds();
            if (taGuild != nullptr)
                taGuild->removeMember(nick);
        }
    }
}

void GuildRecv::processGuildUpdateCoords(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    if (taGuild != nullptr)
    {
        GuildMember *const m = taGuild->getMember(id);
        if (m != nullptr)
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
    if (g == nullptr)
        return;

    g->setName(guildName);
    g->setEmblemId(emblem);
    if (taGuild == nullptr)
        taGuild = g;
    if ((guildTab == nullptr) && (chatWindow != nullptr))
    {
        guildTab = new GuildTab(chatWindow);
        if (config.getBoolValue("showChatHistory"))
            guildTab->loadFromLogFile("#Guild");
        if (localPlayer != nullptr)
            localPlayer->addGuild(taGuild);
        guildHandler->memberList();
    }

    if (localPlayer != nullptr)
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
    if (taGuild != nullptr)
    {
        GuildMember *const m = taGuild->getMember(accountId, charId);
        if (m != nullptr)
        {
            m->setOnline(online != 0);
            if (online != 0)
                m->setGender(gender);
            if (guildTab != nullptr)
                guildTab->showOnline(m->getName(), fromBool(online, Online));
            if (socialWindow != nullptr)
                socialWindow->updateGuildCounter();
        }
    }
}

void GuildRecv::processGuildExpulsion(Net::MessageIn &msg)
{
    const std::string nick = msg.readString(24, "name");
    msg.readString(40, "message");

    GuildRecv::processGuildExpulsionContinue(nick);
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
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("being id");
    msg.readInt32("guild id");
    msg.readInt16("emblem id");
}

void GuildRecv::processOnlineInfo(Net::MessageIn &msg)
{
    // look like unused packet
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("being id");
    msg.readInt32("char id");
    msg.readInt32("online");
}

}  // namespace EAthena
