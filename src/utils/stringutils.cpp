/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "utils/stringutils.h"

#include <string>
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <list>
#include <ctime>

#include "debug.h"

static int UTF8_MAX_SIZE = 10;

std::string &trim(std::string &str)
{
    size_t pos = str.find_last_not_of(' ');
    if (pos != std::string::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');

        if (pos != std::string::npos)
            str.erase(0, pos);
    }
    else
    {
        // There is nothing else but whitespace in the string
        str.clear();
    }
    return str;
}

std::string &toLower(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), &tolower);
    return str;
}

std::string &toUpper(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), &toupper);
    return str;
}

unsigned int atox(const std::string &str)
{
    unsigned int value;
    sscanf(str.c_str(), "0x%06x", &value);

    return value;
}

const char *ipToString(const uint32_t address)
{
    static char asciiIP[18];

    snprintf(asciiIP, sizeof(asciiIP), "%i.%i.%i.%i",
            static_cast<unsigned char>(address),
            static_cast<unsigned char>(address >> 8),
            static_cast<unsigned char>(address >> 16),
            static_cast<unsigned char>(address >> 24));
    asciiIP[17] = 0;

    return asciiIP;
}

std::string strprintf(const char *const format, ...)
{
    char buf[257];
    va_list(args);
    va_start(args, format);
    int nb = vsnprintf(buf, 256, format, args);
    buf[256] = 0;
    va_end(args);
    if (nb < 256)
        return buf;

    // The static size was not big enough, try again with a dynamic allocation.
    ++nb;
    char *buf2 = new char[nb];
    va_start(args, format);
    vsnprintf(buf2, nb, format, args);
    va_end(args);
    std::string res(buf2);
    delete [] buf2;
    return res;
}

std::string removeColors(std::string msg)
{
    for (unsigned int f = 0; f < msg.length() - 2 && msg.length() > 2; f++)
    {
        while (msg.length() > f + 2 && msg.at(f) == '#'
               && msg.at(f + 1) == '#')
        {
            msg = msg.erase(f, 3);
        }
    }
    return msg;
}

int compareStrI(const std::string &a, const std::string &b)
{
    std::string::const_iterator itA = a.begin();
    const std::string::const_iterator endA = a.end();
    std::string::const_iterator itB = b.begin();
    const std::string::const_iterator endB = b.end();

    for (; itA < endA && itB < endB; ++itA, ++itB)
    {
        const int comp = tolower(*itA) - tolower(*itB);
        if (comp)
            return comp;
    }

    // Check string lengths
    if (itA == endA && itB == endB)
        return 0;
    else if (itA == endA)
        return -1;
    else
        return 1;
}


bool isWordSeparator(const signed char chr)
{
    return (chr == ' ' || chr == ',' || chr == '.' || chr == '"');
}

const std::string findSameSubstring(const std::string &restrict str1,
                                    const std::string &restrict str2)
{
    const int minLength = str1.length() > str2.length()
        ? static_cast<int>(str2.length()) : static_cast<int>(str1.length());
    for (int f = 0; f < minLength; f ++)
    {
        if (str1.at(f) != str2.at(f))
            return str1.substr(0, f);
    }
    return str1.substr(0, minLength);
}

const std::string findSameSubstringI(const std::string &restrict s1,
                                     const std::string &restrict s2)
{
    std::string str1 = s1;
    std::string str2 = s2;
    toLower(str1);
    toLower(str2);

    const size_t minLength = str1.length() > str2.length()
        ? str2.length() : str1.length();
    for (size_t f = 0; f < minLength; f ++)
    {
        if (str1.at(f) != str2.at(f))
            return s1.substr(0, f);
    }
    return s1.substr(0, minLength);
}

size_t findI(std::string str, std::string subStr)
{
    str = toLower(str);
    subStr = toLower(subStr);
    return str.find(subStr);
}

size_t findI(std::string text, StringVect &list)
{
    toLower(text);
    FOR_EACH (StringVectCIter, i, list)
    {
        std::string subStr = *i;
        subStr = toLower(subStr);
        const size_t idx = text.find(subStr);
        if (idx != std::string::npos)
            return idx;
    }
    return std::string::npos;
}

