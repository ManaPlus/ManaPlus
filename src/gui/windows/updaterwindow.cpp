/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "gui/windows/updaterwindow.h"

#include "client.h"
#include "configuration.h"
#include "main.h"
#include "settings.h"

#include "enums/gui/layouttype.h"

#include "fs/files.h"
#include "fs/mkdir.h"
#include "fs/paths.h"

#include "fs/virtfs/fs.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/staticbrowserbox.h"

#include "net/download.h"
#include "net/updatetypeoperators.h"

#include "resources/db/moddb.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include <sys/stat.h>

#include <fstream>
#include <sstream>

#include "debug.h"

UpdaterWindow *updaterWindow = nullptr;

const std::string xmlUpdateFile("resources.xml");
const std::string txtUpdateFile("resources2.txt");
const std::string updateServer2
    ("http://download.manaplus.org/manaplus/updates/");
const std::string updateServer3
    ("http://download2.manaplus.org/manaplus/updates/");
const std::string updateServer4
    ("http://download.evolonline.org/manaplus/updates/");
const std::string updateServer5
    ("http://download3.manaplus.org/manaplus/updates/");

/**
 * Load the given file into a vector of updateFiles.
 */
static STD_VECTOR<UpdateFile> loadXMLFile(const std::string &fileName,
                                          const bool loadMods)
{
    STD_VECTOR<UpdateFile> files;
    XML::Document doc(fileName, UseVirtFs_false, SkipError_false);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "updates"))
    {
        logger->log("Error loading update file: %s", fileName.c_str());
        return files;
    }

    for_each_xml_child_node(fileNode, rootNode)
    {
        const bool isMod = xmlNameEqual(fileNode, "mod");
        if (!xmlNameEqual(fileNode, "update") && !isMod)
            continue;

        UpdateFile file;
        file.name = XML::getProperty(fileNode, "file", "");
        file.hash = XML::getProperty(fileNode, "hash", "");
        file.type = XML::getProperty(fileNode, "type", "data");
        file.desc = XML::getProperty(fileNode, "description", "");
        file.group = XML::getProperty(fileNode, "group", "");
        if (!file.group.empty() && (!isMod || !loadMods))
            continue;

        const std::string version = XML::getProperty(
            fileNode, "version", "");
        if (!version.empty())
        {
            if (version > CHECK_VERSION)
                continue;
        }
        const std::string notVersion = XML::getProperty(
            fileNode, "notVersion", "");
        if (!notVersion.empty())
        {
            if (notVersion <= CHECK_VERSION)
                continue;
        }
        if (XML::getProperty(fileNode, "required", "yes") == "yes")
            file.required = true;
        else
            file.required = false;

        if (checkPath(file.name))
            files.push_back(file);
    }

    return files;
}

static STD_VECTOR<UpdateFile> loadTxtFile(const std::string &fileName)
{
    STD_VECTOR<UpdateFile> files;
    std::ifstream fileHandler;
    fileHandler.open(fileName.c_str(), std::ios::in);

    if (fileHandler.is_open())
    {
        while (fileHandler.good())
        {
            char name[256];
            char hash[50];
            fileHandler.getline(name, 256, ' ');
            fileHandler.getline(hash, 50);

            UpdateFile thisFile;
            thisFile.name = name;
            thisFile.hash = hash;
            thisFile.type = "data";
            thisFile.group.clear();
            thisFile.required = true;
            thisFile.desc.clear();

            if (!thisFile.name.empty() && checkPath(thisFile.name))
                files.push_back(thisFile);
        }
    }
    else
    {
        logger->log("Error loading update file: %s", fileName.c_str());
    }
    fileHandler.close();

    return files;
}

