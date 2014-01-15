/*
 *  Emote database
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "resources/db/emotedb.h"

#include "animatedsprite.h"
#include "client.h"
#include "logger.h"

#include "configuration.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    EmoteInfos mEmoteInfos;
    EmoteToEmote mEmotesAlt;
    EmoteInfo mUnknown;
    bool mLoaded = false;
    int mLastEmote = 0;
}

void EmoteDB::load()
{
    if (mLoaded)
        unload();

    EmoteSprite *const unknownSprite = new EmoteSprite;
    unknownSprite->sprite = AnimatedSprite::load(
        paths.getStringValue("spriteErrorFile"));
    unknownSprite->name = "unknown";
    mUnknown.sprites.push_back(unknownSprite);

    logger->log1("Initializing emote database...");

    mLastEmote = 0;
    loadXmlFile(paths.getStringValue("emotesFile"));
    loadXmlFile(paths.getStringValue("emotesPatchFile"));
    loadXmlDir("emotesPatchDir", loadXmlFile);
    loadSpecialXmlFile("graphics/sprites/manaplus_emotes.xml");

    mLoaded = true;
}

void EmoteDB::loadXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName);
    XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "emotes"))
    {
        logger->log("Emote Database: Error while loading %s!",
            fileName.c_str());
        return;
    }

    // iterate <emote>s
    for_each_xml_child_node(emoteNode, rootNode)
    {
        if (xmlNameEqual(emoteNode, "include"))
        {
            const std::string name = XML::getProperty(emoteNode, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
        else if (!xmlNameEqual(emoteNode, "emote"))
        {
            continue;
        }

        const int id = XML::getProperty(emoteNode, "id", -1);
        // skip hight images
        if (id > 19 || (client->isTmw() && id > 13))
            continue;

        if (id == -1)
        {
            logger->log("Emote Database: Emote with missing ID in %s!",
                paths.getStringValue("emotesFile").c_str());
            continue;
        }
        EmoteInfo *currentInfo = nullptr;
        if (mEmoteInfos.find(id) != mEmoteInfos.end())
            currentInfo = mEmoteInfos[id];
        else
            currentInfo = new EmoteInfo;
        currentInfo->time = XML::getProperty(emoteNode, "time", 500);

        for_each_xml_child_node(spriteNode, emoteNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                EmoteSprite *const currentSprite = new EmoteSprite;
                currentSprite->sprite = AnimatedSprite::load(
                    paths.getStringValue("sprites").append(std::string(
                    reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content))),
                    XML::getProperty(spriteNode, "variant", 0));
                currentSprite->name = XML::langProperty(
                    spriteNode, "name", "");
                currentInfo->sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                currentInfo->particles.push_back(reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content));
            }
        }
        mEmoteInfos[id] = currentInfo;
        if (id > mLastEmote)
            mLastEmote = id;
    }
}

void EmoteDB::loadSpecialXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName);
    XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "emotes"))
    {
        logger->log1("Emote Database: Error while loading"
                     " manaplus_emotes.xml!");
        return;
    }

    // iterate <emote>s
    for_each_xml_child_node(emoteNode, rootNode)
    {
        if (xmlNameEqual(emoteNode, "include"))
        {
            const std::string name = XML::getProperty(emoteNode, "name", "");
            if (!name.empty())
                loadSpecialXmlFile(name);
            continue;
        }
        else if (!xmlNameEqual(emoteNode, "emote"))
        {
            continue;
        }

        const int id = XML::getProperty(emoteNode, "id", -1);
        if (id == -1)
        {
            logger->log1("Emote Database: Emote with missing ID in "
                         "manaplus_emotes.xml!");
            continue;
        }
        const int altId = XML::getProperty(emoteNode, "altid", -1);

        EmoteInfo *currentInfo = nullptr;
        if (mEmoteInfos.find(id) != mEmoteInfos.end())
            currentInfo = mEmoteInfos[id];
        if (!currentInfo)
            currentInfo = new EmoteInfo;
        currentInfo->time = XML::getProperty(emoteNode, "time", 500);

        for_each_xml_child_node(spriteNode, emoteNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                EmoteSprite *const currentSprite = new EmoteSprite;
                currentSprite->sprite = AnimatedSprite::load(
                    paths.getStringValue("sprites").append(std::string(
                    reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content))),
                    XML::getProperty(spriteNode, "variant", 0));
                currentSprite->name = XML::langProperty(
                    spriteNode, "name", "");
                currentInfo->sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                currentInfo->particles.push_back(reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content));
            }
        }
        mEmoteInfos[id] = currentInfo;
        if (altId != -1)
            mEmotesAlt[altId] = id;

        if (id > mLastEmote)
            mLastEmote = id;
    }
}

void EmoteDB::unload()
{
    FOR_EACH (EmoteInfos::const_iterator, i, mEmoteInfos)
    {
        if (i->second)
        {
            std::list<EmoteSprite*> &sprites = i->second->sprites;
            while (!sprites.empty())
            {
                delete sprites.front()->sprite;
                delete sprites.front();
                sprites.pop_front();
            }
            delete i->second;
        }
    }

    mEmoteInfos.clear();

    std::list<EmoteSprite*> &sprites = mUnknown.sprites;
    while (!sprites.empty())
    {
        delete sprites.front()->sprite;
        delete sprites.front();
        sprites.pop_front();
    }

    mLoaded = false;
}

const EmoteInfo *EmoteDB::get(const int id, const bool allowNull)
{
    const EmoteInfos::const_iterator i = mEmoteInfos.find(id);

    if (i == mEmoteInfos.end())
    {
        if (allowNull)
            return nullptr;
        logger->log("EmoteDB: Warning, unknown emote ID %d requested", id);
        return &mUnknown;
    }
    else
    {
        return i->second;
    }
}

const EmoteInfo *EmoteDB::get2(int id, const bool allowNull)
{
    const EmoteToEmote::const_iterator it = mEmotesAlt.find(id);
    if (it != mEmotesAlt.end())
        id = (*it).second;

    const EmoteInfos::const_iterator i = mEmoteInfos.find(id);

    if (i == mEmoteInfos.end())
    {
        if (allowNull)
            return nullptr;
        logger->log("EmoteDB: Warning, unknown emote ID %d requested", id);
        return &mUnknown;
    }
    else
    {
        return i->second;
    }
}

const EmoteSprite *EmoteDB::getSprite(const int id, const bool allowNull)
{
    const EmoteInfo *const info = get(id, allowNull);
    if (!info)
        return nullptr;

    return info->sprites.front();
}

const int &EmoteDB::getLast()
{
    return mLastEmote;
}
