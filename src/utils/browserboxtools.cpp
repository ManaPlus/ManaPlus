/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "utils/browserboxtools.h"

#include "main.h"
#include "settings.h"

#include "input/inputmanager.h"

#include "utils/stringutils.h"

#include "debug.h"

void BrowserBoxTools::replaceVars(std::string &data)
{
    data = replaceAll(data, "%VER%", SMALL_VERSION);
    data = replaceAll(data, "%SUPPORTURL%", settings.supportUrl);
}

void BrowserBoxTools::replaceKeys(std::string &data)
{
    size_t idx1 = data.find("###");
    while (idx1 != std::string::npos)
    {
        const size_t idx2 = data.find(';', idx1);
        if (idx2 == std::string::npos)
            break;

        const std::string str = inputManager.getKeyValueByNameLong(
            data.substr(idx1 + 3, idx2 - idx1 - 3));
        data.replace(idx1, idx2 - idx1 + 1, str);

        idx1 = data.find("###");
    }
}
