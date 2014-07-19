/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#ifndef IPC_H
#define IPC_H

#include "net/sdltcpnet.h"

#include <SDL_thread.h>

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

    static int acceptLoop(void *ptr);

    static void start();

    static void stop();

private:
    void setPort(const unsigned short port)
    { mPort = port; }

    unsigned int mNumReqs;
    TcpNet::Socket mSocket;
    SDL_Thread *mThread;
    unsigned short mPort;
    bool mListen;
};

extern IPC *ipc;

#endif  // IPC_H
