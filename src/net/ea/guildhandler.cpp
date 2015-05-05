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

#include "net/ea/guildhandler.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"

#include "net/messagein.h"

#include "gui/widgets/tabs/chat/guildtab.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/notifytypes.h"

#include "debug.h"

GuildTab *guildTab = nullptr;

namespace Ea
{
Guild *taGuild = nullptr;

bool GuildHandler::showBasicInfo = false;

GuildHandler::GuildHandler()
{
    showBasicInfo = false;
}

GuildHandler::~GuildHandler()
{
    delete2(guildTab);
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

void GuildHandler::processGuildCreateResponse(Net::MessageIn &msg)
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

void GuildHandler::processGuildMasterOrMember(Net::MessageIn &msg)
{
    msg.readInt32("type");  // Type (0x57 for member, 0xd7 for master)
}

void GuildHandler::processGuildBasicInfo(Net::MessageIn &msg)
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
    std::string master = msg.readString(24, "master name");
    std::string castle = msg.readString(16, "castles");
    msg.readInt32("unused");

    if (guildTab && showBasicInfo)
    {
        showBasicInfo = false;
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild name: %s"),
            name.c_str()), ChatMsgType::BY_SERVER);
        // TRANSLATORS: guild info message
        guildTab->chatLog(strprintf(_("Guild master: %s"),
            master.c_str()), ChatMsgType::BY_SERVER);
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

    Guild *const g = Guild::getGuild(static_cast<int16_t>(guildId));
    if (!g)
        return;
    g->setName(name);
    g->setEmblemId(emblem);
}

void GuildHandler::processGuildAlianceInfo(Net::MessageIn &msg)
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

void GuildHandler::processGuildMemberList(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");
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
        const int id = msg.readInt32("account id");
        const int charId = msg.readInt32("char id");
        msg.readInt16("hair");
        msg.readInt16("hair color");
        const int gender = msg.readInt16("gender");
        const int race = msg.readInt16("class");
        const int level = msg.readInt16("level");
        const int exp = msg.readInt32("exp");
        const int online = msg.readInt32("online");
        const int pos = msg.readInt32("position");
        msg.skip(50, "unused");
        std::string name = msg.readString(24, "name");

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
                    name, ActorType::Player);
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

void GuildHandler::processGuildPosNameList(Net::MessageIn &msg)
{
    if (!taGuild)
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

void GuildHandler::processGuildPosInfoList(Net::MessageIn &msg)
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

void GuildHandler::processGuildPositionChanged(Net::MessageIn &msg)
{
    msg.readInt16("len");
    msg.readInt32("id");
    msg.readInt32("mode");
    msg.readInt32("same ip");
    msg.readInt32("exp mode");
    msg.readString(24, "name");
}

void GuildHandler::processGuildMemberPosChange(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const int accountId = msg.readInt32("account id");
    const int charId = msg.readInt32("char id");
    const int pos = msg.readInt32("position");
    if (taGuild)
    {
        GuildMember *const m = taGuild->getMember(accountId, charId);
        if (m)
            m->setPos(pos);
    }
}

void GuildHandler::processGuildEmblemData(Net::MessageIn &msg)
{
    const int length = msg.readInt16("len");

    msg.readInt32("guild id");
    msg.readInt32("emblem id");
    if (length < 12)
        return;
    msg.skip(length - 12, "emblem data");
}

void GuildHandler::processGuildSkillInfo(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 6) / 37;
    msg.readInt16("skill points");

    if (skillDialog)
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
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Guild,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog)
        skillDialog->updateModels();
}

void GuildHandler::processGuildNotice(Net::MessageIn &msg)
{
    const std::string msg1 = msg.readString(60, "msg1");
    const std::string msg2 = msg.readString(120, "msg2");
    if (guildTab)
    {
        guildTab->chatLog(msg1, ChatMsgType::BY_SERVER);
        guildTab->chatLog(msg2, ChatMsgType::BY_SERVER);
    }
}

