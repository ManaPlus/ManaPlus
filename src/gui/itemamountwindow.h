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

#ifndef ITEM_AMOUNT_WINDOW_H
#define ITEM_AMOUNT_WINDOW_H

#include "gui/widgets/window.h"

#include <guichan/keylistener.hpp>
#include <guichan/actionlistener.hpp>

class Icon;
class IntTextField;
class Item;
class ItemsModal;
class ItemPopup;
class Label;

namespace gcn
{
    class DropDown;
    class Slider;
}

/**
 * Window used for selecting the amount of items to drop, trade or split.
 *
 * \ingroup Interface
 */
class ItemAmountWindow : public Window,
                         public gcn::ActionListener,
                         public gcn::KeyListener
{
    public:
        enum Usage
        {
            TradeAdd = 0,
            ItemDrop,
            StoreAdd,
            StoreRemove,
            ItemSplit,
            ShopBuyAdd,
            ShopSellAdd
        };

        /**
         * Called when receiving actions from widget.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Sets default amount value.
         */
        void resetAmount();

        // MouseListener
        void mouseMoved(gcn::MouseEvent &event);
        void mouseExited(gcn::MouseEvent &event);

        /**
         * Schedules the Item Amount window for deletion.
         */
        void close();

        void keyReleased(gcn::KeyEvent &keyEvent);

        /**
         * Creates the dialog, or bypass it if there aren't enough items.
         */
        static void showWindow(const Usage usage, Window *const parent,
                               Item *const item, int maxRange = 0);

        ~ItemAmountWindow();

    private:
        static void finish(Item *const item, const int amount,
                           const int price, const Usage usage);

        ItemAmountWindow(const Usage usage, Window *const parent,
                         Item *const item, const int maxRange = 0);

        IntTextField *mItemAmountTextField;   /**< Item amount caption. */
        IntTextField *mItemPriceTextField;   /**< Item price caption. */
        Label *mGPLabel;
        Item *mItem;
        Icon *mItemIcon;

        int mMax;
        Usage mUsage;
        ItemPopup *mItemPopup;

        /**
         * Item Amount buttons.
         */
        gcn::Slider *mItemAmountSlide;

        gcn::Slider *mItemPriceSlide;

        gcn::DropDown *mItemDropDown;

        ItemsModal *mItemsModal;

        bool mEnabledKeyboard;
        int mPrice;
};

#endif // ITEM_AMOUNT_WINDOW_H
