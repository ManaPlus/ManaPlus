/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "resources/itemdb.h"

#include "client.h"
#include "log.h"

#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"
#include "configuration.h"

#include <libxml/tree.h>

#include <cassert>

namespace
{
    ItemDB::ItemInfos mItemInfos;
    ItemDB::NamedItemInfos mNamedItemInfos;
    ItemInfo *mUnknown;
    bool mLoaded = false;
    std::vector<std::string> mTagNames;
    std::map<std::string, int> mTags;
}

// Forward declarations
static void loadSpriteRef(ItemInfo *itemInfo, xmlNodePtr node);
static void loadSoundRef(ItemInfo *itemInfo, xmlNodePtr node);
static void loadFloorSprite(SpriteDisplay *display, xmlNodePtr node);
static void loadReplaceSprite(ItemInfo *itemInfo, xmlNodePtr replaceNode);
static int parseSpriteName(std::string &name);

static char const *const fields[][2] =
{
    { "attack",    N_("Attack %+d")    },
    { "defense",   N_("Defense %+d")   },
    { "hp",        N_("HP %+d")        },
    { "mp",        N_("MP %+d")        }
};

static std::list<ItemDB::Stat> extraStats;

void ItemDB::setStatsList(const std::list<ItemDB::Stat> &stats)
{
    extraStats = stats;
}

static ItemType itemTypeFromString(const std::string &name)
{
    if (name == "generic" || name == "other")
    {
        return ITEM_UNUSABLE;
    }
    else if (name == "usable")
    {
        return ITEM_USABLE;
    }
    else if (name == "equip-1hand")
    {
        return ITEM_EQUIPMENT_ONE_HAND_WEAPON;
    }
    else if (name == "equip-2hand")
    {
        return ITEM_EQUIPMENT_TWO_HANDS_WEAPON;
    }
    else if (name == "equip-torso")
    {
        return ITEM_EQUIPMENT_TORSO;
    }
    else if (name == "equip-arms")
    {
        return ITEM_EQUIPMENT_ARMS;
    }
    else if (name == "equip-head")
    {
        return ITEM_EQUIPMENT_HEAD;
    }
    else if (name == "equip-legs")
    {
        return ITEM_EQUIPMENT_LEGS;
    }
    else if (name == "equip-shield")
    {
        return ITEM_EQUIPMENT_SHIELD;
    }
    else if (name == "equip-ring")
    {
        return ITEM_EQUIPMENT_RING;
    }
    else if (name == "equip-charm")
    {
        return ITEM_EQUIPMENT_CHARM;
    }
    else if (name == "equip-necklace" || name == "equip-neck")
    {
        return ITEM_EQUIPMENT_NECKLACE;
    }
    else if (name == "equip-feet")
    {
        return ITEM_EQUIPMENT_FEET;
    }
    else if (name == "equip-ammo")
    {
        return ITEM_EQUIPMENT_AMMO;
    }
    else if (name == "racesprite")
    {
        return ITEM_SPRITE_RACE;
    }
    else if (name == "hairsprite")
    {
        return ITEM_SPRITE_HAIR;
    }
    else
    {
        logger->log("Unknown item type: " + name);
        return ITEM_UNUSABLE;
    }
}

