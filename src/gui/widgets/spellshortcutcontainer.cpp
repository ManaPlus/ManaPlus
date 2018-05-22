/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/widgets/spellshortcutcontainer.h"

#include "dragdrop.h"
#include "settings.h"
#include "spellmanager.h"

#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/shortcut/itemshortcut.h"
#include "gui/shortcut/spellshortcut.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/spellpopup.h"

#include "gui/windows/shortcutwindow.h"

#include "debug.h"

SpellShortcutContainer::SpellShortcutContainer(Widget2 *const widget,
                                               const unsigned number) :
    ShortcutContainer(widget),
    mNumber(number),
    mSpellClicked(false)
{
    if (spellShortcut != nullptr)
        mMaxItems = spellShortcut->getSpellsCount();
    else
        mMaxItems = 0;
}

SpellShortcutContainer::~SpellShortcutContainer()
{
}

void SpellShortcutContainer::setSkin(const Widget2 *const widget,
                                     Skin *const skin)
{
    ShortcutContainer::setSkin(widget, skin);
    mForegroundColor = getThemeColor(ThemeColorId::TEXT, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXT_OUTLINE, 255U);
}

void SpellShortcutContainer::draw(Graphics *const graphics)
{
    if (spellShortcut == nullptr)
        return;

    BLOCK_START("SpellShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        mAlpha = settings.guiAlpha;
        if (mBackgroundImg != nullptr)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Font *const font = getFont();

    const int selectedId = spellShortcut->getSelectedItem();
    graphics->setColor(mForegroundColor);
    drawBackground(graphics);

    // +++ in future need reorder images and string drawing.
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        const int itemId = getItemByIndex(i);
        if (selectedId >= 0 && itemId == selectedId)
        {
            graphics->drawRectangle(Rect(itemX + 1, itemY + 1,
                mBoxWidth - 1, mBoxHeight - 1));
        }

        if (spellManager == nullptr)
            continue;

        const TextCommand *const spell = spellManager->getSpell(itemId);
        if (spell != nullptr)
        {
            if (!spell->isEmpty())
            {
                Image *const image = spell->getImage();

                if (image != nullptr)
                {
                    image->setAlpha(1.0F);
                    graphics->drawImage(image, itemX, itemY);
                }
            }

            font->drawString(graphics,
                mForegroundColor,
                mForegroundColor2,
                spell->getSymbol(),
                itemX + 2, itemY + mBoxHeight / 2);
        }
    }

    BLOCK_END("SpellShortcutContainer::draw")
}

void SpellShortcutContainer::safeDraw(Graphics *const graphics)
{
    if (spellShortcut == nullptr)
        return;

    BLOCK_START("SpellShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        mAlpha = settings.guiAlpha;
        if (mBackgroundImg != nullptr)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Font *const font = getFont();

    const int selectedId = spellShortcut->getSelectedItem();
    graphics->setColor(mForegroundColor);
    safeDrawBackground(graphics);

    // +++ in future need reorder images and string drawing.
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        const int itemId = getItemByIndex(i);
        if (selectedId >= 0 && itemId == selectedId)
        {
            graphics->drawRectangle(Rect(itemX + 1, itemY + 1,
                mBoxWidth - 1, mBoxHeight - 1));
        }

        if (spellManager == nullptr)
            continue;

        const TextCommand *const spell = spellManager->getSpell(itemId);
        if (spell != nullptr)
        {
            if (!spell->isEmpty())
            {
                Image *const image = spell->getImage();

                if (image != nullptr)
                {
                    image->setAlpha(1.0F);
                    graphics->drawImage(image, itemX, itemY);
                }
            }

            font->drawString(graphics,
                mForegroundColor,
                mForegroundColor2,
                spell->getSymbol(),
                itemX + 2, itemY + mBoxHeight / 2);
        }
    }

    BLOCK_END("SpellShortcutContainer::draw")
}

