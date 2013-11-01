/*
 *  The ManaPlus Client
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

#include "net/download.h"

#include "configuration.h"
#include "client.h"
#include "logger.h"
#include "main.h"

#include "utils/sdlhelper.h"

#include <curl/curl.h>

#include <SDL_thread.h>

#include <zlib.h>

#define CURLVERSION_ATLEAST(a, b, c) ((LIBCURL_VERSION_MAJOR > (a)) || \
    ((LIBCURL_VERSION_MAJOR == (a)) && (LIBCURL_VERSION_MINOR > (b))) || \
    ((LIBCURL_VERSION_MAJOR == (a)) && (LIBCURL_VERSION_MINOR == (b)) && \
    (LIBCURL_VERSION_PATCH >= (c))))

#if defined __native_client__
#include "utils/files.h"
#endif

#include "debug.h"

const char *DOWNLOAD_ERROR_MESSAGE_THREAD
    = "Could not create download thread!";

enum
{
    OPTIONS_NONE = 0,
    OPTIONS_MEMORY = 1
};

namespace Net
{

Download::Download(void *const ptr, const std::string &url,
                   const DownloadUpdate updateFunction,
                   const bool ignoreError) :
    mPtr(ptr),
    mUrl(url),
    mOptions(),
    mFileName(""),
    mWriteFunction(nullptr),
    mAdler(0),
    mUpdateFunction(updateFunction),
    mThread(nullptr),
    mCurl(nullptr),
    mHeaders(nullptr),
    mError(static_cast<char*>(calloc(CURL_ERROR_SIZE + 1, 1))),
    mIgnoreError(ignoreError)
{
    if (mError)
        mError[0] = 0;

    mOptions.cancel = 0;
    mOptions.memoryWrite = 0;
    mOptions.checkAdler = true;
    const std::string serverName = client->getServerName();
    if (!serverName.empty())
    {
        if (mUrl.find("?") == std::string::npos)
            mUrl.append("?host=");
        else
            mUrl.append("&host=");
        mUrl.append(serverName);
    }
}

Download::~Download()
{
    if (mHeaders)
        curl_slist_free_all(mHeaders);

    mHeaders = nullptr;
    int status;
    if (mThread && SDL_GetThreadID(mThread))
        SDL_WaitThread(mThread, &status);
    mThread = nullptr;
    free(mError);
}

/**
 * Calculates the Alder-32 checksum for the given file.
 */
unsigned long Download::fadler32(FILE *const file)
{
    if (!file)
        return 0;

    // Obtain file size
    fseek(file, 0, SEEK_END);
    const long fileSize = ftell(file);
    if (fileSize < 0)
    {   // file size error
        return 0;
    }
    rewind(file);

    // Calculate Adler-32 checksum
    char *const buffer = new char[fileSize];
    if (!buffer)
        return 0;
    const uInt read = static_cast<uInt>(fread(buffer, 1, fileSize, file));
    unsigned long adler = adler32(0L, Z_NULL, 0);
    adler = adler32(static_cast<uInt>(adler),
        reinterpret_cast<Bytef*>(buffer), read);
    delete [] buffer;
    return adler;
}

unsigned long Download::adlerBuffer(char *buffer, int size)
{
    unsigned long adler = adler32(0L, Z_NULL, 0);
    return adler32(static_cast<uInt>(adler),
        reinterpret_cast<Bytef*>(buffer), size);
}

void Download::addHeader(const std::string &header)
{
    mHeaders = curl_slist_append(mHeaders, header.c_str());
}

void Download::noCache()
{
    addHeader("pragma: no-cache");
    addHeader("Cache-Control: no-cache");
}

void Download::setFile(const std::string &filename, const int64_t adler32)
{
    mOptions.memoryWrite = 0;
    mFileName = filename;

    if (adler32 > -1)
    {
        mAdler = static_cast<unsigned long>(adler32);
        mOptions.checkAdler = true;
    }
    else
    {
        mOptions.checkAdler = 0;
    }
}

void Download::setWriteFunction(WriteFunction write)
{
    mOptions.memoryWrite = true;
    mWriteFunction = write;
}

