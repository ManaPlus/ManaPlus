/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/db/itemdb.h"

#include "resources/map/mapconsts.h"

#include "configuration.h"
#include "logger.h"

#include "resources/beingcommon.h"
#include "resources/iteminfo.h"
#include "resources/itemtypemapdata.h"
#include "resources/spritedirection.h"
#include "resources/spritereference.h"

#include "resources/db/itemdbstat.h"

#include "net/serverfeatures.h"

#include "utils/delete2.h"
#include "utils/dtor.h"

#include "utils/translation/podict.h"

#include "debug.h"

namespace
{
    ItemDB::ItemInfos mItemInfos;
    ItemDB::NamedItemInfos mNamedItemInfos;
    ItemInfo *mUnknown = nullptr;
    bool mLoaded = false;
    bool mConstructed = false;
    StringVect mTagNames;
    std::map<std::string, int> mTags;
    std::map<std::string, ItemSoundEvent::Type> mSoundNames;
}  // namespace

// Forward declarations
static void loadSpriteRef(ItemInfo *const itemInfo,
                          const XmlNodePtr node);
static void loadSoundRef(ItemInfo *const itemInfo,
                         const XmlNodePtr node);
static void loadFloorSprite(SpriteDisplay *const display,
                            const XmlNodePtrConst node);
static void loadReplaceSprite(ItemInfo *const itemInfo,
                              const XmlNodePtr replaceNode);
static void loadOrderSprite(ItemInfo *const itemInfo,
                            const XmlNodePtr node,
                            const bool drawAfter);
static int parseSpriteName(const std::string &name);
static int parseDirectionName(const std::string &name);

namespace
{
    struct FieldType final
    {
#ifdef ADVGCC
        FieldType(const char *const name0,
                  const char *const description0,
                  const bool sign0) :
            name(name0),
            description(description0),
            sign(sign0)
        { }

        A_DELETE_COPY(FieldType)
#endif

        const char *name;
        const char *description;
        const bool sign;
    };

    static const FieldType fields[] =
    {
        // TRANSLATORS: item info label (attack)
        { "attack",   N_("Attack %s"),     true },
        // TRANSLATORS: item info label (attack)
        { "criticalattack", N_("Critical attack %s"), true },
        // TRANSLATORS: item info label (magic attack)
        { "mattack",  N_("M. Attack %s"),  true },
        // TRANSLATORS: item info label (defence)
        { "defense",  N_("Defense %s"),    true },
        // TRANSLATORS: item info label (defence)
        { "criticaldefense", N_("Critical defense %s"), true },
        // TRANSLATORS: item info label (magic defence)
        { "mdefense", N_("M. Defense %s"), true },
        // TRANSLATORS: item info label (health)
        { "hp",       N_("HP %s"),         true },
        // TRANSLATORS: item info label (mana)
        { "mp",       N_("MP %s"),         true },
        // TRANSLATORS: item info label (level)
        { "level",    N_("Level %s"),      false },
        // TRANSLATORS: item info label (moving speed)
        { "speed",    N_("Speed %s"),      true },
        // TRANSLATORS: item info label (health)
        { "range",    N_("Range %s"),      true },
        // TRANSLATORS: item info label (health)
        { "flee",     N_("Flee %s"),       true }
    };
}  // namespace

static std::vector<ItemDB::Stat> extraStats;

void ItemDB::setStatsList(const std::vector<ItemDB::Stat> &stats)
{
    extraStats = stats;
}

static ItemType::Type itemTypeFromString(const std::string &name)
{
    const size_t sz = sizeof(itemTypeMap) / sizeof(itemTypeMap[0]);
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemTypeMap &type = itemTypeMap[f];
        if (type.name == name)
            return type.type;
    }
    logger->log("Unknown item type: " + name);
    return ItemType::UNUSABLE;
}

