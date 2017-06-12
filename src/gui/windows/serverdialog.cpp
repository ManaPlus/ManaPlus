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

#include "gui/windows/serverdialog.h"

#include "chatlogger.h"
#include "client.h"
#include "configuration.h"
#include "main.h"
#include "settings.h"

#include "net/download.h"

#include "fs/paths.h"

#include "gui/widgets/checkbox.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/desktop.h"
#include "gui/windows/editserverdialog.h"
#include "gui/windows/logindialog.h"
#include "gui/windows/serverinfowindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/serverslistbox.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/langs.h"

#include "debug.h"

#ifdef WIN32
#undef ERROR
#endif  // WIN32

static const int MAX_SERVERLIST = 15;

static std::string serverTypeToString(const ServerTypeT type)
{
    switch (type)
    {
        case ServerType::TMWATHENA:
#ifdef TMWA_SUPPORT
            return "TmwAthena";
#else  // TMWA_SUPPORT

            return "";
#endif  // TMWA_SUPPORT

        case ServerType::EATHENA:
            return "eAthena";
        case ServerType::EVOL2:
            return "Evol2";
        default:
        case ServerType::UNKNOWN:
            return "";
    }
}

static uint16_t defaultPortForServerType(const ServerTypeT type)
{
    switch (type)
    {
        default:
        case ServerType::EATHENA:
        case ServerType::EVOL2:
            return 6900;
        case ServerType::UNKNOWN:
        case ServerType::TMWATHENA:
            return 6901;
    }
}

ServerDialog::ServerDialog(ServerInfo *const serverInfo,
                           const std::string &dir) :
    // TRANSLATORS: servers dialog name
    Window(_("Choose Your Server"), Modal_false, nullptr, "server.xml"),
    ActionListener(),
    KeyListener(),
    SelectionListener(),
    mMutex(),
    mServers(ServerInfos()),
    mDir(dir),
    mDescription(new Label(this, std::string())),
    // TRANSLATORS: servers dialog button
    mQuitButton(new Button(this, _("Quit"), "quit", this)),
    // TRANSLATORS: servers dialog button
    mConnectButton(new Button(this, _("Connect"), "connect", this)),
    // TRANSLATORS: servers dialog button
    mAddEntryButton(new Button(this, _("Add"), "addEntry", this)),
    // TRANSLATORS: servers dialog button
    mEditEntryButton(new Button(this, _("Edit"), "editEntry", this)),
    // TRANSLATORS: servers dialog button
    mDeleteButton(new Button(this, _("Delete"), "remove", this)),
    // TRANSLATORS: servers dialog button
    mLoadButton(new Button(this, _("Load"), "load", this)),
    // TRANSLATORS: servers dialog button
    mInfoButton(new Button(this, _("Info"), "info", this)),
    mServersListModel(new ServersListModel(&mServers, this)),
    mServersList(CREATEWIDGETR(ServersListBox, this, mServersListModel)),
    mDownload(nullptr),
    mServerInfo(serverInfo),
    mPersistentIPCheckBox(nullptr),
    mDownloadProgress(-1.0F),
    mDownloadStatus(ServerDialogDownloadStatus::UNKNOWN)
{
    if (isSafeMode)
    {
        // TRANSLATORS: servers dialog name
        setCaption(_("Choose Your Server  *** SAFE MODE ***"));
    }

    setWindowName("ServerDialog");

    setCloseButton(true);

    mPersistentIPCheckBox = new CheckBox(this,
        // TRANSLATORS: servers dialog checkbox
        _("Use same ip for game sub servers"),
        config.getBoolValue("usePersistentIP"),
        this, "persitent ip");

    loadCustomServers();

    mServersList->addMouseListener(this);

    ScrollArea *const usedScroll = new ScrollArea(this, mServersList,
        fromBool(getOptionBool("showbackground"), Opaque),
        "server_background.xml");
    usedScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mServersList->addSelectionListener(this);
    usedScroll->setVerticalScrollAmount(0);

    place(0, 0, usedScroll, 8, 5).setPadding(3);
    place(0, 5, mDescription, 8);
    place(0, 6, mPersistentIPCheckBox, 8);
    place(0, 7, mInfoButton);
    place(1, 7, mAddEntryButton);
    place(2, 7, mEditEntryButton);
    place(3, 7, mLoadButton);
    place(4, 7, mDeleteButton);
    place(6, 7, mQuitButton);
    place(7, 7, mConnectButton);

    // Make sure the list has enough height
    getLayout().setRowHeight(0, 80);

    // Do this manually instead of calling reflowLayout so we can enforce a
    // minimum width.
    int width = 500;
    int height = 350;

    getLayout().reflow(width, height);
    setContentSize(width, height);

    setMinWidth(310);
    setMinHeight(220);
    setDefaultSize(getWidth(), getHeight(), ImagePosition::CENTER);

    setResizable(true);
    addKeyListener(this);

    loadWindowState();
}

