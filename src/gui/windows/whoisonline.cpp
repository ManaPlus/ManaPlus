/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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

#include "actormanager.h"
#include "configuration.h"
#include "guild.h"
#include "party.h"
#ifdef TMWA_SUPPORT
#include "settings.h"
#endif  // TMWA_SUPPORT

#include "gui/onlineplayer.h"
#include "gui/popupmanager.h"
#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/staticbrowserbox.h"

#include "being/beingflag.h"
#include "being/localplayer.h"
#include "being/playerrelations.h"

#ifdef TMWA_SUPPORT
#include "net/download.h"
#endif  // TMWA_SUPPORT
#include "net/packetlimiter.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#ifdef TMWA_SUPPORT
#include "resources/db/groupdb.h"
#endif  // TMWA_SUPPORT

#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/sdlhelper.h"

#ifdef TMWA_SUPPORT
#include "net/net.h"
#else  // TMWA_SUPPORT
#include <curl/curl.h>
#endif  // TMWA_SUPPORT

#include "debug.h"

#ifdef free
#undef free
#endif  // free

#ifdef malloc
#undef malloc
#endif  // malloc

WhoIsOnline *whoIsOnline = nullptr;

namespace
{
    class NameFunctuator final
    {
        public:
            A_DEFAULT_COPY(NameFunctuator)

            bool operator()(const OnlinePlayer *left,
                            const OnlinePlayer *right) const
            {
                return (compareStrI(left->getNick(), right->getNick()) < 0);
            }
    } nameCompare;
}  // namespace

WhoIsOnline::WhoIsOnline() :
    // TRANSLATORS: who is online window name
    Window(_("Who Is Online - Updating"),
        Modal_false,
        nullptr,
        "whoisonline.xml"),
    mUpdateTimer(0),
    mThread(nullptr),
    mMemoryBuffer(nullptr),
    mCurlError(new char[CURL_ERROR_SIZE]),
    mBrowserBox(new StaticBrowserBox(this, Opaque_true,
        "onlinebrowserbox.xml")),
    mScrollArea(new ScrollArea(this,
        mBrowserBox, Opaque_false, std::string())),
    // TRANSLATORS: who is online. button.
    mUpdateButton(new Button(this, _("Update"), "update", BUTTON_SKIN, this)),
    mOnlinePlayers(),
    mOnlineNicks(),
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
    mGroupFriends(true),
#ifdef TMWA_SUPPORT
    mWebList(Net::getNetworkType() == ServerType::TMWATHENA),
#endif
    mServerSideList(serverFeatures->haveServerOnlineList())
{
    mCurlError[0] = 0;
    setWindowName("WhoIsOnline");
}

void WhoIsOnline::postInit()
{
    Window::postInit();
    const int h = 350;
    const int w = 200;
    setDefaultSize(w, h, ImagePosition::CENTER, 0, 0);

    setVisible(Visible_false);
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);
    setSaveVisible(true);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    mUpdateButton->setEnabled(false);
    mUpdateButton->setDimension(Rect(5, 5, w - 10, 20 + 5));

    mBrowserBox->setOpaque(Opaque_false);
    mScrollArea->setDimension(Rect(5, 20 + 10, w - 10, h - 10 - 30));
    mScrollArea->setSize(w - 10, h - 10 - 30);
    mScrollArea->setSelectable(false);
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

    SDL::WaitThread(mThread);
    mThread = nullptr;
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
    if ((event == nullptr) || event->getButton() == MouseButton::LEFT)
    {
        if (chatWindow != nullptr)
        {
            const std::string text = decodeLinkText(link);
            if (config.getBoolValue("whispertab"))
            {
                chatWindow->localChatInput("/q " + text);
            }
            else
            {
                chatWindow->addInputText(
                    std::string("/w \"").append(text).append("\" "),
                    true);
            }
        }
    }
    else if (event->getButton() == MouseButton::RIGHT)
    {
        if ((localPlayer != nullptr) && link == localPlayer->getName())
            return;

        if (popupMenu != nullptr)
        {
            if (actorManager != nullptr)
            {
                const std::string text = decodeLinkText(link);
                Being *const being = actorManager->findBeingByName(
                    text, ActorType::Player);

                if ((being != nullptr) && (popupManager != nullptr))
                {
                    popupMenu->showPopup(viewport->mMouseX,
                        viewport->mMouseY,
                        being);
                    return;
                }
            }
            popupMenu->showPlayerPopup(link);
        }
    }
}