UpdaterWindow::UpdaterWindow(const std::string &restrict updateHost,
                             const std::string &restrict updatesDir,
                             const bool applyUpdates,
                             const UpdateTypeT updateType) :
    // TRANSLATORS: updater window name
    Window(_("Updating..."), Modal_false, nullptr, "update.xml"),
    ActionListener(),
    KeyListener(),
    mDownloadProgress(0.0F),
    mUpdateHost(updateHost),
    mUpdatesDir(updatesDir),
    mUpdatesDirReal(updatesDir),
    mCurrentFile("news.txt"),
    mNewLabelCaption(),
    mDownloadMutex(),
    mCurrentChecksum(0),
    mMemoryBuffer(nullptr),
    mDownload(nullptr),
    mUpdateFiles(),
    mTempUpdateFiles(),
    mUpdateServerPath(mUpdateHost),
    mItemLinkHandler(new ItemLinkHandler),
    // TRANSLATORS: updater window label
    mLabel(new Label(this, _("Connecting..."))),
    // TRANSLATORS: updater window button
    mCancelButton(new Button(this, _("Cancel"), "cancel", BUTTON_SKIN, this)),
    // TRANSLATORS: updater window button
    mPlayButton(new Button(this, _("Play"), "play", BUTTON_SKIN, this)),
    mProgressBar(new ProgressBar(this, 0.0, 310, 0,
        ProgressColorId::PROG_UPDATE,
        "updateprogressbar.xml", "updateprogressbar_fill.xml")),
    mBrowserBox(new StaticBrowserBox(this, Opaque_true,
        "browserbox.xml")),
    mScrollArea(new ScrollArea(this, mBrowserBox,
        Opaque_true, "update_background.xml")),
    mDownloadStatus(UpdateDownloadStatus::UPDATE_NEWS),
    mDownloadedBytes(0),
    mUpdateIndex(0),
    mUpdateIndexOffset(0),
    mUpdateType(updateType),
    mStoreInMemory(true),
    mDownloadComplete(true),
    mUserCancel(false),
    mLoadUpdates(applyUpdates),
    mValidateXml(false),
    mSkipPatches(false)
{
    setWindowName("UpdaterWindow");
    setResizable(true);
    setDefaultSize(450, 400, ImagePosition::CENTER, 0, 0);
    setMinWidth(310);
    setMinHeight(220);

    mProgressBar->setSmoothProgress(false);
    mBrowserBox->setOpaque(Opaque_false);
    mBrowserBox->setLinkHandler(mItemLinkHandler);
    mBrowserBox->setProcessVars(true);
    mBrowserBox->setEnableKeys(true);
    mBrowserBox->setEnableTabs(true);
    mBrowserBox->setEnableImages(true);
    mPlayButton->setEnabled(false);

    ContainerPlacer placer(nullptr, nullptr);
    placer = getPlacer(0, 0);

    placer(0, 0, mScrollArea, 5, 3).setPadding(3);
    placer(0, 3, mLabel, 5, 1);
    placer(0, 4, mProgressBar, 5, 1);
    placer(3, 5, mCancelButton, 1, 1);
    placer(4, 5, mPlayButton, 1, 1);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    addKeyListener(this);

    if (mUpdateHost.empty())
    {
        const STD_VECTOR<std::string> &mirrors = settings.updateMirrors;
        if (mirrors.begin() != mirrors.end())
            mUpdateHost = *mirrors.begin();
        mSkipPatches = true;
    }

    loadWindowState();
}

void UpdaterWindow::postInit()
{
    Window::postInit();
    setVisible(Visible_true);
    mCancelButton->requestFocus();
    removeProtocol(mUpdateServerPath);

    download();
}

UpdaterWindow::~UpdaterWindow()
{
    if (mLoadUpdates)
        loadUpdates();

    if (mDownload != nullptr)
    {
        mDownload->cancel();

        delete2(mDownload)
    }
    free(mMemoryBuffer);
    delete2(mItemLinkHandler)
}

void UpdaterWindow::setProgress(const float p)
{
    // Do delayed progress bar update, since Guichan isn't thread-safe
    MutexLocker lock(&mDownloadMutex);
    mDownloadProgress = p;
}

void UpdaterWindow::setLabel(const std::string &str)
{
    // Do delayed label text update, since Guichan isn't thread-safe
    MutexLocker lock(&mDownloadMutex);
    mNewLabelCaption = str;
}

void UpdaterWindow::enable()
{
    mCancelButton->setEnabled(false);
    mPlayButton->setEnabled(true);
    mPlayButton->requestFocus();

    if (client->getState() != State::GAME)
    {
        if ((mUpdateType & UpdateType::Close) != 0)
            client->setState(State::LOAD_DATA);
    }
    else
    {
        deleteSelf();
    }
}

void UpdaterWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        // Register the user cancel
        mUserCancel = true;
        // Skip the updating process
        if (mDownloadStatus != UpdateDownloadStatus::UPDATE_COMPLETE)
        {
            if (mDownload != nullptr)
                mDownload->cancel();
            mDownloadStatus = UpdateDownloadStatus::UPDATE_ERROR;
        }
    }
    else if (eventId == "play")
    {
        if (client->getState() != State::GAME)
            client->setState(State::LOAD_DATA);
        else
            deleteSelf();
    }
}

