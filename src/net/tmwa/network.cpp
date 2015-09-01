/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/tmwa/network.h"

#include "logger.h"

#include "net/packetinfo.h"

#include "net/tmwa/messagehandler.h"
#include "net/tmwa/messagein.h"
#include "net/tmwa/protocol.h"

#include "utils/delete2.h"

#include "debug.h"

namespace TmwAthena
{

static const unsigned int packet_lengths_size = 0x0230U;
static const unsigned int messagesSize = 0xFFFFU;
Network *Network::mInstance = nullptr;

Network::Network() :
    Ea::Network(),
    mMessageHandlers(new MessageHandler*[messagesSize])
{
    mInstance = this;
    memset(&mMessageHandlers[0], 0, sizeof(MessageHandler*) * 0xffff);
    mPackets = new PacketInfo[messagesSize];
#include "net/tmwa/recvpackets.inc"
}

Network::~Network()
{
    clearHandlers();
    delete2Arr(mMessageHandlers);
    mInstance = nullptr;
}

void Network::registerHandler(MessageHandler *const handler)
{
    if (!handler)
        return;

    for (const uint16_t *i = handler->handledMessages; *i; ++i)
        mMessageHandlers[*i] = handler;

    handler->setNetwork(this);
}

void Network::unregisterHandler(MessageHandler *const handler)
{
    if (!handler)
        return;

    for (const uint16_t *i = handler->handledMessages; *i; ++i)
        mMessageHandlers[*i] = nullptr;

    handler->setNetwork(nullptr);
}

void Network::clearHandlers()
{
    for (size_t f = 0; f < messagesSize; f ++)
    {
        if (mMessageHandlers[f])
        {
            mMessageHandlers[f]->setNetwork(nullptr);
            mMessageHandlers[f] = nullptr;
        }
    }
}

void Network::dispatchMessages()
{
    BLOCK_START("Network::dispatchMessages 1")
    mPauseDispatch = false;
    while (messageReady())
    {
        SDL_mutexP(mMutexIn);
        BLOCK_START("Network::dispatchMessages 2")
        const unsigned int msgId = readWord(0);
        int len = -1;
        if (msgId == SMSG_SERVER_VERSION_RESPONSE)
            len = 10;
        else if (msgId < packet_lengths_size)
            len = mPackets[msgId].len;

        if (len == -1)
            len = readWord(2);

        MessageIn msg(mInBuffer, len);
        msg.postInit(mPackets[msgId].name);
        SDL_mutexV(mMutexIn);
        BLOCK_END("Network::dispatchMessages 2")
        BLOCK_START("Network::dispatchMessages 3")

        if (len == 0)
        {
            // need copy data for safty
            std::string str = strprintf(
                "Wrong packet %u 0x%x received. Exiting.",
                msgId, msgId);
            logger->safeError(str);
        }

        if (msgId < messagesSize)
        {
            MessageHandler *const handler = mMessageHandlers[msgId];
            if (handler)
                handler->handleMessage(msg);
            else
                logger->log("Unhandled packet: %u 0x%x", msgId, msgId);
        }

        skip(len);
        if (mPauseDispatch)
        {
            BLOCK_END("Network::dispatchMessages 3")
            break;
        }
        BLOCK_END("Network::dispatchMessages 3")
    }
    BLOCK_END("Network::dispatchMessages 1")
}

bool Network::messageReady()
{
    int len = -1;

    SDL_mutexP(mMutexIn);
    if (mInSize >= 2)
    {
        const int msgId = readWord(0);
        if (msgId == SMSG_SERVER_VERSION_RESPONSE)
        {
            len = 10;
        }
        else
        {
            if (msgId >= 0 && static_cast<unsigned int>(msgId)
                < packet_lengths_size)
            {
                len = mPackets[msgId].len;
            }
        }

        if (len == -1 && mInSize > 4)
            len = readWord(2);
    }

    const bool ret = (mInSize >= static_cast<unsigned int>(len));
    SDL_mutexV(mMutexIn);

    return ret;
}

Network *Network::instance()
{
    return mInstance;
}

}  // namespace TmwAthena
