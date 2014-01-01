/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef RESOURCES_ACTION_H
#define RESOURCES_ACTION_H

#include <map>

#include "localconsts.h"

class Animation;

/**
 * An action consists of several animations, one for each direction.
 */
class Action final
{
    public:
        Action() noexcept;

        A_DELETE_COPY(Action)

        ~Action();

        void setAnimation(const int direction,
                          Animation *const animation) noexcept;

        const Animation *getAnimation(int direction) const
                                      noexcept A_WARN_UNUSED;

        unsigned getNumber() const noexcept A_WARN_UNUSED
        { return mNumber; }

        void setNumber(const unsigned n) noexcept
        { mNumber = n; }

        void setLastFrameDelay(const int delay) noexcept;

    protected:
        typedef std::map<int, Animation*> Animations;
        typedef Animations::iterator AnimationIter;

        Animations mAnimations;
        unsigned mNumber;
};

#endif  // RESOURCES_ACTION_H
