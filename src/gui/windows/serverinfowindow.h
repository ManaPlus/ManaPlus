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

#ifndef GUI_WINDOWS_SERVERINFOWINDOW_H
#define GUI_WINDOWS_SERVERINFOWINDOW_H

#include "gui/widgets/window.h"

#include "gui/widgets/linkhandler.h"

#include "listeners/actionlistener.h"

#include "net/serverinfo.h"

class BrowserBox;
class ScrollArea;
class ServerInfo;

class ServerInfoWindow final : public Window,
                               public LinkHandler,
                               public ActionListener
{
    public:
        ServerInfoWindow(ServerInfo &serverInfo);

        A_DELETE_COPY(ServerInfoWindow)

        void postInit() override final;

        ~ServerInfoWindow();

        void action(const ActionEvent &event) override final;

        void handleLink(const std::string &link,
                        MouseEvent *const event A_UNUSED) override final;

    protected:
        void showServerInfo();

        void addSourcesList(const std::vector<ServerUrlInfo> &list,
                            const std::string &comment);

        void addServerComment(const std::string &url,
                              const std::string &comment);

    private:
        ServerInfo mServerInfo;
        BrowserBox *mBrowserBox A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
};

#endif  // GUI_WINDOWS_SERVERINFOWINDOW_H
