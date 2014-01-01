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

#ifndef NET_EA_NPCHANDLER_H
#define NET_EA_NPCHANDLER_H

#include "net/messagein.h"
#include "net/npchandler.h"

class NpcDialog;

namespace Ea
{

class NpcHandler : public Net::NpcHandler
{
    public:
        A_DELETE_COPY(NpcHandler)

        void sendLetter(const int npcId, const std::string &recipient,
                        const std::string &text) const override final;

        void startShopping(const int beingId) const override final;

        void endShopping(const int beingId) const override final;

        virtual int getNpc(Net::MessageIn &msg,
                           bool haveLength) A_WARN_UNUSED = 0;

        void processNpcChoice(Net::MessageIn &msg);

        void processNpcMessage(Net::MessageIn &msg);

        void processNpcClose(Net::MessageIn &msg);

        void processNpcNext(Net::MessageIn &msg);

        void processNpcIntInput(Net::MessageIn &msg);

        void processNpcStrInput(Net::MessageIn &msg);

    protected:
        NpcHandler();

        NpcDialog *mDialog;
};

}  // namespace Ea

#endif  // NET_EA_NPCHANDLER_H