void SpellShortcutContainer::mouseDragged(MouseEvent &event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        if (dragDrop.isEmpty() && mSpellClicked)
        {
            mSpellClicked = false;
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = getItemByIndex(index);
            if (itemId < 0)
                return;
            event.consume();
            TextCommand *const spell = spellManager->getSpell(itemId);
            if (spell != nullptr)
            {
                dragDrop.dragCommand(spell, DragDropSource::Spells, index);
                dragDrop.setItem(spell->getId() + SPELL_MIN_ID);
            }
            else
            {
                dragDrop.clear();
                mSpellClicked = false;
            }
        }
    }
}

void SpellShortcutContainer::mousePressed(MouseEvent &event)
{
    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const MouseButtonT eventButton = event.getButton();
    if (eventButton == MouseButton::LEFT)
    {
        const int itemId = getItemByIndex(index);
        if (itemId > 0)
            mSpellClicked = true;
        event.consume();
    }
    else if (eventButton == MouseButton::MIDDLE)
    {
        if ((spellShortcut == nullptr) || (spellManager == nullptr))
            return;

        event.consume();
        const int itemId = getItemByIndex(index);
        spellManager->invoke(itemId);
    }
}

void SpellShortcutContainer::mouseReleased(MouseEvent &event)
{
    if ((spellShortcut == nullptr) || (spellManager == nullptr))
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
    {
        dragDrop.clear();
        return;
    }

    const int itemId = getItemByIndex(index);
    const MouseButtonT eventButton = event.getButton();

    if (eventButton == MouseButton::LEFT)
    {
        mSpellClicked = false;

        if (itemId < 0)
            return;

        const int selectedId = spellShortcut->getSelectedItem();
        event.consume();

        if (!dragDrop.isEmpty())
        {
            if (dragDrop.getSource() == DragDropSource::Spells)
            {
                const int oldIndex = dragDrop.getTag();
                const int idx = mNumber * SPELL_SHORTCUT_ITEMS;
                spellManager->swap(idx + index, idx + oldIndex);
                spellManager->save();
                dragDrop.clear();
                dragDrop.deselect();
            }
        }
        else
        {
            if (selectedId != itemId)
            {
                const TextCommand *const
                    spell = spellManager->getSpell(itemId);
                if ((spell != nullptr) && !spell->isEmpty())
                {
                    const int num = itemShortcutWindow->getTabIndex();
                    if (num >= 0 && num < CAST_S32(SHORTCUT_TABS)
                        && (itemShortcut[num] != nullptr))
                    {
                        itemShortcut[num]->setItemSelected(
                            spell->getId() + SPELL_MIN_ID);
                    }
                    spellShortcut->setItemSelected(spell->getId());
                }
            }
            else
            {
                const int num = itemShortcutWindow->getTabIndex();
                if (num >= 0 && num < CAST_S32(SHORTCUT_TABS)
                    && (itemShortcut[num] != nullptr))
                {
                    itemShortcut[num]->setItemSelected(-1);
                }
                spellShortcut->setItemSelected(-1);
            }
        }
    }
    else if (eventButton == MouseButton::RIGHT)
    {
        TextCommand *spell = nullptr;
        if (itemId >= 0)
            spell = spellManager->getSpell(itemId);

        if ((spell != nullptr) && (popupMenu != nullptr))
        {
            popupMenu->showSpellPopup(viewport->mMouseX,
                viewport->mMouseY,
                spell);
        }
    }
}

// Show ItemTooltip
void SpellShortcutContainer::mouseMoved(MouseEvent &event)
{
    if (spellPopup == nullptr ||
        spellShortcut == nullptr ||
        spellManager == nullptr)
    {
        return;
    }

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = getItemByIndex(index);
    spellPopup->setVisible(Visible_false);
    const TextCommand *const spell = spellManager->getSpell(itemId);
    if ((spell != nullptr) && !spell->isEmpty())
    {
        spellPopup->setItem(spell);
        spellPopup->view(viewport->mMouseX, viewport->mMouseY);
    }
    else
    {
        spellPopup->setVisible(Visible_false);
    }
}

void SpellShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (spellPopup != nullptr)
        spellPopup->setVisible(Visible_false);
}

void SpellShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (spellPopup != nullptr)
        spellPopup->setVisible(Visible_false);
}

int SpellShortcutContainer::getItemByIndex(const int index) const
{
    return spellShortcut->getItem(
        (mNumber * SPELL_SHORTCUT_ITEMS) + index);
}
