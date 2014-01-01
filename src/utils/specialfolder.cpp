/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifdef WIN32
#include "utils/specialfolder.h"
#include <windows.h>

#include "debug.h"

/*
 * Retrieve the pathname of special folders on win32, or an empty string
 * on error / if the folder does not exist.
 * See http://msdn.microsoft.com/en-us/library/bb762494(VS.85).aspx for
 * a list of folder ids
 */
std::string getSpecialFolderLocation(const int folderId)
{
    std::string ret;
    LPITEMIDLIST pItemIdList;
    LPMALLOC pMalloc;
    char szPath[_MAX_PATH];

    // get the item ID list for folderId
    HRESULT hr = SHGetSpecialFolderLocation(nullptr, folderId, &pItemIdList);
    if (hr != S_OK)
        return ret;

    // convert the ID list into a file system path
    if (SHGetPathFromIDList(pItemIdList, szPath) == FALSE)
        return ret;

    // get the IMalloc pointer and free all resources we used
    hr = SHGetMalloc(&pMalloc);
    pMalloc->Free(pItemIdList);
    pMalloc->Release();

    ret = szPath;
    return ret;
}

#endif
