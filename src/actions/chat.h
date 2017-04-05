/*
 *  The ManaPlus Client
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

#ifndef ACTIONS_CHAT_H
#define ACTIONS_CHAT_H

#define decHandler(name) bool name(InputEvent &event)

struct InputEvent;

namespace Actions
{
    decHandler(toggleChat);
    decHandler(prevChatTab);
    decHandler(nextChatTab);
    decHandler(closeChatTab);
    decHandler(closeAllChatTabs);
    decHandler(ignoreAllWhispers);
    decHandler(scrollChatUp);
    decHandler(scrollChatDown);
    decHandler(msg);
    decHandler(msgText);
    decHandler(msg2);
    decHandler(query);
    decHandler(clearChatTab);
    decHandler(createParty);
    decHandler(createGuild);
    decHandler(party);
    decHandler(guild);
    decHandler(me);
    decHandler(toggle);
    decHandler(kickParty);
    decHandler(kickGuild);
    decHandler(addText);
    decHandler(clearChat);
    decHandler(chatGeneralTab);
    decHandler(chatDebugTab);
    decHandler(chatBattleTab);
    decHandler(chatTradeTab);
    decHandler(chatLangTab);
    decHandler(chatGmTab);
    decHandler(chatPartyTab);
    decHandler(chatGuildTab);
    decHandler(hat);
    decHandler(chatClipboard);
    decHandler(guildNotice);
    decHandler(translate);
}  // namespace Actions

#undef decHandler

#endif  // ACTIONS_CHAT_H
