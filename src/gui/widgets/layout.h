/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "localconsts.h"

#include <vector>

class BasicContainer2;
class LayoutCell;
class Widget;

/**
 * This class is a helper for adding widgets to nested tables in a window.
 */
class ContainerPlacer final
{
    public:
        explicit ContainerPlacer(BasicContainer2 *c = nullptr,
                                 LayoutCell *lc = nullptr) :
            mContainer(c), mCell(lc)
        {}

        /**
         * Gets the pointed cell.
         */
        LayoutCell &getCell() A_WARN_UNUSED
        { return *mCell; }

        /**
         * Returns a placer for the same container but to an inner cell.
         */
        ContainerPlacer at(const int x, const int y) A_WARN_UNUSED;

        /**
         * Adds the given widget to the container and places it in the layout.
         * @see LayoutArray::place
         */
        LayoutCell &operator()(const int x, const int y, Widget *const wg,
            const int w = 1, const int h = 1);

    private:
        BasicContainer2 *mContainer;
        LayoutCell *mCell;
};

/**
 * This class contains a rectangular array of cells.
 */
class LayoutArray final
{
    friend class LayoutCell;

    public:
        LayoutArray();

        ~LayoutArray();

        /**
         * Returns a reference on the cell at given position.
         */
        LayoutCell &at(const int x, const int y,
                       const int w = 1, const int h = 1) A_WARN_UNUSED;

        /**
         * Places a widget in a given cell.
         * @param w number of columns the widget spawns.
         * @param h number of rows the widget spawns.
         * @note When @a w is 1, the width of column @a x is reset to zero if
         *       it was AUTO_DEF. Similarly for @a h.
         */
        LayoutCell &place(Widget *const widget, const int x, const int y,
                          const int w = 1, const int h = 1);

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
        void align(int &restrict pos, int &restrict size, const int dim,
                   LayoutCell const &restrict cell,
                   const int *restrict const sizes, const int sizeCount) const;

        /**
         * Ensures the private vectors are large enough.
         */
        void resizeGrid(int w, const int h);

        /**
         * Gets the column/row sizes along a given axis.
         * @param upp target size for the array. Ignored if AUTO_DEF.
         */
        std::vector<int> getSizes(const int dim, int upp) const A_WARN_UNUSED;

        /**
         * Gets the total size along a given axis.
         */
        int getSize(const int dim) const A_WARN_UNUSED;

        std::vector<int> mSizes[2];
        std::vector< std::vector < LayoutCell * > > mCells;

        int mSpacing;
};

/**
 * This class describes the formatting of a widget in the cell of a layout
 * table. Horizontally, a widget can either fill the width of the cell (minus
 * the cell padding), or it can retain its size and be flushed left, or flush
 * right, or centered in the cell. The process is similar for the vertical
 * alignment, except that top is represented by LEFT and bottom by RIGHT.
 */
class LayoutCell
{
    friend class Layout;
    friend class LayoutArray;

    public:
        enum Alignment
        {
            LEFT = 0,
            RIGHT,
            CENTER,
            FILL
        };

        virtual ~LayoutCell();

        /**
         * Sets the padding around the cell content.
         */
        LayoutCell &setPadding(int p)
        { mHPadding = p; mVPadding = p; return *this; }

        /**
         * Sets the vertical padding around the cell content.
         */
        LayoutCell &setVPadding(int p)
        { mVPadding = p; return *this; }

        /**
         * Sets the horisontal padding around the cell content.
         */
        LayoutCell &setHPadding(int p)
        { mHPadding = p; return *this; }

        /**
         * Sets the horizontal alignment of the cell content.
         */
        LayoutCell &setHAlign(Alignment a)
        { mAlign[0] = a; return *this; }

        /**
         * Sets the vertical alignment of the cell content.
         */
        LayoutCell &setVAlign(Alignment a)
        { mAlign[1] = a; return *this; }

        /**
         * @see LayoutArray::at
         */
        LayoutCell &at(int x, int y) A_WARN_UNUSED
        { return getArray().at(x, y); }

        /**
         * @see LayoutArray::place
         */
        LayoutCell &place(Widget *wg, int x, int y, int w = 1, int h = 1)
        { return getArray().place(wg, x, y, w, h); }

        /**
         * @see LayoutArray::matchColWidth
         */
        void matchColWidth(int n1, int n2)
        { getArray().matchColWidth(n1, n2); }

        /**
         * @see LayoutArray::setColWidth
         */
        void setColWidth(int n, int w)
        { getArray().setColWidth(n, w); }

        /**
         * @see LayoutArray::setRowHeight
         */
        void setRowHeight(int n, int h)
        { getArray().setRowHeight(n, h); }

        /**
         * @see LayoutArray::extend.
         */
        void extend(int x, int y, int w, int h)
        { getArray().extend(x, y, w, h); }

        /**
         * Sets the minimum widths and heights of this cell and of all the
         * inner cells.
         */
        void computeSizes();

        void setType(int t)
        { mType = t; }

        int getWidth() const A_WARN_UNUSED
        { return mExtent[0]; }

        int getHeight() const A_WARN_UNUSED
        { return mExtent[1]; }

        void setWidth(int w)
        { mExtent[0] = w; }

        void setHeight(int h)
        { mExtent[1] = h; }

        enum
        {
            NONE = 0,
            WIDGET,
            ARRAY
        };

    private:
        LayoutCell():
            mWidget(nullptr),
            mHPadding(0),
            mVPadding(0),
            mType(NONE)
        {
            mExtent[0] = 0;
            mExtent[1] = 0;
            mAlign[0] = 0;
            mAlign[1] = 0;
            mNbFill[0] = 0;
            mNbFill[1] = 0;
            mSize[0] = 0;
            mSize[1] = 0;
        }

        // Copy not allowed, as the cell may own an array.
        explicit LayoutCell(LayoutCell const &);
        LayoutCell &operator=(LayoutCell const &);

        union
        {
            Widget *mWidget;
            LayoutArray *mArray;
        };

        /**
         * Returns the embedded array. Creates it if the cell does not contain
         * anything yet. Aborts if it contains a widget.
         */
        LayoutArray &getArray();

        /**
         * @see LayoutArray::reflow
         */
        void reflow(int nx, int ny, int nw, int nh);

        int mSize[2];
        int mHPadding;
        int mVPadding;
        int mExtent[2];
        int mAlign[2];
        int mNbFill[2];
        int mType;
};

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

        /**
         * When the minimum size of the layout is less than the available size,
         * the remaining pixels are equally split amongst the FILL items.
         */
        enum
        {
            AUTO_DEF = -42, /**< Default value, behaves like AUTO_ADD. */
            AUTO_SET = -43, /**< Uses the share as the new size. */
            AUTO_ADD = -44  /**< Adds the share to the current size. */
        };

    private:
        bool mComputed;
};

#endif  // GUI_WIDGETS_LAYOUT_H
