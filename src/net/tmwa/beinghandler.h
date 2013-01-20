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

#ifndef NET_TA_BEINGHANDLER_H
#define NET_TA_BEINGHANDLER_H

#include "net/beinghandler.h"
#include "net/net.h"

#include "net/ea/beinghandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class BeingHandler final : public MessageHandler, public Ea::BeingHandler
{
    public:
        BeingHandler(bool enableSync);

        A_DELETE_COPY(BeingHandler)

        virtual void handleMessage(Net::MessageIn &msg);

        virtual void requestNameById(int id);

        virtual void undress(Being *being);

    protected:
        virtual void processBeingChangeLook(Net::MessageIn &msg, bool look2);

        void processNameResponse2(Net::MessageIn &msg);

        virtual void processPlayerMoveUpdate(Net::MessageIn &msg, int type);
};

} // namespace TmwAthena

#endif // NET_TA_BEINGHANDLER_H
