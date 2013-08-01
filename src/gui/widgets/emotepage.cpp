/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "gui/widgets/emotepage.h"

#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "debug.h"

namespace
{
    static const unsigned int emoteWidth = 17;
    static const unsigned int emoteHeight = 18;
}  // namespace

EmotePage::EmotePage(const Widget2 *const widget) :
    gcn::Widget(),
    Widget2(widget),
    mEmotes(ResourceManager::getInstance()->getImageSet(
        "graphics/sprites/chatemotes.png", emoteWidth, emoteHeight)),
    mSelectedIndex(-1)
{
    addMouseListener(this);
}

EmotePage::~EmotePage()
{
    if (mEmotes)
    {
        mEmotes->decRef();
        mEmotes = nullptr;
    }
}

void EmotePage::draw(gcn::Graphics *graphics)
{
    BLOCK_START("EmotePage::draw")

    if (!mEmotes)
        return;

    const std::vector<Image*> &images = mEmotes->getImages();

    Graphics *const g = static_cast<Graphics*>(graphics);
    const unsigned int width = mDimension.width;
    unsigned int x = 0;
    unsigned int y = 0;

    FOR_EACH(std::vector<Image*>::const_iterator, it, images)
    {
        const Image *const image = *it;
        if (image)
        {
            g->drawImage(image, x, y);
            x += emoteWidth;
            if (x + emoteWidth > width)
            {
                x = 0;
                y += emoteHeight;
            }
        }
    }

    BLOCK_END("EmotePage::draw")
}

void EmotePage::mousePressed(gcn::MouseEvent &mouseEvent)
{
    mSelectedIndex = getIndexFromGrid(mouseEvent.getX(), mouseEvent.getY());
    distributeActionEvent();
}

int EmotePage::getIndexFromGrid(const int x, const int y) const
{
    const int width = mDimension.width;
    if (x < 0 || x > width || y < 0 || y > mDimension.height)
        return -1;
    const int cols = width / emoteWidth;
    const unsigned int index = (y / emoteHeight) * cols + (x / emoteWidth);
    if (index >= mEmotes->size())
        return -1;
    return index;
}

void EmotePage::resetAction()
{
    mSelectedIndex = -1;
}
