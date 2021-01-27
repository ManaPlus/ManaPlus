/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_CONTAINERPLACER_H
#define GUI_WIDGETS_CONTAINERPLACER_H

#include "localconsts.h"

class BasicContainer2;
class LayoutCell;
class Widget;

/**
 * This class is a helper for adding widgets to nested tables in a window.
 */
class ContainerPlacer final
{
    public:
        ContainerPlacer(BasicContainer2 *const c,
                        LayoutCell *const lc) :
            mContainer(c),
            mCell(lc)
        {}

        A_DEFAULT_COPY(ContainerPlacer)

        /**
         * Gets the pointed cell.
         */
        LayoutCell &getCell() A_WARN_UNUSED
        { return *mCell; }

        /**
         * Returns a placer for the same container but to an inner cell.
         */
        ContainerPlacer at(const int x,
                           const int y) A_WARN_UNUSED;

        /**
         * Adds the given widget to the container and places it in the layout.
         * @see LayoutArray::place
         */
        LayoutCell &operator()(const int x,
                               const int y,
                               Widget *const wg,
                               const int w,
                               const int h);

    private:
        BasicContainer2 *mContainer;
        LayoutCell *mCell;
};

#endif  // GUI_WIDGETS_CONTAINERPLACER_H
