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

#ifndef NET_EA_NPCHANDLER_H
#define NET_EA_NPCHANDLER_H

#include "net/messagein.h"
#include "net/net.h"
#include "net/npchandler.h"

#include <map>

class NpcDialog;

namespace Ea
{

class NpcHandler : public Net::NpcHandler
{
    public:
        NpcHandler();

        A_DELETE_COPY(NpcHandler)

        void sendLetter(int npcId, const std::string &recipient,
                        const std::string &text);

        void startShopping(int beingId);

        void endShopping(int beingId);

        void clearDialogs();

        virtual int getNpc(Net::MessageIn &msg, bool haveLength) = 0;

        void processNpcChoice(Net::MessageIn &msg);

        void processNpcMessage(Net::MessageIn &msg);

        void processNpcClose(Net::MessageIn &msg);

        void processNpcNext(Net::MessageIn &msg);

        void processNpcIntInput(Net::MessageIn &msg);

        void processNpcStrInput(Net::MessageIn &msg);

    protected:
        typedef struct
        {
            NpcDialog* dialog;
        } Wrapper;
        typedef std::map<int, Wrapper> NpcDialogs;
        NpcDialogs mNpcDialogs;
        NpcDialog *mDialog;
};

} // namespace Ea

#endif // NET_EA_NPCHANDLER_H
