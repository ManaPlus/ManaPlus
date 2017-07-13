/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "render/graphics.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imageset.h"

#include "resources/loaders/imagesetloader.h"

#include "utils/delete2.h"
#include "utils/foreach.h"

#include "debug.h"

namespace
{
    const unsigned int emoteWidth = 17;
    const unsigned int emoteHeight = 18;
}  // namespace

EmotePage::EmotePage(const Widget2 *const widget) :
    Widget(widget),
    MouseListener(),
    WidgetListener(),
    mEmotes(Loader::getImageSet(
        "graphics/sprites/chatemotes.png", emoteWidth, emoteHeight)),
    mVertexes(new ImageCollection),
    mSelectedIndex(-1)
{
    addMouseListener(this);
    addWidgetListener(this);
    mAllowLogic = false;
}

EmotePage::~EmotePage()
{
    if (mEmotes != nullptr)
    {
        mEmotes->decRef();
        mEmotes = nullptr;
    }
    delete2(mVertexes);
}

void EmotePage::draw(Graphics *const graphics)
{
    BLOCK_START("EmotePage::draw")

    if (mRedraw)
    {
        if (mEmotes == nullptr)
            return;

        const STD_VECTOR<Image*> &images = mEmotes->getImages();

        const unsigned int width = mDimension.width;
        unsigned int x = 0;
        unsigned int y = 0;

        mRedraw = false;
        mVertexes->clear();
        FOR_EACH (STD_VECTOR<Image*>::const_iterator, it, images)
        {
            const Image *const image = *it;
            if (image != nullptr)
            {
                graphics->calcTileCollection(mVertexes, image, x, y);
                x += emoteWidth;
                if (x + emoteWidth > width)
                {
                    x = 0;
                    y += emoteHeight;
                }
            }
        }
        graphics->finalize(mVertexes);
    }
    graphics->drawTileCollection(mVertexes);

    BLOCK_END("EmotePage::draw")
}

void EmotePage::safeDraw(Graphics *const graphics)
{
    BLOCK_START("EmotePage::safeDraw")

    if (mEmotes == nullptr)
        return;

    const STD_VECTOR<Image*> &images = mEmotes->getImages();

    const unsigned int width = mDimension.width;
    unsigned int x = 0;
    unsigned int y = 0;

    FOR_EACH (STD_VECTOR<Image*>::const_iterator, it, images)
    {
        const Image *const image = *it;
        if (image != nullptr)
        {
            graphics->drawImage(image, x, y);
            x += emoteWidth;
            if (x + emoteWidth > width)
            {
                x = 0;
                y += emoteHeight;
            }
        }
    }

    BLOCK_END("EmotePage::safeDraw")
}

void EmotePage::mousePressed(MouseEvent &event)
{
    mSelectedIndex = getIndexFromGrid(event.getX(), event.getY());
    event.consume();
    distributeActionEvent();
}

int EmotePage::getIndexFromGrid(const int x, const int y) const
{
    const int width = mDimension.width;
    if (x < 0 || x > width || y < 0 || y > mDimension.height)
        return -1;
    const int cols = width / emoteWidth;
    const int index = (y / emoteHeight) * cols + (x / emoteWidth);
    if (index >= CAST_S32(mEmotes->size()))
        return -1;
    return index;
}

void EmotePage::resetAction()
{
    mSelectedIndex = -1;
}

void EmotePage::widgetResized(const Event &event A_UNUSED)
{
    mRedraw = true;
}

void EmotePage::widgetMoved(const Event &event A_UNUSED)
{
    mRedraw = true;
}
