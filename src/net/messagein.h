/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_MESSAGEIN_H
#define NET_MESSAGEIN_H

#include "enums/simpletypes/beingid.h"

#include <string>

#include "localconsts.h"

namespace Net
{

/**
 * Used for parsing an incoming message.
 *
 * \ingroup Network
 */
class MessageIn notfinal
{
    public:
        A_DELETE_COPY(MessageIn)

        virtual ~MessageIn();

        /**
         * Returns the message ID.
         */
        int getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

        /**
         * Returns the message length.
         */
        unsigned int getLength() const noexcept2 A_WARN_UNUSED
        { return mLength; }

        /**
         * Returns the length of unread data.
         */
        unsigned int getUnreadLength() const noexcept2 A_WARN_UNUSED
        { return mLength > mPos ? mLength - mPos : 0; }

        /**< Reads a byte. */
        virtual unsigned char readUInt8(const char *const str);

        /**< Reads a byte. */
        virtual signed char readInt8(const char *const str);

        /**< Reads a short. */
        virtual int16_t readInt16(const char *const str) = 0;

        virtual uint16_t readUInt16(const char *const str) = 0;

        /**< Reads a long. */
        virtual int32_t readInt32(const char *const str) = 0;

        virtual int64_t readInt64(const char *const str) = 0;

        virtual BeingId readBeingId(const char *const str) = 0;

        /**
         * Reads a special 3 byte block used by eAthena, containing x and y
         * coordinates and direction.
         */
        virtual void readCoordinates(uint16_t &restrict x,
                                     uint16_t &restrict y,
                                     uint8_t &restrict direction,
                                     const char *const str);

        /**
         * Reads a special 5 byte block used by eAthena, containing a source
         * and destination coordinate pair.
         */
        virtual void readCoordinatePair(uint16_t &restrict srcX,
                                        uint16_t &restrict srcY,
                                        uint16_t &restrict dstX,
                                        uint16_t &restrict dstY,
                                        const char *const str);

        /**
         * Skips a given number of bytes.
         */
        virtual void skip(const unsigned int length,
                          const char *const str);

        void skipToEnd(const char *const str);

        /**
         * Reads a string. If a length is not given (-1), it is assumed
         * that the length of the string is stored in a short at the
         * start of the string.
         */
        virtual std::string readString(int length,
                                       const char *const dstr);

        virtual std::string readRawString(int length,
                                          const char *const dstr);

        unsigned char *readBytes(int length,
                                 const char *const dstr);

        static uint8_t fromServerDirection(const uint8_t serverDir)
                                           A_WARN_UNUSED;

        unsigned int getVersion() const noexcept2 A_WARN_UNUSED
        { return mVersion; }

    protected:
        /**
         * Constructor.
         */
        MessageIn(const char *const data, const unsigned int length);

        const char *mData;     /**< The message data. */
        unsigned int mLength;  /**< The length of the data. */

        /**
         * Actual position in the packet. From 0 to packet->length.
         * A value bigger than packet->length means EOP was reached when
         * reading it.
         */
        unsigned int mPos;
        unsigned int mVersion;
        uint16_t mId;          /**< The message ID. */
        bool mIgnore;
};

}  // namespace Net

#endif  // NET_MESSAGEIN_H
