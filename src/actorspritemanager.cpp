/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "actorspritemanager.h"

#include "configuration.h"
#include "localplayer.h"
#include "logger.h"
#include "main.h"
#include "playerinfo.h"

#include "gui/chatwindow.h"
#include "gui/equipmentwindow.h"
#include "gui/socialwindow.h"
#include "gui/viewport.h"

#include "gui/widgets/chattab.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/iteminfo.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include <algorithm>
#include <list>

#include "debug.h"

#define for_actors ActorSpritesConstIterator it, it_end; \
for (it = mActors.begin(), it_end = mActors.end() ; it != it_end; ++it)

class FindBeingFunctor
{
    public:
        bool operator() (ActorSprite *actor)
        {
            if (!actor || actor->getType() == ActorSprite::FLOOR_ITEM
                || actor->getType() == ActorSprite::PORTAL)
            {
                return false;
            }
            Being* b = static_cast<Being*>(actor);

            unsigned other_y = y + ((b->getType()
                == ActorSprite::NPC) ? 1 : 0);
            const Vector &pos = b->getPosition();
            return (static_cast<unsigned>(pos.x) / 32 == x &&
                (static_cast<unsigned>(pos.y) / 32 == y
                || static_cast<unsigned>(pos.y) / 32 == other_y) &&
                b->isAlive() && (type == ActorSprite::UNKNOWN
                || b->getType() == type));
        }

        Uint16 x, y;
        ActorSprite::Type type;
} beingFinder;

class FindBeingEqualFunctor
{
    public:
        bool operator() (Being *being)
        {
            if (!being || !findBeing)
                return false;
            return being->getId() == findBeing->getId();
        }

        Being *findBeing;
} beingEqualFinder;

class SortBeingFunctor
{
    public:
        bool operator() (Being* being1,  Being* being2)
        {
            if (!being1 || !being2)
                return false;

            if (priorityBeings)
            {
                int w1 = defaultPriorityIndex;
                int w2 = defaultPriorityIndex;
                std::map<std::string, int>::const_iterator it1
                    = priorityBeings->find(being1->getName());
                std::map<std::string, int>::const_iterator it2
                    = priorityBeings->find(being2->getName());
                if (it1 != priorityBeings->end())
                    w1 = (*it1).second;
                if (it2 != priorityBeings->end())
                    w2 = (*it2).second;

                if (w1 != w2)
                    return w1 < w2;
            }
            if (being1->getDistance() != being2->getDistance())
            {
                if (specialDistance && being1->getDistance() <= 2
                    && being2->getDistance() <= attackRange
                    && being2->getDistance() > 2)
                {
                    return false;
                }
                else if (specialDistance && being2->getDistance() <= 2
                         && being1->getDistance() <= attackRange
                         && being1->getDistance() > 2)
                {
                    return true;
                }
                return being1->getDistance() < being2->getDistance();
            }

            int d1, d2;
#ifdef MANASERV_SUPPORT
            if (Net::getNetworkType() == ServerInfo::MANASERV)
            {
                const Vector &pos1 = being1->getPosition();
                d1 = abs((static_cast<int>(pos1.x)) - x)
                    + abs((static_cast<int>(pos1.y)) - y);
                const Vector &pos2 = being2->getPosition();
                d2 = abs((static_cast<int>(pos2.x)) - x)
                    + abs((static_cast<int>(pos2.y)) - y);
            }
            else
#endif
            {
                d1 = abs(being1->getTileX() - x) + abs(being1->getTileY() - y);
                d2 = abs(being2->getTileX() - x) + abs(being2->getTileY() - y);
            }

            if (d1 != d2)
                return d1 < d2;
            if (attackBeings)
            {
                int w1 = defaultAttackIndex;
                int w2 = defaultAttackIndex;
                std::map<std::string, int>::const_iterator it1
                    = attackBeings->find(being1->getName());
                std::map<std::string, int>::const_iterator it2
                    = attackBeings->find(being2->getName());
                if (it1 != attackBeings->end())
                    w1 = (*it1).second;
                if (it2 != attackBeings->end())
                    w2 = (*it2).second;

                if (w1 != w2)
                    return w1 < w2;
            }

            return (being1->getName() < being2->getName());
        }
        int x, y;
        std::map<std::string, int> *attackBeings;
        int defaultAttackIndex;
        std::map<std::string, int> *priorityBeings;
        int defaultPriorityIndex;
        bool specialDistance;
        int attackRange;
} beingSorter;

ActorSpriteManager::ActorSpriteManager() :
    mMap(nullptr)
{
    mSpellHeal1 = serverConfig.getValue("spellHeal1", "#lum");
    mSpellHeal2 = serverConfig.getValue("spellHeal2", "#inma");
    mSpellItenplz = serverConfig.getValue("spellItenplz", "#itenplz");
    mTargetDeadPlayers = config.getBoolValue("targetDeadPlayers");
    mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
    mCyclePlayers = config.getBoolValue("cyclePlayers");
    mCycleMonsters = config.getBoolValue("cycleMonsters");
    mExtMouseTargeting = config.getBoolValue("extMouseTargeting");

    config.addListener("targetDeadPlayers", this);
    config.addListener("targetOnlyReachable", this);
    config.addListener("cyclePlayers", this);
    config.addListener("cycleMonsters", this);
    config.addListener("extMouseTargeting", this);

    loadAttackList();
}

