/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/skin.h"
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
    mEquipedColor(getThemeColor(ThemeColorId::ITEM_EQUIPPED, 255U)),
    mEquipedColor2(getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE, 255U)),
    mUnEquipedColor(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED, 255U)),
    mUnEquipedColor2(getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE,
        255U)),
    mNumber(number),
    mKeyOffsetX(2),
    mKeyOffsetY(2),
    mItemClicked(false)
{
    mMaxItems = ItemShortcut::getItemCount();
}

ItemShortcutContainer::~ItemShortcutContainer()
{
}

void ItemShortcutContainer::setSkin(const Widget2 *const widget,
                                    Skin *const skin)
{
    ShortcutContainer::setSkin(widget, skin);
    mEquipedColor = getThemeColor(ThemeColorId::ITEM_EQUIPPED, 255U);
    mEquipedColor2 = getThemeColor(ThemeColorId::ITEM_EQUIPPED_OUTLINE, 255U);
    mUnEquipedColor = getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED, 255U);
    mUnEquipedColor2 = getThemeColor(ThemeColorId::ITEM_NOT_EQUIPPED_OUTLINE,
        255U);
    mForegroundColor = getThemeColor(ThemeColorId::TEXT, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXT_OUTLINE, 255U);
    if (mSkin != nullptr)
    {
        mKeyOffsetX = mSkin->getOption("keyOffsetX", 2);
        mKeyOffsetY = mSkin->getOption("keyOffsetY", 2);
    }
}

void ItemShortcutContainer::draw(Graphics *const graphics)
{
    BLOCK_START("ItemShortcutContainer::draw")
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (selShortcut == nullptr)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg != nullptr)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    drawBackground(graphics);

    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
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
            itemX + mKeyOffsetX,
            itemY + mKeyOffsetY);

        const int itemId = selShortcut->getItem(i);
        const ItemColor itemColor = selShortcut->getItemColor(i);

        if (itemId < 0)
            continue;

        // this is item
        if (itemId < SPELL_MIN_ID)
        {
            const Item *const item = inv->findItem(itemId, itemColor);
            if (item != nullptr)
            {
                // Draw item icon.
                Image *const image = item->getImage();
                if (image != nullptr)
                {
                    std::string caption;
                    if (item->getQuantity() > 1)
                        caption = toString(item->getQuantity());
                    else if (item->isEquipped() == Equipped_true)
                        caption = "Eq.";

                    image->setAlpha(1.0F);
                    graphics->drawImage(image,
                        itemX + mImageOffsetX,
                        itemY + mImageOffsetY);
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
        else if (itemId < SKILL_MIN_ID && (spellManager != nullptr))
        {   // this is magic shortcut
            const TextCommand *const spell = spellManager
                ->getSpellByItem(itemId);
            if (spell != nullptr)
            {
                if (!spell->isEmpty())
                {
                    Image *const image = spell->getImage();

                    if (image != nullptr)
                    {
                        image->setAlpha(1.0F);
                        graphics->drawImage(image,
                            itemX + mImageOffsetX,
                            itemY + mImageOffsetY);
                    }
                }

                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    spell->getSymbol(),
                    itemX + mTextOffsetX,
                    itemY + mTextOffsetY);
            }
        }
        else if (skillDialog != nullptr)
        {
            const SkillInfo *const skill = skillDialog->getSkill(
                itemId - SKILL_MIN_ID);
            if (skill != nullptr)
            {
                Image *const image = skill->data->icon;

                if (image != nullptr)
                {
                    image->setAlpha(1.0F);
                    graphics->drawImage(image,
                        itemX + mImageOffsetX,
                        itemY + mImageOffsetY);
                }
                if (!skill->data->haveIcon)
                {
                    font->drawString(graphics,
                        mForegroundColor,
                        mForegroundColor,
                        skill->data->shortName,
                        itemX + mTextOffsetX,
                        itemY + mTextOffsetY);
                }
            }
        }
    }
    BLOCK_END("ItemShortcutContainer::draw")
}

