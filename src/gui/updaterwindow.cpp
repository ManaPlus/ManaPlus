/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "logger.h"
#include "main.h"

#include "gui/sdlinput.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"

#include "net/download.h"
#include "net/logindata.h"

#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"

#include <iostream>
#include <fstream>

#include <sys/stat.h>

#include "debug.h"

const std::string xmlUpdateFile = "resources.xml";
const std::string txtUpdateFile = "resources2.txt";
const std::string updateServer2
    = "http://download.evolonline.org/manaplus/updates/";

std::vector<updateFile> loadXMLFile(const std::string &fileName);
std::vector<updateFile> loadTxtFile(const std::string &fileName);

/**
 * Load the given file into a vector of updateFiles.
 */
std::vector<updateFile> loadXMLFile(const std::string &fileName)
{
    std::vector<updateFile> files;
    XML::Document doc(fileName, false);
    xmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "updates"))
    {
        logger->log("Error loading update file: %s", fileName.c_str());
        return files;
    }

    for_each_xml_child_node(fileNode, rootNode)
    {
        // Ignore all tags except for the "update" tags
        if (!xmlStrEqual(fileNode->name, BAD_CAST "update"))
            continue;

        updateFile file;
        file.name = XML::getProperty(fileNode, "file", "");
        file.hash = XML::getProperty(fileNode, "hash", "");
        file.type = XML::getProperty(fileNode, "type", "data");
        file.desc = XML::getProperty(fileNode, "description", "");
        if (XML::getProperty(fileNode, "required", "yes") == "yes")
            file.required = true;
        else
            file.required = false;

        files.push_back(file);
    }

    return files;
}

