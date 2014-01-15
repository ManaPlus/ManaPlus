/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/windows/serverdialog.h"

#include "chatlogger.h"
#include "client.h"
#include "configuration.h"
#include "main.h"

#include "input/keydata.h"
#include "input/keyevent.h"

#include "gui/sdlfont.h"

#include "gui/windows/editserverdialog.h"
#include "gui/windows/logindialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "utils/gettext.h"
#include "utils/langs.h"

#include <guichan/font.hpp>

#include <string>

#include "debug.h"

static const int MAX_SERVERLIST = 15;

static std::string serverTypeToString(const ServerInfo::Type type)
{
    switch (type)
    {
        case ServerInfo::TMWATHENA:
            return "TmwAthena";
        case ServerInfo::EVOL:
            return "Evol";
#ifdef EATHENA_SUPPORT
        case ServerInfo::EATHENA:
            return "eAthena";
#endif
#ifdef MANASERV_SUPPORT
        case ServerInfo::MANASERV:
            return "ManaServ";
#else
        case ServerInfo::MANASERV:
#endif
#ifndef EATHENA_SUPPORT
        case ServerInfo::EATHENA:
#endif
        default:
        case ServerInfo::UNKNOWN:
            return "";
    }
}

static uint16_t defaultPortForServerType(const ServerInfo::Type type)
{
    switch (type)
    {
        default:
        case ServerInfo::EATHENA:
#ifdef EATHENA_SUPPORT
            return 6900;
#else
            return 6901;
#endif
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

ServersListModel::ServersListModel(ServerInfos *const servers,
                                   ServerDialog *const parent) :
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
    myServer.append(server.hostname);
    return myServer;
}

void ServersListModel::setVersionString(const int index,
                                        const std::string &version)
{
    if (index < 0 || index >= static_cast<int>(mVersionStrings.size()))
        return;

    if (version.empty())
    {
        mVersionStrings[index] = VersionString(0, "");
    }
    else
    {
        mVersionStrings[index] = VersionString(
            gui->getFont()->getWidth(version), version);
    }
}

class ServersListBox final : public ListBox
{
public:
    ServersListBox(const Widget2 *const widget,
                   ServersListModel *const model) :
        ListBox(widget, model, "serverslistbox.xml"),
        mNotSupportedColor(getThemeColor(Theme::SERVER_VERSION_NOT_SUPPORTED)),
        mNotSupportedColor2(getThemeColor(
            Theme::SERVER_VERSION_NOT_SUPPORTED_OUTLINE))
    {
        mHighlightColor = getThemeColor(Theme::HIGHLIGHT);
    }

    void draw(gcn::Graphics *graphics) override final
    {
        if (!mListModel)
            return;

        ServersListModel *const model = static_cast<ServersListModel *const>(
            mListModel);
        Graphics *const g = static_cast<Graphics*>(graphics);

        updateAlpha();

        mHighlightColor.a = static_cast<int>(mAlpha * 255.0F);
        g->setColor(mHighlightColor);

        const int height = getRowHeight();
        mNotSupportedColor.a = static_cast<int>(mAlpha * 255.0F);

        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(gcn::Rectangle(mPadding,
                height * mSelected + mPadding, getWidth() - 2 * mPadding,
                height));
        }

        gcn::Font *const font1 = boldFont;
        gcn::Font *const font2 = getFont();
        const int fontHeight = font1->getHeight();
        const int pad1 = fontHeight + mPadding;
        const int pad2 = height / 4 + mPadding;
        const int width = getWidth();
        // Draw the list elements
        for (int i = 0, y = 0; i < model->getNumberOfElements();
             ++i, y += height)
        {
            const ServerInfo &info = model->getServer(i);

            if (mSelected == i)
            {
                g->setColorAll(mForegroundSelectedColor,
                    mForegroundSelectedColor2);
            }
            else
            {
                g->setColorAll(mForegroundColor, mForegroundColor2);
            }

            int top;
            int x = mPadding;

            if (!info.name.empty())
            {
                x += font1->getWidth(info.name) + 15;
                font1->drawString(graphics, info.name, mPadding, y + mPadding);
                top = y + pad1;
            }
            else
            {
                top = y + pad2;
            }

            if (!info.description.empty())
                font2->drawString(graphics, info.description, x, y + mPadding);
            font2->drawString(graphics, model->getElementAt(i), mPadding, top);

            if (info.version.first > 0)
            {
                g->setColorAll(mNotSupportedColor, mNotSupportedColor2);
                font2->drawString(graphics, info.version.second,
                    width - info.version.first - mPadding, top);
            }
        }
    }

    unsigned int getRowHeight() const override final
    {
        return 2 * getFont()->getHeight() + 5;
    }
private:
    gcn::Color mNotSupportedColor;
    gcn::Color mNotSupportedColor2;
};


