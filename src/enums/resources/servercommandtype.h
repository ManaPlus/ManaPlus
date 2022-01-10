/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_SERVERCOMMANDTYPE_H
#define ENUMS_RESOURCES_SERVERCOMMANDTYPE_H

#include "enums/simpletypes/enumdefines.h"

#define servercommandFirst(name) name = 0,
#define servercommand(name) name,
#define servercommand2(name1, name2) name1,

enumStart(ServerCommandType)
{
#include "resources/servercommands.inc"
    Max
}
enumEnd(ServerCommandType);

#undef servercommandFirst
#undef servercommand
#undef servercommand2

SERVERCOMMANDS_VOID
#undef SERVERCOMMANDS_VOID

#endif  // ENUMS_RESOURCES_SERVERCOMMANDTYPE_H
