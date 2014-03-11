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

#ifndef NET_SERVERINFO_H
#define NET_SERVERINFO_H

#include "utils/stringutils.h"

#include <SDL_stdinc.h>

#include <string>
#include <vector>

#include "localconsts.h"

class ServerInfo final
{
public:
    enum Type
    {
        UNKNOWN = 0,
        TMWATHENA,
        EVOL,
        EATHENA
    };

    typedef std::pair<int, std::string> VersionString;

    Type type;
    std::string name;
    std::string hostname;
    std::string althostname;
    std::string description;
    std::string registerUrl;
    std::string onlineListUrl;
    std::string supportUrl;
    uint16_t port;
    VersionString version;

    bool save;

    ServerInfo() :
        type(TMWATHENA),
        name(),
        hostname(),
        althostname(),
        description(),
        registerUrl(),
        onlineListUrl(),
        supportUrl(),
        port(6901),
        version(),
        save(false)
    {
        version.first = 0;
    }

    ServerInfo(const ServerInfo &info) :
        type(info.type),
        name(info.name),
        hostname(info.hostname),
        althostname(info.althostname),
        description(info.description),
        registerUrl(info.registerUrl),
        onlineListUrl(info.onlineListUrl),
        supportUrl(info.supportUrl),
        port(info.port),
        version(),
        save(info.save)
    {
        version.first = info.version.first;
        version.second = info.version.second;
    }

    bool isValid() const A_WARN_UNUSED
    {
        return !(hostname.empty() || port == 0 || type == UNKNOWN);
    }

    void clear()
    {
        type = UNKNOWN;
        name.clear();
        hostname.clear();
        althostname.clear();
        port = 0;
        description.clear();
        registerUrl.clear();
        onlineListUrl.clear();
        supportUrl.clear();
        version.first = 0;
        version.second.clear();
        save = false;
    }

    bool operator==(const ServerInfo &other) const
    {
        return (type == other.type && hostname == other.hostname &&
                port == other.port);
    }

    bool operator!=(const ServerInfo &other) const
    {
        return (type != other.type || hostname != other.hostname ||
                port != other.port);
    }

    static Type parseType(const std::string &serverType) A_WARN_UNUSED
    {
        if (compareStrI(serverType, "tmwathena") == 0)
            return TMWATHENA;
        if (compareStrI(serverType, "evol") == 0)
            return EVOL;
#ifdef EATHENA_SUPPORT
        else if (compareStrI(serverType, "eathena") == 0)
            return EATHENA;
#else
        else if (compareStrI(serverType, "eathena") == 0)
            return TMWATHENA;
#endif
        return UNKNOWN;
    }
};

typedef std::vector<ServerInfo> ServerInfos;

#endif  // NET_SERVERINFO_H
