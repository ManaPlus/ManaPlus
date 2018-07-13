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

#ifndef NET_PETHANDLER_H
#define NET_PETHANDLER_H

#include "being/being.h"

class Item;

namespace Net
{

class PetHandler notfinal
{
    public:
        PetHandler()
        { }

        A_DELETE_COPY(PetHandler)

        virtual ~PetHandler()
        { }

        virtual void move(const int x,
                          const int y) const = 0;

        virtual void emote(const uint8_t emoteId) = 0;

        virtual void catchPet(const Being *const being) const = 0;

        virtual void sendPetMessage(const int data) const = 0;

        virtual void setName(const std::string &name) const = 0;

        virtual void requestStatus() const = 0;

        virtual void feed() const = 0;

        virtual void dropLoot() const = 0;

        virtual void returnToEgg() const = 0;

        virtual void unequip() const = 0;

        virtual void setDirection(const unsigned char type) const = 0;

        virtual void evolution(const Item *item) const = 0;
};

}  // namespace Net

extern Net::PetHandler *petHandler;

#endif  // NET_PETHANDLER_H
