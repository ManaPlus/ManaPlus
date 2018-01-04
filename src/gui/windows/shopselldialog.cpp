/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifdef TMWA_SUPPORT

#include "gui/windows/shopselldialog.h"

#include "being/playerinfo.h"

#include "gui/windows/tradewindow.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/shoplistbox.h"

#include "net/buysellhandler.h"

#include "debug.h"

ShopSellDialog::ShopSellDialog(const std::string &nick) :
    SellDialog(IsSell_true, Advanced_false),
    mNick(nick)
{
}

void ShopSellDialog::sellAction(const ActionEvent &event A_UNUSED)
{
    if (mAmountItems <= 0 || mAmountItems > mMaxItems)
        return;

    const int selectedItem = mShopItemList->getSelected();
    ShopItem *const item = mShopItems->at(selectedItem);
    if (item == nullptr || PlayerInfo::isItemProtected(item->getId()))
        return;
    buySellHandler->sendSellRequest(mNick, item, mAmountItems);

    if (tradeWindow != nullptr)
        tradeWindow->addAutoItem(mNick, item, mAmountItems);
}

#endif  // TMWA_SUPPORT
