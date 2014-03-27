/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/shortcutcontainer.h"

#include "graphicsvertexes.h"

#include "gui/gui.h"

#include "utils/delete2.h"

#include "debug.h"

float ShortcutContainer::mAlpha = 1.0;

ShortcutContainer::ShortcutContainer(Widget2 *const widget) :
    Widget(widget),
    WidgetListener(),
    MouseListener(),
    mBackgroundImg(nullptr),
    mMaxItems(0),
    mBoxWidth(1),
    mBoxHeight(1),
    mGridWidth(1),
    mGridHeight(1),
    mVertexes(new ImageCollection),
    mRedraw(true)
{
    mAllowLogic = false;
}

ShortcutContainer::~ShortcutContainer()
{
    if (gui)
        gui->removeDragged(this);

    delete2(mVertexes);
}

void ShortcutContainer::widgetResized(const Event &event A_UNUSED)
{
    mGridWidth = mDimension.width / mBoxWidth;

    if (mGridWidth < 1)
        mGridWidth = 1;

    mGridHeight = mMaxItems / static_cast<unsigned int>(mGridWidth);

    if (mMaxItems % mGridWidth != 0 || mGridHeight < 1)
        ++mGridHeight;

    setHeight(mGridHeight * mBoxHeight);
    mRedraw = true;
}

int ShortcutContainer::getIndexFromGrid(const int pointX,
                                        const int pointY) const
{
    const Rect tRect = Rect(0, 0,
        mGridWidth * mBoxWidth, mGridHeight * mBoxHeight);

    int index = ((pointY / mBoxHeight) * mGridWidth) + pointX / mBoxWidth;

    if (!tRect.isPointInRect(pointX, pointY) ||
        index >= static_cast<int>(mMaxItems) || index < 0)
    {
        index = -1;
    }

    return index;
}

void ShortcutContainer::drawBackground(Graphics *g)
{
    if (mBackgroundImg)
    {
        if (isBatchDrawRenders(openGLMode))
        {
            if (mRedraw)
            {
                mRedraw = false;
                mVertexes->clear();
                for (unsigned i = 0; i < mMaxItems; i ++)
                {
                    g->calcTileCollection(mVertexes, mBackgroundImg,
                        (i % mGridWidth) * mBoxWidth,
                        (i / mGridWidth) * mBoxHeight);
                }
            }
            g->drawTileCollection(mVertexes);
        }
        else
        {
            for (unsigned i = 0; i < mMaxItems; i ++)
            {
                g->drawImage(mBackgroundImg, (i % mGridWidth) * mBoxWidth,
                    (i / mGridWidth) * mBoxHeight);
            }
        }
    }
}

void ShortcutContainer::widgetMoved(const Event& event A_UNUSED)
{
    mRedraw = true;
}
