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

#ifndef UTILS_TRANSLATION_MANAGER_H
#define UTILS_TRANSLATION_MANAGER_H

#include "localconsts.h"

#include <string>
#include <vector>

class PoDict;

class TranslationManager
{
    public:
        static PoDict *loadLang(std::vector<std::string> lang,
                                std::string subName,
                                PoDict *dict = nullptr);

        static void init();

        static void close();

        static void loadCurrentLang();

        static bool translateFile(const std::string &fileName,
                                  PoDict *dict,
                                  std::vector<std::string> &lines);
};

#endif // UTILS_TRANSLATION_MANAGER_H
