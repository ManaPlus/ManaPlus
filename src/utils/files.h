/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef UTILS_FILES_H
#define UTILS_FILES_H

#include <string>

namespace Files
{
#ifdef ANDROID
    void extractLocale();
#endif

#if defined(ANDROID) || defined(__native_client__)
    typedef void (*CopyFileCallbackPtr) (int cnt);

    void setCopyCallBack(CopyFileCallbackPtr callback);

    void copyPhysFsFile(const std::string &inFile, const std::string &outFile);

    void copyPhysFsDir(const std::string &inDir, const std::string &outDir);

    void extractZip(const std::string &zipName, const std::string &inDir,
                    const std::string &outDir);
#endif  // ANDROID __native_client__

    int renameFile(const std::string &pFrom, const std::string &pTo);
}  // namespace Files

#endif  // UTILS_FILES_H