static std::string useButtonFromItemType(const ItemType::Type &type)
{
    const size_t sz = sizeof(itemTypeMap) / sizeof(itemTypeMap[0]);
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemTypeMap &item = itemTypeMap[f];
        if (item.type == type)
            return gettext(item.useButton.c_str());
    }
    logger->log("Unknown item type");
    return std::string();
}

static std::string useButton2FromItemType(const ItemType::Type &type)
{
    const size_t sz = sizeof(itemTypeMap) / sizeof(itemTypeMap[0]);
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemTypeMap &item = itemTypeMap[f];
        if (item.type == type)
            return gettext(item.useButton2.c_str());
    }
    logger->log("Unknown item type");
    return std::string();
}

static void initStatic()
{
    mConstructed = true;
    mSoundNames["hit"] = ItemSoundEvent::HIT;
    mSoundNames["strike"] = ItemSoundEvent::MISS;
    mSoundNames["miss"] = ItemSoundEvent::MISS;
    mSoundNames["use"] = ItemSoundEvent::USE;
    mSoundNames["equip"] = ItemSoundEvent::EQUIP;
    mSoundNames["unequip"] = ItemSoundEvent::UNEQUIP;
    mSoundNames["drop"] = ItemSoundEvent::DROP;
    mSoundNames["pickup"] = ItemSoundEvent::PICKUP;
    mSoundNames["take"] = ItemSoundEvent::TAKE;
    mSoundNames["put"] = ItemSoundEvent::PUT;
}

void ItemDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing item database...");

    if (!mConstructed)
        initStatic();

    int tagNum = 0;

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

    mUnknown = new ItemInfo;
    // TRANSLATORS: item name
    mUnknown->setName(_("Unknown item"));
    mUnknown->setDisplay(SpriteDisplay());
    std::string errFile = paths.getStringValue("spriteErrorFile");
    mUnknown->setSprite(errFile, Gender::MALE, 0);
    mUnknown->setSprite(errFile, Gender::FEMALE, 0);
    mUnknown->setSprite(errFile, Gender::OTHER, 0);
    mUnknown->addTag(mTags["All"]);
    loadXmlFile(paths.getStringValue("itemsFile"), tagNum);
    loadXmlFile(paths.getStringValue("itemsPatchFile"), tagNum);

    StringVect list;
    BeingCommon::getIncludeFiles(paths.getStringValue("itemsPatchDir"),
        list,
        ".xml");
    FOR_EACH (StringVectCIter, it, list)
        loadXmlFile(*it, tagNum);
}

