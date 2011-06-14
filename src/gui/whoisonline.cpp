/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "whoisonline.h"

#include <SDL.h>
#include <SDL_thread.h>
#include <vector>
#include <algorithm>

#include "gui/viewport.h"
#include "gui/widgets/button.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/chattab.h"

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "localplayer.h"
#include "playerrelations.h"
#include "main.h"

#include "gui/chatwindow.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

// Curl should be included after Guichan to avoid Windows redefinitions
#include <curl/curl.h>

#include "debug.h"

#ifdef free
#undef free
#endif

#ifdef malloc
#undef malloc
#endif

bool stringCompare(const std::string &left, const std::string &right);

bool stringCompare(const std::string &left, const std::string &right )
{
    for (std::string::const_iterator lit = left.begin(),
         rit = right.begin();
         lit != left.end() && rit != right.end(); ++lit, ++rit)
    {
        if (tolower(*lit) < tolower(*rit))
            return true;
        else if (tolower(*lit) > tolower(*rit))
            return false;
    }
    if (left.size() < right.size())
        return true;
    return false;
}

WhoIsOnline::WhoIsOnline():
    Window(_("Who Is Online - Updating")),
    mThread(NULL),
    mDownloadStatus(UPDATE_LIST),
    mDownloadComplete(true),
    mDownloadedBytes(0),
    mMemoryBuffer(NULL),
    mCurlError(new char[CURL_ERROR_SIZE]),
    mAllowUpdate(true),
    mShowLevel(false)
{
    mCurlError[0] = 0;
    setWindowName("WhoIsOnline");

    const int h = 350;
    const int w = 200;
    setDefaultSize(w, h, ImageRect::CENTER);
//    setContentSize(w, h);
    setCloseButton(true);
    setResizable(true);

    mUpdateButton = new Button(_("Update"), "update", this);
    mUpdateButton->setEnabled(false);
    mUpdateButton->setDimension(gcn::Rectangle(5, 5, w - 10, 20 + 5));

    mBrowserBox = new BrowserBox();
    mScrollArea = new ScrollArea(mBrowserBox);
    mScrollArea->setOpaque(false);
    mBrowserBox->setOpaque(false);
    mBrowserBox->setHighlightMode(BrowserBox::BACKGROUND);
    mScrollArea->setDimension(gcn::Rectangle(5, 20 + 10, w - 10, h - 10 - 30));
    mScrollArea->setSize(w - 10, h - 10 - 30);
    mBrowserBox->setLinkHandler(this);

    add(mUpdateButton);
    add(mScrollArea);

    mUpdateTimer = 0;
    setLocationRelativeTo(getParent());

    loadWindowState();

    download();

    config.addListener("updateOnlineList", this);
    mUpdateOnlineList = config.getBoolValue("updateOnlineList");
}

WhoIsOnline::~WhoIsOnline()
{
    config.removeListener("updateOnlineList", this);

    if (mThread && SDL_GetThreadID(mThread))
        SDL_WaitThread(mThread, NULL);

    free(mMemoryBuffer);
    mMemoryBuffer = 0;

    // Remove possibly leftover temporary download
    delete[] mCurlError;
}

