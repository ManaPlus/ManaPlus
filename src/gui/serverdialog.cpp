/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/serverdialog.h"

#include "chatlogger.h"
#include "client.h"
#include "configuration.h"
#include "logger.h"
#include "main.h"

#include "gui/editserverdialog.h"
#include "gui/gui.h"
#include "gui/logindialog.h"
#include "gui/okdialog.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"

#include "gui/widgets/button.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"

#include "net/net.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"
#include "widgets/dropdown.h"

#include <guichan/font.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

#include "debug.h"

static const int MAX_SERVERLIST = 15;

static std::string serverTypeToString(ServerInfo::Type type)
{
    switch (type)
    {
        case ServerInfo::TMWATHENA:
            return "TmwAthena";
        case ServerInfo::EVOL:
            return "Evol";
#ifdef MANASERV_SUPPORT
        case ServerInfo::MANASERV:
            return "ManaServ";
#else
        case ServerInfo::MANASERV:
#endif
        default:
        case ServerInfo::UNKNOWN:
            return "";
    }
}

static unsigned short defaultPortForServerType(ServerInfo::Type type)
{
    switch (type)
    {
        default:
        case ServerInfo::UNKNOWN:
        case ServerInfo::TMWATHENA:
        case ServerInfo::EVOL:
#ifdef MANASERV_SUPPORT
            return 6901;
        case ServerInfo::MANASERV:
            return 9601;
#else
        case ServerInfo::MANASERV:
            return 6901;
#endif
    }
}

ServersListModel::ServersListModel(ServerInfos *servers, ServerDialog *parent):
        mServers(servers),
        mVersionStrings(servers->size(), VersionString(0, "")),
        mParent(parent)
{
}

int ServersListModel::getNumberOfElements()
{
    MutexLocker lock = mParent->lock();
    return static_cast<int>(mServers->size());
}

std::string ServersListModel::getElementAt(int elementIndex)
{
    MutexLocker lock = mParent->lock();
    const ServerInfo &server = mServers->at(elementIndex);
    std::string myServer;
    myServer += server.hostname;
//    myServer += ":";
//    myServer += toString(server.port);
    return myServer;
}

void ServersListModel::setVersionString(int index, const std::string &version)
{
    if (index >= (int)mVersionStrings.size())
        return;

    if (version.empty())
    {
        mVersionStrings[index] = VersionString(0, "");
    }
    else
    {
        int width = gui->getFont()->getWidth(version);
        mVersionStrings[index] = VersionString(width, version);
    }
}

class ServersListBox : public ListBox
{
public:
    ServersListBox(ServersListModel *model):
            ListBox(model)
    {
        mHighlightColor = Theme::getThemeColor(Theme::HIGHLIGHT);
        mTextColor = Theme::getThemeColor(Theme::TEXT);
        mNotSupportedColor = Theme::getThemeColor(
            Theme::SERVER_VERSION_NOT_SUPPORTED);
    }

    void draw(gcn::Graphics *graphics)
    {
        if (!mListModel)
            return;

        ServersListModel *model = static_cast<ServersListModel*>(mListModel);

        updateAlpha();

        mHighlightColor.a = static_cast<int>(mAlpha * 255.0f);
        graphics->setColor(mHighlightColor);
        graphics->setFont(getFont());

        const int height = getRowHeight();
        mNotSupportedColor.a = static_cast<int>(mAlpha * 255.0f);

        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(gcn::Rectangle(0,
                height * mSelected, getWidth(), height));
        }

