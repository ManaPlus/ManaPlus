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

#ifndef TRADEWINDOW_H
#define TRADEWINDOW_H

#include "guichanfwd.h"

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/selectionlistener.hpp>

#include <memory>

class Inventory;
class Item;
class ItemContainer;
class ScrollArea;

/**
 * Trade dialog.
 *
 * \ingroup Interface
 */
class TradeWindow : public Window, gcn::ActionListener, gcn::SelectionListener
{
    public:
        /**
         * Constructor.
         */
        TradeWindow();

        /**
         * Destructor.
         */
        ~TradeWindow();

        /**
         * Displays expected money in the trade window.
         */
        void setMoney(int quantity);

        /**
         * Add an item to the trade window.
         */
        void addItem(int id, bool own, int quantity,
                     int refine, unsigned char color);

        /**
         * Reset both item containers
         */
        void reset();

        /**
         * Add an item to the trade window.
         */
        void addItem2(int id, bool own, int quantity, int refine,
                      unsigned char color, bool equipment);

        /**
         * Change quantity of an item.
         */
        void changeQuantity(int index, bool own, int quantity);

        /**
         * Increase quantity of an item.
         */
        void increaseQuantity(int index, bool own, int quantity);

        /**
         * Player received ok message from server
         */
        void receivedOk(bool own);

        /**
         * Send trade packet.
         */
        void tradeItem(Item *item, int quantity, bool check = false);

        /**
         * Updates the labels and makes sure only one item is selected in
         * either my inventory or partner inventory.
         */
        void valueChanged(const gcn::SelectionEvent &event);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Closes the Trade Window, as well as telling the server that the
         * window has been closed.
         */
        void close();

        /**
         * Clear auto trade items.
         */
        void clear();

        /**
         * Add item what will be added to trade.
         */
        void addAutoItem(std::string nick, Item* item, int amount);

        void addAutoMoney(std::string nick, int money);

        void initTrade(std::string nick);

        std::string getAutoTradeNick() const
        { return mAutoAddToNick; }

        bool checkItem(Item *item);

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
        void setStatus(Status s);

        typedef const std::auto_ptr<Inventory> InventoryPtr;
        InventoryPtr mMyInventory;
        InventoryPtr mPartnerInventory;

        ItemContainer *mMyItemContainer;
        ItemContainer *mPartnerItemContainer;

        gcn::Label *mMoneyLabel;
        gcn::Button *mAddButton;
        gcn::Button *mOkButton;
        gcn::Button  *mMoneyChangeButton;
        gcn::TextField *mMoneyField;

        Status mStatus;
        bool mOkOther, mOkMe;
        Item* mAutoAddItem;
        std::string mAutoAddToNick;
        int mGotMoney;
        int mGotMaxMoney;
        int mAutoMoney;
        int mAutoAddAmount;
};

extern TradeWindow *tradeWindow;

#endif
