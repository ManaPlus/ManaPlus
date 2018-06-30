/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "const/resources/map/map.h"

#include "configuration.h"

#include "enums/resources/spritedirection.h"

#include "fs/virtfs/tools.h"

#include "resources/iteminfo.h"
#include "resources/itemmenuitem.h"
#include "resources/itemtypemapdata.h"

#include "resources/db/itemfielddb.h"

#include "resources/sprite/spritereference.h"

#ifdef TMWA_SUPPORT
#include "net/net.h"
#endif  // TMWA_SUPPORT

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/itemxmlutils.h"
#include "utils/stdmove.h"
#include "utils/stringmap.h"

#include "debug.h"

namespace
{
    ItemDB::ItemInfos mItemInfos;
    ItemDB::NamedItemInfos mNamedItemInfos;
    ItemInfo *mUnknown = nullptr;
    bool mLoaded = false;
    bool mConstructed = false;
    StringVect mTagNames;
    StringIntMap mTags;
    std::map<std::string, ItemSoundEvent::Type> mSoundNames;
    int mNumberOfHairstyles = 1;
}  // namespace

// Forward declarations
static void loadSpriteRef(ItemInfo *const itemInfo,
                          XmlNodeConstPtr node) A_NONNULL(1);
static void loadSoundRef(ItemInfo *const itemInfo,
                         XmlNodeConstPtr node) A_NONNULL(1);
static void loadFloorSprite(SpriteDisplay &display,
                            XmlNodeConstPtrConst node);
static void loadReplaceSprite(ItemInfo *const itemInfo,
                              XmlNodeConstPtr replaceNode) A_NONNULL(1);
static void loadOrderSprite(ItemInfo *const itemInfo,
                            XmlNodeConstPtr node,
                            const bool drawAfter) A_NONNULL(1);
static int parseSpriteName(const std::string &name);
static int parseDirectionName(const std::string &name);

static ItemDbTypeT itemTypeFromString(const std::string &name)
{
    const size_t sz = sizeof(itemTypeMap) / sizeof(itemTypeMap[0]);
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemTypeMap &type = itemTypeMap[f];
        if (type.name == name)
            return type.type;
    }
    logger->log("Unknown item type: " + name);
    return ItemDbType::UNUSABLE;
}

static std::string useButtonFromItemType(const ItemDbTypeT &type)
{
    const size_t sz = sizeof(itemTypeMap) / sizeof(itemTypeMap[0]);
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemTypeMap &item = itemTypeMap[f];
        if (item.type == type)
        {
            if (item.useButton.empty())
                return std::string();
            return gettext(item.useButton.c_str());
        }
    }
    logger->log("Unknown item type");
    return std::string();
}

static std::string useButton2FromItemType(const ItemDbTypeT &type)
{
    const size_t sz = sizeof(itemTypeMap) / sizeof(itemTypeMap[0]);
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemTypeMap &item = itemTypeMap[f];
        if (item.type == type)
        {
            if (item.useButton2.empty())
                return std::string();
            return gettext(item.useButton2.c_str());
        }
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
    mSoundNames["usecard"] = ItemSoundEvent::USECARD;
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
    mUnknown->setSprite(errFile, Gender::HIDDEN, 0);
    mUnknown->addTag(mTags["All"]);
    loadXmlFile(paths.getStringValue("itemsFile"),
        tagNum,
        SkipError_false);
    loadXmlFile(paths.getStringValue("itemsPatchFile"),
        tagNum,
        SkipError_true);

    StringVect list;
    VirtFs::getFilesInDir(paths.getStringValue("itemsPatchDir"),
        list,
        ".xml");
    FOR_EACH (StringVectCIter, it, list)
        loadXmlFile(*it, tagNum, SkipError_true);

    // Hairstyles are encoded as negative numbers. Count how far negative
    // we can go.
    int hairstyles = 1;
    while (ItemDB::exists(-hairstyles) &&
           ItemDB::get(-hairstyles).getSprite(Gender::MALE,
           BeingTypeId_zero) != paths.getStringValue("spriteErrorFile"))
    {
        hairstyles ++;
    }
    mNumberOfHairstyles = hairstyles;

    int races = 100;
    while (ItemDB::exists(-races) &&
           ItemDB::get(-races).getSprite(Gender::MALE, BeingTypeId_zero) !=
           paths.getStringValue("spriteErrorFile"))
    {
        races ++;
    }
}

