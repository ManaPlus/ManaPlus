/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/updaterwindow.h"

#include "client.h"
#include "configuration.h"
#include "keydata.h"
#include "keyevent.h"

#include "gui/sdlinput.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"

#include "net/logindata.h"

#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/mkdir.h"
#include "utils/paths.h"
#include "utils/process.h"

#include <iostream>
#include <fstream>

#include <sys/stat.h>

#include "debug.h"

const std::string xmlUpdateFile = "resources.xml";
const std::string txtUpdateFile = "resources2.txt";
const std::string updateServer2
    = "http://download.evolonline.org/manaplus/updates/";

std::vector<UpdateFile> loadXMLFile(const std::string &fileName);
std::vector<UpdateFile> loadTxtFile(const std::string &fileName);

/**
 * Load the given file into a vector of updateFiles.
 */
std::vector<UpdateFile> loadXMLFile(const std::string &fileName)
{
    std::vector<UpdateFile> files;
    XML::Document doc(fileName, false);
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "updates"))
    {
        logger->log("Error loading update file: %s", fileName.c_str());
        return files;
    }

    for_each_xml_child_node(fileNode, rootNode)
    {
        if (!xmlNameEqual(fileNode, "update"))
            continue;

        if (XML::getProperty(fileNode, "group", "default") != "default")
            continue;

        UpdateFile file;
        file.name = XML::getProperty(fileNode, "file", "");
        file.hash = XML::getProperty(fileNode, "hash", "");
        file.type = XML::getProperty(fileNode, "type", "data");
        file.desc = XML::getProperty(fileNode, "description", "");
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

std::vector<UpdateFile> loadTxtFile(const std::string &fileName)
{
    std::vector<UpdateFile> files;
    std::ifstream fileHandler;
    fileHandler.open(fileName.c_str(), std::ios::in);

    if (fileHandler.is_open())
    {
        while (fileHandler.good())
        {
            char name[256], hash[50];
            fileHandler.getline(name, 256, ' ');
            fileHandler.getline(hash, 50);

            UpdateFile thisFile;
            thisFile.name = name;
            thisFile.hash = hash;
            thisFile.type = "data";
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

UpdaterWindow::UpdaterWindow(const std::string &updateHost,
                             const std::string &updatesDir,
                             const bool applyUpdates,
                             const int updateType):
    // TRANSLATORS: updater window name
    Window(_("Updating..."), false, nullptr, "update.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    mDownloadStatus(UPDATE_NEWS),
    mUpdateHost(updateHost),
    mUpdatesDir(updatesDir),
    mUpdatesDirReal(updatesDir),
    mCurrentFile("news.txt"),
    mNewLabelCaption(),
    mDownloadProgress(0.0f),
    mDownloadMutex(),
    mCurrentChecksum(0),
    mStoreInMemory(true),
    mDownloadComplete(true),
    mUserCancel(false),
    mDownloadedBytes(0),
    mMemoryBuffer(nullptr),
    mDownload(nullptr),
    mUpdateFiles(),
    mTempUpdateFiles(),
    mUpdateIndex(0),
    mUpdateIndexOffset(0),
    mLoadUpdates(applyUpdates),
    mUpdateType(updateType),
    // TRANSLATORS: updater window label
    mLabel(new Label(this, _("Connecting..."))),
    // TRANSLATORS: updater window button
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    // TRANSLATORS: updater window button
    mPlayButton(new Button(this, _("Play"), "play", this)),
    mProgressBar(new ProgressBar(this, 0.0, 310, 0)),
    mBrowserBox(new BrowserBox(this)),
    mScrollArea(new ScrollArea(mBrowserBox, true, "update_background.xml")),
    mUpdateServerPath(mUpdateHost)
{
    setWindowName("UpdaterWindow");
    setResizable(true);
    setDefaultSize(450, 400, ImageRect::CENTER);
    setMinWidth(310);
    setMinHeight(220);

    mProgressBar->setSmoothProgress(false);
    mBrowserBox->setOpaque(false);
    mBrowserBox->setLinkHandler(this);
    mPlayButton->setEnabled(false);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mScrollArea, 5, 3).setPadding(3);
    placer(0, 3, mLabel, 5);
    placer(0, 4, mProgressBar, 5);
    placer(3, 5, mCancelButton);
    placer(4, 5, mPlayButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    addKeyListener(this);

    loadWindowState();
    setVisible(true);
    mCancelButton->requestFocus();

    removeProtocol(mUpdateServerPath);

    // Try to download the updates list
    download();
}

UpdaterWindow::~UpdaterWindow()
{
    if (mLoadUpdates)
        loadUpdates();

    if (mDownload)
    {
        mDownload->cancel();

        delete mDownload;
        mDownload = nullptr;
    }
    free(mMemoryBuffer);
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

    if (mUpdateType & LoginData::Upd_Close)
        Client::setState(STATE_LOAD_DATA);
}

void UpdaterWindow::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        // Register the user cancel
        mUserCancel = true;
        // Skip the updating process
        if (mDownloadStatus != UPDATE_COMPLETE)
        {
            mDownload->cancel();
            mDownloadStatus = UPDATE_ERROR;
        }
    }
    else if (eventId == "play")
    {
        Client::setState(STATE_LOAD_DATA);
    }
}

void UpdaterWindow::keyPressed(gcn::KeyEvent &keyEvent)
{
    const int actionId = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        action(gcn::ActionEvent(nullptr, mCancelButton->getActionEventId()));
        Client::setState(STATE_LOGIN);
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        if (mDownloadStatus == UPDATE_COMPLETE ||
            mDownloadStatus == UPDATE_ERROR)
        {
            action(gcn::ActionEvent(nullptr, mPlayButton->getActionEventId()));
        }
        else
        {
            action(gcn::ActionEvent(nullptr,
                mCancelButton->getActionEventId()));
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

    mMemoryBuffer[mDownloadedBytes] = '\0';
    mBrowserBox->clearRows();

    std::string newsName = mUpdatesDir + "/local/help/news.txt";
    mkdir_r((mUpdatesDir + "/local/help/").c_str());
    bool firstLine(true);
    std::ofstream file;
    std::stringstream ss(mMemoryBuffer);
    std::string line;
    file.open(newsName.c_str(), std::ios::out);
    while (std::getline(ss, line, '\n'))
    {
        if (firstLine)
        {
            firstLine = false;
            const size_t i = line.find("##9 Latest client version: ##6");
            if (!i)
                continue;

            if (file.is_open())
                file << line << std::endl;
            mBrowserBox->addRow(line);
        }
        else
        {
            if (file.is_open())
                file << line << std::endl;
            mBrowserBox->addRow(line);
        }
    }

    file.close();
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
#if defined(ANDROID)
            mBrowserBox->addRow("", true);
            mBrowserBox->addRow("##1You can download from [[@@"
                "https://play.google.com/store/apps/details?id=org.evolonline"
                ".beta.manaplus|Google Play@@]", true);
            mBrowserBox->addRow("##1ManaPlus updated.", true);
#elif defined(WIN32)
            mBrowserBox->addRow("", true);
            mBrowserBox->addRow("  ##1[@@http://download.evolonline.org/"
                "manaplus/download/manaplus-win32.exe|download here@@]", true);
#else
            mBrowserBox->addRow("", true);
            mBrowserBox->addRow("  ##1@@http://manaplus.evolonline.org/|"
                "http://manaplus.evolonline.org/@@", true);
            mBrowserBox->addRow("##1You can download it from", true);
            mBrowserBox->addRow("##1ManaPlus updated.", true);
#endif
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

int UpdaterWindow::updateProgress(void *ptr, DownloadStatus status,
                                  size_t dt, size_t dn)
{
    UpdaterWindow *const uw = reinterpret_cast<UpdaterWindow *>(ptr);
    if (!uw)
        return -1;

    if (status == DOWNLOAD_STATUS_COMPLETE)
    {
        uw->mDownloadComplete = true;
    }
    else if (status == DOWNLOAD_STATUS_ERROR ||
             status == DOWNLOAD_STATUS_CANCELLED)
    {
        if (uw->mDownloadStatus == UPDATE_COMPLETE)
        {   // ignoring error in last state (was UPDATE_PATCH)
            uw->mDownloadStatus = UPDATE_COMPLETE;
            uw->mDownloadComplete = true;
            free(uw->mMemoryBuffer);
            uw->mMemoryBuffer = nullptr;
        }
        else
        {
            uw->mDownloadStatus = UPDATE_ERROR;
        }
    }

    if (!dt)
        dt = 1;

    float progress = static_cast<float>(dn) /
                     static_cast<float>(dt);

    if (progress != progress)
        progress = 0.0f;  // check for NaN
    if (progress < 0.0f)
        progress = 0.0f;  // no idea how this could ever happen,
                          // but why not check for it anyway.
    if (progress > 1.0f)
        progress = 1.0f;

    uw->setLabel(std::string(uw->mCurrentFile).append(" (")
        .append(toString(static_cast<int>(progress * 100))).append("%)"));

    uw->setProgress(progress);

    if (Client::getState() != STATE_UPDATE
        || uw->mDownloadStatus == UPDATE_ERROR)
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
    uw->mMemoryBuffer = static_cast<char*>(realloc(uw->mMemoryBuffer,
                                           uw->mDownloadedBytes + totalMem));
    if (uw->mMemoryBuffer)
    {
        memcpy(&(uw->mMemoryBuffer[uw->mDownloadedBytes]), ptr, totalMem);
        uw->mDownloadedBytes += static_cast<int>(totalMem);
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
    if (mDownloadStatus == UPDATE_PATCH)
    {
        mDownload = new Net::Download(this,
            "http://manaplus.evolonline.org/update/" + mCurrentFile,
            updateProgress, true);
    }
    else
    {
        mDownload = new Net::Download(this, std::string(mUpdateHost).append(
            "/").append(mCurrentFile), updateProgress);
    }

    if (mStoreInMemory)
    {
        mDownload->setWriteFunction(UpdaterWindow::memoryWrite);
    }
    else
    {
        if (mDownloadStatus == UPDATE_RESOURCES)
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

    if (mDownloadStatus != UPDATE_RESOURCES)
        mDownload->noCache();

    setLabel(mCurrentFile + " (0%)");
    mDownloadComplete = false;

    mDownload->start();
}

void UpdaterWindow::loadUpdates()
{
    const ResourceManager *const resman = ResourceManager::getInstance();

    if (mUpdateFiles.empty())
    {   // updates not downloaded
        mUpdateFiles = loadXMLFile(std::string(mUpdatesDir).append(
            "/").append(xmlUpdateFile));
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
    const unsigned sz = static_cast<unsigned>(mUpdateFiles.size());
    for (mUpdateIndex = 0; mUpdateIndex < sz; mUpdateIndex++)
    {
        UpdaterWindow::addUpdateFile(resman, mUpdatesDir, fixPath,
            mUpdateFiles[mUpdateIndex].name, false);
    }
    loadManaPlusUpdates(mUpdatesDir, resman);
}

void UpdaterWindow::loadLocalUpdates(const std::string &dir)
{
    const ResourceManager *const resman = ResourceManager::getInstance();

    std::vector<UpdateFile> updateFiles
        = loadXMLFile(std::string(dir).append("/").append(xmlUpdateFile));

    if (updateFiles.empty())
    {
        logger->log("Warning this server does not have a"
                    " %s file falling back to %s", xmlUpdateFile.c_str(),
                    txtUpdateFile.c_str());
        updateFiles = loadTxtFile(std::string(dir).append(
            "/").append(txtUpdateFile));
    }

    std::string fixPath = dir + "/fix";
    for (unsigned int updateIndex = 0, sz = static_cast<unsigned int>(
         updateFiles.size()); updateIndex < sz; updateIndex ++)
    {
        UpdaterWindow::addUpdateFile(resman, dir, fixPath,
            updateFiles[updateIndex].name, false);
    }
    loadManaPlusUpdates(dir, resman);
}

void UpdaterWindow::loadManaPlusUpdates(const std::string &dir,
                                        const ResourceManager *const resman)
{
    std::string fixPath = dir + "/fix";
    std::vector<UpdateFile> updateFiles
        = loadXMLFile(std::string(fixPath).append("/").append(xmlUpdateFile));

    for (unsigned int updateIndex = 0, sz = static_cast<unsigned int>(
         updateFiles.size()); updateIndex < sz; updateIndex ++)
    {
        std::string name = updateFiles[updateIndex].name;
        if (strStartWith(name, "manaplus_"))
        {
            struct stat statbuf;
            std::string file = std::string(fixPath).append("/").append(name);
            if (!stat(file.c_str(), &statbuf))
                resman->addToSearchPath(file, false);
        }
    }
}

void UpdaterWindow::addUpdateFile(const ResourceManager *const resman,
                                  const std::string &path,
                                  const std::string &fixPath,
                                  const std::string &file,
                                  const bool append)
{
    const std::string tmpPath = std::string(path).append("/").append(file);
    if (!append)
        resman->addToSearchPath(tmpPath, append);

    const std::string fixFile = std::string(fixPath).append("/").append(file);
    struct stat statbuf;
    if (!stat(fixFile.c_str(), &statbuf))
        resman->addToSearchPath(fixFile, append);

    if (append)
        resman->addToSearchPath(tmpPath, append);
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
        if (mUpdateFiles.size() && mUpdateIndex <= mUpdateFiles.size())
        {
            mProgressBar->setText(strprintf("%u/%u", mUpdateIndex
                + mUpdateIndexOffset + 1, static_cast<unsigned>(
                mUpdateFiles.size()) + static_cast<int>(
                mTempUpdateFiles.size()) + 1));
        }
        else
        {
            mProgressBar->setText("");
        }
    }

    switch (mDownloadStatus)
    {
        case UPDATE_ERROR:
            mBrowserBox->addRow("");
            // TRANSLATORS: update message
            mBrowserBox->addRow(_("##1  The update process is incomplete."));
            // TRANSLATORS: Continues "The update process is incomplete.".
            mBrowserBox->addRow(_("##1  It is strongly recommended that"));
            // TRANSLATORS: Begins "It is strongly recommended that".
            mBrowserBox->addRow(_("##1  you try again later."));

            mBrowserBox->addRow(mDownload->getError());
            mScrollArea->setVerticalScrollAmount(
                    mScrollArea->getVerticalMaxScroll());
            mDownloadStatus = UPDATE_COMPLETE;
            break;
        case UPDATE_NEWS:
            if (mDownloadComplete)
            {
                // Parse current memory buffer as news and dispose of the data
                loadNews();

                mCurrentFile = xmlUpdateFile;
                mStoreInMemory = false;
                mDownloadStatus = UPDATE_LIST;
                download();  // download() changes mDownloadComplete to false
            }
            break;
        case UPDATE_PATCH:
            if (mDownloadComplete)
            {
                // Parse current memory buffer as news and dispose of the data
                loadPatch();

                mUpdateHost = updateServer2 + mUpdateServerPath;
                mUpdatesDir.append("/fix");
                mCurrentFile = xmlUpdateFile;
                mStoreInMemory = false;
                mDownloadStatus = UPDATE_LIST2;
                download();
            }
            break;

        case UPDATE_LIST:
            if (mDownloadComplete)
            {
                if (mCurrentFile == xmlUpdateFile)
                {
                    mUpdateFiles = loadXMLFile(std::string(mUpdatesDir).append(
                        "/").append(xmlUpdateFile));

                    if (mUpdateFiles.empty())
                    {
                        logger->log("Warning this server does not have a %s"
                                    " file falling back to %s",
                                    xmlUpdateFile.c_str(),
                                    txtUpdateFile.c_str());

                        // If the resources.xml file fails,
                        // fall back onto a older version
                        mCurrentFile = txtUpdateFile;
                        mStoreInMemory = false;
                        mDownloadStatus = UPDATE_LIST;
                        download();
                        break;
                    }
                }
                else if (mCurrentFile == txtUpdateFile)
                {
                    mUpdateFiles = loadTxtFile(std::string(mUpdatesDir).append(
                        "/").append(txtUpdateFile));
                }
                mStoreInMemory = false;
                mDownloadStatus = UPDATE_RESOURCES;
            }
            break;
        case UPDATE_RESOURCES:
            if (mDownloadComplete)
            {
                if (mUpdateIndex < mUpdateFiles.size())
                {
                    UpdateFile thisFile = mUpdateFiles[mUpdateIndex];
                    if (!thisFile.required)
                    {
                        // This statement checks to see if the file type
                        // is music, and if download-music is true
                        // If it fails, this statement returns true,
                        // and results in not downloading the file
                        // Else it will ignore the break,
                        // and download the file.

                        if (!(thisFile.type == "music"
                            && config.getBoolValue("download-music")))
                        {
                            mUpdateIndex++;
                            break;
                        }
                    }
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
                    // Download of updates completed
//                    mDownloadStatus = UPDATE_COMPLETE;
                    mCurrentFile = "latest.txt";
                    mStoreInMemory = true;
                    mDownloadStatus = UPDATE_PATCH;
                    download();  // download() changes
                                 // mDownloadComplete to false
                }
            }
            break;
        case UPDATE_LIST2:
            if (mDownloadComplete)
            {
                if (mCurrentFile == xmlUpdateFile)
                {
                    mTempUpdateFiles = loadXMLFile(std::string(
                        mUpdatesDir).append("/").append(xmlUpdateFile));
                }
                mUpdateIndexOffset = mUpdateIndex;
                mUpdateIndex = 0;
                mStoreInMemory = false;
                mDownloadStatus = UPDATE_RESOURCES2;
                download();
            }
            break;
        case UPDATE_RESOURCES2:
            if (mDownloadComplete)
            {
                if (mUpdateIndex < mTempUpdateFiles.size())
                {
                    UpdateFile thisFile = mTempUpdateFiles[mUpdateIndex];
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
                    mDownloadStatus = UPDATE_COMPLETE;
                }
            }
            break;
        case UPDATE_COMPLETE:
            mUpdatesDir = mUpdatesDirReal;
            enable();
            // TRANSLATORS: updater window label
            setLabel(_("Completed"));
            break;
        case UPDATE_IDLE:
            break;
        default:
            logger->log("UpdaterWindow::logic unknown status: "
                        + toString(static_cast<unsigned>(mDownloadStatus)));
            break;
    }
    BLOCK_END("UpdaterWindow::logic")
}

bool UpdaterWindow::validateFile(const std::string &filePath,
                                 const unsigned long hash)
{
    FILE *file = fopen(filePath.c_str(), "rb");
    if (!file)
        return false;

    const unsigned long adler = Net::Download::fadler32(file);
    fclose(file);
    return adler == hash;
}

void UpdaterWindow::handleLink(const std::string &link,
                               gcn::MouseEvent *event A_UNUSED)
{
    if (strStartWith(link, "http://") || strStartWith(link, "https://"))
        openBrowser(link);
}
