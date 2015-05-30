/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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
#include "emoteshortcut.h"
#include "settings.h"

#include "input/inputmanager.h"

#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/popups/textpopup.h"

#include "input/inputactionoperators.h"

#include "resources/emotesprite.h"
#include "resources/image.h"

#include "resources/db/emotedb.h"

#include "utils/stringutils.h"

#include "debug.h"

static const int MAX_ITEMS = 48;

EmoteShortcutContainer::EmoteShortcutContainer(Widget2 *const widget) :
    ShortcutContainer(widget),
    mEmoteImg(),
    mEmoteClicked(false),
    mEmoteMoved(0)
{
    if (mBackgroundImg)
        mBackgroundImg->setAlpha(settings.guiAlpha);

    // Setup emote sprites
    for (int i = 0; i <= EmoteDB::getLast(); i++)
    {
        const EmoteSprite *const sprite = EmoteDB::getSprite(i, true);
        if (sprite && sprite->sprite)
            mEmoteImg.push_back(sprite);
    }

    mMaxItems = MAX_ITEMS;
}

EmoteShortcutContainer::~EmoteShortcutContainer()
{
}

void EmoteShortcutContainer::setWidget2(const Widget2 *const widget)
{
    Widget2::setWidget2(widget);
    mForegroundColor = getThemeColor(ThemeColorId::TEXT);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXT_OUTLINE);
}

void EmoteShortcutContainer::draw(Graphics *graphics)
{
    if (!emoteShortcut)
        return;

    BLOCK_START("EmoteShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    drawBackground(graphics);

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
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int emoteX = (i % mGridWidth) * mBoxWidth;
        const int emoteY = (i / mGridWidth) * mBoxHeight;

        // Draw emote keyboard shortcut.
        const std::string key = inputManager.getKeyValueString(
            InputAction::EMOTE_1 + i);

        font->drawString(graphics,
            mForegroundColor,
            mForegroundColor2,
            key,
            emoteX + 2, emoteY + 2);
    }

    BLOCK_END("EmoteShortcutContainer::draw")
}

void EmoteShortcutContainer::safeDraw(Graphics *graphics)
{
    if (!emoteShortcut)
        return;

    BLOCK_START("EmoteShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    safeDrawBackground(graphics);

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
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int emoteX = (i % mGridWidth) * mBoxWidth;
        const int emoteY = (i / mGridWidth) * mBoxHeight;

        // Draw emote keyboard shortcut.
        const std::string key = inputManager.getKeyValueString(
            InputAction::EMOTE_1 + i);

        font->drawString(graphics,
            mForegroundColor,
            mForegroundColor2,
            key,
            emoteX + 2, emoteY + 2);
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

    if (event.getButton() == MouseButton::LEFT)
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
    if (!emoteShortcut || !textPopup)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    textPopup->setVisible(Visible_false);

    if (static_cast<size_t>(index) < mEmoteImg.size() && mEmoteImg[index])
    {
        const EmoteSprite *const sprite = mEmoteImg[index];
        textPopup->show(viewport->mMouseX, viewport->mMouseY,
            strprintf("%s, %d", sprite->name.c_str(), sprite->id));
    }
}

void EmoteShortcutContainer::mouseExited(MouseEvent &event A_UNUSED)
{
    if (textPopup)
        textPopup->setVisible(Visible_false);
}

void EmoteShortcutContainer::widgetHidden(const Event &event A_UNUSED)
{
    if (textPopup)
        textPopup->setVisible(Visible_false);
}
