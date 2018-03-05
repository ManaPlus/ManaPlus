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

#include "net/eathena/messagein.h"

#include "logger.h"

#include "net/net.h"

#include "utils/stringutils.h"

#include "debug.h"

namespace EAthena
{

MessageIn::MessageIn(const char *const data,
                     const unsigned int length) :
    Net::MessageIn(data, length)
{
}

void MessageIn::postInit(const char *const str,
                         const unsigned int version)
{
    // Read the message ID
    mId = readId();
    mVersion = version;
    IGNOREDEBUGLOG;
    DEBUGLOG2("Receive packet", 0, "MessageIn");
#ifdef ENABLEDEBUGLOG
    if (mVersion > 0)
    {
        const std::string verStr = toString(mVersion);
        DEBUGLOG2("Version", 0, verStr.c_str());
    }
#endif  // ENABLEDEBUGLOG

    readInt16(str);
}

}  // namespace EAthena
