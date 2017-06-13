/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#include "gui/windows/serverinfowindow.h"

#include "enums/gui/layouttype.h"

#include "gui/gui.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "utils/gettext.h"
#include "utils/process.h"
#include "utils/stringutils.h"

#include "debug.h"

ServerInfoWindow::ServerInfoWindow(ServerInfo &serverInfo) :
    // TRANSLATORS: servers dialog name
    Window(_("Server info"), Modal_false, nullptr, "server_info.xml"),
    LinkHandler(),
    ActionListener(),
    mServerInfo(serverInfo),
    mBrowserBox(new BrowserBox(this, BrowserBoxMode::AUTO_SIZE, Opaque_true,
        "browserbox.xml")),
    mScrollArea(new ScrollArea(this, mBrowserBox,
        Opaque_true, "serverinfo_background.xml"))
{
    setMinWidth(300);
    setMinHeight(220);
    setContentSize(455, 350);
    setWindowName("ServerInfoWindow");
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);

    setDefaultSize(500, 400, ImagePosition::CENTER);

    mBrowserBox->setOpaque(Opaque_false);
    mBrowserBox->setLinkHandler(this);
    if (gui != nullptr)
        mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVars(true);
    mBrowserBox->setEnableTabs(true);

    place(0, 0, mScrollArea, 5, 3).setPadding(3);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    loadWindowState();
    enableVisibleSound(true);
    widgetResized(Event(nullptr));
}

void ServerInfoWindow::postInit()
{
    Window::postInit();
    setVisible(Visible_true);
    showServerInfo();
}

ServerInfoWindow::~ServerInfoWindow()
{
}

void ServerInfoWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
    }
}

void ServerInfoWindow::handleLink(const std::string &link,
                                  MouseEvent *const event A_UNUSED)
{
    if (strStartWith(link, "http://") ||
        strStartWith(link, "https://"))
    {
        openBrowser(link);
    }
}

void ServerInfoWindow::showServerInfo()
{
    const ServerFreeTypeT type = mServerInfo.freeType;
    if (type == ServerFreeType::Free)
    {
        // TRANSLATORS: server info comment
        mBrowserBox->addRow(_("##BServer with free license."));
    }
    else if (type == ServerFreeType::NonFree)
    {
        // TRANSLATORS: server info comment
        mBrowserBox->addRow(_("##BServer with non free license."));
    }
    else if (type == ServerFreeType::Unknown ||
             type == ServerFreeType::NotSet)
    {
        // TRANSLATORS: server info comment
        mBrowserBox->addRow(_("##BServer unknown licesne."));
    }
    mBrowserBox->addRow("");
    addServerComment(mServerInfo.serverUrl,
        // TRANSLATORS: server info comment
        _("Server"));
    addServerComment(mServerInfo.supportUrl,
        // TRANSLATORS: server info comment
        _("Support"));
    addServerComment(mServerInfo.registerUrl,
        // TRANSLATORS: server info comment
        _("Register"));

    addSourcesList(mServerInfo.nonFreeSources,
        // TRANSLATORS: server info non free comment
        _("##BNon free sources"));
    addSourcesList(mServerInfo.freeSources,
        // TRANSLATORS: server info non free comment
        _("##BFree sources"));
}

void ServerInfoWindow::addSourcesList(const std::vector<ServerUrlInfo> &list,
                                      const std::string &comment)
{
    if (!list.empty())
    {
        mBrowserBox->addRow("");
        mBrowserBox->addRow(comment);
        FOR_EACH (std::vector<ServerUrlInfo>::const_iterator, it, list)
        {
            const ServerUrlInfo &info = *it;
            addServerComment(info.url,
                info.name);
        }
    }
}

void ServerInfoWindow::addServerComment(const std::string &url,
                                        const std::string &comment)
{
    std::string str;
    if (!url.empty())
    {
        if (strStartWith(url, "http://") ||
            strStartWith(url, "https://"))
        {
            str = strprintf("%s: @@%s|@@##0",
                comment.c_str(),
                url.c_str());
        }
        else
        {
            str = strprintf("%s: %s",
                comment.c_str(),
                url.c_str());
        }
        mBrowserBox->addRow(str);
    }
}