void UpdaterWindow::keyPressed(KeyEvent &event)
{
    const InputActionT actionId = event.getActionId();
    if (actionId == InputAction::GUI_CANCEL)
    {
        action(ActionEvent(nullptr, mCancelButton->getActionEventId()));
        if (client->getState() != State::GAME)
            client->setState(State::LOGIN);
        else
            deleteSelf();
    }
    else if (actionId == InputAction::GUI_SELECT ||
             actionId == InputAction::GUI_SELECT2)
    {
        if (mDownloadStatus == UpdateDownloadStatus::UPDATE_COMPLETE ||
            mDownloadStatus == UpdateDownloadStatus::UPDATE_ERROR ||
            mDownloadStatus == UpdateDownloadStatus::UPDATE_IDLE)
        {
            action(ActionEvent(nullptr, mPlayButton->getActionEventId()));
        }
        else
        {
            action(ActionEvent(nullptr, mCancelButton->getActionEventId()));
        }
    }
}

void UpdaterWindow::loadNews()
{
    if (mMemoryBuffer == nullptr)
    {
        logger->log1("Couldn't load news");
        return;
    }

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(realloc(
        mMemoryBuffer, mDownloadedBytes + 1));
    if (mMemoryBuffer == nullptr)
    {
        logger->log1("Couldn't load news");
        return;
    }
    mMemoryBuffer[mDownloadedBytes] = '\0';
    mBrowserBox->clearRows();

    std::string newsName = mUpdatesDir + "/local/help/news.txt";
    mkdir_r((mUpdatesDir + "/local/help/").c_str());
    bool firstLine(true);
    std::ofstream file;
    std::stringstream ss(mMemoryBuffer);
    std::string line;
    file.open(newsName.c_str(), std::ios::out);
    int cnt = 0;
    const int maxNews = 50;
    while (std::getline(ss, line, '\n'))
    {
        cnt ++;
        if (firstLine)
        {
            firstLine = false;
            const size_t i = line.find("##9 Latest client version: ##6");
            if (i == 0U)
                continue;

            if (file.is_open())
                file << line << std::endl;
            if (cnt < maxNews)
                mBrowserBox->addRow(line, false);
        }
        else
        {
            if (file.is_open())
                file << line << std::endl;
            if (cnt < maxNews)
                mBrowserBox->addRow(line, false);
        }
    }

    file.close();
    if (cnt > maxNews)
    {
        mBrowserBox->addRow("", false);
        // TRANSLATORS: updater window checkbox
        mBrowserBox->addRow("news", _("Show all news (can be slow)"));
        mBrowserBox->addRow("", false);
    }
    // Free the memory buffer now that we don't need it anymore
    free(mMemoryBuffer);
    mMemoryBuffer = nullptr;
    mDownloadedBytes = 0;

    mBrowserBox->updateHeight();
    mScrollArea->setVerticalScrollAmount(0);
}

void UpdaterWindow::loadPatch()
{
    if (mMemoryBuffer == nullptr)
    {
        logger->log1("Couldn't load patch");
        return;
    }

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(
        realloc(mMemoryBuffer, mDownloadedBytes + 1));
    if (mMemoryBuffer == nullptr)
    {
        logger->log1("Couldn't load patch");
        return;
    }
    mMemoryBuffer[mDownloadedBytes] = '\0';

    std::string version;

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    if (line != nullptr)
    {
        version = line;
        if (serverVersion == 0)
        {
            line = strtok(nullptr, "\n");
            if (line != nullptr)
            {
                mBrowserBox->addRow(strprintf("##9 Latest client version: "
                    "##6ManaPlus %s##0", line), true);
            }
        }
        if (version > CHECK_VERSION)
        {
            mBrowserBox->addRow("", true);
#if defined(ANDROID)
            const std::string url = "androidDownloadUrl";
            const std::string text = "androidDownloadUrl";
#elif defined(WIN32)
            const std::string url = "windowsDownloadUrl";
            const std::string text = "windowsDownloadUrl";
#else  // defined(ANDROID)

            const std::string url = "otherDownloadUrl";
            const std::string text = "otherDownloadUrl";
#endif  // defined(ANDROID)

            mBrowserBox->addRow(std::string("  ##1[@@").append(
                branding.getStringValue(url)).append("|").append(
                branding.getStringValue(text)).append("@@]"), true);
            mBrowserBox->addRow("##1You can download it from", true);
            mBrowserBox->addRow("##1ManaPlus updated.", true);
        }
        else
        {
            mBrowserBox->addRow("You have latest client version.", true);
        }
    }

    // Free the memory buffer now that we don't need it anymore
    free(mMemoryBuffer);
    mMemoryBuffer = nullptr;
    mDownloadedBytes = 0;

    mBrowserBox->updateHeight();
    mScrollArea->setVerticalScrollAmount(0);
}