bool Download::start()
{
    logger->log("Starting download: %s", mUrl.c_str());

    mThread = SDL::createThread(&downloadThread, "download", this);
    if (!mThread)
    {
        logger->log1(DOWNLOAD_ERROR_MESSAGE_THREAD);
        if (mError)
            strcpy(mError, DOWNLOAD_ERROR_MESSAGE_THREAD);
        mUpdateFunction(mPtr, DOWNLOAD_STATUS_THREAD_ERROR, 0, 0);
        if (!mIgnoreError)
            return false;
    }

    return true;
}

void Download::cancel()
{
    logger->log("Canceling download: %s", mUrl.c_str());

    mOptions.cancel = true;
    if (mThread && SDL_GetThreadID(mThread))
        SDL_WaitThread(mThread, nullptr);

    mThread = nullptr;
}

const char *Download::getError() const
{
    return mError;
}

int Download::downloadProgress(void *clientp, double dltotal, double dlnow,
                               double ultotal A_UNUSED, double ulnow A_UNUSED)
{
    Download *const d = reinterpret_cast<Download *const>(clientp);
    if (!d)
        return -5;

    if (d->mOptions.cancel)
    {
        return d->mUpdateFunction(d->mPtr, DOWNLOAD_STATUS_CANCELLED,
                                  static_cast<size_t>(dltotal),
                                  static_cast<size_t>(dlnow));
    }

    return d->mUpdateFunction(d->mPtr, DOWNLOAD_STATUS_IDLE,
                              static_cast<size_t>(dltotal),
                              static_cast<size_t>(dlnow));
}

