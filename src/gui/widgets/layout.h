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

#ifndef GUI_WIDGETS_LAYOUT_H
#define GUI_WIDGETS_LAYOUT_H

#include "gui/widgets/layoutcell.h"

#include "localconsts.h"

/**
 * This class is an helper for setting the position of widgets. They are
 * positioned along the cells of some rectangular tables. The layout may either
 * be a single table or a tree of nested tables.
 *
 * The size of a given table column can either be set manually or be chosen
 * from the widest widget of the column. An empty column has a AUTO_DEF width,
 * which means it will be extended so that the layout fits its minimum width.
 *
 * The process is similar for table rows. By default, there is a spacing of 4
 * pixels between rows and between columns, and a margin of 6 pixels around the
 * whole layout.
 */
class Layout final : public LayoutCell
{
    public:
        Layout();

        A_DELETE_COPY(Layout)

        /**
         * Sets the margin around the layout.
         */
        void setMargin(int m)
        { setPadding(m); }

        /**
         * Sets the positions of all the widgets.
         * @see LayoutArray::reflow
         */
        void reflow(int &restrict nW, int &restrict nH);

    private:
        bool mComputed;
};

#endif  // GUI_WIDGETS_LAYOUT_H
