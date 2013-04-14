/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef TABLE_H
#define TABLE_H

#include "localconsts.h"

#include "gui/widgets/tablemodel.h"
#include "gui/widgets/widget2.h"

#include <guichan/keylistener.hpp>
#include <guichan/mouselistener.hpp>
#include <guichan/widget.hpp>

#include <vector>

class GuiTableActionListener;

/**
 * A table, with rows and columns made out of sub-widgets. Largely inspired by
 * (and can be thought of as a generalisation of) the guichan listbox
 * implementation.
 *
 * Normally you want this within a ScrollArea.
 *
 * \ingroup GUI
 */
class GuiTable final : public gcn::Widget,
                       public Widget2,
                       public gcn::MouseListener,
                       public gcn::KeyListener,
                       public TableModelListener
{
    // so that the action listener can call distributeActionEvent
    friend class GuiTableActionListener;

public:
    GuiTable(const Widget2 *const widget,
             TableModel *const initial_model = nullptr,
             const bool opacity = true);

    A_DELETE_COPY(GuiTable)

    virtual ~GuiTable();

    /**
     * Retrieves the active table model
     */
    const TableModel *getModel() const A_WARN_UNUSED;

    /**
     * Sets the table model
     *
     * Note that actions issued by widgets returned from the model will update
     * the table selection, but only AFTER any event handlers installed within
     * the widget have been triggered. To be notified after such an update, add
     * an action listener to the table instead.
     */
    void setModel(TableModel *const m);

    void setSelected(int row, int column);

    int getSelectedRow() const A_WARN_UNUSED;

    int getSelectedColumn() const A_WARN_UNUSED;

    void setSelectedRow(int selected);

    void setSelectedColumn(int selected);

    bool isWrappingEnabled() const A_WARN_UNUSED
    { return mWrappingEnabled; }

    void setWrappingEnabled(bool wrappingEnabled)
    { mWrappingEnabled = wrappingEnabled; }

    gcn::Rectangle getChildrenArea() override A_WARN_UNUSED;

    /**
     * Toggle whether to use linewise selection mode, in which the table selects
     * an entire line at a time, rather than a single cell.
     *
     * Note that column information is tracked even in linewise selection mode;
     * this mode therefore only affects visualisation.
     *
     * Disabled by default.
     *
     * \param linewise: Whether to enable linewise selection mode
     */
    void setLinewiseSelection(bool linewise);

    // Inherited from Widget
    virtual void draw(gcn::Graphics* graphics) override;

    virtual gcn::Widget *getWidgetAt(int x, int y) override A_WARN_UNUSED;

    virtual void moveToTop(gcn::Widget *child) override;

    virtual void moveToBottom(gcn::Widget *child) override;

    virtual void _setFocusHandler(gcn::FocusHandler* focusHandler) override;

    // Inherited from KeyListener
    virtual void keyPressed(gcn::KeyEvent& keyEvent) override;

    /**
     * Sets the table to be opaque, that is sets the table
     * to display its background.
     *
     * @param opaque True if the table should be opaque, false otherwise.
     */
    virtual void setOpaque(bool opaque)
    { mOpaque = opaque; }

    /**
     * Checks if the table is opaque, that is if the table area displays its
     * background.
     *
     * @return True if the table is opaque, false otherwise.
     */
    virtual bool isOpaque() const A_WARN_UNUSED
    { return mOpaque; }

    // Inherited from MouseListener
    virtual void mousePressed(gcn::MouseEvent& mouseEvent) override;

    virtual void mouseWheelMovedUp(gcn::MouseEvent& mouseEvent) override;

    virtual void mouseWheelMovedDown(gcn::MouseEvent& mouseEvent) override;

    virtual void mouseDragged(gcn::MouseEvent& mouseEvent) override;

    // Constraints inherited from TableModelListener
    virtual void modelUpdated(bool) override;

    void requestFocus();

protected:
    /** Frees all action listeners on inner widgets. */
    virtual void uninstallActionListeners();
    /** Installs all action listeners on inner widgets. */
    virtual void installActionListeners();

    virtual int getRowHeight() const A_WARN_UNUSED;
    virtual int getColumnWidth(int i) const A_WARN_UNUSED;

private:
    int getRowForY(int y) const A_WARN_UNUSED;     // -1 on error
    int getColumnForX(int x) const A_WARN_UNUSED;  // -1 on error
    void recomputeDimensions();
    bool mLinewiseMode;
    bool mWrappingEnabled;
    bool mOpaque;

    static float mAlpha;

    /**
     * Holds the background color of the table.
     */
    gcn::Color mBackgroundColor;

    TableModel *mModel;

    int mSelectedRow;
    int mSelectedColumn;

    /** If someone moves a fresh widget to the top, we must display it. */
    gcn::Widget *mTopWidget;

    /** Vector for compactness; used as a list in practice. */
    std::vector<GuiTableActionListener *> mActionListeners;

    gcn::Color mHighlightColor;
};

#endif  // TABLE_H
