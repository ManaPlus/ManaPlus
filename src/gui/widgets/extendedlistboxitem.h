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

#ifndef GUI_WIDGETS_EXTENDEDLISTBOXITEM_H
#define GUI_WIDGETS_EXTENDEDLISTBOXITEM_H

#include <string>

#include "localconsts.h"

struct ExtendedListBoxItem final
{
    ExtendedListBoxItem(const int row0,
                        const std::string &text0,
                        const bool image0,
                        const int y0) :
        row(row0),
        text(text0),
        image(image0),
        y(y0)
    {
    }

    A_DEFAULT_COPY(ExtendedListBoxItem)

    int row;
    std::string text;
    bool image;
    int y;
};

#endif  // GUI_WIDGETS_EXTENDEDLISTBOXITEM_H
