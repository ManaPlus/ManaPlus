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

#ifndef UTILS_FOREACH_H
#define UTILS_FOREACH_H

#define FOR_EACH(type, iter, array) for (type iter = array.begin(), \
    iter##_fend = array.end(); iter != iter##_fend; ++ iter)

#define FOR_EACHR(type, iter, array) for (type iter = array.rbegin(), \
    iter##_fend = array.rend(); iter != iter##_fend; ++ iter)

#define FOR_EACHP(type, iter, array) for (type iter = array->begin(), \
    iter##_fend = array->end(); iter != iter##_fend; ++ iter)

#define FOR_EACH_SAFE(type, iter, array) for (type iter = array.begin(); \
    iter != array.end(); ++ iter)

#endif  // UTILS_FOREACH_H
