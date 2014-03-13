/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/windows/whoisonline.h"

#include "guild.h"
#include "party.h"

#include "gui/viewport.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/scrollarea.h"

#include "actormanager.h"
#include "client.h"
#include "configuration.h"
#include "main.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "net/download.h"
#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/gettext.h"
#include "utils/sdlhelper.h"

#include <SDL_thread.h>
#include <vector>
#include <algorithm>

// Curl should be included after Guichan to avoid Windows redefinitions
#include <curl/curl.h>

#include "debug.h"

#ifdef free
#undef free
#endif

#ifdef malloc
#undef malloc
#endif

class NameFunctuator final
{
    public:
        bool operator()(const OnlinePlayer *left,
                        const OnlinePlayer *right) const
        {
            return (compareStrI(left->getNick(), right->getNick()) < 0);
        }
} nameCompare;

WhoIsOnline::WhoIsOnline() :
    // TRANSLATORS: who is online window name
    Window(_("Who Is Online - Updating"), false, nullptr, "whoisonline.xml"),
    mUpdateTimer(0),
    mThread(nullptr),
    mMemoryBuffer(nullptr),
    mCurlError(new char[CURL_ERROR_SIZE]),
    mBrowserBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
        "onlinebrowserbox.xml")),
    mScrollArea(new ScrollArea(this, mBrowserBox, false)),
    mOnlinePlayers(),
    mOnlineNicks(),
    // TRANSLATORS: who is online. button.
    mUpdateButton(new Button(this, _("Update"), "update", this)),
    mFriends(),
    mNeutral(),
    mDisregard(),
    mEnemy(),
    mDownloadedBytes(0),
    mDownloadStatus(UPDATE_LIST),
    mDownloadComplete(true),
    mAllowUpdate(true),
    mShowLevel(false),
    mUpdateOnlineList(config.getBoolValue("updateOnlineList")),

    mGroupFriends(true)
{
    mCurlError[0] = 0;
    setWindowName("WhoIsOnline");
}

void WhoIsOnline::postInit()
{
    const int h = 350;
    const int w = 200;
    setDefaultSize(w, h, ImageRect::CENTER);

    setVisible(false);
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);
    setSaveVisible(true);

    mUpdateButton->setEnabled(false);
    mUpdateButton->setDimension(Rect(5, 5, w - 10, 20 + 5));

    mBrowserBox->setOpaque(false);
    mScrollArea->setDimension(Rect(5, 20 + 10, w - 10, h - 10 - 30));
    mScrollArea->setSize(w - 10, h - 10 - 30);
    mBrowserBox->setLinkHandler(this);

    add(mUpdateButton);
    add(mScrollArea);

    setLocationRelativeTo(getParent());

    loadWindowState();
    enableVisibleSound(true);

    download();

    widgetResized(Event(nullptr));
    config.addListener("updateOnlineList", this);
    config.addListener("groupFriends", this);
    mGroupFriends = config.getBoolValue("groupFriends");
}

WhoIsOnline::~WhoIsOnline()
{
    config.removeListeners(this);
    CHECKLISTENERS

    if (mThread && SDL_GetThreadID(mThread))
        SDL_WaitThread(mThread, nullptr);

    free(mMemoryBuffer);
    mMemoryBuffer = nullptr;

    // Remove possibly leftover temporary download
    delete []mCurlError;

    FOR_EACH (std::set<OnlinePlayer*>::iterator, itd, mOnlinePlayers)
        delete *itd;
    mOnlinePlayers.clear();
    mOnlineNicks.clear();
}

void WhoIsOnline::handleLink(const std::string& link, MouseEvent *event)
{
    if (!event || event->getButton() == MouseEvent::LEFT)
    {
        if (chatWindow)
        {
            const std::string text = decodeLinkText(link);
            if (config.getBoolValue("whispertab"))
            {
                chatWindow->localChatInput("/q " + text);
            }
            else
            {
                chatWindow->addInputText(std::string("/w \"").append(
                    text).append("\" "));
            }
        }
    }
    else if (event->getButton() == MouseEvent::RIGHT)
    {
        if (player_node && link == player_node->getName())
            return;

        if (viewport)
        {
            if (actorManager)
            {
                const std::string text = decodeLinkText(link);
                Being *const being = actorManager->findBeingByName(
                    text, Being::PLAYER);

                if (being && viewport)
                {
                    viewport->showPopup(being);
                    return;
                }
            }
            viewport->showPlayerPopup(link);
        }
    }
}