void WhoIsOnline::updateWindow(size_t numOnline)
{
    // Set window caption
    // TRANSLATORS: who is online window name
    setCaption(_("Who Is Online - ") + toString(CAST_U32(numOnline)));

    // List the online people
    std::sort(mFriends.begin(), mFriends.end(), nameCompare);
    std::sort(mNeutral.begin(), mNeutral.end(), nameCompare);
    std::sort(mDisregard.begin(), mDisregard.end(), nameCompare);
    bool addedFromSection(false);
    FOR_EACH (STD_VECTOR<OnlinePlayer*>::const_iterator, it, mFriends)
    {
        mBrowserBox->addRow((*it)->getText(), false);
        addedFromSection = true;
    }
    if (addedFromSection == true)
    {
        mBrowserBox->addRow("---", false);
        addedFromSection = false;
    }
    FOR_EACH (STD_VECTOR<OnlinePlayer*>::const_iterator, it, mEnemy)
    {
        mBrowserBox->addRow((*it)->getText(), false);
        addedFromSection = true;
    }
    if (addedFromSection == true)
    {
        mBrowserBox->addRow("---", false);
        addedFromSection = false;
    }
    FOR_EACH (STD_VECTOR<OnlinePlayer*>::const_iterator, it, mNeutral)
    {
        mBrowserBox->addRow((*it)->getText(), false);
        addedFromSection = true;
    }
    if (addedFromSection == true && !mDisregard.empty())
        mBrowserBox->addRow("---", false);

    FOR_EACH (STD_VECTOR<OnlinePlayer*>::const_iterator, it, mDisregard)
        mBrowserBox->addRow((*it)->getText(), false);

    if (mScrollArea->getVerticalMaxScroll() <
        mScrollArea->getVerticalScrollAmount())
    {
        mScrollArea->setVerticalScrollAmount(
            mScrollArea->getVerticalMaxScroll());
    }
    mBrowserBox->updateHeight();
}

void WhoIsOnline::handlerPlayerRelation(const std::string &nick,
                                        OnlinePlayer *const player)
{
    if (player == nullptr)
        return;
    switch (playerRelations.getRelation(nick))
    {
        case Relation::NEUTRAL:
        default:
            setNeutralColor(player);
            mNeutral.push_back(player);
            break;

        case Relation::FRIEND:
            player->setText("2");
            if (mGroupFriends)
                mFriends.push_back(player);
            else
                mNeutral.push_back(player);
            break;

        case Relation::DISREGARDED:
        case Relation::BLACKLISTED:
            player->setText("8");
            mDisregard.push_back(player);
            break;

        case Relation::ENEMY2:
            player->setText("1");
            mEnemy.push_back(player);
            break;

        case Relation::IGNORED:
        case Relation::ERASED:
            // Ignore the ignored.
            break;
    }
}

void WhoIsOnline::loadList(const STD_VECTOR<OnlinePlayer*> &list)
{
    mBrowserBox->clearRows();
    const size_t numOnline = list.size();

    FOR_EACH (std::set<OnlinePlayer*>::iterator, itd, mOnlinePlayers)
        delete *itd;
    mOnlinePlayers.clear();
    mOnlineNicks.clear();

    mShowLevel = config.getBoolValue("showlevel");

    FOR_EACH (STD_VECTOR<OnlinePlayer*>::const_iterator, it, list)
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
        if (chatWindow != nullptr)
            chatWindow->updateOnline(mOnlineNicks);
        if (socialWindow != nullptr)
            socialWindow->updateActiveList();
        if (actorManager != nullptr)
            actorManager->updateSeenPlayers(mOnlineNicks);
    }
    updateSize();
    mFriends.clear();
    mNeutral.clear();
    mDisregard.clear();
    mEnemy.clear();
}

#ifdef TMWA_SUPPORT
void WhoIsOnline::loadWebList()
{
    if (mMemoryBuffer == nullptr)
        return;

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(
        realloc(mMemoryBuffer, mDownloadedBytes + 1));
    if (mMemoryBuffer == nullptr)
        return;

    mMemoryBuffer[mDownloadedBytes] = '\0';

    mBrowserBox->clearRows();
    bool listStarted(false);
    std::string lineStr;
    size_t numOnline(0U);

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    const std::string gmText("(GM)");
    const std::string gmText2("(gm)");

    FOR_EACH (std::set<OnlinePlayer*>::iterator, itd, mOnlinePlayers)
        delete *itd;

    mOnlinePlayers.clear();
    mOnlineNicks.clear();

    mShowLevel = config.getBoolValue("showlevel");

    while (line != nullptr)
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
                pos = lineStr.find('/', 0);

                if (pos != std::string::npos)
                    lineStr = lineStr.substr(0, pos);

                int level = 0;
                if (!lineStr.empty())
                    level = atoi(lineStr.c_str());

                if (actorManager != nullptr)
                {
                    Being *const being = actorManager->findBeingByName(
                        nick, ActorType::Player);
                    if (being != nullptr)
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
                    CAST_U8(255), level,
                    Gender::UNSPECIFIED, -1, -1);
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
    mBrowserBox->updateHeight();

    // Free the memory buffer now that we don't need it anymore
    free(mMemoryBuffer);
    mMemoryBuffer = nullptr;
    mFriends.clear();
    mNeutral.clear();
    mDisregard.clear();
    mEnemy.clear();
}