static void loadMenu(XmlNodePtrConst parentNode,
                     STD_VECTOR<ItemMenuItem> &menu)
{
    for_each_xml_child_node(node, parentNode)
    {
        if (xmlNameEqual(node, "menu"))
        {
            const std::string name1 = XML::langProperty(node,
                "name1", "");
            const std::string name2 = XML::langProperty(node,
                "name2", "");
            const std::string command1 = XML::getProperty(node,
                "command1", "");
            const std::string command2 = XML::getProperty(node,
                "command2", command1);
            menu.push_back(ItemMenuItem(name1,
                name2,
                command1,
                command2));
        }
    }
}

static bool getIsEquipment(const ItemDbTypeT type)
{
    switch (type)
    {
        case ItemDbType::EQUIPMENT_ONE_HAND_WEAPON:
        case ItemDbType::EQUIPMENT_TWO_HANDS_WEAPON:
        case ItemDbType::EQUIPMENT_TORSO:
        case ItemDbType::EQUIPMENT_ARMS:
        case ItemDbType::EQUIPMENT_HEAD:
        case ItemDbType::EQUIPMENT_LEGS:
        case ItemDbType::EQUIPMENT_SHIELD:
        case ItemDbType::EQUIPMENT_RING:
        case ItemDbType::EQUIPMENT_NECKLACE:
        case ItemDbType::EQUIPMENT_FEET:
        case ItemDbType::EQUIPMENT_AMMO:
        case ItemDbType::EQUIPMENT_CHARM:
            return true;
        case ItemDbType::UNUSABLE:
        case ItemDbType::USABLE:
        case ItemDbType::CARD:
        case ItemDbType::SPRITE_RACE:
        case ItemDbType::SPRITE_HAIR:
        default:
            return false;
    }
}

