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

#include "net/npchandler.h"

#include "localconsts.h"

class NpcDialog;

namespace Net
{
    class MessageIn;
}

namespace Ea
{

class NpcHandler notfinal : public Net::NpcHandler
{
    public:
        A_DELETE_COPY(NpcHandler)

        static void processNpcChoice(Net::MessageIn &msg);

        static void processNpcMessage(Net::MessageIn &msg);

        static void processNpcClose(Net::MessageIn &msg);

        static void processNpcNext(Net::MessageIn &msg);

        static void processNpcIntInput(Net::MessageIn &msg);

        static void processNpcStrInput(Net::MessageIn &msg);

    protected:
        NpcHandler();

        static NpcDialog *mDialog;

        static bool mRequestLang;
};

}  // namespace Ea

#endif  // NET_EA_NPCHANDLER_H
