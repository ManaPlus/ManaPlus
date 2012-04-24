/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "utils/stringutils.h"

#include <string.h>
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <list>

#include <sys/time.h>

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
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}

std::string &toUpper(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), toupper);
    return str;
}

unsigned int atox(const std::string &str)
{
    unsigned int value;
    sscanf(str.c_str(), "0x%06x", &value);

    return value;
}

const char *ipToString(int address)
{
    static char asciiIP[18];

    sprintf(asciiIP, "%i.%i.%i.%i",
            static_cast<unsigned char>(address),
            static_cast<unsigned char>(address >> 8),
            static_cast<unsigned char>(address >> 16),
            static_cast<unsigned char>(address >> 24));

    return asciiIP;
}

std::string strprintf(char const *format, ...)
{
    char buf[257];
    va_list(args);
    va_start(args, format);
    int nb = vsnprintf(buf, 256, format, args);
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

/*
std::string &removeBadChars(std::string &str)
{
    size_t pos;
    do
    {
        pos = str.find_first_of("@#[]");
        if (pos != std::string::npos)
            str.erase(pos, 1);
    }
    while (pos != std::string::npos);

    return str;
}
*/

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
    std::string::const_iterator endA = a.end();
    std::string::const_iterator itB = b.begin();
    std::string::const_iterator endB = b.end();

    for (; itA < endA, itB < endB; ++itA, ++itB)
    {
        int comp = tolower(*itA) - tolower(*itB);
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


bool isWordSeparator(char chr)
{
    return (chr == ' ' || chr == ',' || chr == '.' || chr == '"');
}

const std::string findSameSubstring(const std::string &str1,
                                    const std::string &str2)
{
    int minLength = str1.length() > str2.length()
        ? static_cast<int>(str2.length()) : static_cast<int>(str1.length());
    for (int f = 0; f < minLength; f ++)
    {
        if (str1.at(f) != str2.at(f))
            return str1.substr(0, f);
    }
    return str1.substr(0, minLength);
}

const std::string findSameSubstringI(const std::string &s1,
                                     const std::string &s2)
{
    std::string str1 = s1;
    std::string str2 = s2;
    toLower(str1);
    toLower(str2);

    size_t minLength = str1.length() > str2.length()
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
    std::string str = toLower(text);
    size_t idx;
    for (StringVectCIter i = list.begin(), i_end = list.end();
         i != i_end; ++ i)
    {
        std::string subStr = *i;
        subStr = toLower(subStr);
        idx = str.find(subStr);
        if (idx != std::string::npos)
            return idx;
    }
    return std::string::npos;
}

int base = 94;
int start = 33;

const std::string encodeStr(unsigned int value, unsigned int size)
{
    std::string buf;

    do
    {
        buf += static_cast<char>(value % base + start);
        value /= base;
    }
    while (value);

    while (buf.length() < size)
        buf += static_cast<char>(start);
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
    size_t pos1 = str.rfind(".");
    if (pos1 != std::string::npos)
    {
        size_t pos2 = str.rfind("/");
        size_t pos3 = str.rfind("\\");
        if (pos3 != std::string::npos)
        {
            if (pos2 == std::string::npos || pos3 > pos2)
                pos2 = pos3;
        }
        if (pos2 == std::string::npos)
            pos2 = -1;

        int size = static_cast<int>(pos1) - static_cast<int>(pos2) - 1;
        if (size > 0)
            str = str.substr(pos2 + 1, size);
    }
    return str;
}

std::string removeSpriteIndex(std::string str)
{
    size_t pos1 = str.rfind("[");

    if (pos1 != std::string::npos)
    {
        size_t pos2 = str.rfind("/");
        size_t pos3 = str.rfind("\\");
        if (pos3 != std::string::npos)
        {
            if (pos2 == std::string::npos || pos3 > pos2)
                pos2 = pos3;
        }
        if (pos2 == std::string::npos)
            pos2 = -1;

        int size = static_cast<int>(pos1) - static_cast<int>(pos2) - 1;
        if (size > 0)
            str = str.substr(pos2 + 1, size);
    }
    return str;
}

const char* getSafeUtf8String(std::string text)
{
    int size = static_cast<int>(text.size()) + UTF8_MAX_SIZE;
    char* buf = new char[size];
    memcpy(buf, text.c_str(), text.size());
    memset(buf + text.size(), 0, UTF8_MAX_SIZE);
    return buf;
}

void getSafeUtf8String(std::string text, char *buf)
{
    int size = static_cast<int>(text.size()) + UTF8_MAX_SIZE;
    if (size > 65500)
        text = text.substr(0, 65500);
    memcpy(buf, text.c_str(), text.size());
    memset(buf + text.size(), 0, UTF8_MAX_SIZE);
    return;
}

std::string getFileName(std::string path)
{
    size_t pos1 = path.rfind("/");
    size_t pos2 = path.rfind("\\");
    if (pos1 == std::string::npos)
        pos1 = pos2;
    else if (pos2 != std::string::npos && pos2 > pos1)
        pos1 = pos2;

    if (pos1 == std::string::npos)
        return path;
    return path.substr(pos1 + 1);
}

std::string getFileDir(std::string path)
{
    size_t pos1 = path.rfind("/");
    size_t pos2 = path.rfind("\\");
    if (pos1 == std::string::npos)
        pos1 = pos2;
    else if (pos2 != std::string::npos && pos2 > pos1)
        pos1 = pos2;

    if (pos1 == std::string::npos)
        return path;
    return path.substr(0, pos1);
}

std::string& replaceAll(std::string& context, const std::string& from,
                        const std::string& to)
{
    if (from.empty())
        return context;
    size_t lookHere = 0;
    size_t foundHere;
    while ((foundHere = context.find(from, lookHere)) != std::string::npos)
    {
        context.replace(foundHere, from.size(), to);
        lookHere = foundHere + to.size();
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
    size_t idx = 0;
    size_t pos1 = text.find("&");
    while (pos1 != std::string::npos)
    {
        idx = pos1 + 1;
        if (idx >= text.size())
            break;

        size_t f;
        for (f = idx; f < text.size(); f ++)
        {
            if (text[f] < '0' || text[f] > '9')
                break;
        }
        if (idx + 1 < f && text[f] == ';')
        {
            std::string str = " ";
            str[0] = static_cast<char>(atoi(text.substr(
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

std::set<int> splitToIntSet(const std::string &text, char separator)
{
    std::set<int> tokens;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
        tokens.insert(atoi(item.c_str()));

    return tokens;
}

std::list<int> splitToIntList(const std::string &text, char separator)
{
    std::list<int> tokens;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
        tokens.push_back(atoi(item.c_str()));

    return tokens;
}


std::list<std::string> splitToStringList(const std::string &text,
                                         char separator)
{
    std::list<std::string> tokens;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, separator))
        tokens.push_back(item);

    return tokens;
}

void splitToStringVector(StringVect &tokens, const std::string &text,
                         char separator)
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

std::string combineDye(std::string file, std::string dye)
{
    if (dye.empty())
        return file;
    size_t pos = file.find_last_of("|");
    if (pos != std::string::npos)
        return file.substr(0, pos) + "|" + dye;
    return file + "|" + dye;
}

std::string combineDye2(std::string file, std::string dye)
{
    if (dye.empty())
        return file;

    size_t pos = file.find_last_of("|");
    if (pos != std::string::npos)
    {
        std::string dye1 = file.substr(pos + 1);
        std::string str("");
        file = file.substr(0, pos);
        std::list<std::string> list1 = splitToStringList(dye1, ';');
        std::list<std::string> list2 = splitToStringList(dye, ';');
        for (std::list<std::string>::const_iterator it1 = list1.begin(),
             it2 = list2.begin(), it1_end = list1.end(), it2_end = list2.end();
             it1 != it1_end && it2 != it2_end; ++it1, ++it2)
        {
            str += (*it1) + ":" + (*it2) + ";";
        }
        return file + "|" + str;
    }
    else
    {
        return file;
    }
}

std::string packList(std::list<std::string> &list)
{
    std::list<std::string>::const_iterator i = list.begin();
    std::string str("");
    while (i != list.end())
    {
        str = str + (*i) + "|";
        ++ i;
    }
    if (str.size() > 1)
        str = str.substr(0, str.size() - 1);
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
    for (unsigned f = 1; f < str.size(); f ++)
        hex += strprintf("%%%2x", static_cast<int>(str[f]));
    return hex;
}

void deleteCharLeft(std::string &str, unsigned *pos)
{
    if (!pos)
        return;

    while (*pos > 0)
    {
        (*pos)--;
        int v = str[*pos];
        str.erase(*pos, 1);
        if ((v & 192) != 128)
            break;
    }
}

bool findLast(std::string &str1, std::string str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    std::string tmp = str1.substr(s1 - s2);
    if (tmp == str2)
        return true;
    return false;
}

bool findFirst(std::string &str1, std::string str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    std::string tmp = str1.substr(0, s2);
    if (tmp == str2)
        return true;
    return false;
}

bool findCutLast(std::string &str1, std::string str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    std::string tmp = str1.substr(s1 - s2);
    if (tmp == str2)
    {
        str1 = str1.substr(0, s1 - s2);
        return true;
    }
    return false;
}

bool findCutFirst(std::string &str1, std::string str2)
{
    const size_t s1 = str1.size();
    const size_t s2 = str2.size();
    if (s1 < s2)
        return false;
    std::string tmp = str1.substr(0, s2);
    if (tmp == str2)
    {
        str1 = str1.substr(s2);
        return true;
    }
    return false;
}

std::string &removeProtocol(std::string &url)
{
    size_t i = url.find("://");
    if (i != std::string::npos)
        url = url.substr(i + 3);
    return url;
}

bool strStartWith(std::string str1, std::string str2)
{
    if (str1.size() < str2.size())
        return false;
    return str1.substr(0, str2.size()) == str2;
}

std::string getDateString()
{
    char buffer[80];

    time_t rawtime;
    struct tm *timeinfo;

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 79, "%Y-%m-%d", timeinfo);
    return std::string(buffer);
}
