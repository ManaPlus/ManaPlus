/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/widgets/itemshortcutcontainer.h"

#include "client.h"
#include "configuration.h"
#include "inventory.h"
#include "inputmanager.h"
#include "item.h"
#include "itemshortcut.h"
#include "spellshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/inventorywindow.h"
#include "gui/itempopup.h"
#include "gui/skilldialog.h"
#include "gui/spellpopup.h"
#include "gui/viewport.h"

#include "resources/image.h"

#include "debug.h"

ItemShortcutContainer::ItemShortcutContainer(const unsigned number) :
    ShortcutContainer(),
    mItemClicked(false),
    mItemMoved(nullptr),
    mNumber(number),
    mItemPopup(new ItemPopup),
    mSpellPopup(new SpellPopup),
    mEquipedColor(getThemeColor(Theme::ITEM_EQUIPPED)),
    mUnEquipedColor(getThemeColor(Theme::ITEM_NOT_EQUIPPED))
{
    addMouseListener(this);
    addWidgetListener(this);

    mBackgroundImg = Theme::getImageFromThemeXml(
        "item_shortcut_background.xml", "background.xml");
    if (itemShortcut[mNumber])
        mMaxItems = itemShortcut[mNumber]->getItemCount();
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
    mForegroundColor = getThemeColor(Theme::TEXT);
}

ItemShortcutContainer::~ItemShortcutContainer()
{
    if (mBackgroundImg)
    {
        mBackgroundImg->decRef();
        mBackgroundImg = nullptr;
    }
    delete mItemPopup;
    mItemPopup = nullptr;
    delete mSpellPopup;
    mSpellPopup = nullptr;
}

void ItemShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mEquipedColor = getThemeColor(Theme::ITEM_EQUIPPED);
    mUnEquipedColor = getThemeColor(Theme::ITEM_NOT_EQUIPPED);
    mForegroundColor = getThemeColor(Theme::TEXT);
}

void ItemShortcutContainer::draw(gcn::Graphics *graphics)
{
    BLOCK_START("ItemShortcutContainer::draw")
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    mAlpha = Client::getGuiAlpha();
    if (Client::getGuiAlpha() != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Graphics *const g = static_cast<Graphics*>(graphics);

    graphics->setFont(getFont());

    if (mBackgroundImg)
    {
        for (unsigned i = 0; i < mMaxItems; i++)
        {
            g->drawImage(mBackgroundImg, (i % mGridWidth) * mBoxWidth,
                (i / mGridWidth) * mBoxHeight);
        }
    }

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        // Draw item keyboard shortcut.
        std::string key = inputManager.getKeyValueString(
            Input::KEY_SHORTCUT_1 + i);
        graphics->setColor(mForegroundColor);

        g->drawText(key, itemX + 2, itemY + 2, gcn::Graphics::LEFT);

        const int itemId = selShortcut->getItem(i);
        const unsigned char itemColor = selShortcut->getItemColor(i);

        if (itemId < 0)
            continue;

        // this is item
        if (itemId < SPELL_MIN_ID)
        {
            const Item *const item = inv->findItem(itemId, itemColor);

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
                    g->drawText(caption, itemX + mBoxWidth / 2,
                        itemY + mBoxHeight - 14, gcn::Graphics::CENTER);
                }
            }
        }
        else if (itemId < SKILL_MIN_ID && spellManager)
        {   // this is magic shortcut
            const TextCommand *const spell = spellManager
                ->getSpellByItem(itemId);
            if (spell)
            {
                if (!spell->isEmpty())
                {
                    Image *const image = spell->getImage();

                    if (image)
                    {
                        image->setAlpha(1.0f);
                        g->drawImage(image, itemX, itemY);
                    }
                }

                g->drawText(spell->getSymbol(), itemX + 2,
                            itemY + mBoxHeight / 2, gcn::Graphics::LEFT);
            }
        }
        else if (skillDialog)
        {
            const SkillInfo *const skill = skillDialog->getSkill(
                itemId - SKILL_MIN_ID);
            if (skill)
            {
                Image *const image = skill->icon;

                if (image)
                {
                    image->setAlpha(1.0f);
                    g->drawImage(image, itemX, itemY);
                }

                g->drawText(skill->shortName, itemX + 2,
                    itemY + mBoxHeight / 2, gcn::Graphics::LEFT);
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

            g->drawImage(image, tPosX, tPosY);
            g->drawText(toString(mItemMoved->getQuantity()),
                        tPosX + mBoxWidth / 2, tPosY + mBoxHeight - 14,
                        gcn::Graphics::CENTER);
        }
    }
    BLOCK_END("ItemShortcutContainer::draw")
}

