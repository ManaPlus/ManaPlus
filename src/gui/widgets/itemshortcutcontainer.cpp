/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/itemshortcutcontainer.h"

#include "client.h"
#include "dragdrop.h"
#include "inventory.h"
#include "item.h"
#include "itemshortcut.h"
#include "spellshortcut.h"

#include "being/playerinfo.h"

#include "input/inputmanager.h"

#include "gui/font.h"
#include "gui/viewport.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/spellpopup.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/skilldialog.h"

#include "gui/widgets/skillinfo.h"

#include "resources/image.h"

#include "debug.h"

ItemShortcutContainer::ItemShortcutContainer(Widget2 *const widget,
                                             const unsigned number) :
    ShortcutContainer(widget),
    mItemClicked(false),
    mNumber(number),
    mItemPopup(new ItemPopup),
    mSpellPopup(new SpellPopup),
    mEquipedColor(getThemeColor(Theme::ITEM_EQUIPPED)),
    mEquipedColor2(getThemeColor(Theme::ITEM_EQUIPPED_OUTLINE)),
    mUnEquipedColor(getThemeColor(Theme::ITEM_NOT_EQUIPPED)),
    mUnEquipedColor2(getThemeColor(Theme::ITEM_NOT_EQUIPPED_OUTLINE))
{
    mItemPopup->postInit();
    mSpellPopup->postInit();

    addMouseListener(this);
    addWidgetListener(this);

    mForegroundColor2 = getThemeColor(Theme::TEXT_OUTLINE);
    mBackgroundImg = Theme::getImageFromThemeXml(
        "item_shortcut_background.xml", "background.xml");
    if (itemShortcut[mNumber])
        mMaxItems = itemShortcut[mNumber]->getItemCount();
    else
        mMaxItems = 0;

    if (mBackgroundImg)
    {
        mBackgroundImg->setAlpha(client->getGuiAlpha());
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
    mEquipedColor2 = getThemeColor(Theme::ITEM_EQUIPPED_OUTLINE);
    mUnEquipedColor = getThemeColor(Theme::ITEM_NOT_EQUIPPED);
    mUnEquipedColor2 = getThemeColor(Theme::ITEM_NOT_EQUIPPED_OUTLINE);
    mForegroundColor = getThemeColor(Theme::TEXT);
    mForegroundColor2 = getThemeColor(Theme::TEXT_OUTLINE);
}

void ItemShortcutContainer::draw(Graphics *graphics)
{
    BLOCK_START("ItemShortcutContainer::draw")
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    if (client->getGuiAlpha() != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = client->getGuiAlpha();
    }

    Font *const font = getFont();
    drawBackground(graphics);

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    // +++ for future usage need reorder drawing images before text or back
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        // Draw item keyboard shortcut.
        const std::string key = inputManager.getKeyValueString(
            Input::KEY_SHORTCUT_1 + i);
        graphics->setColorAll(mForegroundColor, mForegroundColor);
        font->drawString(graphics, key, itemX + 2, itemY + 2);

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

                    image->setAlpha(1.0F);
                    graphics->drawImage(image, itemX, itemY);
                    if (item->isEquipped())
                    {
                        graphics->setColorAll(mEquipedColor, mEquipedColor2);
                    }
                    else
                    {
                        graphics->setColorAll(mUnEquipedColor,
                            mUnEquipedColor2);
                    }
                    font->drawString(graphics, caption,
                        itemX + (mBoxWidth - font->getWidth(caption)) / 2,
                        itemY + mBoxHeight - 14);
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
                        image->setAlpha(1.0F);
                        graphics->drawImage(image, itemX, itemY);
                    }
                }

                font->drawString(graphics, spell->getSymbol(),
                    itemX + 2, itemY + mBoxHeight / 2);
            }
        }
        else if (skillDialog)
        {
            const SkillInfo *const skill = skillDialog->getSkill(
                itemId - SKILL_MIN_ID);
            if (skill)
            {
                Image *const image = skill->data->icon;

                if (image)
                {
                    image->setAlpha(1.0F);
                    graphics->drawImage(image, itemX, itemY);
                }

                font->drawString(graphics, skill->data->shortName, itemX + 2,
                    itemY + mBoxHeight / 2);
            }
        }
    }
    BLOCK_END("ItemShortcutContainer::draw")
}

