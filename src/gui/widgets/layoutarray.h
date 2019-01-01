/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_LAYOUTARRAY_H
#define GUI_WIDGETS_LAYOUTARRAY_H

#include "utils/vector.h"

#include "localconsts.h"

class LayoutCell;
class Widget;

/**
 * This class contains a rectangular array of cells.
 */
class LayoutArray final
{
    friend class LayoutCell;

    public:
        LayoutArray();

        A_DEFAULT_COPY(LayoutArray)

        ~LayoutArray();

        /**
         * Returns a reference on the cell at given position.
         */
        LayoutCell &at(const int x,
                       const int y,
                       const int w,
                       const int h) A_WARN_UNUSED;

        /**
         * Places a widget in a given cell.
         * @param w number of columns the widget spawns.
         * @param h number of rows the widget spawns.
         * @note When @a w is 1, the width of column @a x is reset to zero if
         *       it was AUTO_DEF. Similarly for @a h.
         */
        LayoutCell &place(Widget *const widget,
                          const int x,
                          const int y,
                          const int w,
                          const int h);

        /**
         * Sets the minimum width of a column.
         */
        void setColWidth(const int n, const int w);

        /**
         * Sets the minimum height of a row.
         */
        void setRowHeight(const int n, const int h);

        /**
         * Sets the widths of two columns to the maximum of their widths.
         */
        void matchColWidth(const int n1, const int n2);

        /**
         * Spawns a cell over several columns/rows.
         */
        void extend(const int x, const int y, const int w, const int h);

        /**
         * Computes and sets the positions of all the widgets.
         * @param nW width of the array, used to resize the AUTO_ columns.
         * @param nH height of the array, used to resize the AUTO_ rows.
         */
        void reflow(const int nX, const int nY, const int nW, const int nH);

    private:
        // Copy not allowed, as the array owns all its cells.
        explicit LayoutArray(LayoutArray const &);
        LayoutArray &operator=(LayoutArray const &);

        /**
         * Gets the position and size of a widget along a given axis
         */
        void align(int &restrict pos,
                   int &restrict size,
                   const int dim,
                   LayoutCell const &restrict cell,
                   const int *restrict const sizes,
                   const int sizeCount) const A_NONNULL(6);

        /**
         * Ensures the private vectors are large enough.
         */
        void resizeGrid(int w, const int h);

        /**
         * Gets the column/row sizes along a given axis.
         * @param upp target size for the array. Ignored if AUTO_DEF.
         */
        STD_VECTOR<int> getSizes(const int dim, int upp) const A_WARN_UNUSED;

        /**
         * Gets the total size along a given axis.
         */
        int getSize(const int dim) const A_WARN_UNUSED;

        STD_VECTOR<int> mSizes[2];
        STD_VECTOR< STD_VECTOR < LayoutCell * > > mCells;

        int mSpacing;
};

#endif  // GUI_WIDGETS_LAYOUTARRAY_H
