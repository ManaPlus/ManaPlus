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

#include "fs/virtfs/file.h"

#include "debug.h"

namespace VirtFs
{

File::File(const FsFuncs *restrict const funcs0,
           const uint8_t *restrict const buf,
           const size_t sz) :
    funcs(funcs0),
    mBuf(buf),
    mPos(0U),
    mSize(sz),
    mFd(FILEHDEFAULT)
{
}

File::File(const FsFuncs *restrict const funcs0,
           FILEHTYPE fd) :
    funcs(funcs0),
    mBuf(nullptr),
    mPos(0U),
    mSize(0U),
    mFd(fd)
{
}

File::~File()
{
    if (mFd != FILEHDEFAULT)
        FILECLOSE(mFd);
    delete [] mBuf;
}

}  // namespace VirtFs
