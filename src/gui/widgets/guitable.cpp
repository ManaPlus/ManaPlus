/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/guitable.h"

#include "settings.h"

#include "gui/gui.h"

#include "gui/models/tablemodel.h"

#include "listeners/guitableactionlistener.h"

#include "render/graphics.h"

#include "utils/delete2.h"
#include "utils/dtor.h"

#include "debug.h"

float GuiTable::mAlpha = 1.0;

GuiTable::GuiTable(const Widget2 *const widget,
                   TableModel *const initial_model,
                   const Opaque opacity) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    TableModelListener(),
    mModel(nullptr),
    mTopWidget(nullptr),
    mActionListeners2(),
    mHighlightColor(getThemeColor(ThemeColorId::HIGHLIGHT)),
    mSelectedRow(-1),
    mSelectedColumn(-1),
    mLinewiseMode(false),
    mWrappingEnabled(false),
    mOpaque(opacity),
    mSelectableGui(true)
{
    mAllowLogic = false;
    mBackgroundColor = getThemeColor(ThemeColorId::BACKGROUND);

    setModel(initial_model);
    setFocusable(true);

    addMouseListener(this);
    addKeyListener(this);
}

GuiTable::~GuiTable()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    uninstallActionListeners();
    delete2(mModel);
}

const TableModel *GuiTable::getModel() const
{
    return mModel;
}

void GuiTable::setModel(TableModel *const new_model)
{
    if (mModel != nullptr)
    {
        uninstallActionListeners();
        mModel->removeListener(this);
    }

    mModel = new_model;
    installActionListeners();

    new_model->installListener(this);
    recomputeDimensions();
}

void GuiTable::recomputeDimensions()
{
    if (mModel == nullptr)
        return;

    const int rows_nr = mModel->getRows();
    const int columns_nr = mModel->getColumns();
    int width = 0;

    if (mSelectableGui)
    {
        if (mSelectedRow >= rows_nr)
            mSelectedRow = rows_nr - 1;

        if (mSelectedColumn >= columns_nr)
            mSelectedColumn = columns_nr - 1;
    }

    for (int i = 0; i < columns_nr; i++)
        width += getColumnWidth(i);

    setWidth(width);
    setHeight(getRowHeight() * rows_nr);
}

void GuiTable::setSelected(const int row, const int column)
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

int GuiTable::getRowHeight() const
{
    return mModel->getRowHeight() + 4;  // border
}

int GuiTable::getColumnWidth(const int i) const
{
    return mModel->getColumnWidth(i) + 4;  // border
}

void GuiTable::setSelectedRow(const int selected)
{
    if (!mSelectableGui)
    {
        mSelectedRow = -1;
    }
    else
    {
        const int rows = mModel->getRows();
        if (selected < 0 && !mWrappingEnabled)
        {
            mSelectedRow = -1;
        }
        else if (selected >= rows && mWrappingEnabled)
        {
            mSelectedRow = 0;
        }
        else if ((selected >= rows && !mWrappingEnabled) ||
                 (selected < 0 && mWrappingEnabled))
        {
            mSelectedRow = rows - 1;
        }
        else
        {
            mSelectedRow = selected;
        }
    }
}

void GuiTable::setSelectedColumn(const int selected)
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

void GuiTable::uninstallActionListeners()
{
    delete_all(mActionListeners2);
    mActionListeners2.clear();
}

void GuiTable::installActionListeners()
{
    const int rows = mModel->getRows();
    const int columns = mModel->getColumns();

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            Widget *const widget = mModel->getElementAt(row, column);
            if (widget != nullptr)
            {
                mActionListeners2.push_back(new GuiTableActionListener(
                    this, widget, row, column));
            }
        }
    }

    setFocusHandler(getFocusHandler());
}

