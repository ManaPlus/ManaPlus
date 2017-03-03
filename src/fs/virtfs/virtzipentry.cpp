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

#ifndef USE_PHYSFS

#include "fs/virtfs/virtzipentry.h"

#include "fs/virtfs/ziplocalheader.h"

#include "utils/dtor.h"

#include "debug.h"

VirtZipEntry::VirtZipEntry(const std::string &restrict archiveName,
                           VirtFsFuncs *restrict const funcs0) :
    VirtFsEntry(FsEntryType::Zip, funcs0),
    mHeaders()
{
    root = archiveName;
}

VirtZipEntry::~VirtZipEntry()
{
    delete_all(mHeaders);
}
#endif  // USE_PHYSFS
