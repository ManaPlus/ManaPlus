/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/windows/buyingstoreselldialog.h"

#include "actormanager.h"

#include "being/playerinfo.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/shoplistbox.h"

#include "net/buyingstorehandler.h"

#include "resources/inventory/inventory.h"

#include "resources/item/shopitem.h"

#include "debug.h"

BuyingStoreSellDialog::BuyingStoreSellDialog(const BeingId accountId,
                                             const int storeId) :
    SellDialog(IsSell_true, Advanced_false),
    mAccountId(accountId),
    mStoreId(storeId)
{
}

void BuyingStoreSellDialog::sellAction(const ActionEvent &event A_UNUSED)
{
    if (mAmountItems <= 0 || mAmountItems > mMaxItems)
        return;

    const int selectedItem = mShopItemList->getSelected();
    const ShopItem *const item1 = mShopItems->at(selectedItem);
    if ((item1 == nullptr) || PlayerInfo::isItemProtected(item1->getId()))
        return;
    const Being *const being = actorManager->findBeing(mAccountId);
    if (being == nullptr)
        return;
    const Item *const item2 = PlayerInfo::getInventory()->findItem(
        item1->getId(),
        item1->getColor());
    if (item2 == nullptr)
        return;

    mPlayerMoney += mAmountItems * item1->getPrice();
    mMaxItems -= mAmountItems;
    buyingStoreHandler->sell(being, mStoreId, item2, mAmountItems);
}
