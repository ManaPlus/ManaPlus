/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_GAMEHANDLER_H
#define NET_GAMEHANDLER_H

#include "net/logindata.h"

#include <iosfwd>

namespace Net
{

class GameHandler
{
    public:
        virtual ~GameHandler()
        {}

        virtual void connect() = 0;

        virtual bool isConnected() const = 0;

        virtual void disconnect() = 0;

        virtual void who() const = 0;

        virtual void quit() const = 0;

        virtual void ping(const int tick) const = 0;

        virtual bool removeDeadBeings() const = 0;

        virtual void disconnect2() const = 0;

        virtual bool canUseMagicBar() const A_WARN_UNUSED = 0;

        virtual bool mustPing() const A_WARN_UNUSED = 0;

        virtual void mapLoadedEvent() const = 0;

        virtual void initEngines() const = 0;

        virtual void clear() = 0;
};

}  // namespace Net

#endif  // NET_GAMEHANDLER_H
