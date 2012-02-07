/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_EA_GAMEHANDLER_H
#define NET_EA_GAMEHANDLER_H

#include "listener.h"

#include "net/gamehandler.h"
#include "net/messagein.h"
#include "net/net.h"

namespace Ea
{

class GameHandler : public Net::GameHandler, public Listener
{
    public:
        GameHandler();

        virtual void processEvent(Channels channel,
                                  const Event &event);

        virtual void who();

        virtual bool removeDeadBeings() const
        { return true; }

        virtual void setMap(const std::string map);

        virtual bool canUseMagicBar() const
        { return true; }

        virtual void mapLoadedEvent() = 0;

        virtual void processMapLogin(Net::MessageIn &msg);

        virtual void processWhoAnswer(Net::MessageIn &msg);

        virtual void processCharSwitchResponse(Net::MessageIn &msg);

        virtual void processMapQuitResponse(Net::MessageIn &msg);

    protected:
        std::string mMap;
        int mCharID; /// < Saved for map-server switching
};

} // namespace Ea

#endif // NET_EA_GAMEHANDLER_H
