/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "being/playerrelations.h"

#include "actormanager.h"
#include "configuration.h"

#include "being/localplayer.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "listeners/playerrelationslistener.h"

#include <algorithm>

#include "debug.h"

static const char *const PLAYER_IGNORE_STRATEGY_NOP = "nop";
static const char *const PLAYER_IGNORE_STRATEGY_EMOTE0 = "emote0";
static const char *const DEFAULT_IGNORE_STRATEGY
    = PLAYER_IGNORE_STRATEGY_EMOTE0;

static const char *const NAME = "name";
static const char *const RELATION = "relation";

static const unsigned int IGNORE_EMOTE_TIME = 100;

typedef std::map<std::string, PlayerRelation *> PlayerRelations;
typedef PlayerRelations::const_iterator PlayerRelationsCIter;
typedef std::list<PlayerRelationsListener *> PlayerRelationListeners;
typedef PlayerRelationListeners::const_iterator PlayerRelationListenersCIter;

class SortPlayersFunctor final
{
    public:
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
    public ConfigurationListManager<std::pair<std::string, PlayerRelation *>,
        std::map<std::string, PlayerRelation *> *>
{
public:
    ConfigurationObject *writeConfigItem(
        const std::pair<std::string, PlayerRelation *> &value,
        ConfigurationObject *const cobj) const override final
    {
        if (!cobj || !value.second)
            return nullptr;
        cobj->setValue(NAME, value.first);
        cobj->setValue(RELATION, toString(
                static_cast<int>(value.second->mRelation)));

        return cobj;
    }

    std::map<std::string, PlayerRelation *> *
    readConfigItem(const ConfigurationObject *const cobj,
                   std::map<std::string, PlayerRelation *>
                   *const container) const override final
    {
        if (!cobj)
            return container;
        const std::string name = cobj->getValue(NAME, "");
        if (name.empty())
            return container;

        if (!(*container)[name])
        {
            const int v = cobj->getValueInt(RELATION,
                static_cast<int>(PlayerRelation::NEUTRAL));

            (*container)[name] = new PlayerRelation(
                static_cast<PlayerRelation::Relation>(v));
        }
        // otherwise ignore the duplicate entry

        return container;
    }
};

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

PlayerRelation::PlayerRelation(const Relation relation) :
    mRelation(relation)
{
}

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
    const std::vector<PlayerIgnoreStrategy *> *const strategies
        = getPlayerIgnoreStrategies();

    if (!strategies)
        return -1;

    const size_t sz = strategies->size();
    for (size_t i = 0; i < sz; i++)
    {
        if ((*strategies)[i]->mShortName == name)
            return i;
    }

    return -1;
}

