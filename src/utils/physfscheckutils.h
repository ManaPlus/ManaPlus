/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef UTILS_PHYSFSCHECKUTILS_H
#define UTILS_PHYSFSCHECKUTILS_H

#include "localconsts.h"
#ifdef DEBUG_PHYSFS

#include <SDL.h>

SDL_RWops *FakePHYSFSRWOPS_openRead(const char *restrict const name,
                                    const char *restrict const file,
                                    const unsigned line);

void FakePhysFSClose(SDL_RWops *const rwops);

void reportPhysfsLeaks();

#endif  // DEBUG_PHYSFS
#endif  // UTILS_PHYSFSCHECKUTILS_H
