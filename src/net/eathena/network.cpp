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

#include "net/eathena/network.h"

#include "logger.h"

#include "net/eathena/messagehandler.h"
#include "net/eathena/messagein.h"
#include "net/eathena/packets.h"
#include "net/eathena/protocol.h"

#include "utils/delete2.h"

#include "debug.h"

namespace EAthena
{

static const unsigned int packet_lengths_size
    = static_cast<unsigned int>(sizeof(packet_lengths) / sizeof(int16_t));
static const unsigned int messagesSize = 0xFFFFU;
Network *Network::mInstance = nullptr;

Network::Network() :
    Ea::Network(),
    mMessageHandlers(new MessageHandler*[messagesSize])
{
    mInstance = this;
    memset(&mMessageHandlers[0], 0, sizeof(MessageHandler*) * 0xffff);
}

Network::~Network()
{
    clearHandlers();
    delete2(mMessageHandlers);
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
    while (messageReady())
    {
        SDL_mutexP(mMutexIn);
        const unsigned int msgId = readWord(0);
        int len = -1;
        if (msgId == SMSG_UPDATE_HOST2)
        {
            len = -1;
        }
        else
        {
            if (msgId < packet_lengths_size)
                len = packet_lengths[msgId];
        }

        if (len == -1)
            len = readWord(2);

        MessageIn msg(mInBuffer, len);
        msg.postInit();
        SDL_mutexV(mMutexIn);

        if (len == 0)
        {
            // need copy data for safty
            std::string str = strprintf("Wrong packet %u ""received. Exiting.",
                msgId);
            logger->safeError(str);
        }

        if (msgId < messagesSize)
        {
            MessageHandler *const handler = mMessageHandlers[msgId];
            if (handler)
                handler->handleMessage(msg);
            else
                logger->log("Unhandled packet: %x", msgId);
        }

        skip(len);
    }
}

bool Network::messageReady()
{
    int len = -1;

    SDL_mutexP(mMutexIn);
    if (mInSize >= 2)
    {
        const int msgId = readWord(0);
        if (msgId == SMSG_UPDATE_HOST2)
            len = -1;
        else
            len = packet_lengths[msgId];

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

}  // namespace EAthena
