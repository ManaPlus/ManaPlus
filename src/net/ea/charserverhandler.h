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

#ifndef NET_EA_CHARSERVERHANDLER_H
#define NET_EA_CHARSERVERHANDLER_H

#include "net/charserverhandler.h"
#include "net/messagein.h"
#include "net/serverinfo.h"

namespace Ea
{

class Network;

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler : public Net::CharServerHandler
{
    public:
        A_DELETE_COPY(CharServerHandler)

        virtual void setCharSelectDialog(CharSelectDialog *const window)
                                         override final;

        /**
         * Sets the character create dialog. The handler will clean up this
         * dialog when a new character is succesfully created, and will unlock
         * the dialog when a new character failed to be created.
         */
        virtual void setCharCreateDialog(CharCreateDialog *const window)
                                         override final;

        virtual void requestCharacters() override final;

        virtual unsigned int baseSprite() const override final A_WARN_UNUSED;

        virtual unsigned int hairSprite() const override final A_WARN_UNUSED;

        virtual unsigned int maxSprite() const override final A_WARN_UNUSED;

        virtual void connect() = 0;

        virtual void processCharLoginError(Net::MessageIn &msg) const;

        virtual void processCharCreate(Net::MessageIn &msg,
                                       const bool withColors);

        virtual void processCharCreateFailed(Net::MessageIn &msg);

        virtual void processCharDelete(Net::MessageIn &msg);

        virtual void processCharDeleteFailed(Net::MessageIn &msg);

        virtual void processCharMapInfo(Net::MessageIn &restrict msg,
                                        Network *restrict const network,
                                        ServerInfo &restrict mapServer);

        virtual void processChangeMapServer(Net::MessageIn &restrict msg,
                                            Network *restrict const network,
                                            ServerInfo &restrict mapServer)
                                            const;

        virtual void clear() override final;

    protected:
        CharServerHandler();

        virtual void readPlayerData(Net::MessageIn &msg,
                                    Net::Character *const character,
                                    const bool withColors) const = 0;
};

}  // namespace Ea

#endif  // NET_EA_CHARSERVERHANDLER_H
