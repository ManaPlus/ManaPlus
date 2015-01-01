/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "actormanager.h"

#include "configuration.h"
#include "settings.h"
#include "main.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/viewport.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/questswindow.h"

#include "input/inputmanager.h"

#include "utils/checkutils.h"
#include "utils/gettext.h"

#include "net/beinghandler.h"
#include "net/charserverhandler.h"
#include "net/packetlimiter.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include <algorithm>
#include <list>

#include "debug.h"

#define for_actors for (ActorSpritesConstIterator it = mActors.begin(), \
    it_end = mActors.end() ; it != it_end; ++it)

#define for_actorsm for (ActorSpritesIterator it = mActors.begin(), \
    it_end = mActors.end() ; it != it_end; ++it)

ActorManager *actorManager = nullptr;

class FindBeingFunctor final
{
    public:
        bool operator() (const ActorSprite *const actor) const
        {
            if (!actor || actor->getType() == ActorType::FloorItem
                || actor->getType() == ActorType::Portal)
            {
                return false;
            }
            const Being *const b = static_cast<const Being* const>(actor);

            const unsigned other_y = y + ((b->getType()
                == ActorType::Npc) ? 1 : 0);
            const Vector &pos = b->getPosition();
            return (static_cast<unsigned>(pos.x) / mapTileSize == x &&
                (static_cast<unsigned>(pos.y) / mapTileSize == y
                || static_cast<unsigned>(pos.y) / mapTileSize == other_y) &&
                b->isAlive() && (type == ActorType::Unknown
                || b->getType() == type));
        }

        uint16_t x, y;
        ActorType::Type type;
} beingActorFinder;

class FindBeingEqualFunctor final
{
    public:
        bool operator() (const Being *const being) const
        {
            if (!being || !findBeing)
                return false;
            return being->getId() == findBeing->getId();
        }

        Being *findBeing;
} beingEqualActorFinder;

