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

#ifndef UTILS_PROCESS_H
#define UTILS_PROCESS_H

#include <string>

int execFileWait(const std::string &pathName, const std::string &name,
                 const std::string &arg1, const std::string &arg2,
                 int waitTime = 0);

bool execFile(const std::string &pathName, const std::string &name,
              const std::string &arg1, const std::string &arg2);

bool openBrowser(std::string url);

void setPriority(const bool big);

#endif  // UTILS_PROCESS_H