void WhoIsOnline::updateWindow(size_t numOnline)
{
    // Set window caption
    // TRANSLATORS: who is online window name
    setCaption(_("Who Is Online - ") + toString(numOnline));

    // List the online people
    std::sort(mFriends.begin(), mFriends.end(), nameCompare);
    std::sort(mNeutral.begin(), mNeutral.end(), nameCompare);
    std::sort(mDisregard.begin(), mDisregard.end(), nameCompare);
    bool addedFromSection(false);
    FOR_EACH (std::vector<OnlinePlayer*>::const_iterator, it, mFriends)
    {
        mBrowserBox->addRow((*it)->getText());
        addedFromSection = true;
    }
    if (addedFromSection == true)
    {
        mBrowserBox->addRow("---");
        addedFromSection = false;
    }
    FOR_EACH (std::vector<OnlinePlayer*>::const_iterator, it, mEnemy)
    {
        mBrowserBox->addRow((*it)->getText());
        addedFromSection = true;
    }
    if (addedFromSection == true)
    {
        mBrowserBox->addRow("---");
        addedFromSection = false;
    }
    FOR_EACH (std::vector<OnlinePlayer*>::const_iterator, it, mNeutral)
    {
        mBrowserBox->addRow((*it)->getText());
        addedFromSection = true;
    }
    if (addedFromSection == true && !mDisregard.empty())
        mBrowserBox->addRow("---");

    FOR_EACH (std::vector<OnlinePlayer*>::const_iterator, it, mDisregard)
        mBrowserBox->addRow((*it)->getText());

    if (mScrollArea->getVerticalMaxScroll() <
        mScrollArea->getVerticalScrollAmount())
    {
        mScrollArea->setVerticalScrollAmount(
            mScrollArea->getVerticalMaxScroll());
    }
}

void WhoIsOnline::handlerPlayerRelation(const std::string &nick,
                                        OnlinePlayer *const player)
{
    switch (player_relations.getRelation(nick))
    {
        case PlayerRelation::NEUTRAL:
        default:
            setNeutralColor(player);
            mNeutral.push_back(player);
            break;

        case PlayerRelation::FRIEND:
            player->setText("2");
            if (mGroupFriends)
                mFriends.push_back(player);
            else
                mNeutral.push_back(player);
            break;

        case PlayerRelation::DISREGARDED:
        case PlayerRelation::BLACKLISTED:
            player->setText("8");
            mDisregard.push_back(player);
            break;

        case PlayerRelation::ENEMY2:
            player->setText("1");
            mEnemy.push_back(player);
            break;

        case PlayerRelation::IGNORED:
        case PlayerRelation::ERASED:
            // Ignore the ignored.
            break;
    }
}

void WhoIsOnline::loadList(std::vector<OnlinePlayer*> &list)
{
    mBrowserBox->clearRows();
    const size_t numOnline = list.size();

    FOR_EACH (std::set<OnlinePlayer*>::iterator, itd, mOnlinePlayers)
        delete *itd;
    mOnlinePlayers.clear();
    mOnlineNicks.clear();

    mShowLevel = config.getBoolValue("showlevel");

    FOR_EACH (std::vector<OnlinePlayer*>::const_iterator, it, list)
    {
        OnlinePlayer *player = *it;
        const std::string nick = player->getNick();
        mOnlinePlayers.insert(player);
        mOnlineNicks.insert(nick);

        if (!mShowLevel)
            player->setLevel(0);

        handlerPlayerRelation(nick, player);
    }

    updateWindow(numOnline);
    if (!mOnlineNicks.empty())
    {
        if (chatWindow)
            chatWindow->updateOnline(mOnlineNicks);
        if (socialWindow)
            socialWindow->updateActiveList();
    }
    updateSize();
    mFriends.clear();
    mNeutral.clear();
    mDisregard.clear();
    mEnemy.clear();
}