int Download::downloadThread(void *ptr)
{
    int attempts = 0;
    bool complete = false;
    Download *const d = reinterpret_cast<Download*>(ptr);
    CURLcode res;

    if (!d)
        return 0;

    const std::string outFilename = !d->mOptions.memoryWrite
        ? (d->mFileName + ".part") : "";

    while (attempts < 3 && !complete && !d->mOptions.cancel)
    {
        d->mUpdateFunction(d->mPtr, DOWNLOAD_STATUS_STARTING, 0, 0);

        if (d->mOptions.cancel)
        {
            // need terminate thread?
            d->mThread = nullptr;
            return 0;
        }

        d->mCurl = curl_easy_init();

        if (d->mCurl && !d->mOptions.cancel)
        {
            FILE *file = nullptr;
            logger->log("Downloading: %s", d->mUrl.c_str());

            curl_easy_setopt(d->mCurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(d->mCurl, CURLOPT_HTTPHEADER, d->mHeaders);

            if (d->mOptions.memoryWrite)
            {
                curl_easy_setopt(d->mCurl, CURLOPT_FAILONERROR, 1);
                curl_easy_setopt(d->mCurl, CURLOPT_WRITEFUNCTION,
                                 d->mWriteFunction);
                curl_easy_setopt(d->mCurl, CURLOPT_WRITEDATA, d->mPtr);
            }
            else
            {
                file = fopen(outFilename.c_str(), "w+b");
                if (file)
                    curl_easy_setopt(d->mCurl, CURLOPT_WRITEDATA, file);
            }

            curl_easy_setopt(d->mCurl, CURLOPT_USERAGENT,
                strprintf(PACKAGE_EXTENDED_VERSION,
                branding.getStringValue("appName").c_str()).c_str());
            curl_easy_setopt(d->mCurl, CURLOPT_ERRORBUFFER, d->mError);
            curl_easy_setopt(d->mCurl, CURLOPT_URL, d->mUrl.c_str());
            curl_easy_setopt(d->mCurl, CURLOPT_NOPROGRESS, 0);
            curl_easy_setopt(d->mCurl, CURLOPT_PROGRESSFUNCTION,
                             &downloadProgress);
            curl_easy_setopt(d->mCurl, CURLOPT_PROGRESSDATA, ptr);
            curl_easy_setopt(d->mCurl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(d->mCurl, CURLOPT_CONNECTTIMEOUT, 30);
            curl_easy_setopt(d->mCurl, CURLOPT_TIMEOUT, 1800);
            addProxy(d->mCurl);
            secureCurl(d->mCurl);

            if ((res = curl_easy_perform(d->mCurl)) != 0
                && !d->mOptions.cancel)
            {
                switch (res)
                {
                    case CURLE_ABORTED_BY_CALLBACK:
                        d->mOptions.cancel = true;
                        break;
                    case CURLE_COULDNT_CONNECT:
                    default:
                    {
                        if (d->mError)
                        {
                            logger->log("curl error %d: %s host: %s",
                                res, d->mError, d->mUrl.c_str());
                        }
                        break;
                    }
                }

                if (d->mOptions.cancel)
                    break;

                d->mUpdateFunction(d->mPtr, DOWNLOAD_STATUS_ERROR, 0, 0);

                if (!d->mOptions.memoryWrite)
                {
                    if (file)
                    {
                        fclose(file);
                        file = nullptr;
                    }
                    ::remove(outFilename.c_str());
                }
                attempts++;
                continue;
            }

            curl_easy_cleanup(d->mCurl);
            d->mCurl = nullptr;

            if (!d->mOptions.memoryWrite)
            {
                // Don't check resources.xml checksum
                if (d->mOptions.checkAdler)
                {
                    const unsigned long adler = fadler32(file);

                    if (d->mAdler != adler)
                    {
                        if (file)
                        {
                            fclose(file);
                            file = nullptr;
                        }

                        // Remove the corrupted file
                        ::remove(d->mFileName.c_str());
                        logger->log("Checksum for file %s failed: (%lx/%lx)",
                            d->mFileName.c_str(),
                            adler, d->mAdler);
                        attempts++;
                        continue;  // Bail out here to avoid the renaming
                    }
                }
                if (file)
                {
                    fclose(file);
                    file = nullptr;
                }

                // Any existing file with this name is deleted first, otherwise
                // the rename will fail on Windows.
                if (!d->mOptions.cancel)
                {
                    ::remove(d->mFileName.c_str());
#if defined __native_client__
                    Files::renameFile(
#else
                    ::rename(
#endif
                    outFilename.c_str(), d->mFileName.c_str());

                    // Check if we can open it and no errors were encountered
                    // during renaming
                    file = fopen(d->mFileName.c_str(), "rb");
                    if (file)
                    {
                        fclose(file);
                        file = nullptr;
                        complete = true;
                    }
                }
            }
            else
            {
                // It's stored in memory, we're done
                complete = true;
            }
        }

        if (d->mCurl)
        {
            curl_easy_cleanup(d->mCurl);
            d->mCurl = nullptr;
        }

        if (d->mOptions.cancel)
        {
            // need ternibate thread?
            d->mThread = nullptr;
            return 0;
        }
        attempts++;
    }

    if (d->mOptions.cancel)
    {
        // Nothing to do...
    }
    else if (!complete || attempts >= 3)
    {
        d->mUpdateFunction(d->mPtr, DOWNLOAD_STATUS_ERROR, 0, 0);
    }
    else
    {
        d->mUpdateFunction(d->mPtr, DOWNLOAD_STATUS_COMPLETE, 0, 0);
    }

    d->mThread = nullptr;
    return 0;
}

void Download::addProxy(CURL *const curl)
{
    const int mode = config.getIntValue("downloadProxyType");
    if (!mode)
        return;

    if (mode > 1)
    {
        curl_easy_setopt(curl, CURLOPT_PROXY,
            config.getStringValue("downloadProxy").c_str());
    }
    switch (mode)
    {
        case 1:  // direct connection
        default:
            curl_easy_setopt(curl, CURLOPT_PROXY, "");
            break;
        case 2:  // HTTP
            break;
        case 3:  // HTTP 1.0
#if CURLVERSION_ATLEAST(7, 19, 4)
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP_1_0);
#endif
            break;
        case 4:  // SOCKS4
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
            break;
        case 5:  // SOCKS4A
#if CURLVERSION_ATLEAST(7, 18, 0)
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4A);
#else
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
#endif
            break;
        case 6:  // SOCKS5
#if CURLVERSION_ATLEAST(7, 18, 0)
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
#endif
            break;
        case 7:  // SOCKS5 hostname
#if CURLVERSION_ATLEAST(7, 18, 0)
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE,
                CURLPROXY_SOCKS5_HOSTNAME);
#endif
            break;
    }
}

void Download::secureCurl(CURL *const curl)
{
#if CURLVERSION_ATLEAST(7, 19, 4)
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS,
        CURLPROTO_HTTP | CURLPROTO_HTTPS);
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS,
        CURLPROTO_HTTP | CURLPROTO_HTTPS);
#endif
#if CURLVERSION_ATLEAST(7, 21, 0)
    curl_easy_setopt(curl, CURLOPT_WILDCARDMATCH, 0);
#endif
#if CURLVERSION_ATLEAST(7, 15, 1)
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3);
#endif
}

}  // namespace Net
