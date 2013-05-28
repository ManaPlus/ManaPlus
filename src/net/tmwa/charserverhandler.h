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

#ifndef NET_TMWA_CHARSERVERHANDLER_H
#define NET_TMWA_CHARSERVERHANDLER_H

#include "net/charserverhandler.h"

#include "net/ea/charserverhandler.h"

#include "net/tmwa/messagehandler.h"

#include "net/ea/token.h"

class LoginData;

namespace TmwAthena
{

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler final : public MessageHandler,
                                public Ea::CharServerHandler
{
    public:
        CharServerHandler();

        A_DELETE_COPY(CharServerHandler)

        virtual void handleMessage(Net::MessageIn &msg) override;

        void chooseCharacter(Net::Character *const character) override;

        void newCharacter(const std::string &name, const int slot,
                          const bool gender, const int hairstyle,
                          const int hairColor, const unsigned char race,
                          const std::vector<int> &stats) const override;

        void deleteCharacter(Net::Character *const character) override;

        void switchCharacter() const override;

        void connect() override;

        void processCharLogin(Net::MessageIn &msg);

    protected:
        void readPlayerData(Net::MessageIn &msg,
                            Net::Character *const character,
                            const bool withColors) const override;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_CHARSERVERHANDLER_H