ServerDialog::ServerDialog(ServerInfo *const serverInfo,
                           const std::string &dir) :
    // TRANSLATORS: servers dialog name
    Window(_("Choose Your Server"), false, nullptr, "server.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    gcn::SelectionListener(),
    mMutex(),
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
    mServers(ServerInfos()),
    mServersListModel(new ServersListModel(&mServers, this)),
    mServersList(new ServersListBox(this, mServersListModel)),
    mDir(dir),
    mDownloadStatus(DOWNLOADING_UNKNOWN),
    mDownload(nullptr),
    mDownloadProgress(-1.0F),
    mServerInfo(serverInfo),
    mPersistentIPCheckBox(nullptr)
{
    mServersList->postInit();

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

    ScrollArea *const usedScroll = new ScrollArea(mServersList,
        getOptionBool("showbackground"), "server_background.xml");
    usedScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mServersList->addSelectionListener(this);
    usedScroll->setVerticalScrollAmount(0);

    place(0, 0, usedScroll, 7, 5).setPadding(3);
    place(0, 5, mDescription, 7);
    place(0, 6, mPersistentIPCheckBox, 7);
    place(0, 7, mAddEntryButton);
    place(1, 7, mEditEntryButton);
    place(2, 7, mLoadButton);
    place(3, 7, mDeleteButton);
    place(5, 7, mQuitButton);
    place(6, 7, mConnectButton);

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
    setDefaultSize(getWidth(), getHeight(), ImageRect::CENTER);

    setResizable(true);
    addKeyListener(this);

    loadWindowState();
}

void ServerDialog::postInit()
{
    setVisible(true);

    mConnectButton->requestFocus();

    loadServers(true);

    mServersList->setSelected(0);  // Do this after for the Delete button

    if (needUpdateServers())
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

void ServerDialog::connectToSelectedServer()
{
    if (client->getState() == STATE_CONNECT_SERVER)
        return;

    const int index = mServersList->getSelected();
    if (index < 0)
        return;

    if (mDownload)
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
    mServerInfo->save = true;

    if (chatLogger)
        chatLogger->setServerName(mServerInfo->hostname);

    saveCustomServers(*mServerInfo);

    if (!LoginDialog::savedPasswordKey.empty())
    {
        if (mServerInfo->hostname != LoginDialog::savedPasswordKey)
        {
            LoginDialog::savedPassword.clear();
            client->reloadWallpaper();
        }
    }

    config.setValue("usePersistentIP",
        mPersistentIPCheckBox->isSelected());
    client->setState(STATE_CONNECT_SERVER);
}

void ServerDialog::action(const gcn::ActionEvent &event)
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
        (new EditServerDialog(this, ServerInfo(), -1))->postInit();
    }
    else if (eventId == "editEntry")
    {
        const int index = mServersList->getSelected();
        if (index >= 0)
        {
            (new EditServerDialog(this, mServers.at(index),
                index))->postInit();
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
}

void ServerDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    switch (static_cast<KeyEvent*>(&keyEvent)->getActionId())
    {
        case Input::KEY_GUI_CANCEL:
            keyEvent.consume();
            client->setState(STATE_EXIT);
            return;

        case Input::KEY_GUI_SELECT:
        case Input::KEY_GUI_SELECT2:
            keyEvent.consume();
            action(gcn::ActionEvent(nullptr,
                mConnectButton->getActionEventId()));
            return;

        case Input::KEY_GUI_INSERT:
            (new EditServerDialog(this, ServerInfo(), -1))->postInit();
            return;

        case Input::KEY_GUI_DELETE:
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

        case Input::KEY_GUI_BACKSPACE:
        {
            const int index = mServersList->getSelected();
            if (index >= 0)
            {
                (new EditServerDialog(this, mServers.at(index),
                    index))->postInit();
            }
            return;
        }

        default:
            break;
    }
    if (!keyEvent.isConsumed())
        mServersList->keyPressed(keyEvent);
}

void ServerDialog::valueChanged(const gcn::SelectionEvent &)
{
    const int index = mServersList->getSelected();
    if (index == -1)
    {
        mDeleteButton->setEnabled(false);
        return;
    }
    mDeleteButton->setEnabled(true);
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
    BLOCK_START("ServerDialog::logic")
    {
        MutexLocker tempLock(&mMutex);
        if (mDownloadStatus == DOWNLOADING_COMPLETE)
        {
            mDownloadStatus = DOWNLOADING_OVER;
            mDescription->setCaption(std::string());
        }
        else if (mDownloadStatus == DOWNLOADING_IN_PROGRESS)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(strprintf(_("Downloading server list..."
                "%2.2f%%"), static_cast<double>(mDownloadProgress * 100)));
        }
        else if (mDownloadStatus == DOWNLOADING_IDLE)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(_("Waiting for server..."));
        }
        else if (mDownloadStatus == DOWNLOADING_PREPARING)
        {
            // TRANSLATORS: servers dialog label
            mDescription->setCaption(_("Preparing download"));
        }
        else if (mDownloadStatus == DOWNLOADING_ERROR)
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

    if (listFile.empty())
        listFile = config.getStringValue("onlineServerList");

    // Fall back to manaplus.org when neither branding
    // nor config set it
    if (listFile.empty())
        listFile = "http://manaplus.org/serverlist.xml";

    if (mDownload)
    {
        mDownload->cancel();
        delete mDownload;
        mDownload = nullptr;
    }

    mDownload = new Net::Download(this, listFile, &downloadUpdate);
    mDownload->setFile(std::string(mDir).append("/").append(
        branding.getStringValue("onlineServerFile")));
    mDownload->start();

    config.setValue("serverslistupdate", getDateString());
}

