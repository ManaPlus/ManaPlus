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

#include "actorspritemanager.h"

#include "configuration.h"
#include "localplayer.h"
#include "log.h"
#include "main.h"
#include "playerinfo.h"
#include "playerrelations.h"

#include "gui/widgets/chattab.h"
#include "gui/killstats.h"
#include "gui/chatwindow.h"
#include "gui/skilldialog.h"
#include "gui/viewport.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include <algorithm>
#include <list>
#include <vector>

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
                std::map<std::string, int>::iterator it1
                    = priorityBeings->find(being1->getName());
                std::map<std::string, int>::iterator it2
                    = priorityBeings->find(being2->getName());
                if (it1 != priorityBeings->end())
                    w1 = (*it1).second;
                if (it2 != priorityBeings->end())
                    w2 = (*it2).second;

                if (w1 != w2)
                    return w1 < w2;
            }
            if (being1->getDistance() != being2->getDistance())
                return being1->getDistance() < being2->getDistance();

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
                std::map<std::string, int>::iterator it1
                    = attackBeings->find(being1->getName());
                std::map<std::string, int>::iterator it2
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

} beingSorter;

ActorSpriteManager::ActorSpriteManager() :
    mMap(0)
{
    mSpellHeal1 = serverConfig.getValue("spellHeal1", "#lum");
    mSpellHeal2 = serverConfig.getValue("spellHeal2", "#inma");
    mSpellItenplz = serverConfig.getValue("spellItenplz", "#itenplz");
    mTargetDeadPlayers = config.getBoolValue("targetDeadPlayers");
    mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
    mCyclePlayers = config.getBoolValue("cyclePlayers");
    mCycleMonsters = config.getBoolValue("cycleMonsters");

    config.addListener("targetDeadPlayers", this);
    config.addListener("targetOnlyReachable", this);
    config.addListener("cyclePlayers", this);
    config.addListener("cycleMonsters", this);
}

ActorSpriteManager::~ActorSpriteManager()
{
    config.removeListener("targetDeadPlayers", this);
    config.removeListener("targetOnlyReachable", this);
    config.removeListener("cyclePlayers", this);
    config.removeListener("cycleMonsters", this);
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
}

Being *ActorSpriteManager::createBeing(int id, ActorSprite::Type type,
                                       Uint16 subtype)
{
    Being *being = new Being(id, type, subtype, mMap);

    mActors.insert(being);
    return being;
}

FloorItem *ActorSpriteManager::createItem(int id, int itemId, int x, int y,
                                          int amount, unsigned char color)
{
    FloorItem *floorItem = new FloorItem(id, itemId, x, y,
        mMap, amount, color);

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
        ActorSprite *actor = *it;
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

    return NULL;
}

Being *ActorSpriteManager::findBeing(int x, int y,
                                     ActorSprite::Type type) const
{
    beingFinder.x = static_cast<Uint16>(x);
    beingFinder.y = static_cast<Uint16>(y);
    beingFinder.type = type;

    ActorSpritesConstIterator it = find_if(mActors.begin(), mActors.end(),
                                           beingFinder);

    return (it == mActors.end()) ? NULL : static_cast<Being*>(*it);
}

Being *ActorSpriteManager::findBeingByPixel(int x, int y,
                                            bool allPlayers) const
{
    if (!mMap)
        return NULL;

    bool targetDead = mTargetDeadPlayers;
    const int xtol = 16;
    const int uptol = 32;

    for_actors
    {
        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);

        if ((being->isAlive()
            || (targetDead && being->getType() == Being::PLAYER))
            && (allPlayers ||  being != player_node))
        {

            if ((being->getPixelX() - xtol <= x) &&
                (being->getPixelX() + xtol > x) &&
                (being->getPixelY() - uptol <= y) &&
                (being->getPixelY() > y))
            {
                return being;
            }
        }
    }

    return NULL;
}

void ActorSpriteManager::findBeingsByPixel(std::list<Being*> &beings,
                                           int x, int y, bool allPlayers) const
{
    if (!mMap)
        return;

    bool targetDead = mTargetDeadPlayers;
    const int xtol = 16;
    const int uptol = 32;

    for_actors
    {
        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);

        if ((being->isAlive()
            || (targetDead && being->getType() == Being::PLAYER))
            && (allPlayers ||  being != player_node))
        {

            if ((being->getPixelX() - xtol <= x) &&
                (being->getPixelX() + xtol > x) &&
                (being->getPixelY() - uptol <= y) &&
                (being->getPixelY() > y))
            {
                beings.push_back(being);
            }
        }
    }
}

