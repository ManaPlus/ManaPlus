/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/widgets/itemshortcutcontainer.h"

#include "dragdrop.h"
#include "settings.h"
#include "spellmanager.h"

#include "being/playerinfo.h"

#include "input/inputmanager.h"

#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/shortcut/itemshortcut.h"
#include "gui/shortcut/spellshortcut.h"

#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/skillpopup.h"
#include "gui/popups/spellpopup.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/skilldialog.h"

#include "input/inputactionoperators.h"

#include "const/resources/skill.h"

#include "utils/stringutils.h"

#include "debug.h"

ItemShortcutContainer::ItemShortcutContainer(Widget2 *const widget,
                                             const unsigned number) :
    ShortcutContainer(widget),
    mItemClicked(false),
    mNumber(number),
    mEquipedColor(getThemeColor(ThemeColorId::ITEM_EQUIPPED)),
    mEquipedColor2(getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE)),
    mUnEquipedColor(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED)),
    mUnEquipedColor2(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE))
{
    if (itemShortcut[mNumber])
        mMaxItems = itemShortcut[mNumber]->getItemCount();
    else
        mMaxItems = 0;
}

ItemShortcutContainer::~ItemShortcutContainer()
{
}

void ItemShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mEquipedColor = getThemeColor(ThemeColorId::ITEM_EQUIPPED);
    mEquipedColor2 = getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE);
    mUnEquipedColor = getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED);
    mUnEquipedColor2 = getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE);
    mForegroundColor = getThemeColor(ThemeColorId::TEXT);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXT_OUTLINE);
}

void ItemShortcutContainer::draw(Graphics *const graphics)
{
    BLOCK_START("ItemShortcutContainer::draw")
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
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
            InputAction::SHORTCUT_1 + i);
        font->drawString(graphics,
            mForegroundColor,
            mForegroundColor,
            key,
            itemX + 2, itemY + 2);

        const int itemId = selShortcut->getItem(i);
        const ItemColor itemColor = selShortcut->getItemColor(i);

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

                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    spell->getSymbol(),
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

                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    skill->data->shortName,
                    itemX + 2,
                    itemY + mBoxHeight / 2);
            }
        }
    }
    BLOCK_END("ItemShortcutContainer::draw")
}

void ItemShortcutContainer::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ItemShortcutContainer::draw")
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    safeDrawBackground(graphics);

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
            InputAction::SHORTCUT_1 + i);
        font->drawString(graphics,
            mForegroundColor,
            mForegroundColor,
            key,
            itemX + 2, itemY + 2);

        const int itemId = selShortcut->getItem(i);
        const ItemColor itemColor = selShortcut->getItemColor(i);

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

                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    spell->getSymbol(),
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

                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    skill->data->shortName,
                    itemX + 2,
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

    if (event.getButton() == MouseButton::LEFT)
    {
        if (dragDrop.isEmpty() && mItemClicked)
        {
            mItemClicked = false;

            const int index = getIndexFromGrid(event.getX(), event.getY());
            if (index == -1)
                return;

            const int itemId = selShortcut->getItem(index);
            const ItemColor itemColor = selShortcut->getItemColor(index);

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
                    dragDrop.dragItem(item, DragDropSource::Shortcuts, index);
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
                        DragDropSource::Shortcuts, index);
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
                    const std::string itemData = selShortcut->getItemData(
                        index);
                    selShortcut->removeItem(index);
                    dragDrop.dragSkill(skill,
                        DragDropSource::Shortcuts,
                        index);
                    dragDrop.setItem(itemId);
                    dragDrop.setItemColor(itemColor);
                    dragDrop.setItemData(itemData);
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

    if (event.getButton() == MouseButton::LEFT)
    {
        event.consume();
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
    else if (event.getButton() == MouseButton::RIGHT)
    {
        event.consume();
        if (popupMenu && selShortcut && viewport)
        {
            popupMenu->showItemPopup(viewport->mMouseX,
                viewport->mMouseY,
                selShortcut->getItem(index),
                selShortcut->getItemColor(index));
        }
    }
}

void ItemShortcutContainer::mouseReleased(MouseEvent &event)
{
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (!selShortcut)
        return;

    if (event.getButton() == MouseButton::LEFT)
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
            if (dragDrop.getSource() == DragDropSource::Shortcuts)
            {
                const int oldIndex = dragDrop.getTag();
                selShortcut->setItem(oldIndex, dragDrop.getItem(),
                    dragDrop.getItemColor());
                selShortcut->swap(oldIndex, index);
            }
            else
            {
                selShortcut->setItemData(index,
                    dragDrop.getItemData());
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
    const ItemColor itemColor = selShortcut->getItemColor(index);

    if (itemId < 0)
        return;

    if (itemId < SPELL_MIN_ID)
    {
        skillPopup->setVisible(Visible_false);
        spellPopup->setVisible(Visible_false);

        Inventory *const inv = PlayerInfo::getInventory();
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
    else if (itemId < SKILL_MIN_ID && spellManager)
    {
        skillPopup->setVisible(Visible_false);
        itemPopup->setVisible(Visible_false);
        const TextCommand *const spell = spellManager->getSpellByItem(itemId);
        if (spell && viewport)
        {
            spellPopup->setItem(spell);
            spellPopup->view(viewport->mMouseX,
                viewport->mMouseY);
        }
        else
        {
            spellPopup->setVisible(Visible_false);
        }
    }
    else if (skillDialog)
    {
        itemPopup->setVisible(Visible_false);
        spellPopup->setVisible(Visible_false);
        const SkillInfo *const skill = skillDialog->getSkillByItem(itemId);
        if (!skill)
            return;

        const std::string data = selShortcut->getItemData(index);
        CastTypeT castType = CastType::Default;
        int offsetX = 0;
        int offsetY = 0;
        if (!data.empty())
        {
            std::vector<int> vect;
            splitToIntVector(vect, data, ' ');
            const size_t sz = vect.size();
            if (sz > 0)
                castType = static_cast<CastTypeT>(vect[0]);
            if (sz > 2)
            {
                offsetX = vect[1];
                offsetY = vect[2];
            }
        }
        skillPopup->show(skill,
            toInt(itemColor, int),
            castType,
            offsetX,
            offsetY);
        skillPopup->position(viewport->mMouseX,
            viewport->mMouseY);
    }
}

// Hide ItemTooltip
void ItemShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (itemPopup)
        itemPopup->setVisible(Visible_false);
    if (spellPopup)
        spellPopup->setVisible(Visible_false);
    if (skillPopup)
        skillPopup->setVisible(Visible_false);
}

void ItemShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (itemPopup)
        itemPopup->setVisible(Visible_false);
    if (spellPopup)
        spellPopup->setVisible(Visible_false);
}
