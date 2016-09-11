/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef BEING_PLAYERRELATIONS_H
#define BEING_PLAYERRELATIONS_H

#include "utils/stringvector.h"

#include "enums/being/relation.h"

#include <list>
#include <map>

#include "localconsts.h"

class Being;
class PlayerIgnoreStrategy;
class PlayerRelationsListener;

struct PlayerRelation;

/**
 * Player relations class, represents any particular relations and/or
 * preferences the user of the local client has wrt other players (identified
 * by std::string).
 */
class PlayerRelationsManager final
{
    public:
        PlayerRelationsManager();

        A_DELETE_COPY(PlayerRelationsManager)

        ~PlayerRelationsManager();

        /**
         * Initialise player relations manager (load config file etc.)
         */
        void init();

        /**
         * Load configuration from our config file, or substitute defaults.
         */
        void load();

        /**
         * Save configuration to our config file.
         */
        void store() const;

        /**
         * Determines whether the player in question is being ignored, filtered by
         * the specified flags.
         */
        unsigned int checkPermissionSilently(const std::string &player_name,
                                             const unsigned int flags)
                                             const A_WARN_UNUSED;

        /**
         * Tests whether the player in question is being ignored for any of the
         * actions in the specified flags. If so, trigger appropriate side effects
         * if requested by the player.
         */
        bool hasPermission(const Being *const being,
                           const unsigned int flags) const A_WARN_UNUSED;

        bool hasPermission(const std::string &being,
                           const unsigned int flags) const A_WARN_UNUSED;

        /**
         * Updates the relationship with this player.
         */
        void setRelation(const std::string &name,
                         const RelationT relation);

        /**
         * Updates the relationship with this player.
         */
        RelationT getRelation(const std::string &name) const A_WARN_UNUSED;

        /**
         * Deletes the information recorded for a player.
         */
        void removePlayer(const std::string &name);

        /**
         * Retrieves the default permissions.
         */
        unsigned int getDefault() const A_WARN_UNUSED;

        /**
         * Sets the default permissions.
         */
        void setDefault(const unsigned int permissions);

        /**
         * Retrieves all known player ignore strategies.
         *
         * The player ignore strategies are allocated statically and must
         * not be deleted.
         */
        std::vector<PlayerIgnoreStrategy *> *getPlayerIgnoreStrategies()
            A_WARN_UNUSED;

        /**
         * Return the current player ignore strategy.
         *
         * \return A player ignore strategy, or nullptr
         */
        const PlayerIgnoreStrategy *getPlayerIgnoreStrategy() const
                                                              A_WARN_UNUSED
        { return mIgnoreStrategy; }

        /**
         * Sets the strategy to call when ignoring players.
         */
        void setPlayerIgnoreStrategy(PlayerIgnoreStrategy *const strategy)
        { mIgnoreStrategy = strategy; }

        /**
         * For a given ignore strategy short name, find the appropriate index
         * in the ignore strategies vector.
         *
         * \param The short name of the ignore strategy to look up
         * \return The appropriate index, or -1
         */
        int getPlayerIgnoreStrategyIndex(const std::string &shortname)
                                         A_WARN_UNUSED;

        /**
         * Retrieves a sorted vector of all players for which we have any
         * relations recorded.
         */
        StringVect *getPlayers() const RETURNS_NONNULL A_WARN_UNUSED;

        StringVect *getPlayersByRelation(const RelationT rel)
                                         const A_WARN_UNUSED;

        /**
         * Removes all recorded player info.
         */
        void clear();

        /**
         * Do we persist our `ignore' setup?
         */
        bool getPersistIgnores() const
        { return mPersistIgnores; }

        void ignoreTrade(const std::string &name) const;

        bool isGoodName(Being *const being) const A_WARN_UNUSED;

        bool isGoodName(const std::string &name) const A_WARN_UNUSED;

        /**
         * Change the `ignore persist' flag.
         *
         * @param value Whether to persist ignores
         */
        void setPersistIgnores(const bool value)
        { mPersistIgnores = value; }

        void addListener(PlayerRelationsListener *const listener)
        { mListeners.push_back(listener); }

        void removeListener(PlayerRelationsListener *const listener)
        { mListeners.remove(listener); }

        bool checkBadRelation(const std::string &name) const A_WARN_UNUSED;

    private:
        void signalUpdate(const std::string &name);

        bool mPersistIgnores;  // If NOT set, we delete the
                               // ignored data upon reloading
        unsigned int mDefaultPermissions;

        static bool checkName(const std::string &name) A_WARN_UNUSED;

        PlayerIgnoreStrategy *mIgnoreStrategy;
        std::map<std::string, PlayerRelation *> mRelations;
        std::list<PlayerRelationsListener *> mListeners;
        std::vector<PlayerIgnoreStrategy *> mIgnoreStrategies;
};


extern PlayerRelationsManager player_relations;  // singleton representation
                                                 // of player relations


#endif  // BEING_PLAYERRELATIONS_H