void ItemShortcutContainer::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ItemShortcutContainer::draw")
    const ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (selShortcut == nullptr)
    {
        BLOCK_END("ItemShortcutContainer::draw")
        return;
    }

    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg != nullptr)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    safeDrawBackground(graphics);

    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
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
            itemX + mKeyOffsetX,
            itemY + mKeyOffsetY);

        const int itemId = selShortcut->getItem(i);
        const ItemColor itemColor = selShortcut->getItemColor(i);

        if (itemId < 0)
            continue;

        // this is item
        if (itemId < SPELL_MIN_ID)
        {
            const Item *const item = inv->findItem(itemId, itemColor);
            if (item != nullptr)
            {
                // Draw item icon.
                Image *const image = item->getImage();
                if (image != nullptr)
                {
                    std::string caption;
                    if (item->getQuantity() > 1)
                        caption = toString(item->getQuantity());
                    else if (item->isEquipped() == Equipped_true)
                        caption = "Eq.";

                    image->setAlpha(1.0F);
                    graphics->drawImage(image,
                        itemX + mImageOffsetX,
                        itemY + mImageOffsetY);
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
        else if (itemId < SKILL_MIN_ID && (spellManager != nullptr))
        {   // this is magic shortcut
            const TextCommand *const spell = spellManager
                ->getSpellByItem(itemId);
            if (spell != nullptr)
            {
                if (!spell->isEmpty())
                {
                    Image *const image = spell->getImage();

                    if (image != nullptr)
                    {
                        image->setAlpha(1.0F);
                        graphics->drawImage(image,
                            itemX + mImageOffsetX,
                            itemY + mImageOffsetY);
                    }
                }

                font->drawString(graphics,
                    mForegroundColor,
                    mForegroundColor,
                    spell->getSymbol(),
                    itemX + mTextOffsetX,
                    itemY + mTextOffsetY);
            }
        }
        else if (skillDialog != nullptr)
        {
            const SkillInfo *const skill = skillDialog->getSkill(
                itemId - SKILL_MIN_ID);
            if (skill != nullptr)
            {
                Image *const image = skill->data->icon;

                if (image != nullptr)
                {
                    image->setAlpha(1.0F);
                    graphics->drawImage(image,
                        itemX + mImageOffsetX,
                        itemY + mImageOffsetY);
                }
                if (!skill->data->haveIcon)
                {
                    font->drawString(graphics,
                        mForegroundColor,
                        mForegroundColor,
                        skill->data->shortName,
                        itemX + mTextOffsetX,
                        itemY + mTextOffsetY);
                }
            }
        }
    }
    BLOCK_END("ItemShortcutContainer::draw")
}

void ItemShortcutContainer::mouseDragged(MouseEvent &event)
{
    if (mNumber == SHORTCUT_AUTO_TAB)
        return;
    ItemShortcut *const selShortcut = itemShortcut[mNumber];
    if (selShortcut == nullptr)
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
                if (PlayerInfo::getInventory() == nullptr)
                    return;

                const Item *const item = PlayerInfo::getInventory()->findItem(
                    itemId, itemColor);

                if (item != nullptr)
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
                if (spellManager == nullptr)
                {
                    dragDrop.clear();
                    return;
                }

                const TextCommand *const spell = spellManager->getSpellByItem(
                    itemId);
                if (spell != nullptr)
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
                if (skillDialog == nullptr)
                {
                    dragDrop.clear();
                    return;
                }
                const SkillInfo *const skill
                    = skillDialog->getSkillByItem(itemId);
                if (skill != nullptr)
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
    if (selShortcut == nullptr)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        event.consume();
        // Stores the selected item if theirs one.
        if (selShortcut->isItemSelected() && (inventoryWindow != nullptr) &&
            (inventoryWindow->isWindowVisible()
            || selShortcut->getSelectedItem() >= SPELL_MIN_ID))
        {
            selShortcut->setItem(index);
            selShortcut->setItemSelected(-1);
            if (spellShortcut != nullptr)
                spellShortcut->setItemSelected(-1);
            inventoryWindow->unselectItem();
        }
        else if (selShortcut->getItem(index) != 0)
        {
            mItemClicked = true;
        }
    }
    else if (event.getButton() == MouseButton::RIGHT)
    {
        event.consume();
        if (popupMenu != nullptr &&
            viewport != nullptr)
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
    if (selShortcut == nullptr)
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
            if ((selShortcut->getItem(index) != 0) && mItemClicked)
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
    if (selShortcut == nullptr)
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
        if (inv == nullptr)
            return;

        const Item *const item = inv->findItem(itemId, itemColor);
        if ((item != nullptr) && (viewport != nullptr))
        {
            itemPopup->setItem(item, false);
            itemPopup->position(viewport->mMouseX, viewport->mMouseY);
        }
        else
        {
            itemPopup->setVisible(Visible_false);
        }
    }
    else if (itemId < SKILL_MIN_ID && (spellManager != nullptr))
    {
        skillPopup->setVisible(Visible_false);
        itemPopup->setVisible(Visible_false);
        const TextCommand *const spell = spellManager->getSpellByItem(itemId);
        if ((spell != nullptr) && (viewport != nullptr))
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
    else if (skillDialog != nullptr)
    {
        itemPopup->setVisible(Visible_false);
        spellPopup->setVisible(Visible_false);
        const SkillInfo *const skill = skillDialog->getSkillByItem(itemId);
        if (skill == nullptr)
            return;

        const std::string data = selShortcut->getItemData(index);
        CastTypeT castType = CastType::Default;
        int offsetX = 0;
        int offsetY = 0;
        if (!data.empty())
        {
            STD_VECTOR<int> vect;
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
    if (itemPopup != nullptr)
        itemPopup->setVisible(Visible_false);
    if (spellPopup != nullptr)
        spellPopup->setVisible(Visible_false);
    if (skillPopup != nullptr)
        skillPopup->setVisible(Visible_false);
}

void ItemShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (itemPopup != nullptr)
        itemPopup->setVisible(Visible_false);
    if (spellPopup != nullptr)
        spellPopup->setVisible(Visible_false);
}