std::vector<updateFile> loadTxtFile(const std::string &fileName)
{
    std::vector<updateFile> files;
    std::ifstream fileHandler;
    fileHandler.open(fileName.c_str(), std::ios::in);

    if (fileHandler.is_open())
    {
        while (fileHandler.good())
        {
            char name[256], hash[50];
            fileHandler.getline(name, 256, ' ');
            fileHandler.getline(hash, 50);

            updateFile thisFile;
            thisFile.name = name;
            thisFile.hash = hash;
            thisFile.type = "data";
            thisFile.required = true;
            thisFile.desc = "";

            if (!thisFile.name.empty())
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
                             bool applyUpdates,
                             int updateType):
    Window(_("Updating..."), false, nullptr, "update.xml"),
    mDownloadStatus(UPDATE_NEWS),
    mUpdateHost(updateHost),
    mUpdatesDir(updatesDir),
    mUpdatesDirReal(updatesDir),
    mCurrentFile("news.txt"),
    mDownloadProgress(0.0f),
    mCurrentChecksum(0),
    mStoreInMemory(true),
    mDownloadComplete(true),
    mUserCancel(false),
    mDownloadedBytes(0),
    mMemoryBuffer(nullptr),
    mDownload(nullptr),
    mUpdateIndex(0),
    mUpdateIndexOffset(0),
    mLoadUpdates(applyUpdates),
    mUpdateType(updateType)
{
    mBrowserBox = new BrowserBox;
    mScrollArea = new ScrollArea(mBrowserBox);
    mLabel = new Label(_("Connecting..."));
    mProgressBar = new ProgressBar(0.0, 310, 20);
    mCancelButton = new Button(_("Cancel"), "cancel", this);
    mPlayButton = new Button(_("Play"), "play", this);

    mProgressBar->setSmoothProgress(false);
    mBrowserBox->setOpaque(false);
    mPlayButton->setEnabled(false);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mScrollArea, 5, 3).setPadding(3);
    placer(0, 3, mLabel, 5);
    placer(0, 4, mProgressBar, 5);
    placer(3, 5, mCancelButton);
    placer(4, 5, mPlayButton);

    reflowLayout(450, 400);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    addKeyListener(this);

    center();
    setVisible(true);
    mCancelButton->requestFocus();

    mUpdateServerPath = mUpdateHost;
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

void UpdaterWindow::setProgress(float p)
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
    if (event.getId() == "cancel")
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
    else if (event.getId() == "play")
    {
        Client::setState(STATE_LOAD_DATA);
    }
}

void UpdaterWindow::keyPressed(gcn::KeyEvent &keyEvent)
{
    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ESCAPE)
    {
        action(gcn::ActionEvent(nullptr, mCancelButton->getActionEventId()));
        Client::setState(STATE_WORLD_SELECT);
    }
    else if (key.getValue() == Key::ENTER)
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

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    while (line)
    {
        mBrowserBox->addRow(line);
        line = strtok(nullptr, "\n");
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
    mMemoryBuffer[mDownloadedBytes] = '\0';

    std::string version;

    // Tokenize and add each line separately
    char *line = strtok(mMemoryBuffer, "\n");
    if (line)
    {
        version = line;
        if (version > CHECK_VERSION)
        {
            mBrowserBox->addRow("", true);
            mBrowserBox->addRow("  ##1http://manaplus.evolonline.org/", true);
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

int UpdaterWindow::updateProgress(void *ptr, DownloadStatus status,
                                  size_t dt, size_t dn)
{
    UpdaterWindow *uw = reinterpret_cast<UpdaterWindow *>(ptr);
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
        progress = 0.0f; // check for NaN
    if (progress < 0.0f)
        progress = 0.0f; // no idea how this could ever happen,
                         // but why not check for it anyway.
    if (progress > 1.0f)
        progress = 1.0f;

    uw->setLabel(uw->mCurrentFile + " ("
        + toString(static_cast<int>(progress * 100)) + "%)");

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
    UpdaterWindow *uw = reinterpret_cast<UpdaterWindow *>(stream);
    size_t totalMem = size * nmemb;
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
        mDownload = new Net::Download(this, mUpdateHost + "/" + mCurrentFile,
                                      updateProgress);
    }

    if (mStoreInMemory)
    {
        mDownload->setWriteFunction(UpdaterWindow::memoryWrite);
    }
    else
    {
        if (mDownloadStatus == UPDATE_RESOURCES)
        {
            mDownload->setFile(mUpdatesDir + "/" + mCurrentFile,
                               mCurrentChecksum);
        }
        else
        {
            mDownload->setFile(mUpdatesDir + "/" + mCurrentFile);
        }
    }

    if (mDownloadStatus != UPDATE_RESOURCES)
        mDownload->noCache();

    setLabel(mCurrentFile + " (0%)");
    mDownloadComplete = false;

    // TODO: check return
    mDownload->start();
}

void UpdaterWindow::loadUpdates()
{
    ResourceManager *resman = ResourceManager::getInstance();

    if (mUpdateFiles.empty())
    {   // updates not downloaded
        mUpdateFiles = loadXMLFile(mUpdatesDir + "/" + xmlUpdateFile);
        if (mUpdateFiles.empty())
        {
            logger->log("Warning this server does not have a"
                        " %s file falling back to %s", xmlUpdateFile.c_str(),
                        txtUpdateFile.c_str());
            mUpdateFiles = loadTxtFile(mUpdatesDir + "/" + txtUpdateFile);
        }
    }

    std::string fixPath = mUpdatesDir + "/fix";
    for (mUpdateIndex = 0; mUpdateIndex < mUpdateFiles.size(); mUpdateIndex++)
    {
        UpdaterWindow::addUpdateFile(resman, mUpdatesDir, fixPath,
            mUpdateFiles[mUpdateIndex].name, false);
    }
}

void UpdaterWindow::loadLocalUpdates(std::string dir)
{
    ResourceManager *resman = ResourceManager::getInstance();

    std::vector<updateFile> updateFiles
        = loadXMLFile(dir + "/" + xmlUpdateFile);

    if (updateFiles.empty())
    {
        logger->log("Warning this server does not have a"
                    " %s file falling back to %s", xmlUpdateFile.c_str(),
                    txtUpdateFile.c_str());
        updateFiles = loadTxtFile(dir + "/" + txtUpdateFile);
    }

    std::string fixPath = dir + "/fix";
    for (unsigned int updateIndex = 0;
         updateIndex < updateFiles.size(); updateIndex++)
    {
        UpdaterWindow::addUpdateFile(resman, dir, fixPath, 
            updateFiles[updateIndex].name, false);
    }
}

void UpdaterWindow::addUpdateFile(ResourceManager *resman, std::string path,
                                  std::string fixPath, std::string file,
                                  bool append)
{
    if (!append)
        resman->addToSearchPath(path + "/" + file, append);

    const std::string fixFile = fixPath + "/" + file;
    struct stat statbuf;
    if (!stat(fixFile.c_str(), &statbuf))
        resman->addToSearchPath(fixFile, append);

    if (append)
        resman->addToSearchPath(path + "/" + file, append);
}

void UpdaterWindow::logic()
{
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
            mProgressBar->setText(strprintf("%d/%d", mUpdateIndex
                + mUpdateIndexOffset + 1, (int)mUpdateFiles.size()
                + (int)mTempUpdateFiles.size() + 1));
        }
        else
        {
            mProgressBar->setText("");
        }
    }

    switch (mDownloadStatus)
    {
        case UPDATE_ERROR:
            // TODO: Only send complete sentences to gettext
            mBrowserBox->addRow("");
            mBrowserBox->addRow(_("##1  The update process is incomplete."));
            // TRANSLATORS: Continues "you try again later.".
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
                download(); // download() changes mDownloadComplete to false
            }
            break;
        case UPDATE_PATCH:
            if (mDownloadComplete)
            {
                // Parse current memory buffer as news and dispose of the data
                loadPatch();

                mUpdateHost = updateServer2 + mUpdateServerPath;
                mUpdatesDir += "/fix";
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
                    mUpdateFiles = loadXMLFile(
                        mUpdatesDir + "/" + xmlUpdateFile);

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
                    mUpdateFiles = loadTxtFile(
                        mUpdatesDir + "/" + txtUpdateFile);
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
                    updateFile thisFile = mUpdateFiles[mUpdateIndex];
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

                    std::ifstream temp(
                            (mUpdatesDir + "/" + mCurrentFile).c_str());

                    if (!temp.is_open() || !validateFile(mUpdatesDir + "/"
                        + mCurrentFile, mCurrentChecksum))
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
                    download(); // download() changes
                                // mDownloadComplete to false
                }
            }
            break;
        case UPDATE_LIST2:
            if (mDownloadComplete)
            {
                if (mCurrentFile == xmlUpdateFile)
                {
                    mTempUpdateFiles = loadXMLFile(
                        mUpdatesDir + "/" + xmlUpdateFile);
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
                    updateFile thisFile = mTempUpdateFiles[mUpdateIndex];
                    mCurrentFile = thisFile.name;
                    std::string checksum;
                    checksum = thisFile.hash;
                    std::stringstream ss(checksum);
                    ss >> std::hex >> mCurrentChecksum;

                    std::ifstream temp(
                            (mUpdatesDir + "/" + mCurrentFile).c_str());

                    if (!temp.is_open() || !validateFile(mUpdatesDir + "/"
                        + mCurrentFile, mCurrentChecksum))
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
            setLabel(_("Completed"));
            break;
        case UPDATE_IDLE:
            break;
        default:
            logger->log("UpdaterWindow::logic unknown status: "
                        + toString(static_cast<unsigned>(mDownloadStatus)));
            break;
    }
}

bool UpdaterWindow::validateFile(std::string filePath, unsigned long hash)
{
    FILE *file = fopen(filePath.c_str(), "rb");
    if (!file)
        return false;

    unsigned long adler = Net::Download::fadler32(file);
    fclose(file);
    return adler == hash;
}
