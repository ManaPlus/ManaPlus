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

#ifndef NET_EATHENA_MESSAGEOUT_H
#define NET_EATHENA_MESSAGEOUT_H

#include "net/messageout.h"

#include "localconsts.h"

namespace EAthena
{

class Network;

/**
 * Used for building an outgoing message.
 *
 * \ingroup Network
 */
class MessageOut final : public Net::MessageOut
{
    public:
        /**
         * Constructor.
         */
        explicit MessageOut(const int16_t id);

        A_DELETE_COPY(MessageOut)

        void writeInt16(const int16_t value);        /**< Writes a short. */

        void writeInt32(const int32_t value);        /**< Writes a long. */

        /**
         * Encodes coordinates and direction in 3 bytes.
         */
        void writeCoordinates(const uint16_t x,
                              const uint16_t y,
                              unsigned char direction);

        void resetPos()
        { mPos = 0; }

    private:
        void expand(const size_t size);

        Network *mNetwork;
};

}  // namespace EAthena

#endif  // NET_EATHENA_MESSAGEOUT_H
