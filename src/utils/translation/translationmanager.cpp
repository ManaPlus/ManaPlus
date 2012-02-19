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

#include "utils/translation/translationmanager.h"

#include "utils/langs.h"

#include "utils/translation/podict.h"
#include "utils/translation/poparser.h"

#include <string.h>

#include "localconsts.h"
#include "logger.h"

#include "debug.h"

void TranslationManager::init()
{
    if (translator)
        delete translator;
    translator = PoParser::getEmptyDict();
}

void TranslationManager::loadCurrentLang()
{
    if (translator)
        delete translator;
    translator = loadLang(getLang());
}

void TranslationManager::close()
{
    delete translator;
    translator = nullptr;
}

PoDict *TranslationManager::loadLang(LangVect lang)
{
    std::string name = "";
    PoParser parser;

    LangIter it = lang.begin();
    LangIter it_end = lang.end();

    for (; it != it_end; ++ it)
    {
        if (*it == "C")
            continue;

        if (parser.checkLang(*it))
        {
            name = *it;
            break;
        }
    }
    if (!name.empty())
        return parser.load(name);
    logger->log("can't find client data translation");
    return PoParser::getEmptyDict();
}
