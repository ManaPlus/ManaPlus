/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#ifndef WHOISONLINE_H
#define WHOISONLINE_H

#include "configlistener.h"

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include "utils/stringvector.h"

#include "utils/mutex.h"

#include <set>

#include <guichan/actionlistener.hpp>

class BrowserBox;
class ScrollArea;

struct SDL_Thread;

class OnlinePlayer
{
    public:
        OnlinePlayer(std::string nick, unsigned char status,
                     char level, unsigned char gender, char version) :
            mNick(nick),
            mText(""),
            mStatus(status),
            mLevel(level),
            mVersion(version),
            mGender(gender)
        {
        }

        const std::string getNick() const
        { return mNick; }

        unsigned char getStaus() const
        { return mStatus; }

        char getVersion() const
        { return mVersion; }

        char getLevel() const
        { return mLevel; }

        const std::string getText() const
        { return mText; }

        void setText(std::string str);

        void setLevel(char level)
        { mLevel = level; }

    private:
        std::string mNick;

        std::string mText;

        unsigned char mStatus;

        char mLevel;

        char mVersion;

        unsigned char mGender;
};

/**
 * Update progress window GUI
 *
 * \ingroup GUI
 */
class WhoIsOnline : public Window,
                    public LinkHandler,
                    public gcn::ActionListener,
                    public ConfigListener
{
public:
    /**
     * Constructor.
     */
    WhoIsOnline();

    /**
     * Destructor
     */
    ~WhoIsOnline();

    /**
     * Loads and display online list from the memory buffer.
     */
    void loadWebList();

    void loadList(std::vector<OnlinePlayer*> &list);

    void handleLink(const std::string& link, gcn::MouseEvent *event);

    void logic();

    void slowLogic();

    void action(const gcn::ActionEvent &event);

    void widgetResized(const gcn::Event &event);

    const std::set<OnlinePlayer*> &getOnlinePlayers()
    { return mOnlinePlayers; }

    const std::set<std::string> &getOnlineNicks()
    { return mOnlineNicks; }

    void setAllowUpdate(bool n)
    { mAllowUpdate = n; }

    void optionChanged(const std::string &name);

    void updateList(StringVect &list);

    void readFromWeb();

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

    const std::string prepareNick(std::string nick, int level,
                                  std::string color) const;

    void updateWindow(std::vector<OnlinePlayer*> &friends,
                      std::vector<OnlinePlayer*> &neutral,
                      std::vector<OnlinePlayer*> &disregard,
                      std::vector<OnlinePlayer*> enemy,
                      int numOnline);

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

    gcn::Button *mUpdateButton;
    bool mAllowUpdate;
    bool mShowLevel;
    bool mUpdateOnlineList;
    bool mGroupFriends;
};

extern WhoIsOnline *whoIsOnline;

#endif
