/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_LAYOUTCELL_H
#define GUI_WIDGETS_LAYOUTCELL_H

#include "localconsts.h"

class LayoutArray;
class Widget;

/**
 * This class describes the formatting of a widget in the cell of a layout
 * table. Horizontally, a widget can either fill the width of the cell (minus
 * the cell padding), or it can retain its size and be flushed left, or flush
 * right, or centered in the cell. The process is similar for the vertical
 * alignment, except that top is represented by LEFT and bottom by RIGHT.
 */
class LayoutCell notfinal
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
        LayoutCell &setHAlign(const Alignment a)
        { mAlign[0] = a; return *this; }

        /**
         * Sets the vertical alignment of the cell content.
         */
        LayoutCell &setVAlign(const Alignment a)
        { mAlign[1] = a; return *this; }

        /**
         * @see LayoutArray::at
         */
        LayoutCell &at(const int x, const int y) A_WARN_UNUSED;

        /**
         * @see LayoutArray::place
         */
        LayoutCell &place(Widget *const wg,
                          const int x, const int y,
                          const int w = 1, const int h = 1);

        /**
         * @see LayoutArray::matchColWidth
         */
        void matchColWidth(const int n1, const int n2);

        /**
         * @see LayoutArray::setColWidth
         */
        void setColWidth(const int n, const int w);

        /**
         * @see LayoutArray::setRowHeight
         */
        void setRowHeight(const int n, const int h);

        /**
         * @see LayoutArray::extend.
         */
        void extend(const int x, const int y,
                    const int w, const int h);

        /**
         * Sets the minimum widths and heights of this cell and of all the
         * inner cells.
         */
        void computeSizes();

        void setType(int t)
        { mType = t; }

        int getWidth() const noexcept2 A_WARN_UNUSED
        { return mExtent[0]; }

        int getHeight() const noexcept2 A_WARN_UNUSED
        { return mExtent[1]; }

        void setWidth(const int w) noexcept2
        { mExtent[0] = w; }

        void setHeight(const int h) noexcept2
        { mExtent[1] = h; }

        enum
        {
            NONE = 0,
            WIDGET,
            ARRAY
        };

        static LayoutCell emptyCell;

    private:
        LayoutCell() :
            mWidget(nullptr),
            mHPadding(0),
            mVPadding(0),
            mType(NONE)
        {
            mExtent[0] = 0;
            mExtent[1] = 0;
            mAlign[0] = LEFT;
            mAlign[1] = LEFT;
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
        Alignment mAlign[2];
        int mNbFill[2];
        int mType;
};

#endif  // GUI_WIDGETS_LAYOUTCELL_H
