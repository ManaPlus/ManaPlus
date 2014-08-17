/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "dragdrop.h"
#include "dropshortcut.h"
#include "settings.h"

#include "being/playerinfo.h"

#include "gui/popupmanager.h"
#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"

#include "gui/windows/inventorywindow.h"

#include "resources/image.h"

#include "utils/delete2.h"
#include "utils/stringutils.h"

#include "debug.h"

DropShortcutContainer::DropShortcutContainer(Widget2 *const widget) :
    ShortcutContainer(widget),
    mItemClicked(false),
    mEquipedColor(getThemeColor(Theme::ITEM_EQUIPPED)),
    mEquipedColor2(getThemeColor(Theme::ITEM_EQUIPPED_OUTLINE)),
    mUnEquipedColor(getThemeColor(Theme::ITEM_NOT_EQUIPPED)),
    mUnEquipedColor2(getThemeColor(Theme::ITEM_NOT_EQUIPPED_OUTLINE))
{
    if (dropShortcut)
        mMaxItems = dropShortcut->getItemCount();
    else
        mMaxItems = 0;
}

DropShortcutContainer::~DropShortcutContainer()
{
}

void DropShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mEquipedColor = getThemeColor(Theme::ITEM_EQUIPPED);
    mEquipedColor2 = getThemeColor(Theme::ITEM_EQUIPPED_OUTLINE);
    mUnEquipedColor = getThemeColor(Theme::ITEM_NOT_EQUIPPED);
    mUnEquipedColor2 = getThemeColor(Theme::ITEM_NOT_EQUIPPED_OUTLINE);
}

void DropShortcutContainer::draw(Graphics *graphics)
{
    if (!dropShortcut)
        return;

    BLOCK_START("DropShortcutContainer::draw")
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
        BLOCK_END("DropShortcutContainer::draw")
        return;
    }

    Font *const font = getFont();

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

                image->setAlpha(1.0F);
                graphics->drawImage(image, itemX, itemY);
                if (item->isEquipped())
                    graphics->setColorAll(mEquipedColor, mEquipedColor2);
                else
                    graphics->setColorAll(mUnEquipedColor, mUnEquipedColor2);
                font->drawString(graphics, caption,
                    itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                    itemY + mBoxHeight - 14);
            }
        }
    }
    BLOCK_END("DropShortcutContainer::draw")
}

void DropShortcutContainer::mouseDragged(MouseEvent &event)
{
    if (!dropShortcut)
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        if (dragDrop.isEmpty() && mItemClicked)
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
                dragDrop.dragItem(item, DRAGDROP_SOURCE_DROP);
                dropShortcut->removeItem(index);
            }
            else
            {
                dragDrop.clear();
            }
        }
    }
}

void DropShortcutContainer::mousePressed(MouseEvent &event)
{
    if (!dropShortcut || !inventoryWindow)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    event.consume();

    const MouseButton::Type eventButton = event.getButton();
    if (eventButton == MouseButton::LEFT)
    {
        if (dropShortcut->getItem(index) > 0)
        {
            mItemClicked = true;
        }
        else
        {
            if (dragDrop.isSelected())
            {
                dropShortcut->setItems(index, dragDrop.getSelected(),
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

        Item *const item = inv->findItem(dropShortcut->getItem(index),
            dropShortcut->getItemColor(index));

        if (popupMenu)
        {
            popupMenu->showDropPopup(viewport->mMouseX,
                viewport->mMouseY,
                item);
        }
    }
}

void DropShortcutContainer::mouseReleased(MouseEvent &event)
{
    if (!dropShortcut)
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        if (dropShortcut->isItemSelected())
            dropShortcut->setItemSelected(-1);

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
                dropShortcut->setItems(index, dragDrop.getItem(),
                    dragDrop.getItemColor());
                dragDrop.clear();
                dragDrop.deselect();
            }
        }

        mItemClicked = false;
    }
}

// Show ItemTooltip
void DropShortcutContainer::mouseMoved(MouseEvent &event)
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
        itemPopup->setItem(item);
        itemPopup->position(viewport->mMouseX, viewport->mMouseY);
    }
    else
    {
        itemPopup->setVisible(false);
    }
}

void DropShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (itemPopup)
        itemPopup->setVisible(false);
}

void DropShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (itemPopup)
        itemPopup->setVisible(false);
}
