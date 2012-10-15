/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/widgets/guitable.h"

#include "client.h"
#include "configuration.h"
#include "keyevent.h"

#include "gui/sdlinput.h"
#include "gui/theme.h"

#include "utils/dtor.h"

#include <guichan/actionlistener.hpp>
#include <guichan/graphics.hpp>
#include <guichan/key.hpp>

#include "debug.h"

float GuiTable::mAlpha = 1.0;

class GuiTableActionListener final : public gcn::ActionListener
{
public:
    GuiTableActionListener(GuiTable *_table, gcn::Widget *_widget,
                           int _row, int _column);

    A_DELETE_COPY(GuiTableActionListener)

    virtual ~GuiTableActionListener();

    virtual void action(const gcn::ActionEvent& actionEvent);

protected:
    GuiTable *mTable;
    int mRow;
    int mColumn;
    gcn::Widget *mWidget;
};


GuiTableActionListener::GuiTableActionListener(GuiTable *table,
                                               gcn::Widget *widget, int row,
                                               int column) :
    ActionListener(),
    mTable(table),
    mRow(row),
    mColumn(column),
    mWidget(widget)
{
    if (widget)
    {
        widget->addActionListener(this);
        widget->_setParent(table);
    }
}

GuiTableActionListener::~GuiTableActionListener()
{
    if (mWidget)
    {
        mWidget->removeActionListener(this);
        mWidget->_setParent(nullptr);
    }
}

void GuiTableActionListener::action(const gcn::ActionEvent
                                    &actionEvent A_UNUSED)
{
    mTable->setSelected(mRow, mColumn);
    mTable->distributeActionEvent();
}


GuiTable::GuiTable(TableModel *const initial_model, const bool opacity) :
    gcn::Widget(),
    gcn::MouseListener(),
    gcn::KeyListener(),
    mLinewiseMode(false),
    mWrappingEnabled(false),
    mOpaque(opacity),
    mModel(nullptr),
    mSelectedRow(0),
    mSelectedColumn(0),
    mTopWidget(nullptr)
{
    mBackgroundColor = Theme::getThemeColor(Theme::BACKGROUND);
    mHighlightColor = Theme::getThemeColor(Theme::HIGHLIGHT);
    setModel(initial_model);
    setFocusable(true);

    addMouseListener(this);
    addKeyListener(this);
}

GuiTable::~GuiTable()
{
    uninstallActionListeners();
    delete mModel;
    mModel = nullptr;
}

TableModel *GuiTable::getModel() const
{
    return mModel;
}

void GuiTable::setModel(TableModel *const new_model)
{
    if (mModel)
    {
        uninstallActionListeners();
        mModel->removeListener(this);
    }

    mModel = new_model;
    installActionListeners();

    if (new_model)
    {
        new_model->installListener(this);
        recomputeDimensions();
    }
}

void GuiTable::recomputeDimensions()
{
    if (!mModel)
        return;

    const int rows_nr = mModel->getRows();
    const int columns_nr = mModel->getColumns();
    int width = 0;

    if (mSelectedRow >= rows_nr)
        mSelectedRow = rows_nr - 1;

    if (mSelectedColumn >= columns_nr)
        mSelectedColumn = columns_nr - 1;

    for (int i = 0; i < columns_nr; i++)
        width += getColumnWidth(i);

    setWidth(width);
    setHeight(getRowHeight() * rows_nr);
}

void GuiTable::setSelected(int row, int column)
{
    mSelectedColumn = column;
    mSelectedRow = row;
}

int GuiTable::getSelectedRow() const
{
    return mSelectedRow;
}

int GuiTable::getSelectedColumn() const
{
    return mSelectedColumn;
}

void GuiTable::setLinewiseSelection(const bool linewise)
{
    mLinewiseMode = linewise;
}

int GuiTable::getRowHeight() const
{
    if (mModel)
        return mModel->getRowHeight() + 1; // border
    else
        return 0;
}

int GuiTable::getColumnWidth(int i) const
{
    if (mModel)
        return mModel->getColumnWidth(i) + 1; // border
    else
        return 0;
}

void GuiTable::setSelectedRow(int selected)
{
    if (!mModel)
    {
        mSelectedRow = -1;
    }
    else
    {
        if (selected < 0 && !mWrappingEnabled)
        {
            mSelectedRow = -1;
        }
        else if (selected >= mModel->getRows() && mWrappingEnabled)
        {
            mSelectedRow = 0;
        }
        else if ((selected >= mModel->getRows() && !mWrappingEnabled) ||
                 (selected < 0 && mWrappingEnabled))
        {
            mSelectedRow = mModel->getRows() - 1;
        }
        else
        {
            mSelectedRow = selected;
        }
    }
}

