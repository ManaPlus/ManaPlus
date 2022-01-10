/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef ENUMS_INPUT_INPUTGROUP_H
#define ENUMS_INPUT_INPUTGROUP_H

#include "localconsts.h"

namespace Input
{
    enum InputGroup
    {
        GRP_DEFAULT = 1,           // default game key
        GRP_CHAT = 2,              // chat key
        GRP_EMOTION = 4,           // emotions key
        GRP_OUTFIT = 8,            // outfit key
        GRP_GUI = 16,              // gui key
        GRP_MOVETOPOINT = 32,      // move to point key
        GRP_GUICHAN = 64,          // for guichan based controls
        GRP_REPEAT = 128,          // repeat emulation keys
        GRP_PET_EMOTION = 256,     // pet emotions key
        GRP_HOMUN_EMOTION = 512    // homunculus/mercenary emotetions key
    };
}  // namespace Input

#endif  // ENUMS_INPUT_INPUTGROUP_H
