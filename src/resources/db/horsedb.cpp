/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "configuration.h"
#include "logger.h"

#include "resources/beingcommon.h"
#include "resources/horseinfo.h"

#include "utils/dtor.h"

#include "debug.h"

namespace
{
    HorseInfos mHorseInfos;
    HorseInfo mUnknown;
    bool mLoaded = false;
}

#define loadSprite(name) \
    { \
        SpriteReference *const currentSprite = new SpriteReference; \
        currentSprite->sprite = paths.getStringValue("sprites").append( \
            std::string(reinterpret_cast<const char*>( \
            spriteNode->xmlChildrenNode->content))); \
        currentSprite->variant = XML::getProperty( \
            spriteNode, "variant", 0); \
        currentInfo->name.push_back(currentSprite); \
    }

static void loadDownSprites(XmlNodePtrConst parentNode,
                            HorseInfo *currentInfo);

static void loadUpSprites(XmlNodePtrConst parentNode,
                          HorseInfo *currentInfo);

void HorseDB::load()
{
    if (mLoaded)
        unload();

    SpriteReference *currentSprite = new SpriteReference;
    currentSprite->sprite = paths.getStringValue("sprites").append(
        paths.getStringValue("spriteErrorFile"));
    currentSprite->variant = 0;
    mUnknown.downSprites.push_back(currentSprite);

    currentSprite = new SpriteReference;
    currentSprite->sprite = paths.getStringValue("sprites").append(
        paths.getStringValue("spriteErrorFile"));
    currentSprite->variant = 0;
    mUnknown.upSprites.push_back(currentSprite);

    mUnknown.upOffsetX = 0;
    mUnknown.upOffsetY = 0;
    mUnknown.downOffsetX = 0;
    mUnknown.downOffsetY = 0;

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

        if (!currentInfo)
            continue;
        const int offsetX = XML::getProperty(horseNode, "offsetX", 0);
        const int offsetY = XML::getProperty(horseNode, "offsetY", 0);
        currentInfo->upOffsetX = XML::getProperty(horseNode,
            "upOffsetX", offsetX);
        currentInfo->upOffsetY = XML::getProperty(horseNode,
            "upOffsetY", offsetY);
        currentInfo->downOffsetX = XML::getProperty(horseNode,
            "downOffsetX", offsetX);
        currentInfo->downOffsetY = XML::getProperty(horseNode,
            "downOffsetY", offsetY);

        for_each_xml_child_node(spriteNode, horseNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
                loadSprite(downSprites)

            if (xmlNameEqual(spriteNode, "down"))
                loadDownSprites(spriteNode, currentInfo);
            else if (xmlNameEqual(spriteNode, "up"))
                loadUpSprites(spriteNode, currentInfo);
        }
        mHorseInfos[id] = currentInfo;
    }
}

static void loadDownSprites(XmlNodePtrConst parentNode,
                            HorseInfo *currentInfo)
{
    for_each_xml_child_node(spriteNode, parentNode)
    {
        if (!spriteNode->xmlChildrenNode)
            continue;

        if (xmlNameEqual(spriteNode, "sprite"))
            loadSprite(downSprites)
    }
}

static void loadUpSprites(XmlNodePtrConst parentNode,
                          HorseInfo *currentInfo)
{
    for_each_xml_child_node(spriteNode, parentNode)
    {
        if (!spriteNode->xmlChildrenNode)
            continue;

        if (xmlNameEqual(spriteNode, "sprite"))
            loadSprite(upSprites)
    }
}

void HorseDB::unload()
{
    FOR_EACH (HorseInfos::const_iterator, i, mHorseInfos)
    {
        delete_all(i->second->upSprites);
        delete_all(i->second->downSprites);
        delete i->second;
    }
    mHorseInfos.clear();

    delete_all(mUnknown.upSprites);
    delete_all(mUnknown.downSprites);
    mUnknown.upSprites.clear();
    mUnknown.downSprites.clear();

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
