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

#ifndef ENUMS_NET_DELETEITEMREASON_H
#define ENUMS_NET_DELETEITEMREASON_H

#include "enums/simpletypes/enumdefines.h"

enumStart(DeleteItemReason)
{
    Normal         = 0,
    SkillUse       = 1,
    FailRefine     = 2,
    MaterialChange = 3,
    ToStorage      = 4,
    ToCart         = 5,
    Sold           = 6,
    Analysis       = 7
}
enumEnd(DeleteItemReason);

#endif  // ENUMS_NET_DELETEITEMREASON_H
