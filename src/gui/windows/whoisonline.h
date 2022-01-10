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

#ifndef GUI_WINDOWS_WHOISONLINE_H
#define GUI_WINDOWS_WHOISONLINE_H

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class OnlinePlayer;
class ScrollArea;
class StaticBrowserBox;

/**
 * Update progress window GUI
 *
 * \ingroup GUI
 */
class WhoIsOnline final : public Window,
                          public LinkHandler,
                          public ActionListener,
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
        ~WhoIsOnline() override final;

        void postInit() override final;

#ifdef TMWA_SUPPORT
        /**
         * Loads and display online list from the memory buffer.
         */
        void loadWebList();
#endif  // TMWA_SUPPORT

        void loadList(const STD_VECTOR<OnlinePlayer*> &list);

        void handleLink(const std::string& link,
                        MouseEvent *event) override final;

        void logic() override final;

        void slowLogic();

        void action(const ActionEvent &event) override final;

        void widgetResized(const Event &event) override final;

        const std::set<OnlinePlayer*> &getOnlinePlayers() const noexcept2
                                                        A_WARN_UNUSED
        { return mOnlinePlayers; }

        const std::set<std::string> &getOnlineNicks() const noexcept2
                                                    A_WARN_UNUSED
        { return mOnlineNicks; }

        void setAllowUpdate(const bool n) noexcept2
        { mAllowUpdate = n; }

        void optionChanged(const std::string &name) override final;

        void updateList(StringVect &list);

        void readFromWeb();

        static void setNeutralColor(OnlinePlayer *const player);

        void getPlayerNames(StringVect &names);

    private:
        void download();

        void updateSize();

        void handlerPlayerRelation(const std::string &nick,
                                   OnlinePlayer *const player);

#ifdef TMWA_SUPPORT
        /**
         * The thread function that download the files.
         */
        static int downloadThread(void *ptr);

        /**
         * A libcurl callback for writing to memory.
         */
        static size_t memoryWrite(void *restrict ptr,
                                  size_t size,
                                  size_t nmemb,
                                  FILE *restrict stream);
#endif  // TMWA_SUPPORT

        const std::string prepareNick(const std::string &restrict nick,
                                      const int level,
                                      const std::string &restrict color)
                                      const A_WARN_UNUSED;

        void updateWindow(size_t numOnline);

        enum DownloadStatus
        {
            UPDATE_ERROR = 0,
            UPDATE_COMPLETE,
            UPDATE_LIST
        };

        time_t mUpdateTimer;

        /** A thread that use libcurl to download updates. */
        SDL_Thread *mThread;

        /** Buffer for files downloaded to memory. */
        char *mMemoryBuffer;

        /** Buffer to handler human readable error provided by curl. */
        char *mCurlError A_NONNULLPOINTER;

        StaticBrowserBox *mBrowserBox A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        Button *mUpdateButton A_NONNULLPOINTER;

        std::set<OnlinePlayer*> mOnlinePlayers;
        std::set<std::string> mOnlineNicks;

        STD_VECTOR<OnlinePlayer*> mFriends;
        STD_VECTOR<OnlinePlayer*> mNeutral;
        STD_VECTOR<OnlinePlayer*> mDisregard;
        STD_VECTOR<OnlinePlayer*> mEnemy;

        /** Byte count currently downloaded in mMemoryBuffer. */
        int mDownloadedBytes;

        /** Status of the current download. */
        WhoIsOnline::DownloadStatus mDownloadStatus;

        /** Flag that show if current download is complete. */
        bool mDownloadComplete;
        bool mAllowUpdate;
        bool mShowLevel;
        bool mUpdateOnlineList;
        bool mGroupFriends;
#ifdef TMWA_SUPPORT
        bool mWebList;
#endif
        bool mServerSideList;
};

extern WhoIsOnline *whoIsOnline;

#endif  // GUI_WINDOWS_WHOISONLINE_H
