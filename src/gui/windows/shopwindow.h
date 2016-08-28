/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_SHOPWINDOW_H
#define GUI_WINDOWS_SHOPWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/buyingstoremodelistener.h"
#include "listeners/buyingstoreslotslistener.h"
#include "listeners/selectionlistener.h"
#include "listeners/vendingmodelistener.h"
#include "listeners/vendingslotslistener.h"

class Button;
class CheckBox;
class Item;
class ScrollArea;
class ShopItem;
class ShopItems;
class ShopListBox;
class TabStrip;

/**
 * The buy dialog.
 *
 * \ingroup Interface
 */
class ShopWindow final : public Window,
                         public VendingModeListener,
                         public VendingSlotsListener,
                         public BuyingStoreModeListener,
                         public BuyingStoreSlotsListener,
                         public ActionListener,
                         public SelectionListener
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

        A_DELETE_COPY(ShopWindow)

        /**
         * Destructor
         */
        ~ShopWindow();

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

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

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !instances.empty(); }

        void setItemSelected(const int id)
        { mSelectedItem = id; updateButtonsAndLabels(); }

        void addBuyItem(const Item *const item, const int amount,
                        const int price);

        void addSellItem(const Item *const item, const int amount,
                         const int price);

        void loadList();

        void saveList() const;

#ifdef TMWA_SUPPORT
        void setAcceptPlayer(const std::string &name)
        { mAcceptPlayer = name; }

        const std::string &getAcceptPlayer() const A_WARN_UNUSED
        { return mAcceptPlayer; }

        void announce(ShopItems *const list, const int mode);

        void giveList(const std::string &nick, const int mode);

        void sendMessage(const std::string &nick, std::string data,
                         const bool random = false);

        static void showList(const std::string &nick, std::string data);

        void processRequest(const std::string &nick, std::string data,
                            const int mode);
        void updateTimes();

        static bool checkFloodCounter(time_t &counterTime) A_WARN_UNUSED;

        bool findShopItem(const ShopItem *const shopItem,
                          const int mode) const A_WARN_UNUSED;

#endif
        static int sumAmount(const Item *const shopItem) A_WARN_UNUSED;

        bool isShopEmpty() const A_WARN_UNUSED;

        void vendingEnabled(const bool b) override final;

        void vendingSlotsChanged(const int slots) override final;

        void buyingStoreEnabled(const bool b) override final;

        void buyingStoreSlotsChanged(const int slots) override final;

        void setShopName(const std::string &name);

    private:
#ifdef TMWA_SUPPORT
        void startTrade();
#endif
        void updateSelection();

        void updateShopName();

        typedef std::list<ShopWindow*> DialogList;
        static DialogList instances;

        Button *mCloseButton A_NONNULLPOINTER;

        ShopItems *mBuyShopItems A_NONNULLPOINTER;
        ShopItems *mSellShopItems A_NONNULLPOINTER;
        ShopItem *mTradeItem;

        ShopListBox *mBuyShopItemList A_NONNULLPOINTER;
        ShopListBox *mSellShopItemList A_NONNULLPOINTER;
        ShopListBox *mCurrentShopItemList;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        Button *mAddButton A_NONNULLPOINTER;
        Button *mDeleteButton A_NONNULLPOINTER;
        Button *mAnnounceButton;
        Button *mPublishButton;
        Button *mRenameButton;
        CheckBox *mAnnounceLinks;
        TabStrip *mTabs;
        std::string mAcceptPlayer;
        std::string mTradeNick;
        std::string mSellShopName;
        int mSelectedItem;
        time_t mAnnonceTime;
        time_t mLastRequestTimeList;
        time_t mLastRequestTimeItem;
        int mRandCounter;
        int mTradeMoney;
        int mSellShopSize;
        int mBuyShopSize;
        bool isBuySelected;
        bool mHaveVending;
        bool mEnableBuyingStore;
        bool mEnableVending;
};

extern ShopWindow *shopWindow;

#endif  // GUI_WINDOWS_SHOPWINDOW_H
