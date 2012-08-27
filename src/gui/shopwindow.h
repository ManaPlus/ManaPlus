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

#ifndef SHOP_H
#define SHOP_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/selectionlistener.hpp>

class CheckBox;
class Item;
class ListBox;
class ShopItem;
class ShopItems;
class ShopListBox;

namespace gcn
{
    class Button;
    class CheckBox;
    class Label;
    class ScrollArea;
}

/**
 * The buy dialog.
 *
 * \ingroup Interface
 */
class ShopWindow : public Window, public gcn::ActionListener,
                  public gcn::SelectionListener
{
    public:

        enum ShopMode
        {
            BUY = 0,
            SELL = 1
        };

        /**
         * Constructor.
         *
         * @see Window::Window
         */
        ShopWindow();

        /**
         * Destructor
         */
        ~ShopWindow();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Updates the labels according to the selected item.
         */
        void valueChanged(const gcn::SelectionEvent &event);

        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        /**
         * Sets the visibility of this window.
         */
        void setVisible(bool visible);

        /**
         * Returns true if any instances exist.
         */
        static bool isActive()
        { return !instances.empty(); }

        void setItemSelected(const int id)
        { mSelectedItem = id; updateButtonsAndLabels(); }

        void addBuyItem(const Item *const item, const int amount,
                        const int price);

        void addSellItem(const Item *const item, const int amount,
                         const int price);

        void loadList();

        void saveList();

        void announce(ShopItems *const list, const int mode);

        void giveList(const std::string &nick, const int mode);

        void setAcceptPlayer(std::string name)
        { mAcceptPlayer = name; }

        const std::string &getAcceptPlayer() const
        { return mAcceptPlayer; }

        void sendMessage(const std::string &nick, std::string data,
                         const bool random = false);

        void showList(const std::string &nick, std::string data) const;

        void processRequest(std::string nick, std::string data,
                            const int mode);

        bool findShopItem(const ShopItem *const shopItem, const int mode);

        static int sumAmount(const Item *const shopItem);

        void updateTimes();

        bool checkFloodCounter(int &counterTime) const;

        bool isShopEmpty();

    private:
        void startTrade();

        typedef std::list<ShopWindow*> DialogList;
        static DialogList instances;

        gcn::Button *mCloseButton;
        ShopListBox *mBuyShopItemList;
        ShopListBox *mSellShopItemList;
        gcn::ScrollArea *mBuyScrollArea;
        gcn::ScrollArea *mSellScrollArea;
        gcn::Label *mBuyLabel;
        gcn::Label *mSellLabel;
        gcn::Button *mBuyAddButton;
        gcn::Button *mBuyDeleteButton;
        gcn::Button *mBuyAnnounceButton;
        gcn::Button *mBuyAuctionButton;
        gcn::Button *mSellAddButton;
        gcn::Button *mSellDeleteButton;
        gcn::Button *mSellAnnounceButton;
        gcn::Button *mSellAuctionButton;
        gcn::CheckBox *mAnnounceLinks;

        ShopItems *mBuyShopItems;
        ShopItems *mSellShopItems;

        int mSelectedItem;
        int mAnnonceTime;
        int mLastRequestTimeList;
        int mLastRequestTimeItem;
        int mRandCounter;
        std::string mAcceptPlayer;
        ShopItem *mTradeItem;
        std::string mTradeNick;
        int mTradeMoney;
        int mAnnounceCounter[2];
};

extern ShopWindow *shopWindow;

#endif