ActorSpriteManager::~ActorSpriteManager()
{
    config.removeListeners(this);
    storeAttackList();
    clear();
}

void ActorSpriteManager::setMap(Map *map)
{
    mMap = map;

    if (player_node)
        player_node->setMap(map);
}

void ActorSpriteManager::setPlayer(LocalPlayer *player)
{
    player_node = player;
    mActors.insert(player);
    if (socialWindow)
        socialWindow->updateAttackFilter();
    if (socialWindow)
        socialWindow->updatePickupFilter();
}

Being *ActorSpriteManager::createBeing(int id, ActorSprite::Type type,
                                       Uint16 subtype)
{
    Being *being = new Being(id, type, subtype, mMap);

    mActors.insert(being);
    return being;
}

FloorItem *ActorSpriteManager::createItem(int id, int itemId, int x, int y,
                                          int amount, unsigned char color,
                                          int subX, int subY)
{
    FloorItem *floorItem = new FloorItem(id, itemId, x, y,
        mMap, amount, color, subX, subY);

    if (!checkForPickup(floorItem))
        floorItem->disableHightlight();
    mActors.insert(floorItem);
    return floorItem;
}

void ActorSpriteManager::destroy(ActorSprite *actor)
{
    if (!actor || actor == player_node)
        return;

    mDeleteActors.insert(actor);
}

void ActorSpriteManager::erase(ActorSprite *actor)
{
    if (!actor || actor == player_node)
        return;

    mActors.erase(actor);
}

void ActorSpriteManager::undelete(ActorSprite *actor)
{
    if (!actor || actor == player_node)
        return;

    ActorSpritesConstIterator it, it_end;

    for (it = mDeleteActors.begin(), it_end = mDeleteActors.end();
         it != it_end; ++it)
    {
        if (*it == actor)
        {
            mDeleteActors.erase(*it);
            return;
        }
    }
}

Being *ActorSpriteManager::findBeing(int id) const
{
    for_actors
    {
        ActorSprite *actor = *it;
        if (actor->getId() == id &&
            actor->getType() != ActorSprite::FLOOR_ITEM)
        {
            return static_cast<Being*>(actor);
        }
    }

    return nullptr;
}

Being *ActorSpriteManager::findBeing(int x, int y,
                                     ActorSprite::Type type) const
{
    beingFinder.x = static_cast<Uint16>(x);
    beingFinder.y = static_cast<Uint16>(y);
    beingFinder.type = type;

    ActorSpritesConstIterator it = find_if(mActors.begin(), mActors.end(),
                                           beingFinder);

    return (it == mActors.end()) ? nullptr : static_cast<Being*>(*it);
}

Being *ActorSpriteManager::findBeingByPixel(int x, int y,
                                            bool allPlayers) const
{
    if (!mMap)
        return nullptr;

    bool targetDead = mTargetDeadPlayers;

    if (mExtMouseTargeting)
    {
        Being *tempBeing = nullptr;
        bool noBeing(false);

        for_actors
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorSprite::PORTAL)
                continue;

            if ((*it)->getType() == ActorSprite::FLOOR_ITEM)
            {
                if (!noBeing)
                {
                    FloorItem *floor = static_cast<FloorItem*>(*it);
                    if (!noBeing && (floor->getPixelX() - 32 <= x) &&
                        (floor->getPixelX() + 32 > x) &&
                        (floor->getPixelY() - 64 <= y) &&
                        (floor->getPixelY() + 16 > y))
                    {
                        noBeing = true;
                    }
                }
                continue;
            }

            Being *being = static_cast<Being*>(*it);

            if ((being->isAlive()
                || (targetDead && being->getType() == Being::PLAYER))
                && (allPlayers ||  being != player_node))
            {

                if ((being->getPixelX() - 16 <= x) &&
                    (being->getPixelX() + 16 > x) &&
                    (being->getPixelY() - 32 <= y) &&
                    (being->getPixelY() > y))
                {
                    return being;
                }
                else if (!noBeing && (being->getPixelX() - 32 <= x) &&
                         (being->getPixelX() + 32 > x) &&
                         (being->getPixelY() - 64 <= y) &&
                         (being->getPixelY() + 16 > y))
                {
                    if (tempBeing)
                        noBeing = true;
                    else
                        tempBeing = being;
                }
            }
        }

        if (noBeing)
            return nullptr;
        return tempBeing;
    }
    else
    {
        for_actors
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorSprite::PORTAL ||
                (*it)->getType() == ActorSprite::FLOOR_ITEM)
            {
                continue;
            }

            Being *being = static_cast<Being*>(*it);

            if ((being->getPixelX() - 16 <= x) &&
                (being->getPixelX() + 16 > x) &&
                (being->getPixelY() - 32 <= y) &&
                (being->getPixelY() > y))
            {
                return being;
            }
        }
        return nullptr;
    }
}

