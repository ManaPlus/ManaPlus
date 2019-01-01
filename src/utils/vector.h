/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2019  The ManaPlus Developers
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

#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#ifdef ENABLE_STLDEBUG
#include "debug/mse/msemstdvector.h"
#define STD_VECTOR mse::mstd::vector
#else  // ENABLE_STLDEBUG
#include <vector>
#define STD_VECTOR std::vector
#endif  // ENABLE_STLDEBUG

#endif  // UTILS_VECTOR_H
