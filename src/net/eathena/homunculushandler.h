/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef NET_EATHENA_HOMUNCULUSHANDLER_H
#define NET_EATHENA_HOMUNCULUSHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/homunculushandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class HomunculusHandler final : public MessageHandler,
                                public Net::HomunculusHandler
{
    public:
        HomunculusHandler();

        A_DELETE_COPY(HomunculusHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void setName(const std::string &name) const override final;

        void moveToMaster() const override final;

        void move(const int x, const int y) const override final;

        void attack(const int targetId, const Keep keep) const override final;

        void feed() const override final;

        void fire() const override final;

        void talk(const std::string &restrict text) const override final;

        void emote(const uint8_t emoteId) const override final;

        void setDirection(const unsigned char type) const override final;

    protected:
        static void processHomunculusSkills(Net::MessageIn &msg);

        static void processHomunculusData(Net::MessageIn &msg);

        static void processHomunculusInfo(Net::MessageIn &msg);

        static void processHomunculusSkillUp(Net::MessageIn &msg);

        static void processHomunculusFood(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_EATHENA_HOMUNCULUSHANDLER_H