void GuiTable::setSelectedColumn(int selected)
{
    if (!mModel)
    {
        mSelectedColumn = -1;
    }
    else
    {
        const int columns = mModel->getColumns();
        if ((selected >= columns && mWrappingEnabled) ||
            (selected < 0 && !mWrappingEnabled))
        {
            mSelectedColumn = 0;
        }
        else if ((selected >= columns && !mWrappingEnabled) ||
                 (selected < 0 && mWrappingEnabled))
        {
            mSelectedColumn = columns - 1;
        }
        else
        {
            mSelectedColumn = selected;
        }
    }
}

void GuiTable::uninstallActionListeners()
{
    delete_all(mActionListeners);
    mActionListeners.clear();
}

void GuiTable::installActionListeners()
{
    if (!mModel)
        return;

    const int rows = mModel->getRows();
    const int columns = mModel->getColumns();

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            gcn::Widget *const widget = mModel->getElementAt(row, column);
            if (widget)
            {
                mActionListeners.push_back(new GuiTableActionListener(
                    this, widget, row, column));
            }
        }
    }

    _setFocusHandler(_getFocusHandler()); // propagate focus handler to widgets
}

// -- widget ops
void GuiTable::draw(gcn::Graphics* graphics)
{
    if (!mModel || !getRowHeight())
        return;

    if (Client::getGuiAlpha() != mAlpha)
        mAlpha = Client::getGuiAlpha();

    if (mOpaque)
    {
        mBackgroundColor.a = static_cast<int>(mAlpha * 255.0f);
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
    }

    // First, determine how many rows we need to draw, and where we should start.
    int first_row = -(getY() / getRowHeight());

    if (first_row < 0)
        first_row = 0;

    unsigned rows_nr = 1 + (getHeight() / getRowHeight()); // May overestimate
                                                           // by one.

    unsigned max_rows_nr;
    if (mModel->getRows() < first_row)
        max_rows_nr = 0;
    else
        max_rows_nr = mModel->getRows() - first_row; // clip if neccessary:
    if (max_rows_nr < rows_nr)
        rows_nr = max_rows_nr;

    // Now determine the first and last column
    // Take the easy way out; these are usually bounded and all visible.
    const unsigned first_column = 0;
    const unsigned last_column1 = mModel->getColumns();

    // Set up everything for drawing
    const int height = getRowHeight();
    int y_offset = first_row * height;

    for (unsigned r = first_row; r < first_row + rows_nr; ++r)
    {
        int x_offset = 0;

        for (unsigned c = first_column; c + 1 <= last_column1; ++c)
        {
            gcn::Widget *const widget = mModel->getElementAt(r, c);
            const int width = getColumnWidth(c);
            if (widget)
            {
                gcn::Rectangle bounds(x_offset, y_offset, width, height);

                if (widget == mTopWidget)
                {
                    bounds.height = widget->getHeight();
                    bounds.width = widget->getWidth();
                }

                widget->setDimension(bounds);

                if (mSelectedRow > 0)
                {
                    mHighlightColor.a = static_cast<int>(mAlpha * 255.0f);
                    graphics->setColor(mHighlightColor);

                    if (mLinewiseMode && r == static_cast<unsigned>(
                        mSelectedRow) && c == 0)
                    {
                        graphics->fillRectangle(gcn::Rectangle(0, y_offset,
                            getWidth(), height));
                    }
                    else if (!mLinewiseMode && mSelectedColumn > 0
                             && c == static_cast<unsigned>(mSelectedColumn)
                             && r == static_cast<unsigned>(mSelectedRow))
                    {
                        graphics->fillRectangle(gcn::Rectangle(
                            x_offset, y_offset, width, height));
                    }
                }
                graphics->pushClipArea(bounds);
                widget->draw(graphics);
                graphics->popClipArea();
            }

            x_offset += width;
        }

        y_offset += height;
    }

    if (mTopWidget)
    {
        const gcn::Rectangle bounds = mTopWidget->getDimension();
        graphics->pushClipArea(bounds);
        mTopWidget->draw(graphics);
        graphics->popClipArea();
    }
}

void GuiTable::moveToTop(gcn::Widget *widget)
{
    gcn::Widget::moveToTop(widget);
    mTopWidget = widget;
}

void GuiTable::moveToBottom(gcn::Widget *widget)
{
    gcn::Widget::moveToBottom(widget);
    if (widget == mTopWidget)
        mTopWidget = nullptr;
}