void ItemDB::loadXmlFile(const std::string &fileName, int &tagNum)
{
    XML::Document doc(fileName);
    const XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "items"))
    {
        logger->log("ItemDB: Error while loading %s!", fileName.c_str());
        mLoaded = true;
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, tagNum);
            continue;
        }
        if (!xmlNameEqual(node, "item"))
            continue;

        const int id = XML::getProperty(node, "id", 0);
        ItemInfo *itemInfo = nullptr;

        if (id == 0)
        {
            logger->log("ItemDB: Invalid or missing item ID in %s!",
                fileName.c_str());
            continue;
        }
        else if (mItemInfos.find(id) != mItemInfos.end())
        {
            logger->log("ItemDB: Redefinition of item ID %d", id);
            itemInfo = mItemInfos[id];
        }
        if (!itemInfo)
            itemInfo = new ItemInfo;

        const std::string typeStr = XML::getProperty(node, "type", "other");
        const int weight = XML::getProperty(node, "weight", 0);
        const int view = XML::getProperty(node, "view", 0);

        std::string name = XML::langProperty(node, "name", "");
        std::string image = XML::getProperty(node, "image", "");
        std::string floor = XML::getProperty(node, "floor", "");
        std::string description = XML::langProperty(node, "description", "");
        std::string attackAction = XML::getProperty(node, "attack-action", "");
        std::string skyAttackAction = XML::getProperty(
            node, "skyattack-action", "");
        std::string waterAttackAction = XML::getProperty(
            node, "waterattack-action", "");
        std::string drawBefore = XML::getProperty(node, "drawBefore", "");
        std::string drawAfter = XML::getProperty(node, "drawAfter", "");
        const int pet = XML::getProperty(node, "pet", 0);
        const int maxFloorOffset = XML::getIntProperty(
            node, "maxFloorOffset", mapTileSize, 0, mapTileSize);
        std::string useButton = XML::langProperty(node, "useButton", "");
        std::string useButton2 = XML::langProperty(node, "useButton2", "");
        std::string colors;
        colors = XML::getProperty(node, "colors", "");

        // check for empty hair palete
        if (colors.empty() && id <= -1 && id > -100)
            colors = "hair";

        std::string tags[3];
        tags[0] = XML::getProperty(node, "tag",
            XML::getProperty(node, "tag1", ""));
        tags[1] = XML::getProperty(node, "tag2", "");
        tags[2] = XML::getProperty(node, "tag3", "");

        const int drawPriority = XML::getProperty(node, "drawPriority", 0);

        const int attackRange = XML::getProperty(node, "attack-range", 0);
        std::string missileParticle = XML::getProperty(
            node, "missile-particle", "");
        const int hitEffectId = XML::getProperty(node, "hit-effect-id",
            paths.getIntValue("hitEffectId"));
        const int criticalEffectId = XML::getProperty(
            node, "critical-hit-effect-id",
            paths.getIntValue("criticalHitEffectId"));
        const int missEffectId = XML::getProperty(node, "miss-effect-id",
            paths.getIntValue("missEffectId"));

        SpriteDisplay display;
        display.image = image;
        if (floor != "")
            display.floor = floor;
        else
            display.floor = image;

        itemInfo->setId(id);
        // TRANSLATORS: item info name
        itemInfo->setName(name.empty() ? _("unnamed") : name);
        itemInfo->setDescription(description);
        itemInfo->setType(itemTypeFromString(typeStr));
        if (useButton.empty())
            useButton = useButtonFromItemType(itemInfo->getType());
        itemInfo->setUseButton(useButton);
        if (useButton2.empty())
            useButton2 = useButton2FromItemType(itemInfo->getType());
        itemInfo->setUseButton2(useButton2);
        itemInfo->addTag(mTags["All"]);
        itemInfo->setPet(pet);
        itemInfo->setProtected(XML::getBoolProperty(
            node, "sellProtected", false));

        switch (itemInfo->getType())
        {
            case ItemType::USABLE:
                itemInfo->addTag(mTags["Usable"]);
                break;
            case ItemType::UNUSABLE:
                itemInfo->addTag(mTags["Unusable"]);
                break;
            default:
            case ItemType::EQUIPMENT_ONE_HAND_WEAPON:
            case ItemType::EQUIPMENT_TWO_HANDS_WEAPON:
            case ItemType::EQUIPMENT_TORSO:
            case ItemType::EQUIPMENT_ARMS:
            case ItemType::EQUIPMENT_HEAD:
            case ItemType::EQUIPMENT_LEGS:
            case ItemType::EQUIPMENT_SHIELD:
            case ItemType::EQUIPMENT_RING:
            case ItemType::EQUIPMENT_NECKLACE:
            case ItemType::EQUIPMENT_FEET:
            case ItemType::EQUIPMENT_AMMO:
            case ItemType::EQUIPMENT_CHARM:
            case ItemType::SPRITE_RACE:
            case ItemType::SPRITE_HAIR:
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
        itemInfo->setSkyAttackAction(skyAttackAction);
        itemInfo->setWaterAttackAction(waterAttackAction);
        itemInfo->setAttackRange(attackRange);
        itemInfo->setMissileParticleFile(missileParticle);
        itemInfo->setHitEffectId(hitEffectId);
        itemInfo->setCriticalHitEffectId(criticalEffectId);
        itemInfo->setMissEffectId(missEffectId);
        itemInfo->setDrawBefore(-1, parseSpriteName(drawBefore));
        itemInfo->setDrawAfter(-1, parseSpriteName(drawAfter));
        itemInfo->setDrawPriority(-1, drawPriority);
        itemInfo->setColorsList(colors);
        itemInfo->setMaxFloorOffset(maxFloorOffset);
        itemInfo->setPickupCursor(XML::getProperty(
            node, "pickupCursor", "pickup"));

        std::string effect;
        for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++ i)
        {
            std::string value = XML::getProperty(node, fields[i].name, "");
            if (value.empty())
                continue;
            if (!effect.empty())
                effect.append(" / ");
            if (fields[i].sign && isDigit(value))
                value = "+" + value;
            effect.append(strprintf(gettext(fields[i].description),
                value.c_str()));
        }
        FOR_EACH (std::vector<Stat>::const_iterator, it, extraStats)
        {
            std::string value = XML::getProperty(
                node, it->tag.c_str(), "");
            if (value.empty())
                continue;
            if (!effect.empty())
                effect.append(" / ");
            if (isDigit(value))
                value = "+" + value;
            effect.append(strprintf(it->format.c_str(), value.c_str()));
        }
        std::string temp = XML::langProperty(node, "effect", "");
        if (!effect.empty() && !temp.empty())
            effect.append(" / ");
        effect.append(temp);
        itemInfo->setEffect(effect);

        for_each_xml_child_node(itemChild, node)
        {
            if (xmlNameEqual(itemChild, "sprite"))
            {
                loadSpriteRef(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "particlefx"))
            {
                if (itemChild->xmlChildrenNode)
                {
                    display.particles.push_back(reinterpret_cast<const char*>(
                        itemChild->xmlChildrenNode->content));
                }
            }
            else if (xmlNameEqual(itemChild, "sound"))
            {
                loadSoundRef(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "floor"))
            {
                loadFloorSprite(&display, itemChild);
            }
            else if (xmlNameEqual(itemChild, "replace"))
            {
                loadReplaceSprite(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "drawAfter"))
            {
                loadOrderSprite(itemInfo, itemChild, true);
            }
            else if (xmlNameEqual(itemChild, "drawBefore"))
            {
                loadOrderSprite(itemInfo, itemChild, false);
            }
        }

