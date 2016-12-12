/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/hostsgroup.h"

#include "utils/stringutils.h"

#include "localconsts.h"

class ServerInfo final
{
    public:
        typedef std::pair<int, std::string> VersionString;

        ServerTypeT type;
        std::string name;
        std::string hostname;
        std::string althostname;
        std::string description;
        std::string registerUrl;
        std::string onlineListUrl;
        std::string supportUrl;
        std::string defaultHostName;
        std::vector<std::string> updateMirrors;
        std::vector<HostsGroup> updateHosts;
        uint16_t port;
        VersionString version;
        int packetVersion;
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
            defaultHostName(),
            updateMirrors(),
            updateHosts(),
#ifdef TMWA_SUPPORT
            port(6901),
#else  // TMWA_SUPPORT
            port(6900),
#endif  // TMWA_SUPPORT
            version(),
            packetVersion(),
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
            defaultHostName(info.defaultHostName),
            updateMirrors(info.updateMirrors),
            updateHosts(info.updateHosts),
            port(info.port),
            version(),
            packetVersion(info.packetVersion),
            save(info.save),
            persistentIp(info.persistentIp)
        {
            version.first = info.version.first;
            version.second = info.version.second;
        }

        ServerInfo &operator=(const ServerInfo &info)
        {
            type = info.type;
            name = info.name;
            hostname = info.hostname;
            althostname = info.althostname;
            description = info.description;
            registerUrl = info.registerUrl;
            onlineListUrl = info.onlineListUrl;
            supportUrl = info.supportUrl;
            defaultHostName = info.defaultHostName;
            updateMirrors = info.updateMirrors;
            updateHosts = info.updateHosts;
            port = info.port;
            save = info.save;
            persistentIp = info.persistentIp;
            version.first = info.version.first;
            version.second = info.version.second;
            packetVersion = info.packetVersion;
            return *this;
        }

        bool isValid() const noexcept2 A_WARN_UNUSED
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
            defaultHostName.clear();
            updateMirrors.clear();
            updateHosts.clear();
            version.first = 0;
            version.second.clear();
            packetVersion = 0;
            save = false;
            persistentIp = true;
        }

        bool operator==(const ServerInfo &other) const
        {
            return (name == other.name && hostname == other.hostname &&
                    port == other.port);
        }

        bool operator!=(const ServerInfo &other) const
        {
            return (name != other.name || hostname != other.hostname ||
                    port != other.port);
        }

        static ServerTypeT parseType(const std::string &serverType)
                                     A_WARN_UNUSED
        {
#ifdef TMWA_SUPPORT
            if (compareStrI(serverType, "tmwathena") == 0)
                return ServerType::TMWATHENA;
            else
#endif  // TMWA_SUPPORT
            if (compareStrI(serverType, "eathena") == 0)
                return ServerType::EATHENA;
            else if (compareStrI(serverType, "hercules") == 0)
                return ServerType::EATHENA;
            else if (compareStrI(serverType, "evol2") == 0)
                return ServerType::EVOL2;
            return ServerType::UNKNOWN;
        }
};

typedef std::vector<ServerInfo> ServerInfos;

#endif  // NET_SERVERINFO_H
