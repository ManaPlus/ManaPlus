/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_INVENTORYWINDOW_H
#define GUI_WINDOWS_INVENTORYWINDOW_H

#include "resources/inventory/inventory.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/attributelistener.h"
#include "listeners/inventorylistener.h"
#include "listeners/keylistener.h"
#include "listeners/selectionlistener.h"

class Button;
class DropDown;
class InventoryWindow;
class Item;
class ItemContainer;
class LayoutCell;
class ProgressBar;
class SortListModelInv;
class TabStrip;
class TextField;

extern InventoryWindow *inventoryWindow;
extern InventoryWindow *storageWindow;
extern InventoryWindow *cartWindow;

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
                              public AttributeListener
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
        ~InventoryWindow() override final;

        void postInit() override final;

        void storeSortOrder() const;

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
         * Closes the Storage Window, as well as telling the server that the
         * window has been closed.
         */
        void close() override final;

        void slotsChanged(const Inventory *const inventory) override final;

        bool isMainInventory() const A_WARN_UNUSED
        {
            return mInventory != nullptr ?
                mInventory->isMainInventory() : false;
        }

        /**
         * Returns true if any instances exist.
         */
        static bool isStorageActive() A_WARN_UNUSED
        { return storageWindow != nullptr; }

        void updateDropButton();

        void updateButtons(const Item *item);

        bool isInputFocused() const A_WARN_UNUSED;

        void widgetResized(const Event &event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseExited(MouseEvent &event) override final;

        void setVisible(Visible visible) override final;

        void unsetInventory();

        void attributeChanged(const AttributesT id,
                              const int64_t oldVal,
                              const int64_t newVal) override final;

        void combineItems(const int index1,
                          const int index2);

        void moveItemToCraft(const int craftSlot);

        static bool isAnyInputFocused();

        static InventoryWindow *getFirstVisible();

        static void nextTab();

        static void prevTab();

    private:
        /**
         * Updates the weight bar.
         */
        void updateWeight();

        typedef std::list<InventoryWindow*> WindowList;
        static WindowList invInstances;

        Inventory *mInventory;
        ItemContainer *mItems A_NONNULLPOINTER;

        Button *mUseButton;
        Button *mDropButton;
        Button *mOutfitButton;
        Button *mShopButton;
        Button *mCartButton;
        Button *mEquipmentButton;
        Button *mStoreButton;
        Button *mRetrieveButton;
        Button *mInvCloseButton;

        ProgressBar *mWeightBar;
        ProgressBar *mSlotsBar A_NONNULLPOINTER;
        TabStrip *mFilter;
        SortListModelInv *mSortModel A_NONNULLPOINTER;
        DropDown *mSortDropDown A_NONNULLPOINTER;
        TextField *mNameFilter A_NONNULLPOINTER;
        LayoutCell *mSortDropDownCell;
        LayoutCell *mNameFilterCell;
        LayoutCell *mFilterCell;
        LayoutCell *mSlotsBarCell;

        bool mSplit;
        bool mCompactMode;
};

#endif  // GUI_WINDOWS_INVENTORYWINDOW_H
