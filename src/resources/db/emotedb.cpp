/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "client.h"

#include "configuration.h"

#include "utils/checkutils.h"

#include "resources/beingcommon.h"
#include "resources/emoteinfo.h"
#include "resources/emotesprite.h"

#include "resources/sprite/animatedsprite.h"

#include "debug.h"

namespace
{
    EmoteInfos mEmoteInfos;
    EmoteToEmote mEmotesAlt;
    EmoteInfo mUnknown;
    bool mLoaded = false;
    int mLastEmote = 0;
}  // namespace

void EmoteDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing emote database...");

    EmoteSprite *const unknownSprite = new EmoteSprite;
    unknownSprite->sprite = AnimatedSprite::load(pathJoin(paths.getStringValue(
        "sprites"), paths.getStringValue(
        "spriteErrorFile")),
        0);
    unknownSprite->name = "unknown";
    unknownSprite->id = 0;
    mUnknown.sprites.push_back(unknownSprite);

    mLastEmote = 0;
    loadXmlFile(paths.getStringValue("emotesFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("emotesPatchFile"), SkipError_true);
    loadXmlDir("emotesPatchDir", loadXmlFile)
    loadSpecialXmlFile("graphics/sprites/manaplus_emotes.xml",
        SkipError_false);

    mLoaded = true;
}

void EmoteDB::loadXmlFile(const std::string &fileName,
                          const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodePtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "emotes"))
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
                loadXmlFile(name, skipError);
            continue;
        }
        else if (!xmlNameEqual(emoteNode, "emote"))
        {
            continue;
        }

        const int id = XML::getProperty(emoteNode, "id", -1);
        // skip hight images
        if (id > 19 || (Client::isTmw() && id > 13))
            continue;

        if (id == -1)
        {
            reportAlways("Emote Database: Emote with missing ID in %s!",
                paths.getStringValue("emotesFile").c_str())
            continue;
        }
        EmoteInfo *currentInfo = nullptr;
        if (mEmoteInfos.find(id) != mEmoteInfos.end())
            currentInfo = mEmoteInfos[id];
        else
            currentInfo = new EmoteInfo;
        if (currentInfo == nullptr)
            continue;
        currentInfo->time = XML::getProperty(emoteNode, "time", 500);
        currentInfo->effectId = XML::getProperty(emoteNode, "effect", -1);

        for_each_xml_child_node(spriteNode, emoteNode)
        {
            if (!XmlHaveChildContent(spriteNode))
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                EmoteSprite *const currentSprite = new EmoteSprite;
                currentSprite->sprite = AnimatedSprite::load(pathJoin(
                    paths.getStringValue("sprites"),
                    XmlChildContent(spriteNode)),
                    XML::getProperty(spriteNode, "variant", 0));
                currentSprite->name = XML::langProperty(
                    spriteNode, "name", "");
                currentSprite->id = id;
                currentInfo->sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                currentInfo->particles.push_back(XmlChildContent(spriteNode));
            }
        }
        mEmoteInfos[id] = currentInfo;
        if (id > mLastEmote)
            mLastEmote = id;
    }
}

void EmoteDB::loadSpecialXmlFile(const std::string &fileName,
                                 const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodePtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "emotes"))
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
                loadSpecialXmlFile(name, skipError);
            continue;
        }
        else if (!xmlNameEqual(emoteNode, "emote"))
        {
            continue;
        }

        const int id = XML::getProperty(emoteNode, "id", -1);
        if (id == -1)
        {
            reportAlways("Emote Database: Emote with missing ID in "
                "manaplus_emotes.xml!")
            continue;
        }
        const int altId = XML::getProperty(emoteNode, "altid", -1);

        EmoteInfo *currentInfo = nullptr;
        if (mEmoteInfos.find(id) != mEmoteInfos.end())
            currentInfo = mEmoteInfos[id];
        if (currentInfo == nullptr)
            currentInfo = new EmoteInfo;
        currentInfo->time = XML::getProperty(emoteNode, "time", 500);
        currentInfo->effectId = XML::getProperty(emoteNode, "effect", -1);

        for_each_xml_child_node(spriteNode, emoteNode)
        {
            if (!XmlHaveChildContent(spriteNode))
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                EmoteSprite *const currentSprite = new EmoteSprite;
                currentSprite->sprite = AnimatedSprite::load(pathJoin(
                    paths.getStringValue("sprites"),
                    XmlChildContent(spriteNode)),
                    XML::getProperty(spriteNode, "variant", 0));
                currentSprite->name = XML::langProperty(
                    spriteNode, "name", "");
                currentSprite->id = id;
                currentInfo->sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                currentInfo->particles.push_back(XmlChildContent(spriteNode));
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
    logger->log1("Unloading emote database...");
    FOR_EACH (EmoteInfos::const_iterator, i, mEmoteInfos)
    {
        if (i->second != nullptr)
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
        reportAlways("EmoteDB: Warning, unknown emote ID %d requested",
            id)
        return &mUnknown;
    }
    return i->second;
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
        reportAlways("EmoteDB: Warning, unknown emote ID %d requested",
            id)
        return &mUnknown;
    }
    return i->second;
}

const EmoteSprite *EmoteDB::getSprite(const int id, const bool allowNull)
{
    const EmoteInfo *const info = get(id, allowNull);
    if (info == nullptr)
        return nullptr;

    return info->sprites.front();
}

const int &EmoteDB::getLast()
{
    return mLastEmote;
}

int EmoteDB::size()
{
    return static_cast<signed int>(mEmoteInfos.size());
}

int EmoteDB::getIdByIndex(const int index)
{
    if (index < 0 || index >= static_cast<signed int>(mEmoteInfos.size()))
        return 0;
    EmoteInfos::const_iterator it = mEmoteInfos.begin();
    for (int f = 0; f < index; f ++)
        ++ it;
    return (*it).first;
}