// -- widget ops
void GuiTable::draw(Graphics *const graphics)
{
    if (getRowHeight() == 0)
        return;

    BLOCK_START("GuiTable::draw")
    if (settings.guiAlpha != mAlpha)
        mAlpha = settings.guiAlpha;

    const Rect &rect = mDimension;
    const int width = rect.width;
    const int height = rect.height;
    const int y = rect.y;
    if (mOpaque == Opaque_true)
    {
        mBackgroundColor.a = CAST_U32(mAlpha * 255.0F);
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(Rect(0, 0, width, height));
    }

    // First, determine how many rows we need to draw,
    // and where we should start.
    int rHeight = getRowHeight();
    if (rHeight == 0)
        rHeight = 1;
    int first_row = -(y / rHeight);

    if (first_row < 0)
        first_row = 0;

    unsigned int rows_nr = CAST_U32(1 +
        height / rHeight);  // May overestimate by one.
    unsigned int max_rows_nr;
    if (mModel->getRows() < first_row)
    {
        max_rows_nr = 0U;
    }
    else
    {
        max_rows_nr = CAST_U32(
            mModel->getRows() - first_row);  // clip if neccessary:
    }
    if (max_rows_nr < rows_nr)
        rows_nr = max_rows_nr;

    // Now determine the first and last column
    // Take the easy way out; these are usually bounded and all visible.
    const unsigned first_column = 0;
    const unsigned last_column1 = CAST_U32(
        mModel->getColumns());

    int y_offset = first_row * rHeight;

    for (unsigned int r = CAST_U32(first_row);
         r < CAST_U32(first_row) + rows_nr;
         ++r)
    {
        int x_offset = 0;

        for (unsigned c = first_column; c + 1 <= last_column1; ++c)
        {
            Widget *const widget = mModel->getElementAt(CAST_S32(r),
                CAST_S32(c));
            const int cWidth = CAST_S32(getColumnWidth(
                CAST_S32(c)));
            if (widget != nullptr)
            {
                Rect bounds(x_offset, y_offset, cWidth, rHeight);

                if (widget == mTopWidget)
                {
                    bounds.height = widget->getHeight();
                    bounds.width = widget->getWidth();
                }

                widget->setDimension(bounds);

                if (mSelectedRow > -1)
                {
                    mHighlightColor.a = CAST_U32(
                        mAlpha * 255.0F);
                    graphics->setColor(mHighlightColor);

                    if (mLinewiseMode && r == CAST_U32(
                        mSelectedRow) && c == 0)
                    {
                        graphics->fillRectangle(Rect(0, y_offset,
                            width, rHeight));
                    }
                    else if (!mLinewiseMode && mSelectedColumn > 0
                             && c == CAST_U32(mSelectedColumn)
                             && r == CAST_U32(mSelectedRow))
                    {
                        graphics->fillRectangle(Rect(
                            x_offset, y_offset, cWidth, rHeight));
                    }
                }
                graphics->pushClipArea(bounds);
                widget->draw(graphics);
                graphics->popClipArea();
            }

            x_offset += cWidth;
        }

        y_offset += rHeight;
    }

    if (mTopWidget != nullptr)
    {
        const Rect &bounds = mTopWidget->getDimension();
        graphics->pushClipArea(bounds);
        mTopWidget->draw(graphics);
        graphics->popClipArea();
    }
    BLOCK_END("GuiTable::draw")
}

void GuiTable::safeDraw(Graphics *const graphics)
{
    if (getRowHeight() == 0)
        return;

    BLOCK_START("GuiTable::draw")
    if (settings.guiAlpha != mAlpha)
        mAlpha = settings.guiAlpha;

    const Rect &rect = mDimension;
    const int width = rect.width;
    const int height = rect.height;
    const int y = rect.y;
    if (mOpaque == Opaque_true)
    {
        mBackgroundColor.a = CAST_U32(mAlpha * 255.0F);
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(Rect(0, 0, width, height));
    }

    // First, determine how many rows we need to draw,
    // and where we should start.
    int rHeight = getRowHeight();
    if (rHeight == 0)
        rHeight = 1;
    int first_row = -(y / rHeight);

    if (first_row < 0)
        first_row = 0;

    unsigned int rows_nr = CAST_U32(
        1 + height / rHeight);  // May overestimate by one.
    unsigned int max_rows_nr;
    if (mModel->getRows() < first_row)
    {
        max_rows_nr = 0;
    }
    else
    {
        max_rows_nr = CAST_U32(
            mModel->getRows() - first_row);  // clip if neccessary:
    }
    if (max_rows_nr < rows_nr)
        rows_nr = max_rows_nr;

    // Now determine the first and last column
    // Take the easy way out; these are usually bounded and all visible.
    const unsigned int first_column = 0;
    const unsigned int last_column1 = CAST_U32(
        mModel->getColumns());

    int y_offset = first_row * rHeight;

    for (unsigned int r = CAST_U32(first_row);
         r < CAST_U32(first_row + CAST_S32(rows_nr));
         ++r)
    {
        int x_offset = 0;

        for (unsigned c = first_column; c + 1 <= last_column1; ++c)
        {
            Widget *const widget = mModel->getElementAt(CAST_S32(r),
                CAST_S32(c));
            const int cWidth = CAST_S32(getColumnWidth(
                CAST_S32(c)));
            if (widget != nullptr)
            {
                Rect bounds(x_offset, y_offset, cWidth, rHeight);

                if (widget == mTopWidget)
                {
                    bounds.height = widget->getHeight();
                    bounds.width = widget->getWidth();
                }

                widget->setDimension(bounds);

                if (mSelectedRow > -1)
                {
                    mHighlightColor.a = CAST_U32(
                        mAlpha * 255.0F);
                    graphics->setColor(mHighlightColor);

                    if (mLinewiseMode && r == CAST_U32(
                        mSelectedRow) && c == 0)
                    {
                        graphics->fillRectangle(Rect(0, y_offset,
                            width, rHeight));
                    }
                    else if (!mLinewiseMode && mSelectedColumn > 0
                             && c == CAST_U32(mSelectedColumn)
                             && r == CAST_U32(mSelectedRow))
                    {
                        graphics->fillRectangle(Rect(
                            x_offset, y_offset, cWidth, rHeight));
                    }
                }
                graphics->pushClipArea(bounds);
                widget->safeDraw(graphics);
                graphics->popClipArea();
            }

            x_offset += cWidth;
        }

        y_offset += rHeight;
    }

    if (mTopWidget != nullptr)
    {
        const Rect &bounds = mTopWidget->getDimension();
        graphics->pushClipArea(bounds);
        mTopWidget->safeDraw(graphics);
        graphics->popClipArea();
    }
    BLOCK_END("GuiTable::draw")
}

