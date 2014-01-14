/*
 *  The ManaPlus Client
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

#ifndef UTILS_PATHS_H
#define UTILS_PATHS_H

#include <string>

#include "localconsts.h"

std::string getRealPath(const std::string &str) A_WARN_UNUSED;

bool isRealPath(const std::string &str) A_WARN_UNUSED;

bool checkPath(const std::string &path) A_WARN_UNUSED;

std::string &fixDirSeparators(std::string &str);

std::string removeLast(const std::string &str) A_WARN_UNUSED;

std::string getSelfName() A_WARN_UNUSED;

std::string getPicturesDir() A_WARN_UNUSED;

#ifdef ANDROID
std::string getSdStoragePath() A_WARN_UNUSED;
#endif

std::string getPackageDir() A_WARN_UNUSED;

void setPackageDir(const std::string &dir);

#endif  // UTILS_PATHS_H
