/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2018  The ManaPlus Developers
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

#ifndef NET_EATHENA_PETHANDLER_H
#define NET_EATHENA_PETHANDLER_H

#include "net/pethandler.h"

namespace EAthena
{

class PetHandler final :  public Net::PetHandler
{
    public:
        PetHandler();

        A_DELETE_COPY(PetHandler)

        ~PetHandler() override final;

        void move(const int x,
                  const int y) const override final;

        void emote(const uint8_t emoteId) override final;

        void catchPet(const Being *const being) const override final;

        void sendPetMessage(const int data) const override final;

        void setName(const std::string &name) const override final;

        void requestStatus() const override final;

        void feed() const override final;

        void dropLoot() const override final;

        void returnToEgg() const override final;

        void unequip() const override final;

        void setDirection(const unsigned char type) const override final;

        void evolution(const Item *const item) const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_PETHANDLER_H
