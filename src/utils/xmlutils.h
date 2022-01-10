/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#ifndef UTILS_XMLUTILS_H
#define UTILS_XMLUTILS_H

#include "enums/simpletypes/skiperror.h"

#include "utils/vector.h"

#include <string>
#include <map>

void readXmlIntVector(const std::string &fileName,
                      const std::string &rootName,
                      const std::string &sectionName,
                      const std::string &itemName,
                      const std::string &attributeName,
                      STD_VECTOR<int> &arr,
                      const SkipError skipError);

void readXmlStringMap(const std::string &fileName,
                      const std::string &rootName,
                      const std::string &sectionName,
                      const std::string &itemName,
                      const std::string &attributeKeyName,
                      const std::string &attributeValueName,
                      std::map<std::string, std::string> &arr,
                      const SkipError skipError);

void readXmlIntMap(const std::string &fileName,
                   const std::string &rootName,
                   const std::string &sectionName,
                   const std::string &itemName,
                   const std::string &attributeKeyName,
                   const std::string &attributeValueName,
                   std::map<int32_t, int32_t> &arr,
                   const SkipError skipError);

#endif  // UTILS_XMLUTILS_H
