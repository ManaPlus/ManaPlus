/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "being/playerrelations.h"

#include "actormanager.h"
#include "configuration.h"
#include "logger.h"

#include "being/localplayer.h"
#include "being/playerignorestrategy.h"
#include "being/playerrelation.h"

#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "listeners/playerrelationslistener.h"

#include "debug.h"

static const unsigned int FIRST_IGNORE_EMOTE = 14;

typedef std::map<std::string, PlayerRelation *> PlayerRelations;
typedef PlayerRelations::const_iterator PlayerRelationsCIter;
typedef std::list<PlayerRelationsListener *> PlayerRelationListeners;
typedef PlayerRelationListeners::const_iterator PlayerRelationListenersCIter;

static const char *const PLAYER_IGNORE_STRATEGY_NOP = "nop";
static const char *const PLAYER_IGNORE_STRATEGY_EMOTE0 = "emote0";
static const char *const DEFAULT_IGNORE_STRATEGY =
    PLAYER_IGNORE_STRATEGY_EMOTE0;

static const char *const NAME = "name";
static const char *const RELATION = "relation";

static const unsigned int IGNORE_EMOTE_TIME = 100;

namespace
{
    class SortPlayersFunctor final
    {
        public:
            SortPlayersFunctor()
            { }

            A_DEFAULT_COPY(SortPlayersFunctor)

            bool operator() (const std::string &str1,
                             const std::string &str2) const
            {
                std::string s1 = str1;
                std::string s2 = str2;
                toLower(s1);
                toLower(s2);
                if (s1 == s2)
                    return str1 < str2;
                return s1 < s2;
            }
    } playersRelSorter;

    // (De)serialisation class
    class PlayerConfSerialiser final :
        public ConfigurationListManager<std::pair<std::string,
            PlayerRelation *>, std::map<std::string, PlayerRelation *> *>
    {
        public:
            PlayerConfSerialiser()
            { }

            A_DELETE_COPY(PlayerConfSerialiser)

            ConfigurationObject *writeConfigItem(
                const std::pair<std::string, PlayerRelation *> &value,
                ConfigurationObject *const cobj) const override final
            {
                if (cobj == nullptr ||
                    value.second == nullptr)
                {
                    return nullptr;
                }
                cobj->setValue(NAME, value.first);
                cobj->setValue(RELATION, toString(
                    CAST_S32(value.second->mRelation)));

                return cobj;
            }

            std::map<std::string, PlayerRelation *> *
            readConfigItem(const ConfigurationObject *const cobj,
                           std::map<std::string, PlayerRelation *>
                           *const container) const override final
            {
                if (cobj == nullptr ||
                    container == nullptr)
                {
                    return container;
                }
                const std::string name = cobj->getValue(NAME, "");
                if (name.empty())
                    return container;

                if ((*container)[name] == nullptr)
                {
                    const int v = cobj->getValueInt(RELATION,
                        CAST_S32(Relation::NEUTRAL));

                    (*container)[name] = new PlayerRelation(
                        static_cast<RelationT>(v));
                }
                // otherwise ignore the duplicate entry

                return container;
            }
    };
}  // namespace

static PlayerConfSerialiser player_conf_serialiser;  // stateless singleton

const unsigned int PlayerRelation::RELATION_PERMISSIONS[RELATIONS_NR] =
{
    /* NEUTRAL */     0,  // we always fall back to the defaults anyway
    /* FRIEND  */     EMOTE | SPEECH_FLOAT | SPEECH_LOG | WHISPER | TRADE,
    /* DISREGARDED*/  EMOTE | SPEECH_FLOAT,
    /* IGNORED */     0,
    /* ERASED */      INVISIBLE,
    /* BLACKLISTED */ SPEECH_LOG | WHISPER,
    /* ENEMY2 */      EMOTE | SPEECH_FLOAT | SPEECH_LOG | WHISPER | TRADE
};

PlayerRelationsManager::PlayerRelationsManager() :
    mPersistIgnores(false),
    mDefaultPermissions(PlayerRelation::DEFAULT),
    mIgnoreStrategy(nullptr),
    mRelations(),
    mListeners(),
    mIgnoreStrategies()
{
}

PlayerRelationsManager::~PlayerRelationsManager()
{
    delete_all(mIgnoreStrategies);

    FOR_EACH (PlayerRelationsCIter, it, mRelations)
        delete it->second;
    mRelations.clear();
}

