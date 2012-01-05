/*
 *  The ManaPlus Client
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

#ifndef AUCTIONMANAGER_H
#define AUCTIONMANAGER_H

#include "utils/dtor.h"

#include <map>
#include <string>
#include <vector>

class AuctionManager
{
    public:
        AuctionManager();

        ~AuctionManager();

        static void init();

        void send(std::string msg);

        bool processAuctionMessage(std::string msg);

        static bool getEnableAuctionBot()
        { return mEnableAuctionBot; }

        void clear();

        void reload();

        void sendMail(std::string mail);

    private:
        static bool mEnableAuctionBot;
};

extern AuctionManager *auctionManager;

#endif // AUCTIONMANAGER_H
