/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/progressindicator.h"

#include "gui/gui.h"

#include "resources/imageset.h"

#include "resources/animation/animation.h"
#include "resources/animation/simpleanimation.h"

#include "utils/delete2.h"

#include "debug.h"

ProgressIndicator::ProgressIndicator(const Widget2 *const widget) :
    Widget(widget),
    mIndicator(nullptr)
{
    ImageSet *const images = Theme::getImageSetFromTheme(
        "progress-indicator.png", 32, 32);

    if (images != nullptr)
    {
        Animation *const anim = new Animation("progress indicator");
        for (ImageSet::size_type i = 0, fsz = images->size();
             i < fsz;
             ++i)
        {
            anim->addFrame(images->get(i), 100, 0, 0, 100);
        }
        mIndicator = new SimpleAnimation(anim);
        images->decRef();
    }

    setSize(32, 32);
}

ProgressIndicator::~ProgressIndicator()
{
    if (gui != nullptr)
        gui->removeDragged(this);

    delete2(mIndicator)
}

void ProgressIndicator::logic()
{
    BLOCK_START("ProgressIndicator::logic")
    if (mIndicator != nullptr)
        mIndicator->update(10);
    BLOCK_END("ProgressIndicator::logic")
}

void ProgressIndicator::draw(Graphics *const graphics)
{
    BLOCK_START("ProgressIndicator::draw")
    if (mIndicator != nullptr)
    {
        // Draw the indicator centered on the widget
        const int x = (mDimension.width - 32) / 2;
        const int y = (mDimension.height - 32) / 2;
        mIndicator->draw(graphics, x, y);
    }
    BLOCK_END("ProgressIndicator::draw")
}

void ProgressIndicator::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ProgressIndicator::draw")
    if (mIndicator != nullptr)
    {
        // Draw the indicator centered on the widget
        const int x = (mDimension.width - 32) / 2;
        const int y = (mDimension.height - 32) / 2;
        mIndicator->draw(graphics, x, y);
    }
    BLOCK_END("ProgressIndicator::draw")
}
