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

#ifndef NET_TMWA_SP_H
#define NET_TMWA_SP_H

namespace Sp
{
    enum
    {
        SPEED        = 0,
        BASEEXP      = 1,
        JOBEXP       = 2,
        KARMA        = 3,
        MANNER       = 4,
        HP           = 5,
        MAXHP        = 6,
        SP           = 7,
        MAXSP        = 8,
        STATUSPOINT  = 9,
        BASELEVEL    = 11,
        SKILLPOINT   = 12,
        STR          = 13,
        AGI          = 14,
        VIT          = 15,
        INT          = 16,
        DEX          = 17,
        LUK          = 18,
        CLASS        = 19,
        ZENY         = 20,
        SEX          = 21,
        NEXTBASEEXP  = 22,
        JOB_MOD      = 23,  // SP_NEXTJOBEXP
        WEIGHT       = 24,
        MAXWEIGHT    = 25,
        USTR         = 32,
        UAGI         = 33,
        UVIT         = 34,
        UINT         = 35,
        UDEX         = 36,
        ULUK         = 37,
        ATK1         = 41,
        ATK2         = 42,
        MATK1        = 43,
        MATK2        = 44,
        DEF1         = 45,
        DEF2         = 46,
        MDEF1        = 47,
        MDEF2        = 48,
        HIT          = 49,
        FLEE1        = 50,
        FLEE2        = 51,
        CRITICAL     = 52,
        ASPD         = 53,
        JOBLEVEL     = 55,

        GM_LEVEL     = 500
    };
}  // namespace Sp

#endif  // NET_TMWA_SP_H
