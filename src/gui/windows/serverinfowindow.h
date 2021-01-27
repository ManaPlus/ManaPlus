/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/serverinfo.h"

class ScrollArea;
class ServerInfo;
class StaticBrowserBox;

class ServerInfoWindow final : public Window,
                               public LinkHandler
{
    public:
        explicit ServerInfoWindow(ServerInfo &serverInfo);

        A_DELETE_COPY(ServerInfoWindow)

        void postInit() override final;

        ~ServerInfoWindow() override final;

        void handleLink(const std::string &link,
                        MouseEvent *const event A_UNUSED) override final;

        void close() override final;

    protected:
        void showServerInfo();

        void addSourcesList(const STD_VECTOR<ServerUrlInfo> &list,
                            const std::string &comment);

        void addServerComment(const std::string &url,
                              const std::string &comment);

    private:
        ServerInfo mServerInfo;
        StaticBrowserBox *mBrowserBox A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
};

extern ServerInfoWindow *serverInfoWindow;

#endif  // GUI_WINDOWS_SERVERINFOWINDOW_H