/*
        logger->log("start dump item: %d", id);
        if (itemInfo->isRemoveSprites())
        {
            for (int f = 0; f < 10; f ++)
            {
                logger->log("dir: %d", f);
                SpriteToItemMap *const spriteToItems
                    = itemInfo->getSpriteToItemReplaceMap(f);
                if (!spriteToItems)
                {
                    logger->log("null");
                    continue;
                }
                for (SpriteToItemMapCIter itr = spriteToItems->begin(),
                     itr_end = spriteToItems->end(); itr != itr_end; ++ itr)
                {
                    const int remSprite = itr->first;
                    const std::map<int, int> &itemReplacer = itr->second;
                    logger->log("sprite: %d", remSprite);

                    for (std::map<int, int>::const_iterator
                         repIt = itemReplacer.begin(),
                         repIt_end = itemReplacer.end();
                         repIt != repIt_end; ++ repIt)
                    {
                        logger->log("from %d to %d", repIt->first,
                            repIt->second);
                    }
                }
            }
        }

        logger->log("--------------------------------");
        logger->log("end dump item");
*/

        itemInfo->setDisplay(display);

        mItemInfos[id] = itemInfo;
        if (!name.empty())
        {
            temp = normalize(name);
            mNamedItemInfos[temp] = itemInfo;
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
#undef CHECK_PARAM
    }

    mLoaded = true;
}

const StringVect &ItemDB::getTags()
{
    return mTagNames;
}

int ItemDB::getTagId(const std::string &tagName)
{
    return mTags[tagName];
}

