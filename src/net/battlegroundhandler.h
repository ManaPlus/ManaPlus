/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef NET_BATTLEGROUNDHANDLER_H
#define NET_BATTLEGROUNDHANDLER_H

#include "enums/net/battlegroundtype.h"

#include <string>

#include "localconsts.h"

namespace Net
{

class BattleGroundHandler notfinal
{
    public:
        BattleGroundHandler()
        { }

        A_DELETE_COPY(BattleGroundHandler)

        virtual ~BattleGroundHandler()
        { }

        virtual void registerBg(const BattleGroundTypeT &type,
                                const std::string &name) const = 0;

        virtual void rekoveRequest(const std::string &name) const = 0;

        virtual void beginAck(const bool result,
                              const std::string &bgName,
                              const std::string &gameName) const = 0;

        virtual void checkState(const std::string &name) const = 0;
};

}  // namespace Net

extern Net::BattleGroundHandler *battleGroundHandler;

#endif  // NET_BATTLEGROUNDHANDLER_H
