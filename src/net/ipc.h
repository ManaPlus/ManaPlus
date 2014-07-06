/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  Vincent Petithory <vincent.petithory@gmail.com>
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

class IPC
{
public:
    /**
     * Constructor.
     */
    IPC(unsigned short port);
  
    /**
     * Destructor.
     */
    ~IPC();
  
    bool init();
    unsigned short port();
    static int acceptLoop(void *ptr);
    static void start();
    static void stop();
  
private:
    bool mListen;
    unsigned int mNumReqs;
    unsigned short mPort;
    TcpNet::Socket mSocket;
    SDL_Thread *mThread;
};

extern IPC *ipc;

#endif  // IPC_H
