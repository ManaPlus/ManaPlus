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

#include "utils/translation/translationmanager.h"

#include "utils/stringutils.h"

#include "utils/translation/podict.h"
#include "utils/translation/poparser.h"

#include "resources/resourcemanager.h"

#include "logger.h"

#include "debug.h"

void TranslationManager::init()
{
    delete translator;
    translator = PoParser::getEmptyDict();
}

void TranslationManager::loadCurrentLang()
{
    delete translator;
    translator = loadLang(getLang(), "");
    translator = loadLang(getLang(), "help/", translator);
}

void TranslationManager::close()
{
    delete translator;
    translator = nullptr;
}

PoDict *TranslationManager::loadLang(const LangVect &lang,
                                     const std::string &subName,
                                     PoDict *const dict)
{
    std::string name;
    PoParser parser;

    FOR_EACH (LangIter, it, lang)
    {
        if (*it == "C")
            continue;

//        logger->log("check file: " + subName + *it);
        if (parser.checkLang(subName + *it))
        {
            name = *it;
            break;
        }
    }
    if (!name.empty())
        return parser.load(name, subName + name, dict);
    logger->log("can't find client data translation");
    if (dict)
        return dict;
    return PoParser::getEmptyDict();
}

bool TranslationManager::translateFile(const std::string &fileName,
                                       PoDict *const dict,
                                       StringVect &lines)
{
    if (!dict || fileName.empty())
        return false;

    int contentsLength;
    const ResourceManager *const resman = ResourceManager::getInstance();
    char *fileContents = static_cast<char*>(
        resman->loadFile(fileName, contentsLength));

    if (!fileContents)
    {
        logger->log("Couldn't load file: %s", fileName.c_str());
        return false;
    }
    std::string str = std::string(fileContents, contentsLength);

    size_t oldPos1 = std::string::npos;
    size_t pos1;

    while ((pos1 = str.find("<<")) != std::string::npos)
    {
        if (pos1 == oldPos1)
            break;  // detected infinite loop
        const size_t pos2 = str.find(">>", pos1 + 2);
        if (pos2 == std::string::npos)
            break;
        const std::string key(str.substr(pos1 + 2, pos2 - pos1 - 2));
        const std::string key2("<<" + str.substr(
            pos1 + 2, pos2 - pos1 - 2) + ">>");
        const std::string val(dict->getStr(key));
        replaceAll(str, key2, val);
        oldPos1 = pos1;
    }

    std::istringstream iss(str);
    std::string line;

    while (getline(iss, line))
        lines.push_back(line);

    free(fileContents);
    return true;
}
