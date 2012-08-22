/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/widgets/emoteshortcutcontainer.h"

#include "animatedsprite.h"
#include "client.h"
#include "configuration.h"
#include "emoteshortcut.h"
#include "graphics.h"
#include "inputmanager.h"
#include "inventory.h"
#include "item.h"
#include "itemshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"

#include "gui/textpopup.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "resources/image.h"

#include "utils/dtor.h"

#include "debug.h"

static const int MAX_ITEMS = 48;

EmoteShortcutContainer::EmoteShortcutContainer():
    ShortcutContainer(),
    mEmoteClicked(false),
    mEmoteMoved(0),
    mEmotePopup(new TextPopup)
{
    addMouseListener(this);
    addWidgetListener(this);

    mBackgroundImg = Theme::getImageFromThemeXml(
        "item_shortcut_background.xml", "background.xml");

    if (mBackgroundImg)
        mBackgroundImg->setAlpha(Client::getGuiAlpha());

    // Setup emote sprites
    for (int i = 0; i <= EmoteDB::getLast(); i++)
    {
        const EmoteSprite* sprite = EmoteDB::getSprite(i, true);
        if (sprite && sprite->sprite)
            mEmoteImg.push_back(sprite);
    }

//    mMaxItems = EmoteDB::getLast() < MAX_ITEMS ? EmoteDB::getLast() : MAX_ITEMS;
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
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));
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

void EmoteShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!emoteShortcut)
        return;

    mAlpha = Client::getGuiAlpha();
    if (Client::getGuiAlpha() != mAlpha && mBackgroundImg)
        mBackgroundImg->setAlpha(mAlpha);

    Graphics *g = static_cast<Graphics*>(graphics);

    graphics->setFont(getFont());

    if (mBackgroundImg)
    {
        for (unsigned i = 0; i < mMaxItems; i++)
        {
            g->drawImage(mBackgroundImg, (i % mGridWidth) * mBoxWidth,
                (i / mGridWidth) * mBoxHeight);
        }
    }

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int emoteX = (i % mGridWidth) * mBoxWidth;
        const int emoteY = (i / mGridWidth) * mBoxHeight;

        // Draw emote keyboard shortcut.
        std::string key = inputManager.getKeyValueString(
            Input::KEY_EMOTE_1 + i);

        graphics->setColor(getForegroundColor());
        g->drawText(key, emoteX + 2, emoteY + 2, gcn::Graphics::LEFT);
    }
    unsigned sz = static_cast<unsigned>(mEmoteImg.size());
    for (unsigned i = 0; i < mMaxItems; i++)
    {
        if (i < sz && mEmoteImg[i] && mEmoteImg[i]->sprite)
        {
            mEmoteImg[i]->sprite->draw(g, (i % mGridWidth) * mBoxWidth + 2,
                (i / mGridWidth) * mBoxHeight + 10);
        }
    }

    if (mEmoteMoved && mEmoteMoved < static_cast<unsigned>(sz) + 1
        && mEmoteMoved > 0)
    {
        // Draw the emote image being dragged by the cursor.
        const EmoteSprite* sprite = mEmoteImg[mEmoteMoved - 1];
        if (sprite && sprite->sprite)
        {
            const AnimatedSprite *spr = sprite->sprite;
            const int tPosX = mCursorPosX - (spr->getWidth() / 2);
            const int tPosY = mCursorPosY - (spr->getHeight() / 2);

            spr->draw(g, tPosX, tPosY);
        }
    }
}

void EmoteShortcutContainer::mouseDragged(gcn::MouseEvent &event A_UNUSED)
{
/*
    if (!emoteShortcut)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mEmoteMoved && mEmoteClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

//            const unsigned char emoteId = emoteShortcut->getEmote(index);
            const unsigned char emoteId
                = static_cast<unsigned char>(index + 1);

            if (emoteId)
            {
                mEmoteMoved = emoteId;
                emoteShortcut->removeEmote(index);
            }
        }
        if (mEmoteMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
*/
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

        if (mEmoteClicked)
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
