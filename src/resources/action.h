/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef ACTION_H
#define ACTION_H

#include "utils/xml.h"

#include <map>

class Animation;


/**
 * An action consists of several animations, one for each direction.
 */
class Action
{
    public:
        Action();

        ~Action();

        void setAnimation(int direction, Animation *animation);

        Animation *getAnimation(int direction) const;

        unsigned getNumber() const
        { return mNumber; }

        void setNumber(unsigned n)
        { mNumber = n; }

        void setLastFrameDelay(int delay);

    protected:
        typedef std::map<int, Animation*> Animations;
        typedef Animations::iterator AnimationIter;

        Animations mAnimations;
        unsigned mNumber;
};

#endif
