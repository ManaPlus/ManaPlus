/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/widgets/virtshortcutcontainer.h"

#include "dragdrop.h"
#include "settings.h"

#include "being/playerinfo.h"

#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/shortcut/shortcutbase.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"

#include "gui/windows/inventorywindow.h"

#include "utils/stringutils.h"

#include "debug.h"

VirtShortcutContainer::VirtShortcutContainer(Widget2 *const widget,
                                             ShortcutBase *const shortcut) :
    ShortcutContainer(widget),
    mItemClicked(false),
    mEquipedColor(getThemeColor(ThemeColorId::ITEM_EQUIPPED)),
    mEquipedColor2(getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE)),
    mUnEquipedColor(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED)),
    mUnEquipedColor2(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE)),
    mShortcut(shortcut)
{
    if (mShortcut)
        mMaxItems = mShortcut->getItemCount();
    else
        mMaxItems = 0;
}

VirtShortcutContainer::~VirtShortcutContainer()
{
}

void VirtShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mEquipedColor = getThemeColor(ThemeColorId::ITEM_EQUIPPED);
    mEquipedColor2 = getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE);
    mUnEquipedColor = getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED);
    mUnEquipedColor2 = getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE);
}

void VirtShortcutContainer::draw(Graphics *const graphics)
{
    if (!mShortcut)
        return;

    BLOCK_START("VirtShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        mAlpha = settings.guiAlpha;
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    drawBackground(graphics);

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
    {
        BLOCK_END("VirtShortcutContainer::draw")
        return;
    }

    Font *const font = getFont();

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        if (mShortcut->getItem(i) < 0)
            continue;

        const Item *const item = inv->findItem(mShortcut->getItem(i),
            mShortcut->getItemColor(i));

        if (item)
        {
            // Draw item icon.
            Image *const image = item->getImage();

            if (image)
            {
                std::string caption;
                if (item->getQuantity() > 1)
                    caption = toString(item->getQuantity());
                else if (item->isEquipped() == Equipped_true)
                    caption = "Eq.";

                image->setAlpha(1.0F);
                graphics->drawImage(image, itemX, itemY);
                if (item->isEquipped() == Equipped_true)
                {
                    font->drawString(graphics,
                        mEquipedColor,
                        mEquipedColor2,
                        caption,
                        itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                        itemY + mBoxHeight - 14);
                }
                else
                {
                    font->drawString(graphics,
                        mUnEquipedColor,
                        mUnEquipedColor2,
                        caption,
                        itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                        itemY + mBoxHeight - 14);
                }
            }
        }
    }
    BLOCK_END("VirtShortcutContainer::draw")
}

void VirtShortcutContainer::safeDraw(Graphics *const graphics)
{
    if (!mShortcut)
        return;

    BLOCK_START("VirtShortcutContainer::safeDraw")
    if (settings.guiAlpha != mAlpha)
    {
        mAlpha = settings.guiAlpha;
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    safeDrawBackground(graphics);

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
    {
        BLOCK_END("VirtShortcutContainer::safeDraw")
        return;
    }

    Font *const font = getFont();

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        if (mShortcut->getItem(i) < 0)
            continue;

        const Item *const item = inv->findItem(mShortcut->getItem(i),
            mShortcut->getItemColor(i));

        if (item)
        {
            // Draw item icon.
            Image *const image = item->getImage();

            if (image)
            {
                std::string caption;
                if (item->getQuantity() > 1)
                    caption = toString(item->getQuantity());
                else if (item->isEquipped() == Equipped_true)
                    caption = "Eq.";

                image->setAlpha(1.0F);
                graphics->drawImage(image, itemX, itemY);
                if (item->isEquipped() == Equipped_true)
                {
                    font->drawString(graphics,
                        mEquipedColor,
                        mEquipedColor2,
                        caption,
                        itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                        itemY + mBoxHeight - 14);
                }
                else
                {
                    font->drawString(graphics,
                        mUnEquipedColor,
                        mUnEquipedColor2,
                        caption,
                        itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                        itemY + mBoxHeight - 14);
                }
            }
        }
    }
    BLOCK_END("VirtShortcutContainer::safeDraw")
}

void VirtShortcutContainer::mouseDragged(MouseEvent &event)
{
    if (!mShortcut)
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        if (dragDrop.isEmpty() && mItemClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = mShortcut->getItem(index);
            const ItemColor itemColor = mShortcut->getItemColor(index);

            if (itemId < 0)
                return;

            const Inventory *const inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            Item *const item = inv->findItem(itemId, itemColor);

            if (item)
            {
                dragDrop.dragItem(item, DragDropSource::Drop);
                mShortcut->removeItem(index);
            }
            else
            {
                dragDrop.clear();
            }
        }
    }
}

void VirtShortcutContainer::mousePressed(MouseEvent &event)
{
    if (!mShortcut || !inventoryWindow)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    event.consume();

    const MouseButtonT eventButton = event.getButton();
    if (eventButton == MouseButton::LEFT)
    {
        if (mShortcut->getItem(index) > 0)
        {
            mItemClicked = true;
        }
        else
        {
            if (dragDrop.isSelected())
            {
                mShortcut->setItems(index, dragDrop.getSelected(),
                    dragDrop.getSelectedColor());
                dragDrop.deselect();
            }
        }
    }
    else if (eventButton == MouseButton::RIGHT)
    {
        const Inventory *const inv = PlayerInfo::getInventory();
        if (!inv)
            return;

        Item *const item = inv->findItem(mShortcut->getItem(index),
            mShortcut->getItemColor(index));

        if (popupMenu)
        {
            popupMenu->showDropPopup(viewport->mMouseX,
                viewport->mMouseY,
                item);
        }
    }
}

void VirtShortcutContainer::mouseReleased(MouseEvent &event)
{
    if (!mShortcut)
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        if (mShortcut->isItemSelected())
            mShortcut->setItemSelected(-1);

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            dragDrop.clear();
            return;
        }
        if (!dragDrop.isEmpty())
        {
            if (dragDrop.isSourceItemContainer())
            {
                mShortcut->setItems(index, dragDrop.getItem(),
                    dragDrop.getItemColor());
                dragDrop.clear();
                dragDrop.deselect();
            }
        }

        mItemClicked = false;
    }
}

// Show ItemTooltip
void VirtShortcutContainer::mouseMoved(MouseEvent &event)
{
    if (!mShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = mShortcut->getItem(index);
    const ItemColor itemColor = mShortcut->getItemColor(index);

    if (itemId < 0)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const Item *const item = inv->findItem(itemId, itemColor);

    if (item && viewport)
    {
        itemPopup->setItem(item, false);
        itemPopup->position(viewport->mMouseX, viewport->mMouseY);
    }
    else
    {
        itemPopup->setVisible(Visible_false);
    }
}

void VirtShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (itemPopup)
        itemPopup->setVisible(Visible_false);
}

void VirtShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (itemPopup)
        itemPopup->setVisible(Visible_false);
}
