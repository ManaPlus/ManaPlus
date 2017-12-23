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

#ifndef GUI_WINDOWS_ITEMAMOUNTWINDOW_H
#define GUI_WINDOWS_ITEMAMOUNTWINDOW_H

#include "gui/widgets/window.h"

#include "enums/gui/itemamountwindowusage.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class DropDown;
class Icon;
class IntTextField;
class Item;
class ItemsModal;
class Label;
class Slider;

/**
 * Window used for selecting the amount of items to drop, trade or split.
 *
 * \ingroup Interface
 */
class ItemAmountWindow final : public Window,
                               public ActionListener,
                               public KeyListener
{
    public:
        A_DELETE_COPY(ItemAmountWindow)

        void postInit() override final;

        /**
         * Called when receiving actions from widget.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Sets default amount value.
         */
        void resetAmount();

        // MouseListener
        void mouseMoved(MouseEvent &event) override final;

        void mouseExited(MouseEvent &event) override final;

        /**
         * Schedules the Item Amount window for deletion.
         */
        void close() override final;

        void keyReleased(KeyEvent &event) override final;

        /**
         * Creates the dialog, or bypass it if there aren't enough items.
         */
        static void showWindow(const ItemAmountWindowUsageT usage,
                               Window *const parent,
                               Item *const item,
                               int maxRange,
                               const int tag);

        ~ItemAmountWindow();

#ifndef UNITTESTS
    private:
#endif  // UNITTESTS
        static void finish(Item *const item,
                           const int amount,
                           const int price,
                           const ItemAmountWindowUsageT usage);

        ItemAmountWindow(const ItemAmountWindowUsageT usage,
                         Window *const parent,
                         Item *const item,
                         const int maxRange);

        /**< Item amount caption. */
        IntTextField *mItemAmountTextField A_NONNULLPOINTER;
        /**< Item price caption. */
        IntTextField *mItemPriceTextField;
        Label *mGPLabel;
        Item *mItem;
        Icon *mItemIcon A_NONNULLPOINTER;

        /**
         * Item Amount buttons.
         */
        Slider *mItemAmountSlide A_NONNULLPOINTER;
        Slider *mItemPriceSlide;
        DropDown *mItemDropDown;
        ItemsModal *mItemsModal;

        int mPrice;
        int mMax;
        ItemAmountWindowUsageT mUsage;
        bool mEnabledKeyboard;
};

#endif  // GUI_WINDOWS_ITEMAMOUNTWINDOW_H