void ItemShortcutContainer::mouseDragged(MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    if (event.getButton() == MouseEvent::LEFT)
    {
        if (dragDrop.isEmpty() && mItemClicked)
        {
            mItemClicked = false;

            const int index = getIndexFromGrid(event.getX(), event.getY());
            if (index == -1)
                return;

            const int itemId = selShortcut->getItem(index);
            const unsigned char itemColor = selShortcut->getItemColor(index);

            if (itemId < 0)
                return;

            event.consume();
            if (itemId < SPELL_MIN_ID)
            {   // items
                if (!PlayerInfo::getInventory())
                    return;

                const Item *const item = PlayerInfo::getInventory()->findItem(
                    itemId, itemColor);

                if (item)
                {
                    selShortcut->removeItem(index);
                    dragDrop.dragItem(item, DRAGDROP_SOURCE_SHORTCUTS, index);
                }
                else
                {
                    dragDrop.clear();
                }
            }
            else if (itemId < SKILL_MIN_ID)
            {   // spells/commands
                if (!spellManager)
                {
                    dragDrop.clear();
                    return;
                }

                const TextCommand *const spell = spellManager->getSpellByItem(
                    itemId);
                if (spell)
                {
                    selShortcut->removeItem(index);
                    dragDrop.dragCommand(spell,
                        DRAGDROP_SOURCE_SHORTCUTS, index);
                    dragDrop.setItem(itemId);
                }
                else
                {
                    dragDrop.clear();
                }
            }
            else
            {   // skills
                if (!skillDialog)
                {
                    dragDrop.clear();
                    return;
                }
                const SkillInfo *const skill
                    = skillDialog->getSkillByItem(itemId);
                if (skill)
                {
                    selShortcut->removeItem(index);
                    dragDrop.dragSkill(skill,
                        DRAGDROP_SOURCE_SHORTCUTS, index);
                    dragDrop.setItem(itemId);
                }
                else
                {
                    dragDrop.clear();
                }
            }
        }
    }
}

void ItemShortcutContainer::mousePressed(MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == MouseEvent::LEFT)
    {
        // Stores the selected item if theirs one.
        if (selShortcut->isItemSelected() && inventoryWindow &&
            (inventoryWindow->isWindowVisible()
            || selShortcut->getSelectedItem() >= SPELL_MIN_ID))
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
    else if (event.getButton() == MouseEvent::RIGHT)
    {
        if (viewport && selShortcut)
        {
            viewport->showItemPopup(selShortcut->getItem(index),
                selShortcut->getItemColor(index));
        }
    }
}

void ItemShortcutContainer::mouseReleased(MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    if (event.getButton() == MouseEvent::LEFT)
    {
        if (selShortcut->isItemSelected())
            selShortcut->setItemSelected(-1);

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
            return;

        if (dragDrop.isEmpty())
        {
            if (selShortcut->getItem(index) && mItemClicked)
                selShortcut->useItem(index);
        }
        else
        {
            if (dragDrop.getSource() == DRAGDROP_SOURCE_SHORTCUTS)
            {
                const int oldIndex = dragDrop.getTag();
                selShortcut->setItem(oldIndex, dragDrop.getItem(),
                    dragDrop.getItemColor());
                selShortcut->swap(oldIndex, index);
            }
            else
            {
                selShortcut->setItem(index, dragDrop.getItem(),
                    dragDrop.getItemColor());
            }
            dragDrop.clear();
            dragDrop.deselect();
        }

        mItemClicked = false;
    }
}

void ItemShortcutContainer::mouseMoved(MouseEvent &event)
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

        Inventory *const inv = PlayerInfo::getInventory();
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
void ItemShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}

void ItemShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}
