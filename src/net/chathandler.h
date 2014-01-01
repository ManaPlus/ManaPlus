/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_CHATHANDLER_H
#define NET_CHATHANDLER_H

#include <iosfwd>
#include <string>

#include "localconsts.h"

namespace Net
{
class ChatHandler
{
    public:
        virtual ~ChatHandler()
        { }

        virtual void talk(const std::string &restrict text,
                          const std::string &restrict channel) const = 0;

        virtual void talkPet(const std::string &restrict text,
                             const std::string &restrict channel) const = 0;

        virtual void talkRaw(const std::string &text) const = 0;

        virtual void me(const std::string &restrict text,
                        const std::string &restrict channel) const = 0;

        virtual void privateMessage(const std::string &restrict recipient,
                                    const std::string &restrict text) = 0;

        virtual void who() const = 0;

        virtual void sendRaw(const std::string &args) const = 0;

        virtual void ignoreAll() const = 0;

        virtual void unIgnoreAll() const = 0;

        virtual void clear() = 0;
};

}  // namespace Net

#endif  // NET_CHATHANDLER_H
