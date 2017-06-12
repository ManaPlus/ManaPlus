/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_SERVERDIALOG_H
#define GUI_WINDOWS_SERVERDIALOG_H

#include "gui/widgets/window.h"

#include "enums/gui/serverdialogdonwloadstatus.h"

#include "enums/net/downloadstatus.h"

#include "net/serverinfo.h"

#include "utils/mutex.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"
#include "listeners/selectionlistener.h"

class Button;
class CheckBox;
class Label;
class ListBox;
class ServersListModel;

namespace Net
{
    class Download;
}  // namespace Net

/**
 * The server choice dialog.
 *
 * \ingroup Interface
 */
class ServerDialog final : public Window,
                           public ActionListener,
                           public KeyListener,
                           public SelectionListener
{
    public:
        /**
         * Constructor
         *
         * @see Window::Window
         */
        ServerDialog(ServerInfo *const serverInfo, const std::string &dir);

        A_DELETE_COPY(ServerDialog)

        void postInit() override final;

        /**
         * Destructor
         */
        ~ServerDialog();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        void keyPressed(KeyEvent &event) override final;

        /**
         * Called when the selected value changed in the servers list box.
         */
        void valueChanged(const SelectionEvent &event) override final;

        void mouseClicked(MouseEvent &event) override final;

        void logic() override final;

        void updateServer(const ServerInfo &server, const int index);

        void connectToSelectedServer();

        void close() override final;

    protected:
        friend class ServersListModel;

        MutexLocker lock()
        { return MutexLocker(&mMutex); }

    private:
        friend class EditServerDialog;

        /**
         * Called to load a list of available server from an online xml file.
         */
        void downloadServerList();

        void loadServers(const bool addNew = true);

        void loadCustomServers();

        void saveCustomServers(const ServerInfo &currentServer = ServerInfo(),
                               const int index = -1);

        bool needUpdateServers() const;

        static int downloadUpdate(void *ptr,
                                  const DownloadStatusT status,
                                  size_t total,
                                  const size_t remaining);

        Mutex mMutex;
        ServerInfos mServers;
        const std::string &mDir;
        Label  *mDescription A_NONNULLPOINTER;
        Button *mQuitButton A_NONNULLPOINTER;
        Button *mConnectButton A_NONNULLPOINTER;
        Button *mAddEntryButton A_NONNULLPOINTER;
        Button *mEditEntryButton A_NONNULLPOINTER;
        Button *mDeleteButton A_NONNULLPOINTER;
        Button *mLoadButton A_NONNULLPOINTER;
        Button *mInfoButton A_NONNULLPOINTER;
        ServersListModel *mServersListModel A_NONNULLPOINTER;
        ListBox *mServersList A_NONNULLPOINTER;

        /** Status of the current download. */
        Net::Download *mDownload;
        ServerInfo *mServerInfo;
        CheckBox *mPersistentIPCheckBox;
        float mDownloadProgress;
        ServerDialogDownloadStatusT mDownloadStatus;
};

#endif  // GUI_WINDOWS_SERVERDIALOG_H
