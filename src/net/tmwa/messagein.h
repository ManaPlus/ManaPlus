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

#ifndef NET_TMWA_MESSAGEIN_H
#define NET_TMWA_MESSAGEIN_H

#include "net/messagein.h"

#include "localconsts.h"

namespace TmwAthena
{

/**
 * Used for parsing an incoming message.
 *
 * \ingroup Network
 */
class MessageIn final : public Net::MessageIn
{
    public:
        /**
         * Constructor.
         */
        MessageIn(const char *const data, const unsigned int length);

        A_DELETE_COPY(MessageIn)

        void postInit();

        int16_t readInt16();            /**< Reads a short. */
        int32_t readInt32();            /**< Reads a long. */
};

}  // namespace TmwAthena

#endif  // NET_TMWA_MESSAGEIN_H
