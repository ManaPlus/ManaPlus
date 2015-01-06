/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "enums/net/servertype.h"

#include "utils/stringutils.h"

#include <SDL_stdinc.h>

#include <string>
#include <vector>

#include "localconsts.h"

class ServerInfo final
{
    public:
        typedef std::pair<int, std::string> VersionString;

        ServerType::Type type;
        std::string name;
        std::string hostname;
        std::string althostname;
        std::string description;
        std::string registerUrl;
        std::string onlineListUrl;
        std::string supportUrl;
        std::vector<std::string> updateMirrors;
        uint16_t port;
        VersionString version;
        bool save;
        bool persistentIp;

        ServerInfo() :
            type(ServerType::TMWATHENA),
            name(),
            hostname(),
            althostname(),
            description(),
            registerUrl(),
            onlineListUrl(),
            supportUrl(),
            updateMirrors(),
#ifdef TMWA_SUPPORT
            port(6901),
#else
            port(6900),
#endif
            version(),
            save(false),
            persistentIp(true)
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
            updateMirrors(info.updateMirrors),
            port(info.port),
            version(),
            save(info.save),
            persistentIp(info.persistentIp)
        {
            version.first = info.version.first;
            version.second = info.version.second;
        }

        bool isValid() const A_WARN_UNUSED
        {
            return !(hostname.empty()
                || port == 0
                || type == ServerType::UNKNOWN);
        }

        void clear()
        {
            type = ServerType::UNKNOWN;
            name.clear();
            hostname.clear();
            althostname.clear();
            port = 0;
            description.clear();
            registerUrl.clear();
            onlineListUrl.clear();
            supportUrl.clear();
            updateMirrors.clear();
            version.first = 0;
            version.second.clear();
            save = false;
            persistentIp = true;
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

        static ServerType::Type parseType(const std::string &serverType)
                                          A_WARN_UNUSED
        {
#ifdef TMWA_SUPPORT
            if (compareStrI(serverType, "tmwathena") == 0)
                return ServerType::TMWATHENA;
            if (compareStrI(serverType, "evol") == 0)
                return ServerType::EVOL;
#else
            if (compareStrI(serverType, "tmwathena") == 0
                || compareStrI(serverType, "evol") == 0)
                return ServerType::EATHENA;
#endif
#ifdef EATHENA_SUPPORT
            else if (compareStrI(serverType, "eathena") == 0)
                return ServerType::EATHENA;
            else if (compareStrI(serverType, "evol2") == 0)
                return ServerType::EVOL2;
#else
            else if (compareStrI(serverType, "eathena") == 0)
                return ServerType::TMWATHENA;
            else if (compareStrI(serverType, "evol2") == 0)
                return ServerType::TMWATHENA;
#endif
            return ServerType::UNKNOWN;
        }
};

typedef std::vector<ServerInfo> ServerInfos;

#endif  // NET_SERVERINFO_H
