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

#ifndef UTILS_PERFSTAT_H
#define UTILS_PERFSTAT_H

#include "const/utils/perfstat.h"

#include "localconsts.h"

struct PerfStats
{
    PerfStats()
    {
        for (int f = 0; f < 16; f ++)
            ticks[f] = -1;
    }

    int ticks[16];
};

namespace Perf
{
    void init();
    void nextFrame();
    void selectWorstFrame();
    int getTime(const size_t frameId,
                const size_t counterId);
    int getWorstTime(const size_t counterId);
}

extern PerfStats perfStats[PERFSTAT_MAX];
extern size_t perfFrameId;
extern size_t prevPerfFrameId;
extern int *perfFrame;
extern PerfStats worstFrameStats;
extern int skipPerfFrames;

#define PERF_STAT(n) \
    perfFrame[n] = tick_time

#define PERF_NEXTFRAME() \
    Perf::nextFrame()

#endif  // UTILS_PERFSTAT_H