int UpdaterWindow::updateProgress(void *ptr,
                                  const DownloadStatusT status,
                                  size_t dt,
                                  const size_t dn)
{
    UpdaterWindow *const uw = reinterpret_cast<UpdaterWindow *>(ptr);
    if (uw == nullptr)
        return -1;

    if (status == DownloadStatus::Complete)
    {
        uw->mDownloadComplete = true;
    }
    else if (status == DownloadStatus::Error ||
             status == DownloadStatus::Cancelled)
    {
        if (uw->mDownloadStatus == UpdateDownloadStatus::UPDATE_COMPLETE ||
            uw->mDownloadStatus == UpdateDownloadStatus::UPDATE_NEWS)
        {   // ignoring error in last state (was UPDATE_PATCH)
            uw->mDownloadStatus = UpdateDownloadStatus::UPDATE_COMPLETE;
            uw->mDownloadComplete = true;
            free(uw->mMemoryBuffer);
            uw->mMemoryBuffer = nullptr;
        }
        else
        {
            uw->mDownloadStatus = UpdateDownloadStatus::UPDATE_ERROR;
        }
    }

    if (dt == 0U)
        dt = 1;

    float progress = static_cast<float>(dn) /
                     static_cast<float>(dt);

    if (progress != progress)
        progress = 0.0F;  // check for NaN
    if (progress < 0.0F)
        progress = 0.0F;  // no idea how this could ever happen,
                          // but why not check for it anyway.
    if (progress > 1.0F)
        progress = 1.0F;

    uw->setLabel(std::string(uw->mCurrentFile).append(" (")
        .append(toString(CAST_S32(progress * 100))).append("%)"));

    uw->setProgress(progress);

    if ((client->getState() != State::UPDATE &&
        client->getState() != State::GAME) ||
        uw->mDownloadStatus == UpdateDownloadStatus::UPDATE_ERROR)
    {
        // If the action was canceled return an error code to stop the mThread
        return -1;
    }

    return 0;
}

size_t UpdaterWindow::memoryWrite(void *ptr, size_t size,
                                  size_t nmemb, void *stream)
{
    UpdaterWindow *const uw = reinterpret_cast<UpdaterWindow *>(stream);
    const size_t totalMem = size * nmemb;
    if (uw == nullptr)
        return 0;
    uw->mMemoryBuffer = static_cast<char*>(realloc(uw->mMemoryBuffer,
        CAST_SIZE(uw->mDownloadedBytes) + totalMem));
    if (uw->mMemoryBuffer != nullptr)
    {
        memcpy(&(uw->mMemoryBuffer[uw->mDownloadedBytes]), ptr, totalMem);
        uw->mDownloadedBytes += CAST_S32(totalMem);
    }

    return totalMem;
}

void UpdaterWindow::download()
{
    if (mDownload != nullptr)
    {
        mDownload->cancel();
        delete mDownload;
    }
    if (mDownloadStatus == UpdateDownloadStatus::UPDATE_PATCH)
    {
        mDownload = new Net::Download(this,
            branding.getStringValue("updateMirror1") + mCurrentFile,
            &updateProgress,
            true, false, mValidateXml);
        for (int f = 2; f < 8; f ++)
        {
            const std::string url = branding.getStringValue(
                "updateMirror" + toString(f));
            if (!url.empty())
                mDownload->addMirror(url + mCurrentFile);
        }
    }
    else
    {
        mDownload = new Net::Download(this,
            urlJoin(mUpdateHost, mCurrentFile),
            &updateProgress,
            false, false, mValidateXml);

        if (mDownloadStatus == UpdateDownloadStatus::UPDATE_LIST2 ||
            mDownloadStatus == UpdateDownloadStatus::UPDATE_RESOURCES2)
        {
            const std::string str = urlJoin(mUpdateServerPath, mCurrentFile);
            mDownload->addMirror(updateServer3 + str);
            mDownload->addMirror(updateServer4 + str);
            mDownload->addMirror(updateServer5 + str);
        }
        else
        {
            const STD_VECTOR<std::string> &mirrors = settings.updateMirrors;
            FOR_EACH (STD_VECTOR<std::string>::const_iterator, it, mirrors)
            {
                mDownload->addMirror(pathJoin(*it,
                    mCurrentFile));
            }
        }
    }

    if (mStoreInMemory)
    {
        mDownload->setWriteFunction(&UpdaterWindow::memoryWrite);
    }
    else
    {
        if (mDownloadStatus == UpdateDownloadStatus::UPDATE_RESOURCES)
        {
            mDownload->setFile(pathJoin(mUpdatesDir, mCurrentFile),
                mCurrentChecksum);
        }
        else
        {
            mDownload->setFile(pathJoin(mUpdatesDir,
                mCurrentFile),
                -1);
        }
    }

    if (mDownloadStatus != UpdateDownloadStatus::UPDATE_RESOURCES)
        mDownload->noCache();

    setLabel(mCurrentFile + " (0%)");
    mDownloadComplete = false;

    mDownload->start();
}

