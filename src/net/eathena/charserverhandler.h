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

#ifndef NET_EATHENA_CHARSERVERHANDLER_H
#define NET_EATHENA_CHARSERVERHANDLER_H

#include "net/charhandler.h"

#include "net/ea/charserverhandler.h"

#include "net/eathena/messagehandler.h"

#include "net/ea/token.h"

class LoginData;

namespace EAthena
{

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler final : public MessageHandler,
                                public Ea::CharServerHandler
{
    public:
        CharServerHandler();

        virtual void handleMessage(Net::MessageIn &msg);

        void chooseCharacter(Net::Character *character);

        void newCharacter(const std::string &name, int slot,
                          bool gender, int hairstyle, int hairColor,
                          unsigned char race,
                          const std::vector<int> &stats);

        void deleteCharacter(Net::Character *character);

        void switchCharacter();

        void connect();

        void processCharLogin(Net::MessageIn &msg);

    protected:
        void readPlayerData(Net::MessageIn &msg, Net::Character *character,
                            bool);
};

} // namespace EAthena

#endif // NET_EATHENA_CHARSERVERHANDLER_H
