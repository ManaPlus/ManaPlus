/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/mercenaryhandler.h"

#include "debug.h"

extern Net::MercenaryHandler *mercenaryHandler;

namespace TmwAthena
{

MercenaryHandler::MercenaryHandler()
{
    mercenaryHandler = this;
}

void MercenaryHandler::fire() const
{
}

void MercenaryHandler::moveToMaster() const
{
}

void MercenaryHandler::move(const int x A_UNUSED, const int y A_UNUSED) const
{
}

void MercenaryHandler::attack(const BeingId targetId A_UNUSED,
                              const Keep keep A_UNUSED) const
{
}

void MercenaryHandler::talk(const std::string &restrict text A_UNUSED) const
{
}

void MercenaryHandler::emote(const uint8_t emoteId A_UNUSED) const
{
}

void MercenaryHandler::setDirection(const unsigned char type A_UNUSED) const
{
}

}  // namespace TmwAthena
