/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "utils/parameters.h"

#include "utils/stringutils.h"

#include "debug.h"

static inline void addToken(StringVect &tokens,
                            std::string str) A_INLINE;
static inline void addToken(StringVect &tokens,
                            std::string str)
{
    std::string item = trim(str);
    const size_t sz = item.size();
    if (sz > 1)
    {
        if (str[0] == '\"' &&
            str[sz - 1] == '\"' &&
            str[sz - 2] != '\\')
        {
            item = item.substr(1, sz - 2);
            replaceAll(item, "\\\"", "\"");
            tokens.push_back(item);
            return;
        }
    }
    replaceAll(item, "\\\"", "\"");
    if (!item.empty())
        tokens.push_back(item);
}

static inline size_t findNextQuote(const std::string &str,
                                   const char quote,
                                   const size_t pos) A_INLINE;
static inline size_t findNextQuote(const std::string &str,
                                   const char quote,
                                   const size_t pos)
{
    size_t idx = str.find(quote, pos);
    while (idx > 0 &&
           idx != std::string::npos &&
           str[idx - 1] == '\\')
    {
        idx = str.find(quote, idx + 1);
    }
    return idx;
}

static inline size_t findNextSplit(const std::string &str,
                                   const std::string &separator,
                                   const char quote) A_INLINE;
static inline size_t findNextSplit(const std::string &str,
                                   const std::string &separator,
                                   const char quote)
{
    size_t pos = 0;
    while (true)
    {
        // search for next separator
        size_t idx1 = findAny(str, separator, pos);
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
            // set position for next separator or quote
            pos = idx2 + 1;
        }
        else
        {
            return idx1;
        }
    }
}

bool splitParameters(StringVect &tokens,
                     std::string text,
                     const std::string &separator,
                     const char quote)
{
    size_t idx = findNextSplit(text, separator, quote);

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
