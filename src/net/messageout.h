/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_MESSAGEOUT_H
#define NET_MESSAGEOUT_H

#include "enums/simpletypes/beingid.h"

#include "utils/cast.h"

#include <string>

#include "localconsts.h"

UTILS_CAST_H

#define createOutPacket(name) MessageOut outMsg(name); \
    outMsg.writeInt16(CAST_S16(name), #name)

namespace Net
{

/**
 * Used for building an outgoing message.
 *
 * \ingroup Network
 */
class MessageOut notfinal
{
    public:
        A_DELETE_COPY(MessageOut)

        /**< Writes a byte. */
        virtual void writeInt8(const int8_t value,
                               const char *const str);

        /**< Writes a short. */
        virtual void writeInt16(const int16_t value,
                                const char *const str) = 0;

        /**< Writes a long. */
        virtual void writeInt32(const int32_t value,
                                const char *const str) = 0;

        virtual void writeBeingId(const BeingId value,
                                  const char *const str) = 0;

        /**
         * Writes a string. If a fixed length is not given (-1), it is stored
         * as a short at the start of the string.
         */
        virtual void writeString(const std::string &string,
                                 int length,
                                 const char *const str);

        /**
         * Writes a string. If a fixed length is not given (-1), it is stored
         * as a short at the start of the string.
         */
        virtual void writeStringNoLog(const std::string &string,
                                      int length,
                                      const char *const str);

        /**
         * Returns the content of the message.
         */
        virtual const char *getData() const A_WARN_UNUSED;

        /**
         * Returns the length of the data.
         */
        virtual unsigned int getDataSize() const A_WARN_UNUSED;

        virtual ~MessageOut()
        { }

        static unsigned char toServerDirection(unsigned char direction)
                                               A_CONST;

    protected:
        /**
         * Constructor.
         */
        explicit MessageOut(const int16_t id);

        /**
         * Expand the packet data to be able to hold more data.
         *
         * NOTE: For performance enhancements this method could allocate extra
         * memory in advance instead of expanding size every time more data is
         * added.
         */
        virtual void expand(size_t size) const = 0;

        char *mData;                         /**< Data building up. */
        unsigned int mDataSize;              /**< Size of data. */
        unsigned int mPos;                   /**< Position in the data. */
        uint16_t mId;
        bool mIgnore;
};

}  // namespace Net

#endif  // NET_MESSAGEOUT_H