void PlayerRelationsManager::clear()
{
    StringVect *const names = getPlayers();
    FOR_EACHP (StringVectCIter, it, names)
        removePlayer(*it);
    delete names;
}

static const char *const PERSIST_IGNORE_LIST = "persistent-player-list";
static const char *const PLAYER_IGNORE_STRATEGY = "player-ignore-strategy";
static const char *const DEFAULT_PERMISSIONS = "default-player-permissions";

int PlayerRelationsManager::getPlayerIgnoreStrategyIndex(
    const std::string &name)
{
    const STD_VECTOR<PlayerIgnoreStrategy *> *const strategies
        = getPlayerIgnoreStrategies();

    if (strategies == nullptr)
        return -1;

    const size_t sz = strategies->size();
    for (size_t i = 0; i < sz; i++)
    {
        if ((*strategies)[i]->mShortName == name)
            return CAST_S32(i);
    }

    return -1;
}

void PlayerRelationsManager::load()
{
    Configuration *const cfg = &serverConfig;
    clear();

    mPersistIgnores = (cfg->getValue(PERSIST_IGNORE_LIST, 1) != 0);
    mDefaultPermissions = CAST_S32(cfg->getValue(DEFAULT_PERMISSIONS,
                                           mDefaultPermissions));

    const std::string ignore_strategy_name = cfg->getValue(
        PLAYER_IGNORE_STRATEGY, DEFAULT_IGNORE_STRATEGY);
    const int ignore_strategy_index = getPlayerIgnoreStrategyIndex(
        ignore_strategy_name);

    if (ignore_strategy_index >= 0)
    {
        setPlayerIgnoreStrategy((*getPlayerIgnoreStrategies())
                                [ignore_strategy_index]);
    }

    cfg->getList<std::pair<std::string, PlayerRelation *>,
                   std::map<std::string, PlayerRelation *> *>
        ("player",  &(mRelations), &player_conf_serialiser);
}


void PlayerRelationsManager::init()
{
    load();

    if (!mPersistIgnores)
    {
        clear();  // Yes, we still keep them around in the config file
                  // until the next update.
    }

    FOR_EACH (PlayerRelationListenersCIter, it, mListeners)
        (*it)->updateAll();
}

void PlayerRelationsManager::store() const
{
    serverConfig.setList<std::map<std::string,
        PlayerRelation *>::const_iterator,
        std::pair<std::string, PlayerRelation *>,
        std::map<std::string, PlayerRelation *> *>
        ("player", mRelations.begin(), mRelations.end(),
        &player_conf_serialiser);

    serverConfig.setValue(DEFAULT_PERMISSIONS, mDefaultPermissions);
    serverConfig.setValue(PERSIST_IGNORE_LIST, mPersistIgnores);
    serverConfig.setValue(PLAYER_IGNORE_STRATEGY,
        mIgnoreStrategy != nullptr ? mIgnoreStrategy->mShortName :
        DEFAULT_IGNORE_STRATEGY);

    serverConfig.write();
}

void PlayerRelationsManager::signalUpdate(const std::string &name)
{
    FOR_EACH (PlayerRelationListenersCIter, it, mListeners)
        (*it)->updatedPlayer(name);

    if (actorManager != nullptr)
    {
        Being *const being = actorManager->findBeingByName(
            name, ActorType::Player);

        if (being != nullptr &&
            being->getType() == ActorType::Player)
        {
            being->updateColors();
        }
    }
}

unsigned int PlayerRelationsManager::checkPermissionSilently(
    const std::string &player_name, const unsigned int flags) const
{
    const std::map<std::string, PlayerRelation *>::const_iterator
        it = mRelations.find(player_name);
    if (it == mRelations.end())
    {
        return mDefaultPermissions & flags;
    }
    else
    {
        const PlayerRelation *const r = (*it).second;
        unsigned int permissions = PlayerRelation::RELATION_PERMISSIONS[
            CAST_S32(r->mRelation)];

        switch (r->mRelation)
        {
            case Relation::NEUTRAL:
                permissions = mDefaultPermissions;
                break;

            case Relation::FRIEND:
                permissions |= mDefaultPermissions;  // widen
                break;

            case Relation::DISREGARDED:
            case Relation::IGNORED:
            case Relation::ERASED:
            case Relation::BLACKLISTED:
            case Relation::ENEMY2:
            default:
                permissions &= mDefaultPermissions;  // narrow
                break;
        }

        return permissions & flags;
    }
}

