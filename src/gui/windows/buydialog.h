/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_BUYDIALOG_H
#define GUI_WINDOWS_BUYDIALOG_H

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/beingid.h"
#include "enums/simpletypes/itemcolor.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/selectionlistener.h"

class Being;
class Button;
class DropDown;
class ShopItem;
class ShopItems;
class ShopListBox;
class SortListModelBuy;
class IntTextField;
class Label;
class ScrollArea;
class Slider;
class TextField;

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
        BuyDialog(const BeingId npcId,
                  const std::string &currency);

#ifdef TMWA_SUPPORT
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        BuyDialog(const std::string &nick,
                  const std::string &currency);
#endif  // TMWA_SUPPORT

        /**
         * Constructor.
         *
         * @see Window::Window
         */
        BuyDialog(const Being *const being,
                  const std::string &currency);

        A_DELETE_COPY(BuyDialog)

        /**
         * Destructor
         */
        ~BuyDialog();

        enum
        {
#ifdef TMWA_SUPPORT
            Nick    = -1,
#endif  // TMWA_SUPPORT
            Items   = -2,
            Market  = -3,
            Cash    = -4,
            Vending = -5
        };

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
        ShopItem *addItem(const int id,
                          const ItemTypeT type,
                          const ItemColor color,
                          const int amount,
                          const int price);

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
        void setVisible(Visible visible) override final;

        void sort();

        void close() override final;

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
        void updateSlider(const int selectedItem);

        void applyNameFilter(const std::string &filter);

        typedef std::list<BuyDialog*> DialogList;
        static DialogList instances;

        Button *mBuyButton A_NONNULLPOINTER;
        Button *mConfirmButton A_NONNULLPOINTER;
        Button *mQuitButton A_NONNULLPOINTER;
        Button *mAddMaxButton A_NONNULLPOINTER;
        Button *mIncreaseButton A_NONNULLPOINTER;
        Button *mDecreaseButton A_NONNULLPOINTER;
        ShopListBox *mShopItemList A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        Label *mMoneyLabel A_NONNULLPOINTER;
        Label *mQuantityLabel A_NONNULLPOINTER;
        Slider *mSlider A_NONNULLPOINTER;
        Label *mAmountLabel A_NONNULLPOINTER;
        IntTextField *mAmountField A_NONNULLPOINTER;
        ShopItems *mShopItems A_NONNULLPOINTER;
        SortListModelBuy *mSortModel;
        DropDown *mSortDropDown;
        TextField *mFilterTextField A_NONNULLPOINTER;
        Label *mFilterLabel;

        std::string mNick;
        std::string mCurrency;
        BeingId mNpcId;
        int mMoney;
        int mAmountItems;
        int mMaxItems;
        bool mAdvanced;
};

#endif  // GUI_WINDOWS_BUYDIALOG_H
