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

#ifndef GUI_WINDOWS_UPDATERWINDOW_H
#define GUI_WINDOWS_UPDATERWINDOW_H

#include "enums/gui/updatedownloadstatus.h"

#include "enums/net/downloadstatus.h"
#include "enums/net/updatetype.h"

#include "enums/simpletypes/append.h"

#include "gui/widgets/window.h"

#include "resources/updatefile.h"

#include "utils/mutex.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class BrowserBox;
class Button;
class ItemLinkHandler;
class Label;
class ProgressBar;
class ScrollArea;

namespace Net
{
    class Download;
}

/**
 * Update progress window GUI
 *
 * \ingroup GUI
 */
class UpdaterWindow final : public Window,
                            public ActionListener,
                            public KeyListener
{
    public:
        /**
         * Constructor.
         *
         * @param updateHost Host where to get the updated files.
         * @param updatesDir Directory where to store updates (should be
         *  absolute and already created).
         * @param applyUpdates If true, the update window will pass the updates
         *  to teh resource manager.
         */
        UpdaterWindow(const std::string &restrict updateHost,
                      const std::string &restrict updatesDir,
                      const bool applyUpdates,
                      const UpdateTypeT updateType);

        A_DELETE_COPY(UpdaterWindow)

        /**
         * Destructor
         */
        ~UpdaterWindow();

        void postInit() override final;

        /**
         * Set's progress bar status
         */
        void setProgress(const float p);

        /**
         * Set's label above progress
         */
        void setLabel(const std::string &);

        /**
         * Enables play button
         */
        void enable();

        /**
         * Loads and display news. Assumes the news file contents have been
         * loaded into the memory buffer.
         */
        void loadNews();

        void loadPatch();

        void action(const ActionEvent &event) override final;

        void keyPressed(KeyEvent &event) override final;

        void logic() override final;

        void loadFile(std::string file);

        void deleteSelf();

        static void loadLocalUpdates(const std::string &dir);

        static void unloadUpdates(const std::string &dir);

        static void addUpdateFile(const std::string &restrict path,
                                  const std::string &restrict fixPath,
                                  const std::string &restrict file,
                                  const Append append);

        static void removeUpdateFile(const std::string &restrict path,
                                     const std::string &restrict fixPath,
                                     const std::string &filerestrict);

        static void loadManaPlusUpdates(const std::string &dir);

        static void unloadManaPlusUpdates(const std::string &dir);

        static unsigned long getFileHash(const std::string &filePath);

        static void loadMods(const std::string &dir,
                             const std::vector<UpdateFile> &updateFiles);

        static void loadDirMods(const std::string &dir);

        static void unloadMods(const std::string &dir);

    private:
        void download();

        /**
         * Loads the updates this window has gotten into the resource manager
         */
        void loadUpdates();

        /**
         * A download callback for progress updates.
         */
        static int updateProgress(void *ptr,
                                  const DownloadStatusT status,
                                  size_t dt,
                                  const size_t dn);

        /**
         * A libcurl callback for writing to memory.
         */
        static size_t memoryWrite(void *ptr, size_t size, size_t nmemb,
                                  void *stream);

        static bool validateFile(const std::string &filePath,
                                 const unsigned long hash) A_WARN_UNUSED;

        /** The new progress value to be set in the logic method. */
        float mDownloadProgress;

        /** Host where we get the updated files. */
        std::string mUpdateHost;

        /** Place where the updates are stored (absolute path). */
        std::string mUpdatesDir;

        std::string mUpdatesDirReal;

        /** The file currently downloading. */
        std::string mCurrentFile;

        /** The new label caption to be set in the logic method. */
        std::string mNewLabelCaption;

        // The mutex used to guard access to mNewLabelCaption
        // and mDownloadProgress.
        Mutex mDownloadMutex;

        /** The Adler32 checksum of the file currently downloading. */
        unsigned long mCurrentChecksum;

        /** Buffer for files downloaded to memory. */
        char *mMemoryBuffer;

        /** Download handle. */
        Net::Download *mDownload;

        /** List of files to download. */
        std::vector<UpdateFile> mUpdateFiles;

        /** List of temp files to download. */
        std::vector<UpdateFile> mTempUpdateFiles;

        std::string mUpdateServerPath;

        ItemLinkHandler *mItemLinkHandler A_NONNULLPOINTER;
        Label *mLabel A_NONNULLPOINTER;
        Button *mCancelButton A_NONNULLPOINTER;
        Button *mPlayButton A_NONNULLPOINTER;
        ProgressBar *mProgressBar A_NONNULLPOINTER;
        BrowserBox *mBrowserBox A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;

        /** Status of the current download. */
        UpdateDownloadStatusT mDownloadStatus;

        /** Byte count currently downloaded in mMemoryBuffer. */
        int mDownloadedBytes;

        /** Index of the file to be downloaded. */
        unsigned int mUpdateIndex;

        /** Index offset for disaplay downloaded file. */
        unsigned int mUpdateIndexOffset;

        UpdateTypeT mUpdateType;

        /** A flag to indicate whether to use a memory buffer or a regular
         * file.
         */
        bool mStoreInMemory;

        /** Flag that show if current download is complete. */
        bool mDownloadComplete;

        /** Flag that show if the user has canceled the update. */
        bool mUserCancel;

        /** Tells ~UpdaterWindow() if it should load updates */
        bool mLoadUpdates;

        bool mValidateXml;

        bool mSkipPatches;
};

extern UpdaterWindow *updaterWindow;

#endif  // GUI_WINDOWS_UPDATERWINDOW_H
