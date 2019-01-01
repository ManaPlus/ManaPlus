/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#ifndef UTILS_VIRTFSTOOLS_H
#define UTILS_VIRTFSTOOLS_H

#include "enums/simpletypes/append.h"

#include "utils/stringvector.h"

#include "localconsts.h"

namespace VirtFs
{
    void searchAndAddArchives(const std::string &restrict path,
                              const std::string &restrict ext,
                              const Append append);
    void searchAndRemoveArchives(const std::string &restrict path,
                                 const std::string &restrict ext);
    void getFilesInDir(const std::string &dir,
                       StringVect &list,
                       const std::string &ext);
    std::string getPath(const std::string &file);
    bool loadTextFile(const std::string &fileName,
                      StringVect &lines);
    std::string loadTextFileString(const std::string &fileName);
}  // namespace VirtFs

#endif  // UTILS_VIRTFSTOOLS_H