void WhoIsOnline::loadWebList()
{
    if (!mMemoryBuffer)
        return;

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(
        realloc(mMemoryBuffer, mDownloadedBytes + 1));
    if (!mMemoryBuffer)
        return;

    mMemoryBuffer[mDownloadedBytes] = '\0';

    mBrowserBox->clearRows();
    bool listStarted(false);
    std::string lineStr;
    int numOnline(0);

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    const std::string gmText("(GM)");
    const std::string gmText2("(gm)");

    FOR_EACH (std::set<OnlinePlayer*>::iterator, itd, mOnlinePlayers)
        delete *itd;

    mOnlinePlayers.clear();
    mOnlineNicks.clear();

    mShowLevel = config.getBoolValue("showlevel");

    while (line)
    {
        std::string nick;
        lineStr = line;
        trim(lineStr);
        if (listStarted == true)
        {
            if (lineStr.find(" users are online.") == std::string::npos)
            {
                if (lineStr.length() > 24)
                {
                    nick = lineStr.substr(0, 24);
                    lineStr = lineStr.substr(25);
                }
                else
                {
                    nick = lineStr;
                    lineStr.clear();
                }
                trim(nick);

                bool isGM(false);
                size_t pos = lineStr.find(gmText, 0);
                if (pos != std::string::npos)
                {
                    lineStr = lineStr.substr(pos + gmText.length());
                    isGM = true;
                }
                else
                {
                    pos = lineStr.find(gmText2, 0);
                    if (pos != std::string::npos)
                    {
                        lineStr = lineStr.substr(pos + gmText.length());
                        isGM = true;
                    }
                }

                trim(lineStr);
                pos = lineStr.find("/", 0);

                if (pos != std::string::npos)
                    lineStr = lineStr.substr(0, pos);

                int level = 0;
                if (!lineStr.empty())
                    level = atoi(lineStr.c_str());

                if (actorManager)
                {
                    Being *const being = actorManager->findBeingByName(
                        nick, Being::PLAYER);
                    if (being)
                    {
                        if (level > 0)
                        {
                            being->setLevel(level);
                            being->updateName();
                        }
                        else
                        {
                            if (being->getLevel() > 1)
                                level = being->getLevel();
                        }
                    }
                }

                if (!mShowLevel)
                    level = 0;

                OnlinePlayer *const player = new OnlinePlayer(nick,
                    static_cast<signed char>(255), level,
                    GENDER_UNSPECIFIED, -1);
                mOnlinePlayers.insert(player);
                mOnlineNicks.insert(nick);

                if (isGM)
                    player->setIsGM(true);

                numOnline++;
                handlerPlayerRelation(nick, player);
            }
        }
        else if (lineStr.find("------------------------------")
                 != std::string::npos)
        {
            listStarted = true;
        }
        line = strtok(nullptr, "\n");
    }

    updateWindow(numOnline);

    // Free the memory buffer now that we don't need it anymore
    free(mMemoryBuffer);
    mMemoryBuffer = nullptr;
    mFriends.clear();
    mNeutral.clear();
    mDisregard.clear();
    mEnemy.clear();
}

size_t WhoIsOnline::memoryWrite(void *ptr, size_t size,
                                size_t nmemb, FILE *stream)
{
    if (!stream)
        return 0;

    WhoIsOnline *const wio = reinterpret_cast<WhoIsOnline *>(stream);
    const size_t totalMem = size * nmemb;
    wio->mMemoryBuffer = static_cast<char*>(realloc(wio->mMemoryBuffer,
        wio->mDownloadedBytes + totalMem));
    if (wio->mMemoryBuffer)
    {
        memcpy(&(wio->mMemoryBuffer[wio->mDownloadedBytes]), ptr, totalMem);
        wio->mDownloadedBytes += static_cast<int>(totalMem);
    }

    return totalMem;
}

int WhoIsOnline::downloadThread(void *ptr)
{
    int attempts = 0;
    WhoIsOnline *const wio = reinterpret_cast<WhoIsOnline *>(ptr);
    CURLcode res;
    const std::string url(client->getOnlineUrl() + "/online.txt");

    while (attempts < 1 && !wio->mDownloadComplete)
    {
        CURL *curl = curl_easy_init();
        if (curl)
        {
            if (!wio->mAllowUpdate)
            {
                curl_easy_cleanup(curl);
                curl = nullptr;
                break;
            }
            wio->mDownloadedBytes = 0;
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                                   &WhoIsOnline::memoryWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, ptr);

            curl_easy_setopt(curl, CURLOPT_USERAGENT,
                strprintf(PACKAGE_EXTENDED_VERSION,
                branding.getStringValue("appName").c_str()).c_str());

            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, wio->mCurlError);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, ptr);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 7);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
            Net::Download::addProxy(curl);
            Net::Download::secureCurl(curl);

            // Make sure the resources2.txt and news.txt aren't cached,
            // in order to always get the latest version.
            struct curl_slist *pHeaders = nullptr;
            pHeaders = curl_slist_append(
                pHeaders, "pragma: no-cache");
            pHeaders = curl_slist_append(pHeaders,
                "Cache-Control: no-cache");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pHeaders);

            if ((res = curl_easy_perform(curl)) != 0)
            {
                wio->mDownloadStatus = UPDATE_ERROR;
                switch (res)
                {
                    case CURLE_COULDNT_CONNECT:
                    default:
                        std::cerr << "curl error "
                                  << static_cast<unsigned>(res) << ": "
                                  << wio->mCurlError << " host: "
                                  << url.c_str() << std::endl;
                    break;
                }
                attempts++;
                curl_easy_cleanup(curl);
                curl_slist_free_all(pHeaders);
                curl = nullptr;
                continue;
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(pHeaders);

            // It's stored in memory, we're done
            wio->mDownloadComplete = true;
        }
        if (!wio->mAllowUpdate)
            break;
        attempts++;
    }

    if (!wio->mDownloadComplete)
        wio->mDownloadStatus = UPDATE_ERROR;
    return 0;
}

