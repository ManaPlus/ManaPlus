/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef NET_EA_CHARSERVERHANDLER_H
#define NET_EA_CHARSERVERHANDLER_H

#include "net/charhandler.h"
#include "net/messagein.h"
#include "net/net.h"
#include "net/serverinfo.h"

class LoginData;

namespace Ea
{

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler : public Net::CharHandler
{
    public:
        CharServerHandler();

        virtual void setCharSelectDialog(CharSelectDialog *window);

        /**
         * Sets the character create dialog. The handler will clean up this
         * dialog when a new character is succesfully created, and will unlock
         * the dialog when a new character failed to be created.
         */
        virtual void setCharCreateDialog(CharCreateDialog *window);

        virtual void requestCharacters();

        virtual unsigned int baseSprite() const;

        virtual unsigned int hairSprite() const;

        virtual unsigned int maxSprite() const;

        virtual void connect() = 0;

        virtual void processCharLogin(Net::MessageIn &msg) = 0;

        virtual void processCharLoginError(Net::MessageIn &msg);

        virtual void processCharCreate(Net::MessageIn &msg, bool withColors);

        virtual void processCharCreateFailed(Net::MessageIn &msg);

        virtual void processCharDelete(Net::MessageIn &msg);

        virtual void processCharDeleteFailed(Net::MessageIn &msg);

        virtual void clear();

    protected:
        virtual void readPlayerData(Net::MessageIn &msg,
                                    Net::Character *character,
                                    bool withColors) = 0;
};

} // namespace Ea

#endif // NET_EA_CHARSERVERHANDLER_H
