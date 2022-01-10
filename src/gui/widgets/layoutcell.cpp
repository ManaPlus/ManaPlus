/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/layoutcell.h"

#include "gui/widgets/layoutarray.h"
#include "gui/widgets/widget.h"

#include "utils/delete2.h"

#include "debug.h"

static LayoutArray tempArray;
LayoutCell LayoutCell::emptyCell;

LayoutCell::~LayoutCell()
{
    if (mType == ARRAY)
        delete2(mArray)
}

LayoutArray &LayoutCell::getArray()
{
    if (mType == WIDGET)
        return tempArray;
    if (mType == ARRAY)
    {
        if (mArray == nullptr)
            return tempArray;
        return *mArray;
    }

    mArray = new LayoutArray;
    mType = ARRAY;
    mExtent[0] = 1;
    mExtent[1] = 1;
    mHPadding = 0;
    mVPadding = 0;
    mAlign[0] = FILL;
    mAlign[1] = FILL;
    return *mArray;
}

void LayoutCell::reflow(int nx, int ny, int nw, int nh)
{
    if (mType == NONE)
        return;

    nx += mHPadding;
    ny += mVPadding;
    nw -= 2 * mHPadding;
    nh -= 2 * mVPadding;
    if (mType == ARRAY)
        mArray->reflow(nx, ny, nw, nh);
    else
        mWidget->setDimension(Rect(nx, ny, nw, nh));
}

void LayoutCell::computeSizes()
{
    if (mType != ARRAY)
        return;

    STD_VECTOR <STD_VECTOR <LayoutCell *> >::const_iterator
        i = mArray->mCells.begin();
    const STD_VECTOR <STD_VECTOR <LayoutCell *> >::const_iterator
        i_end = mArray->mCells.end();
    while (i != i_end)
    {
        STD_VECTOR <LayoutCell *>::const_iterator j = i->begin();
        while (j != i->end())
        {
            LayoutCell *const cell = *j;
            if ((cell != nullptr) && cell->mType == ARRAY)
                cell->computeSizes();

            ++j;
        }
        ++i;
    }

    mSize[0] = mArray->getSize(0);
    mSize[1] = mArray->getSize(1);
}

LayoutCell &LayoutCell::at(const int x, const int y)
{
    return getArray().at(x, y, 1, 1);
}

LayoutCell &LayoutCell::place(Widget *const wg,
                              const int x, const int y,
                              const int w, const int h)
{
    return getArray().place(wg, x, y, w, h);
}

void LayoutCell::matchColWidth(const int n1, const int n2)
{
    getArray().matchColWidth(n1, n2);
}

void LayoutCell::setColWidth(const int n, const int w)
{
    getArray().setColWidth(n, w);
}

void LayoutCell::setRowHeight(const int n, const int h)
{
    getArray().setRowHeight(n, h);
}

void LayoutCell::extend(const int x, const int y,
                        const int w, const int h)
{
    getArray().extend(x, y, w, h);
}