void PlayerRelationsManager::load(const bool oldConfig)
{
    Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;
    clear();

    mPersistIgnores = cfg->getValue(PERSIST_IGNORE_LIST, 1);
    mDefaultPermissions = static_cast<int>(cfg->getValue(DEFAULT_PERMISSIONS,
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
                          mIgnoreStrategy ? mIgnoreStrategy->mShortName
                          : DEFAULT_IGNORE_STRATEGY);

    serverConfig.write();
}

void PlayerRelationsManager::signalUpdate(const std::string &name)
{
    FOR_EACH (PlayerRelationListenersCIter, it, mListeners)
        (*it)->updatedPlayer(name);

    if (actorManager)
    {
        Being *const being = actorManager->findBeingByName(
            name, Being::PLAYER);

        if (being && being->getType() == Being::PLAYER)
            being->updateColors();
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
        unsigned int permissions =
            PlayerRelation::RELATION_PERMISSIONS[r->mRelation];

        switch (r->mRelation)
        {
            case PlayerRelation::NEUTRAL:
                permissions = mDefaultPermissions;
                break;

            case PlayerRelation::FRIEND:
                permissions |= mDefaultPermissions;  // widen
                break;

            case PlayerRelation::DISREGARDED:
            case PlayerRelation::IGNORED:
            case PlayerRelation::ERASED:
            case PlayerRelation::BLACKLISTED:
            case PlayerRelation::ENEMY2:
            default:
                permissions &= mDefaultPermissions;  // narrow
        }

        return permissions & flags;
    }
}

bool PlayerRelationsManager::hasPermission(const Being *const being,
                                           const unsigned int flags) const
{
    if (!being)
        return false;

    if (being->getType() == ActorSprite::PLAYER)
        return hasPermission(being->getName(), flags) == flags;
    return true;
}

bool PlayerRelationsManager::hasPermission(const std::string &name,
                                           const unsigned int flags) const
{
    if (!actorManager)
        return false;

    const unsigned int rejections = flags
        & ~checkPermissionSilently(name, flags);
    const bool permitted = (rejections == 0);

    if (!permitted)
    {
        // execute `ignore' strategy, if possible
        if (mIgnoreStrategy)
        {
            Being *const b = actorManager->findBeingByName(
                name, ActorSprite::PLAYER);

            if (b && b->getType() == ActorSprite::PLAYER)
                mIgnoreStrategy->ignore(b, rejections);
        }
    }

    return permitted;
}

void PlayerRelationsManager::setRelation(const std::string &player_name,
                                         const PlayerRelation::Relation
                                         relation)
{
    if (!player_node || (relation != PlayerRelation::NEUTRAL
        && player_node->getName() == player_name))
    {
        return;
    }

    PlayerRelation *const r = mRelations[player_name];
    if (!r)
        mRelations[player_name] = new PlayerRelation(relation);
    else
        r->mRelation = relation;

    signalUpdate(player_name);
}

StringVect *PlayerRelationsManager::getPlayers() const
{
    StringVect *const retval = new StringVect();

    FOR_EACH (PlayerRelationsCIter, it, mRelations)
    {
        if (it->second)
            retval->push_back(it->first);
    }

    std::sort(retval->begin(), retval->end(), playersRelSorter);

    return retval;
}

StringVect *PlayerRelationsManager::getPlayersByRelation(
    const PlayerRelation::Relation rel) const
{
    StringVect *const retval = new StringVect();

    FOR_EACH (PlayerRelationsCIter, it, mRelations)
    {
        if (it->second && it->second->mRelation == rel)
            retval->push_back(it->first);
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


PlayerRelation::Relation PlayerRelationsManager::getRelation(
    const std::string &name) const
{
    const std::map<std::string, PlayerRelation *>::const_iterator
        it = mRelations.find(name);
    if (it != mRelations.end())
        return (*it).second->mRelation;

    return PlayerRelation::NEUTRAL;
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

    const PlayerRelation::Relation relation = getRelation(name);

    if (relation == PlayerRelation::IGNORED
        || relation == PlayerRelation::DISREGARDED
        || relation == PlayerRelation::BLACKLISTED
        || relation == PlayerRelation::ERASED)
    {
        return;
    }
    else
    {
        player_relations.setRelation(name, PlayerRelation::BLACKLISTED);
    }
}

bool PlayerRelationsManager::checkBadRelation(const std::string &name) const
{
    if (name.empty())
        return true;

    const PlayerRelation::Relation relation = getRelation(name);

    if (relation == PlayerRelation::IGNORED
        || relation == PlayerRelation::DISREGARDED
        || relation == PlayerRelation::BLACKLISTED
        || relation == PlayerRelation::ERASED
        || relation == PlayerRelation::ENEMY2)
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

    void ignore(Being *const being,
                const unsigned int flags A_UNUSED) const override final
    {
        if (!being)
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

    void ignore(Being *const being,
                const unsigned int flags A_UNUSED) const override final
    {
        if (!being)
            return;

        logger->log("ignoring: " + being->getName());
        being->flashName(200);
    }
};

class PIS_emote final : public PlayerIgnoreStrategy
{
public:
    PIS_emote(const uint8_t emote_nr, const std::string &description,
              const std::string &shortname) :
        PlayerIgnoreStrategy(),
        mEmotion(emote_nr)
    {
        mDescription = description;
        mShortName = shortname;
    }

    void ignore(Being *const being,
                const unsigned int flags A_UNUSED) const override final
    {
        if (!being)
            return;

        being->setEmote(mEmotion, IGNORE_EMOTE_TIME);
    }
    uint8_t mEmotion;
};

std::vector<PlayerIgnoreStrategy *> *
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
    if (!being)
        return false;
    if (being->getGoodStatus() != -1)
        return (being->getGoodStatus() == 1);

    const std::string name = being->getName();
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

    if (name.substr(0, 1) == " " || lastChar == " " || lastChar == "."
        || name.find("  ") != std::string::npos)
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

PlayerRelationsManager player_relations;