size_t WhoIsOnline::memoryWrite(void *restrict ptr,
                                size_t size,
                                size_t nmemb,
                                FILE *restrict stream)
{
    if (stream == nullptr)
        return 0;

    WhoIsOnline *restrict const wio =
        reinterpret_cast<WhoIsOnline *restrict>(stream);
    const size_t totalMem = size * nmemb;
    wio->mMemoryBuffer = static_cast<char*>(realloc(wio->mMemoryBuffer,
        CAST_SIZE(wio->mDownloadedBytes) + totalMem));
    if (wio->mMemoryBuffer != nullptr)
    {
        memcpy(&(wio->mMemoryBuffer[wio->mDownloadedBytes]), ptr, totalMem);
        wio->mDownloadedBytes += CAST_S32(totalMem);
    }

    return totalMem;
}

int WhoIsOnline::downloadThread(void *ptr)
{
    int attempts = 0;
    WhoIsOnline *const wio = reinterpret_cast<WhoIsOnline *>(ptr);
    if (wio == nullptr)
        return 0;
    CURLcode res;
    const std::string url(settings.onlineListUrl + "/online.txt");

    while (attempts < 1 && !wio->mDownloadComplete)
    {
        CURL *curl = curl_easy_init();
        if (curl != nullptr)
        {
            if (!wio->mAllowUpdate)
            {
                curl_easy_cleanup(curl);
                curl = nullptr;
                break;
            }
            wio->mDownloadedBytes = 0;
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                                   &WhoIsOnline::memoryWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, ptr);

            curl_easy_setopt(curl,
                CURLOPT_USERAGENT,
                settings.userAgent.c_str());

            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, wio->mCurlError);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, ptr);

#if LIBCURL_VERSION_NUM >= 0x070a00
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
#endif  // LIBCURL_VERSION_NUM >= 0x070a00
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 7);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
            Net::Download::addHeaders(curl);
            Net::Download::addProxy(curl);
            Net::Download::secureCurl(curl);
            Net::Download::addCommonFlags(curl);

            // Make sure the resources2.txt and news.txt aren't cached,
            // in order to always get the latest version.
            curl_slist *pHeaders = nullptr;
            pHeaders = curl_slist_append(
                pHeaders, "pragma: no-cache");
            pHeaders = curl_slist_append(pHeaders,
                "Cache-Control: no-cache");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pHeaders);

            if ((res = curl_easy_perform(curl)) != 0)
            {
                wio->mDownloadStatus = UPDATE_ERROR;
                PRAGMA45(GCC diagnostic push)
                PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
                switch (res)
                {
                    case CURLE_COULDNT_CONNECT:
                    default:
                        std::cerr << "curl error "
                                  << CAST_U32(res) << ": "
                                  << wio->mCurlError << " host: "
                                  << url.c_str() << std::endl;
                    break;
                }
                PRAGMA45(GCC diagnostic pop)
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
#endif  // TMWA_SUPPORT

void WhoIsOnline::download()
{
    if (mServerSideList)
    {
        if (PacketLimiter::limitPackets(PacketType::PACKET_ONLINELIST))
            playerHandler->requestOnlineList();
    }
#ifdef TMWA_SUPPORT
    else if (mWebList)
    {
        mDownloadComplete = true;
        SDL::WaitThread(mThread);
        mThread = nullptr;
        mDownloadComplete = false;
        mThread = SDL::createThread(&WhoIsOnline::downloadThread,
            "whoisonline", this);
        if (mThread == nullptr)
            mDownloadStatus = UPDATE_ERROR;
    }
#endif  // TMWA_SUPPORT
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

#ifdef TMWA_SUPPORT
    switch (mDownloadStatus)
    {
        case UPDATE_ERROR:
            logger->assertLog("Failed to fetch the online list:");
            if (mCurlError != nullptr)
                logger->assertLog("%s", mCurlError);
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
                    if (chatWindow != nullptr)
                        chatWindow->updateOnline(mOnlineNicks);
                    if (socialWindow != nullptr)
                        socialWindow->updateActiveList();
                    if (actorManager != nullptr)
                        actorManager->updateSeenPlayers(mOnlineNicks);
                }
            }
            break;
        case UPDATE_COMPLETE:
        default:
            break;
    }
