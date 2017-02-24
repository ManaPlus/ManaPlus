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

#include "fs/virtfileprivate.h"

#include <unistd.h>
#include <zlib.h>

#include "debug.h"

VirtFilePrivate::VirtFilePrivate() :
    mFile(nullptr),
    mFd(-1)
{
}

VirtFilePrivate::VirtFilePrivate(const int fd) :
    mFile(nullptr),
    mFd(fd)
{
}

VirtFilePrivate::VirtFilePrivate(PHYSFS_file *restrict const file) :
    mFile(file),
    mFd(-1)
{
}

VirtFilePrivate::~VirtFilePrivate()
{
    if (mFile != nullptr)
        PHYSFS_close(mFile);
    if (mFd != -1)
        close(mFd);
}
