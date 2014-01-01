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

#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H

#include "flooritem.h"

#include "being/being.h"

#include "localconsts.h"

class LocalPlayer;
class Map;

typedef std::set<ActorSprite*> ActorSprites;
typedef ActorSprites::iterator ActorSpritesIterator;
typedef ActorSprites::const_iterator ActorSpritesConstIterator;

class ActorManager final: public ConfigListener
{
    public:
        ActorManager();

        A_DELETE_COPY(ActorManager)

        ~ActorManager();

        /**
         * Sets the map on which ActorSprites are created.
         */
        void setMap(Map *const map);

        /**
         * Sets the current player.
         */
        void setPlayer(LocalPlayer *const player);

        /**
         * Create a Being and add it to the list of ActorSprites.
         */
        Being *createBeing(const int id, const ActorSprite::Type type,
                           const uint16_t subtype) A_WARN_UNUSED;

        /**
         * Create a FloorItem and add it to the list of ActorSprites.
         */
        FloorItem *createItem(const int id, const int itemId,
                              const int x, const int y,
                              const int amount, const unsigned char color,
                              const int subX, const int subY);

        /**
         * Destroys the given ActorSprite at the end of
         * ActorManager::logic.
         */
        void destroy(ActorSprite *const actor);

        void erase(ActorSprite *const actor);

        void undelete(const ActorSprite *const actor);

        /**
         * Returns a specific Being, by id;
         */
        Being *findBeing(const int id) const A_WARN_UNUSED;

        /**
         * Returns a being at specific coordinates.
         */
        Being *findBeing(const int x, const int y, const ActorSprite::Type
                         type = ActorSprite::UNKNOWN) const A_WARN_UNUSED;

        /**
         * Returns a being at the specific pixel.
         */
        Being *findBeingByPixel(const int x, const int y,
                                const bool allPlayers = false)
                                const A_WARN_UNUSED;

        /**
         * Returns a beings at the specific pixel.
         */
        void findBeingsByPixel(std::vector<ActorSprite*> &beings,
                               const int x, const int y,
                               const bool allPlayers) const;

        /**
         * Returns a portal at the specific tile.
         */
        Being *findPortalByTile(const int x, const int y) const A_WARN_UNUSED;

        /**
         * Returns a specific FloorItem, by id.
         */
        FloorItem *findItem(const int id) const A_WARN_UNUSED;

        /**
         * Returns a FloorItem at specific coordinates.
         */
        FloorItem *findItem(const int x, const int y) const A_WARN_UNUSED;

        /**
         * Returns a being nearest to specific coordinates.
         *
         * @param x           X coordinate in pixels.
         * @param y           Y coordinate in pixels.
         * @param maxTileDist Maximal distance in tiles. If minimal distance is
         *                    larger, no being is returned.
         * @param type        The type of being to look for.
         */
        Being *findNearestLivingBeing(const int x, const int y,
                                      int maxTileDist,
                                      const ActorSprite::Type
                                      type = Being::UNKNOWN,
                                      const Being *const
                                      excluded = nullptr) const A_WARN_UNUSED;

        /**
         * Returns a being nearest to another being.
         *
         * @param aroundBeing The being to search around.
         * @param maxTileDist Maximal distance in tiles. If minimal distance is
         *                    larger, no being is returned.
         * @param type        The type of being to look for.
         */
        Being *findNearestLivingBeing(const Being *const aroundBeing,
                                      const int maxTileDist,
                                      const ActorSprite::Type
                                      type = Being::UNKNOWN)
                                      const A_WARN_UNUSED;

        /**
         * Finds a being by name and (optionally) by type.
         */
        Being *findBeingByName(const std::string &name,
                               const ActorSprite::Type
                               type = Being::UNKNOWN) const A_WARN_UNUSED;

       /**
        * Finds a nearest being by name and (optionally) by type.
        */
        Being *findNearestByName(const std::string &name,
                                 const Being::Type &type = Being::UNKNOWN)
                                 const A_WARN_UNUSED;

       /**
        * Heal all players in distance.
        *
        * \param maxdist maximal distance. If minimal distance is larger,
        *                no being is returned
        */
//        void HealAllTargets(Being *aroundBeing, int maxdist,
//                                            Being::Type type) const;

        void healTarget() const;

        void heal(const Being *const target) const;

        void itenplz() const;

        /**
         * Returns the whole list of beings.
         */
        const ActorSprites &getAll() const;

        /**
         * Returns true if the given ActorSprite is in the manager's list,
         * false otherwise.
         *
         * \param actor the ActorSprite to search for
         */
        bool hasActorSprite(const ActorSprite *const actor)
                            const A_WARN_UNUSED;

        /**
         * Performs ActorSprite logic and deletes ActorSprite scheduled to be
         * deleted.
         */
        void logic();

        /**
         * Destroys all ActorSprites except the local player
         */
        void clear();

        void addBlock(const uint32_t id);

        void deleteBlock(const uint32_t id);

