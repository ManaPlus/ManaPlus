/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/widgets/emoteshortcutcontainer.h"

#include "animatedsprite.h"
#include "client.h"
#include "emoteshortcut.h"

#include "input/inputmanager.h"

#include "gui/font.h"
#include "gui/viewport.h"

#include "gui/popups/textpopup.h"

#include "resources/image.h"

#include "resources/db/emotedb.h"

#include "debug.h"

static const int MAX_ITEMS = 48;

EmoteShortcutContainer::EmoteShortcutContainer(Widget2 *const widget) :
    ShortcutContainer(widget),
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

void EmoteShortcutContainer::draw(Graphics *graphics)
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

    Font *const font = getFont();
    drawBackground(graphics);

    graphics->setColorAll(mForegroundColor, mForegroundColor2);
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int emoteX = (i % mGridWidth) * mBoxWidth;
        const int emoteY = (i / mGridWidth) * mBoxHeight;

        // Draw emote keyboard shortcut.
        const std::string key = inputManager.getKeyValueString(
            Input::KEY_EMOTE_1 + i);

        font->drawString(graphics, key, emoteX + 2, emoteY + 2);
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
                sprite->draw(graphics,
                    (i % mGridWidth) * mBoxWidth + 2,
                    (i / mGridWidth) * mBoxHeight + 10);
            }
        }
    }

    BLOCK_END("EmoteShortcutContainer::draw")
}

void EmoteShortcutContainer::mouseDragged(MouseEvent &event A_UNUSED)
{
}

void EmoteShortcutContainer::mousePressed(MouseEvent &event)
{
    if (!emoteShortcut)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    event.consume();
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

void EmoteShortcutContainer::mouseReleased(MouseEvent &event)
{
    if (!emoteShortcut)
        return;

    if (event.getButton() == MouseEvent::LEFT)
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

void EmoteShortcutContainer::mouseMoved(MouseEvent &event)
{
    if (!emoteShortcut || !mEmotePopup)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    mEmotePopup->setVisible(false);

    if (static_cast<unsigned>(index) < mEmoteImg.size() && mEmoteImg[index])
    {
        const EmoteSprite *const sprite = mEmoteImg[index];
        mEmotePopup->show(viewport->getMouseX(), viewport->getMouseY(),
            strprintf("%s, %d", sprite->name.c_str(), sprite->id));
    }
}

void EmoteShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (mEmotePopup)
        mEmotePopup->setVisible(false);
}

void EmoteShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (mEmotePopup)
        mEmotePopup->setVisible(false);
}
