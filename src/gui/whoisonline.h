/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef WHOISONLINE_H
#define WHOISONLINE_H

#include <string>
#include <set>

#include "configlistener.h"

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include "../utils/mutex.h"

#include <guichan/actionlistener.hpp>

class BrowserBox;
class ScrollArea;

struct SDL_Thread;

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
    void loadList();

    void handleLink(const std::string& link, gcn::MouseEvent *event);

    void logic();

    void action(const gcn::ActionEvent &event);

    void widgetResized(const gcn::Event &event);

    std::set<std::string> &getOnlinePlayers()
    { return mOnlinePlayers; }

    void setAllowUpdate(bool n)
    { mAllowUpdate = n; }

    void optionChanged(const std::string &name);

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
    std::set<std::string> mOnlinePlayers;

    gcn::Button *mUpdateButton;
    bool mAllowUpdate;
    bool mShowLevel;
    bool mUpdateOnlineList;
};

#endif