class SortBeingFunctor final
{
    public:
        bool operator() (const Being *const being1,
                         const Being *const being2) const
        {
            if (!being1 || !being2)
                return false;

            if (priorityBeings)
            {
                int w1 = defaultPriorityIndex;
                int w2 = defaultPriorityIndex;
                const std::map<std::string, int>::const_iterator it1
                    = priorityBeings->find(being1->getName());
                const std::map<std::string, int>::const_iterator it2
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

            const int d1 = abs(being1->getTileX() - x)
                + abs(being1->getTileY() - y);
            const int d2 = abs(being2->getTileX() - x)
                + abs(being2->getTileY() - y);

            if (d1 != d2)
                return d1 < d2;
            if (attackBeings)
            {
                int w1 = defaultAttackIndex;
                int w2 = defaultAttackIndex;
                const std::map<std::string, int>::const_iterator it1
                    = attackBeings->find(being1->getName());
                const std::map<std::string, int>::const_iterator it2
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
        std::map<std::string, int> *attackBeings;
        std::map<std::string, int> *priorityBeings;
        int x;
        int y;
        int defaultAttackIndex;
        int defaultPriorityIndex;
        int attackRange;
        bool specialDistance;
} beingActorSorter;

ActorManager::ActorManager() :
    mActors(),
    mDeleteActors(),
    mBlockedBeings(),
    mMap(nullptr),
    mSpellHeal1(serverConfig.getValue("spellHeal1", "#lum")),
    mSpellHeal2(serverConfig.getValue("spellHeal2", "#inma")),
    mSpellItenplz(serverConfig.getValue("spellItenplz", "#itenplz")),
    mTargetDeadPlayers(config.getBoolValue("targetDeadPlayers")),
    mTargetOnlyReachable(config.getBoolValue("targetOnlyReachable")),
    mCyclePlayers(config.getBoolValue("cyclePlayers")),
    mCycleMonsters(config.getBoolValue("cycleMonsters")),
    mCycleNPC(config.getBoolValue("cycleNPC")),
    mExtMouseTargeting(config.getBoolValue("extMouseTargeting"))
{
    config.addListener("targetDeadPlayers", this);
    config.addListener("targetOnlyReachable", this);
    config.addListener("cyclePlayers", this);
    config.addListener("cycleMonsters", this);
    config.addListener("cycleNPC", this);
    config.addListener("extMouseTargeting", this);

    loadAttackList();
}

ActorManager::~ActorManager()
{
    config.removeListeners(this);
    CHECKLISTENERS
    storeAttackList();
    clear();
}

void ActorManager::setMap(Map *const map)
{
    mMap = map;

    if (localPlayer)
        localPlayer->setMap(map);
}

void ActorManager::setPlayer(LocalPlayer *const player)
{
    localPlayer = player;
    mActors.insert(player);
    if (socialWindow)
        socialWindow->updateAttackFilter();
    if (socialWindow)
        socialWindow->updatePickupFilter();
}

Being *ActorManager::createBeing(const int id,
                                 const ActorType::Type type,
                                 const uint16_t subtype)
{
    Being *const being = new Being(id, type, subtype, mMap);

    mActors.insert(being);
    if (type == ActorType::Player
        || type == ActorType::Npc
        || type == ActorType::Mercenary
        || type == ActorType::Pet)
    {
        being->updateFromCache();
        beingHandler->requestNameById(id);
        if (localPlayer)
            localPlayer->checkNewName(being);
    }
    else if (type == ActorType::Monster)
    {
        if (serverFeatures->haveMonsterName())
            beingHandler->requestNameById(id);
    }
    else if (type == ActorType::Portal)
    {
        beingHandler->requestNameById(id);
    }
    if (type == ActorType::Player)
    {
        if (botCheckerWindow)
            botCheckerWindow->updateList();
        if (socialWindow)
            socialWindow->updateActiveList();
    }
    else if (type == ActorType::Npc)
    {
        if (questsWindow)
            questsWindow->addEffect(being);
    }
    return being;
}

FloorItem *ActorManager::createItem(const int id, const int itemId,
                                    const int x, const int y,
                                    const int amount,
                                    const unsigned char color,
                                    const int subX, const int subY)
{
    FloorItem *const floorItem = new FloorItem(id, itemId,
        x, y, amount, color);
    floorItem->postInit(mMap, subX, subY);

    if (!checkForPickup(floorItem))
        floorItem->disableHightlight();
    mActors.insert(floorItem);
    return floorItem;
}

void ActorManager::destroy(ActorSprite *const actor)
{
    if (!actor || actor == localPlayer)
        return;

    mDeleteActors.insert(actor);
}

void ActorManager::erase(ActorSprite *const actor)
{
    if (!actor || actor == localPlayer)
        return;

    mActors.erase(actor);
}

void ActorManager::undelete(const ActorSprite *const actor)
{
    if (!actor || actor == localPlayer)
        return;

    FOR_EACH (ActorSpritesConstIterator, it, mDeleteActors)
    {
        if (*it == actor)
        {
            mDeleteActors.erase(*it);
            return;
        }
    }
}

Being *ActorManager::findBeing(const int id) const
{
    for_actors
    {
        ActorSprite *const actor = *it;
        if (actor->getId() == id &&
            actor->getType() != ActorType::FloorItem)
        {
            return static_cast<Being*>(actor);
        }
    }

    return nullptr;
}

Being *ActorManager::findBeing(const int x, const int y,
                               const ActorType::Type type) const
{
    beingActorFinder.x = static_cast<uint16_t>(x);
    beingActorFinder.y = static_cast<uint16_t>(y);
    beingActorFinder.type = type;

    const ActorSpritesConstIterator it = std::find_if(
        mActors.begin(), mActors.end(), beingActorFinder);

    return (it == mActors.end()) ? nullptr : static_cast<Being*>(*it);
}

Being *ActorManager::findBeingByPixel(const int x, const int y,
                                      const bool allPlayers) const
{
    if (!mMap)
        return nullptr;

    const bool targetDead = mTargetDeadPlayers;
    const bool modActive = inputManager.isActionActive(
        InputAction::STOP_ATTACK);

    if (mExtMouseTargeting)
    {
        Being *tempBeing = nullptr;
        bool noBeing(false);

        for_actorsm
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorType::Portal)
                continue;

            if ((*it)->getType() == ActorType::FloorItem)
            {
                if (!noBeing)
                {
                    const FloorItem *const floor
                        = static_cast<const FloorItem*>(*it);
                    const int px = floor->getPixelX();
                    const int py = floor->getPixelY();
                    if ((px - mapTileSize     <= x) &&
                        (px + mapTileSize     >  x) &&
                        (py - mapTileSize * 2 <= y) &&
                        (py + mapTileSize / 2 >  y))
                    {
                        noBeing = true;
                    }
                }
                continue;
            }

            Being *const being = static_cast<Being*>(*it);

            if (being->getInfo() && !(being->getInfo()->isTargetSelection()
                || modActive))
            {
                continue;
            }

            if ((being->mAction != BeingAction::DEAD
                || (targetDead && being->getType() == ActorType::Player))
                && (allPlayers || being != localPlayer))
            {
                const int px = being->getPixelX();
                const int py = being->getPixelY();
                if ((px - mapTileSize / 2 <= x) &&
                    (px + mapTileSize / 2 >  x) &&
                    (py - mapTileSize     <= y) &&
                    (py                   >  y))
                {
                    return being;
                }
                else if (!noBeing &&
                         (px - mapTileSize     <= x) &&
                         (px + mapTileSize     >  x) &&
                         (py - mapTileSize * 2 <= y) &&
                         (py + mapTileSize / 2 >  y))
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
        for_actorsm
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorType::Portal ||
                (*it)->getType() == ActorType::FloorItem)
            {
                continue;
            }

            Being *const being = static_cast<Being*>(*it);

            if (being->getInfo() && !(being->getInfo()->isTargetSelection()
                || modActive))
            {
                continue;
            }

            const int px = being->getPixelX();
            const int py = being->getPixelY();
            if ((px - mapTileSize / 2 <= x) &&
                (px + mapTileSize / 2 >  x) &&
                (py - mapTileSize     <= y) &&
                (py                   >  y))
            {
                return being;
            }
        }
        return nullptr;
    }
}

void ActorManager::findBeingsByPixel(std::vector<ActorSprite*> &beings,
                                     const int x, const int y,
                                     const bool allPlayers) const
{
    if (!mMap)
        return;

    const int xtol = mapTileSize / 2;
    const int uptol = mapTileSize;
    const bool modActive = inputManager.isActionActive(
        InputAction::STOP_ATTACK);

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::Portal)
            continue;