void WhoIsOnline::handleLink(const std::string& link, gcn::MouseEvent *event)
{
    if (!event || event->getButton() == gcn::MouseEvent::LEFT)
    {
        if (chatWindow)
        {
            if (config.getBoolValue("whispertab"))
                chatWindow->localChatInput("/q " + link);
            else
                chatWindow->addInputText("/w \"" + link + "\" ");
        }
    }
    else if (event->getButton() == gcn::MouseEvent::RIGHT)
    {
        if (player_node && link == player_node->getName())
            return;

        if (viewport)
        {
            if (actorSpriteManager)
            {
                Being* being = actorSpriteManager->findBeingByName(
                    link, Being::PLAYER);

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

void WhoIsOnline::loadList()
{
    if (!mMemoryBuffer)
        return;

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(
        realloc(mMemoryBuffer, mDownloadedBytes + 1));
    mMemoryBuffer[mDownloadedBytes] = '\0';

    mBrowserBox->clearRows();
    bool listStarted(false);
    std::string lineStr;
    int numOnline(0);
    std::vector<std::string> friends;
    std::vector<std::string> neutral;
    std::vector<std::string> disregard;

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    const std::string gmText = "(GM)";
    mOnlinePlayers.clear();

    mShowLevel = config.getBoolValue("showlevel");

    while (line != NULL)
    {
        std::string nick;
        lineStr = line;
        trim(lineStr);
        if (listStarted == true)
        {
            size_t found;
            found = lineStr.find(" users are online.");
            if (found == std::string::npos)
            {
                int level = 0;

                std::string::size_type pos = 0;
                if (lineStr.length() > 24)
                {
                    nick = lineStr.substr(0, 24);
                    lineStr = lineStr.substr(25);
                }
                else
                {
                    nick = lineStr;
                    lineStr = "";
                }
                trim(nick);

                pos = lineStr.find(gmText, 0);
                if (pos != std::string::npos)
                    lineStr = lineStr.substr(pos + gmText.length());

                trim(lineStr);
                pos = lineStr.find("/", 0);

                if (pos != std::string::npos)
                    lineStr = lineStr.substr(0, pos);

                if (!lineStr.empty())
                    level = atoi(lineStr.c_str());

                if (actorSpriteManager)
                {
                    Being *being = actorSpriteManager->findBeingByName(
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

                mOnlinePlayers.insert(nick);

                numOnline++;
                switch (player_relations.getRelation(nick))
                {
                    case PlayerRelation::NEUTRAL:
                        neutral.push_back(prepareNick(nick, level, "0"));
                        break;

                    case PlayerRelation::FRIEND:
                        friends.push_back(prepareNick(nick, level, "2"));
                        break;

                    case PlayerRelation::DISREGARDED:
                        disregard.push_back(prepareNick(nick, level, "8"));
                        break;

                    case PlayerRelation::IGNORED:
                    case PlayerRelation::ERASED:
                    default:
                        //Ignore the ignored.
                        break;
                }
            }
        }
        else if (lineStr.find("------------------------------")
                 != std::string::npos)
        {
            listStarted = true;
        }
        line = strtok(NULL, "\n");
    }

    //Set window caption
    setCaption(_("Who Is Online - ") + toString(numOnline));

    //List the online people
    sort(friends.begin(), friends.end(), stringCompare);
    sort(neutral.begin(), neutral.end(), stringCompare);
    sort(disregard.begin(), disregard.end(), stringCompare);
    bool addedFromSection(false);
    for (int i = 0; i < static_cast<int>(friends.size()); i++)
    {
        mBrowserBox->addRow(friends.at(i));
        addedFromSection = true;
    }
    if (addedFromSection == true)
    {
        mBrowserBox->addRow("---");
        addedFromSection = false;
    }
    for (int i = 0; i < static_cast<int>(neutral.size()); i++)
    {
        mBrowserBox->addRow(neutral.at(i));
        addedFromSection = true;
    }
    if (addedFromSection == true && !disregard.empty())
    {
        mBrowserBox->addRow("---");
        addedFromSection = false;
    }
    for (int i = 0; i < static_cast<int>(disregard.size()); i++)
    {
        mBrowserBox->addRow(disregard.at(i));
    }

    // Free the memory buffer now that we don't need it anymore
    free(mMemoryBuffer);
    mMemoryBuffer = 0;

    if (mScrollArea->getVerticalMaxScroll() <
        mScrollArea->getVerticalScrollAmount())
    {
        mScrollArea->setVerticalScrollAmount(
            mScrollArea->getVerticalMaxScroll());
    }
}

size_t WhoIsOnline::memoryWrite(void *ptr, size_t size,
                                size_t nmemb, FILE *stream)
{
    WhoIsOnline *wio = reinterpret_cast<WhoIsOnline *>(stream);
    size_t totalMem = size * nmemb;
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
    WhoIsOnline *wio = reinterpret_cast<WhoIsOnline *>(ptr);
    CURL *curl;
    CURLcode res;

    std::string url(Client::getServerName() + "/online.txt");

    while (attempts < 1 && !wio->mDownloadComplete)
    {
        curl = curl_easy_init();

        if (curl)
        {
            if (!wio->mAllowUpdate)
            {
                curl_easy_cleanup(curl);
                curl = 0;
                break;
            }
            wio->mDownloadedBytes = 0;
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                                   WhoIsOnline::memoryWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, ptr);

            curl_easy_setopt(curl, CURLOPT_USERAGENT,
                             strprintf(PACKAGE_EXTENDED_VERSION, branding
                             .getValue("appShort", "mana").c_str()).c_str());

            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, wio->mCurlError);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, ptr);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 7);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

            struct curl_slist *pHeaders = 0;
            // Make sure the resources2.txt and news.txt aren't cached,
            // in order to always get the latest version.
            pHeaders = curl_slist_append(pHeaders, "pragma: no-cache");
            pHeaders =
                curl_slist_append(pHeaders, "Cache-Control: no-cache");
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
                curl = 0;
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

//    wio->mThread = 0;
    return 0;
}

void WhoIsOnline::download()
{
    mDownloadComplete = true;
    if (mThread && SDL_GetThreadID(mThread))
        SDL_WaitThread(mThread, NULL);

    mDownloadComplete = false;
    mThread = SDL_CreateThread(WhoIsOnline::downloadThread, this);

    if (mThread == NULL)
        mDownloadStatus = UPDATE_ERROR;
}

void WhoIsOnline::logic()
{
    // Update Scroll logic
    mScrollArea->logic();

    if (!mAllowUpdate)
        return;

    if (mUpdateTimer == 0)
        mUpdateTimer = cur_time;

    double timeDiff = difftime(cur_time, mUpdateTimer);
    int timeLimit = isVisible() ? 20 : 120;

    if (mUpdateOnlineList && timeDiff >= timeLimit
        && mDownloadStatus != UPDATE_LIST)
    {
        if (mDownloadComplete == true)
        {
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
            setCaption(_("Who Is Online - error"));
            mUpdateButton->setEnabled(true);
            mUpdateTimer = cur_time + 240;
            break;
        case UPDATE_LIST:
            if (mDownloadComplete == true)
            {
                loadList();
                mDownloadStatus = UPDATE_COMPLETE;
                mUpdateButton->setEnabled(true);
                mUpdateTimer = 0;
                updateSize();
                if (!mOnlinePlayers.empty() && chatWindow)
                    chatWindow->updateOnline(mOnlinePlayers);
            }
            break;
        case UPDATE_COMPLETE:
        default:
            break;
    }
}

void WhoIsOnline::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "update")
    {
        if (mDownloadStatus == UPDATE_COMPLETE)
        {
            mUpdateTimer = cur_time - 20;
            if (mUpdateButton)
                mUpdateButton->setEnabled(false);
            setCaption(_("Who Is Online - Update"));
            if (mThread && SDL_GetThreadID(mThread))
            {
                SDL_WaitThread(mThread, NULL);
                mThread = NULL;
            }
            mDownloadComplete = true;
        }
    }
}

void WhoIsOnline::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);
    updateSize();
}

void WhoIsOnline::updateSize()
{
    if (mDownloadStatus == UPDATE_COMPLETE)
    {
        const gcn::Rectangle area = getChildrenArea();
        if (mUpdateButton)
            mUpdateButton->setWidth(area.width - 10);

        if (mScrollArea)
            mScrollArea->setSize(area.width - 10, area.height - 10 - 30);
    }
}

const std::string WhoIsOnline::prepareNick(std::string nick, int level,
                                           std::string color) const
{
    if (mShowLevel && level > 1)
    {
        return strprintf("@@%s|##%s%s (%d)@@", nick.c_str(),
                         color.c_str(), nick.c_str(), level);
    }
    else
    {
        return strprintf("@@%s|##%s%s@@", nick.c_str(),
                         color.c_str(), nick.c_str());
    }
}

void WhoIsOnline::optionChanged(const std::string &name)
{
    if (name == "updateOnlineList")
        mUpdateOnlineList = config.getBoolValue("updateOnlineList");
}