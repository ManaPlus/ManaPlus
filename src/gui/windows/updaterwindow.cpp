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

#include "gui/windows/updaterwindow.h"

#include "client.h"
#include "configuration.h"
#include "main.h"
#include "settings.h"

#include "enums/gui/layouttype.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"

#include "net/download.h"
#include "net/updatetypeoperators.h"

#include "resources/db/moddb.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/files.h"
#include "utils/physfstools.h"
#include "utils/gettext.h"
#include "utils/mkdir.h"
#include "utils/paths.h"

#include <sys/stat.h>

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
static std::vector<UpdateFile> loadXMLFile(const std::string &fileName,
                                           const bool loadMods)
{
    std::vector<UpdateFile> files;
    XML::Document doc(fileName, UseResman_false, SkipError_false);
    const XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "updates"))
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

static std::vector<UpdateFile> loadTxtFile(const std::string &fileName)
{
    std::vector<UpdateFile> files;
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
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    // TRANSLATORS: updater window button
    mPlayButton(new Button(this, _("Play"), "play", this)),
    mProgressBar(new ProgressBar(this, 0.0, 310, 0,
        ProgressColorId::PROG_UPDATE,
        "updateprogressbar.xml", "updateprogressbar_fill.xml")),
    mBrowserBox(new BrowserBox(this, BrowserBoxMode::AUTO_SIZE, Opaque_true,
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
    setDefaultSize(450, 400, ImagePosition::CENTER);
    setMinWidth(310);
    setMinHeight(220);

    mProgressBar->setSmoothProgress(false);
    mBrowserBox->setOpaque(Opaque_false);
    mBrowserBox->setLinkHandler(mItemLinkHandler);
    mBrowserBox->setProcessVars(true);
    mBrowserBox->setEnableKeys(true);
    mBrowserBox->setEnableTabs(true);
    mPlayButton->setEnabled(false);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mScrollArea, 5, 3).setPadding(3);
    placer(0, 3, mLabel, 5);
    placer(0, 4, mProgressBar, 5);
    placer(3, 5, mCancelButton);
    placer(4, 5, mPlayButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    addKeyListener(this);

    if (mUpdateHost.empty())
    {
        const std::vector<std::string> &mirrors = settings.updateMirrors;
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

    if (mDownload)
    {
        mDownload->cancel();

        delete2(mDownload)
    }
    free(mMemoryBuffer);
    delete2(mItemLinkHandler);
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
        if (mUpdateType & UpdateType::Close)
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
            if (mDownload)
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
            mDownloadStatus == UpdateDownloadStatus::UPDATE_ERROR)
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
    if (!mMemoryBuffer)
    {
        logger->log1("Couldn't load news");
        return;
    }

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(realloc(
        mMemoryBuffer, mDownloadedBytes + 1));
    if (!mMemoryBuffer)
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
            if (!i)
                continue;

            if (file.is_open())
                file << line << std::endl;
            if (cnt < maxNews)
                mBrowserBox->addRow(line);
        }
        else
        {
            if (file.is_open())
                file << line << std::endl;
            if (cnt < maxNews)
                mBrowserBox->addRow(line);
        }
    }

    file.close();
    if (cnt > maxNews)
    {
        mBrowserBox->addRow("");
        // TRANSLATORS: updater window checkbox
        mBrowserBox->addRow("news", _("Show all news (can be slow)"));
        mBrowserBox->addRow("");
    }
    // Free the memory buffer now that we don't need it anymore
    free(mMemoryBuffer);
    mMemoryBuffer = nullptr;
    mDownloadedBytes = 0;

    mScrollArea->setVerticalScrollAmount(0);
}

