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

#include "utils/langs.h"

#include "configuration.h"

#include "debug.h"

static const char *getLangName()
{
    const char *const lang = getenv("LANG");
    if (lang && strlen(lang) > 1000)
        return nullptr;
    return lang;
}

LangVect getLang()
{
    LangVect langs;
    std::string lang = config.getStringValue("lang").c_str();
    if (lang.empty())
    {
        const char *const lng = getLangName();
        if (!lng)
            return langs;
        lang = lng;
    }
    else if (lang.size() > 1000)
    {
        return langs;
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
    const std::string lang = config.getStringValue("lang").c_str();
    if (lang.empty())
    {
        const char *const lng = getLangName();
        if (!lng)
            return "";
        return lng;
    }
    else if (lang.size() > 1000)
    {
        return "";
    }
    return lang;
}

std::string getLangShort()
{
    std::string lang = config.getStringValue("lang").c_str();
    if (lang.empty())
    {
        const char *const lng = getLangName();
        if (!lng)
            return "";
        lang = lng;
    }
    else if (lang.size() > 1000)
    {
        return "";
    }

    size_t dot = lang.find(".");
    if (dot != std::string::npos)
        lang = lang.substr(0, dot);
    dot = lang.find("_");
    if (dot != std::string::npos)
        return lang.substr(0, dot);
    return lang;
}
