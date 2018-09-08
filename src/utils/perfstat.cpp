/*
 *  The ManaPlus Client
 *  Copyright (C) 2018  The ManaPlus Developers
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

#include "utils/perfstat.h"

#include "logger.h"

#include "utils/timer.h"

#include "debug.h"

PerfStats perfStats[PERFSTAT_MAX];

size_t perfFrameId = 0;
size_t prevPerfFrameId = PERFSTAT_MAX;
int *perfFrame = &perfStats[perfFrameId].ticks[0];
PerfStats worstFrameStats;
int worstTime = -1;
int skipPerfFrames = -1;

namespace Perf
{
    void init()
    {
        logger->log("perf stats init");
        worstTime = -1;
        perfFrameId = 0;
        prevPerfFrameId = 0;
        for (size_t f = 0; f < PERFSTAT_LAST_STAT; f ++)
        {
            worstFrameStats.ticks[f] = 0;
        }
        for (size_t f = 0; f < PERFSTAT_MAX; f ++)
        {
            PerfStats &perf = perfStats[f];
            for (size_t d = 0; d < PERFSTAT_LAST_STAT; d ++)
            {
                perf.ticks[d] = 0;
            }
        }
        skipPerfFrames = 0;
    }

    void nextFrame()
    {
        if (skipPerfFrames > 0)
        {
            skipPerfFrames --;
//            logger->log("skip frames: %d", skipPerfFrames);
            return;
        }
        else if (skipPerfFrames < 0)
        {
            return;
        }
        prevPerfFrameId = perfFrameId;
        perfFrameId ++;
        if (perfFrameId >= PERFSTAT_MAX)
        {
            perfFrameId = 0;
            selectWorstFrame();
        }
        perfFrame = &perfStats[perfFrameId].ticks[0];
    }

    void selectWorstFrame()
    {
        int time = worstTime;
        int index = -1;
        for (size_t f = 0; f < PERFSTAT_MAX; f ++)
        {
            if (f == perfFrameId)
                continue;
            const int time1 = Perf::getTime(f, PERFSTAT_FPS_STAT - 1);
            if (time1 > time)
            {
                time = time1;
                index = f;
            }
        }
        if (index >= 0)
        {
            worstFrameStats = perfStats[index];
            logger->log("worst frame: %d, %d",
                perfStats[index].ticks[PERFSTAT_FPS_STAT - 1] -
                perfStats[index].ticks[0],
                worstFrameStats.ticks[PERFSTAT_FPS_STAT - 1] -
                worstFrameStats.ticks[0]);
            worstTime = time;
        }
    }

    int getTime(const size_t frameId,
                const size_t counterId)
    {
        const PerfStats &perf = perfStats[frameId];
        const int val1 = perf.ticks[0];
        const int val2 = perf.ticks[counterId];
        if (val2 >= val1)
            return val2 - val1;
        return val1 - val1;
    }

    int getWorstTime(const size_t counterId)
    {
        const int val1 = worstFrameStats.ticks[0];
        const int val2 = worstFrameStats.ticks[counterId];
        if (val2 >= val1)
            return val2 - val1;
        return val1 - val1;
    }
}