void ItemShortcutContainer::mouseDragged(gcn::MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mItemMoved && mItemClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = selShortcut->getItem(index);
            const unsigned char itemColor = selShortcut->getItemColor(index);

            if (itemId < 0)
                return;

            if (itemId < SPELL_MIN_ID)
            {
                if (!PlayerInfo::getInventory())
                    return;

                Item *const item = PlayerInfo::getInventory()->findItem(
                    itemId, itemColor);

                if (item)
                {
                    mItemMoved = item;
                    selShortcut->removeItem(index);
                }
            }
            else if (itemId < SKILL_MIN_ID && spellManager)
            {
                const TextCommand *const spell = spellManager->getSpellByItem(
                    itemId);
                if (spell)
                    selShortcut->removeItem(index);
            }
            else
            {
                selShortcut->removeItem(index);
            }
        }
        if (mItemMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
}

void ItemShortcutContainer::mousePressed(gcn::MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        // Stores the selected item if theirs one.
        if (selShortcut->isItemSelected() && inventoryWindow &&
            (inventoryWindow->isVisible() || selShortcut->getSelectedItem()
            >= SPELL_MIN_ID))
        {
            selShortcut->setItem(index);
            selShortcut->setItemSelected(-1);
            if (spellShortcut)
                spellShortcut->setItemSelected(-1);
            inventoryWindow->unselectItem();
        }
        else if (selShortcut->getItem(index))
        {
            mItemClicked = true;
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        if (viewport && selShortcut)
        {
            viewport->showItemPopup(selShortcut->getItem(index),
                selShortcut->getItemColor(index));
        }
    }
}

void ItemShortcutContainer::mouseReleased(gcn::MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (selShortcut->isItemSelected())
            selShortcut->setItemSelected(-1);

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            mItemMoved = nullptr;
            return;
        }
        if (mItemMoved)
        {
            selShortcut->setItems(index, mItemMoved->getId(),
                mItemMoved->getColor());
            mItemMoved = nullptr;
        }
        else if (selShortcut->getItem(index) && mItemClicked)
        {
            selShortcut->useItem(index);
        }

        if (mItemClicked)
            mItemClicked = false;
    }
}

// Show ItemTooltip
void ItemShortcutContainer::mouseMoved(gcn::MouseEvent &event)
{
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = selShortcut->getItem(index);
    const unsigned char itemColor = selShortcut->getItemColor(index);

    if (itemId < 0)
        return;

    if (itemId < SPELL_MIN_ID)
    {
        mSpellPopup->setVisible(false);

        if (!PlayerInfo::getInventory())
            return;

        const Item *const item = PlayerInfo::getInventory()->findItem(
            itemId, itemColor);

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
    else if (itemId < SKILL_MIN_ID && spellManager)
    {
        mItemPopup->setVisible(false);
        const TextCommand *const spell = spellManager->getSpellByItem(itemId);
        if (spell && viewport)
        {
            mSpellPopup->setItem(spell);
            mSpellPopup->view(viewport->getMouseX(), viewport->getMouseY());
        }
        else
        {
            mSpellPopup->setVisible(false);
        }
    }
    else if (skillDialog)
    {
        mItemPopup->setVisible(false);
    }
}

// Hide ItemTooltip
void ItemShortcutContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}

void ItemShortcutContainer::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}