void GuildHandler::processGuildInvite(Net::MessageIn &msg)
{
    const int guildId = msg.readInt32("guild id");
    const std::string guildName = msg.readString(24, "guild name");

    if (socialWindow)
        socialWindow->showGuildInvite(guildName, guildId, "");
}

void GuildHandler::processGuildInviteAck(Net::MessageIn &msg)
{
    const uint8_t flag = msg.readUInt8("flag");
    if (!guildTab)
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

void GuildHandler::processGuildLeave(Net::MessageIn &msg)
{
    const std::string nick = msg.readString(24, "nick");
    msg.readString(40, "message");

    if (taGuild)
        taGuild->removeMember(nick);

    if (!localPlayer)
        return;

    if (nick == localPlayer->getName())
    {
        if (taGuild)
        {
            taGuild->removeFromMembers();
            taGuild->clearMembers();
        }
        NotifyManager::notify(NotifyTypes::GUILD_LEFT);
        delete2(guildTab)

        if (socialWindow && taGuild)
            socialWindow->removeTab(taGuild);
        if (actorManager)
            actorManager->updatePlayerColors();
    }
    else
    {
        NotifyManager::notify(NotifyTypes::GUILD_USER_LEFT, nick);
        if (actorManager)
        {
            Being *const b = actorManager->findBeingByName(
                nick, ActorType::Player);

            if (b)
                b->clearGuilds();
            if (taGuild)
                taGuild->removeMember(nick);
        }
    }
}

void GuildHandler::processGuildMessage(Net::MessageIn &msg)
{
    const int msgLength = msg.readInt16("len") - 4;

    if (msgLength <= 0)
        return;
    if (guildTab)
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
            guildTab->chatLog(chatMsg);
        }
    }
}

void GuildHandler::processGuildSkillUp(Net::MessageIn &msg)
{
    msg.readInt16("skill id");
    msg.readInt16("level");
    msg.readInt16("sp");
    msg.readInt16("range");
    msg.readUInt8("unused?");
}

void GuildHandler::processGuildReqAlliance(Net::MessageIn &msg)
{
    msg.readInt32("id");
    msg.readString(24, "name");
}

void GuildHandler::processGuildReqAllianceAck(Net::MessageIn &msg)
{
    msg.readInt32("flag");
}

void GuildHandler::processGuildDelAlliance(Net::MessageIn &msg)
{
    msg.readInt32("guild id");
    msg.readInt32("flag");
}

void GuildHandler::processGuildOppositionAck(Net::MessageIn &msg)
{
    msg.readUInt8("flag");
}

void GuildHandler::processGuildBroken(Net::MessageIn &msg)
{
    msg.readInt32("flag");
}

void GuildHandler::clear() const
{
    taGuild = nullptr;
}

ChatTab *GuildHandler::getTab() const
{
    return guildTab;
}

void GuildHandler::processGuildExpulsionContinue(const std::string &nick)
{
    if (taGuild)
        taGuild->removeMember(nick);

    if (!localPlayer)
        return;

    if (nick == localPlayer->getName())
    {
        if (taGuild)
        {
            taGuild->removeFromMembers();
            taGuild->clearMembers();
        }
        NotifyManager::notify(NotifyTypes::GUILD_KICKED);
        delete2(guildTab)

        if (socialWindow && taGuild)
            socialWindow->removeTab(taGuild);
        if (actorManager)
            actorManager->updatePlayerColors();
    }
    else
    {
        NotifyManager::notify(NotifyTypes::GUILD_USER_KICKED, nick);
        if (actorManager)
        {
            Being *const b = actorManager->findBeingByName(
                nick, ActorType::Player);

            if (b)
                b->clearGuilds();
            if (taGuild)
                taGuild->removeMember(nick);
        }
    }
}

}  // namespace Ea
