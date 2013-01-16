/*
 *  The ManaPlus Client
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

#include "guildmanager.h"

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "guild.h"
#include "localplayer.h"

#include "gui/socialwindow.h"

#include "gui/widgets/guildchattab.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "utils/gettext.h"

#include "debug.h"

bool GuildManager::mEnableGuildBot = false;

GuildManager::GuildManager() :
    mGotInfo(false),
    mGotName(false),
    mSentInfoRequest(false),
    mSentNameRequest(false),
    mHavePower(false),
    mTab(nullptr),
    mRequest(false)
{
}

GuildManager::~GuildManager()
{
    delete mTab;
    mTab = nullptr;
}

void GuildManager::init()
{
    int val = serverConfig.getValue("enableGuildBot", -1);
    if (val == -1)
    {
        if (Client::isTmw())
            val = 1;
        else
            val = 0;
        serverConfig.setValue("enableGuildBot", val);
    }
    mEnableGuildBot = val;
    if (mEnableGuildBot)
    {
        if (!guildManager)
            guildManager = new GuildManager();
        else
            guildManager->reload();
    }
    else if (guildManager)
    {
        delete guildManager;
        guildManager = nullptr;
    }
}

void GuildManager::reload()
{
    mGotInfo = false;
    mGotName = false;
    mHavePower = false;
    mRequest = false;
    mSentNameRequest = false;
    mSentInfoRequest = false;
    mTempList.clear();

    if (socialWindow)
    {
        Guild *const guild = Guild::getGuild(1);
        if (guild && socialWindow)
            socialWindow->removeTab(guild);
    }
    delete mTab;
    mTab = nullptr;
}

void GuildManager::send(std::string msg) const
{
    Net::getChatHandler()->privateMessage("guild", msg);
}

void GuildManager::chat(std::string msg)
{
    if (!player_node || !mTab)
        return;

    Net::getChatHandler()->privateMessage("guild", msg);
    mTab->chatLog(player_node->getName(), msg);
}

void GuildManager::getNames(StringVect &names) const
{
    const Guild *const guild = createGuild();
    if (guild)
        guild->getNames(names);
}

void GuildManager::requestGuildInfo()
{
    if (mRequest)
        return;

    if (!mGotName && !mSentNameRequest)
    {
        if (!Client::limitPackets(PACKET_WHISPER))
            return;
        send("!info " + toString(tick_time));
        mRequest = true;
        mSentNameRequest = true;
    }
    else if (!mGotInfo && !mSentInfoRequest && !mSentNameRequest)
    {
        if (!Client::limitPackets(PACKET_WHISPER))
            return;
        send("!getonlineinfo " + toString(tick_time));
        mRequest = true;
        mSentInfoRequest = true;
    }
}

void GuildManager::updateList()
{
    Guild *const guild = Guild::getGuild(1);
    if (guild)
    {
        guild->setServerGuild(false);
        StringVectCIter it = mTempList.begin();
        const StringVectCIter it_end = mTempList.end();
        int i = 0;
        while (it != it_end)
        {
            std::string name = *it;
            if (name.size() > 1)
            {
                const int status = atoi(name.substr(name.size() - 1).c_str());

                name = name.substr(0, name.size() - 1);
                GuildMember *const m = guild->addMember(i, 0, name);
                if (m)
                {
                    m->setOnline(status & 1);
                    m->setGender(GENDER_UNSPECIFIED);
                    if (status & 2)
                        m->setPos(10);
                    else
                        m->setPos(0);
                    if (player_node && name == player_node->getName())
                    {
                        mHavePower = (status & 2);
                        m->setOnline(true);
                    }
                }
            }
            ++ it;
            i ++;
        }
        guild->sort();
        createTab(guild);
        if (actorSpriteManager)
        {
            actorSpriteManager->updatePlayerGuild();
            actorSpriteManager->updatePlayerColors();
        }
    }
    mTempList.clear();
    mSentInfoRequest = false;
    mGotInfo = true;
}

void GuildManager::createTab(Guild *const guild)
{
    if (!mTab)
    {
        mTab = new GuildChatTab(chatWindow);
        if (config.getBoolValue("showChatHistory"))
            mTab->loadFromLogFile("#Guild");
        if (player_node)
            player_node->addGuild(guild);
    }
}

Guild *GuildManager::createGuild() const
{
    Guild *const guild = Guild::getGuild(1);
    if (!guild)
        return nullptr;

    guild->setServerGuild(false);
    return guild;
}

bool GuildManager::processGuildMessage(std::string msg)
{
    const bool res = process(msg);

    if (!mRequest)
        requestGuildInfo();

    return res;
}

bool GuildManager::process(std::string msg)
{
    if (msg.size() > 4 && msg[0] == '#' && msg[1] == '#')
        msg = msg.substr(3);

    const bool haveNick = (msg.find(": ") != std::string::npos);

    if (!haveNick && findCutLast(msg, " is now Offline."))
    {
        Guild *const guild = createGuild();
        if (!guild)
            return false;
        if (msg.size() < 4)
            return false;
        if (msg[0] == '#' && msg[1] == '#')
            msg = msg.substr(3);

        GuildMember *const m = guild->addMember(msg);
        if (m)
            m->setOnline(false);
        guild->sort();
        mRequest = false;
        return true;
    }
    else if (!haveNick && findCutLast(msg, " is now Online."))
    {
        Guild *const guild = createGuild();
        if (!guild)
            return false;
        if (msg.size() < 4)
            return false;
        if (msg[0] == '#' && msg[1] == '#')
            msg = msg.substr(3);
        GuildMember *const m = guild->addMember(msg);
        if (m)
            m->setOnline(true);
        guild->sort();
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "Welcome to the "))
    {
        Guild *const guild = createGuild();
        if (!guild)
            return false;
//        logger->log("welcome message: %s", msg.c_str());
        const size_t pos = msg.find("! (");
        if (pos == std::string::npos)
            return false;
        msg = msg.substr(0, pos);
        guild->setName(msg);
        if (player_node)
            player_node->setGuildName(msg);
        mGotName = true;
        mSentNameRequest = false;
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "Player name: "))
    {
        Guild *const guild = createGuild();
        if (!guild)
            return false;
        size_t pos = msg.find("Access Level: ");
        if (pos == std::string::npos)
            return false;

        msg = msg.substr(pos);
        if (!findCutFirst(msg, "Access Level: "))
            return false;

        pos = msg.find(", Guild:");
        if (pos == std::string::npos)
            return false;

        const int level = atoi(msg.substr(0, pos).c_str());
        if (level >= 10)
            mHavePower = true;
        else
            mHavePower = false;

        msg = msg.substr(pos + strlen(", Guild:"));
        pos = msg.find(", No. Of Online Players: ");
        if (pos == std::string::npos)
            return false;

        msg = msg.substr(0, pos);
//        logger->log("guild name: %s", msg.c_str());

        guild->setName(msg);
        if (player_node)
            player_node->setGuildName(msg);
        mGotName = true;
        mSentNameRequest = false;
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "OL#"))
    {
//        logger->log("OL");
        mTempList.clear();
        splitToStringVector(mTempList, msg, '#');
        if (msg.size() < 1 || msg[msg.size() - 1] != '#')
            updateList();
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "oL#"))
    {
//        logger->log("oL");
        splitToStringVector(mTempList, msg, '#');
        if (msg.size() < 1 || msg[msg.size() - 1] != '#')
            updateList();
        mRequest = false;
        return true;
    }
    else if (msg == "You are currently not in a guild. For more information "
             "or to discuss the possibility of adding you own guild "
             "please contact Jero.")
    {
        mRequest = true;
        return true;
    }
    else if (findCutFirst(msg, "You have been invited to the ")
             && findCutLast(msg, " guild chat. If you would like to accept "
             "this invitation please reply \"yes\" and if not then \"no\" ."))
    {
        if (socialWindow)
            socialWindow->showGuildInvite(msg, 1, "");
        return true;
    }
    else if (!haveNick && (findCutLast(msg, " has been removed "
             "from the Guild.") || findCutLast(msg, " has left the Guild.")))
    {
        Guild *const guild = createGuild();
        if (!guild)
            return false;
        if (msg.size() < 4)
            return false;
        if (msg[0] == '#' && msg[1] == '#')
            msg = msg.substr(3);

        if (actorSpriteManager)
        {
            Being *const b = actorSpriteManager->findBeingByName(
                msg, Being::PLAYER);

            if (b)
            {
                b->clearGuilds();
                b->setGuildName("");
                b->updateColors();
            }
        }

        guild->removeMember(msg);
        return true;
    }
    else if (msg == "You have been removed from the Guild"
             || msg == "You have left the Guild")
    {
        return afterRemove();
    }
    else
    {
        Guild *const guild = createGuild();
        if (!guild)
            return false;
        if (!mTab)
        {
            createTab(guild);
        }
        if (mTab)
        {
            const size_t pos = msg.find(": ", 0);
            if (pos != std::string::npos)
            {
                std::string sender_name = ((pos == std::string::npos)
                    ? "" : msg.substr(0, pos));
                if (!guild->getMember(sender_name))
                {
                    mTab->chatLog(msg);
                    return true;
                }

                msg.erase(0, pos + 2);
                if (msg.size() > 3 && msg[0] == '#' && msg[1] == '#')
                    msg.erase(0, 3);

                trim(msg);
                mTab->chatLog(sender_name, msg);
            }
            else
            {
                mTab->chatLog(msg);
            }
            return true;
        }
    }
    return false;
}

void GuildManager::kick(std::string msg) const
{
    send("!remove " + msg);
}

void GuildManager::invite(std::string msg) const
{
    send("!invite " + msg);
}

void GuildManager::leave() const
{
    send("!leave");
}

void GuildManager::notice(std::string msg) const
{
    if (msg.empty())
        send("!removemotd");
    else
        send("!setmotd " + msg);
}

void GuildManager::clear() const
{
    if (socialWindow)
    {
        Guild *const guild = Guild::getGuild(1);
        if (guild)
            socialWindow->removeTab(guild);
    }
}

void GuildManager::inviteResponse(const bool response) const
{
    if (response)
        send("yes");
    else
        send("no");
}

bool GuildManager::afterRemove()
{
    Guild *const guild = createGuild();
    if (!guild)
        return false;
    guild->removeFromMembers();
    guild->clearMembers();
    if (player_node)
    {
        player_node->setGuildName("");
        player_node->clearGuilds();
    }
    SERVER_NOTICE(_("You have left the guild."))
    delete mTab;
    mTab = nullptr;

    if (socialWindow)
        socialWindow->removeTab(guild);
    if (actorSpriteManager)
    {
        actorSpriteManager->updatePlayerGuild();
        actorSpriteManager->updatePlayerColors();
    }
    reload();
    return true;
}
