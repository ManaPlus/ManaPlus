/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef NET_EA_ADMINHANDLER_H
#define NET_EA_ADMINHANDLER_H

#include "net/adminhandler.h"

#include "localconsts.h"

namespace Ea
{

class AdminHandler notfinal : public Net::AdminHandler
{
    public:
        AdminHandler()
        { }

        A_DELETE_COPY(AdminHandler)

        virtual ~AdminHandler()
        { }

        void kickName(const std::string &name) const final;

        void ban(const int playerId) const final A_CONST;

        void banName(const std::string &name) const final;

        void unban(const int playerId) const final A_CONST;

        void unbanName(const std::string &name) const final;

        void ipcheckName(const std::string &name) const final;

        void slide(const int x, const int y) const final;

        void createItems(const int id,
                         const ItemColor color,
                         const int amount) const final;

        void spawn(const std::string &name) const final;
};

}  // namespace Ea

#endif  // NET_EA_ADMINHANDLER_H
