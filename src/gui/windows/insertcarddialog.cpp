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

#include "gui/windows/insertcarddialog.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/button.h"
#include "gui/widgets/shoplistbox.h"

#include "net/inventoryhandler.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/item/shopitem.h"

#include "debug.h"

InsertCardDialog::InsertCardDialog(const int itemIndex,
                                   const Item *const item) :
    SellDialog(IsSell_false, Advanced_false),
    mItemIndex(itemIndex)
{
    // TRANSLATORS: insert card dialog name
    setWindowName(_("Insert card"));
    if (item != nullptr)
    {
        // TRANSLATORS: insert card dialog name
        setCaption(strprintf(_("Insert card %s"),
            item->getName().c_str()));
    }
    else
    {
        // TRANSLATORS: insert card dialog name
        setCaption(strprintf(_("Insert card %s"),
            ""));
    }
}

void InsertCardDialog::initButtons()
{
    // TRANSLATORS: insert card dialog button
    mSellButton->setCaption(_("Insert"));
    mSellButton->adjustSize();
    mShopItems->setMergeDuplicates(false);
}

void InsertCardDialog::sellAction(const ActionEvent &event A_UNUSED)
{
    if (mAmountItems <= 0 || mAmountItems > mMaxItems)
        return;

    const int selectedItem = mShopItemList->getSelected();
    const ShopItem *const item = mShopItems->at(selectedItem);
    if (item == nullptr)
        return;
    inventoryHandler->insertCard(mItemIndex, item->getInvIndex());
    scheduleDelete();
}
