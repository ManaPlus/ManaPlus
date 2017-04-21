/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/widgets/itemlinkhandler.h"

#include "itemcolormanager.h"
#include "settings.h"

#include "gui/viewport.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"

#include "gui/widgets/createwidget.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/questswindow.h"

#include "input/inputmanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/db/itemdb.h"

#include "listeners/inputactionremotelistener.h"
#include "listeners/openurllistener.h"

#include "debug.h"

namespace
{
    OpenUrlListener listener;
}  // namespace

ItemLinkHandler::ItemLinkHandler() :
    LinkHandler(),
    mAllowCommands(true)
{
}

ItemLinkHandler::~ItemLinkHandler()
{
}

void ItemLinkHandler::handleCommandLink(const std::string &link,
                                        const std::string &prefix)
{
    std::string cmd;
    std::string args;

    const std::string cmdStr = link.substr(prefix.size());
    if (!parse2Str(cmdStr, cmd, args))
    {
        cmd = cmdStr;
        args.clear();
    }
    if (mAllowCommands)
    {
        inputManager.executeRemoteChatCommand(cmd, args, nullptr);
    }
    else
    {
        inputActionRemoteListener.setCommand(cmd, args);
        ConfirmDialog *const confirmDlg = CREATEWIDGETR(ConfirmDialog,
            // TRANSLATORS: dialog message
            _("Run command"),
            strprintf("/%s %s", cmd.c_str(), args.c_str()),
            SOUND_REQUEST,
            false,
            Modal_true);
        confirmDlg->addActionListener(&inputActionRemoteListener);
    }
}

void ItemLinkHandler::handleHelpLink(const std::string &link)
{
    if (helpWindow)
    {
        helpWindow->loadHelp(link.substr(7));
        helpWindow->requestMoveToTop();
    }
}

void ItemLinkHandler::handleHttpLink(const std::string &link,
                                     const MouseEvent *const event)
{
    if (!event)
        return;
    std::string url = link;
    replaceAll(url, " ", "");
    listener.url = url;
    const MouseButtonT button = event->getButton();
    if (button == MouseButton::LEFT)
    {
        ConfirmDialog *const confirmDlg = CREATEWIDGETR(ConfirmDialog,
            // TRANSLATORS: dialog message
            _("Open url"),
            url,
            SOUND_REQUEST,
            false,
            Modal_true);
        confirmDlg->addActionListener(&listener);
    }
    else if (button == MouseButton::RIGHT)
    {
        if (popupMenu)
            popupMenu->showLinkPopup(url);
    }
}

void ItemLinkHandler::handleItemLink(const std::string &link)
{
    if (!itemPopup || link.empty())
        return;

    const char ch = link[0];
    if (ch < '0' || ch > '9')
        return;

    std::vector<int> str;
    splitToIntVector(str, link, ',');
    if (str.empty())
        return;

    const int id = str[0];

    if (id > 0)
    {
        str.erase(str.begin());
        while (str.size() < maxCards)
            str.push_back(0);
        const ItemColor color =
            ItemColorManager::getColorFromCards(&str[0]);

        const ItemInfo &itemInfo = ItemDB::get(id);
        // +++ need add support for options.
        itemPopup->setItem(itemInfo, color, true, -1, &str[0], nullptr);
        if (itemPopup->isPopupVisible())
        {
            itemPopup->setVisible(Visible_false);
        }
        else if (viewport)
        {
            itemPopup->position(viewport->mMouseX,
                viewport->mMouseY);
        }
    }
}

void ItemLinkHandler::handleSearchLink(const std::string &link)
{
    if (helpWindow)
    {
        helpWindow->search(link.substr(1));
        helpWindow->requestMoveToTop();
    }
}

void ItemLinkHandler::handleLink(const std::string &link,
                                 MouseEvent *const event)
{
    if (link.empty())
        return;

    if (strStartWith(link, "http://") || strStartWith(link, "https://"))
    {
        handleHttpLink(link, event);
    }
    else if (link[0] == '?')
    {
        handleSearchLink(link);
    }
    else if (strStartWith(link, "help://"))
    {
        handleHelpLink(link);
    }
    else if (strStartWith(link, settings.linkCommandSymbol))
    {
        handleCommandLink(link, settings.linkCommandSymbol);
    }
    else if (strStartWith(link, "="))
    {
        handleCommandLink(link, "=");
    }
    else if (link == "news")
    {
        if (helpWindow)
            helpWindow->loadHelpSimple("news");
    }
    else if (link == "copyright")
    {
        inputManager.executeAction(InputAction::WINDOW_ABOUT);
    }
    else if (link[0] == 'q')
    {
        questsWindow->selectQuest(atoi(link.substr(1).c_str()));
    }
    else
    {
        handleItemLink(link);
    }
}
