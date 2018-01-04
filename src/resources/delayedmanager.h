/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef RESOURCES_DELAYEDMANAGER_H
#define RESOURCES_DELAYEDMANAGER_H

#include <list>

#include "localconsts.h"

class AnimationDelayLoad;

typedef std::list<AnimationDelayLoad*> DelayedAnim;
typedef DelayedAnim::iterator DelayedAnimIter;

/**
 * A class for loading and managing resources.
 */
class DelayedManager final
{
    public:
        DelayedManager()
        { }

        A_DELETE_COPY(DelayedManager)

        static void addDelayedAnimation(AnimationDelayLoad *const animation)
        { mDelayedAnimations.push_back(animation); }

        static void delayedLoad();

        static void removeDelayLoad(const AnimationDelayLoad
                                    *const delayedLoad);


    private:
        static DelayedAnim mDelayedAnimations;
};

#endif  // RESOURCES_DELAYEDMANAGER_H