void ItemDB::loadXmlFile(const std::string &fileName,
                         int &tagNum,
                         const SkipError skipError)
{
    if (fileName.empty())
    {
        mLoaded = true;
        return;
    }

    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "items"))
    {
        logger->log("ItemDB: Error while loading %s!", fileName.c_str());
        mLoaded = true;
        return;
    }

    const ItemFieldInfos &requiredFields =
        ItemFieldDb::getRequiredFields();
    const ItemFieldInfos &addFields =
        ItemFieldDb::getAddFields();

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, tagNum, skipError);
            continue;
        }
        if (!xmlNameEqual(node, "item"))
            continue;

        const int id = XML::getProperty(node, "id", 0);
        ItemInfo *itemInfo = nullptr;

        if (id == 0)
        {
            reportAlways("ItemDB: Invalid or missing item ID in %s!",
                fileName.c_str());
            continue;
        }
        else if (mItemInfos.find(id) != mItemInfos.end())
        {
            logger->log("ItemDB: Redefinition of item ID %d", id);
            itemInfo = mItemInfos[id];
        }
        if (itemInfo == nullptr)
            itemInfo = new ItemInfo;

        const std::string typeStr = XML::getProperty(node, "type", "");
        int weight = XML::getProperty(node, "weight", 0);
        int view = XML::getProperty(node, "view", 0);
        const int cardColor = XML::getProperty(node, "cardColor", -1);
        const int inherit = XML::getProperty(node, "inherit", -1);

        std::string name = XML::langProperty(node, "name", "");
        std::string nameEn = XML::getProperty(node, "name", "");
        std::string image = XML::getProperty(node, "image", "");
        std::string floor = XML::getProperty(node, "floor", "");
        std::string description = XML::langProperty(node, "description", "");
        std::string attackAction = XML::getProperty(node, "attack-action", "");
        std::string skyAttackAction = XML::getProperty(
            node, "skyattack-action", "");
        std::string waterAttackAction = XML::getProperty(
            node, "waterattack-action", "");
        std::string rideAttackAction = XML::getProperty(
            node, "rideattack-action", "");
        std::string drawBefore = XML::getProperty(node, "drawBefore", "");
        std::string drawAfter = XML::getProperty(node, "drawAfter", "");
        const int maxFloorOffset = XML::getIntProperty(
            node, "maxFloorOffset", mapTileSize, 0, mapTileSize);
        const int maxFloorOffsetX = XML::getIntProperty(
            node, "maxFloorOffsetX", maxFloorOffset, 0, mapTileSize);
        const int maxFloorOffsetY = XML::getIntProperty(
            node, "maxFloorOffsetY", maxFloorOffset, 0, mapTileSize);
        std::string useButton = XML::langProperty(node, "useButton", "");
        std::string useButton2 = XML::langProperty(node, "useButton2", "");
        std::string colors = XML::getProperty(node, "colors", "");
        std::string iconColors = XML::getProperty(node, "iconColors", "");
        if (iconColors.empty())
            iconColors = colors;

        // check for empty hair palete
        if (id <= -1 && id > -100)
        {
            if (colors.empty())
                colors = "hair";
            if (iconColors.empty())
                iconColors = "hair";
        }

        std::string tags[3];
        tags[0] = XML::getProperty(node, "tag",
            XML::getProperty(node, "tag1", ""));
        tags[1] = XML::getProperty(node, "tag2", "");
        tags[2] = XML::getProperty(node, "tag3", "");

        const int drawPriority = XML::getProperty(node, "drawPriority", 0);

        int attackRange = XML::getProperty(node, "attack-range", 0);
        std::string missileParticle = XML::getProperty(
            node, "missile-particle", "");
        float missileZ = XML::getFloatProperty(
            node, "missile-z", 32.0f);
        int missileLifeTime = XML::getProperty(
            node, "missile-lifetime", 500);
        float missileSpeed = XML::getFloatProperty(
            node, "missile-speed", 7.0f);
        float missileDieDistance = XML::getFloatProperty(
            node, "missile-diedistance", 8.0f);
        int hitEffectId = XML::getProperty(node, "hit-effect-id",
            paths.getIntValue("hitEffectId"));
        int criticalEffectId = XML::getProperty(
            node, "critical-hit-effect-id",
            paths.getIntValue("criticalHitEffectId"));
        int missEffectId = XML::getProperty(node, "miss-effect-id",
            paths.getIntValue("missEffectId"));

        SpriteDisplay display;
        display.image = image;
        if (!floor.empty())
            display.floor = STD_MOVE(floor);
        else
            display.floor = image;

        const ItemInfo *inheritItemInfo = nullptr;

        if (inherit >= 0)
        {
            if (mItemInfos.find(inherit) != mItemInfos.end())
            {
                inheritItemInfo = mItemInfos[inherit];
            }
            else
            {
                reportAlways("Inherit item %d from not existing item %d",
                    id,
                    inherit);
            }
        }

        itemInfo->setId(id);
        if (name.empty() && (inheritItemInfo != nullptr))
            name = inheritItemInfo->getName();
        // TRANSLATORS: item info name
        itemInfo->setName(name.empty() ? _("unnamed") : name);
        if (nameEn.empty())
        {
            // TRANSLATORS: item info name
            itemInfo->setNameEn(name.empty() ? _("unnamed") : name);
        }
        else
        {
            itemInfo->setNameEn(nameEn);
        }

        if (description.empty() && (inheritItemInfo != nullptr))
            description = inheritItemInfo->getDescription();
        itemInfo->setDescription(description);
        if (typeStr.empty())
        {
            if (inheritItemInfo != nullptr)
                itemInfo->setType(inheritItemInfo->getType());
            else
                itemInfo->setType(itemTypeFromString("other"));
        }
        else
        {
            itemInfo->setType(itemTypeFromString(typeStr));
        }
        itemInfo->setType(itemTypeFromString(typeStr));
        if (useButton.empty() && (inheritItemInfo != nullptr))
            useButton = inheritItemInfo->getUseButton();
        if (useButton.empty())
            useButton = useButtonFromItemType(itemInfo->getType());
        itemInfo->setUseButton(useButton);
        if (useButton2.empty() && (inheritItemInfo != nullptr))
            useButton2 = inheritItemInfo->getUseButton();
        if (useButton2.empty())
            useButton2 = useButton2FromItemType(itemInfo->getType());
        itemInfo->setUseButton2(useButton2);
        itemInfo->addTag(mTags["All"]);
        itemInfo->setProtected(XML::getBoolProperty(
            node, "sellProtected", false));
        if (cardColor != -1)
            itemInfo->setCardColor(fromInt(cardColor, ItemColor));
        else if (inheritItemInfo != nullptr)
            itemInfo->setCardColor(inheritItemInfo->getCardColor());

        switch (itemInfo->getType())
        {
            case ItemDbType::USABLE:
                itemInfo->addTag(mTags["Usable"]);
                break;
            case ItemDbType::CARD:
            case ItemDbType::UNUSABLE:
                itemInfo->addTag(mTags["Unusable"]);
                break;
            default:
            case ItemDbType::EQUIPMENT_ONE_HAND_WEAPON:
            case ItemDbType::EQUIPMENT_TWO_HANDS_WEAPON:
            case ItemDbType::EQUIPMENT_TORSO:
            case ItemDbType::EQUIPMENT_ARMS:
            case ItemDbType::EQUIPMENT_HEAD:
            case ItemDbType::EQUIPMENT_LEGS:
            case ItemDbType::EQUIPMENT_SHIELD:
            case ItemDbType::EQUIPMENT_RING:
            case ItemDbType::EQUIPMENT_NECKLACE:
            case ItemDbType::EQUIPMENT_FEET:
            case ItemDbType::EQUIPMENT_AMMO:
            case ItemDbType::EQUIPMENT_CHARM:
            case ItemDbType::SPRITE_RACE:
            case ItemDbType::SPRITE_HAIR:
                itemInfo->addTag(mTags["Equipment"]);
                break;
        }
        for (int f = 0; f < 3; f++)
        {
            if (!tags[f].empty())
            {
                if (mTags.find(tags[f]) == mTags.end())
                {
                    mTagNames.push_back(tags[f]);
                    mTags[tags[f]] = tagNum ++;
                }
                itemInfo->addTag(mTags[tags[f]]);
            }
        }

        std::string effect;
        readItemStatsString(effect, node, requiredFields);
        readItemStatsString(effect, node, addFields);
        std::string temp = XML::langProperty(node, "effect", "");
        if (!effect.empty() && !temp.empty())
            effect.append(" / ");
        effect.append(temp);

        if (inheritItemInfo != nullptr)
        {
            if (view == 0)
                view = inheritItemInfo->getView();
            if (weight == 0)
                weight = inheritItemInfo->getWeight();
            if (attackAction.empty())
                attackAction = inheritItemInfo->getAttackAction();
            if (skyAttackAction.empty())
                skyAttackAction = inheritItemInfo->getSkyAttackAction();
            if (waterAttackAction.empty())
                waterAttackAction = inheritItemInfo->getWaterAttackAction();
            if (rideAttackAction.empty())
                rideAttackAction = inheritItemInfo->getRideAttackAction();
            if (attackRange == 0)
                attackRange = inheritItemInfo->getAttackRange();
            if (hitEffectId == 0)
                hitEffectId = inheritItemInfo->getHitEffectId();
            if (criticalEffectId == 0)
                criticalEffectId = inheritItemInfo->getCriticalHitEffectId();
            if (missEffectId == 0)
                missEffectId = inheritItemInfo->getMissEffectId();
            if (colors.empty())
                colors = inheritItemInfo->getColorsListName();
            if (iconColors.empty())
                iconColors = inheritItemInfo->getIconColorsListName();
            if (effect.empty())
                effect = inheritItemInfo->getEffect();

            const MissileInfo &inheritMissile =
                inheritItemInfo->getMissileConst();
            if (missileParticle.empty())
                missileParticle = inheritMissile.particle;
            if (missileZ == 32.0F)
                missileZ = inheritMissile.z;
            if (missileLifeTime == 500)
                missileLifeTime = inheritMissile.lifeTime;
            if (missileSpeed == 7.0F)
                missileSpeed = inheritMissile.speed;
            if (missileDieDistance == 8.0F)
                missileDieDistance = inheritMissile.dieDistance;
        }

        itemInfo->setView(view);
        itemInfo->setWeight(weight);
        itemInfo->setAttackAction(attackAction);
        itemInfo->setSkyAttackAction(skyAttackAction);
        itemInfo->setWaterAttackAction(waterAttackAction);
        itemInfo->setRideAttackAction(rideAttackAction);
        itemInfo->setAttackRange(attackRange);
        itemInfo->setHitEffectId(hitEffectId);
        itemInfo->setCriticalHitEffectId(criticalEffectId);
        itemInfo->setMissEffectId(missEffectId);
        itemInfo->setDrawBefore(-1, parseSpriteName(drawBefore));
        itemInfo->setDrawAfter(-1, parseSpriteName(drawAfter));
        itemInfo->setDrawPriority(-1, drawPriority);
        itemInfo->setColorsList(colors);
        itemInfo->setIconColorsList(iconColors);
        itemInfo->setMaxFloorOffsetX(maxFloorOffsetX);
        itemInfo->setMaxFloorOffsetY(maxFloorOffsetY);
        itemInfo->setPickupCursor(XML::getProperty(
            node, "pickupCursor", "pickup"));

        MissileInfo &missile = itemInfo->getMissile();
        missile.particle = STD_MOVE(missileParticle);
        missile.z = missileZ;
        missile.lifeTime = missileLifeTime;
        missile.speed = missileSpeed;
        missile.dieDistance = missileDieDistance;

        for_each_xml_child_node(itemChild, node)
        {
            if (xmlNameEqual(itemChild, "sprite"))
            {
                loadSpriteRef(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "particlefx"))
            {
                if (XmlHaveChildContent(itemChild))
                    display.particles.push_back(XmlChildContent(itemChild));
            }
            else if (xmlNameEqual(itemChild, "sound"))
            {
                loadSoundRef(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "floor"))
            {
                loadFloorSprite(display, itemChild);
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
            else if (xmlNameEqual(itemChild, "inventory"))
            {
                loadMenu(itemChild, itemInfo->getInventoryMenu());
            }
            else if (xmlNameEqual(itemChild, "storage"))
            {
                loadMenu(itemChild, itemInfo->getStorageMenu());
            }
            else if (xmlNameEqual(itemChild, "cart"))
            {
                loadMenu(itemChild, itemInfo->getCartMenu());
            }
            else if (xmlNameEqual(itemChild, "addStats"))
            {
                readItemStatsString(effect, itemChild, addFields);
            }
            else if (xmlNameEqual(itemChild, "requireStats"))
            {
                readItemStatsString(effect, itemChild, requiredFields);
            }
        }
        itemInfo->setEffect(effect);

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
                    const IntMap &itemReplacer = itr->second;
                    logger->log("sprite: %d", remSprite);

                    for (IntMapCIter repIt = itemReplacer.begin(),
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
        if (!nameEn.empty())
        {
            temp = normalize(nameEn);
            mNamedItemInfos[temp] = itemInfo;
        }

        if (!attackAction.empty())
        {
            if (attackRange == 0)
            {
                reportAlways("ItemDB: Missing attack range from weapon %i!",
                    id);
            }
        }

        STD_VECTOR<ItemMenuItem> &inventoryMenu = itemInfo->getInventoryMenu();

        if (inventoryMenu.empty())
        {
            std::string name1 = itemInfo->getUseButton();
            std::string name2 = itemInfo->getUseButton2();
            const bool isEquipment = getIsEquipment(itemInfo->getType());

            if (isEquipment)
            {
                if (name1.empty())
                {
                    // TRANSLATORS: popup menu item
                    name1 = _("Equip");
                }
                if (name2.empty())
                {
                    // TRANSLATORS: popup menu item
                    name2 = _("Unequip");
                }
            }
            else
            {
                if (name1.empty())
                {
                    // TRANSLATORS: popup menu item
                    name1 = _("Use");
                }
                if (name2.empty())
                {
                    // TRANSLATORS: popup menu item
                    name2 = _("Use");
                }
            }
            inventoryMenu.push_back(ItemMenuItem(
                name1,
                name2,
                "useinv 'INVINDEX'",
                "useinv 'INVINDEX'"));
        }

#define CHECK_PARAM(param) \
        if (param.empty()) \
        { \
            logger->log("ItemDB: Missing " #param " attribute for item %i!", \
                id); \
        }

        if (id >= 0 && typeStr != "other")
        {
            CHECK_PARAM(name)
            CHECK_PARAM(description)
            CHECK_PARAM(image)
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

bool ItemDB::exists(const std::string &name)
{
    if (!mLoaded)
        return false;

    const NamedItemInfos::const_iterator i = mNamedItemInfos.find(
        normalize(name));
    return i != mNamedItemInfos.end();
}

const ItemInfo &ItemDB::get(const int id)
{
    if (!mLoaded)
        load();

    const ItemInfos::const_iterator i = mItemInfos.find(id);

    if (i == mItemInfos.end())
    {
        reportAlways("ItemDB: Warning, unknown item ID# %d", id);
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
            reportAlways("ItemDB: Warning, unknown item name \"%s\"",
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

const ItemInfo &ItemDB::getEmpty()
{
    return *mUnknown;
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
#ifdef TMWA_SUPPORT
        if (Net::getNetworkType() == ServerType::TMWATHENA)
            id = -2;
        else
#endif
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
#ifdef TMWA_SUPPORT
        if (Net::getNetworkType() == ServerType::TMWATHENA)
            id = -3;
        else
#endif
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

static void loadSpriteRef(ItemInfo *const itemInfo, XmlNodeConstPtr node)
{
    const std::string gender = XML::getProperty(node, "gender", "unisex");
    if ((node == nullptr) || !XmlHaveChildContent(node))
        return;

    const std::string filename = XmlChildContent(node);

    const int race = XML::getProperty(node, "race", 0);
    if (gender == "male" || gender == "unisex")
        itemInfo->setSprite(filename, Gender::MALE, race);
    if (gender == "female" || gender == "unisex")
        itemInfo->setSprite(filename, Gender::FEMALE, race);
    // "other" is legacy from 20180630
    if (gender == "hidden" || gender == "other" || gender == "unisex")
        itemInfo->setSprite(filename, Gender::HIDDEN, race);
}

static void loadSoundRef(ItemInfo *const itemInfo, XmlNodeConstPtr node)
{
    if (node == nullptr ||
        !XmlHaveChildContent(node))
    {
        return;
    }
    const std::string event = XML::getProperty(node, "event", "");
    const int delay = XML::getProperty(node, "delay", 0);

    const std::map<std::string, ItemSoundEvent::Type>::const_iterator
        it = mSoundNames.find(event);
    if (it != mSoundNames.end())
    {
        const std::string filename = XmlChildContent(node);
        itemInfo->addSound((*it).second, filename, delay);
    }
    else
    {
        reportAlways("ItemDB: Ignoring unknown sound event '%s'",
            event.c_str());
    }
}

static void loadFloorSprite(SpriteDisplay &display,
                            XmlNodeConstPtrConst floorNode)
{
    if (floorNode == nullptr)
        return;
    for_each_xml_child_node(spriteNode, floorNode)
    {
        if (!XmlHaveChildContent(spriteNode))
            continue;
        if (xmlNameEqual(spriteNode, "sprite"))
        {
            SpriteReference *const currentSprite = new SpriteReference;
            currentSprite->sprite = XmlChildContent(spriteNode);
            currentSprite->variant
                = XML::getProperty(spriteNode, "variant", 0);
            display.sprites.push_back(currentSprite);
        }
        else if (xmlNameEqual(spriteNode, "particlefx"))
        {
            display.particles.push_back(XmlChildContent(spriteNode));
        }
    }
}

static void loadReplaceSprite(ItemInfo *const itemInfo,
                              XmlNodeConstPtr replaceNode)
{
    if (replaceNode == nullptr)
        return;
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
                    IntMap *const mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), f);
                    if (mapList == nullptr)
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
                    IntMap *mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), SpriteDirection::DOWN);
                    if (mapList != nullptr)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::DOWNLEFT);
                    if (mapList != nullptr)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::DOWNRIGHT);
                    if (mapList != nullptr)
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
                    IntMap *mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), SpriteDirection::UP);
                    if (mapList != nullptr)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::UPLEFT);
                    if (mapList != nullptr)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), SpriteDirection::UPRIGHT);
                    if (mapList != nullptr)
                        (*mapList)[from] = to;
                }
            }
            break;
        }
        default:
        {
            IntMap *const mapList = itemInfo->addReplaceSprite(
                parseSpriteName(removeSprite), direction);
            if (mapList == nullptr)
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
                            XmlNodeConstPtr node,
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

std::string ItemDB::getNamesStr(const STD_VECTOR<int> &parts)
{
    std::string str;
    FOR_EACH (STD_VECTOR<int>::const_iterator, it, parts)
    {
        const int id = *it;
        if (exists(id))
        {
            if (!str.empty())
                str.append(",");
            str.append(get(id).getName());
        }
    }
    return str;
}

int ItemDB::getNumOfHairstyles()
{
    return mNumberOfHairstyles;
}

#ifdef UNITTESTS
ItemDB::NamedItemInfos &ItemDB::getNamedItemInfosTest()
{
    return mNamedItemInfos;
}

ItemDB::ItemInfos &ItemDB::getItemInfosTest()
{
    return mItemInfos;
}
#endif  // UNITTESTS
