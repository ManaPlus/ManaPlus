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

#ifdef ANDROID
#include <string>

namespace Files
{
    void extractLocale();

    void copyPhysFsFile(const std::string &inFile, const std::string &outFile);

    void copyPhysFsDir(const std::string &inDir, const std::string &outDir);

    void extractZip(const std::string &zipName, const std::string &inDir,
                    const std::string &outDir);
}  // namespace Files

#endif  // ANDROID
#endif  // UTILS_FILES_H
