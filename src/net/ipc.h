/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#ifndef NET_IPC_H
#define NET_IPC_H

#include "localconsts.h"

PRAGMACLANG6GCC(GCC diagnostic push)
PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
#include "net/sdltcpnet.h"
PRAGMACLANG6GCC(GCC diagnostic pop)

#include "utils/vector.h"

#include <string>

class IPC final
{
    public:
        /**
         * Constructor.
         */
        IPC();

        A_DELETE_COPY(IPC)

        /**
         * Destructor.
         */
        ~IPC();

        bool init();

        unsigned short getPort() const A_WARN_UNUSED
        { return mPort; }

        void flush();

        static int acceptLoop(void *ptr);

        static void start();

        static void stop();

    private:
        void setPort(const unsigned short port)
        { mPort = port; }

        unsigned int mNumReqs;
        TcpNet::Socket mSocket;
        STD_VECTOR<std::string> mDelayedCommands;
        SDL_Thread *mThread;
        SDL_mutex *mMutex;
        unsigned short mPort;
        volatile bool mThreadLocked;
        bool mListen;
};

extern IPC *ipc;

#endif  // NET_IPC_H
