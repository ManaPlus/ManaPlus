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

#ifndef ACTORSPRITEMANAGER_H
#define ACTORSPRITEMANAGER_H

#include "being.h"
#include "flooritem.h"

class LocalPlayer;
class Map;

typedef std::set<ActorSprite*> ActorSprites;
typedef ActorSprites::iterator ActorSpritesIterator;
typedef ActorSprites::const_iterator ActorSpritesConstIterator;

class ActorSpriteManager final: public ConfigListener
{
    public:
        ActorSpriteManager();

        ~ActorSpriteManager();

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
                           const uint16_t subtype);

        /**
         * Create a FloorItem and add it to the list of ActorSprites.
         */
        FloorItem *createItem(const int id, const int itemId,
                              const int x, const int y,
                              const int amount, const unsigned char color,
                              const int subX, const int subY);

        /**
         * Destroys the given ActorSprite at the end of
         * ActorSpriteManager::logic.
         */
        void destroy(ActorSprite *const actor);

        void erase(ActorSprite *const actor);

        void undelete(const ActorSprite *const actor);

        /**
         * Returns a specific Being, by id;
         */
        Being *findBeing(const int id) const;

        /**
         * Returns a being at specific coordinates.
         */
        Being *findBeing(const int x, const int y, const ActorSprite::Type
                         type = ActorSprite::UNKNOWN) const;

        /**
         * Returns a being at the specific pixel.
         */
        Being *findBeingByPixel(const int x, const int y,
                                const bool allPlayers = false) const;

        /**
         * Returns a beings at the specific pixel.
         */
        void findBeingsByPixel(std::vector<ActorSprite*> &beings,
                               const int x, const int y,
                               const bool allPlayers) const;

        /**
         * Returns a portal at the specific tile.
         */
        Being *findPortalByTile(const int x, const int y) const;

        /**
         * Returns a specific FloorItem, by id.
         */
        FloorItem *findItem(const int id) const;

        /**
         * Returns a FloorItem at specific coordinates.
         */
        FloorItem *findItem(const int x, const int y) const;

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
                                      excluded = nullptr) const;

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
                                      type = Being::UNKNOWN
                                      ) const;

        /**
         * Finds a being by name and (optionally) by type.
         */
        Being *findBeingByName(const std::string &name,
                               const ActorSprite::Type
                               type = Being::UNKNOWN) const;

       /**
        * Finds a nearest being by name and (optionally) by type.
        */
        Being *findNearestByName(const std::string &name,
                                 const Being::Type &type = Being::UNKNOWN
                                ) const;

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
        bool hasActorSprite(const ActorSprite *const actor) const;

        /**
         * Performs ActorSprite logic and deletes ActorSprite scheduled to be
         * deleted.
         */
        void logic();

        /**
         * Destroys all ActorSprites except the local player
         */
        void clear();

        std::vector<uint32_t> blockedBeings;

        void addBlock(const uint32_t id);

        void deleteBlock(const uint32_t id);

        bool isBlocked(const uint32_t id) const;

        void printAllToChat() const;

        void printBeingsToChat(ActorSprites beings, std::string header) const;

        void printBeingsToChat(std::vector<Being*> beings,
                               std::string header) const;

        void getPlayerNames(StringVect &names,
                            const bool npcNames) const;

        void getMobNames(StringVect &names) const;

        void updatePlayerNames() const;

        void updatePlayerColors() const;

        void updatePlayerGuild() const;

        void parseLevels(std::string levels) const;

        bool pickUpAll(const int x1, const int y1, const int x2, const int y2,
                       const bool serverBuggy = false);

        bool pickUpNearest(const int x, const int y, int maxdist);

        void optionChanged(const std::string &name);

        void removeAttackMob(const std::string &name);

        void removePickupItem(const std::string &name);

        void addPriorityAttackMob(std::string name);

        void addAttackMob(std::string name);

        void addIgnoreAttackMob(std::string name);

        void addPickupItem(std::string name);

        void addIgnorePickupItem(std::string name);

        void setPriorityAttackMobs(std::list<std::string> mobs)
        { mPriorityAttackMobs = mobs; }

        void setAttackMobs(std::list<std::string> mobs)
        { mAttackMobs = mobs; }

        int getPriorityAttackMobsSize() const
        { return static_cast<int>(mPriorityAttackMobs.size()); }

        int getAttackMobsSize() const
        { return static_cast<int>(mAttackMobs.size()); }

        int getPickupItemsSize() const
        { return static_cast<int>(mPickupItems.size()); }

#define defList(list1, mob) \
        bool isIn##list1##List(const std::string &name) const\
        { return m##list1##mob##Set.find(name) != m##list1##mob##Set.end(); }\
        void rebuild##list1##mob();\
        std::set<std::string> get##list1##mob##Set() const\
        { return m##list1##mob##Set; }\
        std::list<std::string> get##list1##mob() const\
        { return m##list1##mob; }

        defList(Attack, Mobs)
        defList(PriorityAttack, Mobs)
        defList(IgnoreAttack, Mobs)
        defList(Pickup, Items)
        defList(IgnorePickup, Items)

        std::map<std::string, int> getAttackMobsMap() const
        { return mAttackMobsMap; }

        std::map<std::string, int> getPriorityAttackMobsMap() const
        { return mPriorityAttackMobsMap; }

        int getAttackMobIndex(std::string name);

        int getPriorityAttackMobIndex(std::string name);

        int getPickupItemIndex(std::string name);

        int getIndexByName(std::string name, std::map<std::string,
                           int> &map) const;

        bool checkForPickup(const FloorItem *const item) const;

    protected:
        bool validateBeing(const Being *const aroundBeing,
                           Being *const being,
                           const Being::Type type,
                           const Being *const excluded = nullptr,
                           const int maxCost = 20) const;

        Being *findNearestLivingBeing(const Being *const aroundBeing,
                                      const int maxdist,
                                      const Being::Type type,
                                      const int x, const int y,
                                      const Being *const
                                      excluded = nullptr) const;

        void loadAttackList();
        void storeAttackList();

        ActorSprites mActors;
        ActorSprites mDeleteActors;
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

extern ActorSpriteManager *actorSpriteManager;

#endif // ACTORSPRITEMANAGER_H
