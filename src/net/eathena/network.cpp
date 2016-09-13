/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/packetinfo.h"

#include "net/ea/adminrecv.h"
#include "net/ea/beingrecv.h"
#include "net/ea/buysellrecv.h"
#include "net/ea/charserverrecv.h"
#include "net/ea/chatrecv.h"
#include "net/ea/gamerecv.h"
#include "net/ea/guildrecv.h"
#include "net/ea/inventoryrecv.h"
#include "net/ea/itemrecv.h"
#include "net/ea/loginrecv.h"
#include "net/ea/maprecv.h"
#include "net/ea/npcrecv.h"
#include "net/ea/partyrecv.h"
#include "net/ea/playerrecv.h"
#include "net/ea/skillrecv.h"
#include "net/ea/traderecv.h"

#include "net/eathena/adminrecv.h"
#include "net/eathena/auctionrecv.h"
#include "net/eathena/bankrecv.h"
#include "net/eathena/battlegroundrecv.h"
#include "net/eathena/beingrecv.h"
#include "net/eathena/buyingstorerecv.h"
#include "net/eathena/buysellrecv.h"
#include "net/eathena/cashshoprecv.h"
#include "net/eathena/charserverrecv.h"
#include "net/eathena/chatrecv.h"
#include "net/eathena/elementalrecv.h"
#include "net/eathena/familyrecv.h"
#include "net/eathena/friendsrecv.h"
#include "net/eathena/gamerecv.h"
#include "net/eathena/generalrecv.h"
#include "net/eathena/guildrecv.h"
#include "net/eathena/homunculusrecv.h"
#include "net/eathena/inventoryrecv.h"
#include "net/eathena/itemrecv.h"
#include "net/eathena/loginrecv.h"
#include "net/eathena/mailrecv.h"
#include "net/eathena/maprecv.h"
#include "net/eathena/marketrecv.h"
#include "net/eathena/mercenaryrecv.h"
#include "net/eathena/npcrecv.h"
#include "net/eathena/partyrecv.h"
#include "net/eathena/petrecv.h"
#include "net/eathena/playerrecv.h"
#include "net/eathena/questrecv.h"
#include "net/eathena/rouletterecv.h"
#include "net/eathena/searchstorerecv.h"
#include "net/eathena/skillrecv.h"
#include "net/eathena/traderecv.h"
#include "net/eathena/vendingrecv.h"

#include "net/eathena/messagein.h"

#include "resources/db/networkdb.h"

#include "utils/checkutils.h"

#include "debug.h"

extern int packetVersion;
extern int serverVersion;

namespace EAthena
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
#include "net/eathena/recvpackets.inc"
    RECVPACKETS_VOID
}

void Network::registerFakeHandlers()
{
    const NetworkInPacketInfos &packets = NetworkDb::getFakePackets();
    FOR_EACH (NetworkInPacketInfosIter, it, packets)
    {
        const size_t id = (*it).first;
        if (id >= packet_lengths_size)
        {
            reportAlways("Wrong fake packet id %d", CAST_S32(id));
            continue;
        }
        if (mPackets[id].len != 0 ||
            mPackets[id].func != nullptr ||
            mPackets[id].version != 0)
        {
            continue;
        }
        const int32_t len = (*it).second;
        logger->log("Add fake packet: %d, %d",
            CAST_S32(id),
            len);
        mPackets[id].name = "fake";
        mPackets[id].len = len;
        mPackets[id].func = nullptr;
        mPackets[id].version = 0;
    }

    const NetworkRemovePacketInfos &removePackets =
        NetworkDb::getRemovePackets();
    FOR_EACH (NetworkRemovePacketInfosIter, it, removePackets)
    {
        const size_t id = *it;
        if (id >= packet_lengths_size)
        {
            reportAlways("Wrong remove packet id %d", CAST_S32(id));
            continue;
        }
        if (mPackets[id].len == 0 &&
            mPackets[id].func == nullptr &&
            mPackets[id].version == 0)
        {
            continue;
        }
        logger->log("Remove packet: %d",
            CAST_S32(id));
        mPackets[id].name = "";
        mPackets[id].len = 0;
        mPackets[id].func = nullptr;
        mPackets[id].version = 0;
    }
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
    mPauseDispatch = false;
    while (messageReady())
    {
        SDL_mutexP(mMutexIn);
        const unsigned int msgId = readWord(0);
        int len = -1;
        if (msgId < packet_lengths_size)
            len = mPackets[msgId].len;
        if (len == -1)
            len = readWord(2);

        MessageIn msg(mInBuffer, len);
        unsigned int ver = mPackets[msgId].version;
        if (ver == 0)
            ver = packetVersion;
        msg.postInit(mPackets[msgId].name, ver);
        SDL_mutexV(mMutexIn);

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
            break;
    }
}

bool Network::messageReady()
{
    int len = -1;

    SDL_mutexP(mMutexIn);
    if (mInSize >= 2)
    {
        const int msgId = readWord(0);
        if (msgId >= 0 &&
            CAST_U32(msgId) < packet_lengths_size)
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

}  // namespace EAthena