void ActorSpriteManager::findBeingsByPixel(std::vector<ActorSprite*> &beings,
                                           int x, int y, bool allPlayers) const
{
    if (!mMap)
        return;

    const int xtol = 16;
    const int uptol = 32;

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::PORTAL)
            continue;

        Being *being = dynamic_cast<Being*>(*it);
        ActorSprite *actor = *it;

        if ((being && (being->isAlive()
            || (mTargetDeadPlayers && being->getType() == Being::PLAYER))
            && (allPlayers ||  being != player_node))
            || actor->getType() == ActorSprite::FLOOR_ITEM)
        {

            if ((actor->getPixelX() - xtol <= x) &&
                (actor->getPixelX() + xtol > x) &&
                (actor->getPixelY() - uptol <= y) &&
                (actor->getPixelY() > y))
            {
                beings.push_back(actor);
            }
        }
    }
}

Being *ActorSpriteManager::findPortalByTile(int x, int y) const
{
    if (!mMap)
        return nullptr;

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() != ActorSprite::PORTAL)
            continue;

        Being *being = static_cast<Being*>(*it);

        if (being->getTileX() == x && being->getTileY() == y)
            return being;
    }

    return nullptr;
}

FloorItem *ActorSpriteManager::findItem(int id) const
{
    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getId() == id &&
            (*it)->getType() == ActorSprite::FLOOR_ITEM)
        {
            return static_cast<FloorItem*>(*it);
        }
    }

    return nullptr;
}

FloorItem *ActorSpriteManager::findItem(int x, int y) const
{
    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getTileX() == x && (*it)->getTileY() == y &&
            (*it)->getType() == ActorSprite::FLOOR_ITEM)
        {
            return static_cast<FloorItem*>(*it);
        }
    }

    return nullptr;
}

bool ActorSpriteManager::pickUpAll(int x1, int y1, int x2, int y2,
                                   bool serverBuggy)
{
    if (!player_node)
        return false;

    bool finded(false);
    bool allowAll = mPickupItemsSet.find("") != mPickupItemsSet.end();
    if (!serverBuggy)
    {
        for_actors
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorSprite::FLOOR_ITEM
                && ((*it)->getTileX() >= x1 && (*it)->getTileX() <= x2)
                && ((*it)->getTileY() >= y1 && (*it)->getTileY() <= y2))
            {
                FloorItem *item = static_cast<FloorItem*>(*it);
                const ItemInfo &info = item->getInfo();
                std::string name;
                if (serverVersion > 0)
                    name = info.getName(item->getColor());
                else
                    name = info.getName();
                if (allowAll)
                {
                    if (mIgnorePickupItemsSet.find(name)
                        == mIgnorePickupItemsSet.end())
                    {
                        if (player_node->pickUp(item))
                            finded = true;
                    }
                }
                else
                {
                    if (mPickupItemsSet.find(name) != mPickupItemsSet.end())
                    {
                        if (player_node->pickUp(item))
                            finded = true;
                    }
                }
            }
        }
    }
    else if (Client::checkPackets(PACKET_PICKUP))
    {
        FloorItem *item = nullptr;
        unsigned cnt = 65535;
        for_actors
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorSprite::FLOOR_ITEM
                && ((*it)->getTileX() >= x1 && (*it)->getTileX() <= x2)
                && ((*it)->getTileY() >= y1 && (*it)->getTileY() <= y2))
            {
                FloorItem *tempItem = static_cast<FloorItem*>(*it);
                if (tempItem->getPickupCount() < cnt)
                {
                    const ItemInfo &info = tempItem->getInfo();
                    std::string name;
                    if (serverVersion > 0)
                        name = info.getName(tempItem->getColor());
                    else
                        name = info.getName();
                    if (allowAll)
                    {
                        if (mIgnorePickupItemsSet.find(name)
                            == mIgnorePickupItemsSet.end())
                        {
                            item = tempItem;
                            cnt = item->getPickupCount();
                            if (cnt == 0)
                            {
                                item->incrementPickup();
                                player_node->pickUp(item);
                                return true;
                            }
                        }
                    }
                    else
                    {
                        if (mPickupItemsSet.find(name)
                            != mPickupItemsSet.end())
                        {
                            item = tempItem;
                            cnt = item->getPickupCount();
                            if (cnt == 0)
                            {
                                item->incrementPickup();
                                player_node->pickUp(item);
                                return true;
                            }
                        }
                    }
                }
            }
        }
        if (item && player_node->pickUp(item))
            finded = true;
    }
    return finded;
}

