/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef NET_EATHENA_CHARSERVERRECV_H
#define NET_EATHENA_CHARSERVERRECV_H

#include "enums/simpletypes/beingid.h"

#include <string>

namespace Net
{
    class MessageIn;

    struct Character;
}  // namespace Net

namespace EAthena
{
    namespace CharServerRecv
    {
        extern std::string mNewName;
        extern uint32_t mPinSeed;
        extern BeingId mPinAccountId;
        extern BeingId mRenameId;
        extern bool mNeedCreatePin;

        void readPlayerData(Net::MessageIn &msg,
                            Net::Character *const character);
        void processPincodeStatus(Net::MessageIn &msg);
        void processCharLogin2(Net::MessageIn &msg);
        void processCharCreate(Net::MessageIn &msg);
        void processCharCheckRename(Net::MessageIn &msg);
        void processCharRename(Net::MessageIn &msg);
        void processCharChangeSlot(Net::MessageIn &msg);
        void processCharDeleteFailed(Net::MessageIn &msg);
        void processCharCaptchaNotSupported(Net::MessageIn &msg);
        void processCharDelete2Ack(Net::MessageIn &msg);
        void processCharDelete2AcceptActual(Net::MessageIn &msg);
        void processCharDelete2CancelAck(Net::MessageIn &msg);
        void processCharCharacters(Net::MessageIn &msg);
        void processCharBanCharList(Net::MessageIn &msg);
        void processCharLogin(Net::MessageIn &msg);
        void processCharMapInfo(Net::MessageIn &msg);
        void processChangeMapServer(Net::MessageIn &msg);
    }  // namespace CharServerRecv
}  // namespace EAthena

#endif  // NET_EATHENA_CHARSERVERRECV_H
