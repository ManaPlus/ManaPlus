/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "shopitem.h"

#include "gui/itempopup.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/shopitems.h"

#include "resources/image.h"

#include <guichan/font.hpp>
#include <guichan/listmodel.hpp>

#include "debug.h"

const int ITEM_ICON_SIZE = 32;

float ShopListBox::mAlpha = 1.0;

ShopListBox::ShopListBox(gcn::ListModel *listModel):
    ListBox(listModel),
    mPlayerMoney(0),
    mShopItems(0)
{
    mRowHeight = getFont()->getHeight();
    init();
}

ShopListBox::ShopListBox(gcn::ListModel *listModel, ShopItems *shopListModel):
    ListBox(listModel),
    mPlayerMoney(0),
    mShopItems(shopListModel)
{
    mRowHeight = std::max(getFont()->getHeight(), ITEM_ICON_SIZE);
    init();
}

void ShopListBox::init()
{
    mPriceCheck = true;
    mItemPopup = new ItemPopup;
    mHighlightColor = Theme::getThemeColor(Theme::HIGHLIGHT);
    mBackgroundColor = Theme::getThemeColor(Theme::BACKGROUND);
    mWarningColor = Theme::getThemeColor(Theme::SHOP_WARNING);
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));
}

void ShopListBox::setPlayersMoney(int money)
{
    mPlayerMoney = money;
}

void ShopListBox::draw(gcn::Graphics *gcnGraphics)
{
    if (!mListModel || !mShopItems)
        return;

    if (Client::getGuiAlpha() != mAlpha)
        mAlpha = Client::getGuiAlpha();

    int alpha = static_cast<int>(mAlpha * 255.0f);
    mHighlightColor.a = alpha;

    Graphics *graphics = static_cast<Graphics*>(gcnGraphics);

    graphics->setFont(getFont());

    // Draw the list elements
    for (int i = 0, y = 0;
         i < mListModel->getNumberOfElements();
         ++i, y += mRowHeight)
    {
        gcn::Color temp;
        gcn::Color* backgroundColor = &mBackgroundColor;
        mBackgroundColor.a = alpha;

        if (mShopItems && mShopItems->at(i) &&
            mPlayerMoney < mShopItems->at(i)->getPrice() && mPriceCheck)
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
                temp.b = (temp.g + mHighlightColor.b) / 2;
                temp.a = alpha;
                backgroundColor = &temp;
            }
        }
        else if (i == mSelected)
        {
            backgroundColor = &mHighlightColor;
        }

        graphics->setColor(*backgroundColor);
        graphics->fillRectangle(gcn::Rectangle(0, y, getWidth(), mRowHeight));

        if (mShopItems)
        {
            Image *icon = mShopItems->at(i)->getImage();
            if (icon)
            {
                icon->setAlpha(1.0f);
                graphics->drawImage(icon, 1, y);
            }
        }
        graphics->setColor(getForegroundColor());
        graphics->drawText(mListModel->getElementAt(i), ITEM_ICON_SIZE + 5,
            y + (ITEM_ICON_SIZE - getFont()->getHeight()) / 2);
    }
}

void ShopListBox::adjustSize()
{
    if (mListModel)
        setHeight(mRowHeight * mListModel->getNumberOfElements());
}

void ShopListBox::setPriceCheck(bool check)
{
    mPriceCheck = check;
}

void ShopListBox::mouseMoved(gcn::MouseEvent &event)
{
    if (!mItemPopup)
        return;

    if (!mShopItems)
    {
        mItemPopup->hide();
        return;
    }

    int index = event.getY() / mRowHeight;

    if (index < 0 || index >= mShopItems->getNumberOfElements())
    {
        mItemPopup->hide();
    }
    else
    {
        Item *item = mShopItems->at(index);
        if (item)
        {
            mItemPopup->setItem(item);
            mItemPopup->position(viewport->getMouseX(), viewport->getMouseY());
        }
        else
        {
            mItemPopup->setVisible(false);
        }
    }
}

void ShopListBox::mouseExited(gcn::MouseEvent& mouseEvent A_UNUSED)
{
    if (!mItemPopup)
        return;

    mItemPopup->hide();
}