void ItemDB::unload()
{
    logger->log1("Unloading item database...");

    delete2(mUnknown);

    delete_all(mItemInfos);
    mItemInfos.clear();
    mNamedItemInfos.clear();
    mTags.clear();
    mTagNames.clear();
    mLoaded = false;
}

bool ItemDB::exists(const int id)
{
    if (!mLoaded)
        return false;

    const ItemInfos::const_iterator i = mItemInfos.find(id);
    return i != mItemInfos.end();
}

const ItemInfo &ItemDB::get(const int id)
{
    if (!mLoaded)
        load();

    const ItemInfos::const_iterator i = mItemInfos.find(id);

    if (i == mItemInfos.end())
    {
        logger->log("ItemDB: Warning, unknown item ID# %d", id);
        return *mUnknown;
    }

    return *(i->second);
}

const ItemInfo &ItemDB::get(const std::string &name)
{
    if (!mLoaded)
        load();

    const NamedItemInfos::const_iterator i = mNamedItemInfos.find(
        normalize(name));

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

const ItemDB::ItemInfos &ItemDB::getItemInfos()
{
    return mItemInfos;
}

static int parseSpriteName(const std::string &name)
{
    int id = -1;
    if (name == "race" || name == "type")
    {
        id = 0;
    }
    else if (name == "shoes" || name == "boot" || name == "boots")
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

static int parseDirectionName(const std::string &name)
{
    int id = -1;
    if (name == "down")
    {
        if (serverFeatures->haveEightDirections())
            id = SpriteDirection::DOWN;
        else
            id = -2;
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
        if (serverFeatures->haveEightDirections())
            id = SpriteDirection::UP;
        else
            id = -3;
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
    else if (name == "downall")
    {
        id = -2;
    }
    else if (name == "upall")
    {
        id = -3;
    }
    // hack for died action.
    else if (name == "died")
    {
        id = 9;
    }

    return id;
}

static void loadSpriteRef(ItemInfo *const itemInfo, const XmlNodePtr node)
{
    const std::string gender = XML::getProperty(node, "gender", "unisex");
    if (!node->xmlChildrenNode)
        return;

    const std::string filename = reinterpret_cast<const char*>(
        node->xmlChildrenNode->content);

    const int race = XML::getProperty(node, "race", 0);
    if (gender == "male" || gender == "unisex")
        itemInfo->setSprite(filename, Gender::MALE, race);
    if (gender == "female" || gender == "unisex")
        itemInfo->setSprite(filename, Gender::FEMALE, race);
    if (gender == "other" || gender == "unisex")
        itemInfo->setSprite(filename, Gender::OTHER, race);
}

static void loadSoundRef(ItemInfo *const itemInfo, const XmlNodePtr node)
{
    if (!node->xmlChildrenNode)
        return;
    const std::string event = XML::getProperty(node, "event", "");
    const std::string filename = reinterpret_cast<const char*>(
        node->xmlChildrenNode->content);
    const int delay = XML::getProperty(node, "delay", 0);

    const std::map<std::string, ItemSoundEvent::Type>::const_iterator
        it = mSoundNames.find(event);
    if (it != mSoundNames.end())
    {
        itemInfo->addSound((*it).second, filename, delay);
    }
    else
    {
        logger->log("ItemDB: Ignoring unknown sound event '%s'",
            event.c_str());
    }
}

static void loadFloorSprite(SpriteDisplay *const display,
                            const XmlNodePtrConst floorNode)
{
    for_each_xml_child_node(spriteNode, floorNode)
    {
        if (!spriteNode->xmlChildrenNode)
            continue;
        if (xmlNameEqual(spriteNode, "sprite"))
        {
            SpriteReference *const currentSprite = new SpriteReference;
            currentSprite->sprite = reinterpret_cast<const char*>(
                spriteNode->xmlChildrenNode->content);
            currentSprite->variant
                = XML::getProperty(spriteNode, "variant", 0);
            display->sprites.push_back(currentSprite);
        }
        else if (xmlNameEqual(spriteNode, "particlefx"))
        {
            display->particles.push_back(reinterpret_cast<const char*>(
                spriteNode->xmlChildrenNode->content));
        }
    }
}

static void loadReplaceSprite(ItemInfo *const itemInfo,
                              const XmlNodePtr replaceNode)
{
    const std::string removeSprite = XML::getProperty(
        replaceNode, "sprite", "");
    const int direction = parseDirectionName(XML::getProperty(
        replaceNode, "direction", "all"));

    itemInfo->setRemoveSprites();

    switch (direction)
    {
        case -1:
        {
            if (removeSprite.empty())
            {  // remove all sprites
                for (int f = 0; f < 10; f ++)
                {
                    for (int sprite = 0; sprite < 13; sprite ++)
                        itemInfo->addReplaceSprite(sprite, f);
                }
            }
            else
            {  // replace only given sprites
                for (int f = 0; f < 10; f ++)
                {
                    std::map<int, int> *const mapList
                        = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), f);
                    if (!mapList)
                        continue;
                    for_each_xml_child_node(itemNode, replaceNode)
                    {
                        if (xmlNameEqual(itemNode, "item"))
                        {
                            const int from = XML::getProperty(
                                itemNode, "from", 0);
                            const int to = XML::getProperty(
                                itemNode, "to", 1);
                            (*mapList)[from] = to;
                        }
                    }
                }
            }
            break;
        }
        case -2:
        {
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), SpriteDirection::DOWN);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), SpriteDirection::DOWNLEFT);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), SpriteDirection::DOWNRIGHT);

            for_each_xml_child_node(itemNode, replaceNode)
            {
                if (xmlNameEqual(itemNode, "item"))
                {
                    const int from = XML::getProperty(itemNode, "from", 0);
                    const int to = XML::getProperty(itemNode, "to", 1);
                    std::map<int, int> *mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), SpriteDirection::DOWN);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::DOWNLEFT);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::DOWNRIGHT);
                    if (mapList)
                        (*mapList)[from] = to;
                }
            }
            break;
        }
        case -3:
        {
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), SpriteDirection::UP);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), SpriteDirection::UPLEFT);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), SpriteDirection::UPRIGHT);

            for_each_xml_child_node(itemNode, replaceNode)
            {
                if (xmlNameEqual(itemNode, "item"))
                {
                    const int from = XML::getProperty(itemNode, "from", 0);
                    const int to = XML::getProperty(itemNode, "to", 1);
                    std::map<int, int> *mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), SpriteDirection::UP);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::UPLEFT);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::UPRIGHT);
                    if (mapList)
                        (*mapList)[from] = to;
                }
            }
            break;
        }
        default:
        {
            std::map<int, int> *const mapList = itemInfo->addReplaceSprite(
                parseSpriteName(removeSprite), direction);
            if (!mapList)
                return;
            for_each_xml_child_node(itemNode, replaceNode)
            {
                if (xmlNameEqual(itemNode, "item"))
                {
                    const int from = XML::getProperty(itemNode, "from", 0);
                    const int to = XML::getProperty(itemNode, "to", 1);
                    (*mapList)[from] = to;
                }
            }
            break;
        }
    }
}

static void loadOrderSprite(ItemInfo *const itemInfo,
                            const XmlNodePtr node,
                            const bool drawAfter)
{
    const int sprite = parseSpriteName(XML::getProperty(node, "name", ""));
    const int priority = XML::getProperty(node, "priority", 0);

    const int direction = parseDirectionName(XML::getProperty(
        node, "direction", "all"));
    if (drawAfter)
        itemInfo->setDrawAfter(direction, sprite);
    else
        itemInfo->setDrawBefore(direction, sprite);
    itemInfo->setDrawPriority(direction, priority);
}
