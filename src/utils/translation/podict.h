/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef UTILS_TRANSLATION_PODICT_H
#define UTILS_TRANSLATION_PODICT_H

#include <string>
#include <map>

typedef std::map <std::string, std::string> PoMap;

class PoDict final
{
    public:
        PoDict(std::string lang);

        ~PoDict();

        const std::string getStr(const std::string &str);

        const char *getChar(const char *const str);

    protected:
        friend class PoParser;

        PoMap *getMap()
        { return &mPoLines; }

        void set(std::string key, std::string value)
        { mPoLines[key] = value; }

        void setLang(std::string lang)
        { mLang = lang; }

    private:
        PoMap mPoLines;
        std::string mLang;
};

extern PoDict *translator;

#endif // UTILS_TRANSLATION_PODICT_H
