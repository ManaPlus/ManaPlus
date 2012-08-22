/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "net/ea/network.h"

#include "configuration.h"
#include "logger.h"

#include "net/messagehandler.h"

#include "net/eathena/protocol.h"

#include "utils/gettext.h"

#include <assert.h>
#include <sstream>

#include "debug.h"

namespace Ea
{

const unsigned int BUFFER_SIZE = 1000000;
const unsigned int BUFFER_LIMIT = 930000;

int networkThread(void *data)
{
    Network *network = static_cast<Network*>(data);

    if (!network || !network->realConnect())
        return -1;

    network->receive();

    return 0;
}

Network::Network() :
    mSocket(nullptr),
    mInBuffer(new char[BUFFER_SIZE]),
    mOutBuffer(new char[BUFFER_SIZE]),
    mInSize(0),
    mOutSize(0),
    mToSkip(0),
    mState(IDLE),
    mWorkerThread(nullptr)
{
    SDLNet_Init();

    mMutex = SDL_CreateMutex();
}

Network::~Network()
{
    if (mState != IDLE && mState != NET_ERROR)
        disconnect();

    SDL_DestroyMutex(mMutex);
    mMutex = nullptr;

    delete []mInBuffer;
    delete []mOutBuffer;

    SDLNet_Quit();
}

bool Network::connect(ServerInfo server)
{
    if (mState != IDLE && mState != NET_ERROR)
    {
        logger->log1("Tried to connect an already connected socket!");
        assert(false);
        return false;
    }

    if (server.hostname.empty())
    {
        setError(_("Empty address given to Network::connect()!"));
        return false;
    }

    logger->log("Network::Connecting to %s:%i",
        server.hostname.c_str(), server.port);

    mServer.hostname = server.hostname;
    mServer.port = server.port;

    // Reset to sane values
    mOutSize = 0;
    mInSize = 0;
    mToSkip = 0;

    mState = CONNECTING;
    mWorkerThread = SDL_CreateThread(networkThread, this);
    if (!mWorkerThread)
    {
        setError("Unable to create network worker thread");
        return false;
    }

    return true;
}

void Network::disconnect()
{
    mState = IDLE;

    if (mWorkerThread && SDL_GetThreadID(mWorkerThread))
    {
        SDL_WaitThread(mWorkerThread, nullptr);
        mWorkerThread = nullptr;
    }

    if (mSocket)
    {
        // need call SDLNet_TCP_DelSocket?
        SDLNet_TCP_Close(mSocket);
        mSocket = nullptr;
        int sleep = config.getIntValue("networksleep");
        if (sleep > 0)
            SDL_Delay(sleep);
    }
}

void Network::flush()
{
    if (!mOutSize || mState != CONNECTED)
        return;

    int ret;

    SDL_mutexP(mMutex);
    ret = SDLNet_TCP_Send(mSocket, mOutBuffer, mOutSize);
    DEBUGLOG("Send " + toString(mOutSize) + " bytes");
    if (ret < static_cast<int>(mOutSize))
    {
        setError("Error in SDLNet_TCP_Send(): " +
            std::string(SDLNet_GetError()));
    }
    mOutSize = 0;
    SDL_mutexV(mMutex);
}

void Network::skip(int len)
{
    SDL_mutexP(mMutex);
    mToSkip += len;
    if (!mInSize)
    {
        SDL_mutexV(mMutex);
        return;
    }

    if (mInSize >= mToSkip)
    {
        mInSize -= mToSkip;
        memmove(mInBuffer, mInBuffer + mToSkip, mInSize);
        mToSkip = 0;
    }
    else
    {
        mToSkip -= mInSize;
        mInSize = 0;
    }
    SDL_mutexV(mMutex);
}

bool Network::realConnect()
{
    IPaddress ipAddress;

    if (SDLNet_ResolveHost(&ipAddress, mServer.hostname.c_str(),
        mServer.port) == -1)
    {
        std::string errorMessage = _("Unable to resolve host \"") +
            mServer.hostname + "\"";
        setError(errorMessage);
        logger->log("SDLNet_ResolveHost: %s", errorMessage.c_str());
        return false;
    }

    mState = CONNECTING;

    mSocket = SDLNet_TCP_Open(&ipAddress);
    if (!mSocket)
    {
        logger->log("Error in SDLNet_TCP_Open(): %s", SDLNet_GetError());
        setError(SDLNet_GetError());
        return false;
    }

    logger->log("Network::Started session with %s:%i",
        ipToString(ipAddress.host), ipAddress.port);

    mState = CONNECTED;

    return true;
}

void Network::receive()
{
    SDLNet_SocketSet set;

    if (!(set = SDLNet_AllocSocketSet(1)))
    {
        setError("Error in SDLNet_AllocSocketSet(): " +
            std::string(SDLNet_GetError()));
        return;
    }

    if (SDLNet_TCP_AddSocket(set, mSocket) == -1)
    {
        setError("Error in SDLNet_AddSocket(): " +
            std::string(SDLNet_GetError()));
    }

    while (mState == CONNECTED)
    {
        // TODO Try to get this to block all the time while still being able
        // to escape the loop
        int numReady = SDLNet_CheckSockets(set, (static_cast<uint32_t>(500)));
        int ret;
        switch (numReady)
        {
            case -1:
                logger->log1("Error: SDLNet_CheckSockets");
                // FALLTHROUGH
            case 0:
                break;

            case 1:
                // Receive data from the socket
                SDL_mutexP(mMutex);
                if (mInSize > BUFFER_LIMIT)
                {
                    SDL_mutexV(mMutex);
                    SDL_Delay(100);
                    continue;
                }

                ret = SDLNet_TCP_Recv(mSocket, mInBuffer + mInSize,
                                      BUFFER_SIZE - mInSize);

                if (!ret)
                {
                    // We got disconnected
                    mState = IDLE;
                    logger->log1("Disconnected.");
                }
                else if (ret < 0)
                {
                    setError(_("Connection to server terminated. ") +
                             std::string(SDLNet_GetError()));
                }
                else
                {
//                    DEBUGLOG("Receive " + toString(ret) + " bytes");
                    mInSize += ret;
                    if (mToSkip)
                    {
                        if (mInSize >= mToSkip)
                        {
                            mInSize -= mToSkip;
                            memmove(mInBuffer, mInBuffer + mToSkip, mInSize);
                            mToSkip = 0;
                        }
                        else
                        {
                            mToSkip -= mInSize;
                            mInSize = 0;
                        }
                    }
                }
                SDL_mutexV(mMutex);
                break;

            default:
                // more than one socket is ready..
                // this should not happen since we only listen once socket.
                std::stringstream errorStream;
                errorStream << "Error in SDLNet_TCP_Recv(), " << numReady
                            << " sockets are ready: " << SDLNet_GetError();
                setError(errorStream.str());
                break;
        }
    }

    if (SDLNet_TCP_DelSocket(set, mSocket) == -1)
        logger->log("Error in SDLNet_DelSocket(): %s", SDLNet_GetError());

    SDLNet_FreeSocketSet(set);
}

void Network::setError(const std::string &error)
{
    logger->log("Network error: %s", error.c_str());
    mError = error;
    mState = NET_ERROR;
}

uint16_t Network::readWord(int pos)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return SDL_Swap16((*(uint16_t*)(mInBuffer + (pos))));
#else
    return (*reinterpret_cast<uint16_t*>(mInBuffer + (pos)));
#endif
}

void Network::fixSendBuffer()
{
    if (mOutSize > BUFFER_LIMIT)
    {
        if (mState != CONNECTED)
            mOutSize = 0;
        else
            flush();
    }
}

} // namespace EAthena