void ItemDB::load()
{
    if (mLoaded)
        unload();

    int tagNum = 0;
    logger->log1("Initializing item database...");

    mUnknown = new ItemInfo;
    mUnknown->setName(_("Unknown item"));
    mUnknown->setDisplay(SpriteDisplay());
    std::string errFile = paths.getStringValue("spriteErrorFile");
    mUnknown->setSprite(errFile, GENDER_MALE);
    mUnknown->setSprite(errFile, GENDER_FEMALE);

    XML::Document doc("items.xml");
    xmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "items"))
    {
        logger->log("ItemDB: Error while loading items.xml!");
        mLoaded = true;
        return;
    }

    mTags.clear();
    mTagNames.clear();
    mTagNames.push_back("All");
    mTagNames.push_back("Usable");
    mTagNames.push_back("Unusable");
    mTagNames.push_back("Equipment");
    mTags["All"] = tagNum ++;
    mTags["Usable"] = tagNum ++;
    mTags["Unusable"] = tagNum ++;
    mTags["Equipment"] = tagNum ++;

    for_each_xml_child_node(node, rootNode)
    {
        if (!xmlStrEqual(node->name, BAD_CAST "item"))
            continue;

        int id = XML::getProperty(node, "id", 0);

        if (id == 0)
        {
            logger->log1("ItemDB: Invalid or missing item ID in items.xml!");
            continue;
        }
        else if (mItemInfos.find(id) != mItemInfos.end())
        {
            logger->log("ItemDB: Redefinition of item ID %d", id);
        }

        std::string typeStr = XML::getProperty(node, "type", "other");
        int weight = XML::getProperty(node, "weight", 0);
        int view = XML::getProperty(node, "view", 0);

        std::string name = XML::getProperty(node, "name", "");
        std::string image = XML::getProperty(node, "image", "");
        std::string floor = XML::getProperty(node, "floor", "");
        std::string description = XML::getProperty(node, "description", "");
        std::string attackAction = XML::getProperty(node, "attack-action", "");
        std::string drawBefore = XML::getProperty(node, "drawBefore", "");
        std::string drawAfter = XML::getProperty(node, "drawAfter", "");
        std::string removeSprite = XML::getProperty(node, "removeSprite", "");
        std::string colors;
        if (serverVersion >= 1)
            colors = XML::getProperty(node, "colors", "");
        else
            colors = "";

        std::string tags[3];
        tags[0] = XML::getProperty(node, "tag",
            XML::getProperty(node, "tag1", ""));
        tags[1] = XML::getProperty(node, "tag2", "");
        tags[2] = XML::getProperty(node, "tag3", "");

        int drawPriority = XML::getProperty(node, "drawPriority", 0);

        int attackRange = XML::getProperty(node, "attack-range", 0);
        std::string missileParticle = XML::getProperty(
            node, "missile-particle", "");

        SpriteDisplay display;
        display.image = image;
        if (floor != "")
            display.floor = floor;
        else
            display.floor = image;

        ItemInfo *itemInfo = new ItemInfo;
        itemInfo->setId(id);
        itemInfo->setName(name.empty() ? _("unnamed") : name);
        itemInfo->setDescription(description);
        itemInfo->setType(itemTypeFromString(typeStr));
        itemInfo->addTag(mTags["All"]);
        switch (itemInfo->getType())
        {
            case ITEM_USABLE:
                itemInfo->addTag(mTags["Usable"]);
                break;
            case ITEM_UNUSABLE:
                itemInfo->addTag(mTags["Unusable"]);
                break;
            default:
            case ITEM_EQUIPMENT_ONE_HAND_WEAPON:
            case ITEM_EQUIPMENT_TWO_HANDS_WEAPON:
            case ITEM_EQUIPMENT_TORSO:
            case ITEM_EQUIPMENT_ARMS:
            case ITEM_EQUIPMENT_HEAD:
            case ITEM_EQUIPMENT_LEGS:
            case ITEM_EQUIPMENT_SHIELD:
            case ITEM_EQUIPMENT_RING:
            case ITEM_EQUIPMENT_NECKLACE:
            case ITEM_EQUIPMENT_FEET:
            case ITEM_EQUIPMENT_AMMO:
            case ITEM_EQUIPMENT_CHARM:
            case ITEM_SPRITE_RACE:
            case ITEM_SPRITE_HAIR:
                itemInfo->addTag(mTags["Equipment"]);
                break;
        }
        for (int f = 0; f < 3; f++)
        {
            if (tags[f] != "")
            {
                if (mTags.find(tags[f]) == mTags.end())
                {
                    mTagNames.push_back(tags[f]);
                    mTags[tags[f]] = tagNum ++;
                }
                itemInfo->addTag(mTags[tags[f]]);
            }
        }

        itemInfo->setView(view);
        itemInfo->setWeight(weight);
        itemInfo->setAttackAction(attackAction);
        itemInfo->setAttackRange(attackRange);
        itemInfo->setMissileParticle(missileParticle);
        itemInfo->setDrawBefore(parseSpriteName(drawBefore));
        itemInfo->setDrawAfter(parseSpriteName(drawAfter));
        itemInfo->setDrawPriority(drawPriority);
        itemInfo->setColorsList(colors);

        std::string effect;
        for (int i = 0; i < int(sizeof(fields) / sizeof(fields[0])); ++i)
        {
            int value = XML::getProperty(node, fields[i][0], 0);
            if (!value)
                continue;
            if (!effect.empty())
                effect += " / ";
            effect += strprintf(gettext(fields[i][1]), value);
        }
        for (std::list<Stat>::iterator it = extraStats.begin();
             it != extraStats.end(); ++it)
        {
            int value = XML::getProperty(node, it->tag.c_str(), 0);
            if (!value)
                continue;
            if (!effect.empty())
                effect += " / ";
            effect += strprintf(it->format.c_str(), value);
        }
        std::string temp = XML::getProperty(node, "effect", "");
        if (!effect.empty() && !temp.empty())
            effect += " / ";
        effect += temp;
        itemInfo->setEffect(effect);

        for_each_xml_child_node(itemChild, node)
        {
            if (xmlStrEqual(itemChild->name, BAD_CAST "sprite"))
            {
                std::string attackParticle = XML::getProperty(
                    itemChild, "particle-effect", "");
                itemInfo->setParticleEffect(attackParticle);

                loadSpriteRef(itemInfo, itemChild);
            }
            else if (xmlStrEqual(itemChild->name, BAD_CAST "sound"))
            {
                loadSoundRef(itemInfo, itemChild);
            }
            else if (xmlStrEqual(itemChild->name, BAD_CAST "floor"))
            {
                loadFloorSprite(&display, itemChild);
            }
            else if (xmlStrEqual(itemChild->name, BAD_CAST "replace"))
            {
                loadReplaceSprite(itemInfo, itemChild);
            }
        }

        itemInfo->setDisplay(display);

        mItemInfos[id] = itemInfo;
        if (!name.empty())
        {
            std::string temp = normalize(name);

            NamedItemInfos::const_iterator itr = mNamedItemInfos.find(temp);
            if (itr == mNamedItemInfos.end())
            {
                mNamedItemInfos[temp] = itemInfo;
            }
            else
            {
                logger->log("ItemDB: Duplicate name of item found item %d",
                    id);
            }
        }

        if (!attackAction.empty())
        {
            if (attackRange == 0)
            {
                logger->log("ItemDB: Missing attack range from weapon %i!",
                    id);
            }
        }

#define CHECK_PARAM(param, error_value) \
        if (param == error_value) \
            logger->log("ItemDB: Missing " #param " attribute for item %i!", \
                        id)

        if (id >= 0 && typeStr != "other")
        {
            CHECK_PARAM(name, "");
            CHECK_PARAM(description, "");
            CHECK_PARAM(image, "");
        }
        // CHECK_PARAM(effect, "");
        // CHECK_PARAM(type, 0);
        // CHECK_PARAM(weight, 0);
        // CHECK_PARAM(slot, 0);

#undef CHECK_PARAM
    }

    mLoaded = true;
}

