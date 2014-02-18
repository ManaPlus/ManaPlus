/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_INVENTORYWINDOW_H
#define GUI_WINDOWS_INVENTORYWINDOW_H

#include "inventory.h"

#include "listeners/depricatedlistener.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"
#include "listeners/selectionlistener.h"

class Button;
class DropDown;
class Item;
class ItemContainer;
class LayoutCell;
class ProgressBar;
class SortListModelInv;
class TabStrip;
class TextField;
class TextPopup;

/**
 * Inventory dialog.
 *
 * \ingroup Interface
 */
class InventoryWindow final : public Window,
                              public ActionListener,
                              public KeyListener,
                              public SelectionListener,
                              public InventoryListener,
                              public DepricatedListener
{
    public:
        /**
         * Constructor.
         */
        explicit InventoryWindow(Inventory *const inventory);

        A_DELETE_COPY(InventoryWindow)

        /**
         * Destructor.
         */
        ~InventoryWindow();

        void postInit() override final;

        void storeSortOrder();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Returns the selected item.
         */
        Item* getSelectedItem() const A_WARN_UNUSED;

        /**
         * Unselect item
         */
        void unselectItem();

        /**
         * Handles closing of the window
         */
        void widgetHidden(const Event &event) override final;

        /**
         * Handles the mouse clicks.
         */
        void mouseClicked(MouseEvent &event) override final;

        /**
         * Handles the key presses.
         */
        void keyPressed(KeyEvent &event) override final;

        /**
         * Handles the key releases.
         */
        void keyReleased(KeyEvent &event) override final;

        /**
         * Updates labels to currently selected item.
         */
        void valueChanged(const SelectionEvent &event) override final;

        /**
         * Sets whether the split button should be shown.
         */
        void setSplitAllowed(const bool allowed);

        /**
         * Closes the Storage Window, as well as telling the server that the
         * window has been closed.
         */
        void close();

        void slotsChanged(Inventory *const inventory);

        bool isMainInventory() const A_WARN_UNUSED
        { return mInventory->isMainInventory(); }

        /**
         * Returns true if any instances exist.
         */
        static bool isStorageActive() A_WARN_UNUSED
        { return invInstances.size() > 1; }

        void updateDropButton();

        void processEvent(const Channels channel,
                          const DepricatedEvent &event) override final;

        void updateButtons(const Item *item = nullptr);

        bool isInputFocused() const A_WARN_UNUSED;

        void widgetResized(const Event &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void setVisible(bool visible) override final;

        static bool isAnyInputFocused();

    private:
        /**
         * Updates the weight bar.
         */
        void updateWeight();

        typedef std::list<InventoryWindow*> WindowList;
        static WindowList invInstances;

        Inventory *mInventory;
        ItemContainer *mItems;

        Button *mUseButton;
        Button *mDropButton;
        Button *mSplitButton;
        Button *mOutfitButton;
        Button *mShopButton;
        Button *mEquipmentButton;
        Button *mStoreButton;
        Button *mRetrieveButton;
        Button *mInvCloseButton;

        ProgressBar *mWeightBar;
        ProgressBar *mSlotsBar;
        TabStrip *mFilter;
        SortListModelInv *mSortModel;
        DropDown *mSortDropDown;
        TextField *mNameFilter;
        LayoutCell *mSortDropDownCell;
        LayoutCell *mNameFilterCell;
        LayoutCell *mFilterCell;
        LayoutCell *mSlotsBarCell;
        TextPopup *mTextPopup;

        bool mSplit;
        bool mCompactMode;
};

extern InventoryWindow *inventoryWindow;

#endif  // GUI_WINDOWS_INVENTORYWINDOW_H
