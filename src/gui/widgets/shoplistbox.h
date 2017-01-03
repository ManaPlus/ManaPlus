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

#ifndef GUI_WIDGETS_SHOPLISTBOX_H
#define GUI_WIDGETS_SHOPLISTBOX_H

#include "gui/widgets/listbox.h"

#include "enums/gui/shoplistboxtype.h"

class ShopItems;

/**
 * A list box, meant to be used inside a scroll area. Same as the Guichan list
 * box except this one doesn't have a background, instead completely relying
 * on the scroll area. It also adds selection listener functionality.
 *
 * \ingroup GUI
 */
class ShopListBox final : public ListBox
{
    public:
        /**
         * Constructor.
         */
        ShopListBox(const Widget2 *const widget,
                    ListModel *const listModel,
                    const ShopListBoxTypeT type);

        /**
         * Constructor with shopitems
         */
        ShopListBox(const Widget2 *const widget,
                    ListModel *const listModel,
                    ShopItems *const shopListModel,
                    const ShopListBoxTypeT type);

        A_DELETE_COPY(ShopListBox)

        /**
         * Draws the list box.
         */
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        /**
         * gives information about the current player's money
         */
        void setPlayersMoney(const int money);

        /**
         * Adjust List draw size
         */
        void adjustSize() override final;

        /**
         * Set on/off the disabling of too expensive items.
         * (Good for selling mode.)
         */
        void setPriceCheck(const bool check);

        void mouseMoved(MouseEvent &event) override final;

        void mouseReleased(MouseEvent& event) override final;

        void mouseExited(MouseEvent& event) override final;

        void setProtectItems(bool p)
        { mProtectItems = p; }

        void setType(const ShopListBoxTypeT type)
        { mType = type; }

        ShopListBoxTypeT getType() const
        { return mType; }

    private:
        int mPlayerMoney;

        /**
         * Keeps another pointer to the same listModel, permitting to
         * use the ShopItems specific functions.
         */
        ShopItems *mShopItems;

        Color mBackgroundColor;
        Color mWarningColor;
        ShopListBoxTypeT mType;

        bool mPriceCheck;
        bool mProtectItems;
};

#endif  // GUI_WIDGETS_SHOPLISTBOX_H