const std::vector<std::string> &ItemDB::getTags()
{
    return mTagNames;
}

int ItemDB::getTagId(std::string tagName)
{
    return mTags[tagName];
}

void ItemDB::unload()
{
    logger->log1("Unloading item database...");

    delete mUnknown;
    mUnknown = 0;

    delete_all(mItemInfos);
    mItemInfos.clear();
    mNamedItemInfos.clear();
    mTags.clear();
    mTagNames.clear();
    mLoaded = false;
}

bool ItemDB::exists(int id)
{
    assert(mLoaded);

    ItemInfos::const_iterator i = mItemInfos.find(id);

    return i != mItemInfos.end();
}

const ItemInfo &ItemDB::get(int id)
{
    assert(mLoaded);

    ItemInfos::const_iterator i = mItemInfos.find(id);

    if (i == mItemInfos.end())
    {
        logger->log("ItemDB: Warning, unknown item ID# %d", id);
        return *mUnknown;
    }

    return *(i->second);
}

const ItemInfo &ItemDB::get(const std::string &name)
{
    assert(mLoaded);

    NamedItemInfos::const_iterator i = mNamedItemInfos.find(normalize(name));

    if (i == mNamedItemInfos.end())
    {
        if (!name.empty())
        {
            logger->log("ItemDB: Warning, unknown item name \"%s\"",
                        name.c_str());
        }
        return *mUnknown;
    }

    return *(i->second);
}

