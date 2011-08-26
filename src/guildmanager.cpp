/*
 *  The ManaPlus Client
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

#include "guildmanager.h"

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "guild.h"
#include "localplayer.h"

#include "gui/socialwindow.h"

#include "gui/widgets/guildtab.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "utils/stringutils.h"

#include "debug.h"


GuildManager::GuildManager() :
    mEnableGuildBot(false),
    mGotInfo(false),
    mGotName(false),
    mHavePower(false),
    mTab(0),
    mRequest(false)
{
}

GuildManager::~GuildManager()
{
    mTab = 0;
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
}

void GuildManager::send(std::string msg)
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

void GuildManager::getNames(std::vector<std::string> &names)
{
}

void GuildManager::requestGuildInfo()
{
    if (mRequest)
        return;

    if (!mGotName)
    {
        send("!info");
        mRequest = true;
    }
    else if (!mGotInfo)
    {
        send("!getonlineinfo");
        mRequest = true;
    }
}

void GuildManager::updateList()
{
    Guild *guild = Guild::getGuild(1);
    if (guild)
    {
        logger->log("filling player");
        guild->setServerGuild(false);
        std::vector<std::string>::iterator it = mTempList.begin();
        std::vector<std::string>::iterator it_end = mTempList.end();
        int i = 0;
        while (it != it_end)
        {
            std::string name = *it;
            if (name.size() > 1)
            {
                int status = atoi(name.substr(name.size() - 1).c_str());
                name = name.substr(0, name.size() - 1);
                GuildMember *m = guild->addMember(i, 0, name);
                if (m)
                {
                    m->setOnline(status & 1);
                    m->setGender(GENDER_UNSPECIFIED);
                }
            }
            ++ it;
            i ++;
        }
        guild->sort();
        if (actorSpriteManager)
            actorSpriteManager->updatePlayerGuild();
    }
    mTempList.clear();
    mGotInfo = true;
    if (!mTab)
    {
        mTab = new GuildTab();
        mTab->loadFromLogFile("#Guild");
        if (player_node)
            player_node->addGuild(guild);
    }
}

bool GuildManager::processGuildMessage(std::string msg)
{
    bool res = process(msg);

    if (!mRequest)
        requestGuildInfo();

    return res;
}

bool GuildManager::process(std::string msg)
{
    if (msg.size() > 4 && msg[0] == '#' && msg[1] == '#')
        msg = msg.substr(3);

    Guild *guild = Guild::getGuild(1);
    if (!guild)
        return false;

    guild->setServerGuild(false);

    if (findCutLast(msg, " is now Offline."))
    {
        if (msg.size() < 4)
            return false;
        if (msg[0] == '#' && msg[1] == '#')
            msg = msg.substr(3);

        GuildMember *m = guild->getMember(msg);
        if (m)
            m->setOnline(false);
        mRequest = false;
        return true;
    }
    else if (findCutLast(msg, " is now Online."))
    {
        if (msg.size() < 4)
            return false;
        if (msg[0] == '#' && msg[1] == '#')
            msg = msg.substr(3);
        GuildMember *m = guild->getMember(msg);
        if (m)
            m->setOnline(true);
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "Welcome to the "))
    {
        logger->log("welcome message: %s", msg.c_str());
        int pos = msg.find("! (");
        if (pos == (int)std::string::npos)
            return false;
        msg = msg.substr(0, pos);
        guild->setName(msg);
        if (player_node)
            player_node->setGuildName(msg);
        mGotName = true;
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "Player name: "))
    {
        int pos = msg.find("Access Level: ");
        if (pos == (int)std::string::npos)
            return false;

        msg = msg.substr(pos);
        if (!findCutFirst(msg, "Access Level: "))
            return false;

        pos = msg.find(", Guild:");
        if (pos == (int)std::string::npos)
            return false;

        int level = atoi(msg.substr(0, pos).c_str());
        if (level >= 10)
            mHavePower = true;
        else
            mHavePower = false;

        msg = msg.substr(pos + strlen(", Guild:"));
        pos = msg.find(", No. Of Online Players: ");
        if (pos == (int)std::string::npos)
            return false;

        msg = msg.substr(0, pos);
//        logger->log("guild name: %s", msg.c_str());

        guild->setName(msg);
        if (player_node)
            player_node->setGuildName(msg);
        mGotName = true;
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "OL#"))
    {
        logger->log("OL");
        mTempList.clear();
        splitToStringVector(mTempList, msg, '#');
        if (msg.size() < 1 || msg[msg.size() - 1] != '#')
            updateList();
        mRequest = false;
        return true;
    }
    else if (findCutFirst(msg, "oL#"))
    {
        logger->log("oL");
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
    else
    {
        if (mTab)
        {
            std::string::size_type pos = msg.find(": ", 0);
            if (pos != std::string::npos)
            {
                std::string sender_name = ((pos == std::string::npos)
                    ? "" : msg.substr(0, pos));

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

void GuildManager::kick(std::string msg)
{
}

void GuildManager::invite(std::string msg)
{
}

void GuildManager::leave()
{
}

void GuildManager::notice(std::string msg)
{
}
