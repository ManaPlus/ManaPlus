/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef RESOURCES_CURSOR_H
#define RESOURCES_CURSOR_H

#include "enums/resources/cursor.h"

#include <string>

#include "localconsts.h"

namespace Cursors
{
    struct StrToCursor final
    {
        A_DEFAULT_COPY(StrToCursor)

        std::string str;
        CursorT cursor;
    };

    CursorT stringToCursor(const std::string &name) A_WARN_UNUSED;
}  // namespace Cursors

#endif  // RESOURCES_CURSOR_H
