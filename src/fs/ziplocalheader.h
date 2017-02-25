/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#ifndef UTILS_ZIPLOCALHEADER_H
#define UTILS_ZIPLOCALHEADER_H

#include "localconsts.h"

#include <string>

struct VirtZipEntry;

struct ZipLocalHeader final
{
    ZipLocalHeader();

    A_DELETE_COPY(ZipLocalHeader)

    std::string fileName;
    VirtZipEntry *zipEntry;
    uint32_t dataOffset;
    uint32_t compressSize;
    uint32_t uncompressSize;
    bool compressed;
};

#endif  // UTILS_ZIPLOCALHEADER_H