void WhoIsOnline::download()
{
    if (serverVersion < 3)
    {
        mDownloadComplete = true;
        if (mThread && SDL_GetThreadID(mThread))
            SDL_WaitThread(mThread, nullptr);

        mDownloadComplete = false;
        mThread = SDL::createThread(&WhoIsOnline::downloadThread,
            "whoisonline", this);
        if (mThread == nullptr)
            mDownloadStatus = UPDATE_ERROR;
    }
    else
    {
        if (client->limitPackets(PACKET_ONLINELIST))
            Net::getPlayerHandler()->requestOnlineList();
    }
}

void WhoIsOnline::logic()
{
    BLOCK_START("WhoIsOnline::logic")
    mScrollArea->logic();
    BLOCK_END("WhoIsOnline::logic")
}

void WhoIsOnline::slowLogic()
{
    if (!mAllowUpdate)
        return;

    BLOCK_START("WhoIsOnline::slowLogic")
    if (mUpdateTimer == 0)
        mUpdateTimer = cur_time;

    const double timeDiff = difftime(cur_time, mUpdateTimer);
    const int timeLimit = isWindowVisible() ? 20 : 120;

    if (mUpdateOnlineList && timeDiff >= timeLimit
        && mDownloadStatus != UPDATE_LIST)
    {
        if (mDownloadComplete == true)
        {
            // TRANSLATORS: who is online window name
            setCaption(_("Who Is Online - Updating"));
            mUpdateTimer = 0;
            mDownloadStatus = UPDATE_LIST;
            download();
        }
    }

    switch (mDownloadStatus)
    {
        case UPDATE_ERROR:
            mBrowserBox->clearRows();
            mBrowserBox->addRow("##1Failed to fetch the online list!");
            mBrowserBox->addRow(mCurlError);
            mDownloadStatus = UPDATE_COMPLETE;
            // TRANSLATORS: who is online window name
            setCaption(_("Who Is Online - error"));
            mUpdateButton->setEnabled(true);
            mUpdateTimer = cur_time + 240;
            mDownloadComplete = true;
            updateSize();
            break;
        case UPDATE_LIST:
            if (mDownloadComplete == true)
            {
                loadWebList();
                mDownloadStatus = UPDATE_COMPLETE;
                mUpdateButton->setEnabled(true);
                mUpdateTimer = 0;
                updateSize();
                if (!mOnlineNicks.empty())
                {
                    if (chatWindow)
                        chatWindow->updateOnline(mOnlineNicks);
                    if (socialWindow)
                        socialWindow->updateActiveList();
                }
            }
            break;
        case UPDATE_COMPLETE:
        default:
            break;
    }
    BLOCK_END("WhoIsOnline::slowLogic")
}

void WhoIsOnline::action(const ActionEvent &event)
{
    if (event.getId() == "update")
    {
        if (serverVersion < 3)
        {
            if (mDownloadStatus == UPDATE_COMPLETE)
            {
                mUpdateTimer = cur_time - 20;
                if (mUpdateButton)
                    mUpdateButton->setEnabled(false);
                // TRANSLATORS: who is online window name
                setCaption(_("Who Is Online - Update"));
                if (mThread && SDL_GetThreadID(mThread))
                {
                    SDL_WaitThread(mThread, nullptr);
                    mThread = nullptr;
                }
                mDownloadComplete = true;
            }
        }
        else
        {
            if (client->limitPackets(PACKET_ONLINELIST))
            {
                mUpdateTimer = cur_time;
                Net::getPlayerHandler()->requestOnlineList();
            }
        }
    }
}

