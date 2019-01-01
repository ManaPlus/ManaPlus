/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/ea/chatrecv.h"

#include "actormanager.h"
#include "configuration.h"
#include "notifymanager.h"

#include "being/being.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/chatwindow.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "net/messagein.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

namespace ChatRecv
{
    WhisperQueue mSentWhispers;
    time_t mMotdTime = 0;
    bool mShowAllLang = false;
    bool mShowMotd = false;
    bool mSkipping = true;
}  // namespace ChatRecv

std::string ChatRecv::getPopLastWhisperNick()
{
    std::string nick;
    if (mSentWhispers.empty())
    {
        nick = "user";
    }
    else
    {
        nick = mSentWhispers.front();
        mSentWhispers.pop();
    }
    return nick;
}

std::string ChatRecv::getLastWhisperNick()
{
    std::string nick;
    if (mSentWhispers.empty())
        nick = "user";
    else
        nick = mSentWhispers.front();
    return nick;
}

void ChatRecv::processWhisperResponseContinue(Net::MessageIn &msg,
                                              const uint8_t type)
{
    const std::string nick = getPopLastWhisperNick();
    switch (type)
    {
        case 0x00:
            // Success (don't need to report)
            break;
        case 0x01:
            if (chatWindow != nullptr)
            {
                chatWindow->addWhisper(nick,
                    // TRANSLATORS: chat message
                    strprintf(_("Whisper could not be sent, %s is offline."),
                    nick.c_str()),
                    ChatMsgType::BY_SERVER);
            }
            break;
        case 0x02:
            if (chatWindow != nullptr)
            {
                chatWindow->addWhisper(nick,
                    // TRANSLATORS: chat message
                    strprintf(_("Whisper could not "
                    "be sent, ignored by %s."), nick.c_str()),
                    ChatMsgType::BY_SERVER);
            }
            break;
        case 0x03:
            if (chatWindow != nullptr)
            {
                chatWindow->addWhisper(nick,
                    // TRANSLATORS: chat message
                    _("Whisper could not "
                    "be sent, you ignored by all players."),
                    ChatMsgType::BY_SERVER);
            }
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
    BLOCK_END("ChatRecv::processWhisperResponse")
}

void ChatRecv::processMVPEffect(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processMVPEffect")
    // Display MVP player
    const BeingId id = msg.readBeingId("being id");
    if (localChatTab != nullptr &&
        actorManager != nullptr &&
        config.getBoolValue("showMVP"))
    {
        const Being *const being = actorManager->findBeing(id);
        if (being == nullptr)
            NotifyManager::notify(NotifyTypes::MVP_PLAYER, "");
        else
            NotifyManager::notify(NotifyTypes::MVP_PLAYER, being->getName());
    }
    BLOCK_END("ChatRecv::processMVPEffect")
}

void ChatRecv::processIgnoreAllResponse(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processIgnoreAllResponse")
    const uint8_t action = msg.readUInt8("action");
    const uint8_t fail = msg.readUInt8("result");
    if (localChatTab == nullptr)
    {
        BLOCK_END("ChatRecv::processIgnoreAllResponse")
        return;
    }

    switch (action)
    {
        case 0:
        {
            switch (fail)
            {
                case 0:
                    NotifyManager::notify(NotifyTypes::WHISPERS_IGNORED);
                    break;
                default:
                    NotifyManager::notify(NotifyTypes::
                        WHISPERS_IGNORE_FAILED);
                    break;
            }
            break;
        }
        case 1:
        {
            switch (fail)
            {
                case 0:
                    NotifyManager::notify(NotifyTypes::WHISPERS_UNIGNORED);
                    break;
                default:
                    NotifyManager::notify(NotifyTypes::
                        WHISPERS_UNIGNORE_FAILED);
                    break;
            }
            break;
        }
        default:
            // unknown result
            break;
    }
    BLOCK_END("ChatRecv::processIgnoreAllResponse")
}

}  // namespace Ea
