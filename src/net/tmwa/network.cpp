/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "net/tmwa/messagein.h"
#include "net/tmwa/protocol.h"

#include "debug.h"

namespace TmwAthena
{
/** Warning: buffers and other variables are shared,
    so there can be only one connection active at a time */

int16_t packet_lengths[] =
{
 10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0040
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,  50,   3,  -1,  55,  17,   3,  37,  46,  -1,  23,  -1,   3, 108,   3,   2,
  3,  28,  19,  11,   3,  -1,   9,   5,  54,  53,  58,  60,  41,   2,   6,   6,
// #0x0080
  7,   3,   2,   2,   2,   5,  16,  12,  10,   7,  29,  23,  -1,  -1,  -1,   0,
  7,  22,  28,   2,   6,  30,  -1,  -1,   3,  -1,  -1,   5,   9,  17,  17,   6,
 23,   6,   6,  -1,  -1,  -1,  -1,   8,   7,   6,   7,   4,   7,   0,  -1,   6,
  8,   8,   3,   3,  -1,   6,   6,  -1,   7,   6,   2,   5,   6,  44,   5,   3,
// #0x00C0
  7,   2,   6,   8,   6,   7,  -1,  -1,  -1,  -1,   3,   3,   6,   6,   2,  27,
  3,   4,   4,   2,  -1,  -1,   3,  -1,   6,  14,   3,  -1,  28,  29,  -1,  -1,
 30,  30,  26,   2,   6,  26,   3,   3,   8,  19,   5,   2,   3,   2,   2,   2,
  3,   2,   6,   8,  21,   8,   8,   2,   2,  26,   3,  -1,   6,  27,  30,  10,
// #0x0100
  2,   6,   6,  30,  79,  31,  10,  10,  -1,  -1,   4,   6,   6,   2,  11,  -1,
 10,  39,   4,  10,  31,  35,  10,  18,   2,  13,  15,  20,  68,   2,   3,  16,
  6,  14,  -1,  -1,  21,   8,   8,   8,   8,   8,   2,   2,   3,   4,   2,  -1,
  6,  86,   6,  -1,  -1,   7,  -1,   6,   3,  16,   4,   4,   4,   6,  24,  26,
// #0x0140
 22,  14,   6,  10,  23,  19,   6,  39,   8,   9,   6,  27,  -1,   2,   6,   6,
110,   6,  -1,  -1,  -1,  -1,  -1,   6,  -1,  54,  66,  54,  90,  42,   6,  42,
 -1,  -1,  -1,  -1,  -1,  30,  -1,   3,  14,   3,  30,  10,  43,  14, 186, 182,
 14,  30,  10,   3,  -1,   6, 106,  -1,   4,   5,   4,  -1,   6,   7,  -1,  -1,
// #0x0180
  6,   3,  106,  10,  10, 34,   0,   6,   8,   4,   4,   4,  29,  -1,  10,   6,
 90,  86,  24,   6,  30, 102,   9,   4,   8,   4,  14,  10,   4,   6,   2,   6,
  3,   3,  35,   5,  11,  26,  -1,   4,   4,   6,  10,  12,   6,  -1,   4,   4,
 11,   7,  -1,  67,  12,  18, 114,   6,   3,   6,  26,  26,  26,  26,   2,   3,
// #0x01C0
  2,  14,  10,  -1,  22,  22,   4,   2,  13,  97,   0,   9,   9,  29,   6,  28,
  8,  14,  10,  35,   6,   8,   4,  11,  54,  53,  60,   2,  -1,  47,  33,   6,
 30,   8,  34,  14,   2,   6,  26,   2,  28,  81,   6,  10,  26,   2,  -1,  -1,
 -1,  -1,  20,  10,  32,   9,  34,  14,   2,   6,  48,  56,  -1,   4,   5,  10,
// #0x0200
 26,   0,   0,   0,  18,   0,   0,   0,   0,   0,   0,  19,  10,   0,   0,   0,
  2,  -1,  16,   0,   8,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 -1, 122,  -1,  -1,  -1,  -1,  10,  -1,  -1,   0,   0,   0,   0,   0,   0,   0,
};

static const int packet_lengths_size
    = static_cast<int>(sizeof(packet_lengths) / sizeof(int16_t));
static const int messagesSize = 0xffff;
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
    delete mMessageHandlers;
    mMessageHandlers = nullptr;
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
    for (int f = 0; f < messagesSize; f ++)
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
    while (messageReady())
    {
        SDL_mutexP(mMutexIn);
        BLOCK_START("Network::dispatchMessages 2")
        const int msgId = readWord(0);
        int len = -1;
        if (msgId == SMSG_SERVER_VERSION_RESPONSE)
            len = 10;
        else if (msgId == SMSG_UPDATE_HOST2)
            len = -1;
        else if (msgId >= 0 && msgId < packet_lengths_size)
            len = packet_lengths[msgId];

        if (len == -1)
            len = readWord(2);

        MessageIn msg(mInBuffer, len);
        msg.postInit();
        SDL_mutexV(mMutexIn);
        BLOCK_END("Network::dispatchMessages 2")
        BLOCK_START("Network::dispatchMessages 3")

        if (len == 0)
        {
            // need copy data for safty
            std::string str = strprintf("Wrong packet %d ""received. Exiting.",
                msgId);
            logger->safeError(str);
        }

        if (msgId >= 0 && msgId < messagesSize)
        {
            MessageHandler *const handler = mMessageHandlers[msgId];
            if (handler)
                handler->handleMessage(msg);
            else
                logger->log("Unhandled packet: %x", msgId);
        }

        skip(len);
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
        else if (msgId == SMSG_UPDATE_HOST2)
        {
            len = -1;
        }
        else
        {
            if (msgId >= 0 && msgId < packet_lengths_size)
                len = packet_lengths[msgId];
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
