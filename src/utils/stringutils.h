/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "utils/stringvector.h"

#include <sstream>
#include <list>
#include <set>

#include "localconsts.h"

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
unsigned int atox(const std::string &str) A_WARN_UNUSED;

template<typename T> std::string toString(const T &arg) A_WARN_UNUSED;

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

std::string toStringPrint(const unsigned int val);

/**
 * Converts the given IP address to a string.
 *
 * The returned string is statically allocated, and shouldn't be freed. It is
 * changed upon the next use of this method.
 *
 * @param address the address to convert to a string
 * @return the string representation of the address
 */
const char *ipToString(const int address) A_WARN_UNUSED;

/**
 * A safe version of sprintf that returns a std::string of the result.
 */
std::string strprintf(const char *const format, ...) A_WARN_UNUSED
#ifdef __GNUC__
    /* This attribute is nice: it even works through gettext invokation. For
       example, gcc will complain that strprintf(_("%s"), 42) is ill-formed. */
#ifdef __OpenBSD__
    __attribute__((__format__(printf, 1, 2)))
#else
    __attribute__((__format__(gnu_printf, 1, 2)))
#endif
#endif
;

/**
 * Removes colors from a string
 *
 * @param msg the string to remove the colors from
 * @return string without colors
 */
std::string removeColors(std::string msg) A_WARN_UNUSED;

const std::string findSameSubstring(const std::string &restrict str1,
                                    const std::string &restrict str2);

const std::string findSameSubstringI(const std::string &restrict str1,
                                     const std::string &restrict str2);

/**
 * Compares the two strings case-insensitively.
 *
 * @param a the first string in the comparison
 * @param b the second string in the comparison
 * @return 0 if the strings are equal, positive if the first is greater,
 *           negative if the second is greater
 */
int compareStrI(const std::string &a, const std::string &b) A_WARN_UNUSED;

/**
 * Tells wether the character is a word separator.
 */
bool isWordSeparator(const signed char chr) A_WARN_UNUSED;

size_t findI(std::string str, std::string subStr) A_WARN_UNUSED;

size_t findI(std::string text, StringVect &list) A_WARN_UNUSED;

const std::string encodeStr(unsigned int value,
                            const unsigned int size = 0) A_WARN_UNUSED;

unsigned int decodeStr(const std::string &str) A_WARN_UNUSED;

std::string extractNameFromSprite(std::string str) A_WARN_UNUSED;

std::string removeSpriteIndex(std::string str) A_WARN_UNUSED;

const char* getSafeUtf8String(std::string text) A_WARN_UNUSED;

void getSafeUtf8String(std::string text, char *const buf);

std::string getFileName(const std::string &path) A_WARN_UNUSED;

std::string getFileDir(const std::string &path) A_WARN_UNUSED;

std::string& replaceAll(std::string& context, const std::string &restrict from,
                        const std::string &restrict to);

/**
 * Returns a bool value depending on the given string value.
 *
 * @param text the string used to get the bool value
 * @return a boolean value..
 */
bool getBoolFromString(const std::string &text) A_WARN_UNUSED;

void replaceSpecialChars(std::string &text);

/**
 * Normalize a string, which means lowercase and trim it.
 */
std::string normalize(const std::string &name) A_WARN_UNUSED;

void splitToIntSet(std::set<int> &tokens, const std::string &text,
                   const char separator);

std::list<int> splitToIntList(const std::string &text,
                              const char separator) A_WARN_UNUSED;

std::list<std::string> splitToStringList(const std::string &text,
                                         const char separator) A_WARN_UNUSED;

void splitToStringVector(StringVect &tokens,
                         const std::string &text, const char separator);

void splitToStringSet(std::set<std::string> &tokens,
                      const std::string &text, const char separator);

void splitToIntVector(std::vector<int> &tokens,
                      const std::string &text, const char separator);

std::string combineDye(std::string file, const std::string &dye) A_WARN_UNUSED;

std::string combineDye2(std::string file,
                        const std::string &dye) A_WARN_UNUSED;

std::string combineDye3(std::string file,
                        const std::string &dye) A_WARN_UNUSED;

std::string packList(const std::list<std::string> &list) A_WARN_UNUSED;

std::list<std::string> unpackList(const std::string &str) A_WARN_UNUSED;

std::string stringToHexPath(const std::string &str) A_WARN_UNUSED;

void deleteCharLeft(std::string &str, unsigned *const pos);

bool findLast(const std::string &restrict str1,
              const std::string &restrict str2) A_WARN_UNUSED;

bool findFirst(const std::string &restrict str1,
               const std::string &restrict str2) A_WARN_UNUSED;

bool findCutLast(std::string &restrict str1,
                 const std::string &restrict str2) A_WARN_UNUSED;

bool findCutFirst(std::string &restrict str1,
                  const std::string &restrict str2) A_WARN_UNUSED;

std::string &removeProtocol(std::string &url);

bool strStartWith(const std::string &restrict str,
                  const std::string &restrict start) A_WARN_UNUSED;

std::string getDateString() A_WARN_UNUSED;

signed char parseBoolean(const std::string &value);

std::string encodeLinkText(std::string data);

std::string decodeLinkText(std::string data);

bool isDigit(const std::string &str);

#endif  // UTILS_STRINGUTILS_H
