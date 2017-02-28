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

#include "fs/virtfs/virtfileprivate.h"

#include <unistd.h>
#include <zlib.h>

#include "debug.h"

VirtFilePrivate::VirtFilePrivate() :
    mBuf(nullptr),
    mPos(0U),
    mSize(0U),
    mFd(-1)
{
}

VirtFilePrivate::VirtFilePrivate(const int fd) :
    mBuf(nullptr),
    mPos(0U),
    mSize(0U),
    mFd(fd)
{
}

VirtFilePrivate::VirtFilePrivate(uint8_t *restrict const buf,
                                 const size_t sz) :
    mBuf(buf),
    mPos(0U),
    mSize(sz),
    mFd(-1)
{
}

VirtFilePrivate::~VirtFilePrivate()
{
    if (mFd != -1)
        close(mFd);
    if (mBuf)
        delete [] mBuf;
}
