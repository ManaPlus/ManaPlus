/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "resources/delayedmanager.h"

#include "resources/sprite/animationdelayload.h"

#include "utils/foreach.h"
#include "utils/timer.h"

#include "debug.h"

DelayedAnim DelayedManager::mDelayedAnimations;

void DelayedManager::delayedLoad()
{
    BLOCK_START("DelayedManager::delayedLoad")
    static int loadTime = 0;
    if (loadTime < cur_time)
    {
        loadTime = tick_time;

        int k = 0;
        DelayedAnimIter it = mDelayedAnimations.begin();
        const DelayedAnimIter it_end = mDelayedAnimations.end();
        while (it != it_end && k < 1)
        {
            (*it)->load();
            AnimationDelayLoad *tmp = *it;
            it = mDelayedAnimations.erase(it);
            delete tmp;
            k ++;
        }
        const int time2 = tick_time;
        if (time2 > loadTime)
            loadTime = time2 + (time2 - loadTime) * 2 + 10;
        else
            loadTime = time2 + 3;
    }
    BLOCK_END("DelayedManager::delayedLoad")
}

void DelayedManager::removeDelayLoad(const AnimationDelayLoad
                                     *const delayedLoad)
{
    FOR_EACH (DelayedAnimIter, it, mDelayedAnimations)
    {
        if (*it == delayedLoad)
        {
            mDelayedAnimations.erase(it);
            return;
        }
    }
}
