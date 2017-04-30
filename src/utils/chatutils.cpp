/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "utils/chatutils.h"

#include "actormanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "const/gui/chat.h"

#include "gui/widgets/tabs/chat/whispertab.h"

#include "net/chathandler.h"
#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"
#include "net/serverfeatures.h"

#ifdef TMWA_SUPPORT
#include "net/tmwa/guildmanager.h"
#endif  // TMWA_SUPPORT

#include "utils/stringutils.h"

#include "debug.h"

void outStringNormal(ChatTab *const tab,
                     const std::string &str,
                     const std::string &def)
{
    if (!localPlayer)
        return;

    if (!tab)
    {
        chatHandler->talk(str, GENERAL_CHANNEL);
        return;
    }

    switch (tab->getType())
    {
        case ChatTabType::PARTY:
        {
            partyHandler->chat(str);
            break;
        }
        case ChatTabType::GUILD:
        {
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
            {
#ifdef TMWA_SUPPORT
                if (guild->getServerGuild())
                {
                    if (Net::getNetworkType() == ServerType::TMWATHENA)
                        return;
                    guildHandler->chat(str);
                }
                else if (guildManager)
                {
                    guildManager->chat(str);
                }
#else  // TMWA_SUPPORT

                if (guild->getServerGuild())
                    guildHandler->chat(str);
#endif  // TMWA_SUPPORT
            }
            break;
        }
        case ChatTabType::WHISPER:
        {
            const WhisperTab *const whisper
                = static_cast<const WhisperTab *const>(tab);
            tab->chatLog(localPlayer->getName(), str);
            chatHandler->privateMessage(whisper->getNick(), str);
            break;
        }
        case ChatTabType::DEBUG:
        case ChatTabType::BATTLE:
            break;
        default:
        case ChatTabType::UNKNOWN:
        case ChatTabType::INPUT:
        case ChatTabType::TRADE:
        case ChatTabType::LANG:
        case ChatTabType::GM:
        case ChatTabType::CHANNEL:
            chatHandler->talk(def, GENERAL_CHANNEL);
            break;
    }
}

void replaceVars(std::string &str)
{
    if (!localPlayer || !actorManager)
        return;

    if (str.find("<PLAYER>") != std::string::npos)
    {
        const Being *target = localPlayer->getTarget();
        if (!target || target->getType() != ActorType::Player)
        {
            target = actorManager->findNearestLivingBeing(
                localPlayer, 20, ActorType::Player, AllowSort_true);
        }
        if (target)
            replaceAll(str, "<PLAYER>", target->getName());
        else
            replaceAll(str, "<PLAYER>", "");
    }
    if (str.find("<MONSTER>") != std::string::npos)
    {
        const Being *target = localPlayer->getTarget();
        if (!target || target->getType() != ActorType::Monster)
        {
            target = actorManager->findNearestLivingBeing(
                localPlayer, 20, ActorType::Monster, AllowSort_true);
        }
        if (target)
            replaceAll(str, "<MONSTER>", target->getName());
        else
            replaceAll(str, "<MONSTER>", "");
    }
    if (str.find("<PEOPLE>") != std::string::npos)
    {
        StringVect names;
        std::string newStr;
        actorManager->getPlayerNames(names, NpcNames_false);
        FOR_EACH (StringVectCIter, it, names)
        {
            if (*it != localPlayer->getName())
                newStr.append(*it).append(",");
        }
        if (!newStr.empty())
        {
            if (newStr[newStr.size() - 1] == ',')
                newStr = newStr.substr(0, newStr.size() - 1);
            replaceAll(str, "<PEOPLE>", newStr);
        }
        else
        {
            replaceAll(str, "<PEOPLE>", "");
        }
    }
    if (str.find("<PARTY>") != std::string::npos)
    {
        StringVect names;
        std::string newStr;
        const Party *party = nullptr;
        if (localPlayer->isInParty() &&
            (party = localPlayer->getParty()))
        {
            party->getNames(names);
            FOR_EACH (StringVectCIter, it, names)
            {
                if (*it != localPlayer->getName())
                    newStr.append(*it).append(",");
            }
            if (!newStr.empty())
            {
                if (newStr[newStr.size() - 1] == ',')
                    newStr = newStr.substr(0, newStr.size() - 1);
                replaceAll(str, "<PARTY>", newStr);
            }
            else
            {
                replaceAll(str, "<PARTY>", "");
            }
        }
        else
        {
            replaceAll(str, "<PARTY>", "");
        }
    }
}

std::string textToMe(const std::string &str)
{
    return strprintf("*%s*", str.c_str());
}
