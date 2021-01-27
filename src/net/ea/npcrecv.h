/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_EA_NPCRECV_H
#define NET_EA_NPCRECV_H

#include "localconsts.h"

class NpcDialog;

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace Ea
{
    namespace NpcRecv
    {
        extern NpcDialog *mDialog;
        extern bool mRequestLang;

        void processNpcChoice(Net::MessageIn &msg);
        void processNpcMessage(Net::MessageIn &msg);
        void processNpcClose(Net::MessageIn &msg);
        void processNpcNext(Net::MessageIn &msg);
        void processNpcIntInput(Net::MessageIn &msg);
        void processNpcStrInput(Net::MessageIn &msg);
        void processNpcCommand(Net::MessageIn &msg);
        void processChangeTitle(Net::MessageIn &msg);
    }  // namespace NpcRecv
}  // namespace Ea

#endif  // NET_EA_NPCRECV_H
