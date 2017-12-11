/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "actormanager.h"

#include "game.h"
#include "configuration.h"
#include "settings.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "gui/windows/equipmentwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/questswindow.h"

#include "fs/files.h"

#include "input/inputmanager.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/mathutils.h"
#include "utils/gettext.h"

#include "net/beinghandler.h"
#include "net/charserverhandler.h"
#include "net/packetlimiter.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "resources/chatobject.h"
#include "resources/iteminfo.h"

#include "resources/map/map.h"

#include "resources/db/itemdb.h"

#ifdef TMWA_SUPPORT
#include "being/playerinfo.h"

#include "gui/windows/chatwindow.h"

#include "net/net.h"
#endif  // TMWA_SUPPORT

#include <algorithm>

#include "debug.h"

#define for_actors for (ActorSpritesConstIterator it = mActors.begin(), \
    it_fend = mActors.end(); it != it_fend; ++it)

#define for_actorsm for (ActorSpritesIterator it = mActors.begin(), \
    it_fend = mActors.end(); it != it_fend; ++it)

ActorManager *actorManager = nullptr;

class FindBeingFunctor final
{
    public:
        A_DEFAULT_COPY(FindBeingFunctor)

        bool operator() (const ActorSprite *const actor) const
        {
            if ((actor == nullptr)
                || actor->getType() == ActorType::FloorItem
                || actor->getType() == ActorType::Portal)
            {
                return false;
            }
            const Being *const b = static_cast<const Being *>(actor);

            const unsigned other_y = y
                + ((b->getType() == ActorType::Npc) ? 1 : 0);
            const Vector &pos = b->getPixelPositionF();
            // +++ probably here need use int positions and not float?
            // but for now correct int positions only in Being
            return CAST_U32(pos.x) / mapTileSize == x &&
                (CAST_U32(pos.y) / mapTileSize == y
                || CAST_U32(pos.y) / mapTileSize == other_y) &&
                b->isAlive() && (type == ActorType::Unknown
                || b->getType() == type);
        }

        uint16_t x, y;
        ActorTypeT type;
} beingActorFinder;

class FindBeingEqualFunctor final
{
    public:
        A_DEFAULT_COPY(FindBeingEqualFunctor)

        bool operator() (const Being *const being) const
        {
            if ((being == nullptr) || (findBeing == nullptr))
                return false;
            return being->getId() == findBeing->getId();
        }

        Being *findBeing;
} beingEqualActorFinder;

class SortBeingFunctor final
{
    public:
        A_DEFAULT_COPY(SortBeingFunctor)

