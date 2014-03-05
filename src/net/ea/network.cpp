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

#include "net/ea/network.h"

#include "configuration.h"
#include "logger.h"

#include "utils/gettext.h"
#include "utils/sdlhelper.h"

#include <sstream>

#include "debug.h"

extern unsigned int mLastHost;

namespace Ea
{

const unsigned int BUFFER_SIZE = 1000000;
const unsigned int BUFFER_LIMIT = 930000;

int networkThread(void *data)
{
    Network *const network = static_cast<Network *const>(data);

    if (!network || !network->realConnect())
        return -1;

    network->receive();

    return 0;
}

Network::Network() :
    mSocket(nullptr),
    mServer(),
    mInBuffer(new char[BUFFER_SIZE]),
    mOutBuffer(new char[BUFFER_SIZE]),
    mInSize(0),
    mOutSize(0),
    mToSkip(0),
    mState(IDLE),
    mError(),
    mWorkerThread(nullptr),
    mMutexIn(SDL_CreateMutex()),
    mMutexOut(SDL_CreateMutex()),
    mSleep(config.getIntValue("networksleep"))
{
    TcpNet::init();
}

Network::~Network()
{
    if (mState != IDLE && mState != NET_ERROR)
        disconnect();

    SDL_DestroyMutex(mMutexIn);
    mMutexIn = nullptr;
    SDL_DestroyMutex(mMutexOut);
    mMutexOut = nullptr;

    delete []mInBuffer;
    delete []mOutBuffer;

    TcpNet::quit();
}

bool Network::connect(ServerInfo server)
{
    if (mState != IDLE && mState != NET_ERROR)
    {
        logger->log1("Tried to connect an already connected socket!");
        return false;
    }

    if (server.hostname.empty())
    {
        // TRANSLATORS: error message
        setError(_("Empty address given to Network::connect()!"));
        return false;
    }

    logger->log("Network::Connecting to %s:%i",
        server.hostname.c_str(), server.port);

    mServer.hostname = server.hostname;
    mServer.althostname = server.althostname;
    mServer.port = server.port;

    // Reset to sane values
    mOutSize = 0;
    mInSize = 0;
    mToSkip = 0;

    mState = CONNECTING;
    mWorkerThread = SDL::createThread(&networkThread, "network", this);
    if (!mWorkerThread)
    {
        setError("Unable to create network worker thread");
        return false;
    }

    return true;
}

void Network::disconnect()
{
    BLOCK_START("Network::disconnect")
    mState = IDLE;

    if (mWorkerThread && SDL_GetThreadID(mWorkerThread))
    {
        SDL_WaitThread(mWorkerThread, nullptr);
        mWorkerThread = nullptr;
    }

    if (mSocket)
    {
        TcpNet::closeSocket(mSocket);
        mSocket = nullptr;
        if (mSleep > 0)
            SDL_Delay(mSleep);
    }
    BLOCK_END("Network::disconnect")
}

void Network::flush()
{
    if (!mOutSize || mState != CONNECTED)
        return;

    SDL_mutexP(mMutexOut);
    const int ret = TcpNet::send(mSocket, mOutBuffer, mOutSize);
    DEBUGLOG(std::string("Send ").append(toString(mOutSize)).append(" bytes"));
    if (ret < static_cast<int>(mOutSize))
    {
        SDL_mutexV(mMutexOut);
        setError("Error in TcpNet::send(): " +
            std::string(TcpNet::getError()));
    }
    mOutSize = 0;
    SDL_mutexV(mMutexOut);
}

void Network::skip(const int len)
{
    SDL_mutexP(mMutexIn);
    mToSkip += len;
    if (!mInSize)
    {
        SDL_mutexV(mMutexIn);
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
    SDL_mutexV(mMutexIn);
}

bool Network::realConnect()
{
    IPaddress ipAddress;

    if (TcpNet::resolveHost(&ipAddress, mServer.hostname.c_str(),
        mServer.port) == -1)
    {
        if (mServer.althostname.empty() || TcpNet::resolveHost(&ipAddress,
            mServer.althostname.c_str(), mServer.port) == -1)
        {
            const std::string errorMessage = std::string(
                // TRANSLATORS: error message
                _("Unable to resolve host \"")).append(
                mServer.hostname).append("\".");
            setError(errorMessage);
            logger->log_r("TcpNet::ResolveHost: %s", errorMessage.c_str());
            return false;
        }
        logger->log_r("using alt host name: %s", mServer.althostname.c_str());
    }

    mState = CONNECTING;

    mSocket = TcpNet::open(&ipAddress);
    if (!mSocket)
    {
        logger->log_r("Error in TcpNet::open(): %s", TcpNet::getError());
        setError(TcpNet::getError());
        return false;
    }

    mLastHost = ipAddress.host;
    logger->log_r("Network::Started session with %s:%i",
        ipToString(ipAddress.host), ipAddress.port);

    mState = CONNECTED;
    return true;
}

void Network::receive()
{
    TcpNet::SocketSet set;

    if (!(set = TcpNet::allocSocketSet(1)))
    {
        setError("Error in TcpNet::allocSocketSet(): " +
            std::string(TcpNet::getError()));
        return;
    }

    if (TcpNet::addSocket(set, mSocket) == -1)
    {
        setError("Error in TcpNet::addSocket(): " +
            std::string(TcpNet::getError()));
    }

    while (mState == CONNECTED)
    {
        const int numReady = TcpNet::checkSockets(
            set, (static_cast<uint32_t>(500)));
        switch (numReady)
        {
            case -1:
                logger->log_r("Error: TcpNet::checkSockets");
                break;
                // FALLTHROUGH
            case 0:
                break;

            case 1:
            {
                // Receive data from the socket
                SDL_mutexP(mMutexIn);
                if (mInSize > BUFFER_LIMIT)
                {
                    SDL_mutexV(mMutexIn);
                    SDL_Delay(100);
                    continue;
                }

                const int ret = TcpNet::recv(mSocket, mInBuffer + mInSize,
                    BUFFER_SIZE - mInSize);

                if (!ret)
                {
                    // We got disconnected
                    mState = IDLE;
                    logger->log_r("Disconnected.");
                }
                else if (ret < 0)
                {
                    // TRANSLATORS: error message
                    setError(_("Connection to server terminated. ") +
                             std::string(TcpNet::getError()));
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
                SDL_mutexV(mMutexIn);
                break;
            }

            default:
                // more than one socket is ready..
                // this should not happen since we only listen once socket.
                std::stringstream errorStream;
                errorStream << "Error in TcpNet::recv(), " << numReady
                            << " sockets are ready: " << TcpNet::getError();
                setError(errorStream.str());
                break;
        }
    }

    if (TcpNet::delSocket(set, mSocket) == -1)
        logger->log_r("Error in TcpNet::delSocket(): %s", TcpNet::getError());

    TcpNet::freeSocketSet(set);
}

void Network::setError(const std::string &error)
{
    logger->log_r("Network error: %s", error.c_str());
    mError = error;
    mState = NET_ERROR;
}

uint16_t Network::readWord(const int pos) const
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return SDL_Swap16(*reinterpret_cast<uint16_t*>(mInBuffer + (pos)));
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

}  // namespace Ea
