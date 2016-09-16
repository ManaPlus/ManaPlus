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

#include "gui/widgets/shoplistbox.h"

#include "dragdrop.h"
#include "settings.h"

#include "being/playerinfo.h"

#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"

#include "gui/models/shopitems.h"

#include "gui/windows/itemamountwindow.h"

#include "net/serverfeatures.h"

#include "resources/inventory/inventory.h"

#include "resources/item/shopitem.h"

#include "debug.h"

const int ITEM_ICON_SIZE = 32;

ShopListBox::ShopListBox(const Widget2 *const widget,
                         ListModel *const listModel,
                         const ShopListBoxTypeT type) :
    ListBox(widget, listModel, "shoplistbox.xml"),
    mPlayerMoney(0),
    mShopItems(nullptr),
    mBackgroundColor(getThemeColor(ThemeColorId::BACKGROUND)),
    mWarningColor(getThemeColor(ThemeColorId::SHOP_WARNING)),
    mType(type),
    mPriceCheck(true),
    mProtectItems(false)
{
    mRowHeight = getFont()->getHeight();
    mHighlightColor = getThemeColor(ThemeColorId::HIGHLIGHT);
    mForegroundColor = getThemeColor(ThemeColorId::LISTBOX);
}

ShopListBox::ShopListBox(const Widget2 *const widget,
                         ListModel *const listModel,
                         ShopItems *const shopListModel,
                         const ShopListBoxTypeT type) :
    ListBox(widget, listModel, "shoplistbox.xml"),
    mPlayerMoney(0),
    mShopItems(shopListModel),
    mBackgroundColor(getThemeColor(ThemeColorId::BACKGROUND)),
    mWarningColor(getThemeColor(ThemeColorId::SHOP_WARNING)),
    mType(type),
    mPriceCheck(true),
    mProtectItems(false)
{
    mRowHeight = std::max(getFont()->getHeight(), ITEM_ICON_SIZE);
    mHighlightColor = getThemeColor(ThemeColorId::HIGHLIGHT);
    mForegroundColor = getThemeColor(ThemeColorId::LISTBOX);
}

void ShopListBox::setPlayersMoney(const int money)
{
    mPlayerMoney = money;
}

void ShopListBox::draw(Graphics *const graphics)
{
    BLOCK_START("ShopListBox::draw")
    if (!mListModel || !mShopItems)
    {
        BLOCK_END("ShopListBox::draw")
        return;
    }

    if (settings.guiAlpha != mAlpha)
        mAlpha = settings.guiAlpha;

    const unsigned int alpha = CAST_U32(mAlpha * 255.0F);
    Font *const font = getFont();

    const int sz = mListModel->getNumberOfElements();
    const int fontHeigh = getFont()->getHeight();
    const int width = mDimension.width - 2 * mPadding;
    // Draw the list elements
    for (int i = 0, y = 0;
         i < sz;
         ++i, y += mRowHeight)
    {
        bool needDraw(false);
        Color temp;
        Color* backgroundColor = &mBackgroundColor;

        ShopItem *const item = mShopItems->at(i);
        if (item &&
            (item->getDisabled() ||
            (mPlayerMoney < item->getPrice() && mPriceCheck) ||
            (mProtectItems && PlayerInfo::isItemProtected(item->getId()))))
        {
            if (i != mSelected)
            {
                backgroundColor = &mWarningColor;
                backgroundColor->a = alpha;
            }
            else
            {
                temp = mWarningColor;
                temp.r = (temp.r + mHighlightColor.r) / 2;
                temp.g = (temp.g + mHighlightColor.g) / 2;
                temp.b = (temp.b + mHighlightColor.b) / 2;
                temp.a = alpha;
                backgroundColor = &temp;
            }
            needDraw = true;
        }
        else if (i == mSelected)
        {
            mHighlightColor.a = alpha;
            backgroundColor = &mHighlightColor;
            needDraw = true;
        }
        else
        {
            mBackgroundColor.a = alpha;
        }

        if (needDraw)
        {
            graphics->setColor(*backgroundColor);
            graphics->fillRectangle(Rect(mPadding, y + mPadding,
                width, mRowHeight));
        }

        if (mShopItems && item)
        {
            Image *const icon = item->getImage();
            if (icon)
            {
                icon->setAlpha(1.0F);
                graphics->drawImage(icon, mPadding, y + mPadding);
            }
        }
        if (mSelected == i)
        {
            font->drawString(graphics,
                mForegroundSelectedColor,
                mForegroundSelectedColor2,
                mListModel->getElementAt(i),
                ITEM_ICON_SIZE + mPadding,
                y + (ITEM_ICON_SIZE - fontHeigh) / 2 + mPadding);
        }
        else
        {
            font->drawString(graphics,
                mForegroundColor,
                mForegroundColor2,
                mListModel->getElementAt(i),
                ITEM_ICON_SIZE + mPadding,
                y + (ITEM_ICON_SIZE - fontHeigh) / 2 + mPadding);
        }
    }
    BLOCK_END("ShopListBox::draw")
}

