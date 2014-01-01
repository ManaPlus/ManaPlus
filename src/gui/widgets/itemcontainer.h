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

#ifndef GUI_WIDGETS_ITEMCONTAINER_H
#define GUI_WIDGETS_ITEMCONTAINER_H

#include "gui/widgets/widget2.h"

#include <guichan/keylistener.hpp>
#include <guichan/mouselistener.hpp>
#include <guichan/widget.hpp>
#include <guichan/widgetlistener.hpp>

#include <list>

#include "localconsts.h"

class Image;
class Inventory;
class Item;
class ItemPopup;

namespace gcn
{
    class SelectionListener;
}

/**
 * An item container. Used to show items in inventory and trade dialog.
 *
 * \ingroup GUI
 */
class ItemContainer final : public gcn::Widget,
                            public Widget2,
                            public gcn::KeyListener,
                            public gcn::MouseListener,
                            public gcn::WidgetListener
{
    public:
        /**
         * Constructor. Initializes the graphic.
         *
         * @param inventory
         * @param gridColumns Amount of columns in grid.
         * @param gridRows    Amount of rows in grid.
         * @param offset      Index offset
         */
        ItemContainer(const Widget2 *const widget,
                      Inventory *const inventory,
                      const bool forceQuantity = false);

        A_DELETE_COPY(ItemContainer)

        /**
         * Destructor.
         */
        ~ItemContainer();

        void hidePopup();

        /**
         * Necessary for checking how full the inventory is.
         */
        void logic() override final;

        /**
         * Draws the items.
         */
        void draw(gcn::Graphics *graphics) override final;

        // KeyListener
        void keyPressed(gcn::KeyEvent &event) override final;
        void keyReleased(gcn::KeyEvent &event) override final;

        // MouseListener
        void mousePressed(gcn::MouseEvent &event) override final;
        void mouseDragged(gcn::MouseEvent &event) override final;
        void mouseReleased(gcn::MouseEvent &event) override final;
        void mouseMoved(gcn::MouseEvent &event) override final;
        void mouseExited(gcn::MouseEvent &event) override final;

        // WidgetListener
        void widgetResized(const gcn::Event &event) override final;

        /**
         * Returns the selected item.
         */
        Item *getSelectedItem() const A_WARN_UNUSED;

        /**
         * Sets selected item to NULL.
         */
        void selectNone();

        /**
         * Adds a listener to the list that's notified each time a change to
         * the selection occurs.
         */
        void addSelectionListener(gcn::SelectionListener *listener)
        { mSelectionListeners.push_back(listener); }

        /**
         * Removes a listener from the list that's notified each time a change
         * to the selection occurs.
         */
        void removeSelectionListener(gcn::SelectionListener *listener)
        { mSelectionListeners.remove(listener); }

        void setFilter(const int tag);

        void setSortType(const int sortType);

        void setName(std::string str)
        { mName = str; }

        void updateMatrix();

        bool getClickCount() A_WARN_UNUSED
        { return mClicks; }

    private:
        enum Direction
        {
            Left = 0,
            Right,
            Up,
            Down
        };

        enum SelectionState
        {
            SEL_NONE = 0,
            SEL_SELECTED,
            SEL_SELECTING,
            SEL_DESELECTING,
            SEL_DRAGGING
        };

        /**
         * Sets the currently selected item.
         */
        void setSelectedIndex(const int index);

        /**
         * Determine and set the height of the container.
         */
        void adjustHeight();

        /**
         * Sends out selection events to the list of selection listeners.
         */
        void distributeValueChangedEvent();

        /**
         * Gets the slot index based on the cursor position.
         *
         * @param x The X coordinate position.
         * @param y The Y coordinate position.
         * @return The slot index on success, -1 on failure.
         */
        int getSlotIndex(const int x, const int y) const;

        Inventory *mInventory;
        Image *mSelImg;
        Image *mProtectedImg;
        std::string mName;

        ItemPopup *mItemPopup;
        int *mShowMatrix;
        Skin *mSkin;
        gcn::Color mEquipedColor;
        gcn::Color mEquipedColor2;
        gcn::Color mUnEquipedColor;
        gcn::Color mUnEquipedColor2;
        typedef std::list<gcn::SelectionListener*> SelectionListenerList;
        typedef SelectionListenerList::iterator SelectionListenerIterator;
        SelectionListenerList mSelectionListeners;
        int mGridColumns;
        int mGridRows;
        int mSelectedIndex;
        int mLastUsedSlot;
        int mTag;
        int mSortType;
        int mClicks;
        int mBoxWidth;
        int mBoxHeight;
        int mEquippedTextPadding;
        int mPaddingItemX;
        int mPaddingItemY;
        SelectionState mSelectionStatus;
        bool mForceQuantity;
        bool mDescItems;
};

#endif  // GUI_WIDGETS_ITEMCONTAINER_H
