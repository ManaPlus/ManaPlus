/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include <map>
#include <string>

#include "localconsts.h"

typedef std::map <std::string, std::string> PoMap;

class PoDict final
{
    public:
        explicit PoDict(std::string lang);

        A_DELETE_COPY(PoDict)

        ~PoDict();

        const std::string getStr(const std::string &str);

        const char *getChar(const char *const str);

#ifndef UNITTESTS
    protected:
#endif  // UNITTESTS
        friend class PoParser;

        PoMap *getMap()
        { return &mPoLines; }

        void set(const std::string &key, const std::string &value)
        { mPoLines[key] = value; }

        void setLang(const std::string &lang)
        { mLang = lang; }

    private:
        PoMap mPoLines;
        std::string mLang;
};

extern PoDict *translator;
#ifdef ENABLE_CUSTOMNLS
extern PoDict *mainTranslator;
#endif  // ENABLE_CUSTOMNLS

#endif  // UTILS_TRANSLATION_PODICT_H
