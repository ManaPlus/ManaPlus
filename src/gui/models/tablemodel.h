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

#ifndef GUI_MODELS_TABLEMODEL_H
#define GUI_MODELS_TABLEMODEL_H

#include <set>
#include <vector>

#include "localconsts.h"

class Widget;

class TableModelListener
{
public:
    /**
     * Must be invoked by the TableModel whenever a global change is about to
     * occur or has occurred (e.g., when a row or column is being removed or
     * added).
     *
     * This method is triggered twice, once before and once after the update.
     *
     * \param completed whether we are signalling the end of the update
     */
    virtual void modelUpdated(const bool completed) = 0;

    virtual ~TableModelListener()
    { }
};

/**
 * A model for a regular table of widgets.
 */
class TableModel
{
public:
    virtual ~TableModel()
    { }

    /**
     * Determines the number of rows (lines) in the table
     */
    virtual int getRows() const A_WARN_UNUSED = 0;

    /**
     * Determines the number of columns in each row
     */
    virtual int getColumns() const A_WARN_UNUSED = 0;

    /**
     * Determines the height for each row
     */
    virtual int getRowHeight() const A_WARN_UNUSED = 0;

    /**
     * Determines the width of each individual column
     */
    virtual int getColumnWidth(const int index) const A_WARN_UNUSED = 0;

    /**
     * Retrieves the widget stored at the specified location within the table.
     */
    virtual Widget *getElementAt(const int row, const int column)
                                      const A_WARN_UNUSED = 0;

    virtual void installListener(TableModelListener *const listener);

    virtual void removeListener(TableModelListener *const listener);

protected:
    TableModel() :
        listeners()
    {
    }

    /**
     * Tells all listeners that the table is about to see an update
     */
    virtual void signalBeforeUpdate();

    /**
     * Tells all listeners that the table has seen an update
     */
    virtual void signalAfterUpdate();

private:
    std::set<TableModelListener *> listeners;
};


class StaticTableModel final : public TableModel
{
public:
    StaticTableModel(const int width, const int height);

    A_DELETE_COPY(StaticTableModel)

    ~StaticTableModel();

    /**
     * Inserts a widget into the table model.
     * The model is resized to accomodate the widget's width and height,
     * unless column width / row height have been fixed.
     */
    void set(const int row, const int column, Widget *const widget);

    /**
     * Fixes the column width for a given column; this overrides dynamic width
     * inference.
     *
     * Semantics are undefined for width 0.
     */
    void fixColumnWidth(const int column, const int width);

    /**
     * Fixes the row height; this overrides dynamic height inference.
     *
     * Semantics are undefined for width 0.
     */
    void fixRowHeight(const int height);

    /**
     * Resizes the table model
     */
    void resize();

    int getRows() const override final A_WARN_UNUSED;
    int getColumns() const override final A_WARN_UNUSED;
    int getRowHeight() const override final A_WARN_UNUSED;
    int getWidth() const A_WARN_UNUSED;
    int getHeight() const A_WARN_UNUSED;
    int getColumnWidth(const int index) const override final A_WARN_UNUSED;
    Widget *getElementAt(const int row,
                         const int column) const override final A_WARN_UNUSED;

protected:
    int mRows, mColumns;
    int mHeight;
    std::vector<Widget *> mTableModel;
    std::vector<int> mWidths;
};

#endif  // GUI_MODELS_TABLEMODEL_H