int base = 94;
int start = 33;

const std::string encodeStr(unsigned int value, const unsigned int size)
{
    std::string buf;

    do
    {
        buf += static_cast<signed char>(value % base + start);
        value /= base;
    }
    while (value);

    while (buf.length() < size)
        buf += static_cast<signed char>(start);
    return buf;
}


unsigned int decodeStr(const std::string &str)
{
    if (str.empty())
        return 0;

    int res = str[0] - start;
    int mult = 1;
    for (unsigned int f = 1; f < str.length(); f ++)
    {
        mult *= base;
        res = res + (str[f] - start) * mult;
    }
    return res;
}

std::string extractNameFromSprite(std::string str)
{
    const size_t pos1 = str.rfind(".");
    if (pos1 != std::string::npos)
    {
        size_t pos2 = str.rfind("/");
        const size_t pos3 = str.rfind("\\");
        if (pos3 != std::string::npos)
        {
            if (pos2 == std::string::npos || pos3 > pos2)
                pos2 = pos3;
        }
        if (pos2 == std::string::npos)
            pos2 = static_cast<size_t>(-1);

        const int size = static_cast<int>(pos1) - static_cast<int>(pos2) - 1;
        if (size > 0)
            str = str.substr(pos2 + 1, size);
    }
    return str;
}

std::string removeSpriteIndex(std::string str)
{
    const size_t pos1 = str.rfind("[");

    if (pos1 != std::string::npos)
    {
        size_t pos2 = str.rfind("/");
        const size_t pos3 = str.rfind("\\");
        if (pos3 != std::string::npos)
        {
            if (pos2 == std::string::npos || pos3 > pos2)
                pos2 = pos3;
        }
        if (pos2 == std::string::npos)
            pos2 = static_cast<size_t>(-1);

        const int size = static_cast<int>(pos1) - static_cast<int>(pos2) - 1;
        if (size > 0)
            str = str.substr(pos2 + 1, size);
    }
    return str;
}

const char* getSafeUtf8String(std::string text)
{
    const size_t sz = text.size();
    const size_t size = sz + UTF8_MAX_SIZE;
    char *const buf = new char[size];
    memcpy(buf, text.c_str(), sz);
    memset(buf + sz, 0, UTF8_MAX_SIZE);
    return buf;
}

void getSafeUtf8String(std::string text, char *const buf)
{
    const size_t sz = text.size();
    const size_t size = sz + UTF8_MAX_SIZE;
    if (size > 65500)
    {
        text = text.substr(0, 65500);
        const size_t sz1 = text.size();
        memcpy(buf, text.c_str(), sz1);
        memset(buf + sz1, 0, UTF8_MAX_SIZE);
    }
    else
    {
        memcpy(buf, text.c_str(), sz);
        memset(buf + sz, 0, UTF8_MAX_SIZE);
    }
    return;
}

std::string getFileName(const std::string &path)
{
    size_t pos1 = path.rfind("/");
    const size_t pos2 = path.rfind("\\");
    if (pos1 == std::string::npos)
        pos1 = pos2;
    else if (pos2 != std::string::npos && pos2 > pos1)
        pos1 = pos2;

    if (pos1 == std::string::npos)
        return path;
    return path.substr(pos1 + 1);
}

std::string getFileDir(const std::string &path)
{
    size_t pos1 = path.rfind("/");
    const size_t pos2 = path.rfind("\\");
    if (pos1 == std::string::npos)
        pos1 = pos2;
    else if (pos2 != std::string::npos && pos2 > pos1)
        pos1 = pos2;

    if (pos1 == std::string::npos)
        return path;
    return path.substr(0, pos1);
}

std::string& replaceAll(std::string& context, const std::string &restrict from,
                        const std::string &restrict to)
{
    if (from.empty())
        return context;
    size_t lookHere = 0;
    size_t foundHere;
    const size_t fromSize = from.size();
    const size_t toSize = to.size();
    while ((foundHere = context.find(from, lookHere)) != std::string::npos)
    {
        context.replace(foundHere, fromSize, to);
        lookHere = foundHere + toSize;
    }
    return context;
}

