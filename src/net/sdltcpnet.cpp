/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "net/sdltcpnet.h"

#if defined __linux__ || defined __linux
#include "logger.h"

#include <sys/socket.h>

#if defined(M_TCPOK) && !defined(ANDROID)
#include <netinet/in.h>
#include <netdb.h>
#include <linux/tcp.h>
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <netinet/tcp.h>
// Use linear timeouts for thin streams
#define TCP_THIN_LINEAR_TIMEOUTS 16
// Fast retrans. after 1 dupack
#define TCP_THIN_DUPACK          17
#endif

#endif

#include "debug.h"

// because actual struct is hidden in SDL_net we reinroducing it here
struct TCPsocketHack
{
    int ready;
    int channel;
    IPaddress remoteAddress;
    IPaddress localAddress;
    int sflag;
};

void TcpNet::init()
{
    SDLNet_Init();
}

void TcpNet::quit()
{
    SDLNet_Quit();
}

void TcpNet::closeSocket(const TcpNet::Socket socket)
{
    SDLNet_TCP_Close(socket);
}

int TcpNet::send(const TcpNet::Socket sock, const void *const data,
                 const int len)
{
    return SDLNet_TCP_Send(sock, data, len);
}

const char *TcpNet::getError()
{
    return SDLNet_GetError();
}

int TcpNet::resolveHost(IPaddress *const address, const char *const host,
                        const Uint16 port)
{
    return SDLNet_ResolveHost(address, host, port);
}

TcpNet::Socket TcpNet::open(IPaddress *const ip)
{
    const TcpNet::Socket sock = SDLNet_TCP_Open(ip);
#ifndef __native_client__
    if (sock && ip)
    {
        const TCPsocketHack *const hack
            = reinterpret_cast<const TCPsocketHack *const>(sock);
        // here we using some magic to compare TCPsocket and own padding
        // because actual struct TCPsocket not in headers
        if (hack)
        {
            const IPaddress &addr = hack->remoteAddress;
            if (addr.host == ip->host && addr.port == ip->port)
            {
                const int val = 1;
#ifdef TCP_THIN_LINEAR_TIMEOUTS
                if (setsockopt(hack->channel, IPPROTO_TCP,
                    TCP_THIN_LINEAR_TIMEOUTS, &val, sizeof(val)))
                {
                    logger->log_r("error on set TCP_THIN_LINEAR_TIMEOUTS");
                }
#endif
#ifdef TCP_THIN_DUPACK
                if (setsockopt(hack->channel, IPPROTO_TCP,
                    TCP_THIN_DUPACK, &val, sizeof(val)))
                {
                    logger->log_r("error on set TCP_THIN_DUPACK");
                }
#endif
            }
        }
    }
#endif
    return sock;
}

TcpNet::SocketSet TcpNet::allocSocketSet(const int maxsockets)
{
    return SDLNet_AllocSocketSet(maxsockets);
}

int TcpNet::addSocket(const TcpNet::SocketSet set, const TcpNet::Socket sock)
{
    return SDLNet_TCP_AddSocket(set, sock);
}

int TcpNet::checkSockets(const TcpNet::SocketSet set, const Uint32 timeout)
{
    return SDLNet_CheckSockets(set, timeout);
}

int TcpNet::recv(const TcpNet::Socket sock, void *const data, const int maxlen)
{
    return SDLNet_TCP_Recv(sock, data, maxlen);
}

int TcpNet::delSocket(const TcpNet::SocketSet set, const TcpNet::Socket sock)
{
    return SDLNet_TCP_DelSocket(set, sock);
}

void TcpNet::freeSocketSet(const TcpNet::SocketSet set)
{
    SDLNet_FreeSocketSet(set);
}
