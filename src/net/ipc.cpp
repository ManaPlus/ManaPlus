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

#include "gui/widgets/tabs/chattab.h"
#include "gui/windows/chatwindow.h"
#include "utils/sdlhelper.h"
#include "utils/stringutils.h"
#include <iostream>

#include "logger.h"

#include "debug.h"

IPC *ipc = nullptr;

IPC::IPC(unsigned short port) :
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

    if(SDLNet_ResolveHost(&ip,NULL,mPort) == -1)
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

    mThread = SDL::createThread(&IPC::acceptLoop, "ipc", this);
    if (!mThread)
    {
        logger->log("IPC: unable to create acceptLoop thread");
        return false;
    }
    return true;
}

int IPC::acceptLoop(void *ptr)
{
    IPC *const ipc = reinterpret_cast<IPC*>(ptr);
    const int max_length = 1024;
    SDLNet_SocketSet set;

    set = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(set, ipc->mSocket);
    ipc->mListen = true;
    try
    {
        while (ipc->mListen)
        {
            SDLNet_CheckSockets(set, 250);
            if (!SDLNet_SocketReady(ipc->mSocket))
                continue;

            TCPsocket sock;
            sock = SDLNet_TCP_Accept(ipc->mSocket);
            if (!sock)
            {
                logger->log_r("IPC: unable to accept connection");
                continue;
            }
            char data[max_length] = {0};
            int result;
            result = SDLNet_TCP_Recv(sock, data, max_length);
            if (result <= 0)
            {
                logger->log_r("IPC: unable to accept connection");
                SDLNet_TCP_Close(sock);
                continue;
            }

            std::string req(data);
            req = trim(req);

            std::string resp;
            chatWindow->chatInput(req);
            ipc->mNumReqs++;
            resp = strprintf("[%d] %s\n", ipc->mNumReqs, req.c_str());
      
            int len;
            const char *respc;
            respc = resp.c_str();
            len = strlen(respc)+1;
            result = SDLNet_TCP_Send(sock, respc, len);
            if (result < len)
            {
                logger->log_r("IPC: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                SDLNet_TCP_Close(sock);
                continue;
            }
            SDLNet_TCP_Close(sock);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        SDLNet_TCP_Close(ipc->mSocket);
        return 1;
    }
    SDLNet_TCP_Close(ipc->mSocket);
    return 0;
}

unsigned short IPC::port()
{
    return mPort;
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

    unsigned short ipc_port(44007);
    if (getenv("IPC_PORT"))
        ipc_port = atoi(getenv("IPC_PORT"));

    logger->log("Starting IPC...");
    while (true)
    {
        logger->log(strprintf("  -> trying port %d...", ipc_port));
        ipc = new IPC(ipc_port);
        if (ipc->init())
        {
            logger->log(strprintf("  -> Port %d selected", ipc_port));
            break;
        }
        else
        {
            ipc_port++;
        }
    }
}