const std::map<int, ItemInfo*> &ItemDB::getItemInfos()
{
    return mItemInfos;
}

int parseSpriteName(std::string &name)
{
    int id = -1;
    if (name == "shoes" || name == "boot" || name == "boots")
    {
        id = 1;
    }
    else if (name == "bottomclothes" || name == "bottom" || name == "pants")
    {
        id = 2;
    }
    else if (name == "topclothes" || name == "top"
             || name == "torso" || name == "body")
    {
        id = 3;
    }
    else if (name == "misc1")
    {
        id = 4;
    }
    else if (name == "misc2" || name == "scarf" || name == "scarfs")
    {
        id = 5;
    }
    else if (name == "hair")
    {
        id = 6;
    }
    else if (name == "hat" || name == "hats")
    {
        id = 7;
    }
    else if (name == "wings")
    {
        id = 8;
    }
    else if (name == "glove" || name == "gloves")
    {
        id = 9;
    }
    else if (name == "weapon" || name == "weapons")
    {
        id = 10;
    }
    else if (name == "shield" || name == "shields")
    {
        id = 11;
    }
    else if (name == "amulet" || name == "amulets")
    {
        id = 12;
    }
    else if (name == "ring" || name == "rings")
    {
        id = 13;
    }

    return id;
}

void loadSpriteRef(ItemInfo *itemInfo, xmlNodePtr node)
{
    std::string gender = XML::getProperty(node, "gender", "unisex");
    std::string filename = reinterpret_cast<const char*>(
        node->xmlChildrenNode->content);

    if (gender == "male" || gender == "unisex")
        itemInfo->setSprite(filename, GENDER_MALE);
    if (gender == "female" || gender == "unisex")
        itemInfo->setSprite(filename, GENDER_FEMALE);
}

void loadSoundRef(ItemInfo *itemInfo, xmlNodePtr node)
{
    std::string event = XML::getProperty(node, "event", "");
    std::string filename = reinterpret_cast<const char*>(
        node->xmlChildrenNode->content);

    if (event == "hit")
    {
        itemInfo->addSound(EQUIP_EVENT_HIT, filename);
    }
    else if (event == "strike")
    {
        itemInfo->addSound(EQUIP_EVENT_STRIKE, filename);
    }
    else
    {
        logger->log("ItemDB: Ignoring unknown sound event '%s'",
                event.c_str());
    }
}

void loadFloorSprite(SpriteDisplay *display, xmlNodePtr floorNode)
{
    for_each_xml_child_node(spriteNode, floorNode)
    {
        if (xmlStrEqual(spriteNode->name, BAD_CAST "sprite"))
        {
            SpriteReference *currentSprite = new SpriteReference;
            currentSprite->sprite = reinterpret_cast<const char*>(
                spriteNode->xmlChildrenNode->content);
            currentSprite->variant
                = XML::getProperty(spriteNode, "variant", 0);
            display->sprites.push_back(currentSprite);
        }
        else if (xmlStrEqual(spriteNode->name, BAD_CAST "particlefx"))
        {
            std::string particlefx = reinterpret_cast<const char*>(
                spriteNode->xmlChildrenNode->content);
            display->particles.push_back(particlefx);
        }
    }
}

void loadReplaceSprite(ItemInfo *itemInfo, xmlNodePtr replaceNode)
{
    std::string removeSprite = XML::getProperty(replaceNode, "sprite", "");
    std::map<int, int> &mapList = itemInfo->addReplaceSprite(
        parseSpriteName(removeSprite));

    itemInfo->setRemoveSprites();

    for_each_xml_child_node(itemNode, replaceNode)
    {
        if (xmlStrEqual(itemNode->name, BAD_CAST "item"))
        {
            int from = XML::getProperty(itemNode, "from", 0);
            int to = XML::getProperty(itemNode, "to", 1);
            mapList[from] = to;
        }
    }
}
