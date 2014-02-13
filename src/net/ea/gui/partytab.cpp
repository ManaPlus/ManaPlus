/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "net/ea/gui/partytab.h"

#include "chatlogger.h"
#include "commands.h"
#include "configuration.h"
#include "party.h"
#include "soundconsts.h"
#include "soundmanager.h"

#include "being/localplayer.h"

#include "net/net.h"
#include "net/partyhandler.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

PartyTab::PartyTab(const Widget2 *const widget) :
    // TRANSLATORS: party chat tab name
    ChatTab(widget, _("Party"), "")
{
    setTabColor(&getThemeColor(Theme::PARTY_CHAT_TAB),
        &getThemeColor(Theme::PARTY_CHAT_TAB_OUTLINE));
    setHighlightedTabColor(&getThemeColor(Theme::PARTY_CHAT_TAB_HIGHLIGHTED),
        &getThemeColor(Theme::PARTY_CHAT_TAB_HIGHLIGHTED_OUTLINE));
    setSelectedTabColor(&getThemeColor(Theme::PARTY_CHAT_TAB_SELECTED),
        &getThemeColor(Theme::PARTY_CHAT_TAB_SELECTED_OUTLINE));

    mShowOnline = config.getBoolValue("showPartyOnline");
    config.addListener("showPartyOnline", this);
}

PartyTab::~PartyTab()
{
    config.removeListeners(this);
    CHECKLISTENERS
}

void PartyTab::handleInput(const std::string &msg)
{
    Net::getPartyHandler()->chat(ChatWindow::doReplace(msg));
}

bool PartyTab::handleCommand(const std::string &restrict type,
                             const std::string &restrict args)
{
    if (type == "create" || type == "new")
    {
        if (args.empty())
        {
            // TRANSLATORS: chat error message
            chatLog(_("Party name is missing."), BY_SERVER);
        }
        else
        {
            Net::getPartyHandler()->create(args);
        }
    }
    else if (type == "invite")
    {
        Net::getPartyHandler()->invite(args);
    }
    else if (type == "leave")
    {
        Net::getPartyHandler()->leave();
    }
    else if (type == "kick")
    {
        Net::getPartyHandler()->kick(args);
    }
    else if (type == "item")
    {
        if (args.empty())
        {
            switch (Net::getPartyHandler()->getShareItems())
            {
                case PARTY_SHARE:
                    // TRANSLATORS: chat message
                    chatLog(_("Item sharing enabled."), BY_SERVER);
                    return true;
                case PARTY_SHARE_NO:
                    // TRANSLATORS: chat message
                    chatLog(_("Item sharing disabled."), BY_SERVER);
                    return true;
                case PARTY_SHARE_NOT_POSSIBLE:
                    // TRANSLATORS: chat message
                    chatLog(_("Item sharing not possible."), BY_SERVER);
                    return true;
                case PARTY_SHARE_UNKNOWN:
                    // TRANSLATORS: chat message
                    chatLog(_("Item sharing unknown."), BY_SERVER);
                    return true;
                default:
                    break;
            }
        }

        const signed char opt = parseBoolean(args);

        switch (opt)
        {
            case 1:
                Net::getPartyHandler()->setShareItems(PARTY_SHARE);
                break;
            case 0:
                Net::getPartyHandler()->setShareItems(PARTY_SHARE_NO);
                break;
            case -1:
                chatLog(strprintf(BOOLEAN_OPTIONS, "item"));
                break;
            default:
                break;
        }
    }
    else if (type == "exp")
    {
        if (args.empty())
        {
            switch (Net::getPartyHandler()->getShareExperience())
            {
                case PARTY_SHARE:
                    // TRANSLATORS: chat message
                    chatLog(_("Experience sharing enabled."), BY_SERVER);
                    return true;
                case PARTY_SHARE_NO:
                    // TRANSLATORS: chat message
                    chatLog(_("Experience sharing disabled."), BY_SERVER);
                    return true;
                case PARTY_SHARE_NOT_POSSIBLE:
                    // TRANSLATORS: chat message
                    chatLog(_("Experience sharing not possible."), BY_SERVER);
                    return true;
                case PARTY_SHARE_UNKNOWN:
                    // TRANSLATORS: chat message
                    chatLog(_("Experience sharing unknown."), BY_SERVER);
                    return true;
                default:
                    break;
            }
        }

        const signed char opt = parseBoolean(args);

        switch (opt)
        {
            case 1:
                Net::getPartyHandler()->setShareExperience(PARTY_SHARE);
                break;
            case 0:
                Net::getPartyHandler()->setShareExperience(PARTY_SHARE_NO);
                break;
            case -1:
                chatLog(strprintf(BOOLEAN_OPTIONS, "exp"));
                break;
            default:
                break;
        }
    }
    else
    {
        return false;
    }

    return true;
}

void PartyTab::getAutoCompleteList(StringVect &names) const
{
    if (!player_node)
        return;

    const Party *const p = player_node->getParty();

    if (p)
        p->getNames(names);
}

void PartyTab::getAutoCompleteCommands(StringVect &names) const
{
    names.push_back("/help");
    names.push_back("/invite ");
    names.push_back("/leave");
    names.push_back("/kick ");
    names.push_back("/item");
    names.push_back("/exp");
}

void PartyTab::saveToLogFile(const std::string &msg) const
{
    if (chatLogger)
        chatLogger->log("#Party", msg);
}

void PartyTab::playNewMessageSound() const
{
    soundManager.playGuiSound(SOUND_GUILD);
}

void PartyTab::optionChanged(const std::string &value)
{
    if (value == "showPartyOnline")
        mShowOnline = config.getBoolValue("showPartyOnline");
}

}  // namespace Ea
