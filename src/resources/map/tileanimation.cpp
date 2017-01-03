/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "resources/map/tileanimation.h"

#include "resources/animation/simpleanimation.h"

#include "resources/map/maplayer.h"

#include "utils/delete2.h"

#include "debug.h"

TileAnimation::TileAnimation(Animation *const ani) :
    mAffected(),
    mAnimation(new SimpleAnimation(ani)),
    mLastImage(nullptr)
{
}

TileAnimation::~TileAnimation()
{
    delete2(mAnimation);
}

bool TileAnimation::update(const int ticks)
{
    if (!mAnimation)
        return false;

    // update animation
    if (!mAnimation->update(ticks))
        return false;

    // exchange images
    Image *const img = mAnimation->getCurrentImage();
    if (img != mLastImage)
    {
        FOR_EACH (TilePairVectorCIter, i, mAffected)
        {
            if (i->first)
                i->first->setTile(i->second, img);
        }
        mLastImage = img;
    }
    return true;
}
