/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_HOSTSGROUP_H
#define NET_HOSTSGROUP_H

#include "utils/stringvector.h"

#include "localconsts.h"

struct HostsGroup final
{
    HostsGroup() :
        name(),
        hosts()
    {
    }

    A_DEFAULT_COPY(HostsGroup)

    std::string name;
    StringVect hosts;
};

#endif  // NET_HOSTSGROUP_H
