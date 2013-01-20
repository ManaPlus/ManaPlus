/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_EATHENA_GENERALHANDLER_H
#define NET_EATHENA_GENERALHANDLER_H

#include "listener.h"

#include "net/generalhandler.h"
#include "net/net.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{

class GeneralHandler final : public MessageHandler,
                             public Net::GeneralHandler,
                             public Listener
{
    public:
        GeneralHandler();

        A_DELETE_COPY(GeneralHandler)

        ~GeneralHandler();

        void handleMessage(Net::MessageIn &msg);

        void load();

        void reload();

        void unload();

        void flushNetwork();

        void clearHandlers();

        void processEvent(Channels channel,
                          const DepricatedEvent &event) override;

        void reloadPartially();

        void requestOnlineList();

    protected:
        MessageHandlerPtr mAdminHandler;
        MessageHandlerPtr mBeingHandler;
        MessageHandlerPtr mBuySellHandler;
        MessageHandlerPtr mCharHandler;
        MessageHandlerPtr mChatHandler;
        MessageHandlerPtr mGameHandler;
        MessageHandlerPtr mGuildHandler;
        MessageHandlerPtr mInventoryHandler;
        MessageHandlerPtr mItemHandler;
        MessageHandlerPtr mLoginHandler;
        MessageHandlerPtr mNpcHandler;
        MessageHandlerPtr mPartyHandler;
        MessageHandlerPtr mPlayerHandler;
        MessageHandlerPtr mSpecialHandler;
        MessageHandlerPtr mTradeHandler;
};

} // namespace EAthena

#endif // NET_EATHENA_GENERALHANDLER_H
