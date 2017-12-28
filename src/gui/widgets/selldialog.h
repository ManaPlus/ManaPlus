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

#ifndef GUI_WIDGETS_SELLDIALOG_H
#define GUI_WIDGETS_SELLDIALOG_H

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/advanced.h"
#include "enums/simpletypes/issell.h"
#include "enums/simpletypes/itemcolor.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/selectionlistener.h"

class Button;
class Item;
class Label;
class ScrollArea;
class ShopItem;
class ShopItems;
class ShopListBox;
class Slider;

/**
 * The sell dialog.
 *
 * \ingroup Interface
 */
class SellDialog notfinal : public Window,
                            public ActionListener,
                            private SelectionListener
{
    public:
        /**
         * Constructor.
         */
        SellDialog(const IsSell isSell,
                   const Advanced advanced);

        A_DELETE_COPY(SellDialog)

        /**
         * Destructor
         */
        ~SellDialog() override;

        /**
         * Resets the dialog, clearing inventory.
         */
        void reset();

        /**
         * Adds an item to the inventory.
         */
        void addItem(const Item *const item,
                     const int price);

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
        void setVisible(Visible visible) override final;

        ShopItem *addItem(const int id,
                          const ItemTypeT type,
                          const ItemColor color,
                          const int amount,
                          const int price);

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !instances.empty(); }

        /**
         * Closes all instances.
         */
        static void closeAll();

        void postInit() override;

    protected:
        typedef std::list<SellDialog*> DialogList;
        static DialogList instances;

        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        virtual void sellAction(const ActionEvent &event) = 0;

        virtual void initButtons()
        { }

        Button *mSellButton A_NONNULLPOINTER;
        Button *mQuitButton A_NONNULLPOINTER;
        Button *mConfirmButton A_NONNULLPOINTER;
        Button *mAddMaxButton;
        Button *mIncreaseButton;
        Button *mDecreaseButton;
        ShopListBox *mShopItemList A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        Label *mMoneyLabel;
        Label *mQuantityLabel;
        Slider *mSlider;
        ShopItems *mShopItems A_NONNULLPOINTER;

        int mPlayerMoney;
        int mMaxItems;
        int mAmountItems;

        IsSell mIsSell;
        Advanced mAdvanced;
};

#endif  // GUI_WIDGETS_SELLDIALOG_H
