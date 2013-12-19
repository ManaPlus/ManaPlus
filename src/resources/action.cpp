/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "resources/action.h"

#include "resources/animation.h"
#include "resources/spritedef.h"

#include "utils/dtor.h"

#include "debug.h"

Action::Action() noexcept :
    mAnimations(),
    mNumber(100)
{
}

Action::~Action()
{
    delete_all(mAnimations);
}

const Animation *Action::getAnimation(int direction) const noexcept
{
    Animations::const_iterator i = mAnimations.find(direction);

    if (i == mAnimations.end())
    {
        if (direction == DIRECTION_UPLEFT || direction == DIRECTION_UPRIGHT)
        {
            direction = DIRECTION_UP;
        }
        else if (direction == DIRECTION_DOWNLEFT
                 || direction == DIRECTION_DOWNRIGHT)
        {
            direction = DIRECTION_DOWN;
        }
        i = mAnimations.find(direction);

        // When the given direction is not available, return the first one.
        // (either DEFAULT, or more usually DOWN).
        if (i == mAnimations.end())
            i = mAnimations.begin();
    }

    return (i == mAnimations.end()) ? nullptr : i->second;
}

void Action::setAnimation(const int direction,
                          Animation *const animation) noexcept
{
    mAnimations[direction] = animation;
}

void Action::setLastFrameDelay(const int delay) noexcept
{
    FOR_EACH (AnimationIter, it, mAnimations)
    {
        Animation *const animation = (*it).second;
        if (!animation)
            continue;
        animation->setLastFrameDelay(delay);
    }
}