void UpdaterWindow::loadUpdates()
{
    if (mUpdateFiles.empty())
    {   // updates not downloaded
        mUpdateFiles = loadXMLFile(pathJoin(mUpdatesDir, xmlUpdateFile),
            false);
        if (mUpdateFiles.empty())
        {
            logger->log("Warning this server does not have a"
                        " %s file falling back to %s", xmlUpdateFile.c_str(),
                        txtUpdateFile.c_str());
            mUpdateFiles = loadTxtFile(pathJoin(mUpdatesDir,
                txtUpdateFile));
        }
    }

    std::string fixPath = mUpdatesDir + "/fix";
    const unsigned sz = CAST_U32(mUpdateFiles.size());
    for (mUpdateIndex = 0; mUpdateIndex < sz; mUpdateIndex++)
    {
        const UpdateFile &file = mUpdateFiles[mUpdateIndex];
        if (!file.group.empty())
            continue;
        UpdaterWindow::addUpdateFile(mUpdatesDir,
            fixPath,
            file.name,
            Append_false);
    }
    loadManaPlusUpdates(mUpdatesDir);
    loadMods(mUpdatesDir, mUpdateFiles);
}

void UpdaterWindow::loadLocalUpdates(const std::string &dir)
{
    STD_VECTOR<UpdateFile> updateFiles = loadXMLFile(
        pathJoin(dir, xmlUpdateFile),
        false);

    if (updateFiles.empty())
    {
        logger->log("Warning this server does not have a"
                    " %s file falling back to %s", xmlUpdateFile.c_str(),
                    txtUpdateFile.c_str());
        updateFiles = loadTxtFile(pathJoin(dir,
            txtUpdateFile));
    }

    const std::string fixPath = dir + "/fix";
    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles.size());
         updateIndex < fsz;
         updateIndex++)
    {
        const UpdateFile &file = updateFiles[updateIndex];
        if (!file.group.empty())
            continue;
        UpdaterWindow::addUpdateFile(dir,
            fixPath,
            file.name,
            Append_false);
    }
    loadManaPlusUpdates(dir);
    loadMods(dir, updateFiles);
}

void UpdaterWindow::unloadUpdates(const std::string &dir)
{
    STD_VECTOR<UpdateFile> updateFiles = loadXMLFile(
        pathJoin(dir, xmlUpdateFile),
        true);

    if (updateFiles.empty())
    {
        updateFiles = loadTxtFile(pathJoin(dir,
            txtUpdateFile));
    }

    const std::string fixPath = dir + "/fix";
    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles.size());
         updateIndex < fsz;
         updateIndex++)
    {
        UpdaterWindow::removeUpdateFile(dir,
            fixPath,
            updateFiles[updateIndex].name);
    }
    unloadManaPlusUpdates(dir);
}

void UpdaterWindow::loadManaPlusUpdates(const std::string &dir)
{
    std::string fixPath = dir + "/fix";
    STD_VECTOR<UpdateFile> updateFiles = loadXMLFile(
        pathJoin(fixPath, xmlUpdateFile),
        false);

    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles.size());
         updateIndex < fsz;
         updateIndex++)
    {
        const UpdateFile &file = updateFiles[updateIndex];
        if (!file.group.empty())
            continue;
        const std::string name = file.name;
        if (strStartWith(name, "manaplus_"))
        {
            struct stat statbuf;
            std::string fileName = pathJoin(fixPath,
                name);
            if (stat(fileName.c_str(), &statbuf) == 0)
            {
                VirtFs::mountZip(fileName,
                    Append_false);
            }
        }
    }
}

void UpdaterWindow::unloadManaPlusUpdates(const std::string &dir)
{
    const std::string fixPath = dir + "/fix";
    const STD_VECTOR<UpdateFile> updateFiles = loadXMLFile(
        pathJoin(fixPath, xmlUpdateFile),
        true);

    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles.size());
         updateIndex < fsz;
         updateIndex++)
    {
        std::string name = updateFiles[updateIndex].name;
        if (strStartWith(name, "manaplus_"))
        {
            struct stat statbuf;
            const std::string file = pathJoin(
                fixPath, name);
            if (stat(file.c_str(), &statbuf) == 0)
                VirtFs::unmountZip(file);
        }
    }
}

