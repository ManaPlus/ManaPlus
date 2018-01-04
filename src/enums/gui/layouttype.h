/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef ENUMS_GUI_WIDGETS_LAYOUTTYPE_H
#define ENUMS_GUI_WIDGETS_LAYOUTTYPE_H

#include "localconsts.h"

namespace LayoutType
{
    /**
     * When the minimum size of the layout is less than the available size,
     * the remaining pixels are equally split amongst the FILL items.
     */
    enum Type
    {
        DEF = -42, /**< Default value, behaves like AUTO_ADD. */
        SET = -43, /**< Uses the share as the new size. */
        ADD = -44  /**< Adds the share to the current size. */
    };
}  // namespace LayoutType

#endif  // ENUMS_GUI_WIDGETS_LAYOUTTYPE_H
