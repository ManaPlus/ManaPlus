/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/widgets/itemlinkhandler.h"

#include "gui/viewport.h"

#include "gui/popups/itempopup.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/helpwindow.h"

#include "utils/gettext.h"
#include "utils/process.h"
#include "utils/stringutils.h"

#include "resources/db/itemdb.h"

#include <string>

#include <guichan/actionlistener.hpp>
#include <guichan/mouseinput.hpp>

#include "debug.h"

namespace
{
    struct OpenUrlListener : public gcn::ActionListener
    {
        OpenUrlListener() :
            url()
        {
        }

        A_DELETE_COPY(OpenUrlListener)

        void action(const gcn::ActionEvent &event) override final
        {
            if (event.getId() == "yes")
                openBrowser(url);
        }

        std::string url;
    } listener;
}  // namespace

ItemLinkHandler::ItemLinkHandler() :
    mItemPopup(new ItemPopup)
{
    mItemPopup->postInit();
}

ItemLinkHandler::~ItemLinkHandler()
{
    delete mItemPopup;
    mItemPopup = nullptr;
}

void ItemLinkHandler::handleLink(const std::string &link,
                                 gcn::MouseEvent *event)
{
    if (strStartWith(link, "http://") || strStartWith(link, "https://"))
    {
        if (!event)
            return;
        std::string url = link;
        replaceAll(url, " ", "");
        listener.url = url;
        const int button = event->getButton();
        if (button == gcn::MouseInput::LEFT)
        {
            ConfirmDialog *const confirmDlg = new ConfirmDialog(
                // TRANSLATORS: dialog message
                _("Open url"), url, SOUND_REQUEST, false, true);
            confirmDlg->postInit();
            confirmDlg->addActionListener(&listener);
        }
        else if (button == gcn::MouseInput::RIGHT)
        {
            if (viewport)
                viewport->showLinkPopup(url);
        }
    }
    else if (!link.empty() && link[0] == '?')
    {
        if (helpWindow)
        {
            helpWindow->search(link.substr(1));
            helpWindow->requestMoveToTop();
        }
    }
    else if (strStartWith(link, "help://"))
    {
        if (helpWindow)
        {
            helpWindow->loadHelp(link.substr(7));
            helpWindow->requestMoveToTop();
        }
    }
    else
    {
        if (!mItemPopup || link.empty())
            return;

        const char ch = link[0];
        if (ch < '0' || ch > '9')
            return;

        std::vector<int> str;
        splitToIntVector(str, link, ',');
        if (str.empty())
            return;
        int color = 1;
        if (str.size() > 1)
            color = str[1];
        const int id = str[0];
        if (id > 0)
        {
            const ItemInfo &itemInfo = ItemDB::get(id);
            mItemPopup->setItem(itemInfo, color, true);
            if (mItemPopup->isPopupVisible())
            {
                mItemPopup->setVisible(false);
            }
            else if (viewport)
            {
                mItemPopup->position(viewport->getMouseX(),
                    viewport->getMouseY());
            }
        }
    }
}