bool PlayerRelationsManager::hasPermission(const Being *const being,
                                           const unsigned int flags) const
{
    if (being == nullptr)
        return false;

    if (being->getType() == ActorType::Player)
    {
        return static_cast<unsigned int>(hasPermission(
            being->getName(), flags)) == flags;
    }
    return true;
}

bool PlayerRelationsManager::hasPermission(const std::string &name,
                                           const unsigned int flags) const
{
    if (actorManager == nullptr)
        return false;

    const unsigned int rejections = flags
        & ~checkPermissionSilently(name, flags);
    const bool permitted = (rejections == 0);

    if (!permitted)
    {
        // execute `ignore' strategy, if possible
        if (mIgnoreStrategy != nullptr)
        {
            Being *const b = actorManager->findBeingByName(
                name, ActorType::Player);

            if ((b != nullptr) && b->getType() == ActorType::Player)
                mIgnoreStrategy->ignore(b, rejections);
        }
    }

    return permitted;
}

void PlayerRelationsManager::setRelation(const std::string &player_name,
                                         const RelationT relation)
{
    if (localPlayer == nullptr ||
        (relation != Relation::NEUTRAL &&
        localPlayer->getName() == player_name))
    {
        return;
    }

    PlayerRelation *const r = mRelations[player_name];
    if (r == nullptr)
        mRelations[player_name] = new PlayerRelation(relation);
    else
        r->mRelation = relation;

    signalUpdate(player_name);
    store();
}

StringVect *PlayerRelationsManager::getPlayers() const
{
    StringVect *const retval = new StringVect;

    FOR_EACH (PlayerRelationsCIter, it, mRelations)
    {
        if (it->second != nullptr)
            retval->push_back(it->first);
    }

    std::sort(retval->begin(), retval->end(), playersRelSorter);

    return retval;
}

StringVect *PlayerRelationsManager::getPlayersByRelation(
    const RelationT rel) const
{
    StringVect *const retval = new StringVect;

    FOR_EACH (PlayerRelationsCIter, it, mRelations)
    {
        if ((it->second != nullptr) &&
            it->second->mRelation == rel)
        {
            retval->push_back(it->first);
        }
    }

    std::sort(retval->begin(), retval->end(), playersRelSorter);

    return retval;
}

void PlayerRelationsManager::removePlayer(const std::string &name)
{
    delete mRelations[name];
    mRelations.erase(name);
    signalUpdate(name);
}


RelationT PlayerRelationsManager::getRelation(
    const std::string &name) const
{
    const std::map<std::string, PlayerRelation *>::const_iterator
        it = mRelations.find(name);
    if (it != mRelations.end())
        return (*it).second->mRelation;

    return Relation::NEUTRAL;
}

////////////////////////////////////////
// defaults

unsigned int PlayerRelationsManager::getDefault() const
{
    return mDefaultPermissions;
}

void PlayerRelationsManager::setDefault(const unsigned int permissions)
{
    mDefaultPermissions = permissions;

    store();
    signalUpdate("");
}

void PlayerRelationsManager::ignoreTrade(const std::string &name) const
{
    if (name.empty())
        return;

    const RelationT relation = getRelation(name);

    if (relation == Relation::IGNORED ||
        relation == Relation::DISREGARDED ||
        relation == Relation::BLACKLISTED ||
        relation == Relation::ERASED)
    {
        return;
    }
    else
    {
        playerRelations.setRelation(name, Relation::BLACKLISTED);
    }
}

bool PlayerRelationsManager::checkBadRelation(const std::string &name) const
{
    if (name.empty())
        return true;

    const RelationT relation = getRelation(name);

    if (relation == Relation::IGNORED ||
        relation == Relation::DISREGARDED ||
        relation == Relation::BLACKLISTED ||
        relation == Relation::ERASED ||
        relation == Relation::ENEMY2)
    {
        return true;
    }
    return false;
}

////////////////////////////////////////
// ignore strategies


class PIS_nothing final : public PlayerIgnoreStrategy
{
    public:
        PIS_nothing() :
            PlayerIgnoreStrategy()
        {
            // TRANSLATORS: ignore/unignore action
            mDescription = _("Completely ignore");
            mShortName = PLAYER_IGNORE_STRATEGY_NOP;
        }

