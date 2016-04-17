/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2016  The ManaPlus Developers
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

#ifndef NET_TMWA_PETHANDLER_H
#define NET_TMWA_PETHANDLER_H

#include "net/pethandler.h"

namespace TmwAthena
{

class PetHandler final : public Net::PetHandler
{
    public:
        PetHandler();

        A_DELETE_COPY(PetHandler)

        void move(const int petId,
                  const int x, const int y) const final;

        void spawn(const Being *const being,
                   const int petId,
                   const int x, const int y) const final A_CONST;

        void emote(const uint8_t emoteId,
                   const int petId) final;

        void catchPet(const Being *const being) const final A_CONST;

        void sendPetMessage(const int data) const final A_CONST;

        void setName(const std::string &name) const final A_CONST;

        void requestStatus() const final A_CONST;

        void feed() const final A_CONST;

        void dropLoot() const final A_CONST;

        void returnToEgg() const final A_CONST;

        void unequip() const final A_CONST;

        void setDirection(const unsigned char type) const final;

        void startAi(const bool start) const final;

    protected:
        int mRandCounter;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_PETHANDLER_H
