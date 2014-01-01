/*
 *  The ManaPlus Client
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

#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

#include "localconsts.h"

/**
 * Set the milliseconds value of a tick time.
 */
static const int MILLISECONDS_IN_A_TICK = 10;

extern volatile int fps;
extern volatile int lps;
extern volatile int tick_time;
extern volatile int cur_time;
extern volatile int frame_count;
extern volatile int logic_count;

void startTimers();

void stopTimers();

/**
 * Returns elapsed time. (Warning: supposes the delay is always < 100 seconds)
 */
int get_elapsed_time(const int startTime) A_WARN_UNUSED;

int get_elapsed_time1(const int startTime) A_WARN_UNUSED;

#endif  // UTILS_TIMER_H