gcn::Rectangle GuiTable::getChildrenArea()
{
    return gcn::Rectangle(0, 0, getWidth(), getHeight());
}

// -- KeyListener notifications
void GuiTable::keyPressed(gcn::KeyEvent& keyEvent)
{
    const int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    if (action == Input::KEY_GUI_SELECT)
    {
        distributeActionEvent();
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_UP)
    {
        setSelectedRow(mSelectedRow - 1);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_DOWN)
    {
        setSelectedRow(mSelectedRow + 1);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_LEFT)
    {
        setSelectedColumn(mSelectedColumn - 1);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_RIGHT)
    {
        setSelectedColumn(mSelectedColumn + 1);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_HOME)
    {
        setSelectedRow(0);
        setSelectedColumn(0);
        keyEvent.consume();
    }
    else if (action == Input::KEY_GUI_END && mModel)
    {
        setSelectedRow(mModel->getRows() - 1);
        setSelectedColumn(mModel->getColumns() - 1);
        keyEvent.consume();
    }
}

// -- MouseListener notifications
void GuiTable::mousePressed(gcn::MouseEvent& mouseEvent)
{
    if (!mModel)
        return;

    if (mouseEvent.getButton() == gcn::MouseEvent::LEFT)
    {
        const int row = getRowForY(mouseEvent.getY());
        const int column = getColumnForX(mouseEvent.getX());

        if (row > -1 && column > -1 &&
            row < mModel->getRows() && column < mModel->getColumns())
        {
            mSelectedColumn = column;
            mSelectedRow = row;
        }

        distributeActionEvent();
    }
}

void GuiTable::mouseWheelMovedUp(gcn::MouseEvent& mouseEvent)
{
    if (isFocused())
    {
        if (getSelectedRow() > 0 || (getSelectedRow() == 0
            && mWrappingEnabled))
        {
            setSelectedRow(getSelectedRow() - 1);
        }
        mouseEvent.consume();
    }
}

void GuiTable::mouseWheelMovedDown(gcn::MouseEvent& mouseEvent)
{
    if (isFocused())
    {
        setSelectedRow(getSelectedRow() + 1);
        mouseEvent.consume();
    }
}

void GuiTable::mouseDragged(gcn::MouseEvent& mouseEvent)
{
    if (mouseEvent.getButton() != gcn::MouseEvent::LEFT)
        return;

    // Make table selection update on drag
    const int x = std::max(0, mouseEvent.getX());
    const int y = std::max(0, mouseEvent.getY());

    setSelectedRow(getRowForY(y));
    setSelectedColumn(getColumnForX(x));
}

// -- TableModelListener notifications
void GuiTable::modelUpdated(bool completed)
{
    if (completed)
    {
        recomputeDimensions();
        installActionListeners();
    }
    else
    { // before the update?
        mTopWidget = nullptr; // No longer valid in general
        uninstallActionListeners();
    }
}

gcn::Widget *GuiTable::getWidgetAt(int x, int y)
{
    const int row = getRowForY(y);
    const int column = getColumnForX(x);

    if (mTopWidget && mTopWidget->getDimension().isPointInRect(x, y))
        return mTopWidget;

    if (mModel && row > -1 && column > -1)
    {
        gcn::Widget *const w = mModel->getElementAt(row, column);
        if (w && w->isFocusable())
            return w;
        else
            return nullptr; // Grab the event locally
    }
    else
        return nullptr;
}

int GuiTable::getRowForY(int y) const
{
   int row = -1;

   if (getRowHeight() > 0)
       row = y / getRowHeight();

   if (!mModel || row < 0 || row >= mModel->getRows())
       return -1;
   else
       return row;
}

int GuiTable::getColumnForX(int x) const
{
    if (!mModel)
        return -1;

    int column;
    int delta = 0;

    const int colnum = mModel->getColumns();
    for (column = 0; column < colnum; column ++)
    {
        delta += getColumnWidth(column);
        if (x <= delta)
            break;
    }

    if (column < 0 || column >= colnum)
        return -1;
    else
        return column;
}

void GuiTable::_setFocusHandler(gcn::FocusHandler* focusHandler)
{
// add check for focusHandler. may be need remove it?

    if (!mModel || !focusHandler)
        return;

    gcn::Widget::_setFocusHandler(focusHandler);

    if (mModel)
    {
        for (int r = 0; r < mModel->getRows(); ++r)
        {
            for (int c = 0; c < mModel->getColumns(); ++c)
            {
                gcn::Widget *const w = mModel->getElementAt(r, c);
                if (w)
                    w->_setFocusHandler(focusHandler);
            }
        }
    }
}
