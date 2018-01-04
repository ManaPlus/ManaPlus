/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#include "fs/virtfs/fs.h"

#include "utils/delete2.h"
#include "utils/stringutils.h"

#include "utils/foreach.h"

#include "utils/translation/podict.h"
#include "utils/translation/poparser.h"

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
    const StringVect lang = getLang();
    translator = loadLang(lang, "", nullptr);
    translator = loadLang(lang, "help/", translator);
}

void TranslationManager::loadDictionaryLang()
{
    delete dictionary;
    delete reverseDictionary;
    dictionary = loadLang(getServerLang(), "dict/", nullptr);
    reverseDictionary = reverseLang(dictionary);
}

#ifdef ENABLE_CUSTOMNLS
void TranslationManager::loadGettextLang()
{
    delete mainTranslator;
    mainTranslator = loadLang(getLang(), "manaplus/", nullptr);
}
#endif  // ENABLE_CUSTOMNLS

void TranslationManager::close()
{
    delete2(translator);
    delete2(dictionary);
    delete2(reverseDictionary);
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
        if (PoParser::checkLang(subName + *it))
        {
            name = *it;
            break;
        }
    }
    if (!name.empty())
        return parser.load(name, subName + name, dict);
    logger->log("can't find client data translation");
    if (dict != nullptr)
        return dict;
    return PoParser::getEmptyDict();
}

bool TranslationManager::translateFile(const std::string &fileName,
                                       PoDict *const dict,
                                       StringVect &lines)
{
    if ((dict == nullptr) || fileName.empty())
        return false;

    int contentsLength;
    const char *fileContents = VirtFs::loadFile(fileName,
        contentsLength);

    if (fileContents == nullptr)
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

    delete [] fileContents;
    return true;
}

PoDict *TranslationManager::reverseLang(const PoDict *const dict)
{
    PoDict *const revDict = new PoDict(dict->mLang);
    FOR_EACH (PoMap::const_iterator, it, dict->mPoLines)
    {
        revDict->set((*it).second, (*it).first);
    }
    return revDict;
}