bool ActorSpriteManager::pickUpNearest(int x, int y, int maxdist)
{
    if (!player_node)
        return false;

    maxdist = maxdist * maxdist;
    FloorItem *closestItem = nullptr;
    int dist = 0;
    bool allowAll = mPickupItemsSet.find("") != mPickupItemsSet.end();

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM)
        {
            FloorItem *item = static_cast<FloorItem*>(*it);

            int d = (item->getTileX() - x) * (item->getTileX() - x)
                    + (item->getTileY() - y) * (item->getTileY() - y);

            if ((d < dist || !closestItem) && (!mTargetOnlyReachable
                || player_node->isReachable(item->getTileX(),
                item->getTileY())))
            {
                const ItemInfo &info = item->getInfo();
                std::string name;
                if (serverVersion > 0)
                    name = info.getName(item->getColor());
                else
                    name = info.getName();
                if (allowAll)
                {
                    if (mIgnorePickupItemsSet.find(name)
                        == mIgnorePickupItemsSet.end())
                    {
                        dist = d;
                        closestItem = item;
                    }
                }
                else
                {
                    if (mPickupItemsSet.find(name) != mPickupItemsSet.end())
                    {
                        dist = d;
                        closestItem = item;
                    }
                }
            }
        }
    }
    if (closestItem && player_node && dist <= maxdist)
        return player_node->pickUp(closestItem);

    return false;
}

Being *ActorSpriteManager::findBeingByName(const std::string &name,
                                           ActorSprite::Type type) const
{
    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        if (being->getName() == name &&
            (type == ActorSprite::UNKNOWN || type == being->getType()))
        {
            return being;
        }
    }
    return nullptr;
}

Being *ActorSpriteManager::findNearestByName(const std::string &name,
                                             Being::Type type) const
{
    if (!player_node)
        return nullptr;

    int dist = 0;
    Being* closestBeing = nullptr;
    int x, y;

    x = player_node->getTileX();
    y = player_node->getTileY();

    for_actors
    {
        if (reportTrue(!*it))
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);

        if (being && being->getName() == name &&
            (type == Being::UNKNOWN || type == being->getType()))
        {
            if (being->getType() == Being::PLAYER)
                return being;
            else
            {
                int d = (being->getTileX() - x) * (being->getTileX() - x)
                        + (being->getTileY() - y) * (being->getTileY() - y);

                if (validateBeing(nullptr, being, type, nullptr, 50)
                    && (d < dist || closestBeing == nullptr))
                {
                    dist = d;
                    closestBeing = being;
                }
            }
        }
    }
    return closestBeing;
}

const ActorSprites &ActorSpriteManager::getAll() const
{
    return mActors;
}

void ActorSpriteManager::logic()
{
    for_actors
    {
        if (*it)
            (*it)->logic();
    }

    if (mDeleteActors.empty())
        return;

    for (it = mDeleteActors.begin(), it_end = mDeleteActors.end();
         it != it_end; ++it)
    {
        if (!*it)
            continue;

        if ((*it) && (*it)->getType() == Being::PLAYER)
        {
            Being *being = static_cast<Being*>(*it);
            being->addToCache();
            if (beingEquipmentWindow)
                beingEquipmentWindow->resetBeing(being);
        }
        if (player_node)
        {
            if (player_node->getTarget() == *it)
                player_node->setTarget(nullptr);
            if (player_node->getPickUpTarget() == *it)
                player_node->unSetPickUpTarget();
        }
        if (viewport)
            viewport->clearHover(*it);
    }

    for (it = mDeleteActors.begin(), it_end = mDeleteActors.end();
         it != it_end; ++it)
    {
        mActors.erase(*it);
        delete *it;
    }

    mDeleteActors.clear();
}

void ActorSpriteManager::clear()
{
    if (beingEquipmentWindow)
        beingEquipmentWindow->setBeing(nullptr);

    if (player_node)
    {
        player_node->setTarget(nullptr);
        player_node->unSetPickUpTarget();
        mActors.erase(player_node);
    }

    for_actors
    {
        delete *it;
    }
    mActors.clear();
    mDeleteActors.clear();

    if (player_node)
        mActors.insert(player_node);
}

Being *ActorSpriteManager::findNearestLivingBeing(int x, int y,
                                                  int maxTileDist,
                                                  ActorSprite::Type type,
                                                  Being *excluded) const
{
    const int maxDist = maxTileDist * 32;

    return findNearestLivingBeing(nullptr, maxDist, type, x, y, excluded);
}

Being *ActorSpriteManager::findNearestLivingBeing(Being *aroundBeing,
                                                  int maxDist,
                                                  Being::Type type) const
{
    if (!aroundBeing)
        return nullptr;

    int x = aroundBeing->getTileX();
    int y = aroundBeing->getTileY();

    return findNearestLivingBeing(aroundBeing, maxDist, type,
                                  x, y, aroundBeing);
}

