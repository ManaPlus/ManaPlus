/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef UTILS_STRINGUTILS_H
#define UTILS_STRINGUTILS_H

#include <string>
#include <sstream>
#include <list>
#include <set>
#include <vector>

/**
 * Trims spaces off the end and the beginning of the given string.
 *
 * @param str the string to trim spaces off
 * @return a reference to the trimmed string
 */
std::string &trim(std::string &str);

/**
 * Converts the given string to lower case.
 *
 * @param str the string to convert to lower case
 * @return a reference to the given string converted to lower case
 */
std::string &toLower(std::string &str);

/**
 * Converts the given string to upper case.
 *
 * @param str the string to convert to upper case
 * @return a reference to the given string converted to upper case
 */
std::string &toUpper(std::string &str);


/**
 * Converts an ascii hexidecimal string to an integer
 *
 * @param str the hex string to convert to an int
 * @return the integer representation of the hex string
 */
unsigned int atox(const std::string &str);

/**
 * Converts the given value to a string using std::stringstream.
 *
 * @param arg the value to convert to a string
 * @return the string representation of arg
 */
template<typename T> std::string toString(const T &arg)
{
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

/**
 * Converts the given IP address to a string.
 *
 * The returned string is statically allocated, and shouldn't be freed. It is
 * changed upon the next use of this method.
 *
 * @param address the address to convert to a string
 * @return the string representation of the address
 */
const char *ipToString(int address);

/**
 * A safe version of sprintf that returns a std::string of the result.
 */
std::string strprintf(char const *, ...)
#ifdef __GNUC__
    /* This attribute is nice: it even works through gettext invokation. For
       example, gcc will complain that strprintf(_("%s"), 42) is ill-formed. */
    __attribute__((__format__(__printf__, 1, 2)))
#endif
;

/**
 * Removes bad characters from a string
 *
 * @param str the string to remove the bad chars from
 * @return a reference to the string without bad chars
 */
std::string &removeBadChars(std::string &str);

/**
 * Removes colors from a string
 *
 * @param msg the string to remove the colors from
 * @return string without colors
 */
std::string removeColors(std::string msg);

const std::string findSameSubstring(const std::string &str1,
                                    const std::string &str2);

/**
 * Compares the two strings case-insensitively.
 *
 * @param a the first string in the comparison
 * @param b the second string in the comparison
 * @return 0 if the strings are equal, positive if the first is greater,
 *           negative if the second is greater
 */
int compareStrI(const std::string &a, const std::string &b);

/**
 * Tells wether the character is a word separator.
 */
bool isWordSeparator(char chr);

unsigned long findI(std::string str, std::string subStr);

unsigned long findI(std::string &text, std::vector<std::string> &list);

const std::string encodeStr(unsigned int value, unsigned int size = 0);

unsigned int decodeStr(const std::string &str);

std::string extractNameFromSprite(std::string str);

std::string removeSpriteIndex(std::string str);

const char* getSafeUtf8String(std::string text);

void getSafeUtf8String(std::string text, char *buf);

std::string getFileName(std::string path);

std::string getFileDir(std::string path);

std::string& replaceAll(std::string& context, const std::string& from,
                        const std::string& to);

/**
 * Returns a bool value depending on the given string value.
 *
 * @param text the string used to get the bool value
 * @return a boolean value..
 */
bool getBoolFromString(const std::string &text);

void replaceSpecialChars(std::string &text);

/**
 * Normalize a string, which means lowercase and trim it.
 */
std::string normalize(const std::string &name);

std::set<int> splitToIntSet(const std::string &text, char separator);

std::list<int> splitToIntList(const std::string &text, char separator);

std::list<std::string> splitToStringList(const std::string &text,
                                         char separator);

void splitToStringVector(std::vector<std::string> &tokens,
                         const std::string &text, char separator);

std::string combineDye(std::string file, std::string dye);

std::string combineDye2(std::string file, std::string dye);

std::vector<std::string> getLang();

std::string packList(std::list<std::string> &list);

std::list<std::string> unpackList(const std::string &str);

std::string stringToHexPath(const std::string &str);

void deleteCharLeft(std::string &str, unsigned *pos);

#endif // UTILS_STRINGUTILS_H