void ServerDialog::postInit()
{
    Window::postInit();
    setVisible(Visible_true);

    mConnectButton->requestFocus();

    loadServers(true);

    mServersList->setSelected(0);  // Do this after for the Delete button

    if (needUpdateServers())
        downloadServerList();
}

ServerDialog::~ServerDialog()
{
    if (mDownload != nullptr)
    {
        mDownload->cancel();
        delete2(mDownload)
    }
    delete2(mServersListModel);
}

void ServerDialog::connectToSelectedServer()
{
    if (client->getState() == State::CONNECT_SERVER)
        return;

    const int index = mServersList->getSelected();
    if (index < 0)
        return;

    if (mDownload != nullptr)
        mDownload->cancel();

    mQuitButton->setEnabled(false);
    mConnectButton->setEnabled(false);
    mLoadButton->setEnabled(false);

    ServerInfo server = mServers.at(index);
    mServerInfo->hostname = server.hostname;
    mServerInfo->althostname = server.althostname;
    mServerInfo->port = server.port;
    mServerInfo->type = server.type;
    mServerInfo->name = server.name;
    mServerInfo->description = server.description;
    mServerInfo->registerUrl = server.registerUrl;
    mServerInfo->onlineListUrl = server.onlineListUrl;
    mServerInfo->supportUrl = server.supportUrl;
    mServerInfo->defaultHostName = server.defaultHostName;
    mServerInfo->save = true;
    mServerInfo->persistentIp = server.persistentIp;
    mServerInfo->updateMirrors = server.updateMirrors;
    mServerInfo->packetVersion = server.packetVersion;
    mServerInfo->updateHosts = server.updateHosts;

    settings.persistentIp = mServerInfo->persistentIp;
    settings.supportUrl = mServerInfo->supportUrl;
    settings.updateMirrors = mServerInfo->updateMirrors;

    if (chatLogger != nullptr)
        chatLogger->setServerName(mServerInfo->hostname);

    saveCustomServers(*mServerInfo);

    if (!LoginDialog::savedPasswordKey.empty())
    {
        if (mServerInfo->hostname != LoginDialog::savedPasswordKey)
        {
            LoginDialog::savedPassword.clear();
            if (desktop != nullptr)
                desktop->reloadWallpaper();
        }
    }

    config.setValue("usePersistentIP",
        mPersistentIPCheckBox->isSelected());
    client->setState(State::CONNECT_SERVER);
}

void ServerDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "connect")
    {
        connectToSelectedServer();
    }
    else if (eventId == "quit")
    {
        close();
    }
    else if (eventId == "load")
    {
        downloadServerList();
    }
    else if (eventId == "addEntry")
    {
        CREATEWIDGET(EditServerDialog, this, ServerInfo(), -1);
    }
    else if (eventId == "editEntry")
    {
        const int index = mServersList->getSelected();
        if (index >= 0)
        {
            CREATEWIDGET(EditServerDialog,
                this,
                mServers.at(index),
                index);
        }
    }
    else if (eventId == "remove")
    {
        const int index = mServersList->getSelected();
        if (index >= 0)
        {
            mServersList->setSelected(0);
            mServers.erase(mServers.begin() + index);
            saveCustomServers();
        }
    }
    else if (eventId == "info")
    {
        const int index = mServersList->getSelected();
        if (index >= 0)
        {
            CREATEWIDGET(ServerInfoWindow,
                mServers.at(index));
        }
    }
}

void ServerDialog::keyPressed(KeyEvent &event)
{
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (event.getActionId())
    {
        case InputAction::GUI_CANCEL:
            event.consume();
            client->setState(State::EXIT);
            return;

        case InputAction::GUI_SELECT:
        case InputAction::GUI_SELECT2:
            event.consume();
            action(ActionEvent(nullptr,
                mConnectButton->getActionEventId()));
            return;

        case InputAction::GUI_INSERT:
            CREATEWIDGET(EditServerDialog, this, ServerInfo(), -1);
            return;

        case InputAction::GUI_DELETE:
        {
            const int index = mServersList->getSelected();
            if (index >= 0)
            {
                mServersList->setSelected(0);
                mServers.erase(mServers.begin() + index);
                saveCustomServers();
            }
            return;
        }

        case InputAction::GUI_BACKSPACE:
        {
            const int index = mServersList->getSelected();
            if (index >= 0)
            {
                CREATEWIDGET(EditServerDialog, this, mServers.at(index),
                    index);
            }
            return;
        }

        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)
    if (!event.isConsumed())
        mServersList->keyPressed(event);
}

