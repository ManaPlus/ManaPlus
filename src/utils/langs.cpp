/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "utils/langs.h"

#include "configuration.h"

#include <string.h>
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <list>

#include "debug.h"

LangVect getLang()
{
    LangVect langs;

    std::string lang = config.getStringValue("lang").c_str();
    if (lang.empty())
    {
        const char *const lng = getenv("LANG");
        if (!lng)
            return langs;
        lang = lng;
    }

    size_t dot = lang.find(".");
    if (dot != std::string::npos)
        lang = lang.substr(0, dot);
    langs.push_back(lang);
    dot = lang.find("_");
    if (dot != std::string::npos)
        langs.push_back(lang.substr(0, dot));
    return langs;
}

std::string getLangSimple()
{
    std::string lang = config.getStringValue("lang").c_str();
    if (lang.empty())
    {
        const char *const lng = getenv("LANG");
        if (!lng)
            return "";
        return lng;
    }
    return lang;
}

std::string getLangShort()
{
    std::string lang = config.getStringValue("lang").c_str();
    if (lang.empty())
    {
        const char *const lng = getenv("LANG");
        if (!lng)
            return "";
        lang = lng;
    }

    size_t dot = lang.find(".");
    if (dot != std::string::npos)
        lang = lang.substr(0, dot);
    dot = lang.find("_");
    if (dot != std::string::npos)
        return lang.substr(0, dot);
    return lang;
}
