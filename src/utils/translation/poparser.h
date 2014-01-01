/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef UTILS_TRANSLATION_POPARSER_H
#define UTILS_TRANSLATION_POPARSER_H

#include "utils/translation/podict.h"

#include "localconsts.h"

#include <sstream>
#include <string>

class PoParser final
{
    public:
        PoParser();

        A_DELETE_COPY(PoParser)

        PoDict *load(const std::string &restrict lang,
                     const std::string &restrict fileName = "",
                     PoDict *restrict const dict = nullptr);

        bool checkLang(std::string lang) const;

        static PoDict *getEmptyDict();

    private:
        void setLang(std::string lang)
        { mLang = lang; }

        void openFile(std::string name);

        bool readLine();

        bool readMsgId();

        bool readMsgStr();

        bool checkLine();

        std::string getFileName(std::string lang) const;

        PoDict *getDict() const;

        void convertStr(std::string &str) const;

        // current lang
        std::string mLang;

        // po file object
        std::istringstream mFile;

        // current line from po file
        std::string mLine;

        std::string mMsgId;

        std::string mMsgStr;

        PoDict *mDict;

        bool mReadingId;

        bool mReadingStr;
};

#endif  // UTILS_TRANSLATION_POPARSER_H