void ServerDialog::loadServers(const bool addNew)
{
    XML::Document doc(std::string(mDir).append("/").append(
        branding.getStringValue("onlineServerFile")), false);
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "serverlist"))
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
        if (server.type == ServerInfo::UNKNOWN)
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

        const gcn::Font *const font = gui->getFont();

        for_each_xml_child_node(subNode, serverNode)
        {
            if (xmlNameEqual(subNode, "connection"))
            {
                server.hostname = XML::getProperty(subNode, "hostname", "");
                server.althostname = XML::getProperty(
                    subNode, "althostname", "");
                server.port = static_cast<uint16_t>(
                    XML::getProperty(subNode, "port", 0));

                if (server.port == 0)
                {
                    // If no port is given, use the default for the given type
                    server.port = defaultPortForServerType(server.type);
                }
            }
            else if ((xmlNameEqual(subNode, "description")
                     && server.description.empty()) || (!lang.empty()
                     && xmlNameEqual(subNode, description2.c_str())))
            {
                server.description = reinterpret_cast<const char*>(
                    subNode->xmlChildrenNode->content);
            }
            else if (xmlNameEqual(subNode, "registerurl"))
            {
                server.registerUrl = reinterpret_cast<const char*>(
                    subNode->xmlChildrenNode->content);
            }
            else if (xmlNameEqual(subNode, "onlineListUrl"))
            {
                server.onlineListUrl = reinterpret_cast<const char*>(
                    subNode->xmlChildrenNode->content);
            }
            else if (xmlNameEqual(subNode, "support"))
            {
                server.supportUrl = reinterpret_cast<const char*>(
                    subNode->xmlChildrenNode->content);
            }
        }

        server.version.first = font->getWidth(version);
        server.version.second = version;

        MutexLocker tempLock(&mMutex);
        // Add the server to the local list if it's not already present
        bool found = false;
        for (unsigned int i = 0, sz = static_cast<unsigned int>(
             mServers.size()); i < sz; i++)
        {
            if (mServers[i] == server)
            {
                // Use the name listed in the server list
                mServers[i].name = server.name;
                mServers[i].version = server.version;
                mServers[i].description = server.description;
                mServers[i].registerUrl = server.registerUrl;
                mServers[i].onlineListUrl = server.onlineListUrl;
                mServers[i].supportUrl = server.supportUrl;
                mServers[i].althostname = server.althostname;
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

        ServerInfo server;
        server.name = config.getValue(nameKey, "");
        server.description = config.getValue(descKey, "");
        server.onlineListUrl = config.getValue(onlineListUrlKey, "");
        server.hostname = config.getValue(hostKey, "");
        server.type = ServerInfo::parseType(config.getValue(typeKey, ""));

        const int defaultPort = defaultPortForServerType(server.type);
        server.port = static_cast<uint16_t>(
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
        if (index >= 0 && static_cast<unsigned>(index) < mServers.size())
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

    for (unsigned i = 0, sz = static_cast<unsigned>(mServers.size());
         i < sz && savedServerCount < MAX_SERVERLIST; ++ i)
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

        config.setValue(nameKey, toString(server.name));
        config.setValue(descKey, toString(server.description));
        config.setValue(onlineListUrlKey, toString(server.onlineListUrl));
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
    if (!ptr || status == DOWNLOAD_STATUS_CANCELLED)
        return -1;

    ServerDialog *const sd = reinterpret_cast<ServerDialog*>(ptr);
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

        if (progress != progress || progress < 0.0F)
            progress = 0.0f;
        else if (progress > 1.0F)
            progress = 1.0F;

        MutexLocker lock1(&sd->mMutex);
        sd->mDownloadStatus = DOWNLOADING_IN_PROGRESS;
        sd->mDownloadProgress = progress;
    }

    if (finished)
    {
        sd->loadServers();
        MutexLocker lock1(&sd->mMutex);
        sd->mDownloadStatus = DOWNLOADING_COMPLETE;
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
    if (mDownload)
        mDownload->cancel();
    client->setState(STATE_FORCE_QUIT);
    Window::close();
}