void UpdaterWindow::loadPatch()
{
    if (!mMemoryBuffer)
    {
        logger->log1("Couldn't load patch");
        return;
    }

    // Reallocate and include terminating 0 character
    mMemoryBuffer = static_cast<char*>(
        realloc(mMemoryBuffer, mDownloadedBytes + 1));
    if (!mMemoryBuffer)
    {
        logger->log1("Couldn't load patch");
        return;
    }
    mMemoryBuffer[mDownloadedBytes] = '\0';

    std::string version;

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    if (line)
    {
        version = line;
        if (serverVersion < 1)
        {
            line = strtok(nullptr, "\n");
            if (line)
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

    mScrollArea->setVerticalScrollAmount(0);
}

int UpdaterWindow::updateProgress(void *ptr,
                                  const DownloadStatusT status,
                                  size_t dt,
                                  const size_t dn)
{
    UpdaterWindow *const uw = reinterpret_cast<UpdaterWindow *>(ptr);
    if (!uw)
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

    if (!dt)
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
    if (!uw)
        return 0;
    uw->mMemoryBuffer = static_cast<char*>(realloc(uw->mMemoryBuffer,
        CAST_SIZE(uw->mDownloadedBytes) + totalMem));
    if (uw->mMemoryBuffer)
    {
        memcpy(&(uw->mMemoryBuffer[uw->mDownloadedBytes]), ptr, totalMem);
        uw->mDownloadedBytes += CAST_S32(totalMem);
    }

    return totalMem;
}

void UpdaterWindow::download()
{
    if (mDownload)
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
            std::string(mUpdateHost).append("/").append(mCurrentFile),
            &updateProgress,
            false, false, mValidateXml);

        if (mDownloadStatus == UpdateDownloadStatus::UPDATE_LIST2 ||
            mDownloadStatus == UpdateDownloadStatus::UPDATE_RESOURCES2)
        {
            const std::string str = mUpdateServerPath + "/" + mCurrentFile;
            mDownload->addMirror(updateServer3 + str);
            mDownload->addMirror(updateServer4 + str);
            mDownload->addMirror(updateServer5 + str);
        }
        else
        {
            const std::vector<std::string> &mirrors = settings.updateMirrors;
            FOR_EACH (std::vector<std::string>::const_iterator, it, mirrors)
            {
                mDownload->addMirror(std::string(*it).append(
                    "/").append(mCurrentFile));
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
            mDownload->setFile(std::string(mUpdatesDir).append("/").append(
                mCurrentFile), mCurrentChecksum);
        }
        else
        {
            mDownload->setFile(std::string(mUpdatesDir).append(
                "/").append(mCurrentFile));
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
        mUpdateFiles = loadXMLFile(std::string(mUpdatesDir).append(
            "/").append(xmlUpdateFile), false);
        if (mUpdateFiles.empty())
        {
            logger->log("Warning this server does not have a"
                        " %s file falling back to %s", xmlUpdateFile.c_str(),
                        txtUpdateFile.c_str());
            mUpdateFiles = loadTxtFile(std::string(mUpdatesDir).append(
                "/").append(txtUpdateFile));
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
    std::vector<UpdateFile> updateFiles = loadXMLFile(
        std::string(dir).append("/").append(xmlUpdateFile), false);

    if (updateFiles.empty())
    {
        logger->log("Warning this server does not have a"
                    " %s file falling back to %s", xmlUpdateFile.c_str(),
                    txtUpdateFile.c_str());
        updateFiles = loadTxtFile(std::string(dir).append(
            "/").append(txtUpdateFile));
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
    std::vector<UpdateFile> updateFiles = loadXMLFile(
        std::string(dir).append("/").append(xmlUpdateFile), true);

    if (updateFiles.empty())
    {
        updateFiles = loadTxtFile(std::string(dir).append(
            "/").append(txtUpdateFile));
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
    std::vector<UpdateFile> updateFiles = loadXMLFile(
        std::string(fixPath).append("/").append(xmlUpdateFile), false);

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
            std::string fileName = std::string(fixPath).append(
                "/").append(name);
            if (!stat(fileName.c_str(), &statbuf))
                resourceManager->addToSearchPath(fileName, Append_false);
        }
    }
}

void UpdaterWindow::unloadManaPlusUpdates(const std::string &dir)
{
    const std::string fixPath = dir + "/fix";
    const std::vector<UpdateFile> updateFiles = loadXMLFile(
        std::string(fixPath).append("/").append(xmlUpdateFile), true);

    for (unsigned int updateIndex = 0,
         fsz = CAST_U32(updateFiles.size());
         updateIndex < fsz;
         updateIndex++)
    {
        std::string name = updateFiles[updateIndex].name;
        if (strStartWith(name, "manaplus_"))
        {
            struct stat statbuf;
            const std::string file = std::string(
                fixPath).append("/").append(name);
            if (!stat(file.c_str(), &statbuf))
                resourceManager->removeFromSearchPath(file);
        }
    }
}

void UpdaterWindow::addUpdateFile(const std::string &restrict path,
                                  const std::string &restrict fixPath,
                                  const std::string &restrict file,
                                  const Append append)
{
    const std::string tmpPath = std::string(path).append("/").append(file);
    if (append == Append_false)
        resourceManager->addToSearchPath(tmpPath, append);

    const std::string fixFile = std::string(fixPath).append("/").append(file);
    struct stat statbuf;
    if (!stat(fixFile.c_str(), &statbuf))
        resourceManager->addToSearchPath(fixFile, append);

    if (append == Append_false)
        resourceManager->addToSearchPath(tmpPath, append);
}

void UpdaterWindow::removeUpdateFile(const std::string &restrict path,
                                     const std::string &restrict fixPath,
                                     const std::string &restrict file)
{
    resourceManager->removeFromSearchPath(
        std::string(path).append("/").append(file));
    const std::string fixFile = std::string(fixPath).append("/").append(file);
    struct stat statbuf;
    if (!stat(fixFile.c_str(), &statbuf))
        resourceManager->removeFromSearchPath(fixFile);
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
        if (mUpdateFiles.size()
            && CAST_SIZE(mUpdateIndex) <= mUpdateFiles.size())
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
            mBrowserBox->addRow("");
            // TRANSLATORS: update message
            mBrowserBox->addRow(_("##1  The update process is incomplete."));
            // TRANSLATORS: Continues "The update process is incomplete.".
            mBrowserBox->addRow(_("##1  It is strongly recommended that"));
            // TRANSLATORS: Begins "It is strongly recommended that".
            mBrowserBox->addRow(_("##1  you try again later."));
            if (mDownload)
                mBrowserBox->addRow(mDownload->getError());
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
                mUpdatesDir.append("/fix");
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
                    mUpdateFiles = loadXMLFile(std::string(mUpdatesDir).append(
                        "/").append(xmlUpdateFile), true);

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
                    mUpdateFiles = loadTxtFile(std::string(mUpdatesDir).append(
                        "/").append(txtUpdateFile));
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

                    std::ifstream temp((std::string(mUpdatesDir).append(
                        "/").append(mCurrentFile)).c_str());

                    mValidateXml = false;
                    if (!temp.is_open() || !validateFile(std::string(
                        mUpdatesDir).append("/").append(mCurrentFile),
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
                    mTempUpdateFiles = loadXMLFile(std::string(
                        mUpdatesDir).append("/").append(xmlUpdateFile), true);
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

                    std::ifstream temp((std::string(mUpdatesDir).append(
                        "/").append(mCurrentFile)).c_str());

                    if (!temp.is_open() || !validateFile(std::string(
                        mUpdatesDir).append("/").append(mCurrentFile),
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
    if (!file)
        return false;

    const unsigned long adler = Net::Download::fadler32(file);
    fclose(file);
    return adler == hash;
}

unsigned long UpdaterWindow::getFileHash(const std::string &filePath)
{
    int size = 0;
    const char *const buf = static_cast<const char*>(
        PhysFs::loadFile(filePath, size));
    if (!buf)
        return 0;
    return Net::Download::adlerBuffer(buf, size);
}

void UpdaterWindow::loadFile(std::string file)
{
    mBrowserBox->clearRows();
    trim(file);

    StringVect lines;
    Files::loadTextFileLocal(mUpdatesDir + "/local/help/news.txt", lines);

    for (size_t i = 0, sz = lines.size(); i < sz; ++i)
        mBrowserBox->addRow(lines[i]);
}

void UpdaterWindow::loadMods(const std::string &dir,
                             const std::vector<UpdateFile> &updateFiles)
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

    std::vector<UpdateFile> updateFiles2 = loadXMLFile(
        std::string(fixPath).append("/").append(xmlUpdateFile), true);

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
                std::string fileName = std::string(fixPath).append(
                    "/").append(name);
                if (!stat(fileName.c_str(), &statbuf))
                    resourceManager->addToSearchPath(fileName, Append_false);
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
        if (mod)
        {
            const std::string &localDir = mod->getLocalDir();
            if (!localDir.empty())
            {
                resourceManager->addToSearchPath(dir + "/" + localDir,
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
        if (mod)
        {
            const std::string &localDir = mod->getLocalDir();
            if (!localDir.empty())
                resourceManager->removeFromSearchPath(dir + "/" + localDir);
        }
    }
}

void UpdaterWindow::deleteSelf()
{
    scheduleDelete();
    updaterWindow = nullptr;
}
