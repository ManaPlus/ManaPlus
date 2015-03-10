/*
 *  The ManaPlus Client
 *  Copyright (C) 2015  The ManaPlus Developers
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

namespace EAthena
{
    struct MapTypeProperty2Bits final
    {
        unsigned int party             : 1;  // allow attack party members (PvP)
        unsigned int guild             : 1;  // allow attack guild members (GvG)
        unsigned int siege             : 1;  // show emblem in GvG (WoE castle)
        unsigned int mineffect         : 1;  // mine effect?
        unsigned int nolockon          : 1;  // unknown
        unsigned int countpk           : 1;  // show PvP counter
        unsigned int nopartyformation  : 1;  // prevent party creation/modification
        unsigned int bg                : 1;  // is on battle ground
        unsigned int noitemconsumption : 1;  // unused
        unsigned int usecart           : 1;  // unused
        unsigned int summonstarmiracle : 1;  // unused
        unsigned int SpareBits         : 15; // unused bits
    }  __attribute__((packed));

    union MapTypeProperty2 final
    {
        MapTypeProperty2Bits bits;
        uint32_t data;
    } __attribute__((packed));

}  // namespace EAthena

#endif  // NET_EATHENA_MAPTYPEPROPERTY2_H
