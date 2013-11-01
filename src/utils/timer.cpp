/*
 *  The ManaPlus Client
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

#include "utils/timer.h"

#include "SDL_timer.h"

#include <climits>

#include "debug.h"

namespace
{
#ifdef USE_SDL2
    SDL_TimerID mLogicCounterId(0);
    SDL_TimerID mSecondsCounterId(0);
#else
    SDL_TimerID mLogicCounterId(nullptr);
    SDL_TimerID mSecondsCounterId(nullptr);
#endif
}  // namespace

/**
 * Tells the max tick value,
 * setting it back to zero (and start again).
 */
static const int MAX_TICK_VALUE = INT_MAX / 2;

volatile int tick_time;       /**< Tick counter */
volatile int fps = 0;         /**< Frames counted in the last second */
volatile int lps = 0;         /**< Logic processed per second */
volatile int frame_count = 0; /**< Counts the frames during one second */
volatile int logic_count = 0; /**< Counts the logic during one second */
volatile int cur_time;

static uint32_t nextTick(uint32_t interval, void *param A_UNUSED);
static uint32_t nextSecond(uint32_t interval, void *param A_UNUSED);

/**
 * Advances game logic counter.
 * Called every 10 milliseconds by SDL_AddTimer()
 * @see MILLISECONDS_IN_A_TICK value
 */
static uint32_t nextTick(uint32_t interval, void *param A_UNUSED)
{
    tick_time++;
    if (tick_time == MAX_TICK_VALUE)
        tick_time = 0;
    return interval;
}

/**
 * Updates fps.
 * Called every seconds by SDL_AddTimer()
 */
static uint32_t nextSecond(uint32_t interval, void *param A_UNUSED)
{
    fps = frame_count;
    lps = logic_count;
    frame_count = 0;
    logic_count = 0;

    return interval;
}

/**
 * @return the elapsed time in milliseconds
 * between two tick values.
 */
int get_elapsed_time(const int startTime)
{
    const int time = tick_time;
    if (startTime <= time)
    {
        return (time - startTime) * MILLISECONDS_IN_A_TICK;
    }
    else
    {
        return (time + (MAX_TICK_VALUE - startTime))
            * MILLISECONDS_IN_A_TICK;
    }
}

int get_elapsed_time1(const int startTime)
{
    const int time = tick_time;
    if (startTime <= time)
        return time - startTime;
    else
        return time + (MAX_TICK_VALUE - startTime);
}

void startTimers()
{
    // Initialize logic and seconds counters
    tick_time = 0;
    mLogicCounterId = SDL_AddTimer(MILLISECONDS_IN_A_TICK, nextTick, nullptr);
    mSecondsCounterId = SDL_AddTimer(1000, nextSecond, nullptr);
}

void stopTimers()
{
    SDL_RemoveTimer(mLogicCounterId);
    SDL_RemoveTimer(mSecondsCounterId);
}