        const Being *const being = dynamic_cast<const Being*>(*it);

        if (being && being->getInfo()
            && !(being->getInfo()->isTargetSelection() || modActive))
        {
            continue;
        }

        ActorSprite *const actor = *it;

        if ((being && (being->isAlive()
            || (mTargetDeadPlayers && being->getType() == ActorType::Player))
            && (allPlayers ||  being != localPlayer))
            || actor->getType() == ActorType::FloorItem)
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

Being *ActorManager::findPortalByTile(const int x, const int y) const
{
    if (!mMap)
        return nullptr;

    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getType() != ActorType::Portal)
            continue;

        Being *const being = static_cast<Being*>(*it);

        if (being->getTileX() == x && being->getTileY() == y)
            return being;
    }

    return nullptr;
}

FloorItem *ActorManager::findItem(const int id) const
{
    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getId() == id &&
            (*it)->getType() == ActorType::FloorItem)
        {
            return static_cast<FloorItem*>(*it);
        }
    }

    return nullptr;
}

FloorItem *ActorManager::findItem(const int x, const int y) const
{
    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getTileX() == x && (*it)->getTileY() == y &&
            (*it)->getType() == ActorType::FloorItem)
        {
            return static_cast<FloorItem*>(*it);
        }
    }

    return nullptr;
}

bool ActorManager::pickUpAll(const int x1, const int y1,
                             const int x2, const int y2,
                             const bool serverBuggy)
{
    if (!localPlayer)
        return false;

    bool finded(false);
    const bool allowAll = mPickupItemsSet.find("") != mPickupItemsSet.end();
    if (!serverBuggy)
    {
        for_actorsm
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorType::FloorItem
                && ((*it)->getTileX() >= x1 && (*it)->getTileX() <= x2)
                && ((*it)->getTileY() >= y1 && (*it)->getTileY() <= y2))
            {
                FloorItem *const item = static_cast<FloorItem*>(*it);
                if (allowAll)
                {
                    if (mIgnorePickupItemsSet.find(item->getName())
                        == mIgnorePickupItemsSet.end())
                    {
                        if (localPlayer->pickUp(item))
                            finded = true;
                    }
                }
                else
                {
                    if (mPickupItemsSet.find(item->getName())
                        != mPickupItemsSet.end())
                    {
                        if (localPlayer->pickUp(item))
                            finded = true;
                    }
                }
            }
        }
    }
    else if (PacketLimiter::checkPackets(PACKET_PICKUP))
    {
        FloorItem *item = nullptr;
        unsigned cnt = 65535;
        for_actorsm
        {
            if (!*it)
                continue;

            if ((*it)->getType() == ActorType::FloorItem
                && ((*it)->getTileX() >= x1 && (*it)->getTileX() <= x2)
                && ((*it)->getTileY() >= y1 && (*it)->getTileY() <= y2))
            {
                FloorItem *const tempItem = static_cast<FloorItem*>(*it);
                if (tempItem->getPickupCount() < cnt)
                {
                    if (allowAll)
                    {
                        if (mIgnorePickupItemsSet.find(tempItem->getName())
                            == mIgnorePickupItemsSet.end())
                        {
                            item = tempItem;
                            cnt = item->getPickupCount();
                            if (cnt == 0)
                            {
                                item->incrementPickup();
                                localPlayer->pickUp(item);
                                return true;
                            }
                        }
                    }
                    else
                    {
                        if (mPickupItemsSet.find(tempItem->getName())
                            != mPickupItemsSet.end())
                        {
                            item = tempItem;
                            cnt = item->getPickupCount();
                            if (cnt == 0)
                            {
                                item->incrementPickup();
                                localPlayer->pickUp(item);
                                return true;
                            }
                        }
                    }
                }
            }
        }
        if (item && localPlayer->pickUp(item))
            finded = true;
    }
    return finded;
}

