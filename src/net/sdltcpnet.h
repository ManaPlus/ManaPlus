/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef NET_SDLTCPNET_H
#define NET_SDLTCPNET_H

#include <SDL_net.h>

#include <string>

#include "localconsts.h"

namespace TcpNet
{
    typedef ::SDLNet_SocketSet SocketSet;
    typedef ::TCPsocket Socket;

    void init();

    void quit();

    void closeSocket(TcpNet::Socket socket);

    int send(TcpNet::Socket sock, const void *data, int len);

    char *getError();

    int resolveHost(IPaddress *address, const char *host, Uint16 port);

    TcpNet::Socket open(IPaddress *ip);

    SocketSet allocSocketSet(int maxsockets);

    int addSocket(TcpNet::SocketSet set, TcpNet::Socket sock);

    int checkSockets(TcpNet::SocketSet set, Uint32 timeout);

    int recv(TcpNet::Socket sock, void *data, int maxlen);

    int delSocket(TcpNet::SocketSet set, TcpNet::Socket sock);

    void freeSocketSet(TcpNet::SocketSet set);
}

#endif