Being *ActorSpriteManager::findNearestLivingBeing(Being *aroundBeing,
                                                  int maxDist,
                                                  Being::Type type,
                                                  int x, int y,
                                                  Being *excluded) const
{
    if (!aroundBeing || !player_node)
        return nullptr;

    Being *closestBeing = nullptr;
    std::set<std::string> attackMobs;
    std::set<std::string> priorityMobs;
    std::set<std::string> ignoreAttackMobs;
    std::map<std::string, int> attackMobsMap;
    std::map<std::string, int> priorityMobsMap;
    int defaultAttackIndex = 10000;
    int defaultPriorityIndex = 10000;
    const int attackRange = player_node->getAttackRange();

    bool specialDistance = false;
    if (player_node->getMoveToTargetType() == 7
        && player_node->getAttackRange() > 2)
    {
        specialDistance = true;
    }

    maxDist = maxDist * maxDist;

    bool cycleSelect = (mCyclePlayers && type == Being::PLAYER)
        || (mCycleMonsters && type == Being::MONSTER);

    bool filtered = config.getBoolValue("enableAttackFilter")
        && type != Being::PLAYER;

    bool ignoreDefault = false;
    if (filtered)
    {
        attackMobs = mAttackMobsSet;
        priorityMobs = mPriorityAttackMobsSet;
        ignoreAttackMobs = mIgnoreAttackMobsSet;
        attackMobsMap = mAttackMobsMap;
        priorityMobsMap = mPriorityAttackMobsMap;
        beingSorter.attackBeings = &attackMobsMap;
        beingSorter.priorityBeings = &priorityMobsMap;
        beingSorter.specialDistance = specialDistance;
        beingSorter.attackRange = attackRange;
        if (ignoreAttackMobs.find("") != ignoreAttackMobs.end())
            ignoreDefault = true;
        std::map<std::string, int>::const_iterator
            itr = attackMobsMap.find("");
        if (itr != attackMobsMap.end())
            defaultAttackIndex = (*itr).second;
        itr = priorityMobsMap.find("");
        if (itr != priorityMobsMap.end())
            defaultPriorityIndex = (*itr).second;
    }

    if (cycleSelect)
    {
        std::vector<Being*> sortedBeings;

        for (ActorSprites::const_iterator i = mActors.begin(),
             i_end = mActors.end();
             i != i_end; ++i)
        {
            if (!*i)
                continue;

            if ((*i)->getType() == ActorSprite::FLOOR_ITEM
                || (*i)->getType() == ActorSprite::PORTAL)
            {
                continue;
            }

            Being *being = static_cast<Being*>(*i);

            if (filtered)
            {
                if (ignoreAttackMobs.find(being->getName())
                    != ignoreAttackMobs.end())
                {
                    continue;
                }
                if (ignoreDefault && attackMobs.find(being->getName())
                    == attackMobs.end() && priorityMobs.find(being->getName())
                    == priorityMobs.end())
                {
                    continue;
                }
            }
            if (validateBeing(aroundBeing, being, type, nullptr, maxDist))
            {
                if (being != excluded)
                    sortedBeings.push_back(being);
            }
        }

        // no selectable beings
        if (sortedBeings.empty())
            return nullptr;

        beingSorter.x = x;
        beingSorter.y = y;
        if (filtered)
        {
            beingSorter.attackBeings = &attackMobsMap;
            beingSorter.defaultAttackIndex = defaultAttackIndex;
            beingSorter.priorityBeings = &priorityMobsMap;
            beingSorter.defaultPriorityIndex = defaultPriorityIndex;
        }
        else
        {
            beingSorter.attackBeings = nullptr;
            beingSorter.priorityBeings = nullptr;
        }
        sort(sortedBeings.begin(), sortedBeings.end(), beingSorter);
        if (filtered)
        {
            beingSorter.attackBeings = nullptr;
            beingSorter.priorityBeings = nullptr;
        }

        if (player_node->getTarget() == nullptr)
        {
            Being *target = sortedBeings.at(0);

            if (specialDistance && target->getType() == Being::MONSTER
                && target->getDistance() <= 2)
            {
                return nullptr;
            }
            // if no selected being in vector, return first nearest being
            return target;
        }

        beingEqualFinder.findBeing = player_node->getTarget();
        std::vector<Being*>::const_iterator i = find_if(sortedBeings.begin(),
            sortedBeings.end(), beingEqualFinder);

        if (i == sortedBeings.end() || ++i == sortedBeings.end())
        {
            // if no selected being in vector, return first nearest being
            return sortedBeings.at(0);
        }

        // we find next being after target
        return *i;
    }
    else
    {
        int dist = 0;
        int index = defaultPriorityIndex;

        for (ActorSprites::const_iterator i = mActors.begin(),
             i_end = mActors.end();
             i != i_end; ++i)
        {
            if (!*i)
                continue;

            if ((*i)->getType() == ActorSprite::FLOOR_ITEM
                || (*i)->getType() == ActorSprite::PORTAL)
            {
                continue;
            }
            Being *being = static_cast<Being*>(*i);

            if (filtered)
            {
                if (ignoreAttackMobs.find(being->getName())
                    != ignoreAttackMobs.end())
                {
                    continue;
                }
                if (ignoreDefault && attackMobs.find(being->getName())
                    == attackMobs.end() && priorityMobs.find(being->getName())
                    == priorityMobs.end())
                {
                    continue;
                }
            }

//            Being *being = (*i);

            bool valid = validateBeing(aroundBeing, being, type, excluded, 50);
            int d = being->getDistance();
//            logger->log("dist: %d", dist);
//            logger->log("name: %s, %d, %d", being->getName().c_str(), (int)valid, d);
            if (being->getType() != Being::MONSTER
                || !mTargetOnlyReachable)
            {   // if distance not calculated, use old distance
                d = (being->getTileX() - x) * (being->getTileX() - x)
                    + (being->getTileY() - y) * (being->getTileY() - y);
            }

            if (!valid)
                continue;

            if (specialDistance && being->getDistance() <= 2
                && being->getType() == Being::MONSTER)
            {
                continue;
            }

//            logger->log("being name:" + being->getName());
//            logger->log("index:" + toString(index));
//            logger->log("d:" + toString(d));

            if (valid && !filtered && (d <= dist || !closestBeing))
            {
                dist = d;
                closestBeing = being;
            }
            else if (valid && filtered)
            {
                int w2 = defaultPriorityIndex;
                if (closestBeing)
                {
                    std::map<std::string, int>::const_iterator it2
                        = priorityMobsMap.find(being->getName());
                    if (it2 != priorityMobsMap.end())
                        w2 = (*it2).second;

                    if (w2 < index)
                    {
                        dist = d;
                        closestBeing = being;
                        index = w2;
                        continue;
                    }
                    if (w2 == index && d <= dist)
                    {
                        dist = d;
                        closestBeing = being;
                        index = w2;
                        continue;
                    }
                }

                if (!closestBeing)
                {
                    dist = d;
                    closestBeing = being;
                    std::map<std::string, int>::const_iterator it1
                        = priorityMobsMap.find(being->getName());
                    if (it1 != priorityMobsMap.end())
                        index = (*it1).second;
                    else
                        index = defaultPriorityIndex;
                }
            }
        }
        return (maxDist >= dist) ? closestBeing : nullptr;
    }
}