void UpdaterWindow::addUpdateFile(const std::string &restrict path,
                                  const std::string &restrict fixPath,
                                  const std::string &restrict file,
                                  const Append append)
{
    const std::string tmpPath = pathJoin(path, file);
    if (append == Append_false)
        VirtFs::mountZip(tmpPath, append);

    const std::string fixFile = pathJoin(fixPath, file);
    struct stat statbuf;
    if (stat(fixFile.c_str(), &statbuf) == 0)
        VirtFs::mountZip(fixFile, append);

    if (append == Append_true)
        VirtFs::mountZip(tmpPath, append);
}

void UpdaterWindow::removeUpdateFile(const std::string &restrict path,
                                     const std::string &restrict fixPath,
                                     const std::string &restrict file)
{
    VirtFs::unmountZip(pathJoin(path, file));
    const std::string fixFile = pathJoin(fixPath, file);
    struct stat statbuf;
    if (stat(fixFile.c_str(), &statbuf) == 0)
        VirtFs::unmountZip(fixFile);
}

void UpdaterWindow::logic()
{
    BLOCK_START("UpdaterWindow::logic")
    // Update Scroll logic
    mScrollArea->logic();

    // Synchronize label caption when necessary
    {
        MutexLocker lock(&mDownloadMutex);

        if (mLabel->getCaption() != mNewLabelCaption)
        {
            mLabel->setCaption(mNewLabelCaption);
            mLabel->adjustSize();
        }

        mProgressBar->setProgress(mDownloadProgress);
        if (!mUpdateFiles.empty() &&
            CAST_SIZE(mUpdateIndex) <= mUpdateFiles.size())
        {
            mProgressBar->setText(strprintf("%u/%u", mUpdateIndex
                + mUpdateIndexOffset + 1, CAST_U32(
                mUpdateFiles.size()) + CAST_S32(
                mTempUpdateFiles.size()) + 1));
        }
        else
        {
            mProgressBar->setText("");
        }
    }

    switch (mDownloadStatus)
    {
        case UpdateDownloadStatus::UPDATE_ERROR:
            mBrowserBox->addRow("", false);
            // TRANSLATORS: update message
            mBrowserBox->addRow(_("##1  The update process is incomplete."),
                false);
            // TRANSLATORS: Continues "The update process is incomplete.".
            mBrowserBox->addRow(_("##1  It is strongly recommended that"),
                false);
            // TRANSLATORS: Begins "It is strongly recommended that".
            mBrowserBox->addRow(_("##1  you try again later."),
                false);
            if (mDownload != nullptr)
                mBrowserBox->addRow(mDownload->getError(), false);
            mBrowserBox->updateHeight();
            mScrollArea->setVerticalScrollAmount(
                    mScrollArea->getVerticalMaxScroll());
            mDownloadStatus = UpdateDownloadStatus::UPDATE_COMPLETE;
            break;
        case UpdateDownloadStatus::UPDATE_NEWS:
            if (mDownloadComplete)
            {
                // Parse current memory buffer as news and dispose of the data
                loadNews();

                mValidateXml = true;
                mCurrentFile = xmlUpdateFile;
                mStoreInMemory = false;
                mDownloadStatus = UpdateDownloadStatus::UPDATE_LIST;
                download();  // download() changes mDownloadComplete to false
            }
            break;
        case UpdateDownloadStatus::UPDATE_PATCH:
            if (mDownloadComplete)
            {
                // Parse current memory buffer as news and dispose of the data
                loadPatch();

                mUpdateHost = updateServer2 + mUpdateServerPath;
                mUpdatesDir = pathJoin(mUpdatesDir, "fix");
                mCurrentFile = xmlUpdateFile;
                mValidateXml = true;
                mStoreInMemory = false;
                mDownloadStatus = UpdateDownloadStatus::UPDATE_LIST2;
                download();
            }
            break;

        case UpdateDownloadStatus::UPDATE_LIST:
            if (mDownloadComplete)
            {
                if (mCurrentFile == xmlUpdateFile)
                {
                    mUpdateFiles = loadXMLFile(pathJoin(
                        mUpdatesDir, xmlUpdateFile),
                        true);

                    if (mUpdateFiles.empty())
                    {
                        logger->log("Warning this server does not have a %s"
                                    " file falling back to %s",
                                    xmlUpdateFile.c_str(),
                                    txtUpdateFile.c_str());

                        // If the resources.xml file fails,
                        // fall back onto a older version
                        mCurrentFile = txtUpdateFile;
                        mValidateXml = false;
                        mStoreInMemory = false;
                        mDownloadStatus = UpdateDownloadStatus::UPDATE_LIST;
                        download();
                        break;
                    }
                }
                else if (mCurrentFile == txtUpdateFile)
                {
                    mValidateXml = true;
                    mUpdateFiles = loadTxtFile(pathJoin(mUpdatesDir,
                        txtUpdateFile));
                }
                mStoreInMemory = false;
                mDownloadStatus = UpdateDownloadStatus::UPDATE_RESOURCES;
            }
            break;
        case UpdateDownloadStatus::UPDATE_RESOURCES:
            if (mDownloadComplete)
            {
                if (CAST_SIZE(mUpdateIndex) < mUpdateFiles.size())
                {
                    UpdateFile thisFile = mUpdateFiles[mUpdateIndex];
                    if (thisFile.type == "music"
                        && !config.getBoolValue("download-music"))
                    {
                        mUpdateIndex++;
                        break;
                    }
                    mCurrentFile = thisFile.name;
                    std::string checksum;
                    checksum = thisFile.hash;
                    std::stringstream ss(checksum);
                    ss >> std::hex >> mCurrentChecksum;

                    std::ifstream temp(pathJoin(mUpdatesDir,
                        mCurrentFile).c_str());

                    mValidateXml = false;
                    if (!temp.is_open() || !validateFile(pathJoin(
                        mUpdatesDir, mCurrentFile),
                        mCurrentChecksum))
                    {
                        temp.close();
                        download();
                    }
                    else
                    {
                        temp.close();
                        logger->log("%s already here", mCurrentFile.c_str());
                    }
                    mUpdateIndex++;
                }
                else
                {
                    if (!mSkipPatches)
                    {
                        // Download of updates completed
                        mCurrentFile = "latest.txt";
                        mStoreInMemory = true;
                        mDownloadStatus = UpdateDownloadStatus::UPDATE_PATCH;
                        mValidateXml = false;
                        download();  // download() changes
                                     // mDownloadComplete to false
                    }
                    else
                    {
                        mDownloadStatus =
                            UpdateDownloadStatus::UPDATE_COMPLETE;
                    }
                }
            }
            break;
        case UpdateDownloadStatus::UPDATE_LIST2:
            if (mDownloadComplete)
            {
                if (mCurrentFile == xmlUpdateFile)
                {
                    mTempUpdateFiles = loadXMLFile(pathJoin(
                        mUpdatesDir, xmlUpdateFile),
                        true);
                }
                mUpdateIndexOffset = mUpdateIndex;
                mUpdateIndex = 0;
                mValidateXml = true;
                mStoreInMemory = false;
                mDownloadStatus = UpdateDownloadStatus::UPDATE_RESOURCES2;
                download();
            }
            break;
        case UpdateDownloadStatus::UPDATE_RESOURCES2:
            if (mDownloadComplete)
            {
                mValidateXml = false;
                if (CAST_SIZE(mUpdateIndex)
                    < mTempUpdateFiles.size())
                {
                    const UpdateFile thisFile = mTempUpdateFiles[mUpdateIndex];
                    mCurrentFile = thisFile.name;
                    std::string checksum;
                    checksum = thisFile.hash;
                    std::stringstream ss(checksum);
                    ss >> std::hex >> mCurrentChecksum;

                    std::ifstream temp((pathJoin(mUpdatesDir,
                        mCurrentFile)).c_str());

                    if (!temp.is_open() || !validateFile(pathJoin(
                        mUpdatesDir, mCurrentFile),
                        mCurrentChecksum))
                    {
                        temp.close();
                        download();
                    }
                    else
                    {
                        temp.close();
                        logger->log("%s already here", mCurrentFile.c_str());
                    }
                    mUpdateIndex++;
                }
                else
                {
                    mUpdatesDir = mUpdatesDirReal;
                    mDownloadStatus = UpdateDownloadStatus::UPDATE_COMPLETE;
                }
            }
            break;
        case UpdateDownloadStatus::UPDATE_COMPLETE:
            mUpdatesDir = mUpdatesDirReal;
            enable();
            // TRANSLATORS: updater window label
            setLabel(_("Completed"));
            mDownloadStatus = UpdateDownloadStatus::UPDATE_IDLE;
            break;
        case UpdateDownloadStatus::UPDATE_IDLE:
            break;
        default:
            logger->log("UpdaterWindow::logic unknown status: "
                        + toString(CAST_U32(mDownloadStatus)));
            break;
    }
    BLOCK_END("UpdaterWindow::logic")
}

