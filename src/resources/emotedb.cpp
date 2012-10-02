/*
 *  Emote database
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "resources/emotedb.h"

#include "animatedsprite.h"
#include "client.h"
#include "logger.h"

#include "configuration.h"

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

    mLastEmote = 0;

    EmoteSprite *const unknownSprite = new EmoteSprite;
    unknownSprite->sprite = AnimatedSprite::load(
        paths.getStringValue("spriteErrorFile"));
    unknownSprite->name = "unknown";
    mUnknown.sprites.push_back(unknownSprite);

    logger->log1("Initializing emote database...");

    XML::Document doc("emotes.xml");
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "emotes"))
    {
        logger->log1("Emote Database: Error while loading emotes.xml!");
        return;
    }

    //iterate <emote>s
    for_each_xml_child_node(emoteNode, rootNode)
    {
        if (!xmlNameEqual(emoteNode, "emote"))
            continue;

        const int id = XML::getProperty(emoteNode, "id", -1);
        // skip hight images
        if (id > 19 || (Client::isTmw() && id > 13))
            continue;

        if (id == -1)
        {
            logger->log1("Emote Database: Emote with missing ID in "
                         "emotes.xml!");
            continue;
        }

        EmoteInfo *const currentInfo = new EmoteInfo;

        for_each_xml_child_node(spriteNode, emoteNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                EmoteSprite *const currentSprite = new EmoteSprite;
                std::string file = paths.getStringValue("sprites")
                    + std::string(reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content));
                currentSprite->sprite = AnimatedSprite::load(file,
                    XML::getProperty(spriteNode, "variant", 0));
                currentSprite->name = XML::langProperty(
                    spriteNode, "name", "");
                currentInfo->sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                std::string particlefx = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);
                currentInfo->particles.push_back(particlefx);
            }
        }
        mEmoteInfos[id] = currentInfo;
        if (id > mLastEmote)
            mLastEmote = id;
    }

    XML::Document doc2("graphics/sprites/manaplus_emotes.xml");
    rootNode = doc2.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "emotes"))
    {
        logger->log1("Emote Database: Error while loading"
                     " manaplus_emotes.xml!");
        return;
    }

    //iterate <emote>s
    for_each_xml_child_node(emoteNode, rootNode)
    {
        if (!xmlNameEqual(emoteNode, "emote"))
            continue;

        const int id = XML::getProperty(emoteNode, "id", -1);
        if (id == -1)
        {
            logger->log1("Emote Database: Emote with missing ID in "
                         "manaplus_emotes.xml!");
            continue;
        }
        const int altId = XML::getProperty(emoteNode, "altid", -1);

        EmoteInfo *const currentInfo = new EmoteInfo;

        for_each_xml_child_node(spriteNode, emoteNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                EmoteSprite *const currentSprite = new EmoteSprite;
                std::string file = paths.getStringValue("sprites")
                    + std::string(reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content));
                currentSprite->sprite = AnimatedSprite::load(file,
                                XML::getProperty(spriteNode, "variant", 0));
                currentSprite->name = XML::langProperty(
                    spriteNode, "name", "");
                currentInfo->sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                std::string particlefx = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);
                currentInfo->particles.push_back(particlefx);
            }
        }
        mEmoteInfos[id] = currentInfo;
        if (altId != -1)
            mEmotesAlt[altId] = id;

        if (id > mLastEmote)
            mLastEmote = id;
    }

    mLoaded = true;
}

void EmoteDB::unload()
{
    for (EmoteInfos::const_iterator i = mEmoteInfos.begin(),
         i_end = mEmoteInfos.end(); i != i_end; ++ i)
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

const AnimatedSprite *EmoteDB::getAnimation(const int id, const bool allowNull)
{
    const EmoteInfo *const info = get(id, allowNull);
    if (!info)
        return nullptr;

    return info->sprites.front()->sprite;
}

const AnimatedSprite *EmoteDB::getAnimation2(int id, const bool allowNull)
{
    const EmoteToEmote::const_iterator it = mEmotesAlt.find(id);
    if (it != mEmotesAlt.end())
        id = (*it).second;
    return getAnimation(id, allowNull);
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
