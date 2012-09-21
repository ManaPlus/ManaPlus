/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include "inventory.h"
#include "listener.h"

#include "gui/widgets/window.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/selectionlistener.hpp>

class DropDown;
class Item;
class ItemContainer;
class TabStrip;
class LayoutCell;
class ProgressBar;
class SortListModel;
class TextField;

namespace gcn
{
    class Button;
    class Label;
}

/**
 * Inventory dialog.
 *
 * \ingroup Interface
 */
class InventoryWindow final : public Window,
                              public gcn::ActionListener,
                              public gcn::KeyListener,
                              public gcn::SelectionListener,
                              public InventoryListener,
                              public Listener
{
    public:
        /**
         * Constructor.
         */
        InventoryWindow(Inventory *const inventory);

        /**
         * Destructor.
         */
        ~InventoryWindow();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override;

        /**
         * Returns the selected item.
         */
        Item* getSelectedItem() const;

        /**
         * Unselect item
         */
        void unselectItem();

        /**
         * Handles closing of the window
         */
        void widgetHidden(const gcn::Event &event) override;

        /**
         * Handles the mouse clicks.
         */
        void mouseClicked(gcn::MouseEvent &event) override;

        /**
         * Handles the key presses.
         */
        void keyPressed(gcn::KeyEvent &event) override;

        /**
         * Handles the key releases.
         */
        void keyReleased(gcn::KeyEvent &event) override;

        /**
         * Updates labels to currently selected item.
         */
        void valueChanged(const gcn::SelectionEvent &event) override;

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

        bool isMainInventory() const
        { return mInventory->isMainInventory(); }

        /**
         * Returns true if any instances exist.
         */
        static bool isStorageActive()
        { return instances.size() > 1; }

        void updateDropButton();

        void processEvent(Channels channel,
                          const DepricatedEvent &event) override;

        void updateButtons(const Item *item = nullptr);

        bool isInputFocused() const;

        void widgetResized(const gcn::Event &event) override;

        static bool isAnyInputFocused();

    private:
        /**
         * Updates the weight bar.
         */
        void updateWeight();


        typedef std::list<InventoryWindow*> WindowList;
        static WindowList instances;

        Inventory *mInventory;
        ItemContainer *mItems;

        std::string mWeight, mSlots;

        gcn::Button *mUseButton;
        gcn::Button *mDropButton;
        gcn::Button *mSplitButton;
        gcn::Button *mOutfitButton;
        gcn::Button *mShopButton;
        gcn::Button *mEquipmentButton;
        gcn::Button *mStoreButton;
        gcn::Button *mRetrieveButton;
        gcn::Button *mCloseButton;

        ProgressBar *mWeightBar;
        ProgressBar *mSlotsBar;
        TabStrip *mFilter;
        SortListModel *mSortModel;
        DropDown *mSortDropDown;
        TextField *mNameFilter;
        LayoutCell *mSortDropDownCell;
        LayoutCell *mNameFilterCell;
        LayoutCell *mFilterCell;
        LayoutCell *mSlotsBarCell;

        bool mSplit;
        bool mCompactMode;
};

extern InventoryWindow *inventoryWindow;

#endif
