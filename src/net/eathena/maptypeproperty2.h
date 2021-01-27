/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2019  The ManaPlus Developers
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

#ifndef NET_EATHENA_MAPTYPEPROPERTY2_H
#define NET_EATHENA_MAPTYPEPROPERTY2_H

#include "localconsts.h"

namespace EAthena
{
    struct MapTypeProperty2Bits final
    {
        A_DEFAULT_COPY(MapTypeProperty2Bits)

        uint32_t party             : 1;   // allow attack party members (PvP)
        uint32_t guild             : 1;   // allow attack guild members (GvG)
        uint32_t siege             : 1;   // show emblem in GvG (WoE castle)
        uint32_t mineffect         : 1;   // mine effect?
        uint32_t nolockon          : 1;   // unknown
        uint32_t countpk           : 1;   // show PvP counter
        uint32_t nopartyformation  : 1;   // prevent party creation/
                                          // modification
        uint32_t bg                : 1;   // is on battle ground
        uint32_t noitemconsumption : 1;   // unused
        uint32_t usecart           : 1;   // unused
        uint32_t summonstarmiracle : 1;   // unused
        uint32_t SpareBits         : 15;  // unused bits
    }  __attribute__((packed));

    union MapTypeProperty2 final
    {
        MapTypeProperty2Bits bits;
        uint32_t data;
    } __attribute__((packed));

}  // namespace EAthena

#endif  // NET_EATHENA_MAPTYPEPROPERTY2_H
