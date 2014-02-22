/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/models/tablemodel.h"

#include "utils/dtor.h"

#include "gui/widgets/widget.h"

#include "debug.h"

void TableModel::installListener(TableModelListener *const listener)
{
    if (listener)
        listeners.insert(listener);
}

void TableModel::removeListener(TableModelListener *const listener)
{
    if (listener)
        listeners.erase(listener);
}

void TableModel::signalBeforeUpdate()
{
    for (std::set<TableModelListener *>::const_iterator it = listeners.begin();
         it != listeners.end(); ++it)
    {
        (*it)->modelUpdated(false);
    }
}

void TableModel::signalAfterUpdate()
{
    for (std::set<TableModelListener *>::const_iterator it = listeners.begin();
         it != listeners.end(); ++it)
    {
        if (*it)
            (*it)->modelUpdated(true);
    }
}


#define WIDGET_AT(row, column) (((row) * mColumns) + (column))
#define DYN_SIZE(h) ((h) >= 0)

StaticTableModel::StaticTableModel(const int row, const int column) :
    TableModel(),
    mRows(row),
    mColumns(column),
    mHeight(1),
    mTableModel(),
    mWidths()
{
    mTableModel.resize(row * column, nullptr);
    mWidths.resize(column, 1);
}

StaticTableModel::~StaticTableModel()
{
    delete_all(mTableModel);
    mTableModel.clear();
}

void StaticTableModel::resize()
{
    mRows = getRows();
    mColumns = getColumns();
    mTableModel.resize(mRows * mColumns, nullptr);
}

void StaticTableModel::set(const int row, const int column,
                           Widget *const widget)
{
    if (!widget || row >= mRows || row < 0
        || column >= mColumns || column < 0)
    {
        // raise exn?
        return;
    }

    if (DYN_SIZE(mHeight)
        && widget->getHeight() > mHeight)
    {
        mHeight = widget->getHeight();
    }

    if (DYN_SIZE(mWidths[column])
        && widget->getWidth() > mWidths[column])
    {
        mWidths[column] = widget->getWidth();
    }

    signalBeforeUpdate();

    delete mTableModel[WIDGET_AT(row, column)];

    mTableModel[WIDGET_AT(row, column)] = widget;

    signalAfterUpdate();
}

Widget *StaticTableModel::getElementAt(const int row,
                                       const int column) const
{
    return mTableModel[WIDGET_AT(row, column)];
}

void StaticTableModel::fixColumnWidth(const int column, const int width)
{
    if (width < 0 || column < 0 || column >= mColumns)
        return;

    mWidths[column] = -width;  // Negate to tag as fixed
}

void StaticTableModel::fixRowHeight(const int height)
{
    if (height < 0)
        return;

    mHeight = -height;
}

int StaticTableModel::getRowHeight() const
{
    return abs(mHeight);
}

int StaticTableModel::getColumnWidth(const int column) const
{
    if (column < 0 || column >= mColumns)
        return 0;

    return abs(mWidths[column]);
}

int StaticTableModel::getRows() const
{
    return mRows;
}

int StaticTableModel::getColumns() const
{
    return mColumns;
}

int StaticTableModel::getWidth() const
{
    int width = 0;

    for (size_t i = 0, sz = mWidths.size(); i < sz; i++)
        width += mWidths[i];

    return width;
}

int StaticTableModel::getHeight() const
{
    return mColumns * mHeight;
}