void ServerDialog::valueChanged(const SelectionEvent &event A_UNUSED)
{
    const int index = mServersList->getSelected();
    if (index == -1)
    {
        mDeleteButton->setEnabled(false);
        return;
    }
    mDeleteButton->setEnabled(true);
}

void ServerDialog::mouseClicked(MouseEvent &event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        event.consume();
        if (event.getClickCount() == 2 &&
            event.getSource() == mServersList)
        {
            action(ActionEvent(mConnectButton,
                mConnectButton->getActionEventId()));
        }
    }
}

void ServerDialog::logic()
{
    BLOCK_START("ServerDialog::logic")
    {
        MutexLocker tempLock(&mMutex);
        if (mDownloadStatus == ServerDialogDownloadStatus::COMPLETE)
        {
            loadServers();
            mDownloadStatus = ServerDialogDownloadStatus::OVER;
            mDescription->setCaption(std::string());
        }
        else if (mDownloadStatus == ServerDialogDownloadStatus::IN_PROGRESS)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(strprintf(_("Downloading server list..."
                "%2.2f%%"), static_cast<double>(mDownloadProgress * 100)));
        }
        else if (mDownloadStatus == ServerDialogDownloadStatus::IDLE)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(_("Waiting for server..."));
        }
        else if (mDownloadStatus == ServerDialogDownloadStatus::PREPARING)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(_("Preparing download"));
        }
        else if (mDownloadStatus == ServerDialogDownloadStatus::ERROR)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(_("Error retreiving server list!"));
        }
    }

    Window::logic();
    BLOCK_END("ServerDialog::logic")
}

void ServerDialog::downloadServerList()
{
    // Try to load the configuration value for the onlineServerList
    std::string listFile = branding.getStringValue("onlineServerList");
    std::string listFile2 = branding.getStringValue("onlineServerList2");

    // Fall back to manaplus.org when neither branding
    // nor config set it
    if (listFile.empty())
        listFile = "http://manaplus.org/serverlist.xml";

    if (mDownload != nullptr)
    {
        mDownload->cancel();
        delete2(mDownload)
    }

    mDownload = new Net::Download(this, listFile,
        &downloadUpdate, false, false, true);
    mDownload->setFile(pathJoin(mDir,
        branding.getStringValue("onlineServerFile")));
    if (!listFile2.empty())
        mDownload->addMirror(listFile2);
    mDownload->start();

    config.setValue("serverslistupdate", getDateString());
}

static void loadHostsGroup(XmlNodeConstPtr node,
                           ServerInfo &server)
{
    HostsGroup group;
    group.name = XML::langProperty(node,
        "name",
        // TRANSLATORS: unknown hosts group name
        _("Unknown"));
    for_each_xml_child_node(hostNode, node)
    {
        if (!xmlNameEqual(hostNode, "host"))
            continue;
        const std::string host = XmlChildContent(hostNode);
        if (host.empty())
            continue;
        if (!checkPath(host))
        {
            logger->log1("Warning: incorrect update server name");
            continue;
        }

        group.hosts.push_back(host);
    }
    if (!group.hosts.empty())
        server.updateHosts.push_back(group);
}

