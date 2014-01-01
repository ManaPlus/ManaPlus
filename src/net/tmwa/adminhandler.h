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

#ifndef NET_TMWA_ADMINHANDLER_H
#define NET_TMWA_ADMINHANDLER_H

#include "net/ea/adminhandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class AdminHandler final : public MessageHandler, public Ea::AdminHandler
{
    public:
        AdminHandler();

        A_DELETE_COPY(AdminHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void announce(const std::string &text) const override final;

        void localAnnounce(const std::string &text) const override final;

        void hide(const bool h) const override final;

        void kick(const int playerId) const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_ADMINHANDLER_H