bool ActorSpriteManager::validateBeing(Being *aroundBeing, Being* being,
                                       Being::Type type, Being* excluded,
                                       int maxCost) const
{
    return being && ((being->getType() == type
        || type == Being::UNKNOWN) && (being->isAlive()
        || (mTargetDeadPlayers && type == Being::PLAYER))
        && being != aroundBeing) && being != excluded
        && (type != Being::MONSTER || !mTargetOnlyReachable
        || player_node->isReachable(being, maxCost));
}

void ActorSpriteManager::healTarget()
{
    if (!player_node)
        return;

    heal(player_node->getTarget());
}

void ActorSpriteManager::heal(Being* target)
{
    if (!player_node || !chatWindow || !player_node->isAlive()
        || !Net::getPlayerHandler()->canUseMagic())
    {
        return;
    }

    if (target && player_node->getName() == target->getName())
    {
        if (PlayerInfo::getAttribute(MP) >= 6
            && PlayerInfo::getAttribute(HP)
            != PlayerInfo::getAttribute(MAX_HP))
        {
            if (!Client::limitPackets(PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal1);
        }
    }
    else if (PlayerInfo::getStatEffective(340) < 2
             || PlayerInfo::getStatEffective(341) < 2)
    {
        if (PlayerInfo::getAttribute(MP) >= 6)
        {
            if (target && target->getType() != Being::MONSTER)
            {
                if (!Client::limitPackets(PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1 + " "
                                           + target->getName());
            }
            else if (PlayerInfo::getAttribute(HP)
                     != PlayerInfo::getAttribute(MAX_HP))
            {
                if (!Client::limitPackets(PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1);
            }
        }
    }
    else
    {
        if (PlayerInfo::getAttribute(MP) >= 10 && target
            && target->getType() != Being::MONSTER)
        {
            if (!Client::limitPackets(PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal2 + " " + target->getName());
        }
        else if ((!target || target->getType() == Being::MONSTER)
                 && PlayerInfo::getAttribute(MP) >= 6
                 && PlayerInfo::getAttribute(HP)
                 != PlayerInfo::getAttribute(MAX_HP))
        {
            if (!Client::limitPackets(PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal1);
        }
    }
}

void ActorSpriteManager::itenplz()
{
    if (!player_node || !chatWindow || !player_node->isAlive()
        || !Net::getPlayerHandler()->canUseMagic())
    {
        return;
    }

    if (!Client::limitPackets(PACKET_CHAT))
        return;

    chatWindow->localChatInput(mSpellItenplz);
}

bool ActorSpriteManager::hasActorSprite(ActorSprite *actor) const
{
    for_actors
    {
        if (actor == *it)
            return true;
    }

    return false;
}

void ActorSpriteManager::addBlock(Uint32 id)
{
    bool alreadyBlocked(false);
    for (int i = 0; i < static_cast<int>(blockedBeings.size()); ++i)
    {
        if (id == blockedBeings.at(i))
        {
            alreadyBlocked = true;
            break;
        }
    }
    if (alreadyBlocked == false)
        blockedBeings.push_back(id);
}

void ActorSpriteManager::deleteBlock(Uint32 id)
{
    std::vector<Uint32>::iterator iter = blockedBeings.begin();
    std::vector<Uint32>::iterator iter_end = blockedBeings.end();
    while (iter != iter_end)
    {
        if (*iter == id)
        {
            iter = blockedBeings.erase(iter);
            break;
        }
    }
}

bool ActorSpriteManager::isBlocked(Uint32 id)
{
    bool blocked(false);
    for (int i = 0; i < static_cast<int>(blockedBeings.size()); ++i)
    {
        if (id == blockedBeings.at(i))
        {
            blocked = true;
            break;
        }
    }
    return blocked;
}

void ActorSpriteManager::printAllToChat() const
{
    printBeingsToChat(getAll(), _("Visible on map"));
}

void ActorSpriteManager::printBeingsToChat(ActorSprites beings,
                                           std::string header) const
{
    if (!debugChatTab)
        return;

    debugChatTab->chatLog("---------------------------------------");
    debugChatTab->chatLog(header);
    std::set<ActorSprite*>::const_iterator it;
    std::set<ActorSprite*>::const_iterator it_end = beings.end();
    for (it = beings.begin(); it != it_end; ++it)
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM)
            continue;

        const Being *being = static_cast<Being*>(*it);

        debugChatTab->chatLog(being->getName()
            + " (" + toString(being->getTileX()) + ","
            + toString(being->getTileY()) + ") "
            + toString(being->getSubType()), BY_SERVER);
    }
    debugChatTab->chatLog("---------------------------------------");
}

void ActorSpriteManager::printBeingsToChat(std::vector<Being*> beings,
                                           std::string header) const
{
    if (!debugChatTab)
        return;

    debugChatTab->chatLog("---------------------------------------");
    debugChatTab->chatLog(header);

    std::vector<Being*>::const_iterator i;
    std::vector<Being*>::const_iterator i_end = beings.end();
    for (i = beings.begin(); i != i_end; ++i)
    {
        if (!*i)
            continue;

        const Being *being = *i;

        debugChatTab->chatLog(being->getName()
            + " (" + toString(being->getTileX()) + ","
            + toString(being->getTileY()) + ") "
            + toString(being->getSubType()), BY_SERVER);
    }
    debugChatTab->chatLog("---------------------------------------");
}

void ActorSpriteManager::getPlayerNames(StringVect &names, bool npcNames)
{
    names.clear();

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        if ((being->getType() == ActorSprite::PLAYER
            || (being->getType() == ActorSprite::NPC && npcNames))
            && being->getName() != "")
        {
            names.push_back(being->getName());
        }
    }
}

void ActorSpriteManager::getMobNames(StringVect &names)
{
    names.clear();

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        if (being->getType() == ActorSprite::MONSTER && being->getName() != "")
            names.push_back(being->getName());
    }
}

void ActorSpriteManager::updatePlayerNames()
{
    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        being->setGoodStatus(-1);
        if (being->getType() == ActorSprite::PLAYER && being->getName() != "")
            being->updateName();
    }
}

