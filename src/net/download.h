/*
 *  The ManaPlus Client
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

#ifndef NET_DOWNLOAD_H
#define NET_DOWNLOAD_H

#include "enums/net/downloadstatus.h"

#include <curl/curl.h>

#include <string>
#include <queue>

#include "localconsts.h"

typedef int (*DownloadUpdate)(void *ptr,
                              const DownloadStatusT status,
                              size_t total,
                              const size_t remaining);

// Matches what CURL expects
typedef size_t (*WriteFunction)( void *ptr, size_t size, size_t nmemb,
                                 void *stream);

struct SDL_Thread;

namespace Net
{
class Download final
{
    public:
        Download(void *const ptr,
                 const std::string &url,
                 const DownloadUpdate updateFunction,
                 const bool ignoreError,
                 const bool isUpload,
                 const bool isXml);

        A_DELETE_COPY(Download)

        ~Download();

        void addHeader(const std::string &header);

        /**
         * Convience method for adding no-cache headers.
         */
        void noCache();

        void setFile(const std::string &filename,
                     const int64_t adler32);

        void setWriteFunction(WriteFunction write);

        /**
         * Starts the download thread.
         * @returns true  if thread was created
         *          false if the thread could not be made or download wasn't
         *                properly setup
         */
        bool start();

        /**
         * Cancels the download. Returns immediately, the cancelled status will
         * be noted in the next avialable update call.
         */
        void cancel();

        void addMirror(const std::string &str)
        { mUrlQueue.push(str); }

        const char *getError() const A_WARN_UNUSED;

        void setIgnoreError(const bool n)
        { mIgnoreError = n; }

        static size_t writeFunction(void *ptr, size_t size,
                                    size_t nmemb, void *stream);

        static void prepareForm(curl_httppost **form,
                                const std::string &fileName);

        static unsigned long fadler32(FILE *const file) A_WARN_UNUSED;

        static void addProxy(CURL *const curl);

        static void secureCurl(CURL *const curl);

        static void addHeaders(CURL *const curl);

        static unsigned long adlerBuffer(const char *const buffer, int size);

        static std::string getUploadResponse()
        { return mUploadResponse; }

    private:
        static int downloadThread(void *ptr);
        static int downloadProgress(void *clientp, double dltotal,
                                    double dlnow, double ultotal,
                                    double ulnow);
        static std::string mUploadResponse;
        void *mPtr;
        std::string mUrl;
        struct
        {
            unsigned cancel : 1;
            unsigned memoryWrite: 1;
            unsigned checkAdler: 1;
        } mOptions;
        std::string mFileName;
        std::queue<std::string> mUrlQueue;
        WriteFunction mWriteFunction;
        unsigned long mAdler;
        DownloadUpdate mUpdateFunction;
        SDL_Thread *mThread;
        CURL *mCurl;
        curl_slist *mHeaders;
        curl_httppost *mFormPost;
        char *mError;
        bool mIgnoreError;
        bool mUpload;
        bool mIsXml;
};

}  // namespace Net

#endif  // NET_DOWNLOAD_H