#endif  // TMWA_SUPPORT

    BLOCK_END("WhoIsOnline::slowLogic")
}

void WhoIsOnline::action(const ActionEvent &event)
{
    if (event.getId() == "update")
    {
#ifdef TMWA_SUPPORT
        if (!mServerSideList)
        {
            if (mDownloadStatus == UPDATE_COMPLETE)
            {
                mUpdateTimer = cur_time - 20;
                if (mUpdateButton != nullptr)
                    mUpdateButton->setEnabled(false);
                // TRANSLATORS: who is online window name
                setCaption(_("Who Is Online - Update"));
                SDL::WaitThread(mThread);
                mThread = nullptr;
                mDownloadComplete = true;
            }
        }
        else
#endif  // TMWA_SUPPORT
        {
            if (PacketLimiter::limitPackets(PacketType::PACKET_ONLINELIST))
            {
                mUpdateTimer = cur_time;
                playerHandler->requestOnlineList();
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
    if (mUpdateButton != nullptr)
        mUpdateButton->setWidth(area.width - 10);

    if (mScrollArea != nullptr)
        mScrollArea->setSize(area.width - 10, area.height - 10 - 30);
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
    return strprintf("@@%s|##%s%s@@", text.c_str(),
        color.c_str(), text.c_str());
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
    if (player == nullptr)
        return;

    if ((actorManager != nullptr) && (localPlayer != nullptr))
    {
        const std::string &nick = player->getNick();
        if (nick == localPlayer->getName())
        {
            player->setText("s");
            return;
        }
        if (localPlayer->isInParty())
        {
            const Party *const party = localPlayer->getParty();
            if (party != nullptr)
            {
                if (party->getMember(nick) != nullptr)
                {
                    player->setText("P");
                    return;
                }
            }
        }

        const Being *const being = actorManager->findBeingByName(nick,
            ActorType::Player);
        if (being != nullptr)
        {
            const Guild *const guild2 = localPlayer->getGuild();
            if (guild2 != nullptr)
            {
                const Guild *const guild1 = being->getGuild();
                if (guild1 != nullptr)
                {
                    if (guild1->getId() == guild2->getId()
                        || (guild2->getMember(nick) != nullptr))
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
        if ((guild3 != nullptr) && guild3->isMember(nick))
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

    if (mStatus != 255 && (actorManager != nullptr))
    {
        Being *const being = actorManager->findBeingByName(
            mNick, ActorType::Player);
        if (being != nullptr)
        {
            being->setState(mStatus);
            // for now highlight versions > 3
            if (mVersion > 3)
                being->setAdvanced(true);
        }
    }

#ifdef TMWA_SUPPORT
    if (mGroup != -1 && GroupDb::getShowBadge(mGroup))
    {
        const std::string &name = GroupDb::getName(mGroup);
        mText.append(strprintf("(%s) ", name.c_str()));
    }
    else
#endif
    {
        if ((mStatus != 255 && ((mStatus & BeingFlag::GM) != 0)) || mIsGM)
            mText.append("(GM) ");
    }

    if (mLevel > 0)
        mText.append(strprintf("%d", mLevel));

    if (mGender == Gender::FEMALE)
        mText.append("\u2640");
    else if (mGender == Gender::MALE)
        mText.append("\u2642");

#ifdef TMWA_SUPPORT
    if (mGroup != -1 && GroupDb::getHighlightName(mGroup) && color == "0")
        color = "2";
#endif

    if (mStatus > 0 && mStatus != 255)
    {
        if ((mStatus & BeingFlag::SHOP) != 0)
            mText.append("$");
        if ((mStatus & BeingFlag::AWAY) != 0)
        {
            // TRANSLATORS: this away status writed in player nick
            mText.append(_("A"));
        }
        if ((mStatus & BeingFlag::INACTIVE) != 0)
        {
            // TRANSLATORS: this inactive status writed in player nick
            mText.append(_("I"));
        }

        if (((mStatus & BeingFlag::GM) != 0) && color == "0")
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
