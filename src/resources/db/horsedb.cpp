/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "enums/resources/spritedirection.h"

#include "resources/beingcommon.h"
#include "resources/horseinfo.h"

#include "utils/checkutils.h"
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
        currentSprite->sprite = pathJoin(paths.getStringValue("sprites"), \
            XmlChildContent(spriteNode)); \
        currentSprite->variant = XML::getProperty( \
            spriteNode, "variant", 0); \
        currentInfo->name.push_back(currentSprite); \
    }

static void loadDownSprites(XmlNodePtrConst parentNode,
                            HorseInfo *const currentInfo);

static void loadUpSprites(XmlNodePtrConst parentNode,
                          HorseInfo *const currentInfo);

void HorseDB::load()
{
    if (mLoaded)
        unload();

    SpriteReference *currentSprite = new SpriteReference;
    currentSprite->sprite = pathJoin(paths.getStringValue("sprites"),
        paths.getStringValue("spriteErrorFile"));
    currentSprite->variant = 0;
    mUnknown.downSprites.push_back(currentSprite);

    currentSprite = new SpriteReference;
    currentSprite->sprite = pathJoin(paths.getStringValue("sprites"),
        paths.getStringValue("spriteErrorFile"));
    currentSprite->variant = 0;
    mUnknown.upSprites.push_back(currentSprite);

    logger->log1("Initializing horse database...");

    loadXmlFile(paths.getStringValue("horsesFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("horsesPatchFile"), SkipError_true);
    loadXmlDir("horsesPatchDir", loadXmlFile);

    mLoaded = true;
}

static int parseDirectionName(const std::string &name)
{
    int id = -1;
    if (name == "down")
    {
        id = SpriteDirection::DOWN;
    }
    else if (name == "downleft" || name == "leftdown")
    {
        id = SpriteDirection::DOWNLEFT;
    }
    else if (name == "left")
    {
        id = SpriteDirection::LEFT;
    }
    else if (name == "upleft" || name == "leftup")
    {
        id = SpriteDirection::UPLEFT;
    }
    else if (name == "up")
    {
        id = SpriteDirection::UP;
    }
    else if (name == "upright" || name == "rightup")
    {
        id = SpriteDirection::UPRIGHT;
    }
    else if (name == "right")
    {
        id = SpriteDirection::RIGHT;
    }
    else if (name == "downright" || name == "rightdown")
    {
        id = SpriteDirection::DOWNRIGHT;
    }
    // hack for died action.
    else if (name == "died")
    {
        id = 9;
    }

    return id;
}

static void loadRiderOffset(XmlNodePtrConst node,
                            HorseInfo *const currentInfo)
{
    const std::string dirName = XML::getProperty(node,
        "direction", "");
    const int dir = parseDirectionName(dirName);
    if (dir == -1)
    {
        reportAlways("Wrong or missing horse rider direcion: %s",
            dirName.c_str());
        return;
    }
    HorseOffset &offset = currentInfo->offsets[dir];
    offset.riderOffsetX = XML::getProperty(node,
        "riderOffsetX", 0);
    offset.riderOffsetY = XML::getProperty(node,
        "riderOffsetY", 0);
}

void HorseDB::loadXmlFile(const std::string &fileName,
                          const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
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
                loadXmlFile(name, skipError);
            continue;
        }
        else if (!xmlNameEqual(horseNode, "horse"))
        {
            continue;
        }

        const int id = XML::getProperty(horseNode, "id", -1);

        if (id == -1)
        {
            reportAlways("Horse Database: Horse with missing ID in %s!",
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
        for_each_xml_child_node(spriteNode, horseNode)
        {
            if (xmlNameEqual(spriteNode, "down"))
                loadDownSprites(spriteNode, currentInfo);
            else if (xmlNameEqual(spriteNode, "up"))
                loadUpSprites(spriteNode, currentInfo);
            else if (xmlNameEqual(spriteNode, "offset"))
                loadRiderOffset(spriteNode, currentInfo);
        }
        mHorseInfos[id] = currentInfo;
    }
}

static void loadOffset(XmlNodePtrConst node,
                       HorseInfo *const currentInfo,
                       const bool isUp)
{
    const std::string dirName = XML::getProperty(node,
        "direction", "");
    const int dir = parseDirectionName(dirName);
    if (dir == -1)
    {
        reportAlways("Wrong or missing horse direcion: %s",
            dirName.c_str());
        return;
    }
    HorseOffset &offset = currentInfo->offsets[dir];
    if (isUp)
    {
        offset.upOffsetX = XML::getProperty(node,
            "horseOffsetX", 0);
        offset.upOffsetY = XML::getProperty(node,
            "horseOffsetY", 0);
    }
    else
    {
        offset.downOffsetX = XML::getProperty(node,
            "horseOffsetX", 0);
        offset.downOffsetY = XML::getProperty(node,
            "horseOffsetY", 0);
    }
}

static void loadDownSprites(XmlNodePtrConst parentNode,
                            HorseInfo *const currentInfo)
{
    for_each_xml_child_node(spriteNode, parentNode)
    {
        if (xmlNameEqual(spriteNode, "offset"))
            loadOffset(spriteNode, currentInfo, false);
        if (!XmlHaveChildContent(spriteNode))
            continue;
        if (xmlNameEqual(spriteNode, "sprite"))
            loadSprite(downSprites)
    }
}

static void loadUpSprites(XmlNodePtrConst parentNode,
                          HorseInfo *const currentInfo)
{
    for_each_xml_child_node(spriteNode, parentNode)
    {
        if (xmlNameEqual(spriteNode, "offset"))
            loadOffset(spriteNode, currentInfo, true);
        if (!XmlHaveChildContent(spriteNode))
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
        reportAlways("HorseDB: Warning, unknown horse ID %d requested",
            id);
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