void ActorSpriteManager::updatePlayerColors()
{
    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        if (being->getType() == ActorSprite::PLAYER && being->getName() != "")
            being->updateColors();
    }
}

void ActorSpriteManager::updatePlayerGuild()
{
    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        if (being->getType() == ActorSprite::PLAYER && being->getName() != "")
            being->updateGuild();
    }
}

void ActorSpriteManager::parseLevels(std::string levels)
{
    levels += ", ";
    unsigned int f = 0;
    size_t pos = 0;
    const std::string brkEnd = "), ";

    pos = levels.find(brkEnd, f);
    while (pos != std::string::npos)
    {
        std::string part = levels.substr(f, pos - f);
        if (part.empty())
            break;
        size_t bktPos = part.rfind("(");
        if (bktPos != std::string::npos)
        {
            Being *being = findBeingByName(part.substr(0, bktPos),
                                           Being::PLAYER);
            if (being)
            {
                being->setLevel(atoi(part.substr(bktPos + 1).c_str()));
                being->addToCache();
            }
        }
        f = static_cast<int>(pos + brkEnd.length());
        pos = levels.find(brkEnd, f);
    }
    updatePlayerNames();
}

void ActorSpriteManager::optionChanged(const std::string &name)
{
    if (name == "targetDeadPlayers")
        mTargetDeadPlayers = config.getBoolValue("targetDeadPlayers");
    else if (name == "targetOnlyReachable")
        mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
    else if (name == "cyclePlayers")
        mCyclePlayers = config.getBoolValue("cyclePlayers");
    else if (name == "cycleMonsters")
        mCycleMonsters = config.getBoolValue("cycleMonsters");
    else if (name == "extMouseTargeting")
        mExtMouseTargeting = config.getBoolValue("extMouseTargeting");
}

void ActorSpriteManager::removeAttackMob(const std::string &name)
{
    mPriorityAttackMobs.remove(name);
    mAttackMobs.remove(name);
    mIgnoreAttackMobs.remove(name);
    mPriorityAttackMobsSet.erase(name);
    mAttackMobsSet.erase(name);
    mIgnoreAttackMobsSet.erase(name);
    rebuildAttackMobs();
    rebuildPriorityAttackMobs();
}

void ActorSpriteManager::removePickupItem(const std::string &name)
{
    mPickupItems.remove(name);
    mPickupItemsSet.erase(name);
    mIgnorePickupItems.remove(name);
    mIgnorePickupItemsSet.erase(name);
    rebuildPickupItems();
}

