/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/widgets/emoteshortcutcontainer.h"

#include "animatedsprite.h"
#include "client.h"
#include "emoteshortcut.h"

#include "input/inputmanager.h"

#include "gui/viewport.h"

#include "gui/popups/textpopup.h"

#include "resources/image.h"

#include <guichan/font.hpp>

#include "debug.h"

static const int MAX_ITEMS = 48;

EmoteShortcutContainer::EmoteShortcutContainer():
    ShortcutContainer(),
    mEmoteImg(),
    mEmotePopup(new TextPopup),
    mEmoteClicked(false),
    mEmoteMoved(0)
{
    mEmotePopup->postInit();

    addMouseListener(this);
    addWidgetListener(this);

    mForegroundColor2 = getThemeColor(Theme::TEXT_OUTLINE);
    mBackgroundImg = Theme::getImageFromThemeXml(
        "item_shortcut_background.xml", "background.xml");

    if (mBackgroundImg)
        mBackgroundImg->setAlpha(client->getGuiAlpha());

    // Setup emote sprites
    for (int i = 0; i <= EmoteDB::getLast(); i++)
    {
        const EmoteSprite *const sprite = EmoteDB::getSprite(i, true);
        if (sprite && sprite->sprite)
            mEmoteImg.push_back(sprite);
    }

    mMaxItems = MAX_ITEMS;

    if (mBackgroundImg)
    {
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

EmoteShortcutContainer::~EmoteShortcutContainer()
{
    delete mEmotePopup;

    if (mBackgroundImg)
    {
        mBackgroundImg->decRef();
        mBackgroundImg = nullptr;
    }
}

void EmoteShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mForegroundColor = getThemeColor(Theme::TEXT);
    mForegroundColor2 = getThemeColor(Theme::TEXT_OUTLINE);
}

void EmoteShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!emoteShortcut)
        return;

    BLOCK_START("EmoteShortcutContainer::draw")
    if (client->getGuiAlpha() != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = client->getGuiAlpha();
    }

    Graphics *const g = static_cast<Graphics *const>(graphics);
    gcn::Font *const font = getFont();
    drawBackground(g);

    g->setColorAll(mForegroundColor, mForegroundColor2);
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int emoteX = (i % mGridWidth) * mBoxWidth;
        const int emoteY = (i / mGridWidth) * mBoxHeight;

        // Draw emote keyboard shortcut.
        const std::string key = inputManager.getKeyValueString(
            Input::KEY_EMOTE_1 + i);

        font->drawString(g, key, emoteX + 2, emoteY + 2);
    }
    unsigned sz = static_cast<unsigned>(mEmoteImg.size());
    if (sz > mMaxItems)
        sz = mMaxItems;
    for (unsigned i = 0; i < sz; i++)
    {
        const EmoteSprite *const emoteImg = mEmoteImg[i];
        if (emoteImg)
        {
            const AnimatedSprite *const sprite = emoteImg->sprite;
            if (sprite)
            {
                sprite->draw(g, (i % mGridWidth) * mBoxWidth + 2,
                    (i / mGridWidth) * mBoxHeight + 10);
            }
        }
    }

    BLOCK_END("EmoteShortcutContainer::draw")
}

void EmoteShortcutContainer::mouseDragged(gcn::MouseEvent &event A_UNUSED)
{
}

void EmoteShortcutContainer::mousePressed(gcn::MouseEvent &event)
{
    if (!emoteShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    // Stores the selected emote if there is one.
    if (emoteShortcut->isEmoteSelected())
    {
        emoteShortcut->setEmote(index);
        emoteShortcut->setEmoteSelected(0);
    }
    else if (emoteShortcut->getEmote(index))
    {
        mEmoteClicked = true;
    }
}

void EmoteShortcutContainer::mouseReleased(gcn::MouseEvent &event)
{
    if (!emoteShortcut)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        const int index = getIndexFromGrid(event.getX(), event.getY());

        if (emoteShortcut->isEmoteSelected())
            emoteShortcut->setEmoteSelected(0);

        if (index == -1)
        {
            mEmoteMoved = 0;
            return;
        }

        if (mEmoteMoved)
        {
            emoteShortcut->setEmotes(index, mEmoteMoved);
            mEmoteMoved = 0;
        }
        else if (emoteShortcut->getEmote(index) && mEmoteClicked)
        {
            emoteShortcut->useEmote(index + 1);
        }

        mEmoteClicked = false;
    }
}

void EmoteShortcutContainer::mouseMoved(gcn::MouseEvent &event)
{
    if (!emoteShortcut || !mEmotePopup)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    mEmotePopup->setVisible(false);

    if (static_cast<unsigned>(index) < mEmoteImg.size() && mEmoteImg[index])
    {
        mEmotePopup->show(viewport->getMouseX(), viewport->getMouseY(),
                          mEmoteImg[index]->name);
    }
}

void EmoteShortcutContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mEmotePopup)
        mEmotePopup->setVisible(false);
}

void EmoteShortcutContainer::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (mEmotePopup)
        mEmotePopup->setVisible(false);
}
