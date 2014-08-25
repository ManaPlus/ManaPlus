/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "actions/chat.h"

#include "actormanager.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "itemshortcut.h"
#include "soundmanager.h"

#include "actions/actiondef.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "net/net.h"
#include "net/partyhandler.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

extern ShortcutWindow *spellShortcutWindow;
extern std::string tradePartnerName;
extern QuitDialog *quitDialog;

namespace Actions
{

impHandler0(toggleChat)
{
    return chatWindow ? chatWindow->requestChatFocus() : false;
}

impHandler0(prevChatTab)
{
    if (chatWindow)
    {
        chatWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextChatTab)
{
    if (chatWindow)
    {
        chatWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(closeChatTab)
{
    if (chatWindow)
    {
        chatWindow->closeTab();
        return true;
    }
    return false;
}

impHandler0(closeAllChatTabs)
{
    if (chatWindow)
    {
        chatWindow->removeAllWhispers();
        chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler0(ignoreAllWhispers)
{
    if (chatWindow)
    {
        chatWindow->ignoreAllWhispers();
        chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler0(scrollChatUp)
{
    if (chatWindow && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(-DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

impHandler0(scrollChatDown)
{
    if (chatWindow && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

impHandler(msg)
{
    std::string recvnick;
    std::string msg;

    if (event.args.substr(0, 1) == "\"")
    {
        const size_t pos = event.args.find('"', 1);
        if (pos != std::string::npos)
        {
            recvnick = event.args.substr(1, pos - 1);
            if (pos + 2 < event.args.length())
                msg = event.args.substr(pos + 2, event.args.length());
        }
    }
    else
    {
        const size_t pos = event.args.find(" ");
        if (pos != std::string::npos)
        {
            recvnick = event.args.substr(0, pos);
            if (pos + 1 < event.args.length())
                msg = event.args.substr(pos + 1, event.args.length());
        }
        else
        {
            recvnick = std::string(event.args);
            msg.clear();
        }
    }

    trim(msg);

    if (msg.length() > 0)
    {
        std::string playerName = localPlayer->getName();
        std::string tempNick = recvnick;

        toLower(playerName);
        toLower(tempNick);

        if (tempNick.compare(playerName) == 0 || event.args.empty())
            return true;

        chatWindow->addWhisper(recvnick, msg, ChatMsgType::BY_PLAYER);
    }
    else
    {
        // TRANSLATORS: whisper send
        event.tab->chatLog(_("Cannot send empty whispers!"), ChatMsgType::BY_SERVER);
    }
    return true;
}

impHandler(query)
{
    if (chatWindow)
    {
        if (chatWindow->addWhisperTab(event.args, true))
        {
            chatWindow->saveState();
            return true;
        }
    }

    if (event.tab)
    {
        // TRANSLATORS: new whisper query
        event.tab->chatLog(strprintf(_("Cannot create a whisper tab for nick "
            "\"%s\"! It either already exists, or is you."),
            event.args.c_str()), ChatMsgType::BY_SERVER);
    }
    return true;
}

impHandler0(clearChatTab)
{
    if (chatWindow)
    {
        chatWindow->clearTab();
        return true;
    }
    return false;
}

impHandler(createParty)
{
    if (!event.tab)
        return false;

    if (event.args.empty())
    {
        // TRANSLATORS: create party message
        event.tab->chatLog(_("Party name is missing."), ChatMsgType::BY_SERVER);
    }
    else
    {
        Net::getPartyHandler()->create(event.args);
    }
    return true;
}

}  // namespace Actions