void ServerDialog::loadServers(const bool addNew)
{
    XML::Document doc(pathJoin(mDir,
        branding.getStringValue("onlineServerFile")),
        UseVirtFs_false,
        SkipError_false);
    XmlNodeConstPtr rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "serverlist"))
    {
        logger->log1("Error loading server list!");
        return;
    }

    const int ver = XML::getProperty(rootNode, "version", 0);
    if (ver != 1)
    {
        logger->log("Error: unsupported online server list version: %d",
                    ver);
        return;
    }

    const std::string lang = getLangShort();
    const std::string description2("description_" + lang);

    for_each_xml_child_node(serverNode, rootNode)
    {
        if (!xmlNameEqual(serverNode, "server"))
            continue;

        const std::string type = XML::getProperty(
            serverNode, "type", "unknown");
        ServerInfo server;
        server.type = ServerInfo::parseType(type);

        // Ignore unknown server types
        if (server.type == ServerType::UNKNOWN)
        {
            logger->log("Ignoring server entry with unknown type: %s",
                        type.c_str());
            continue;
        }

        server.name = XML::getProperty(serverNode, "name", std::string());
        std::string version = XML::getProperty(serverNode, "minimumVersion",
                                               std::string());

        const bool meetsMinimumVersion = (compareStrI(version, SMALL_VERSION)
                                   <= 0);

        // For display in the list
        if (meetsMinimumVersion)
            version.clear();
        else if (version.empty())
        {
            // TRANSLATORS: servers dialog label
            version = _("requires a newer version");
        }
        else
        {
            // TRANSLATORS: servers dialog label
            version = strprintf(_("requires v%s"), version.c_str());
        }

        const Font *const font = gui->getFont();

        for_each_xml_child_node(subNode, serverNode)
        {
            if (xmlNameEqual(subNode, "connection"))
            {
                server.hostname = XML::getProperty(subNode, "hostname", "");
                server.althostname = XML::getProperty(
                    subNode, "althostname", "");
                server.port = CAST_U16(
                    XML::getProperty(subNode, "port", 0));
                server.packetVersion = XML::getProperty(subNode,
                    "packetVersion", 0);

                if (server.port == 0)
                {
                    // If no port is given, use the default for the given type
                    server.port = defaultPortForServerType(server.type);
                }
            }
            else if (XmlHaveChildContent(subNode))
            {
                if ((xmlNameEqual(subNode, "description")
                    && server.description.empty()) || (!lang.empty()
                    && xmlNameEqual(subNode, description2.c_str())))
                {
                    server.description = XmlChildContent(subNode);
                }
                else if (xmlNameEqual(subNode, "registerurl"))
                {
                    server.registerUrl = XmlChildContent(subNode);
                }
                else if (xmlNameEqual(subNode, "onlineListUrl"))
                {
                    server.onlineListUrl = XmlChildContent(subNode);
                }
                else if (xmlNameEqual(subNode, "support"))
                {
                    server.supportUrl = XmlChildContent(subNode);
                }
                else if (xmlNameEqual(subNode, "persistentIp"))
                {
                    std::string text = XmlChildContent(subNode);
                    server.persistentIp = (text == "1" || text == "true");
                }
                else if (xmlNameEqual(subNode, "updateMirror"))
                {
                    server.updateMirrors.push_back(XmlChildContent(subNode));
                }
            }
            if (xmlNameEqual(subNode, "updates"))
            {
                loadHostsGroup(subNode, server);
            }
            else if (xmlNameEqual(subNode, "defaultUpdateHost"))
            {
                server.defaultHostName = XML::langProperty(
                    // TRANSLATORS: default hosts group name
                    subNode, "name", _("default"));
            }
        }

        server.version.first = font->getWidth(version);
        server.version.second = version;

        MutexLocker tempLock(&mMutex);
        // Add the server to the local list if it's not already present
        bool found = false;
        for (unsigned int i = 0, fsz = CAST_U32(
             mServers.size()); i < fsz; i++)
        {
            if (mServers[i] == server)
            {
                // Use the name listed in the server list
                mServers[i].name = server.name;
                mServers[i].type = server.type;
                mServers[i].version = server.version;
                mServers[i].description = server.description;
                mServers[i].registerUrl = server.registerUrl;
                mServers[i].onlineListUrl = server.onlineListUrl;
                mServers[i].supportUrl = server.supportUrl;
                mServers[i].althostname = server.althostname;
                mServers[i].persistentIp = server.persistentIp;
                mServers[i].updateMirrors = server.updateMirrors;
                mServers[i].defaultHostName = server.defaultHostName;
                mServers[i].updateHosts = server.updateHosts;
                mServers[i].packetVersion = server.packetVersion;
                mServersListModel->setVersionString(i, version);
                found = true;
                break;
            }
        }
        if (!found && addNew)
            mServers.push_back(server);
    }
    if (mServersList->getSelected() < 0)
        mServersList->setSelected(0);
}