        bool operator() (const Being *const being1,
                         const Being *const being2) const
        {
            if ((being1 == nullptr) || (being2 == nullptr))
                return false;

            if (priorityBeings != nullptr)
            {
                int w1 = defaultPriorityIndex;
                int w2 = defaultPriorityIndex;
                const StringIntMapCIter it1 = priorityBeings->find(
                    being1->getName());
                const StringIntMapCIter it2 = priorityBeings->find(
                    being2->getName());
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
            if (attackBeings != nullptr)
            {
                int w1 = defaultAttackIndex;
                int w2 = defaultAttackIndex;
                const StringIntMapCIter it1 = attackBeings->find(
                    being1->getName());
                const StringIntMapCIter it2 = attackBeings->find(
                    being2->getName());
                if (it1 != attackBeings->end())
                    w1 = (*it1).second;
                if (it2 != attackBeings->end())
                    w2 = (*it2).second;

                if (w1 != w2)
                    return w1 < w2;
            }

            return being1->getName() < being2->getName();
        }
        StringIntMap *attackBeings;
        StringIntMap *priorityBeings;
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
    mActorsIdMap(),
    mIdName(),
    mBlockedBeings(),
    mChars(),
    mMap(nullptr),
#ifdef TMWA_SUPPORT
    mSpellHeal1(serverConfig.getValue("spellHeal1", "#lum")),
    mSpellHeal2(serverConfig.getValue("spellHeal2", "#inma")),
    mSpellItenplz(serverConfig.getValue("spellItenplz", "#itenplz")),
#endif  // TMWA_SUPPORT
    mTargetDeadPlayers(config.getBoolValue("targetDeadPlayers")),
    mTargetOnlyReachable(config.getBoolValue("targetOnlyReachable")),
    mCyclePlayers(config.getBoolValue("cyclePlayers")),
    mCycleMonsters(config.getBoolValue("cycleMonsters")),
    mCycleNPC(config.getBoolValue("cycleNPC")),
    mExtMouseTargeting(config.getBoolValue("extMouseTargeting")),
    mEnableIdCollecting(config.getBoolValue("enableIdCollecting")),
    mPriorityAttackMobs(),
    mPriorityAttackMobsSet(),
    mPriorityAttackMobsMap(),
    mAttackMobs(),
    mAttackMobsSet(),
    mAttackMobsMap(),
    mIgnoreAttackMobs(),
    mIgnoreAttackMobsSet(),
    mPickupItems(),
    mPickupItemsSet(),
    mPickupItemsMap(),
    mIgnorePickupItems(),
    mIgnorePickupItemsSet()
{
    config.addListener("targetDeadPlayers", this);
    config.addListener("targetOnlyReachable", this);
    config.addListener("cyclePlayers", this);
    config.addListener("cycleMonsters", this);
    config.addListener("cycleNPC", this);
    config.addListener("extMouseTargeting", this);
    config.addListener("showBadges", this);
    config.addListener("enableIdCollecting", this);
    config.addListener("visiblenamespos", this);

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

    if (localPlayer != nullptr)
        localPlayer->setMap(map);
}

void ActorManager::setPlayer(LocalPlayer *const player)
{
    localPlayer = player;
    mActors.insert(player);
    mActorsIdMap[player->getId()] = player;
    if (socialWindow != nullptr)
        socialWindow->updateAttackFilter();
    if (socialWindow != nullptr)
        socialWindow->updatePickupFilter();
}

Being *ActorManager::createBeing(const BeingId id,
                                 const ActorTypeT type,
                                 const BeingTypeId subtype)
{
    Being *const being = Being::createBeing(id,
        type,
        subtype,
        mMap);

    mActors.insert(being);

    mActorsIdMap[being->getId()] = being;

    switch (type)
    {
        case ActorType::Player:
        case ActorType::Mercenary:
        case ActorType::Pet:
        case ActorType::Homunculus:
        case ActorType::Npc:
            being->updateFromCache();
            if (beingHandler != nullptr)
                beingHandler->requestNameById(id);
            if (localPlayer != nullptr)
                localPlayer->checkNewName(being);
            break;
        case ActorType::Monster:
#ifdef TMWA_SUPPORT
            if (Net::getNetworkType() != ServerType::TMWATHENA)
#endif  // TMWA_SUPPORT
            {
                beingHandler->requestNameById(id);
            }
            break;
        case ActorType::Portal:
            if ((beingHandler != nullptr) &&
                (serverFeatures != nullptr) &&
                serverFeatures->haveServerWarpNames())
            {
                beingHandler->requestNameById(id);
            }
            break;
        case ActorType::Elemental:
            if (beingHandler != nullptr)
                beingHandler->requestNameById(id);
            break;
        case ActorType::SkillUnit:
            break;
        default:
        case ActorType::FloorItem:
        case ActorType::Avatar:
        case ActorType::Unknown:
            reportAlways("CreateBeing for unknown type %d", CAST_S32(type));
            break;
    }

    if (type == ActorType::Player)
    {
        if (socialWindow != nullptr)
            socialWindow->updateActiveList();
    }
    else if (type == ActorType::Npc)
    {
        if (questsWindow != nullptr)
            questsWindow->addEffect(being);
    }
    return being;
}

FloorItem *ActorManager::createItem(const BeingId id,
                                    const int itemId,
                                    const int x, const int y,
                                    const ItemTypeT itemType,
                                    const int amount,
                                    const int refine,
                                    const ItemColor color,
                                    const Identified identified,
                                    const Damaged damaged,
                                    const int subX, const int subY,
                                    const int *const cards)
{
    FloorItem *const floorItem = new FloorItem(id,
        itemId,
        x, y,
        itemType,
        amount,
        refine,
        color,
        identified,
        damaged,
        cards);
    floorItem->postInit(mMap, subX, subY);

    if (!checkForPickup(floorItem))
        floorItem->disableHightlight();
    mActors.insert(floorItem);
    mActorsIdMap[floorItem->getId()] = floorItem;
    return floorItem;
}

void ActorManager::destroy(ActorSprite *const actor)
{
    returnNullptrV(actor);

    if (actor == localPlayer)
        return;

    mDeleteActors.insert(actor);
}

void ActorManager::erase(ActorSprite *const actor)
{
    returnNullptrV(actor);

    if (actor == localPlayer)
        return;

    mActors.erase(actor);
    const ActorSpritesMapIterator it = mActorsIdMap.find(actor->getId());
    if (it != mActorsIdMap.end() && (*it).second == actor)
        mActorsIdMap.erase(it);
}

void ActorManager::undelete(const ActorSprite *const actor)
{
    returnNullptrV(actor);

    if (actor == localPlayer)
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

Being *ActorManager::findBeing(const BeingId id) const
{
    const ActorSpritesMapConstIterator it = mActorsIdMap.find(id);
    if (it != mActorsIdMap.end())
    {
        ActorSprite *const actor = (*it).second;
        if ((actor != nullptr) &&
            actor->getId() == id &&
            actor->getType() != ActorType::FloorItem)
        {
            return static_cast<Being*>(actor);
        }
    }
    return nullptr;
}

ActorSprite *ActorManager::findActor(const BeingId id) const
{
    const ActorSpritesMapConstIterator it = mActorsIdMap.find(id);
    if (it != mActorsIdMap.end())
    {
        ActorSprite *const actor = (*it).second;
        if ((actor != nullptr) &&
            actor->getId() == id)
        {
            return actor;
        }
    }
    return nullptr;
}

Being *ActorManager::findBeing(const int x, const int y,
                               const ActorTypeT type) const
{
    beingActorFinder.x = CAST_U16(x);
    beingActorFinder.y = CAST_U16(y);
    beingActorFinder.type = type;

    const ActorSpritesConstIterator it = std::find_if(
        mActors.begin(), mActors.end(), beingActorFinder);

    return (it == mActors.end()) ? nullptr : static_cast<Being*>(*it);
}

Being *ActorManager::findBeingByPixel(const int x, const int y,
                                      const AllPlayers allPlayers) const
{
    if (mMap == nullptr)
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
// disabled for performance
//            if (reportTrue(*it == nullptr))
//                continue;

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

            if (being->getInfo() != nullptr &&
                !(being->getInfo()->isTargetSelection() || modActive))
            {
                continue;
            }

            if ((being->mAction != BeingAction::DEAD ||
                (targetDead && being->getType() == ActorType::Player)) &&
                (allPlayers == AllPlayers_true || being != localPlayer))
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
                    if (tempBeing != nullptr)
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
    for_actorsm
    {
// disabled for performance
//            if (reportTrue(*it == nullptr))
//                continue;

        if ((*it)->getType() == ActorType::Portal ||
            (*it)->getType() == ActorType::FloorItem)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);

        if (being->getInfo() != nullptr &&
            !(being->getInfo()->isTargetSelection() || modActive))
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

void ActorManager::findBeingsByPixel(STD_VECTOR<ActorSprite*> &beings,
                                     const int x, const int y,
                                     const AllPlayers allPlayers) const
{
    if (mMap == nullptr)
        return;

    const int xtol = mapTileSize / 2;
    const int uptol = mapTileSize;
    const bool modActive = inputManager.isActionActive(
        InputAction::STOP_ATTACK);

    for_actors
    {
        ActorSprite *const actor = *it;

// disabled for performance
//        if (reportTrue(actor == nullptr))
//            continue;

        const ActorTypeT actorType = actor->getType();
        switch (actorType)
        {
            default:
            case ActorType::Unknown:
            case ActorType::Avatar:
            case ActorType::Portal:
                break;
            case ActorType::FloorItem:
                if ((actor->getPixelX() - xtol <= x) &&
                    (actor->getPixelX() + xtol > x) &&
                    (actor->getPixelY() - uptol <= y) &&
                    (actor->getPixelY() > y))
                {
                    beings.push_back(actor);
                }
                break;
            case ActorType::Player:
            case ActorType::Npc:
            case ActorType::Monster:
            case ActorType::Pet:
            case ActorType::Mercenary:
            case ActorType::Homunculus:
            case ActorType::SkillUnit:
            case ActorType::Elemental:
            {
                const Being *const being = static_cast<const Being*>(*it);
                if (being == nullptr)
                    continue;
                if ((being->getInfo() != nullptr) &&
                    !(being->getInfo()->isTargetSelection() || modActive))
                {
                    continue;
                }
                if ((being->isAlive() ||
                    (mTargetDeadPlayers &&
                    actorType == ActorType::Player)) &&
                    (allPlayers == AllPlayers_true ||
                    being != localPlayer))
                {
                    if ((actor->getPixelX() - xtol <= x) &&
                        (actor->getPixelX() + xtol > x) &&
                        (actor->getPixelY() - uptol <= y) &&
                        (actor->getPixelY() > y))
                    {
                        beings.push_back(actor);
                    }
                }
                break;
            }
        }
    }
}

Being *ActorManager::findPortalByTile(const int x, const int y) const
{
    if (mMap == nullptr)
        return nullptr;

    for_actorsm
    {
// disabled for performance
//        if (reportTrue(*it == nullptr))
//            continue;

        if ((*it)->getType() != ActorType::Portal)
            continue;

        Being *const being = static_cast<Being*>(*it);

        if (being->getTileX() == x && being->getTileY() == y)
            return being;
    }

    return nullptr;
}

FloorItem *ActorManager::findItem(const BeingId id) const
{
    const ActorSpritesMapConstIterator it = mActorsIdMap.find(id);
    if (it != mActorsIdMap.end())
    {
        ActorSprite *const actor = (*it).second;
        returnNullptr(nullptr, actor);
        if (actor->getId() == id &&
            actor->getType() == ActorType::FloorItem)
        {
            return static_cast<FloorItem*>(actor);
        }
    }
    return nullptr;
}

FloorItem *ActorManager::findItem(const int x, const int y) const
{
    for_actorsm
    {
// disabled for performance
//        if (reportTrue(*it == nullptr))
//            continue;

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
                             const bool serverBuggy) const
{
    if (localPlayer == nullptr)
        return false;

    bool finded(false);
    const bool allowAll = mPickupItemsSet.find("") != mPickupItemsSet.end();
    if (!serverBuggy)
    {
        for_actorsm
        {
// disabled for performance
//            if (reportTrue(*it == nullptr))
//                continue;

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
    else if (PacketLimiter::checkPackets(PacketType::PACKET_PICKUP))
    {
        FloorItem *item = nullptr;
        unsigned cnt = 65535;
        for_actorsm
        {
// disabled for performance
//            if (reportTrue(*it == nullptr))
//                continue;

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
        if ((item != nullptr) && localPlayer->pickUp(item))
            finded = true;
    }
    return finded;
}

bool ActorManager::pickUpNearest(const int x, const int y,
                                 int maxdist) const
{
    if (localPlayer == nullptr)
        return false;

    maxdist = maxdist * maxdist;
    FloorItem *closestItem = nullptr;
    int dist = 0;
    const bool allowAll = mPickupItemsSet.find("") != mPickupItemsSet.end();

    for_actorsm
    {
// disabled for performance
//        if (reportTrue(*it == nullptr))
//            continue;

        if ((*it)->getType() == ActorType::FloorItem)
        {
            FloorItem *const item = static_cast<FloorItem*>(*it);

            const int d = (item->getTileX() - x) * (item->getTileX() - x)
                + (item->getTileY() - y) * (item->getTileY() - y);

            if ((d < dist || closestItem == nullptr) &&
                (!mTargetOnlyReachable || localPlayer->isReachable(
                item->getTileX(), item->getTileY(),
                false)))
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
    if ((closestItem != nullptr) && dist <= maxdist)
        return localPlayer->pickUp(closestItem);

    return false;
}

Being *ActorManager::findBeingByName(const std::string &name,
                                     const ActorTypeT type) const
{
    for_actorsm
    {
// disabled for performance
//        if (reportTrue(*it == nullptr))
//            continue;

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
                                       const ActorTypeT &type) const
{
    if (localPlayer == nullptr)
        return nullptr;

    int dist = 0;
    Being* closestBeing = nullptr;
    int x, y;

    x = localPlayer->getTileX();
    y = localPlayer->getTileY();

    for_actorsm
    {
// disabled for performance
//        if (reportTrue(*it == nullptr))
//            continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);

        if ((being != nullptr) && being->getName() == name &&
            (type == ActorType::Unknown || type == being->getType()))
        {
            if (being->getType() == ActorType::Player)
            {
                return being;
            }
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
// disabled for performance
//        if (reportFalse(*it))
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
        const ActorTypeT &type = actor->getType();
        if (type == ActorType::Player)
        {
            const Being *const being = static_cast<const Being*>(actor);
            being->addToCache();
            if (beingEquipmentWindow != nullptr)
                beingEquipmentWindow->resetBeing(being);
        }
        if (localPlayer != nullptr)
        {
            if (localPlayer->getTarget() == actor)
                localPlayer->setTarget(nullptr);
            if (localPlayer->getPickUpTarget() == actor)
                localPlayer->unSetPickUpTarget();
        }
        if (viewport != nullptr)
            viewport->clearHover(*it);
    }

    FOR_EACH (ActorSpritesConstIterator, it, mDeleteActors)
    {
        ActorSprite *actor = *it;
        mActors.erase(actor);

        if (actor != nullptr)
        {
            const ActorSpritesMapIterator itr = mActorsIdMap.find(
                actor->getId());
            if (itr != mActorsIdMap.end() && (*itr).second == actor)
                mActorsIdMap.erase(itr);

            delete actor;
        }
    }

    mDeleteActors.clear();
    BLOCK_END("ActorManager::logic 1")
    BLOCK_END("ActorManager::logic")
}

void ActorManager::clear()
{
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->setBeing(nullptr);

    if (localPlayer != nullptr)
    {
        localPlayer->setTarget(nullptr);
        localPlayer->unSetPickUpTarget();
        mActors.erase(localPlayer);
    }

    for_actors
        delete *it;
    mActors.clear();
    mDeleteActors.clear();
    mActorsIdMap.clear();

    if (localPlayer != nullptr)
    {
        mActors.insert(localPlayer);
        mActorsIdMap[localPlayer->getId()] = localPlayer;
    }

    mChars.clear();
}

Being *ActorManager::findNearestPvpPlayer() const
{
    if (localPlayer == nullptr)
        return nullptr;

    // don't attack players
    if (settings.pvpAttackType == 3)
        return nullptr;

    const Game *const game = Game::instance();
    if (game == nullptr)
        return nullptr;

    const Map *const map = game->getCurrentMap();
    if (map == nullptr)
        return nullptr;

    const int mapPvpMode = map->getPvpMode();
    Being *target = nullptr;
    int minDistSquared = 20000;

    for_actors
    {
        if ((*it)->getType() != ActorType::Player)
            continue;

        Being *const being = static_cast<Being*>(*it);

        if (reportTrue(being == nullptr) ||
            !being->isAlive() ||
            localPlayer == being)
        {
            continue;
        }

        const int teamId = being->getTeamId();
        // this condition is very TMW-specific
        if (!((mapPvpMode != 0) || (teamId != 0)))
            continue;

        if (!LocalPlayer::checAttackPermissions(being))
            continue;

        const int dx = being->getTileX() - localPlayer->getTileX();
        const int dy = being->getTileY() - localPlayer->getTileY();
        const int distSquared = dx * dx + dy * dy;
        if (distSquared < minDistSquared)
        {
            minDistSquared = distSquared;
            target = being;
        }
    }

    return target;
}


Being *ActorManager::findNearestLivingBeing(const int x, const int y,
                                            const int maxTileDist,
                                            const ActorTypeT type,
                                            const Being *const excluded) const
{
    const int maxDist = maxTileDist * mapTileSize;

    return findNearestLivingBeing(nullptr, maxDist,
        type,
        x, y,
        excluded,
        AllowSort_true);
}

Being *ActorManager::findNearestLivingBeing(const Being *const aroundBeing,
                                            const int maxDist,
                                            const ActorTypeT type,
                                            const AllowSort allowSort) const
{
    if (aroundBeing == nullptr)
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
                                            const ActorTypeT &type,
                                            const int x, const int y,
                                            const Being *const excluded,
                                            const AllowSort allowSort) const
{
    if ((aroundBeing == nullptr) || (localPlayer == nullptr))
        return nullptr;

    std::set<std::string> attackMobs;
    std::set<std::string> priorityMobs;
    std::set<std::string> ignoreAttackMobs;
    StringIntMap attackMobsMap;
    StringIntMap priorityMobsMap;
    int defaultAttackIndex = 10000;
    int defaultPriorityIndex = 10000;
    const int attackRange = localPlayer->getAttackRange();

    bool specialDistance = false;
    if (settings.moveToTargetType == 11
        && localPlayer->getAttackRange() > 2)
    {
        specialDistance = true;
    }

    maxDist = maxDist * maxDist;

    const bool cycleSelect = allowSort == AllowSort_true
        && ((mCyclePlayers && type == ActorType::Player)
        || (mCycleMonsters && type == ActorType::Monster)
        || (mCycleNPC && type == ActorType::Npc));

    const bool filtered = allowSort == AllowSort_true
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
        StringIntMapCIter itr = attackMobsMap.find("");
        if (itr != attackMobsMap.end())
            defaultAttackIndex = (*itr).second;
        itr = priorityMobsMap.find("");
        if (itr != priorityMobsMap.end())
            defaultPriorityIndex = (*itr).second;
    }

    if (cycleSelect)
    {
        STD_VECTOR<Being*> sortedBeings;

        FOR_EACH (ActorSprites::iterator, i, mActors)
        {
//  disabled for performance
//            if (reportTrue(*i == nullptr))
//                continue;

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

            if ((being->getInfo() != nullptr)
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
        STD_VECTOR<Being*>::const_iterator i = std::find_if(
            sortedBeings.begin(), sortedBeings.end(), beingEqualActorFinder);

        if (i == sortedBeings.end() || ++i == sortedBeings.end())
        {
            // if no selected being in vector, return first nearest being
            return sortedBeings.at(0);
        }

        // we find next being after target
        return *i;
    }

    int dist = 0;
    int index = defaultPriorityIndex;
    Being *closestBeing = nullptr;

    FOR_EACH (ActorSprites::iterator, i, mActors)
    {
//  disabled for performance
//            if (reportTrue(*i == nullptr))
//                continue;

        if ((*i)->getType() == ActorType::FloorItem ||
            (*i)->getType() == ActorType::Portal)
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

        if ((being->getInfo() != nullptr)
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

        if (!filtered && (d <= dist || (closestBeing == nullptr)))
        {
            dist = d;
            closestBeing = being;
        }
        else if (filtered)
        {
            int w2 = defaultPriorityIndex;
            if (closestBeing != nullptr)
            {
                const StringIntMapCIter it2 =  priorityMobsMap.find(
                    being->getName());
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

            if (closestBeing == nullptr)
            {
                dist = d;
                closestBeing = being;
                const StringIntMapCIter it1 = priorityMobsMap.find(
                    being->getName());
                if (it1 != priorityMobsMap.end())
                    index = (*it1).second;
                else
                    index = defaultPriorityIndex;
            }
        }
    }
    return (maxDist >= dist) ? closestBeing : nullptr;
}

bool ActorManager::validateBeing(const Being *const aroundBeing,
                                 Being *const being,
                                 const ActorTypeT &type,
                                 const Being* const excluded,
                                 const int maxCost) const
{
    if (localPlayer == nullptr)
        return false;
    return (being != nullptr) && ((being->getType() == type
        || type == ActorType::Unknown) && (being->isAlive()
        || (mTargetDeadPlayers && type == ActorType::Player))
        && being != aroundBeing) && being != excluded
        && (type != ActorType::Monster || !mTargetOnlyReachable
        || localPlayer->isReachable(being, maxCost));
}

#ifdef TMWA_SUPPORT
void ActorManager::healTarget() const
{
    if (localPlayer == nullptr)
        return;

    heal(localPlayer->getTarget());
}

void ActorManager::heal(const Being *const target) const
{
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return;

    if (localPlayer == nullptr ||
        chatWindow == nullptr ||
        !localPlayer->isAlive() ||
        !playerHandler->canUseMagic())
    {
        return;
    }

    // self
    if (target != nullptr &&
        localPlayer->getName() == target->getName())
    {
        if (PlayerInfo::getAttribute(Attributes::PLAYER_MP) >= 6
            && PlayerInfo::getAttribute(Attributes::PLAYER_HP)
            != PlayerInfo::getAttribute(Attributes::PLAYER_MAX_HP))
        {
            if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal1);
        }
    }
    // magic levels < 2
    else if (PlayerInfo::getSkillLevel(340) < 2 ||
             PlayerInfo::getSkillLevel(341) < 2)
    {
        if (PlayerInfo::getAttribute(Attributes::PLAYER_MP) >= 6)
        {
            if (target != nullptr &&
                target->getType() != ActorType::Monster)
            {
                if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1 + " "
                                           + target->getName());
            }
            else if (PlayerInfo::getAttribute(Attributes::PLAYER_HP)
                     != PlayerInfo::getAttribute(Attributes::PLAYER_MAX_HP))
            {
                if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1);
            }
        }
    }
    // magic level >= 2 and not self
    else
    {
        // mp > 10 and target not monster
        if (PlayerInfo::getAttribute(Attributes::PLAYER_MP) >= 10 &&
            target != nullptr &&
            target->getType() != ActorType::Monster)
        {
            // target not enemy
            if (playerRelations.getRelation(target->getName()) !=
                Relation::ENEMY2)
            {
                if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal2 + " "
                    + target->getName());
            }
            // target enemy
            else
            {
                if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
                    return;
                chatWindow->localChatInput(mSpellHeal1);
            }
        }
        // heal self if selected monster or selection empty
        else if ((target == nullptr || target->getType() == ActorType::Monster)
                 && PlayerInfo::getAttribute(Attributes::PLAYER_MP) >= 6
                 && PlayerInfo::getAttribute(Attributes::PLAYER_HP)
                 != PlayerInfo::getAttribute(Attributes::PLAYER_MAX_HP))
        {
            if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
                return;
            chatWindow->localChatInput(mSpellHeal1);
        }
    }
}
#endif  // TMWA_SUPPORT

Being* ActorManager::findMostDamagedPlayer(const int maxTileDist) const
{
    if (localPlayer == nullptr)
        return nullptr;

    int maxDamageTaken = 0;
    Being *target = nullptr;

    for_actors
    {
        if ((*it)->getType() != ActorType::Player)
            continue;

        Being *const being = static_cast<Being*>(*it);

        if ((being == nullptr) || !being->isAlive() ||  // don't heal dead
            playerRelations.getRelation(being->getName()) ==
            Relation::ENEMY2 ||                         // don't heal enemy
            localPlayer == being)                       // don't heal self
        {
            continue;
        }

        const int dx = being->getTileX() - localPlayer->getTileX();
        const int dy = being->getTileY() - localPlayer->getTileY();
        const int distance = fastSqrtInt(dx * dx + dy * dy);

        if (distance > maxTileDist)
            continue;

        if (being->getDamageTaken() > maxDamageTaken)
        {
            maxDamageTaken = being->getDamageTaken();
            target = being;
        }
    }

    return target;
}

#ifdef TMWA_SUPPORT
void ActorManager::itenplz() const
{
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return;
    if (localPlayer == nullptr ||
        chatWindow == nullptr ||
        !localPlayer->isAlive() ||
        !playerHandler->canUseMagic())
    {
        return;
    }

    if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
        return;

    chatWindow->localChatInput(mSpellItenplz);
}
#endif  // TMWA_SUPPORT

bool ActorManager::hasActorSprite(const ActorSprite *const actor) const
{
    for_actors
    {
        if (actor == *it)
            return true;
    }

    return false;
}

void ActorManager::addBlock(const BeingId id)
{
    mBlockedBeings.insert(id);
}

void ActorManager::deleteBlock(const BeingId id)
{
    mBlockedBeings.erase(id);
}

bool ActorManager::isBlocked(const BeingId id) const
{
    return mBlockedBeings.find(id) != mBlockedBeings.end();
}

void ActorManager::printAllToChat()
{
    // TRANSLATORS: visible beings on map
    printBeingsToChat(_("Visible on map"));
}

void ActorManager::printBeingsToChat(const std::string &header) const
{
    if (debugChatTab == nullptr)
        return;

    debugChatTab->chatLog("---------------------------------------",
        ChatMsgType::BY_SERVER,
        IgnoreRecord_false,
        TryRemoveColors_true);
    debugChatTab->chatLog(header,
        ChatMsgType::BY_SERVER,
        IgnoreRecord_false,
        TryRemoveColors_true);
    for_actors
    {
// disabled for performance
//        if (reportTrue(*it == nullptr))
//            continue;

        if ((*it)->getType() == ActorType::FloorItem)
            continue;

        const Being *const being = static_cast<const Being*>(*it);

        debugChatTab->chatLog(strprintf("%s (%d,%d) %d",
            being->getName().c_str(), being->getTileX(), being->getTileY(),
            toInt(being->getSubType(), int)),
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
        if (mActorsIdMap.find(being->getId()) == mActorsIdMap.end())
        {
            debugChatTab->chatLog("missing in id map: %s",
                being->getName().c_str());
        }
    }
    debugChatTab->chatLog("---------------------------------------",
        ChatMsgType::BY_SERVER,
        IgnoreRecord_false,
        TryRemoveColors_true);
    FOR_EACH (ActorSpritesMapConstIterator, itr, mActorsIdMap)
    {
        const ActorSprite *const actor = (*itr).second;
        if (actor == nullptr)
            continue;
        if (actor->getId() != (*itr).first)
            debugChatTab->chatLog("Actor with wrong key in map", "");

        bool found(false);

        for_actors
        {
// disabled for performance
//            if (!*it)
//                continue;

            if ((*it)->getId() == actor->getId())
            {
                found = true;
                break;
            }
        }
        if (!found)
            debugChatTab->chatLog("Actor present in map but not in set", "");
    }
}

void ActorManager::printBeingsToChat(const STD_VECTOR<Being*> &beings,
                                     const std::string &header)
{
    if (debugChatTab == nullptr)
        return;

    debugChatTab->chatLog("---------------------------------------",
        ChatMsgType::BY_SERVER,
        IgnoreRecord_false,
        TryRemoveColors_true);
    debugChatTab->chatLog(header,
        ChatMsgType::BY_SERVER,
        IgnoreRecord_false,
        TryRemoveColors_true);

    FOR_EACH (STD_VECTOR<Being*>::const_iterator, i, beings)
    {
        if (*i == nullptr)
            continue;

        const Being *const being = *i;

        debugChatTab->chatLog(strprintf("%s (%d,%d) %d",
            being->getName().c_str(), being->getTileX(), being->getTileY(),
            toInt(being->getSubType(), int)),
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }
    debugChatTab->chatLog("---------------------------------------",
        ChatMsgType::BY_SERVER,
        IgnoreRecord_false,
        TryRemoveColors_true);
}

void ActorManager::getPlayerNames(StringVect &names,
                                  const NpcNames npcNames) const
{
    names.clear();

    for_actors
    {
// disabled for performance
//      if (reportTrue(*it == nullptr))
//          continue;

        if ((*it)->getType() == ActorType::FloorItem ||
            (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        const Being *const being = static_cast<const Being*>(*it);
        if ((being->getType() == ActorType::Player ||
            (being->getType() == ActorType::Npc &&
            npcNames == NpcNames_true)) &&
            !being->getName().empty())
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
// disabled for performance
//      if (reportTrue(*it == nullptr))
//          continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        const Being *const being = static_cast<const Being*>(*it);
        if (being->getType() == ActorType::Monster &&
            !being->getName().empty())
        {
            names.push_back(being->getName());
        }
    }
}

void ActorManager::updatePlayerNames() const
{
    for_actorsm
    {
// disabled for performance
//      if (reportTrue(*it == nullptr))
//          continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        being->setGoodStatus(-1);
        if (being->getType() == ActorType::Player && !being->getName().empty())
            being->updateName();
    }
}

void ActorManager::updatePlayerColors() const
{
    for_actorsm
    {
// disabled for performance
//      if (reportTrue(*it == nullptr))
//          continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        if (being->getType() == ActorType::Player && !being->getName().empty())
            being->updateColors();
    }
}

void ActorManager::updatePlayerGuild() const
{
    for_actorsm
    {
// disabled for performance
//      if (reportTrue(*it == nullptr))
//          continue;

        if ((*it)->getType() == ActorType::FloorItem
            || (*it)->getType() == ActorType::Portal)
        {
            continue;
        }

        Being *const being = static_cast<Being*>(*it);
        if (being->getType() == ActorType::Player && !being->getName().empty())
            being->updateGuild();
    }
}

#ifdef TMWA_SUPPORT
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
        const size_t bktPos = part.rfind('(');
        if (bktPos != std::string::npos)
        {
            Being *const being = findBeingByName(part.substr(0, bktPos),
                ActorType::Player);
            if (being != nullptr)
            {
                being->setLevel(atoi(part.substr(bktPos + 1).c_str()));
                being->addToCache();
            }
        }
        f = CAST_SIZE(pos + brkEnd.length());
        pos = levels.find(brkEnd, f);
    }
    updatePlayerNames();
}
#endif  // TMWA_SUPPORT

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
    else if (name == "showBadges")
        updateBadges();
    else if (name == "visiblenamespos")
        updateBadges();
    else if (name == "enableIdCollecting")
        mEnableIdCollecting = config.getBoolValue("enableIdCollecting");
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
    storeAttackList();
}

void ActorManager::removePickupItem(const std::string &name)
{
    mPickupItems.remove(name);
    mPickupItemsSet.erase(name);
    mIgnorePickupItems.remove(name);
    mIgnorePickupItemsSet.erase(name);
    rebuildPickupItems();
    storeAttackList();
}

#define addMobToList(name, mob) \
{\
    const int sz = get##mob##sSize();\
    if (sz > 0)\
    {\
        const int idx = get##mob##Index("");\
        if (idx + 1 == sz)\
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
    storeAttackList();
}

void ActorManager::addPriorityAttackMob(const std::string &name)
{
    addMobToList(name, PriorityAttackMob);
    storeAttackList();
}

void ActorManager::addIgnoreAttackMob(const std::string &name)
{
    mIgnoreAttackMobs.push_back(name);
    mIgnoreAttackMobsSet.insert(name);
    rebuildAttackMobs();
    rebuildPriorityAttackMobs();
    storeAttackList();
}

void ActorManager::addPickupItem(const std::string &name)
{
    addMobToList(name, PickupItem);
    rebuildPickupItems();
    storeAttackList();
}

void ActorManager::addIgnorePickupItem(const std::string &name)
{
    mIgnorePickupItems.push_back(name);
    mIgnorePickupItemsSet.insert(name);
    rebuildPickupItems();
    storeAttackList();
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
                                 const StringIntMap &map)
{
    const StringIntMapCIter i = map.find(name);
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
        if ((*i).empty())\
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
    else if ((item != nullptr) && mPickupItemsSet.find(item->getName())
             != mPickupItemsSet.end())
    {
        return true;
    }
    return false;
}

bool ActorManager::checkDefaultPickup() const
{
    return mPickupItemsSet.find("") != mPickupItemsSet.end();
}

void ActorManager::updateEffects(const std::map<BeingTypeId, int> &addEffects,
                                 const std::set<BeingTypeId> &removeEffects)
                                 const
{
    for_actorsm
    {
// disabled for performance
//      if (reportTrue(*it == nullptr))
//          continue;
        if ((*it)->getType() != ActorType::Npc)
            continue;
        Being *const being = static_cast<Being*>(*it);
        const BeingTypeId type = being->getSubType();
        if (removeEffects.find(type) != removeEffects.end())
            being->removeSpecialEffect();
        const std::map<BeingTypeId, int>::const_iterator
            idAdd = addEffects.find(type);
        if (idAdd != addEffects.end())
            being->addSpecialEffect((*idAdd).second);
    }
}

Being *ActorManager::cloneBeing(const Being *const srcBeing,
                                const int dx, const int dy,
                                const int id)
{
    returnNullptr(nullptr, srcBeing);
    Being *const dstBeing = actorManager->createBeing(fromInt(
        toInt(srcBeing->getId(), int) + id, BeingId),
        ActorType::Player,
        srcBeing->getSubType());
    if (dstBeing == nullptr)
        return nullptr;
    dstBeing->setGender(srcBeing->getGender());
    dstBeing->setAction(srcBeing->getCurrentAction(), 0);
    dstBeing->setTileCoords(srcBeing->getTileX() + dx,
        srcBeing->getTileY() + dy);
    dstBeing->setName(srcBeing->getName());
    dstBeing->setDirection(srcBeing->getDirection());
    const int sz = CAST_S32(srcBeing->mSprites.size());
    for (int slot = 0; slot < sz; slot ++)
    {
        const int spriteId = srcBeing->getSpriteID(slot);
        const ItemColor color = srcBeing->getSpriteColor(slot);
        dstBeing->setSpriteColorId(slot,
            spriteId,
            color);
    }
    const int hairSlot = charServerHandler->hairSprite();
    const int hairStyle = -srcBeing->getSpriteID(hairSlot);
    const ItemColor hairColor = srcBeing->getHairColor();
    if (hairStyle != 0)
    {
        dstBeing->setSpriteColor(hairSlot,
            hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
    }
    else
    {
        dstBeing->unSetSprite(hairSlot);
    }
    dstBeing->setHairColor(hairColor);
    return dstBeing;
}

void ActorManager::updateBadges() const
{
    const BadgeDrawType::Type showBadges = static_cast<BadgeDrawType::Type>(
        config.getIntValue("showBadges"));
    Being::mShowBadges = showBadges;
    Being::mVisibleNamePos = static_cast<VisibleNamePos::Type>(
        config.getIntValue("visiblenamespos"));

    for_actors
    {
        ActorSprite *const actor = *it;
        if (actor->getType() == ActorType::Player)
        {
            Being *const being = static_cast<Being*>(actor);
            being->showBadges(showBadges != BadgeDrawType::Hide);
        }
    }
}

void ActorManager::updateNameId(const std::string &name,
                                const BeingId beingId)
{
    if (!mEnableIdCollecting)
        return;
    const int id = CAST_S32(beingId);
    if ((id != 0) &&
        (id < 2000000 ||
        id >= 110000000))
    {
        return;
    }

    if (mIdName.find(beingId) == mIdName.end() ||
        mIdName[beingId].find(name) == mIdName[beingId].end())
    {
        mIdName[beingId].insert(name);
        const std::string idStr = toString(id);
        const std::string dateStr = getDateTimeString();
        std::string dir;
        if (beingId != BeingId_zero)
        {
            dir = pathJoin(settings.usersIdDir,
                idStr,
                stringToHexPath(name));
            Files::saveTextFile(dir,
                "info.txt",
                (name + "\n").append(dateStr));
        }

        dir = settings.usersDir;
        dir.append(stringToHexPath(name));
        Files::saveTextFile(dir,
            "seen.txt",
            (name + "\n").append(idStr).append("\n").append(dateStr));
    }
}

void ActorManager::updateSeenPlayers(const std::set<std::string>
                                     &onlinePlayers)
{
    if (!mEnableIdCollecting)
        return;

    FOR_EACH (std::set<std::string>::const_iterator, it, onlinePlayers)
    {
        const std::string name = *it;
        if (findBeingByName(name, ActorType::Player) == nullptr)
            updateNameId(name, BeingId_zero);
    }
}

std::string ActorManager::getSeenPlayerById(const BeingId id) const
{
    if (!mEnableIdCollecting)
        return std::string();

    const IdNameMappingCIter it = mIdName.find(id);
    if (it != mIdName.end())
    {
        if (!it->second.empty())
            return *(it->second.begin());
    }
    return std::string();
}

void ActorManager::removeRoom(const int chatId)
{
    for_actors
    {
        ActorSprite *const actor = *it;
        if (actor->getType() == ActorType::Npc)
        {
            Being *const being = static_cast<Being*>(actor);
            const ChatObject *const chat = being->getChat();
            if ((chat != nullptr) && chat->chatId == chatId)
            {
                being->setChat(nullptr);
            }
        }
    }
}

void ActorManager::updateRoom(const ChatObject *const newChat)
{
    if (newChat == nullptr)
        return;

    for_actors
    {
        const ActorSprite *const actor = *it;
        if (actor->getType() == ActorType::Npc)
        {
            const Being *const being = static_cast<const Being*>(actor);
            ChatObject *const chat = being->getChat();
            if ((chat != nullptr) && chat->chatId == newChat->chatId)
            {
                chat->ownerId = newChat->ownerId;
                chat->maxUsers = newChat->maxUsers;
                chat->type = newChat->type;
                chat->title = newChat->title;
            }
        }
    }
}

std::string ActorManager::findCharById(const int32_t id)
{
    const std::map<int32_t, std::string>::const_iterator it = mChars.find(id);
    if (it == mChars.end())
        return std::string();
    return (*it).second;
}

void ActorManager::addChar(const int32_t id,
                           const std::string &name)
{
    mChars[id] = name;

    if (guiInput == nullptr)
        return;

    guiInput->simulateMouseMove();
}
