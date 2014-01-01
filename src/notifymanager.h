/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <string>

namespace NotifyManager
{
    void notify(const unsigned int message);
    void notify(const unsigned int message, const int num);
    void notify(const unsigned int message, const std::string &str);
    int getIndexBySound(const std::string &sound);
}  // namespace NotifyManager

#endif  // NOTIFYMANAGER_H
