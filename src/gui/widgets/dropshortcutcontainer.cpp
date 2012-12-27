/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#include "gui/widgets/dropshortcutcontainer.h"

#include "gui/inventorywindow.h"
#include "gui/itempopup.h"
#include "gui/viewport.h"

#include "client.h"
#include "dropshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include <guichan/font.hpp>

#include "debug.h"

DropShortcutContainer::DropShortcutContainer():
    ShortcutContainer(),
    mItemClicked(false),
    mItemMoved(nullptr),
    mItemPopup(new ItemPopup),
    mEquipedColor(getThemeColor(Theme::ITEM_EQUIPPED)),
    mUnEquipedColor(getThemeColor(Theme::ITEM_NOT_EQUIPPED))
{
    addMouseListener(this);
    addWidgetListener(this);

    mBackgroundImg = Theme::getImageFromThemeXml(
        "item_shortcut_background.xml", "background.xml");
    if (dropShortcut)
        mMaxItems = dropShortcut->getItemCount();
    else
        mMaxItems = 0;

    if (mBackgroundImg)
    {
        mBackgroundImg->setAlpha(Client::getGuiAlpha());
        mBoxHeight = mBackgroundImg->getHeight();
        mBoxWidth = mBackgroundImg->getWidth();
    }
    else
    {
        mBoxHeight = 1;
        mBoxWidth = 1;
    }
}

DropShortcutContainer::~DropShortcutContainer()
{
    if (mBackgroundImg)
    {
        mBackgroundImg->decRef();
        mBackgroundImg = nullptr;
    }
    delete mItemPopup;
    mItemPopup = nullptr;
}

void DropShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mEquipedColor = getThemeColor(Theme::ITEM_EQUIPPED);
    mUnEquipedColor = getThemeColor(Theme::ITEM_NOT_EQUIPPED);
}

void DropShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!dropShortcut)
        return;

    BLOCK_START("DropShortcutContainer::draw")
    if (Client::getGuiAlpha() != mAlpha)
    {
        mAlpha = Client::getGuiAlpha();
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Graphics *const g = static_cast<Graphics*>(graphics);
    drawBackground(g);

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
    {
        BLOCK_END("DropShortcutContainer::draw")
        return;
    }

    gcn::Font *const font = getFont();

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        if (dropShortcut->getItem(i) < 0)
            continue;

        const Item *const item = inv->findItem(dropShortcut->getItem(i),
            dropShortcut->getItemColor(i));

        if (item)
        {
            // Draw item icon.
            Image *const image = item->getImage();

            if (image)
            {
                std::string caption;
                if (item->getQuantity() > 1)
                    caption = toString(item->getQuantity());
                else if (item->isEquipped())
                    caption = "Eq.";

                image->setAlpha(1.0f);
                g->drawImage(image, itemX, itemY);
                if (item->isEquipped())
                    g->setColor(mEquipedColor);
                else
                    g->setColor(mUnEquipedColor);
                font->drawString(g, caption,
                    itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                    itemY + mBoxHeight - 14);
            }
        }
    }

    if (mItemMoved)
    {
        // Draw the item image being dragged by the cursor.
        const Image *const image = mItemMoved->getImage();
        if (image)
        {
            const int tPosX = mCursorPosX - (image->mBounds.w / 2);
            const int tPosY = mCursorPosY - (image->mBounds.h / 2);
            const std::string str = toString(mItemMoved->getQuantity());

            g->drawImage(image, tPosX, tPosY);
            font->drawString(g, str,
                tPosX + (mBoxWidth / 2 - font->getWidth(str)) / 2,
                tPosY + mBoxHeight - 14);
        }
    }
    BLOCK_END("DropShortcutContainer::draw")
}

void DropShortcutContainer::mouseDragged(gcn::MouseEvent &event)
{
    if (!dropShortcut)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mItemMoved && mItemClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = dropShortcut->getItem(index);
            const unsigned char itemColor = dropShortcut->getItemColor(index);

            if (itemId < 0)
                return;

            const Inventory *const inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            Item *const item = inv->findItem(itemId, itemColor);

            if (item)
            {
                mItemMoved = item;
                dropShortcut->removeItem(index);
            }
        }
        if (mItemMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
}

void DropShortcutContainer::mousePressed(gcn::MouseEvent &event)
{
    if (!dropShortcut || !inventoryWindow)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        // Stores the selected item if theirs one.
        if (dropShortcut->isItemSelected() && inventoryWindow->isVisible())
        {
            dropShortcut->setItem(index);
            dropShortcut->setItemSelected(-1);
            inventoryWindow->unselectItem();
        }
        else if (dropShortcut->getItem(index))
        {
            mItemClicked = true;
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        const Inventory *const inv = PlayerInfo::getInventory();
        if (!inv)
            return;

        Item *const item = inv->findItem(dropShortcut->getItem(index),
            dropShortcut->getItemColor(index));

        if (viewport)
            viewport->showDropPopup(item);
    }
}

void DropShortcutContainer::mouseReleased(gcn::MouseEvent &event)
{
    if (!dropShortcut)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (dropShortcut->isItemSelected())
            dropShortcut->setItemSelected(-1);

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            mItemMoved = nullptr;
            return;
        }
        if (mItemMoved)
        {
            dropShortcut->setItems(index, mItemMoved->getId(),
                mItemMoved->getColor());
            mItemMoved = nullptr;
        }

        if (mItemClicked)
            mItemClicked = false;
    }
}

// Show ItemTooltip
void DropShortcutContainer::mouseMoved(gcn::MouseEvent &event)
{
    if (!dropShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = dropShortcut->getItem(index);
    const unsigned char itemColor = dropShortcut->getItemColor(index);

    if (itemId < 0)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const Item *const item = inv->findItem(itemId, itemColor);

    if (item && viewport)
    {
        mItemPopup->setItem(item);
        mItemPopup->position(viewport->getMouseX(), viewport->getMouseY());
    }
    else
    {
        mItemPopup->setVisible(false);
    }
}

// Hide ItemTooltip
void DropShortcutContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
}

void DropShortcutContainer::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
}