void ShopListBox::safeDraw(Graphics *const graphics)
{
    ShopListBox::draw(graphics);
}

void ShopListBox::adjustSize()
{
    BLOCK_START("ShopListBox::adjustSize")
    if (mListModel)
    {
        setHeight(mRowHeight * mListModel->getNumberOfElements()
            + 2 * mPadding);
    }
    BLOCK_END("ShopListBox::adjustSize")
}

void ShopListBox::setPriceCheck(const bool check)
{
    mPriceCheck = check;
}

void ShopListBox::mouseMoved(MouseEvent &event)
{
    if (!itemPopup || !mRowHeight)
        return;

    if (!mShopItems)
    {
        itemPopup->hide();
        return;
    }

    const int index = (event.getY() - mPadding) / mRowHeight;

    if (index < 0 || index >= mShopItems->getNumberOfElements())
    {
        itemPopup->hide();
    }
    else
    {
        const Item *const item = mShopItems->at(index);
        if (item)
        {
            itemPopup->setItem(item, false);
            itemPopup->position(viewport->mMouseX, viewport->mMouseY);
        }
        else
        {
            itemPopup->setVisible(Visible_false);
        }
    }
}

void ShopListBox::mouseReleased(MouseEvent& event)
{
    ListBox::mouseReleased(event);
    if (event.getType() == MouseEventType::RELEASED2)
    {
        if (dragDrop.isEmpty())
            return;
        const DragDropSourceT src = dragDrop.getSource();
        if (mType != ShopListBoxType::SellShop &&
            mType != ShopListBoxType::BuyShop)
        {
            return;
        }
        if (mType == ShopListBoxType::SellShop &&
            serverFeatures->haveCart() &&
            src != DragDropSource::Cart)
        {
            return;
        }
        Inventory *inventory;
        if (src == DragDropSource::Inventory)
            inventory = PlayerInfo::getInventory();
        else if (src == DragDropSource::Cart)
            inventory = PlayerInfo::getCartInventory();
        else
            return;
        if (!inventory)
            return;
        Item *const item = inventory->getItem(dragDrop.getTag());
        if (mType == ShopListBoxType::BuyShop)
        {
            ItemAmountWindow::showWindow(
                ItemAmountWindowUsage::ShopBuyAdd,
                nullptr,
                item);
        }
        else
        {
            ItemAmountWindow::showWindow(
                ItemAmountWindowUsage::ShopSellAdd,
                nullptr,
                item);
        }
    }

    if (event.getButton() == MouseButton::RIGHT)
    {
        setSelected(std::max(0, getSelectionByMouse(event.getY())));

        if (mSelected < 0 || mSelected >= mShopItems->getNumberOfElements())
            return;

        Item *const item = mShopItems->at(mSelected);
        if (popupMenu && viewport)
        {
            popupMenu->showItemPopup(viewport->mMouseX,
            viewport->mMouseY,
            item);
        }
    }
}

void ShopListBox::mouseExited(MouseEvent& event A_UNUSED)
{
    if (!itemPopup)
        return;

    itemPopup->hide();
}
