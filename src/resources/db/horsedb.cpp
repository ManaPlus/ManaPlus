/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "resources/db/horsedb.h"

#include "animatedsprite.h"
#include "configuration.h"
#include "logger.h"

#include "resources/beingcommon.h"
#include "resources/horseinfo.h"

#include "debug.h"

namespace
{
    HorseInfos mHorseInfos;
    HorseInfo mUnknown;
    bool mLoaded = false;
}

void HorseDB::load()
{
    if (mLoaded)
        unload();

    mUnknown.sprite = AnimatedSprite::load(
        paths.getStringValue("spriteErrorFile"));
    mUnknown.offsetX = 0;
    mUnknown.offsetY = 0;

    logger->log1("Initializing horse database...");

    loadXmlFile(paths.getStringValue("horsesFile"));
    loadXmlFile(paths.getStringValue("horsesPatchFile"));
    loadXmlDir("horsesPatchDir", loadXmlFile);

    mLoaded = true;
}

void HorseDB::loadXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName, UseResman_true, SkipError_false);
    XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "horses"))
    {
        logger->log("Horse Database: Error while loading %s!",
            fileName.c_str());
        return;
    }

    // iterate <emote>s
    for_each_xml_child_node(horseNode, rootNode)
    {
        if (xmlNameEqual(horseNode, "include"))
        {
            const std::string name = XML::getProperty(horseNode, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
        else if (!xmlNameEqual(horseNode, "horse"))
        {
            continue;
        }

        const int id = XML::getProperty(horseNode, "id", -1);

        if (id == -1)
        {
            logger->log("Horse Database: Horse with missing ID in %s!",
                paths.getStringValue("horsesFile").c_str());
            continue;
        }
        HorseInfo *currentInfo = nullptr;
        if (mHorseInfos.find(id) != mHorseInfos.end())
            currentInfo = mHorseInfos[id];
        else
            currentInfo = new HorseInfo;

        currentInfo->offsetX = XML::getProperty(horseNode, "offsetX", 0);
        currentInfo->offsetY = XML::getProperty(horseNode, "offsetY", 0);

        for_each_xml_child_node(spriteNode, horseNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                currentInfo->sprite = AnimatedSprite::load(
                    paths.getStringValue("sprites").append(std::string(
                    reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content))),
                    XML::getProperty(spriteNode, "variant", 0));
                break;
            }
        }
        mHorseInfos[id] = currentInfo;
    }
}

void HorseDB::unload()
{
    FOR_EACH (HorseInfos::const_iterator, i, mHorseInfos)
    {
        delete i->second->sprite;
        delete i->second;
    }
    mHorseInfos.clear();

    delete mUnknown.sprite;

    mLoaded = false;
}

HorseInfo *HorseDB::get(const int id, const bool allowNull)
{
    const HorseInfos::const_iterator i = mHorseInfos.find(id);

    if (i == mHorseInfos.end())
    {
        if (allowNull)
            return nullptr;
        logger->log("HorseDB: Warning, unknown emote ID %d requested", id);
        return &mUnknown;
    }
    else
    {
        return i->second;
    }
}

int HorseDB::size()
{
    return static_cast<signed int>(mHorseInfos.size());
}
