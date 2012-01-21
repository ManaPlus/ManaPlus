/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/emotepopup.h"

#include "animatedsprite.h"
#include "configuration.h"
#include "emoteshortcut.h"
#include "graphics.h"
#include "localplayer.h"
#include "logger.h"

#include "gui/theme.h"

#include "resources/emotedb.h"
#include "resources/image.h"
#include "resources/iteminfo.h"

#include "utils/dtor.h"

#include <guichan/mouseinput.hpp>
#include <guichan/selectionlistener.hpp>

#include "debug.h"

const int EmotePopup::gridWidth = 34;  // emote icon width + 4
const int EmotePopup::gridHeight = 36; // emote icon height + 4

static const int MAX_COLUMNS = 6;

EmotePopup::EmotePopup() :
    Popup("EmotePopup", "emotepopup.xml"),
    mSelectedEmoteIndex(-1),
    mHoveredEmoteIndex(-1),
    mRowCount(1),
    mColumnCount(1)
{
    // Setup emote sprites
    for (int i = 0; i <= EmoteDB::getLast(); ++i)
    {
        const AnimatedSprite *sprite = EmoteDB::getAnimation(i, true);
        if (sprite)
            mEmotes.push_back(sprite);
    }

    mSelectionImage = Theme::getImageFromTheme("selection.png");
    if (!mSelectionImage)
        logger->log1("Error: Unable to load selection.png");

    if (mSelectionImage)
        mSelectionImage->setAlpha(Client::getGuiAlpha());

    addMouseListener(this);
    recalculateSize();
    setVisible(true);
}

EmotePopup::~EmotePopup()
{
    if (mSelectionImage)
    {
        mSelectionImage->decRef();
        mSelectionImage = nullptr;
    }
}

void EmotePopup::draw(gcn::Graphics *graphics)
{
    Popup::draw(graphics);

    if (!mColumnCount)
        return;

    const unsigned int emoteCount = static_cast<unsigned>(mEmotes.size());
    const unsigned int emotesLeft
        = static_cast<unsigned>(mEmotes.size() % mColumnCount);

    for (unsigned int i = 0; i < emoteCount ; i++)
    {
        int row = i / mColumnCount;
        int column = i % mColumnCount;

        unsigned int emoteX = 4 + column * gridWidth;
        unsigned int emoteY = 4 + row * gridHeight;

        // Center the last row when there are less emotes than columns
        if (emotesLeft > 0 && row == mRowCount - 1)
            emoteX += (mColumnCount - emotesLeft) * gridWidth / 2;

        // Draw selection image below hovered item
        if (mSelectionImage && static_cast<int>(i) == mHoveredEmoteIndex)
        {
            static_cast<Graphics*>(graphics)->drawImage(
                    mSelectionImage, emoteX, emoteY + 4);
        }

        // Draw emote icon
        if (mEmotes[i])
            mEmotes[i]->draw(static_cast<Graphics*>(graphics), emoteX, emoteY);
    }
}

void EmotePopup::mousePressed(gcn::MouseEvent &event)
{
    if (event.getButton() != gcn::MouseEvent::LEFT)
        return;

    const int index = getIndexAt(event.getX(), event.getY());
    if (index != -1)
    {
        setSelectedEmoteIndex(index);
        if (emoteShortcut)
        {
            emoteShortcut->setEmoteSelected(
                static_cast<unsigned char>(index + 1));
        }
    }
}

void EmotePopup::mouseMoved(gcn::MouseEvent &event)
{
    Popup::mouseMoved(event);

    mHoveredEmoteIndex = getIndexAt(event.getX(), event.getY());
}

int EmotePopup::getSelectedEmote() const
{
    return 1 + mSelectedEmoteIndex;
}

void EmotePopup::setSelectedEmoteIndex(int index)
{
    if (index == mSelectedEmoteIndex)
        return;

    mSelectedEmoteIndex = index;
    distributeValueChangedEvent();
}

int EmotePopup::getIndexAt(int x, int y) const
{
    if (!gridWidth || !gridHeight)
        return -1;

    const unsigned int emotesLeft
        = static_cast<unsigned>(mEmotes.size() % mColumnCount);
    const unsigned int row = y / gridHeight;
    unsigned int column;

    // Take into account that the last row is centered
    if (emotesLeft > 0 && static_cast<signed>(row) == mRowCount - 1)
    {
        int unsigned emotesMissing = mColumnCount - emotesLeft;
        column = std::min((x - emotesMissing * gridWidth / 2) / gridWidth,
                          emotesLeft - 1);
    }
    else
    {
        column = std::min(x / gridWidth, mColumnCount - 1);
    }

    int unsigned index = column + (row * mColumnCount);

    if (index < mEmotes.size())
        return index;

    return -1;
}

void EmotePopup::recalculateSize()
{
    const unsigned emoteCount = static_cast<unsigned>(mEmotes.size());

    mRowCount = emoteCount / MAX_COLUMNS;
    if (emoteCount % MAX_COLUMNS > 0)
        ++mRowCount;

    if (mRowCount)
        mColumnCount = emoteCount / mRowCount;
    else
        mColumnCount = 1;

    if (emoteCount % mRowCount > 0)
        ++mColumnCount;

    setContentSize(mColumnCount * gridWidth, mRowCount * gridHeight);
}

void EmotePopup::distributeValueChangedEvent()
{
    gcn::SelectionEvent event(this);
    Listeners::const_iterator i_end = mListeners.end();
    Listeners::const_iterator i;

    for (i = mListeners.begin(); i != i_end; ++i)
    {
        if (*i)
            (*i)->valueChanged(event);
    }
}