#define addMobToList(name, mob) \
{\
    int size = get##mob##sSize();\
    if (size > 0)\
    {\
        int idx = get##mob##Index("");\
        if (idx + 1 == size)\
        {\
            std::list<std::string>::iterator itr = m##mob##s.end();\
            -- itr;\
            m##mob##s.insert(itr, name);\
        }\
        else\
        {\
            m##mob##s.push_back(name);\
        }\
    }\
    else\
    {\
        m##mob##s.push_back(name);\
    }\
    m##mob##sSet.insert(name);\
    rebuild##mob##s();\
}

#define rebuildMobsList(mob) \
{\
    m##mob##sMap.clear();\
    std::list<std::string>::const_iterator i = m##mob##s.begin();\
    int cnt = 0;\
    while (i != m##mob##s.end())\
    {\
        m##mob##sMap[*i] = cnt;\
        ++ i;\
        ++ cnt;\
    }\
}

void ActorSpriteManager::addAttackMob(std::string name)
{
    addMobToList(name, AttackMob);
    rebuildPriorityAttackMobs();
}

void ActorSpriteManager::addPriorityAttackMob(std::string name)
{
    addMobToList(name, PriorityAttackMob);
}

void ActorSpriteManager::addIgnoreAttackMob(std::string name)
{
    mIgnoreAttackMobs.push_back(name);
    mIgnoreAttackMobsSet.insert(name);
    rebuildAttackMobs();
    rebuildPriorityAttackMobs();
}

void ActorSpriteManager::addPickupItem(std::string name)
{
    addMobToList(name, PickupItem);
    rebuildPickupItems();
}

void ActorSpriteManager::addIgnorePickupItem(std::string name)
{
    mIgnorePickupItems.push_back(name);
    mIgnorePickupItemsSet.insert(name);
    rebuildPickupItems();
}

void ActorSpriteManager::rebuildPriorityAttackMobs()
{
    rebuildMobsList(PriorityAttackMob);
}

void ActorSpriteManager::rebuildAttackMobs()
{
    rebuildMobsList(AttackMob);
}

void ActorSpriteManager::rebuildPickupItems()
{
    rebuildMobsList(PickupItem);
}

int ActorSpriteManager::getIndexByName(std::string name,
                                       std::map<std::string, int> &map)
{
    std::map<std::string, int>::const_iterator
        i = map.find(name);
    if (i == map.end())
        return -1;

    return (*i).second;
}

int ActorSpriteManager::getPriorityAttackMobIndex(std::string name)
{
    return getIndexByName(name, mPriorityAttackMobsMap);
}

int ActorSpriteManager::getAttackMobIndex(std::string name)
{
    return getIndexByName(name, mAttackMobsMap);
}

int ActorSpriteManager::getPickupItemIndex(std::string name)
{
    return getIndexByName(name, mPickupItemsMap);
}

#define loadList(key, mob) \
{\
    list = unpackList(serverConfig.getValue(key, ""));\
    i = list.begin();\
    i_end = list.end();\
    while (i != i_end)\
    {\
        if (*i == "")\
            empty = true;\
        m##mob##s.push_back(*i);\
        m##mob##sSet.insert(*i);\
        ++ i;\
    }\
}

void ActorSpriteManager::loadAttackList()
{
    bool empty = false;
    std::list<std::string> list;
    std::list<std::string>::const_iterator i;
    std::list<std::string>::const_iterator i_end;

    loadList("attackPriorityMobs", PriorityAttackMob);
    loadList("attackMobs", AttackMob);
    loadList("ignoreAttackMobs", IgnoreAttackMob);
    if (!empty)
    {
        mAttackMobs.push_back("");
        mAttackMobsSet.insert("");
        empty = false;
    }

    loadList("pickupItems", PickupItem);
    loadList("ignorePickupItems", IgnorePickupItem);
    if (!empty)
    {
        mPickupItems.push_back("");
        mPickupItemsSet.insert("");
    }

    rebuildAttackMobs();
    rebuildPriorityAttackMobs();
    rebuildPickupItems();
}

void ActorSpriteManager::storeAttackList()
{
    serverConfig.setValue("attackPriorityMobs", packList(mPriorityAttackMobs));
    serverConfig.setValue("attackMobs", packList(mAttackMobs));
    serverConfig.setValue("ignoreAttackMobs", packList(mIgnoreAttackMobs));

    serverConfig.setValue("pickupItems", packList(mPickupItems));
    serverConfig.setValue("ignorePickupItems", packList(mIgnorePickupItems));
}

bool ActorSpriteManager::checkForPickup(FloorItem *item)
{
    const ItemInfo &info = item->getInfo();
    std::string name;
    if (serverVersion > 0)
        name = info.getName(item->getColor());
    else
        name = info.getName();

    if (mPickupItemsSet.find("") != mPickupItemsSet.end())
    {
        if (mIgnorePickupItemsSet.find(name) == mIgnorePickupItemsSet.end())
            return true;
    }
    else if (mPickupItemsSet.find(name) != mPickupItemsSet.end())
    {
        return true;
    }
    return false;
}
