/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#ifndef UTILS_VIRTFSFUNCS_H
#define UTILS_VIRTFSFUNCS_H

#include "enums/simpletypes/append.h"

#include "localconsts.h"

#include <string>

struct VirtFile;

struct VirtFsFuncs
{
    int (*close) (VirtFile *restrict const file);
    int64_t (*read) (VirtFile *restrict const file,
                     void *restrict const buffer,
                     const uint32_t objSize,
                     const uint32_t objCount);
    int64_t (*write) (VirtFile *restrict const file,
                      const void *restrict const buffer,
                      const uint32_t objSize,
                      const uint32_t objCount);
    int64_t (*fileLength) (VirtFile *restrict const file);
    int64_t (*tell) (VirtFile *restrict const file);
    int (*seek) (VirtFile *restrict const file,
                 const uint64_t pos);
    int (*eof) (VirtFile *restrict const file);
};

#endif  // UTILS_VIRTFSFUNCS_H
