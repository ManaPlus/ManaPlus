/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef NET_EA_NETWORK_H
#define NET_EA_NETWORK_H

#include "net/serverinfo.h"
#include "net/sdltcpnet.h"

#include <SDL_thread.h>

#include <string>

namespace Ea
{

class Network
{
    public:
        Network();

        A_DELETE_COPY(Network)

        virtual ~Network();

        bool connect(ServerInfo server);

        void disconnect();

        ServerInfo getServer() const A_WARN_UNUSED
        { return mServer; }

        int getState() const A_WARN_UNUSED
        { return mState; }

        const std::string &getError() const A_WARN_UNUSED
        { return mError; }

        bool isConnected() const A_WARN_UNUSED
        { return mState == CONNECTED; }

        int getInSize() const A_WARN_UNUSED
        { return mInSize; }

        void skip(const int len);

        void flush();

        void fixSendBuffer();

        // ERROR replaced by NET_ERROR because already defined in Windows
        enum
        {
            IDLE = 0,
            CONNECTED,
            CONNECTING,
            DATA,
            NET_ERROR
        };

    protected:
        friend int networkThread(void *data);

        void setError(const std::string &error);

        uint16_t readWord(const int pos) const A_WARN_UNUSED;

        bool realConnect();

        void receive();

        TcpNet::Socket mSocket;

        ServerInfo mServer;

        char *mInBuffer;
        char *mOutBuffer;
        unsigned int mInSize;
        unsigned int mOutSize;

        unsigned int mToSkip;

        int mState;
        std::string mError;

        SDL_Thread *mWorkerThread;
        SDL_mutex *mMutexIn;
        SDL_mutex *mMutexOut;
        int mSleep;
};

}  // namespace Ea

#endif  // NET_EA_NETWORK_H