void ServerDialog::loadCustomServers()
{
    for (int i = 0; i < MAX_SERVERLIST; ++i)
    {
        const std::string index = toString(i);
        const std::string nameKey("MostUsedServerDescName" + index);
        const std::string descKey("MostUsedServerDescription" + index);
        const std::string hostKey("MostUsedServerName" + index);
        const std::string typeKey("MostUsedServerType" + index);
        const std::string portKey("MostUsedServerPort" + index);
        const std::string onlineListUrlKey
            ("MostUsedServerOnlineList" + index);
        const std::string persistentIpKey("persistentIp" + index);
        const std::string packetVersionKey
            ("MostUsedServerPacketVersion" + index);

        ServerInfo server;
        server.name = config.getValue(nameKey, "");
        server.description = config.getValue(descKey, "");
        server.onlineListUrl = config.getValue(onlineListUrlKey, "");
        server.hostname = config.getValue(hostKey, "");
        server.type = ServerInfo::parseType(config.getValue(typeKey, ""));
        server.persistentIp = config.getValue(
            persistentIpKey, 0) != 0 ? true : false;
        server.packetVersion = config.getValue(packetVersionKey, 0);

        const int defaultPort = defaultPortForServerType(server.type);
        server.port = CAST_U16(
            config.getValue(portKey, defaultPort));

        // skip invalid server
        if (!server.isValid())
            continue;

        server.save = true;
        mServers.push_back(server);
    }
}

void ServerDialog::saveCustomServers(const ServerInfo &currentServer,
                                     const int index)
{
    // Make sure the current server is mentioned first
    if (currentServer.isValid())
    {
        if (index >= 0 && CAST_SIZE(index) < mServers.size())
        {
            mServers[index] = currentServer;
        }
        else
        {
            FOR_EACH (ServerInfos::iterator, i, mServers)
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

    for (unsigned i = 0, fsz = CAST_U32(mServers.size());
         i < fsz && savedServerCount < MAX_SERVERLIST; ++ i)
    {
        const ServerInfo &server = mServers.at(i);

        // Only save servers that were loaded from settings
        if (!(server.save && server.isValid()))
            continue;

        const std::string num = toString(savedServerCount);
        const std::string nameKey("MostUsedServerDescName" + num);
        const std::string descKey("MostUsedServerDescription" + num);
        const std::string hostKey("MostUsedServerName" + num);
        const std::string typeKey("MostUsedServerType" + num);
        const std::string portKey("MostUsedServerPort" + num);
        const std::string onlineListUrlKey
            ("MostUsedServerOnlineList" + num);
        const std::string persistentIpKey("persistentIp" + num);
        const std::string packetVersionKey
            ("MostUsedServerPacketVersion" + num);

        config.setValue(nameKey, server.name);
        config.setValue(descKey, server.description);
        config.setValue(onlineListUrlKey, server.onlineListUrl);
        config.setValue(hostKey, server.hostname);
        config.setValue(typeKey, serverTypeToString(server.type));
        config.setValue(portKey, toString(server.port));
        config.setValue(persistentIpKey, server.persistentIp);
        config.setValue(packetVersionKey, server.packetVersion);
        ++ savedServerCount;
    }

    // Insert an invalid entry at the end to make the loading stop there
    if (savedServerCount < MAX_SERVERLIST)
        config.setValue("MostUsedServerName" + toString(savedServerCount), "");
}

int ServerDialog::downloadUpdate(void *ptr,
                                 const DownloadStatusT status,
                                 size_t total,
                                 const size_t remaining)
{
    if ((ptr == nullptr) || status == DownloadStatus::Cancelled)
        return -1;

    ServerDialog *const sd = reinterpret_cast<ServerDialog*>(ptr);
    bool finished = false;

    if (sd->mDownload == nullptr)
        return -1;

    if (status == DownloadStatus::Complete)
    {
        finished = true;
    }
    else if (CAST_S32(status) < 0)
    {
        logger->log("Error retreiving server list: %s\n",
                    sd->mDownload->getError());
        sd->mDownloadStatus = ServerDialogDownloadStatus::ERROR;
    }
    else
    {
        float progress = static_cast<float>(remaining);
        if (total != 0u)
            progress /= static_cast<float>(total);

        if (progress != progress || progress < 0.0F)
            progress = 0.0f;
        else if (progress > 1.0F)
            progress = 1.0F;

        MutexLocker lock1(&sd->mMutex);
        sd->mDownloadStatus = ServerDialogDownloadStatus::IN_PROGRESS;
        sd->mDownloadProgress = progress;
    }

    if (finished)
    {
        MutexLocker lock1(&sd->mMutex);
        sd->mDownloadStatus = ServerDialogDownloadStatus::COMPLETE;
    }

    return 0;
}

void ServerDialog::updateServer(const ServerInfo &server, const int index)
{
    saveCustomServers(server, index);
}

bool ServerDialog::needUpdateServers() const
{
    if (mServers.empty() || config.getStringValue("serverslistupdate")
        != getDateString())
    {
        return true;
    }

    return false;
}

void ServerDialog::close()
{
    if (mDownload != nullptr)
        mDownload->cancel();
    client->setState(State::FORCE_QUIT);
    Window::close();
}
