/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

#ifndef GUI_WINDOWS_WHOISONLINE_H
#define GUI_WINDOWS_WHOISONLINE_H

#include "configlistener.h"

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include <set>

#include <guichan/actionlistener.hpp>

class BrowserBox;
class Button;
class ScrollArea;

struct SDL_Thread;

class OnlinePlayer final
{
    public:
        OnlinePlayer(const std::string &nick, const unsigned char status,
                     const signed char level, const unsigned char gender,
                     const signed char version) :
            mNick(nick),
            mText(""),
            mStatus(status),
            mLevel(level),
            mVersion(version),
            mGender(gender),
            mIsGM(false)
        {
        }

        A_DELETE_COPY(OnlinePlayer)

        const std::string getNick() const A_WARN_UNUSED
        { return mNick; }

        unsigned char getStaus() const A_WARN_UNUSED
        { return mStatus; }

        void setIsGM(const bool b)
        { mIsGM = b; }

        char getVersion() const A_WARN_UNUSED
        { return mVersion; }

        char getLevel() const A_WARN_UNUSED
        { return mLevel; }

        const std::string getText() const A_WARN_UNUSED
        { return mText; }

        void setText(std::string str);

        void setLevel(const char level)
        { mLevel = level; }

    private:
        std::string mNick;

        std::string mText;

        unsigned char mStatus;

        signed char mLevel;

        signed char mVersion;

        unsigned char mGender;

        bool mIsGM;
};

/**
 * Update progress window GUI
 *
 * \ingroup GUI
 */
class WhoIsOnline final : public Window,
                          public LinkHandler,
                          public gcn::ActionListener,
                          public ConfigListener
{
public:
    /**
     * Constructor.
     */
    WhoIsOnline();

    A_DELETE_COPY(WhoIsOnline)

    /**
     * Destructor
     */
    ~WhoIsOnline();

    void postInit() override final;

    /**
     * Loads and display online list from the memory buffer.
     */
    void loadWebList();

    void loadList(std::vector<OnlinePlayer*> &list);

    void handleLink(const std::string& link,
                    gcn::MouseEvent *event) override final;

    void logic() override final;

    void slowLogic();

    void action(const gcn::ActionEvent &event) override final;

    void widgetResized(const gcn::Event &event) override final;

    const std::set<OnlinePlayer*> &getOnlinePlayers() const A_WARN_UNUSED
    { return mOnlinePlayers; }

    const std::set<std::string> &getOnlineNicks() const A_WARN_UNUSED
    { return mOnlineNicks; }

    void setAllowUpdate(const bool n)
    { mAllowUpdate = n; }

    void optionChanged(const std::string &name) override final;

    void updateList(StringVect &list);

    void readFromWeb();

    void setNeutralColor(OnlinePlayer *const player);

    void getPlayerNames(StringVect &names);

private:
    void download();

    void updateSize();

    /**
     * The thread function that download the files.
     */
    static int downloadThread(void *ptr);

    /**
     * A libcurl callback for writing to memory.
     */
    static size_t memoryWrite(void *ptr, size_t size, size_t nmemb,
                              FILE *stream);

    const std::string prepareNick(const std::string &restrict nick,
                                  const int level,
                                  const std::string &restrict color)
                                  const A_WARN_UNUSED;

    void updateWindow(std::vector<OnlinePlayer*> &restrict friends,
                      std::vector<OnlinePlayer*> &restrict neutral,
                      std::vector<OnlinePlayer*> &restrict disregard,
                      std::vector<OnlinePlayer*> &restrict enemy,
                      size_t numOnline);

    enum DownloadStatus
    {
        UPDATE_ERROR = 0,
        UPDATE_COMPLETE,
        UPDATE_LIST
    };

    /** A thread that use libcurl to download updates. */
    SDL_Thread *mThread;

    /** Status of the current download. */
    DownloadStatus mDownloadStatus;

    /** Flag that show if current download is complete. */
    bool mDownloadComplete;

    /** Byte count currently downloaded in mMemoryBuffer. */
    int mDownloadedBytes;

    /** Buffer for files downloaded to memory. */
    char *mMemoryBuffer;

    /** Buffer to handler human readable error provided by curl. */
    char *mCurlError;

    BrowserBox *mBrowserBox;
    ScrollArea *mScrollArea;
    time_t mUpdateTimer;
    std::set<OnlinePlayer*> mOnlinePlayers;
    std::set<std::string> mOnlineNicks;

    Button *mUpdateButton;
    bool mAllowUpdate;
    bool mShowLevel;
    bool mUpdateOnlineList;
    bool mGroupFriends;
};

extern WhoIsOnline *whoIsOnline;

#endif  // GUI_WINDOWS_WHOISONLINE_H