        // Draw the list elements
        for (int i = 0, y = 0; i < model->getNumberOfElements();
             ++i, y += height)
        {
            ServerInfo info = model->getServer(i);

            graphics->setColor(mTextColor);

            int top;
            int x = 2;

            if (!info.name.empty())
            {
                graphics->setFont(boldFont);
                x += boldFont->getWidth(info.name) + 15;
                graphics->drawText(info.name, 2, y);
                top = y + boldFont->getHeight() + 2;
            }
            else
            {
                top = y + height / 4 + 2;
            }

            graphics->setFont(getFont());

            if (!info.description.empty())
                graphics->drawText(info.description, x, y);
            graphics->drawText(model->getElementAt(i), 2, top);

            if (info.version.first > 0)
            {
                graphics->setColor(mNotSupportedColor);

                graphics->drawText(info.version.second,
                                   getWidth() - info.version.first - 2, top);
            }
        }
    }

    unsigned int getRowHeight() const
    {
        return 2 * getFont()->getHeight() + 5;
    }
private:
    gcn::Color mHighlightColor;
    gcn::Color mTextColor;
    gcn::Color mNotSupportedColor;
};


ServerDialog::ServerDialog(ServerInfo *serverInfo, const std::string &dir):
    Window(_("Choose Your Server"), false, nullptr, "server.xml"),
    mDir(dir),
//    mDownloadStatus(DOWNLOADING_PREPARING),
    mDownloadStatus(DOWNLOADING_UNKNOWN),
    mDownload(nullptr),
    mDownloadProgress(-1.0f),
    mServers(ServerInfos()),
    mServerInfo(serverInfo),
    mPersistentIPCheckBox(nullptr)
{
    if (isSafeMode)
        setCaption(_("Choose Your Server  *** SAFE MODE ***"));

    setWindowName("ServerDialog");

    mPersistentIPCheckBox = new CheckBox(_("Use same ip for game sub servers"),
                                       config.getBoolValue("usePersistentIP"),
                                       this, "persitent ip");

    loadCustomServers();

    mServersListModel = new ServersListModel(&mServers, this);

    mServersList = new ServersListBox(mServersListModel);
    mServersList->addMouseListener(this);

    ScrollArea *usedScroll = new ScrollArea(mServersList);
    usedScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    int n = 0;

    mDescription = new Label(std::string());

    mQuitButton = new Button(_("Quit"), "quit", this);
    mLoadButton = new Button(_("Load"), "load", this);
    mConnectButton = new Button(_("Connect"), "connect", this);
    mAddEntryButton = new Button(_("Add"), "addEntry", this);
    mEditEntryButton = new Button(_("Edit"), "editEntry", this);
    mDeleteButton = new Button(_("Delete"), "remove", this);

    mServersList->addSelectionListener(this);
    usedScroll->setVerticalScrollAmount(0);

    place(0, 0 + n, usedScroll, 7, 5).setPadding(3);
    place(0, 5 + n, mDescription, 7);
    place(0, 6 + n, mPersistentIPCheckBox, 7);
    place(0, 7 + n, mAddEntryButton);
    place(1, 7 + n, mEditEntryButton);
    place(2, 7 + n, mLoadButton);
    place(3, 7 + n, mDeleteButton);
    place(5, 7 + n, mQuitButton);
    place(6, 7 + n, mConnectButton);

    // Make sure the list has enough height
    getLayout().setRowHeight(0, 80);

    // Do this manually instead of calling reflowLayout so we can enforce a
    // minimum width.
    int width = 0, height = 0;
    getLayout().reflow(width, height);
    if (width < 400)
    {
        width = 400;
        getLayout().reflow(width, height);
    }

    setContentSize(width, height);

    setMinWidth(getWidth());
    setMinHeight(getHeight());
    setDefaultSize(getWidth(), getHeight(), ImageRect::CENTER);

    setResizable(true);
    addKeyListener(this);

    loadWindowState();

    setFieldsReadOnly(true);
    mServersList->setSelected(0); // Do this after for the Delete button
    setVisible(true);

    mConnectButton->requestFocus();

    loadServers(true);

    if (mServers.empty())
        downloadServerList();
}

ServerDialog::~ServerDialog()
{
    if (mDownload)
    {
        mDownload->cancel();
        delete mDownload;
        mDownload = nullptr;
    }
    delete mServersListModel;
    mServersListModel = nullptr;
}

void ServerDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "connect")
    {
        if (Client::getState() == STATE_CONNECT_SERVER)
            return;

        if (mDownload)
            mDownload->cancel();

        mQuitButton->setEnabled(false);
        mConnectButton->setEnabled(false);
        mLoadButton->setEnabled(false);

        int index = mServersList->getSelected();
        if (index < 0)
            return;

        ServerInfo server = mServers.at(index);
        mServerInfo->hostname = server.hostname;
        mServerInfo->port = server.port;
        mServerInfo->type = server.type;
        mServerInfo->name = server.name;
        mServerInfo->description = server.description;
        mServerInfo->save = true;

        if (chatLogger)
            chatLogger->setServerName(mServerInfo->hostname);

        saveCustomServers(*mServerInfo);

        if (!LoginDialog::savedPasswordKey.empty())
        {
            if (mServerInfo->hostname != LoginDialog::savedPasswordKey)
                LoginDialog::savedPassword = "";
        }

        config.setValue("usePersistentIP",
            mPersistentIPCheckBox->isSelected());
        Client::setState(STATE_CONNECT_SERVER);
    }
    else if (event.getId() == "quit")
    {
        if (mDownload)
            mDownload->cancel();
        Client::setState(STATE_FORCE_QUIT);
    }
    else if (event.getId() == "load")
    {
        downloadServerList();
    }
    else if (event.getId() == "addEntry")
    {
        new EditServerDialog(this, ServerInfo(), -1);
    }
    else if (event.getId() == "editEntry")
    {
        int index = mServersList->getSelected();
        if (index >= 0)
            new EditServerDialog(this, mServers.at(index), index);
    }
    else if (event.getId() == "remove")
    {
        int index = mServersList->getSelected();
        if (index >= 0)
        {
            mServersList->setSelected(0);
            mServers.erase(mServers.begin() + index);
            saveCustomServers();
        }
    }
}

void ServerDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ESCAPE)
        Client::setState(STATE_EXIT);
    else if (key.getValue() == Key::ENTER)
        action(gcn::ActionEvent(nullptr, mConnectButton->getActionEventId()));
}

void ServerDialog::valueChanged(const gcn::SelectionEvent &)
{
    const int index = mServersList->getSelected();
    if (index == -1)
    {
        mDeleteButton->setEnabled(false);
        return;
    }

    // Update the server and post fields according to the new selection
    const ServerInfo &myServer = mServersListModel->getServer(index);
    mDescription->setCaption(myServer.description);
    setFieldsReadOnly(true);

    mDeleteButton->setEnabled(myServer.save);
}

void ServerDialog::mouseClicked(gcn::MouseEvent &mouseEvent)
{
    if (mouseEvent.getClickCount() == 2 &&
        mouseEvent.getSource() == mServersList)
    {
        action(gcn::ActionEvent(mConnectButton,
                                mConnectButton->getActionEventId()));
    }
}

void ServerDialog::logic()
{
    {
        MutexLocker tempLock(&mMutex);
        if (mDownloadStatus == DOWNLOADING_COMPLETE)
        {
            mDownloadStatus = DOWNLOADING_OVER;

            mDescription->setCaption(std::string());
        }
        else if (mDownloadStatus == DOWNLOADING_IN_PROGRESS)
        {
            mDescription->setCaption(strprintf(_("Downloading server list..."
                "%2.2f%%"), static_cast<double>(mDownloadProgress * 100)));
        }
        else if (mDownloadStatus == DOWNLOADING_IDLE)
        {
            mDescription->setCaption(_("Waiting for server..."));
        }
        else if (mDownloadStatus == DOWNLOADING_PREPARING)
        {
            mDescription->setCaption(_("Preparing download"));
        }
        else if (mDownloadStatus == DOWNLOADING_ERROR)
        {
            mDescription->setCaption(_("Error retreiving server list!"));
        }
    }

    Window::logic();
}