void WhoIsOnline::widgetResized(const Event &event)
{
    Window::widgetResized(event);
    updateSize();
}

void WhoIsOnline::updateSize()
{
    const Rect area = getChildrenArea();
    if (mUpdateButton)
        mUpdateButton->setWidth(area.width - 10);

    if (mScrollArea)
        mScrollArea->setSize(area.width - 10, area.height - 10 - 30);
    if (mBrowserBox)
        mBrowserBox->setWidth(area.width - 10);
}

const std::string WhoIsOnline::prepareNick(const std::string &restrict nick,
                                           const int level,
                                           const std::string &restrict
                                           color) const
{
    const std::string text = encodeLinkText(nick);
    if (mShowLevel && level > 1)
    {
        return strprintf("@@%s|##%s%s (%d)@@", text.c_str(),
            color.c_str(), text.c_str(), level);
    }
    else
    {
        return strprintf("@@%s|##%s%s@@", text.c_str(),
            color.c_str(), text.c_str());
    }
}

void WhoIsOnline::optionChanged(const std::string &name)
{
    if (name == "updateOnlineList")
        mUpdateOnlineList = config.getBoolValue("updateOnlineList");
    else if (name == "groupFriends")
        mGroupFriends = config.getBoolValue("groupFriends");
}

void WhoIsOnline::setNeutralColor(OnlinePlayer *const player)
{
    if (!player)
        return;

    if (actorManager && player_node)
    {
        const std::string &nick = player->getNick();
        if (nick == player_node->getName())
        {
            player->setText("s");
            return;
        }
        if (player_node->isInParty())
        {
            const Party *const party = player_node->getParty();
            if (party)
            {
                if (party->getMember(nick))
                {
                    player->setText("P");
                    return;
                }
            }
        }

        const Being *const being = actorManager->findBeingByName(nick);
        if (being)
        {
            const Guild *const guild2 = player_node->getGuild();
            if (guild2)
            {
                const Guild *const guild1 = being->getGuild();
                if (guild1)
                {
                    if (guild1->getId() == guild2->getId()
                        || guild2->getMember(nick))
                    {
                        player->setText("U");
                        return;
                    }
                }
                else if (guild2->isMember(nick))
                {
                    player->setText("U");
                    return;
                }
            }
        }
        const Guild *const guild3 = Guild::getGuild(1);
        if (guild3 && guild3->isMember(nick))
        {
            player->setText("U");
            return;
        }
    }
    player->setText("0");
}

void WhoIsOnline::getPlayerNames(StringVect &names)
{
    names.clear();
    FOR_EACH (std::set<std::string>::const_iterator, it, mOnlineNicks)
        names.push_back(*it);
}

void OnlinePlayer::setText(std::string color)
{
    mText.clear();

    if (mStatus != 255 && actorManager)
    {
        Being *const being = actorManager->findBeingByName(
            mNick, Being::PLAYER);
        if (being)
        {
            being->setState(mStatus);
            // for now highlight versions > 3
            if (mVersion > 3)
                being->setAdvanced(true);
        }
    }

    if ((mStatus != 255 && mStatus & Being::FLAG_GM) || mIsGM)
        mText.append("(GM) ");

    if (mLevel > 0)
        mText.append(strprintf("%d", mLevel));

    if (mGender == GENDER_FEMALE)
        mText.append("\u2640");
    else if (mGender == GENDER_MALE)
        mText.append("\u2642");

    if (mStatus > 0 && mStatus != 255)
    {
        if (mStatus & Being::FLAG_SHOP)
            mText.append("$");
        if (mStatus & Being::FLAG_AWAY)
        {
            // TRANSLATORS: this away status writed in player nick
            mText.append(_("A"));
        }
        if (mStatus & Being::FLAG_INACTIVE)
        {
            // TRANSLATORS: this inactive status writed in player nick
            mText.append(_("I"));
        }

        if (mStatus & Being::FLAG_GM && color == "0")
            color = "2";
    }
    else if (mIsGM && color == "0")
    {
        color = "2";
    }

    if (mVersion > 0)
        mText.append(strprintf(" - %d", mVersion));

    const std::string text = encodeLinkText(mNick);
    mText = strprintf("@@%s|##%s%s %s@@", text.c_str(), color.c_str(),
        text.c_str(), mText.c_str());
}
