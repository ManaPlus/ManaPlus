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

#ifndef UTILS_LIKELY_H
#define UTILS_LIKELY_H

#ifdef __GNUC__
#define A_LIKELY(x) __builtin_expect (!!(x), 1)
#define A_UNLIKELY(x) __builtin_expect (!!(x), 0)
#else  // __GNUC__
#define A_LIKELY(x) (x)
#define A_UNLIKELY(x) (x)
#endif  // __GNUC__

#endif  // UTILS_LIKELY_H
