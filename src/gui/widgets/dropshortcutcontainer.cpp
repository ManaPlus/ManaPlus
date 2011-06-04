/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/widgets/dropshortcutcontainer.h"

#include "gui/inventorywindow.h"
#include "gui/itempopup.h"
#include "gui/palette.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "configuration.h"
#include "dropshortcut.h"
#include "graphics.h"
#include "inventory.h"
#include "item.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "utils/stringutils.h"

#include "debug.h"

DropShortcutContainer::DropShortcutContainer():
    ShortcutContainer(),
    mItemClicked(false),
    mItemMoved(NULL)
{
    addMouseListener(this);
    addWidgetListener(this);

    mItemPopup = new ItemPopup;

    mBackgroundImg = Theme::getImageFromTheme("item_shortcut_bgr.png");
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
        mBackgroundImg->decRef();
    delete mItemPopup;
    mItemPopup = 0;
}

void DropShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!dropShortcut)
        return;

    if (Client::getGuiAlpha() != mAlpha)
    {
        mAlpha = Client::getGuiAlpha();
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Graphics *g = static_cast<Graphics*>(graphics);

    graphics->setFont(getFont());

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        if (mBackgroundImg)
            g->drawImage(mBackgroundImg, itemX, itemY);

/*        // Draw item keyboard shortcut.
        const char *key = SDL_GetKeyName(
            (SDLKey) keyboard.getKeyValue(keyboard.KEY_SHORTCUT_1 + i));
        graphics->setColor(guiPalette->getColor(Palette::TEXT));
        g->drawText(key, itemX + 2, itemY + 2, gcn::Graphics::LEFT);
*/
        if (dropShortcut->getItem(i) < 0)
            continue;

        Inventory *inv = PlayerInfo::getInventory();
        if (!inv)
            return;

        Item *item = inv->findItem(dropShortcut->getItem(i),
            dropShortcut->getItemColor(i));

        if (item)
        {
            // Draw item icon.
            Image* image = item->getImage();

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
                    g->setColor(Theme::getThemeColor(Theme::ITEM_EQUIPPED));
                else
                    g->setColor(Theme::getThemeColor(Theme::TEXT));
                g->drawText(caption, itemX + mBoxWidth / 2,
                            itemY + mBoxHeight - 14, gcn::Graphics::CENTER);
            }
        }
    }

    if (mItemMoved)
    {
        // Draw the item image being dragged by the cursor.
        Image* image = mItemMoved->getImage();
        if (image)
        {
            const int tPosX = mCursorPosX - (image->getWidth() / 2);
            const int tPosY = mCursorPosY - (image->getHeight() / 2);

            g->drawImage(image, tPosX, tPosY);
            g->drawText(toString(mItemMoved->getQuantity()),
                        tPosX + mBoxWidth / 2, tPosY + mBoxHeight - 14,
                        gcn::Graphics::CENTER);
        }
    }
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
            const int itemColor = dropShortcut->getItemColor(index);

            if (itemId < 0)
                return;

            Inventory *inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            Item *item = inv->findItem(itemId, itemColor);

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
        }
        else if (dropShortcut->getItem(index))
        {
            mItemClicked = true;
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Inventory *inv = PlayerInfo::getInventory();
        if (!inv)
            return;

        Item *item = inv->findItem(dropShortcut->getItem(index),
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
            mItemMoved = NULL;
            return;
        }
        if (mItemMoved)
        {
            dropShortcut->setItems(index, mItemMoved->getId(),
                mItemMoved->getColor());
            mItemMoved = NULL;
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
    const int itemColor = dropShortcut->getItemColor(index);

    if (itemId < 0)
        return;

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    Item *item = inv->findItem(itemId, itemColor);

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
void DropShortcutContainer::mouseExited(gcn::MouseEvent &event _UNUSED_)
{
    mItemPopup->setVisible(false);
}
