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

#include "net/ipc.h"

#include "logger.h"

#include "gui/widgets/tabs/chattab.h"
#include "gui/windows/chatwindow.h"

#include "utils/sdlhelper.h"
#include "utils/stringutils.h"

#include <iostream>

#include "debug.h"

IPC *ipc = nullptr;

IPC::IPC(const unsigned short port) :
    mListen(false),
    mNumReqs(0),
    mPort(port),
    mSocket(nullptr),
    mThread(nullptr)
{
}

IPC::~IPC()
{
}

bool IPC::init()
{
    IPaddress ip;

    if(SDLNet_ResolveHost(&ip, nullptr, mPort) == -1)
    {
        logger->log("IPC: SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }

    mSocket = SDLNet_TCP_Open(&ip);
    if (!mSocket)
    {
        logger->log("IPC: Error in TcpNet::open(): %s", TcpNet::getError());
        return false;
    }

    mThread = SDL::createThread(&acceptLoop, "ipc", this);
    if (!mThread)
    {
        logger->log("IPC: unable to create acceptLoop thread");
        return false;
    }
    return true;
}

int IPC::acceptLoop(void *ptr)
{
    if (!ptr)
        return 1;

    IPC *const ipc1 = reinterpret_cast<IPC*>(ptr);
    const int max_length = 1024;
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(set, ipc1->mSocket);
    ipc->mListen = true;
    while (ipc1->mListen)
    {
        SDLNet_CheckSockets(set, 250);
        if (!SDLNet_SocketReady(ipc1->mSocket))
            continue;

        TCPsocket sock = SDLNet_TCP_Accept(ipc1->mSocket);
        if (!sock)
        {
            logger->log_r("IPC: unable to accept connection");
            continue;
        }
        char data[max_length] = {0};
        int result = SDLNet_TCP_Recv(sock, data, max_length);
        if (result <= 0)
        {
            logger->log_r("IPC: unable to accept connection");
            SDLNet_TCP_Close(sock);
            continue;
        }

        std::string req(data);
        req = trim(req);

        if (chatWindow)
            chatWindow->chatInput(req);
        ipc1->mNumReqs ++;
        const std::string resp = strprintf("[%d] %s\n",
            ipc1->mNumReqs, req.c_str());

        const char *respc = resp.c_str();
        const int len = strlen(respc) + 1;
        result = SDLNet_TCP_Send(sock, respc, len);
        if (result < len)
        {
            logger->log_r("IPC: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            SDLNet_TCP_Close(sock);
            continue;
        }
        SDLNet_TCP_Close(sock);
    }
    SDLNet_TCP_Close(ipc1->mSocket);
    return 0;
}

void IPC::stop()
{
    if (!ipc)
        return;

    logger->log("Stopping IPC...");
    ipc->mListen = false;
    int loopRet;
    SDL_WaitThread(ipc->mThread, &loopRet);
    ipc = nullptr;
}

void IPC::start()
{
    if (ipc)
        return;

    unsigned short port(44007);
    if (getenv("IPC_PORT"))
        port = atoi(getenv("IPC_PORT"));

    logger->log("Starting IPC...");
    for (int port = 44007; port < 65535; port ++)
    {
        logger->log("  -> trying port %d...", ipc_port);
        ipc = new IPC(port);
        if (ipc->init())
        {
            logger->log("  -> Port %d selected", ipc_port);
            break;
        }
        else
        {
            port ++;
        }
    }
}
