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

#ifndef GUI_WINDOWS_SELLDIALOG_H
#define GUI_WINDOWS_SELLDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/selectionlistener.h"

class Button;
class Item;
class Label;
class ScrollArea;
class ShopItems;
class ShopListBox;
class Slider;

/**
 * The sell dialog.
 *
 * \ingroup Interface
 */
class SellDialog final : public Window,
                         private ActionListener,
                         private SelectionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        explicit SellDialog(const int npcId);

        /**
         * Constructor.
         */
        explicit SellDialog(const std::string &nick);

        A_DELETE_COPY(SellDialog)

        /**
         * Destructor
         */
        ~SellDialog();

        void init();

        /**
         * Resets the dialog, clearing inventory.
         */
        void reset();

        /**
         * Adds an item to the inventory.
         */
        void addItem(const Item *const item, const int price);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Updates labels according to selected item.
         *
         * @see SelectionListener::selectionChanged
         */
        void valueChanged(const SelectionEvent &event) override final;

        /**
         * Gives Player's Money amount
         */
        void setMoney(const int amount);

        /**
         * Sets the visibility of this window.
         */
        void setVisible(bool visible) override final;

        void addItem(const int id, const unsigned char color,
                     const int amount, const int price);

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !instances.empty(); }

        /**
         * Closes all instances.
         */
        static void closeAll();

    private:
        typedef std::list<SellDialog*> DialogList;
        static DialogList instances;

        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        std::string mNick;

        Button *mSellButton;
        Button *mQuitButton;
        Button *mAddMaxButton;
        Button *mIncreaseButton;
        Button *mDecreaseButton;
        ShopListBox *mShopItemList;
        ScrollArea *mScrollArea;
        Label *mMoneyLabel;
        Label *mQuantityLabel;
        Slider *mSlider;
        ShopItems *mShopItems;

        int mNpcId;
        int mPlayerMoney;
        int mMaxItems;
        int mAmountItems;
};

#endif  // GUI_WINDOWS_SELLDIALOG_H
