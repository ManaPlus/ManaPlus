/*
 *  The ManaPlus Client
 *  Copyright (C) 2016  The ManaPlus Developers
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

#include "utils/paramerers.h"

#include "logger.h"

#include "utils/stringutils.h"

#include <sstream>

#include "debug.h"

static inline void addToken(StringVect &tokens,
                            std::string str)
{
    const size_t sz = str.size();
    std::string item;
    if (sz > 1)
    {
        if (str[0] == '\"' &&
            str[sz - 1] == '\"' &&
            str[sz - 2] != '\\')
        {
            str = str.substr(1, sz - 2);
            item = trim(str);
            replaceAll(item, "\\\"", "\"");
            tokens.push_back(item);
            return;
        }
    }
    item = trim(str);
    replaceAll(item, "\\\"", "\"");
    if (!item.empty())
        tokens.push_back(item);
}

static inline size_t findNextQuote(const std::string &str,
                                   const char quote,
                                   const size_t pos)
{
    size_t idx = str.find(quote, pos);
    if (idx == std::string::npos)
        return idx;
    while (idx > 0 && str[idx - 1] == '\\')
        idx = str.find(quote, idx + 1);
    return idx;
}

static inline size_t findNextSplit(std::string &str,
                                   const char separator,
                                   const char quote)
{
    size_t pos = 0;
    size_t idx1 = 0;
    while (true)
    {
        // search for next separator
        idx1 = str.find(separator, pos);
        // search for next open quote, skipping escaped quotes
        size_t idx2 = findNextQuote(str, quote, pos);
        if (idx2 == std::string::npos)  // not quotes, return next separator
            return idx1;
        else if (idx1 == std::string::npos)  // also no separators, return npos
            return std::string::npos;

        if (idx2 < idx1)
        {   // first is quote and after separator: for example "test line", ...
            idx2 = findNextQuote(str, quote, idx2 + 1);
            if (idx2 == std::string::npos)
                return std::string::npos;  // no close quote, here error
            // searching next 
            pos = idx2 + 1;
        }
        else
        {
            return idx1;
        }
    }
    return idx1;
}

bool splitParameters(StringVect &tokens,
                     std::string text,
                     const char separator,
                     const char quote)
{
    size_t idx = findNextSplit(text, separator, quote);
    std::string item;

    while (idx != std::string::npos)
    {
        std::string item = text.substr(0, idx);
        addToken(tokens, item);
        text = text.substr(idx + 1);
        idx = findNextSplit(text, separator, quote);
    }

    addToken(tokens, text);
    return true;
}
