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

#if defined __native_client__

#include "net/nacltcpnet.h"

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

struct _SDLNet_SocketSet
{
};

struct _TCPsocket
{
    int ready;
    int channel;
    IPaddress remoteAddress;
    IPaddress localAddress;
    int sflag;
};

void TcpNet::init()
{
}

void TcpNet::quit()
{
}

void TcpNet::closeSocket(const TcpNet::Socket socket)
{
}

int TcpNet::send(const TcpNet::Socket sock, const void *const data,
                 const int len)
{
}

const char *TcpNet::getError()
{
}

int TcpNet::resolveHost(IPaddress *const address, const char *const host,
                        const Uint16 port)
{
}

TcpNet::Socket TcpNet::open(IPaddress *const ip)
{
}

TcpNet::SocketSet TcpNet::allocSocketSet(const int maxsockets)
{
}

int TcpNet::addSocket(const TcpNet::SocketSet set, const TcpNet::Socket sock)
{
}

int TcpNet::checkSockets(const TcpNet::SocketSet set, const Uint32 timeout)
{
}

int TcpNet::recv(const TcpNet::Socket sock, void *const data, const int maxlen)
{
}

int TcpNet::delSocket(const TcpNet::SocketSet set, const TcpNet::Socket sock)
{
}

void TcpNet::freeSocketSet(const TcpNet::SocketSet set)
{
}

#endif  // __native_client__
