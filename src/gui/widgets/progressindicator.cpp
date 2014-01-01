/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "gui/widgets/progressindicator.h"

#include "simpleanimation.h"

#include "resources/animation.h"
#include "resources/imageset.h"

#include "debug.h"

ProgressIndicator::ProgressIndicator() :
    gcn::Widget(),
    Widget2(),
    mIndicator(nullptr)
{
    ImageSet *const images = Theme::getImageSetFromTheme(
        "progress-indicator.png", 32, 32);

    if (images)
    {
        Animation *const anim = new Animation;
        for (ImageSet::size_type i = 0, sz = images->size(); i < sz; ++i)
             anim->addFrame(images->get(i), 100, 0, 0, 100);
        mIndicator = new SimpleAnimation(anim);
        images->decRef();
    }

    setSize(32, 32);
}

ProgressIndicator::~ProgressIndicator()
{
    if (gui)
        gui->removeDragged(this);

    delete mIndicator;
    mIndicator = nullptr;
}

void ProgressIndicator::logic()
{
    BLOCK_START("ProgressIndicator::logic")
    if (mIndicator)
        mIndicator->update(10);
    BLOCK_END("ProgressIndicator::logic")
}

void ProgressIndicator::draw(gcn::Graphics *graphics)
{
    BLOCK_START("ProgressIndicator::draw")
    if (mIndicator)
    {
        // Draw the indicator centered on the widget
        const int x = (mDimension.width - 32) / 2;
        const int y = (mDimension.height - 32) / 2;
        mIndicator->draw(static_cast<Graphics*>(graphics), x, y);
    }
    BLOCK_END("ProgressIndicator::draw")
}