void ServerDialog::setFieldsReadOnly(bool readOnly)
{
    if (!readOnly)
    {
        mServersList->setSelected(-1);
        mDescription->setCaption(std::string());
    }

    mAddEntryButton->setEnabled(readOnly);
    mDeleteButton->setEnabled(false);
    mLoadButton->setEnabled(readOnly);
    mDescription->setVisible(readOnly);
}

void ServerDialog::downloadServerList()
{
    // Try to load the configuration value for the onlineServerList
    std::string listFile = branding.getStringValue("onlineServerList");

    if (listFile.empty())
        listFile = config.getStringValue("onlineServerList");

    // Fall back to manaplus.evolonline.org when neither branding nor config set it
    if (listFile.empty())
    {
        listFile = "http://manaplus.evolonline.org/"
            "serverlist.xml/serverlist.xml";
    }

    if (mDownload)
    {
        mDownload->cancel();
        delete mDownload;
        mDownload = nullptr;
    }

    mDownload = new Net::Download(this, listFile, &downloadUpdate);
    mDownload->setFile(mDir + "/serverlist.xml");
    mDownload->start();
}

void ServerDialog::loadServers(bool addNew)
{
    XML::Document doc(mDir + "/serverlist.xml", false);
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "serverlist"))
    {
        logger->log1("Error loading server list!");
        return;
    }

    int ver = XML::getProperty(rootNode, "version", 0);
    if (ver != 1)
    {
        logger->log("Error: unsupported online server list version: %d",
                    ver);
        return;
    }

    for_each_xml_child_node(serverNode, rootNode)
    {
        if (!xmlNameEqual(serverNode, "server"))
            continue;

        ServerInfo server;

        std::string type = XML::getProperty(serverNode, "type", "unknown");

        server.type = ServerInfo::parseType(type);

        // Ignore unknown server types
        if (server.type == ServerInfo::UNKNOWN)
        {
            logger->log("Ignoring server entry with unknown type: %s",
                        type.c_str());
            continue;
        }

        server.name = XML::getProperty(serverNode, "name", std::string());

        std::string version = XML::getProperty(serverNode, "minimumVersion",
                                               std::string());

        bool meetsMinimumVersion = (compareStrI(version, PACKAGE_VERSION)
                                   <= 0);

        // For display in the list
        if (meetsMinimumVersion)
            version.clear();
        else if (version.empty())
            version = _("requires a newer version");
        else
            version = strprintf(_("requires v%s"), version.c_str());

        for_each_xml_child_node(subNode, serverNode)
        {
            if (xmlNameEqual(subNode, "connection"))
            {
                server.hostname = XML::getProperty(subNode, "hostname", "");
                server.port = static_cast<short unsigned>(
                    XML::getProperty(subNode, "port", 0));

                if (server.port == 0)
                {
                    // If no port is given, use the default for the given type
                    server.port = defaultPortForServerType(server.type);
                }
            }
            else if (xmlNameEqual(subNode, "description"))
            {
                server.description = reinterpret_cast<const char*>(
                    subNode->xmlChildrenNode->content);
            }
        }

        server.version.first = gui->getFont()->getWidth(version);
        server.version.second = version;

        MutexLocker tempLock(&mMutex);
        // Add the server to the local list if it's not already present
        bool found = false;
        for (unsigned int i = 0; i < mServers.size(); i++)
        {
            if (mServers[i] == server)
            {
                // Use the name listed in the server list
                mServers[i].name = server.name;
                mServers[i].version = server.version;
                mServers[i].description = server.description;
                mServersListModel->setVersionString(i, version);
                found = true;
                break;
            }
        }
        if (!found && addNew)
            mServers.push_back(server);
    }
}

