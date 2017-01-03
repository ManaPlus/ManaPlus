/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/ea/adminrecv.h"
#include "net/ea/beingrecv.h"
#include "net/ea/buysellrecv.h"
#include "net/ea/charserverrecv.h"
#include "net/ea/chatrecv.h"
#include "net/ea/gamerecv.h"
#include "net/ea/inventoryrecv.h"
#include "net/ea/itemrecv.h"
#include "net/ea/loginrecv.h"
#include "net/ea/maprecv.h"
#include "net/ea/npcrecv.h"
#include "net/ea/partyrecv.h"
#include "net/ea/playerrecv.h"
#include "net/ea/skillrecv.h"
#include "net/ea/traderecv.h"

#include "net/tmwa/beingrecv.h"
#include "net/tmwa/buysellrecv.h"
#include "net/tmwa/charserverrecv.h"
#include "net/tmwa/chatrecv.h"
#include "net/tmwa/gamerecv.h"
#include "net/tmwa/generalrecv.h"
#include "net/tmwa/inventoryrecv.h"
#include "net/tmwa/itemrecv.h"
#include "net/tmwa/loginrecv.h"
#include "net/tmwa/partyrecv.h"
#include "net/tmwa/playerrecv.h"
#include "net/tmwa/questrecv.h"
#include "net/tmwa/skillrecv.h"
#include "net/tmwa/traderecv.h"

#include "net/tmwa/messagein.h"

#include "debug.h"

namespace TmwAthena
{

static const unsigned int packet_lengths_size = 0xFFFFU;
static const unsigned int messagesSize = 0xFFFFU;
Network *Network::mInstance = nullptr;

Network::Network() :
    Ea::Network()
{
    mInstance = this;
    mPackets = new PacketInfo[messagesSize];
}

Network::~Network()
{
    clearHandlers();
    mInstance = nullptr;
}

void Network::registerHandlers()
{
#include "net/tmwa/recvpackets.inc"
    RECVPACKETS_VOID
}

void Network::clearHandlers()
{
    for (size_t f = 0; f < packet_lengths_size; f ++)
    {
        mPackets[f].name = "";
        mPackets[f].len = 0;
        mPackets[f].func = nullptr;
        mPackets[f].version = 0;
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
        if (msgId < packet_lengths_size)
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
            const PacketFuncPtr func = mPackets[msgId].func;
            if (func)
                func(msg);
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
        if (msgId >= 0 && CAST_U32(msgId)
            < packet_lengths_size)
        {
            len = mPackets[msgId].len;
        }

        if (len == -1 && mInSize > 4)
            len = readWord(2);
    }

    const bool ret = (mInSize >= CAST_U32(len));
    SDL_mutexV(mMutexIn);

    return ret;
}

Network *Network::instance()
{
    return mInstance;
}

}  // namespace TmwAthena
