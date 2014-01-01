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

#ifndef NET_GENERALHANDLER_H
#define NET_GENERALHANDLER_H

namespace Net
{

class GeneralHandler
{
    public:
        virtual ~GeneralHandler()
        { }

        virtual void load() = 0;

        virtual void reload() = 0;

        virtual void unload() = 0;

        virtual void flushNetwork() = 0;

        virtual void clearHandlers() = 0;

        virtual void reloadPartially() const = 0;

        virtual void gameStarted() const = 0;

        virtual void gameEnded() const = 0;
};

}  // namespace Net

#endif  // NET_GENERALHANDLER_H