        bool isBlocked(const uint32_t id) const;

        void printAllToChat() const;

        void printBeingsToChat(const ActorSprites &beings,
                               const std::string &header) const;

        void printBeingsToChat(const std::vector<Being*> &beings,
                               const std::string &header) const;

        void getPlayerNames(StringVect &names,
                            const bool npcNames) const;

        void getMobNames(StringVect &names) const;

        void updatePlayerNames() const;

        void updatePlayerColors() const;

        void updatePlayerGuild() const;

        void parseLevels(std::string levels) const;

        bool pickUpAll(const int x1, const int y1, const int x2, const int y2,
                       const bool serverBuggy = false);

        bool pickUpNearest(const int x, const int y, int maxdist) const;

        void optionChanged(const std::string &name) override final;

        void removeAttackMob(const std::string &name);

        void removePickupItem(const std::string &name);

        void addPriorityAttackMob(const std::string &name);

        void addAttackMob(const std::string &name);

        void addIgnoreAttackMob(const std::string &name);

        void addPickupItem(const std::string &name);

        void addIgnorePickupItem(const std::string &name);

        void setPriorityAttackMobs(std::list<std::string> mobs)
        { mPriorityAttackMobs = mobs; }

        void setAttackMobs(std::list<std::string> mobs)
        { mAttackMobs = mobs; }

        int getPriorityAttackMobsSize() const A_WARN_UNUSED
        { return static_cast<int>(mPriorityAttackMobs.size()); }

        int getAttackMobsSize() const A_WARN_UNUSED
        { return static_cast<int>(mAttackMobs.size()); }

        int getPickupItemsSize() const A_WARN_UNUSED
        { return static_cast<int>(mPickupItems.size()); }

#define defList(list1, mob) \
        bool isIn##list1##List(const std::string &name) const A_WARN_UNUSED\
        { return m##list1##mob##Set.find(name) != m##list1##mob##Set.end(); }\
        void rebuild##list1##mob();\
        std::set<std::string> get##list1##mob##Set() const A_WARN_UNUSED\
        { return m##list1##mob##Set; }\
        std::list<std::string> get##list1##mob() const A_WARN_UNUSED\
        { return m##list1##mob; }

        defList(Attack, Mobs)
        defList(PriorityAttack, Mobs)
        defList(IgnoreAttack, Mobs)
        defList(Pickup, Items)
        defList(IgnorePickup, Items)

        const std::map<std::string, int> &getAttackMobsMap()
                                          const A_WARN_UNUSED
        { return mAttackMobsMap; }

        const std::map<std::string, int> &getPriorityAttackMobsMap()
                                   const A_WARN_UNUSED
        { return mPriorityAttackMobsMap; }

        int getAttackMobIndex(const std::string &name) const A_WARN_UNUSED;

        int getPriorityAttackMobIndex(const std::string &name)
                                      const A_WARN_UNUSED;

        int getPickupItemIndex(const std::string &name) const A_WARN_UNUSED;

        int getIndexByName(const std::string &name, const std::map<std::string,
                           int> &map) const A_WARN_UNUSED;

        bool checkForPickup(const FloorItem *const item) const A_WARN_UNUSED;

        void updateEffects(const std::map<int, int> &addEffects,
                           const std::set<int> &removeEffects) const;

    protected:
        bool validateBeing(const Being *const aroundBeing,
                           Being *const being,
                           const Being::Type &type,
                           const Being *const excluded = nullptr,
                           const int maxCost = 20) const A_WARN_UNUSED;

        Being *findNearestLivingBeing(const Being *const aroundBeing,
                                      const int maxdist,
                                      const Being::Type &type,
                                      const int x, const int y,
                                      const Being *const
                                      excluded = nullptr) const A_WARN_UNUSED;

        void loadAttackList();

        void storeAttackList() const;

        ActorSprites mActors;
        ActorSprites mDeleteActors;
        std::set<uint32_t> mBlockedBeings;
        Map *mMap;
        std::string mSpellHeal1;
        std::string mSpellHeal2;
        std::string mSpellItenplz;
        bool mTargetDeadPlayers;
        bool mTargetOnlyReachable;
        bool mCyclePlayers;
        bool mCycleMonsters;
        bool mCycleNPC;
        bool mExtMouseTargeting;

#define defVarsP(mob) \
        std::list<std::string> mPriority##mob;\
        std::set<std::string> mPriority##mob##Set;\
        std::map<std::string, int> mPriority##mob##Map;

#define defVars(mob) \
        std::list<std::string> m##mob;\
        std::set<std::string> m##mob##Set;\
        std::map<std::string, int> m##mob##Map;\
        std::list<std::string> mIgnore##mob;\
        std::set<std::string> mIgnore##mob##Set;

        defVarsP(AttackMobs)
        defVars(AttackMobs)
        defVars(PickupItems)
};

extern ActorManager *actorManager;

#endif  // ACTORMANAGER_H
