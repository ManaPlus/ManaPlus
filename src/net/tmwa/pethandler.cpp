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

#include "net/tmwa/pethandler.h"

#include "const/gui/chat.h"

#include "net/chathandler.h"

#include "utils/stringutils.h"
#include "utils/timer.h"

#include "debug.h"

extern Net::PetHandler *petHandler;

namespace TmwAthena
{

PetHandler::PetHandler() :
    mRandCounter(1000)
{
    petHandler = this;
}

void PetHandler::move(const int petId A_UNUSED,
                      const int x, const int y) const
{
    chatHandler->talk(strprintf("\302\202\302m%d,%dg%d",
        x, y, tick_time), GENERAL_CHANNEL);
}

void PetHandler::spawn(const Being *const being A_UNUSED,
                       const int petId A_UNUSED,
                       const int x A_UNUSED, const int y A_UNUSED) const
{
}

void PetHandler::emote(const uint8_t emoteId, const int petId A_UNUSED)
{
    mRandCounter ++;
    if (mRandCounter > 10000)
        mRandCounter = 1000;

    chatHandler->talk(strprintf("\302\202\302e%dz%d",
        CAST_S32(emoteId), mRandCounter), GENERAL_CHANNEL);
}

void PetHandler::catchPet(const Being *const being A_UNUSED) const
{
}

void PetHandler::sendPetMessage(const int data A_UNUSED) const
{
}

void PetHandler::setName(const std::string &name A_UNUSED) const
{
}

void PetHandler::requestStatus() const
{
}

void PetHandler::feed() const
{
}

void PetHandler::dropLoot() const
{
}

void PetHandler::returnToEgg() const
{
}

void PetHandler::unequip() const
{
}

void PetHandler::setDirection(const unsigned char type) const
{
    chatHandler->talk(strprintf("\302\202\302d%dg%d",
        CAST_S32(type), tick_time), GENERAL_CHANNEL);
}

void PetHandler::startAi(const bool start) const
{
    chatHandler->talk(strprintf("\302\202\302a%dg%d",
        start ? 1 : 0, tick_time), GENERAL_CHANNEL);
}

}  // namespace TmwAthena
