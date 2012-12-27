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

#include "gui/widgets/shoplistbox.h"

#include "client.h"
#include "configuration.h"
#include "shopitem.h"

#include "gui/itempopup.h"
#include "gui/viewport.h"

#include "gui/widgets/shopitems.h"

#include "resources/image.h"

#include <guichan/font.hpp>
#include <guichan/listmodel.hpp>

#include "debug.h"

const int ITEM_ICON_SIZE = 32;

float ShopListBox::mAlpha = 1.0;

ShopListBox::ShopListBox(const Widget2 *const widget,
                         gcn::ListModel *const listModel) :
    ListBox(widget, listModel),
    mPlayerMoney(0),
    mShopItems(nullptr),
    mItemPopup(new ItemPopup),
    mRowHeight(getFont()->getHeight()),
    mPriceCheck(true),
    mHighlightColor(getThemeColor(Theme::HIGHLIGHT)),
    mBackgroundColor(getThemeColor(Theme::BACKGROUND)),
    mWarningColor(getThemeColor(Theme::SHOP_WARNING))
{
    mForegroundColor = getThemeColor(Theme::LISTBOX);
}

ShopListBox::ShopListBox(const Widget2 *const widget,
                         gcn::ListModel *const listModel,
                         ShopItems *const shopListModel) :
    ListBox(widget, listModel),
    mPlayerMoney(0),
    mShopItems(shopListModel),
    mItemPopup(new ItemPopup),
    mRowHeight(std::max(getFont()->getHeight(), ITEM_ICON_SIZE)),
    mPriceCheck(true),
    mHighlightColor(getThemeColor(Theme::HIGHLIGHT)),
    mBackgroundColor(getThemeColor(Theme::BACKGROUND)),
    mWarningColor(getThemeColor(Theme::SHOP_WARNING))
{
    mForegroundColor = getThemeColor(Theme::LISTBOX);
}

void ShopListBox::init()
{
}

void ShopListBox::setPlayersMoney(const int money)
{
    mPlayerMoney = money;
}

void ShopListBox::draw(gcn::Graphics *gcnGraphics)
{
    BLOCK_START("ShopListBox::draw")
    if (!mListModel || !mShopItems)
    {
        BLOCK_END("ShopListBox::draw")
        return;
    }

    if (Client::getGuiAlpha() != mAlpha)
        mAlpha = Client::getGuiAlpha();

    const int alpha = static_cast<int>(mAlpha * 255.0f);
    Graphics *graphics = static_cast<Graphics*>(gcnGraphics);
    gcn::Font *const font = getFont();

    // Draw the list elements
    for (int i = 0, y = 0;
         i < mListModel->getNumberOfElements();
         ++i, y += mRowHeight)
    {
        bool needDraw(false);
        gcn::Color temp;
        gcn::Color* backgroundColor = &mBackgroundColor;

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
            graphics->fillRectangle(gcn::Rectangle(mPadding, y + mPadding,
                getWidth() - 2 * mPadding, mRowHeight));
        }

        if (mShopItems)
        {
            Image *const icon = mShopItems->at(i)->getImage();
            if (icon)
            {
                icon->setAlpha(1.0f);
                graphics->drawImage(icon, mPadding, y + mPadding);
            }
        }
        if (mSelected == i)
            graphics->setColor(mForegroundSelectedColor);
        else
            graphics->setColor(mForegroundColor);
        font->drawString(graphics, mListModel->getElementAt(i),
            ITEM_ICON_SIZE + mPadding,
            y + (ITEM_ICON_SIZE - getFont()->getHeight()) / 2 + mPadding);
    }
    BLOCK_END("ShopListBox::draw")
}

void ShopListBox::adjustSize()
{
    if (mListModel)
        setHeight(mRowHeight * mListModel->getNumberOfElements());
}

void ShopListBox::setPriceCheck(const bool check)
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

    const int index = (event.getY() - mPadding) / mRowHeight;

    if (index < 0 || index >= mShopItems->getNumberOfElements())
    {
        mItemPopup->hide();
    }
    else
    {
        const Item *const item = mShopItems->at(index);
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