bool ActorManager::pickUpNearest(const int x, const int y,
                                 int maxdist) const
{
    if (!localPlayer)
        return false;

    maxdist = maxdist * maxdist;
    FloorItem *closestItem = nullptr;
    int dist = 0;
    const bool allowAll = mPickupItemsSet.find("") != mPickupItemsSet.end();

    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem)
        {
            FloorItem *const item = static_cast<FloorItem*>(*it);

            const int d = (item->getTileX() - x) * (item->getTileX() - x)
                + (item->getTileY() - y) * (item->getTileY() - y);

            if ((d < dist || !closestItem) && (!mTargetOnlyReachable
                || localPlayer->isReachable(item->getTileX(),
                item->getTileY(), false)))
            {
                if (allowAll)
                {
                    if (mIgnorePickupItemsSet.find(item->getName())
                        == mIgnorePickupItemsSet.end())
                    {
                        dist = d;
                        closestItem = item;
                    }
                }
                else
                {
                    if (mPickupItemsSet.find(item->getName())
                        != mPickupItemsSet.end())
                    {
                        dist = d;
                        closestItem = item;
                    }
                }
            }
        }
    }
    if (closestItem && dist <= maxdist)
        return localPlayer->pickUp(closestItem);

    return false;
}

Being *ActorManager::findBeingByName(const std::string &name,
                                     const ActorType::Type type) const
{
    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        if (being->getName() == name &&
            (type == ActorType::Unknown || type == being->getType()))
        {
            return being;
        }
    }
    return nullptr;
}