Being *ActorSpriteManager::findPortalByTile(int x, int y) const
{
    if (!mMap)
        return NULL;

    for_actors
    {
        if ((*it)->getType() != ActorSprite::PORTAL)
            continue;

        Being *being = static_cast<Being*>(*it);

        if (being->getTileX() == x && being->getTileY() == y)
            return being;
    }

    return NULL;
}

FloorItem *ActorSpriteManager::findItem(int id) const
{
    for_actors
    {
        if ((*it)->getId() == id &&
            (*it)->getType() == ActorSprite::FLOOR_ITEM)
        {
            return static_cast<FloorItem*>(*it);
        }
    }

    return NULL;
}

FloorItem *ActorSpriteManager::findItem(int x, int y) const
{
    for_actors
    {
        if ((*it)->getTileX() == x && (*it)->getTileY() == y &&
            (*it)->getType() == ActorSprite::FLOOR_ITEM)
        {
            return static_cast<FloorItem*>(*it);
        }
    }

    return NULL;
}

bool ActorSpriteManager::pickUpAll(int x1, int y1, int x2, int y2,
                                   bool serverBuggy)
{
    if (!player_node)
        return false;

    bool finded(false);
    if (!serverBuggy)
    {
        for_actors
        {
            if ((*it)->getType() == ActorSprite::FLOOR_ITEM
                && ((*it)->getTileX() >= x1 && (*it)->getTileX() <= x2)
                && ((*it)->getTileY() >= y1 && (*it)->getTileY() <= y2))
            {
                if (player_node->pickUp(static_cast<FloorItem*>(*it)))
                    finded = true;
            }
        }
    }
    else if (Client::checkPackets(PACKET_PICKUP))
    {
        FloorItem *item = 0;
        unsigned cnt = 65535;
        for_actors
        {
            if ((*it)->getType() == ActorSprite::FLOOR_ITEM
                && ((*it)->getTileX() >= x1 && (*it)->getTileX() <= x2)
                && ((*it)->getTileY() >= y1 && (*it)->getTileY() <= y2))
            {
                FloorItem *tempItem = static_cast<FloorItem*>(*it);
                if (tempItem->getPickupCount() < cnt)
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
    FloorItem *closestItem = NULL;
    int dist = 0;

    for_actors
    {
        if ((*it)->getType() == ActorSprite::FLOOR_ITEM)
        {
            FloorItem *item = static_cast<FloorItem*>(*it);

            int d = (item->getTileX() - x) * (item->getTileX() - x)
                    + (item->getTileY() - y) * (item->getTileY() - y);

            if ((d < dist || !closestItem) && (!mTargetOnlyReachable
                || player_node->isReachable(item->getTileX(),
                item->getTileY())))
            {
                dist = d;
                closestItem = item;
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
    return NULL;
}

Being *ActorSpriteManager::findNearestByName(const std::string &name,
                                             Being::Type type) const
{
    if (!player_node)
        return 0;

    int dist = 0;
    Being* closestBeing = NULL;
    int x, y;

    x = player_node->getTileX();
    y = player_node->getTileY();

    for_actors
    {
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

                if (validateBeing(0, being, type, 0, 50)
                    && (d < dist || closestBeing == NULL))
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
        (*it)->logic();

    if (mDeleteActors.empty())
        return;

    for (it = mDeleteActors.begin(), it_end = mDeleteActors.end();
         it != it_end; ++it)
    {
        if ((*it) && (*it)->getType() == Being::PLAYER)
            static_cast<Being*>(*it)->addToCache();
        if (player_node)
        {
            if (player_node->getTarget() == *it)
                player_node->setTarget(0);
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
    if (player_node)
    {
        player_node->setTarget(0);
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

    return findNearestLivingBeing(NULL, maxDist, type, x, y, excluded);
}

Being *ActorSpriteManager::findNearestLivingBeing(Being *aroundBeing,
                                                  int maxDist,
                                                  Being::Type type) const
{
    if (!aroundBeing)
        return 0;

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
        return 0;

    Being *closestBeing = 0;
    std::set<std::string> attackMobs;
    std::set<std::string> priorityMobs;
    std::set<std::string> ignoreAttackMobs;
    std::map<std::string, int> attackMobsMap;
    std::map<std::string, int> priorityMobsMap;
    int defaultAttackIndex = 10000;
    int defaultPriorityIndex = 10000;

    maxDist = maxDist * maxDist;

    bool cycleSelect = (mCyclePlayers && type == Being::PLAYER)
        || (mCycleMonsters && type == Being::MONSTER);

    bool filtered = config.getBoolValue("enableAttackFilter")
        && type != Being::PLAYER;

    bool ignoreDefault = false;
    if (filtered)
    {
        attackMobs = player_node->getAttackMobsSet();
        priorityMobs = player_node->getPriorityAttackMobsSet();
        ignoreAttackMobs = player_node->getIgnoreAttackMobsSet();
        attackMobsMap = player_node->getAttackMobsMap();
        priorityMobsMap = player_node->getPriorityAttackMobsMap();
        beingSorter.attackBeings = &attackMobsMap;
        beingSorter.priorityBeings = &priorityMobsMap;
        if (ignoreAttackMobs.find("") != ignoreAttackMobs.end())
            ignoreDefault = true;
        std::map<std::string, int>::iterator itr = attackMobsMap.find("");
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
            if (validateBeing(aroundBeing, being, type, 0, maxDist))
            {
                if (being != excluded)
                    sortedBeings.push_back(being);
            }
        }

        // no selectable beings
        if (sortedBeings.empty())
            return 0;

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
            beingSorter.attackBeings = 0;
            beingSorter.priorityBeings = 0;
        }
        sort(sortedBeings.begin(), sortedBeings.end(), beingSorter);
        if (filtered)
        {
            beingSorter.attackBeings = 0;
            beingSorter.priorityBeings = 0;
        }

        if (player_node->getTarget() == NULL)
        {
            // if no selected being, return first nearest being
            return sortedBeings.at(0);
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

//            logger->log("being name:" + being->getName());
//            logger->log("index:" + toString(index));
//            logger->log("d:" + toString(d));

            if (valid && !filtered && (d <= dist || closestBeing == 0))
            {
                dist = d;
                closestBeing = being;
            }
            else if (valid && filtered)
            {
                int w2 = defaultPriorityIndex;
                if (closestBeing)
                {
                    std::map<std::string, int>::iterator it2
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
                    std::map<std::string, int>::iterator it1
                        = priorityMobsMap.find(being->getName());
                    if (it1 != priorityMobsMap.end())
                        index = (*it1).second;
                    else
                        index = defaultPriorityIndex;
                }
            }
        }
        return (maxDist >= dist) ? closestBeing : 0;
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

void ActorSpriteManager::healTarget(LocalPlayer* player_node)
{
    if (!player_node)
        return;

    heal(player_node, player_node->getTarget());
}

void ActorSpriteManager::heal(LocalPlayer* player_node, Being* target)
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
    while (iter != blockedBeings.end())
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
    std::set<ActorSprite*>::iterator it;
    for (it = beings.begin(); it != beings.end(); ++it)
    {
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

    std::vector<Being*>::iterator i;
    for (i = beings.begin(); i != beings.end(); ++i)
    {
        const Being *being = *i;

        debugChatTab->chatLog(being->getName()
            + " (" + toString(being->getTileX()) + ","
            + toString(being->getTileY()) + ") "
            + toString(being->getSubType()), BY_SERVER);
    }
    debugChatTab->chatLog("---------------------------------------");
}

void ActorSpriteManager::getPlayerNames(std::vector<std::string> &names,
                                        bool npcNames)
{
    names.clear();

    for_actors
    {
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

void ActorSpriteManager::getMobNames(std::vector<std::string> &names)
{
    names.clear();

    for_actors
    {
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
        if ((*it)->getType() == ActorSprite::FLOOR_ITEM
            || (*it)->getType() == ActorSprite::PORTAL)
        {
            continue;
        }

        Being *being = static_cast<Being*>(*it);
        if (being->getType() == ActorSprite::PLAYER && being->getName() != "")
            being->updateName();
    }
}

void ActorSpriteManager::updatePlayerColors()
{
    for_actors
    {
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
    unsigned long pos = 0;
    const std::string brkEnd = "), ";

    pos = levels.find(brkEnd, f);
    while (pos != std::string::npos)
    {
        std::string part = levels.substr(f, pos - f);
        if (part.empty())
            break;
        unsigned long bktPos = part.rfind("(");
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
}