bool UpdaterWindow::validateFile(const std::string &filePath,
                                 const unsigned long hash)
{
    FILE *const file = fopen(filePath.c_str(), "rb");
    if (file == nullptr)
        return false;

    const unsigned long adler = Net::Download::fadler32(file);
    fclose(file);
    return adler == hash;
}

unsigned long UpdaterWindow::getFileHash(const std::string &filePath)
{
    int size = 0;
    const char *const buf = VirtFs::loadFile(filePath, size);
    if (buf == nullptr)
        return 0;
    unsigned long res = Net::Download::adlerBuffer(buf, size);
    delete [] buf;
    return res;
}

void UpdaterWindow::loadFile(std::string file)
{
    mBrowserBox->clearRows();
    trim(file);

    StringVect lines;
    Files::loadTextFileLocal(mUpdatesDir + "/local/help/news.txt", lines);

    for (size_t i = 0, sz = lines.size(); i < sz; ++i)
        mBrowserBox->addRow(lines[i], false);
    mBrowserBox->updateHeight();
}

void UpdaterWindow::loadMods(const std::string &dir,
                             const STD_VECTOR<UpdateFile> &updateFiles)
{
    ModDB::load();
    std::string modsString = serverConfig.getValue("mods", "");
    std::set<std::string> modsList;
    splitToStringSet(modsList, modsString, '|');

    const std::string fixPath = dir + "/fix";
    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles.size());
         updateIndex < fsz;
         updateIndex ++)
    {
        const UpdateFile &file = updateFiles[updateIndex];
        if (file.group.empty())
            continue;
        const std::set<std::string>::const_iterator
            it = modsList.find(file.group);
        if (it != modsList.end())
        {
            UpdaterWindow::addUpdateFile(dir,
                fixPath,
                file.name,
                Append_false);
        }
    }

    STD_VECTOR<UpdateFile> updateFiles2 = loadXMLFile(
        pathJoin(fixPath, xmlUpdateFile),
        true);

    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles2.size());
         updateIndex < fsz;
         updateIndex++)
    {
        const UpdateFile &file = updateFiles2[updateIndex];
        if (file.group.empty())
            continue;
        std::string name = file.name;
        if (strStartWith(name, "manaplus_"))
        {
            const std::set<std::string>::const_iterator
                it = modsList.find(file.group);
            if (it != modsList.end())
            {
                struct stat statbuf;
                std::string fileName = pathJoin(fixPath,
                    name);
                if (stat(fileName.c_str(), &statbuf) == 0)
                {
                    VirtFs::mountZip(fileName,
                        Append_false);
                }
            }
        }
    }

    loadDirMods(dir + "/local/");
}

