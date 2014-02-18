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

#ifndef GUI_WINDOWS_BUYDIALOG_H
#define GUI_WINDOWS_BUYDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/selectionlistener.h"

class Button;
class DropDown;
class ShopItems;
class ShopListBox;
class SortListModelBuy;
class IntTextField;
class Label;
class ScrollArea;
class Slider;

/**
 * The buy dialog.
 *
 * \ingroup Interface
 */
class BuyDialog final : public Window,
                        public ActionListener,
                        public SelectionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        BuyDialog();

        /**
         * Constructor.
         *
         * @see Window::Window
         */
        explicit BuyDialog(const int npcId);

        /**
         * Constructor.
         *
         * @see Window::Window
         */
        explicit BuyDialog(std::string nick);

        A_DELETE_COPY(BuyDialog)

        /**
         * Destructor
         */
        ~BuyDialog();

        void init();

        /**
         * Resets the dialog, clearing shop inventory.
         */
        void reset();

        /**
         * Sets the amount of available money.
         */
        void setMoney(const int amount);

        /**
         * Adds an item to the shop inventory.
         */
        void addItem(const int id, const unsigned char color,
                     const int amount, const int price);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Returns the number of items in the shop inventory.
         */
        int getNumberOfElements() A_WARN_UNUSED;

        /**
         * Updates the labels according to the selected item.
         */
        void valueChanged(const SelectionEvent &event) override final;

        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        /**
         * Sets the visibility of this window.
         */
        void setVisible(bool visible);

        void sort();

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
        typedef std::list<BuyDialog*> DialogList;
        static DialogList instances;

        int mNpcId;

        Button *mBuyButton;
        Button *mQuitButton;
        Button *mAddMaxButton;
        Button *mIncreaseButton;
        Button *mDecreaseButton;
        ShopListBox *mShopItemList;
        ScrollArea *mScrollArea;
        Label *mMoneyLabel;
        Label *mQuantityLabel;
        Slider *mSlider;
        Label *mAmountLabel;
        IntTextField *mAmountField;

        ShopItems *mShopItems;

        int mMoney;
        int mAmountItems;
        int mMaxItems;
        std::string mNick;
        SortListModelBuy *mSortModel;
        DropDown *mSortDropDown;
};

#endif  // GUI_WINDOWS_BUYDIALOG_H