bool getBoolFromString(const std::string &text)
{
    std::string txt = text;
    toLower(trim(txt));
    if (txt == "true" || txt == "yes" || txt == "on" || txt == "1")
        return true;
    else if (txt == "false" || txt == "no" || txt == "off" || txt == "0")
        return false;
    else
        return static_cast<bool>(atoi(txt.c_str()));
}

void replaceSpecialChars(std::string &text)
{
    size_t pos1 = text.find("&");
    while (pos1 != std::string::npos)
    {
        const size_t idx = pos1 + 1;
        const size_t sz = text.size();
        if (idx >= sz)
            break;

        size_t f;
        for (f = idx; f < sz; f ++)
        {
            if (text[f] < '0' || text[f] > '9')
                break;
        }
        if (idx + 1 < f && text[f] == ';')
        {
            std::string str(" ");
            str[0] = static_cast<signed char>(atoi(text.substr(
                idx, f - idx).c_str()));
            text = text.substr(0, pos1) + str + text.substr(f + 1);
            pos1 += 1;
        }
        else
        {
            pos1 = f + 1;
        }

        pos1 = text.find("&", pos1);
    }
}

std::string normalize(const std::string &name)
{
    std::string normalized = name;
    return toLower(trim(normalized));
}

void splitToIntSet(std::set<int> &tokens,
                   const std::string &text,
                   const char separator)
{
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
        tokens.insert(atoi(item.c_str()));
}

std::list<int> splitToIntList(const std::string &text,
                              const char separator)
{
    std::list<int> tokens;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
        tokens.push_back(atoi(item.c_str()));

    return tokens;
}

std::list<std::string> splitToStringList(const std::string &text,
                                         const char separator)
{
    std::list<std::string> tokens;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
        tokens.push_back(item);

    return tokens;
}

void splitToStringVector(StringVect &tokens, const std::string &text,
                         const char separator)
{
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
    {
        item = trim(item);
        if (!item.empty())
            tokens.push_back(item);
    }
}

void splitToStringSet(std::set<std::string> &tokens, const std::string &text,
                      const char separator)
{
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
    {
        item = trim(item);
        if (!item.empty())
            tokens.insert(item);
    }
}

void splitToIntVector(std::vector<int> &tokens, const std::string &text,
                      const char separator)
{
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
    {
        item = trim(item);
        if (!item.empty())
            tokens.push_back(atoi(item.c_str()));
    }
}

std::string combineDye(std::string file, const std::string &dye)
{
    if (dye.empty())
        return file;
    const size_t pos = file.find_last_of("|");
    if (pos != std::string::npos)
        return file.substr(0, pos).append("|").append(dye);
    return file.append("|").append(dye);
}

std::string combineDye2(std::string file, const std::string &dye)
{
    if (dye.empty())
        return file;

    const size_t pos = file.find_last_of("|");
    if (pos != std::string::npos)
    {
        const std::string dye1 = file.substr(pos + 1);
        std::string str;
        file = file.substr(0, pos);
        const std::list<std::string> list1 = splitToStringList(dye1, ';');
        const std::list<std::string> list2 = splitToStringList(dye, ';');
        for (std::list<std::string>::const_iterator it1 = list1.begin(),
             it2 = list2.begin(), it1_end = list1.end(), it2_end = list2.end();
             it1 != it1_end && it2 != it2_end; ++it1, ++it2)
        {
            str.append(*it1).append(":").append(*it2).append(";");
        }
        return file.append("|").append(str);
    }
    else
    {
        return file;
    }
}

std::string combineDye3(std::string file, const std::string &dye)
{
    if (dye.empty())
        return file;

    const size_t pos = file.find_last_of("|");
    if (pos != std::string::npos)
    {
        const std::string dye1 = file.substr(pos + 1);
        std::string str;
        file = file.substr(0, pos);
        const std::list<std::string> list1 = splitToStringList(dye1, ';');
        const std::list<std::string> list2 = splitToStringList(dye, ';');
        for (std::list<std::string>::const_iterator it1 = list1.begin(),
             it2 = list2.begin(), it1_end = list1.end(), it2_end = list2.end();
             it1 != it1_end && it2 != it2_end; ++it1, ++it2)
        {
            str.append(*it1).append(":").append(*it2).append(";");
        }
        return file.append("|").append(str);
    }
    else
    {
        if (dye.empty() || file.empty())
            return file;
        else
            return file.append("|").append(dye);
    }
}