        A_DELETE_COPY(PIS_nothing)

        void ignore(Being *const being A_UNUSED,
                    const unsigned int flags A_UNUSED) const override final
        {
        }
};

class PIS_dotdotdot final : public PlayerIgnoreStrategy
{
    public:
        PIS_dotdotdot() :
            PlayerIgnoreStrategy()
        {
            // TRANSLATORS: ignore/unignore action
            mDescription = _("Print '...'");
            mShortName = "dotdotdot";
        }

        A_DELETE_COPY(PIS_dotdotdot)

        void ignore(Being *const being,
                    const unsigned int flags A_UNUSED) const override final
        {
            if (being == nullptr)
                return;

            logger->log("ignoring: " + being->getName());
            being->setSpeech("...");
        }
};


class PIS_blinkname final : public PlayerIgnoreStrategy
{
    public:
        PIS_blinkname() :
            PlayerIgnoreStrategy()
        {
            // TRANSLATORS: ignore/unignore action
            mDescription = _("Blink name");
            mShortName = "blinkname";
        }

        A_DELETE_COPY(PIS_blinkname)

        void ignore(Being *const being,
                    const unsigned int flags A_UNUSED) const override final
        {
            if (being == nullptr)
                return;

            logger->log("ignoring: " + being->getName());
            being->flashName(200);
        }
};

class PIS_emote final : public PlayerIgnoreStrategy
{
    public:
        PIS_emote(const uint8_t emote_nr,
                  const std::string &description,
                  const std::string &shortname) :
            PlayerIgnoreStrategy(),
            mEmotion(emote_nr)
        {
            mDescription = description;
            mShortName = shortname;
        }

        A_DELETE_COPY(PIS_emote)

        void ignore(Being *const being,
                    const unsigned int flags A_UNUSED) const override final
        {
            if (being == nullptr)
                return;

            being->setEmote(mEmotion, IGNORE_EMOTE_TIME);
        }
        uint8_t mEmotion;
};

STD_VECTOR<PlayerIgnoreStrategy *> *
PlayerRelationsManager::getPlayerIgnoreStrategies()
{
    if (mIgnoreStrategies.empty())
    {
        // not initialised yet?
        mIgnoreStrategies.push_back(new PIS_emote(FIRST_IGNORE_EMOTE,
                                    // TRANSLATORS: ignore strategi
                                    _("Floating '...' bubble"),
                                    PLAYER_IGNORE_STRATEGY_EMOTE0));
        mIgnoreStrategies.push_back(new PIS_emote(FIRST_IGNORE_EMOTE + 1,
                                    // TRANSLATORS: ignore strategi
                                    _("Floating bubble"),
                                    "emote1"));
        mIgnoreStrategies.push_back(new PIS_nothing);
        mIgnoreStrategies.push_back(new PIS_dotdotdot);
        mIgnoreStrategies.push_back(new PIS_blinkname);
    }
    return &mIgnoreStrategies;
}

bool PlayerRelationsManager::isGoodName(const std::string &name) const
{
    const size_t size = name.size();

    if (size < 3)
        return true;

    const std::map<std::string, PlayerRelation *>::const_iterator
        it = mRelations.find(name);
    if (it != mRelations.end())
        return true;

    return checkName(name);
}

bool PlayerRelationsManager::isGoodName(Being *const being) const
{
    if (being == nullptr)
        return false;
    if (being->getGoodStatus() != -1)
        return (being->getGoodStatus() == 1);

    const std::string &name = being->getName();
    const size_t size = name.size();

    if (size < 3)
        return true;

    const std::map<std::string, PlayerRelation *>::const_iterator
        it = mRelations.find(name);
    if (it != mRelations.end())
        return true;

    const bool status = checkName(name);
    being->setGoodStatus(status ? 1 : 0);
    return status;
}

bool PlayerRelationsManager::checkName(const std::string &name)
{
    const size_t size = name.size();
    const std::string check = config.getStringValue("unsecureChars");
    const std::string lastChar = name.substr(size - 1, 1);

    if (name.substr(0, 1) == " " ||
        lastChar == " " ||
        lastChar == "." ||
        name.find("  ") != std::string::npos)
    {
        return false;
    }
    else if (check.empty())
    {
        return true;
    }
    else if (name.find_first_of(check) != std::string::npos)
    {
        return false;
    }
    else
    {
        return true;
    }
}

PlayerRelationsManager playerRelations;
