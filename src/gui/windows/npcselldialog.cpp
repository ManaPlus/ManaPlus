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

#include "gui/windows/npcselldialog.h"

#include "being/playerinfo.h"

#include "gui/windows/confirmdialog.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/slider.h"

#include "net/buysellhandler.h"
#include "net/net.h"
#include "net/npchandler.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

NpcSellDialog::NpcSellDialog(const BeingId npcId) :
    SellDialog(IsSell_true,
        (Net::getNetworkType() != ServerType::TMWATHENA) ?
        Advanced_true : Advanced_false),
    mNpcId(npcId)
{
}

void NpcSellDialog::sellAction(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    const int selectedItem = mShopItemList->getSelected();
    const ShopItem *const item = mShopItems->at(selectedItem);
    if ((item == nullptr) || PlayerInfo::isItemProtected(item->getId()))
        return;

    if (eventId == "presell")
    {
        if (mAmountItems <= 0 || mAmountItems > mMaxItems)
            return;

        const ItemInfo &info = ItemDB::get(item->getId());
        if (info.isProtected())
        {
            ConfirmDialog *const dialog = CREATEWIDGETR(ConfirmDialog,
                // TRANSLATORS: sell confirmation header
                _("sell item"),
                // TRANSLATORS: sell confirmation message
                strprintf(_("Do you really want to sell %s?"),
                    info.getName().c_str()),
                SOUND_REQUEST,
                false,
                Modal_true);
            dialog->addActionListener(this);
            return;
        }
    }

    if (mAdvanced == Advanced_true)
        sellManyItems(event.getId());
    else
        sellOneItem();
}

void NpcSellDialog::sellManyItems(const std::string &eventId)
{
    if (eventId == "confirm")
    {
        npcHandler->sellItems(mShopItems->allItems());
        close();
    }
    else
    {
        const int selectedItem = mShopItemList->getSelected();
        ShopItem *const item = mShopItems->at(selectedItem);
        item->increaseUsedQuantity(mAmountItems);
        item->update();
        if (mConfirmButton != nullptr)
            mConfirmButton->setEnabled(true);
    }
}

void NpcSellDialog::sellOneItem()
{
    if (mAmountItems <= 0 || mAmountItems > mMaxItems)
        return;

    const int selectedItem = mShopItemList->getSelected();
    ShopItem *const item = mShopItems->at(selectedItem);
    // Attempt sell
    mPlayerMoney += mAmountItems * mShopItems->at(selectedItem)->getPrice();
    mMaxItems -= mAmountItems;
    while (mAmountItems > 0)
    {
        // This order is important, item->getCurrentInvIndex() would
        // return the inventory index of the next Duplicate otherwise.
        const int itemIndex = item->getCurrentInvIndex();
        const int sellCount = item->sellCurrentDuplicate(mAmountItems);
        npcHandler->sellItem(mNpcId, itemIndex, sellCount);
        mAmountItems -= sellCount;
    }

    mPlayerMoney += mAmountItems * mShopItems->at(selectedItem)->getPrice();
    mAmountItems = 1;
    mSlider->setValue(0);

    if (mMaxItems != 0)
    {
        updateButtonsAndLabels();
    }
    else
    {
        // All were sold
        mShopItemList->setSelected(-1);
        mShopItems->del(selectedItem);

        Rect scroll;
        scroll.y = mShopItemList->getRowHeight() * (selectedItem + 1);
        scroll.height = mShopItemList->getRowHeight();
        mShopItemList->showPart(scroll);
    }
}

void NpcSellDialog::close()
{
    buySellHandler->close();
    Window::close();
}