std::string packList(const std::list<std::string> &list)
{
    std::list<std::string>::const_iterator i = list.begin();
    std::string str;
    while (i != list.end())
    {
        str.append(*i).append("|");
        ++ i;
    }
    const size_t sz = str.size();
    if (sz > 1)
        str = str.substr(0, sz - 1);
    return str;
}

std::list<std::string> unpackList(const std::string &str)
{
    return splitToStringList(str, '|');
}

std::string stringToHexPath(const std::string &str)
{
    if (str.empty())
        return "";

    std::string hex = strprintf("%%%2x/", static_cast<int>(str[0]));
    for (unsigned f = 1, sz = static_cast<unsigned>(str.size()); f < sz; f ++)
        hex.append(strprintf("%%%2x", static_cast<int>(str[f])));
    return hex;
}

void deleteCharLeft(std::string &str, unsigned *const pos)
{
    if (!pos)
        return;

    while (*pos > 0)
    {
        (*pos)--;
        const int v = str[*pos];
        str.erase(*pos, 1);
        if ((v & 192) != 128)
            break;
    }
}

bool findLast(const std::string &restrict str1,
              const std::string &restrict str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    if (str1.substr(s1 - s2) == str2)
        return true;
    return false;
}

bool findFirst(const std::string &restrict str1,
               const std::string &restrict str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    if (str1.substr(0, s2) == str2)
        return true;
    return false;
}

bool findCutLast(std::string &restrict str1, const std::string &restrict str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    if (str1.substr(s1 - s2) == str2)
    {
        str1 = str1.substr(0, s1 - s2);
        return true;
    }
    return false;
}

bool findCutFirst(std::string &restrict str1, const std::string &restrict str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    if (str1.substr(0, s2) == str2)
    {
        str1 = str1.substr(s2);
        return true;
    }
    return false;
}

std::string &removeProtocol(std::string &url)
{
    const size_t i = url.find("://");
    if (i != std::string::npos)
        url = url.substr(i + 3);
    return url;
}

bool strStartWith(const std::string &restrict str1,
                  const std::string &restrict str2)
{
    const size_t sz2 = str2.size();
    if (str1.size() < sz2)
        return false;
    return str1.substr(0, sz2) == str2;
}

std::string getDateString()
{
    char buffer[80];
    time_t rawtime;
    time(&rawtime);
    const tm *const timeinfo = localtime(&rawtime);

    strftime(buffer, 79, "%Y-%m-%d", timeinfo);
    return std::string(buffer);
}

signed char parseBoolean(const std::string &value)
{
    const std::string opt = value.substr(0, 1);

    if (opt == "1" ||
        opt == "y" || opt == "Y" ||
        opt == "t" || opt == "T")
        return 1;
    else if (opt == "0" ||
             opt == "n" || opt == "N" ||
             opt == "f" || opt == "F")
        return 0;
    else
        return -1;
}

std::string encodeLinkText(std::string data)
{
    return replaceAll(data, "|", "\342\235\230");
}

std::string decodeLinkText(std::string data)
{
    return replaceAll(data, "\342\235\230", "|");
}

std::string toStringPrint(const unsigned int val)
{
    static char str[100];
    snprintf(str, sizeof(str), "%u 0x%x", val, val);
    str[99] = 0;
    return str;
}

bool isDigit(const std::string &str)
{
    if (str.empty())
        return false;
    const size_t sz = str.size();
    for (size_t f = 0; f < sz; f ++)
    {
        const char &chr = str[f];
        if (chr < '0' || chr > '9')
            return false;
    }
    return true;
}

void secureChatCommand(std::string &str)
{
    if (str[0] == '/' || str[0] == '@' || str[0] == '#')
        str = "_" + str;
}
