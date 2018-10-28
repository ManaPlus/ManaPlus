/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/widgets/emoteshortcutcontainer.h"

#include "settings.h"

#include "input/inputmanager.h"

#include "gui/viewport.h"

#include "gui/fonts/font.h"

#include "gui/shortcut/emoteshortcut.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "input/inputactionoperators.h"

#include "resources/emotesprite.h"

#include "resources/db/emotedb.h"

#include "resources/image/image.h"

#include "resources/sprite/animatedsprite.h"

#include "utils/stringutils.h"

#include "debug.h"

static const int MAX_ITEMS = 48;

EmoteShortcutContainer::EmoteShortcutContainer(Widget2 *restrict const
                                               widget) :
    ShortcutContainer(widget),
    mEmoteImg(),
    mEmoteClicked(false),
    mEmoteMoved(0)
{
    if (mBackgroundImg != nullptr)
        mBackgroundImg->setAlpha(settings.guiAlpha);

    // Setup emote sprites
    for (int i = 0; i <= EmoteDB::getLast(); i++)
    {
        const EmoteSprite *const sprite = EmoteDB::getSprite(i, true);
        if ((sprite != nullptr) && (sprite->sprite != nullptr))
            mEmoteImg.push_back(sprite);
    }

    mMaxItems = MAX_ITEMS;
}

EmoteShortcutContainer::~EmoteShortcutContainer()
{
}

void EmoteShortcutContainer::setSkin(const Widget2 *const widget,
                                     Skin *const skin)
{
    ShortcutContainer::setSkin(widget, skin);
    mForegroundColor = getThemeColor(ThemeColorId::TEXT, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXT_OUTLINE, 255U);
}

void EmoteShortcutContainer::draw(Graphics *restrict graphics) restrict2
{
    if (emoteShortcut == nullptr)
        return;

    BLOCK_START("EmoteShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg != nullptr)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    drawBackground(graphics);

    unsigned sz = CAST_U32(mEmoteImg.size());
    if (sz > mMaxItems)
        sz = mMaxItems;
    for (unsigned i = 0; i < sz; i++)
    {
        const EmoteSprite *restrict const emoteImg = mEmoteImg[i];
        if (emoteImg != nullptr)
        {
            const AnimatedSprite *restrict const sprite = emoteImg->sprite;
            if (sprite != nullptr)
            {
                sprite->draw(graphics,
                    (i % mGridWidth) * mBoxWidth + mImageOffsetX,
                    (i / mGridWidth) * mBoxHeight + mImageOffsetY);
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
            emoteX + mTextOffsetX,
            emoteY + mTextOffsetY);
    }

    BLOCK_END("EmoteShortcutContainer::draw")
}

void EmoteShortcutContainer::safeDraw(Graphics *restrict graphics) restrict2
{
    if (emoteShortcut == nullptr)
        return;

    BLOCK_START("EmoteShortcutContainer::draw")
    if (settings.guiAlpha != mAlpha)
    {
        if (mBackgroundImg != nullptr)
            mBackgroundImg->setAlpha(mAlpha);
        mAlpha = settings.guiAlpha;
    }

    Font *const font = getFont();
    safeDrawBackground(graphics);

    unsigned sz = CAST_U32(mEmoteImg.size());
    if (sz > mMaxItems)
        sz = mMaxItems;
    for (unsigned i = 0; i < sz; i++)
    {
        const EmoteSprite *restrict const emoteImg = mEmoteImg[i];
        if (emoteImg != nullptr)
        {
            const AnimatedSprite *restrict const sprite = emoteImg->sprite;
            if (sprite != nullptr)
            {
                sprite->draw(graphics,
                    (i % mGridWidth) * mBoxWidth + mImageOffsetX,
                    (i / mGridWidth) * mBoxHeight + mImageOffsetY);
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
            emoteX + mTextOffsetX,
            emoteY + mTextOffsetY);
    }

    BLOCK_END("EmoteShortcutContainer::draw")
}

void EmoteShortcutContainer::mouseDragged(MouseEvent &restrict event A_UNUSED)
                                          restrict2
{
}

void EmoteShortcutContainer::mousePressed(MouseEvent &restrict event) restrict2
{
    if (event.isConsumed())
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        if (emoteShortcut == nullptr)
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
        else if (emoteShortcut->getEmote(index) != 0U)
        {
            mEmoteClicked = true;
        }
    }
    else if (event.getButton() == MouseButton::RIGHT)
    {
        if (popupMenu != nullptr)
        {
            event.consume();
            popupMenu->showEmoteType();
        }
    }
}

void EmoteShortcutContainer::mouseReleased(MouseEvent &restrict event)
                                           restrict2
{
    if (emoteShortcut == nullptr)
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

        if (mEmoteMoved != 0U)
        {
            emoteShortcut->setEmotes(index, mEmoteMoved);
            mEmoteMoved = 0;
        }
        else if ((emoteShortcut->getEmote(index) != 0U) && mEmoteClicked)
        {
            emoteShortcut->useEmote(index + 1);
        }

        mEmoteClicked = false;
    }
}

void EmoteShortcutContainer::mouseMoved(MouseEvent &restrict event) restrict2
{
    if ((emoteShortcut == nullptr) || (textPopup == nullptr))
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    textPopup->setVisible(Visible_false);

    if (CAST_SIZE(index) < mEmoteImg.size() && (mEmoteImg[index] != nullptr))
    {
        const EmoteSprite *restrict const sprite = mEmoteImg[index];
        textPopup->show(viewport->mMouseX, viewport->mMouseY,
            strprintf("%s, %d", sprite->name.c_str(), sprite->id));
    }
}

void EmoteShortcutContainer::mouseExited(MouseEvent &restrict event A_UNUSED)
                                         restrict2
{
    if (textPopup != nullptr)
        textPopup->setVisible(Visible_false);
}

void EmoteShortcutContainer::widgetHidden(const Event &restrict event A_UNUSED)
                                          restrict2
{
    if (textPopup != nullptr)
        textPopup->setVisible(Visible_false);
}