void UpdaterWindow::loadDirMods(const std::string &dir)
{
    ModDB::load();
    const ModInfos &mods = ModDB::getAll();

    std::string modsString = serverConfig.getValue("mods", "");
    StringVect modsList;
    splitToStringVector(modsList, modsString, '|');
    FOR_EACH (StringVectCIter, it, modsList)
    {
        const std::string &name = *it;
        const ModInfoCIterator modIt = mods.find(name);
        if (modIt == mods.end())
            continue;
        const ModInfo *const mod = (*modIt).second;
        if (mod != nullptr)
        {
            const std::string &localDir = mod->getLocalDir();
            if (!localDir.empty())
            {
                VirtFs::mountDir(pathJoin(dir, localDir),
                    Append_false);
            }
        }
    }
}

void UpdaterWindow::unloadMods(const std::string &dir)
{
    const ModInfos &mods = ModDB::getAll();
    std::string modsString = serverConfig.getValue("mods", "");
    StringVect modsList;
    splitToStringVector(modsList, modsString, '|');
    FOR_EACH (StringVectCIter, it, modsList)
    {
        const std::string &name = *it;
        const ModInfoCIterator modIt = mods.find(name);
        if (modIt == mods.end())
            continue;
        const ModInfo *const mod = (*modIt).second;
        if (mod != nullptr)
        {
            const std::string &localDir = mod->getLocalDir();
            if (!localDir.empty())
                VirtFs::unmountDir(pathJoin(dir, localDir));
        }
    }
}

void UpdaterWindow::deleteSelf()
{
    scheduleDelete();
    updaterWindow = nullptr;
}