void GuiTable::moveToTop(Widget *const widget)
{
    Widget::moveToTop(widget);
    mTopWidget = widget;
}

void GuiTable::moveToBottom(Widget *const widget)
{
    Widget::moveToBottom(widget);
    if (widget == mTopWidget)
        mTopWidget = nullptr;
}

Rect GuiTable::getChildrenArea()
{
    return Rect(0, 0, mDimension.width, mDimension.height);
}

// -- KeyListener notifications
void GuiTable::keyPressed(KeyEvent& event)
{
    const InputActionT action = event.getActionId();

    if (action == InputAction::GUI_SELECT)
    {
        distributeActionEvent();
        event.consume();
    }
    else if (action == InputAction::GUI_UP)
    {
        setSelectedRow(mSelectedRow - 1);
        event.consume();
    }
    else if (action == InputAction::GUI_DOWN)
    {
        setSelectedRow(mSelectedRow + 1);
        event.consume();
    }
    else if (action == InputAction::GUI_LEFT)
    {
        setSelectedColumn(mSelectedColumn - 1);
        event.consume();
    }
    else if (action == InputAction::GUI_RIGHT)
    {
        setSelectedColumn(mSelectedColumn + 1);
        event.consume();
    }
    else if (action == InputAction::GUI_HOME)
    {
        setSelectedRow(0);
        setSelectedColumn(0);
        event.consume();
    }
    else if (action == InputAction::GUI_END && (mModel != nullptr))
    {
        setSelectedRow(mModel->getRows() - 1);
        setSelectedColumn(mModel->getColumns() - 1);
        event.consume();
    }
}

// -- MouseListener notifications
void GuiTable::mousePressed(MouseEvent& event)
{
    if (!mSelectableGui)
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        const int row = getRowForY(event.getY());
        const int column = getColumnForX(event.getX());

        if (row > -1 && column > -1 &&
            row < mModel->getRows() && column < mModel->getColumns())
        {
            mSelectedColumn = column;
            mSelectedRow = row;
            event.consume();
        }

        distributeActionEvent();
    }
}

void GuiTable::mouseWheelMovedUp(MouseEvent& event)
{
    if (isFocused())
    {
        const int selRow = getSelectedRow();
        if (selRow > 0 || (selRow == 0 && mWrappingEnabled))
            setSelectedRow(selRow - 1);
        event.consume();
    }
}

void GuiTable::mouseWheelMovedDown(MouseEvent& event)
{
    if (isFocused())
    {
        setSelectedRow(getSelectedRow() + 1);
        event.consume();
    }
}

void GuiTable::mouseDragged(MouseEvent& event)
{
    if (event.getButton() != MouseButton::LEFT)
        return;

    // Make table selection update on drag
    const int x = std::max(0, event.getX());
    const int y = std::max(0, event.getY());

    setSelectedRow(getRowForY(y));
    setSelectedColumn(getColumnForX(x));
}

void GuiTable::modelUpdated(const bool completed)
{
    if (completed)
    {
        recomputeDimensions();
        installActionListeners();
    }
    else
    {   // before the update?
        mTopWidget = nullptr;  // No longer valid in general
        uninstallActionListeners();
    }
}

Widget *GuiTable::getWidgetAt(int x, int y)
{
    if (mModel == nullptr)
        return nullptr;

    const int row = getRowForY(y);
    const int column = getColumnForX(x);

    if (mTopWidget != nullptr &&
        mTopWidget->getDimension().isPointInRect(x, y))
    {
        return mTopWidget;
    }

    if (row > -1 && column > -1)
    {
        Widget *const w = mModel->getElementAt(row, column);
        if ((w != nullptr) && w->isFocusable())
            return w;
        else
            return nullptr;  // Grab the event locally
    }
    else
        return nullptr;
}

int GuiTable::getRowForY(const int y) const
{
    int row = -1;

    const int rowHeight = getRowHeight();
    if (rowHeight > 0)
       row = y / rowHeight;

    if (row < 0 || row >= mModel->getRows())
        return -1;
    else
        return row;
}

int GuiTable::getColumnForX(const int x) const
{
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

void GuiTable::setFocusHandler(FocusHandler *const focusHandler)
{
    // add check for focusHandler. may be need remove it?

    if (focusHandler == nullptr)
        return;

    Widget::setFocusHandler(focusHandler);

    const int rows = mModel->getRows();
    const int cols = mModel->getColumns();
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols ; ++c)
        {
            Widget *const w = mModel->getElementAt(r, c);
            if (w != nullptr)
                w->setFocusHandler(focusHandler);
        }
    }
}

void GuiTable::requestFocus()
{
    if (mFocusHandler == nullptr)
        return;
    Widget::requestFocus();
}
