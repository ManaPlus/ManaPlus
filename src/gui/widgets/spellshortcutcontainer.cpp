/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

#include "gui/inventorywindow.h"
#include "gui/okdialog.h"
#include "gui/shortcutwindow.h"
#include "gui/spellpopup.h"
#include "gui/viewport.h"
#include "gui/textcommandeditor.h"

#include "client.h"
#include "spellshortcut.h"
#include "itemshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

SpellShortcutContainer::SpellShortcutContainer(const unsigned number) :
    ShortcutContainer(),
    mSpellMoved(nullptr),
    mSpellPopup(new SpellPopup),
    mNumber(number),
    mForegroundColor2(getThemeColor(Theme::TEXT_OUTLINE)),
    mSpellClicked(false)
{
    addMouseListener(this);
    addWidgetListener(this);

    mBackgroundImg = Theme::getImageFromThemeXml(
        "item_shortcut_background.xml", "background.xml");
    if (spellShortcut)
        mMaxItems = spellShortcut->getSpellsCount();
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

SpellShortcutContainer::~SpellShortcutContainer()
{
    if (mBackgroundImg)
        mBackgroundImg->decRef();
    mBackgroundImg = nullptr;
    delete mSpellPopup;
    mSpellPopup = nullptr;
}

void SpellShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mForegroundColor = getThemeColor(Theme::TEXT);
    mForegroundColor2 = getThemeColor(Theme::TEXT_OUTLINE);
}

void SpellShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!spellShortcut)
        return;

    BLOCK_START("SpellShortcutContainer::draw")
    if (Client::getGuiAlpha() != mAlpha)
    {
        mAlpha = Client::getGuiAlpha();
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Graphics *const g = static_cast<Graphics *const>(graphics);
    gcn::Font *const font = getFont();

    const int selectedId = spellShortcut->getSelectedItem();
    g->setColorAll(mForegroundColor, mForegroundColor2);
    drawBackground(g);

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        const int itemId = spellShortcut->getItem(
            (mNumber * SPELL_SHORTCUT_ITEMS) + i);
        if (selectedId >= 0 && itemId == selectedId)
        {
            g->drawRectangle(gcn::Rectangle(itemX + 1, itemY + 1,
                mBoxWidth - 1, mBoxHeight - 1));
        }

        if (!spellManager)
            continue;

        const TextCommand *const spell = spellManager->getSpell(itemId);
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

            font->drawString(g, spell->getSymbol(),
                itemX + 2, itemY + mBoxHeight / 2);
        }
    }

    if (mSpellMoved)
    {
        // Draw the item image being dragged by the cursor.
    }
    BLOCK_END("SpellShortcutContainer::draw")
}

void SpellShortcutContainer::mouseDragged(gcn::MouseEvent &event)
{
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mSpellMoved && mSpellClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = spellShortcut->getItem(
                (mNumber * SPELL_SHORTCUT_ITEMS) + index);

            if (itemId < 0)
                return;
        }
        if (mSpellMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
}

void SpellShortcutContainer::mousePressed(gcn::MouseEvent &event)
{
    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        // Stores the selected item if theirs one.
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
    }
    else if (event.getButton() == gcn::MouseEvent::MIDDLE)
    {
        if (!spellShortcut || !spellManager)
            return;

        const int itemId = spellShortcut->getItem(
            (mNumber * SPELL_SHORTCUT_ITEMS) + index);
        spellManager->invoke(itemId);
    }
}

void SpellShortcutContainer::mouseReleased(gcn::MouseEvent &event)
{
    if (!spellShortcut || !spellManager)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = spellShortcut->getItem(
        (mNumber * SPELL_SHORTCUT_ITEMS) + index);

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (itemId < 0)
            return;

        const int selectedId = spellShortcut->getSelectedItem();

        if (selectedId != itemId)
        {
            const TextCommand *const spell = spellManager->getSpell(itemId);
            if (spell && !spell->isEmpty())
            {
                const int num = itemShortcutWindow->getTabIndex();
                if (num >= 0 && num < static_cast<int>(SHORTCUT_TABS)
                    && itemShortcut[num])
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
            if (num >= 0 && num < static_cast<int>(SHORTCUT_TABS)
                && itemShortcut[num])
            {
                itemShortcut[num]->setItemSelected(-1);
            }
            spellShortcut->setItemSelected(-1);
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        TextCommand *spell = nullptr;
        if (itemId >= 0)
            spell = spellManager->getSpell(itemId);

        if (spell && viewport)
            viewport->showSpellPopup(spell);
    }
}

// Show ItemTooltip
void SpellShortcutContainer::mouseMoved(gcn::MouseEvent &event)
{
    if (!mSpellPopup || !spellShortcut || !spellManager)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = spellShortcut->getItem(
        (mNumber * SPELL_SHORTCUT_ITEMS) + index);

    mSpellPopup->setVisible(false);
    const TextCommand *const spell = spellManager->getSpell(itemId);
    if (spell && !spell->isEmpty())
    {
        mSpellPopup->setItem(spell);
        mSpellPopup->view(viewport->getMouseX(), viewport->getMouseY());
    }
    else
    {
        mSpellPopup->setVisible(false);
    }
}

// Hide SpellTooltip
void SpellShortcutContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}

void SpellShortcutContainer::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}