Being *ActorManager::findNearestByName(const std::string &name,
                                       const ActorType::Type &type) const
{
    if (!localPlayer)
        return nullptr;

    int dist = 0;
    Being* closestBeing = nullptr;
    int x, y;

    x = localPlayer->getTileX();
    y = localPlayer->getTileY();

    for_actorsm
    {
        if (reportTrue(!*it))
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);

        if (being && being->getName() == name &&
            (type == ActorType::Unknown || type == being->getType()))
        {
            if (being->getType() == ActorType::Player)
            {
                return being;
            }
            else
            {
                const int d = (being->getTileX() - x) * (being->getTileX() - x)
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

const ActorSprites &ActorManager::getAll() const
{
    return mActors;
}

void ActorManager::logic()
{
    BLOCK_START("ActorManager::logic")
    for_actors
    {
        if (*it)
            (*it)->logic();
    }

    if (mDeleteActors.empty())
    {
        BLOCK_END("ActorManager::logic")
        return;
    }

    BLOCK_START("ActorManager::logic 1")
    FOR_EACH (ActorSpritesConstIterator, it, mDeleteActors)
    {
        const ActorSprite *const actor = *it;
        if (!actor)
            continue;

        const ActorType::Type &type = actor->getType();
        if (type == ActorType::Player)
        {
            const Being *const being = static_cast<const Being*>(actor);
            being->addToCache();
            if (beingEquipmentWindow)
                beingEquipmentWindow->resetBeing(being);
        }
        if (localPlayer)
        {
            if (localPlayer->getTarget() == actor)
                localPlayer->setTarget(nullptr);
            if (localPlayer->getPickUpTarget() == actor)
                localPlayer->unSetPickUpTarget();
        }
        if (viewport)
            viewport->clearHover(*it);
    }

    FOR_EACH (ActorSpritesConstIterator, it, mDeleteActors)
    {
        ActorSprite *actor = *it;
        mActors.erase(actor);
        delete actor;
    }

    mDeleteActors.clear();
    BLOCK_END("ActorManager::logic 1")
    BLOCK_END("ActorManager::logic")
}

void ActorManager::clear()
{
    if (beingEquipmentWindow)
        beingEquipmentWindow->setBeing(nullptr);

    if (localPlayer)
    {
        localPlayer->setTarget(nullptr);
        localPlayer->unSetPickUpTarget();
        mActors.erase(localPlayer);
    }

    for_actors
        delete *it;
    mActors.clear();
    mDeleteActors.clear();

    if (localPlayer)
        mActors.insert(localPlayer);
}

Being *ActorManager::findNearestLivingBeing(const int x, const int y,
                                            const int maxTileDist,
                                            const ActorType::Type type,
                                            const Being *const excluded) const
{
    const int maxDist = maxTileDist * mapTileSize;

    return findNearestLivingBeing(nullptr, maxDist,
        type,
        x, y,
        excluded,
        true);
}

Being *ActorManager::findNearestLivingBeing(const Being *const aroundBeing,
                                            const int maxDist,
                                            const ActorType::Type type,
                                            const bool allowSort) const
{
    if (!aroundBeing)
        return nullptr;

    return findNearestLivingBeing(aroundBeing,
        maxDist,
        type,
        aroundBeing->getTileX(),
        aroundBeing->getTileY(),
        aroundBeing,
        allowSort);
}

Being *ActorManager::findNearestLivingBeing(const Being *const aroundBeing,
                                            int maxDist,
                                            const ActorType::Type &type,
                                            const int x, const int y,
                                            const Being *const excluded,
                                            const bool allowSort) const
{
    if (!aroundBeing || !localPlayer)
        return nullptr;

    std::set<std::string> attackMobs;
    std::set<std::string> priorityMobs;
    std::set<std::string> ignoreAttackMobs;
    std::map<std::string, int> attackMobsMap;
    std::map<std::string, int> priorityMobsMap;
    int defaultAttackIndex = 10000;
    int defaultPriorityIndex = 10000;
    const int attackRange = localPlayer->getAttackRange();

    bool specialDistance = false;
    if (settings.moveToTargetType == 7
        && localPlayer->getAttackRange() > 2)
    {
        specialDistance = true;
    }

    maxDist = maxDist * maxDist;

    const bool cycleSelect = allowSort
        && ((mCyclePlayers && type == ActorType::Player)
        || (mCycleMonsters && type == ActorType::Monster)
        || (mCycleNPC && type == ActorType::Npc));

    const bool filtered = allowSort
        && config.getBoolValue("enableAttackFilter")
        && type == ActorType::Monster;
    const bool modActive = inputManager.isActionActive(
        InputAction::STOP_ATTACK);

    bool ignoreDefault = false;
    if (filtered)
    {
        attackMobs = mAttackMobsSet;
        priorityMobs = mPriorityAttackMobsSet;
        ignoreAttackMobs = mIgnoreAttackMobsSet;
        attackMobsMap = mAttackMobsMap;
        priorityMobsMap = mPriorityAttackMobsMap;
        beingActorSorter.attackBeings = &attackMobsMap;
        beingActorSorter.priorityBeings = &priorityMobsMap;
        beingActorSorter.specialDistance = specialDistance;
        beingActorSorter.attackRange = attackRange;
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

        FOR_EACH (ActorSprites::iterator, i, mActors)
        {
            if (!*i)
                continue;

            if ((*i)->getType() == ActorType::FloorItem
                || (*i)->getType() == ActorType::Portal)
            {
                continue;
            }

            Being *const being = static_cast<Being*>(*i);

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

            if (being->getInfo()
                && !(being->getInfo()->isTargetSelection() || modActive))
            {
                continue;
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

        beingActorSorter.x = x;
        beingActorSorter.y = y;
        if (filtered)
        {
            beingActorSorter.attackBeings = &attackMobsMap;
            beingActorSorter.defaultAttackIndex = defaultAttackIndex;
            beingActorSorter.priorityBeings = &priorityMobsMap;
            beingActorSorter.defaultPriorityIndex = defaultPriorityIndex;
        }
        else
        {
            beingActorSorter.attackBeings = nullptr;
            beingActorSorter.priorityBeings = nullptr;
        }
        std::sort(sortedBeings.begin(), sortedBeings.end(), beingActorSorter);
        if (filtered)
        {
            beingActorSorter.attackBeings = nullptr;
            beingActorSorter.priorityBeings = nullptr;
        }

        if (localPlayer->getTarget() == nullptr)
        {
            Being *const target = sortedBeings.at(0);

            if (specialDistance && target->getType() == ActorType::Monster
                && target->getDistance() <= 2)
            {
                return nullptr;
            }
            // if no selected being in vector, return first nearest being
            return target;
        }

        beingEqualActorFinder.findBeing = localPlayer->getTarget();
        std::vector<Being*>::const_iterator i = std::find_if(
            sortedBeings.begin(), sortedBeings.end(), beingEqualActorFinder);

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
        Being *closestBeing = nullptr;

        FOR_EACH (ActorSprites::iterator, i, mActors)
        {
            if (!*i)
                continue;

            if ((*i)->getType() == ActorType::FloorItem
                || (*i)->getType() == ActorType::Portal)
            {
                continue;
            }
            Being *const being = static_cast<Being*>(*i);

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

            if (being->getInfo()
                && !(being->getInfo()->isTargetSelection() || modActive))
            {
                continue;
            }

            const bool valid = validateBeing(aroundBeing, being,
                                             type, excluded, 50);
            int d = being->getDistance();
            if (being->getType() != ActorType::Monster
                || !mTargetOnlyReachable)
            {   // if distance not calculated, use old distance
                d = (being->getTileX() - x) * (being->getTileX() - x)
                    + (being->getTileY() - y) * (being->getTileY() - y);
            }

            if (!valid)
                continue;

            if (specialDistance && being->getDistance() <= 2
                && being->getType() == type)
            {
                continue;
            }

//            logger->log("being name:" + being->getName());
//            logger->log("index:" + toString(index));
//            logger->log("d:" + toString(d));

            if (!filtered && (d <= dist || !closestBeing))
            {
                dist = d;
                closestBeing = being;
            }
            else if (filtered)
            {
                int w2 = defaultPriorityIndex;
                if (closestBeing)
                {
                    const std::map<std::string, int>::const_iterator it2
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
                    const std::map<std::string, int>::const_iterator it1
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

bool ActorManager::validateBeing(const Being *const aroundBeing,
                                 Being *const being,
                                 const ActorType::Type &type,
                                 const Being* const excluded,
                                 const int maxCost) const
{
    if (!localPlayer)
        return false;
    return being && ((being->getType() == type
        || type == ActorType::Unknown) && (being->isAlive()
        || (mTargetDeadPlayers && type == ActorType::Player))
        && being != aroundBeing) && being != excluded
        && (type != ActorType::Monster || !mTargetOnlyReachable
        || localPlayer->isReachable(being, maxCost));
}

void ActorManager::healTarget() const
{
    if (!localPlayer)
        return;

    heal(localPlayer->getTarget());
}

void ActorManager::heal(const Being *const target) const
{
    if (!localPlayer || !chatWindow || !localPlayer->isAlive()
        || !playerHandler->canUseMagic())
    {
        return;
    }

    // self
    if (target && localPlayer->getName() == target->getName())
    {
        if (PlayerInfo::getAttribute(Attributes::MP) >= 6
            && PlayerInfo::getAttribute(Attributes::HP)
            != PlayerInfo::getAttribute(Attributes::MAX_HP))
        {
            if (!PacketLimiter::limitPackets(PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal1);
        }
    }
    // magic levels < 2
    else if (PlayerInfo::getSkillLevel(340) < 2
             || PlayerInfo::getSkillLevel(341) < 2)
    {
        if (PlayerInfo::getAttribute(Attributes::MP) >= 6)
        {
            if (target && target->getType() != ActorType::Monster)
            {
                if (!PacketLimiter::limitPackets(PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1 + " "
                                           + target->getName());
            }
            else if (PlayerInfo::getAttribute(Attributes::HP)
                     != PlayerInfo::getAttribute(Attributes::MAX_HP))
            {
                if (!PacketLimiter::limitPackets(PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1);
            }
        }
    }
    // magic level >= 2 and not self
    else
    {
        // mp > 10 and target not monster
        if (PlayerInfo::getAttribute(Attributes::MP) >= 10 && target
            && target->getType() != ActorType::Monster)
        {
            // target not enemy
            if (player_relations.getRelation(target->getName()) !=
                PlayerRelation::ENEMY2)
            {
                if (!PacketLimiter::limitPackets(PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal2 + " "
                    + target->getName());
            }
            // target enemy
            else
            {
                if (!PacketLimiter::limitPackets(PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1);
            }
        }
        // heal self if selected monster or selection empty
        else if ((!target || target->getType() == ActorType::Monster)
                 && PlayerInfo::getAttribute(Attributes::MP) >= 6
                 && PlayerInfo::getAttribute(Attributes::HP)
                 != PlayerInfo::getAttribute(Attributes::MAX_HP))
        {
            if (!PacketLimiter::limitPackets(PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal1);
        }
    }
}

void ActorManager::itenplz() const
{
    if (!localPlayer || !chatWindow || !localPlayer->isAlive()
        || !playerHandler->canUseMagic())
    {
        return;
    }

    if (!PacketLimiter::limitPackets(PACKET_CHAT))
        return;

    chatWindow->localChatInput(mSpellItenplz);
}

bool ActorManager::hasActorSprite(const ActorSprite *const actor) const
{
    for_actors
    {
        if (actor == *it)
            return true;
    }

    return false;
}

void ActorManager::addBlock(const uint32_t id)
{
    mBlockedBeings.insert(id);
}

void ActorManager::deleteBlock(const uint32_t id)
{
    mBlockedBeings.erase(id);
}

bool ActorManager::isBlocked(const uint32_t id) const
{
    return mBlockedBeings.find(id) != mBlockedBeings.end();
}

void ActorManager::printAllToChat() const
{
    // TRANSLATORS: visible beings on map
    printBeingsToChat(getAll(), _("Visible on map"));
}

void ActorManager::printBeingsToChat(const ActorSprites &beings,
                                     const std::string &header)
{
    if (!debugChatTab)
        return;

    debugChatTab->chatLog("---------------------------------------");
    debugChatTab->chatLog(header);
    FOR_EACH (std::set<ActorSprite*>::const_iterator, it, beings)
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem)
            continue;

        const Being *const being = static_cast<const Being*>(*it);

        debugChatTab->chatLog(strprintf("%s (%d,%d) %d",
            being->getName().c_str(), being->getTileX(), being->getTileY(),
            being->getSubType()), ChatMsgType::BY_SERVER);
    }
    debugChatTab->chatLog("---------------------------------------");
}

void ActorManager::printBeingsToChat(const std::vector<Being*> &beings,
                                     const std::string &header)
{
    if (!debugChatTab)
        return;

    debugChatTab->chatLog("---------------------------------------");
    debugChatTab->chatLog(header);

    FOR_EACH (std::vector<Being*>::const_iterator, i, beings)
    {
        if (!*i)
            continue;

        const Being *const being = *i;

        debugChatTab->chatLog(strprintf("%s (%d,%d) %d",
            being->getName().c_str(), being->getTileX(), being->getTileY(),
            being->getSubType()), ChatMsgType::BY_SERVER);
    }
    debugChatTab->chatLog("---------------------------------------");
}

void ActorManager::getPlayerNames(StringVect &names,
                                  const bool npcNames) const
{
    names.clear();

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        const Being *const being = static_cast<const Being*>(*it);
        if ((being->getType() == ActorType::Player
            || (being->getType() == ActorType::Npc && npcNames))
            && being->getName() != "")
        {
            names.push_back(being->getName());
        }
    }
}

void ActorManager::getMobNames(StringVect &names) const
{
    names.clear();

    for_actors
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        const Being *const being = static_cast<const Being*>(*it);
        if (being->getType() == ActorType::Monster && being->getName() != "")
            names.push_back(being->getName());
    }
}

void ActorManager::updatePlayerNames() const
{
    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        being->setGoodStatus(-1);
        if (being->getType() == ActorType::Player && being->getName() != "")
            being->updateName();
    }
}

void ActorManager::updatePlayerColors() const
{
    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        if (being->getType() == ActorType::Player && being->getName() != "")
            being->updateColors();
    }
}

void ActorManager::updatePlayerGuild() const
{
    for_actorsm
    {
        if (!*it)
            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        if (being->getType() == ActorType::Player && being->getName() != "")
            being->updateGuild();
    }
}

void ActorManager::parseLevels(std::string levels) const
{
    levels.append(", ");
    size_t f = 0;
    const std::string brkEnd("), ");
    size_t pos = levels.find(brkEnd, f);

    while (pos != std::string::npos)
    {
        std::string part = levels.substr(f, pos - f);
        if (part.empty())
            break;
        const size_t bktPos = part.rfind("(");
        if (bktPos != std::string::npos)
        {
            Being *const being = findBeingByName(part.substr(0, bktPos),
                ActorType::Player);
            if (being)
            {
                being->setLevel(atoi(part.substr(bktPos + 1).c_str()));
                being->addToCache();
            }
        }
        f = static_cast<size_t>(pos + brkEnd.length());
        pos = levels.find(brkEnd, f);
    }
    updatePlayerNames();
}

void ActorManager::optionChanged(const std::string &name)
{
    if (name == "targetDeadPlayers")
        mTargetDeadPlayers = config.getBoolValue("targetDeadPlayers");
    else if (name == "targetOnlyReachable")
        mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
    else if (name == "cyclePlayers")
        mCyclePlayers = config.getBoolValue("cyclePlayers");
    else if (name == "cycleMonsters")
        mCycleMonsters = config.getBoolValue("cycleMonsters");
    else if (name == "cycleNPC")
        mCycleNPC = config.getBoolValue("cycleNPC");
    else if (name == "extMouseTargeting")
        mExtMouseTargeting = config.getBoolValue("extMouseTargeting");
}

void ActorManager::removeAttackMob(const std::string &name)
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

void ActorManager::removePickupItem(const std::string &name)
{
    mPickupItems.remove(name);
    mPickupItemsSet.erase(name);
    mIgnorePickupItems.remove(name);
    mIgnorePickupItemsSet.erase(name);
    rebuildPickupItems();
}

#define addMobToList(name, mob) \
{\
    const int size = get##mob##sSize();\
    if (size > 0)\
    {\
        const int idx = get##mob##Index("");\
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

void ActorManager::addAttackMob(const std::string &name)
{
    addMobToList(name, AttackMob);
    rebuildPriorityAttackMobs();
}

void ActorManager::addPriorityAttackMob(const std::string &name)
{
    addMobToList(name, PriorityAttackMob);
}

void ActorManager::addIgnoreAttackMob(const std::string &name)
{
    mIgnoreAttackMobs.push_back(name);
    mIgnoreAttackMobsSet.insert(name);
    rebuildAttackMobs();
    rebuildPriorityAttackMobs();
}

void ActorManager::addPickupItem(const std::string &name)
{
    addMobToList(name, PickupItem);
    rebuildPickupItems();
}

void ActorManager::addIgnorePickupItem(const std::string &name)
{
    mIgnorePickupItems.push_back(name);
    mIgnorePickupItemsSet.insert(name);
    rebuildPickupItems();
}

void ActorManager::rebuildPriorityAttackMobs()
{
    rebuildMobsList(PriorityAttackMob);
}

void ActorManager::rebuildAttackMobs()
{
    rebuildMobsList(AttackMob);
}

void ActorManager::rebuildPickupItems()
{
    rebuildMobsList(PickupItem);
}

int ActorManager::getIndexByName(const std::string &name,
                                 const std::map<std::string, int> &map)
{
    const std::map<std::string, int>::const_iterator
        i = map.find(name);
    if (i == map.end())
        return -1;

    return (*i).second;
}

int ActorManager::getPriorityAttackMobIndex(const std::string &name) const
{
    return getIndexByName(name, mPriorityAttackMobsMap);
}

int ActorManager::getAttackMobIndex(const std::string &name) const
{
    return getIndexByName(name, mAttackMobsMap);
}

int ActorManager::getPickupItemIndex(const std::string &name) const
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

void ActorManager::loadAttackList()
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
    }
    empty = false;

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

void ActorManager::storeAttackList() const
{
    serverConfig.setValue("attackPriorityMobs", packList(mPriorityAttackMobs));
    serverConfig.setValue("attackMobs", packList(mAttackMobs));
    serverConfig.setValue("ignoreAttackMobs", packList(mIgnoreAttackMobs));

    serverConfig.setValue("pickupItems", packList(mPickupItems));
    serverConfig.setValue("ignorePickupItems", packList(mIgnorePickupItems));
}

bool ActorManager::checkForPickup(const FloorItem *const item) const
{
    if (mPickupItemsSet.find("") != mPickupItemsSet.end())
    {
        if (mIgnorePickupItemsSet.find(item->getName())
            == mIgnorePickupItemsSet.end())
        {
            return true;
        }
    }
    else if (item && mPickupItemsSet.find(item->getName())
             != mPickupItemsSet.end())
    {
        return true;
    }
    return false;
}

void ActorManager::updateEffects(const std::map<int, int> &addEffects,
                                 const std::set<int> &removeEffects) const
{
    for_actorsm
    {
        if (!*it || (*it)->getType() != ActorType::Npc)
            continue;
        Being *const being = static_cast<Being*>(*it);
        const int type = being->getSubType();
        if (removeEffects.find(type) != removeEffects.end())
            being->removeSpecialEffect();
        const std::map<int, int>::const_iterator idAdd = addEffects.find(type);
        if (idAdd != addEffects.end())
            being->addSpecialEffect((*idAdd).second);
    }
}

Being *ActorManager::cloneBeing(const Being *const srcBeing,
                                const int dx, const int dy,
                                const int id)
{
    Being *const dstBeing = actorManager->createBeing(srcBeing->getId() + id,
        ActorType::Player,
        srcBeing->getSubType());
    if (!dstBeing)
        return nullptr;
    dstBeing->setGender(srcBeing->getGender());
    dstBeing->setAction(srcBeing->getCurrentAction(), 0);
    dstBeing->setTileCoords(srcBeing->getTileX() + dx,
        srcBeing->getTileY() + dy);
    dstBeing->setName(srcBeing->getName());
    dstBeing->setDirection(srcBeing->getDirection());
    const int sz = static_cast<int>(srcBeing->getSpritesCount());
    for (int slot = 0; slot < sz; slot ++)
    {
        const int spriteId = srcBeing->getSpriteID(slot);
        const unsigned char color = srcBeing->getSpriteColor(slot);
        dstBeing->setSprite(slot, spriteId, "", color, false);
    }
    const int hairSlot = charServerHandler->hairSprite();
    const int hairStyle =  -srcBeing->getSpriteID(hairSlot);
    const unsigned char hairColor = srcBeing->getHairColor();
    dstBeing->setSprite(hairSlot, hairStyle * -1,
        ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
    dstBeing->setHairColor(hairColor);
    return dstBeing;
}