void ServerDialog::loadCustomServers()
{
    for (int i = 0; i < MAX_SERVERLIST; ++i)
    {
        const std::string index = toString(i);
        const std::string nameKey = "MostUsedServerDescName" + index;
        const std::string descKey = "MostUsedServerDescription" + index;
        const std::string hostKey = "MostUsedServerName" + index;
        const std::string typeKey = "MostUsedServerType" + index;
        const std::string portKey = "MostUsedServerPort" + index;

        ServerInfo server;
        server.name = config.getValue(nameKey, "");
        server.description = config.getValue(descKey, "");
        server.hostname = config.getValue(hostKey, "");
        server.type = ServerInfo::parseType(config.getValue(typeKey, ""));

        const int defaultPort = defaultPortForServerType(server.type);
        server.port = static_cast<unsigned short>(
            config.getValue(portKey, defaultPort));

        // skip invalid server
        if (!server.isValid())
            continue;

        server.save = true;
        mServers.push_back(server);
    }
}

void ServerDialog::saveCustomServers(const ServerInfo &currentServer,
                                     int index)
{
    // Make sure the current server is mentioned first
    if (currentServer.isValid())
    {
        if (index >= 0 && (unsigned)index < mServers.size())
        {
            mServers[index] = currentServer;
        }
        else
        {
            ServerInfos::iterator i, i_end = mServers.end();
            for (i = mServers.begin(); i != i_end; ++i)
            {
                if (*i == currentServer)
                {
                    mServers.erase(i);
                    break;
                }
            }
            mServers.insert(mServers.begin(), currentServer);
        }
    }

    int savedServerCount = 0;

    for (unsigned i = 0;
         i < mServers.size() && savedServerCount < MAX_SERVERLIST; ++i)
    {
        const ServerInfo &server = mServers.at(i);

        // Only save servers that were loaded from settings
        if (!(server.save && server.isValid()))
            continue;

        const std::string num = toString(savedServerCount);
        const std::string nameKey = "MostUsedServerDescName" + num;
        const std::string descKey = "MostUsedServerDescription" + num;
        const std::string hostKey = "MostUsedServerName" + num;
        const std::string typeKey = "MostUsedServerType" + num;
        const std::string portKey = "MostUsedServerPort" + num;

        config.setValue(nameKey, toString(server.name));
        config.setValue(descKey, toString(server.description));
        config.setValue(hostKey, toString(server.hostname));
        config.setValue(typeKey, serverTypeToString(server.type));
        config.setValue(portKey, toString(server.port));
        ++ savedServerCount;
    }

    // Insert an invalid entry at the end to make the loading stop there
    if (savedServerCount < MAX_SERVERLIST)
        config.setValue("MostUsedServerName" + toString(savedServerCount), "");
}

int ServerDialog::downloadUpdate(void *ptr, DownloadStatus status,
                                 size_t total, size_t remaining)
{
    if (status == DOWNLOAD_STATUS_CANCELLED)
        return -1;

    ServerDialog *sd = reinterpret_cast<ServerDialog*>(ptr);
    bool finished = false;

    if (!sd->mDownload)
        return -1;

    if (status == DOWNLOAD_STATUS_COMPLETE)
    {
        finished = true;
    }
    else if (status < 0)
    {
        logger->log("Error retreiving server list: %s\n",
                    sd->mDownload->getError());
        sd->mDownloadStatus = DOWNLOADING_ERROR;
    }
    else
    {
        float progress = static_cast<float>(remaining);
        if (total)
            progress /= static_cast<float>(total);

        if (progress != progress)
        {
            progress = 0.0f; // check for NaN
        }
        else if (progress < 0.0f)
        {
            progress = 0.0f; // no idea how this could ever happen,
                             // but why not check for it anyway.
        }
        else if (progress > 1.0f)
        {
            progress = 1.0f;
        }

        MutexLocker lock(&sd->mMutex);
        sd->mDownloadStatus = DOWNLOADING_IN_PROGRESS;
        sd->mDownloadProgress = progress;
    }

    if (finished)
    {
        sd->loadServers();

        MutexLocker lock(&sd->mMutex);
        sd->mDownloadStatus = DOWNLOADING_COMPLETE;
    }

    return 0;
}

void ServerDialog::updateServer(ServerInfo server, int index)
{
    saveCustomServers(server, index);
}
