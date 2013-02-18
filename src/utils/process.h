/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef UTILS_PROCESS_H
#define UTILS_PROCESS_H

#include <string>

int execFileWait(std::string pathName, std::string name,
                 std::string arg1, std::string arg2, int waitTime = 0);

bool execFile(std::string pathName, std::string name,
              std::string arg1, std::string arg2);

bool openBrowser(std::string url);

void setPriority(const bool big);

#endif // UTILS_PROCESS_H
