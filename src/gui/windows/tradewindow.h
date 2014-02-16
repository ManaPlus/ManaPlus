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

#ifndef GUI_WINDOWS_TRADEWINDOW_H
#define GUI_WINDOWS_TRADEWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/selectionlistener.h"

#include <memory>

class Button;
class Inventory;
class Item;
class ItemContainer;
class Label;
class TextField;

/**
 * Trade dialog.
 *
 * \ingroup Interface
 */
class TradeWindow final : public Window,
                          private ActionListener,
                          private SelectionListener
{
    public:
        /**
         * Constructor.
         */
        TradeWindow();

        A_DELETE_COPY(TradeWindow)

        /**
         * Destructor.
         */
        ~TradeWindow();

        /**
         * Displays expected money in the trade window.
         */
        void setMoney(const int quantity);

        /**
         * Add an item to the trade window.
         */
        void addItem(const int id, const bool own, const int quantity,
                     const int refine, const unsigned char color) const;

        /**
         * Reset both item containers
         */
        void reset();

        /**
         * Add an item to the trade window.
         */
        void addItem2(const int id, const bool own, const int quantity,
                      const int refine, const unsigned char color,
                      const bool equipment) const;

        /**
         * Change quantity of an item.
         */
        void changeQuantity(const int index, const bool own,
                            const int quantity) const;

        /**
         * Increase quantity of an item.
         */
        void increaseQuantity(const int index, const bool own,
                              const int quantity) const;

        /**
         * Player received ok message from server
         */
        void receivedOk(const bool own);

        /**
         * Send trade packet.
         */
        void tradeItem(const Item *const item, const int quantity,
                       const bool check = false) const;

        /**
         * Updates the labels and makes sure only one item is selected in
         * either my inventory or partner inventory.
         */
        void valueChanged(const SelectionEvent &event) override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Closes the Trade Window, as well as telling the server that the
         * window has been closed.
         */
        void close() override final;

        /**
         * Clear auto trade items.
         */
        void clear();

        /**
         * Add item what will be added to trade.
         */
        void addAutoItem(const std::string &nick, Item *const item,
                         const int amount);

        void addAutoMoney(const std::string &nick, const int money);

        void initTrade(const std::string &nick);

        std::string getAutoTradeNick() const A_WARN_UNUSED
        { return mAutoAddToNick; }

        bool checkItem(const Item *const item) const A_WARN_UNUSED;

        bool isInpupFocused() const A_WARN_UNUSED;

    private:
        enum Status
        {
            PREPARING = 0, /**< Players are adding items. (1) */
            PROPOSING, /**< Local player has confirmed the trade. (1) */
            ACCEPTING, /**< Accepting the trade. (2) */
            ACCEPTED  /**< Local player has accepted the trade. */
        };

        /**
         * Sets the current status of the trade.
         */
        void setStatus(const Status s);

        typedef const std::auto_ptr<Inventory> InventoryPtr;
        InventoryPtr mMyInventory;
        InventoryPtr mPartnerInventory;

        ItemContainer *mMyItemContainer;
        ItemContainer *mPartnerItemContainer;

        Label *mMoneyLabel;
        Button *mAddButton;
        Button *mOkButton;
        Button  *mMoneyChangeButton;
        TextField *mMoneyField;

        Status mStatus;
        Item* mAutoAddItem;
        std::string mAutoAddToNick;
        int mGotMoney;
        int mGotMaxMoney;
        int mAutoMoney;
        int mAutoAddAmount;
        bool mOkOther;
        bool mOkMe;
};

extern TradeWindow *tradeWindow;

#endif  // GUI_WINDOWS_TRADEWINDOW_H
