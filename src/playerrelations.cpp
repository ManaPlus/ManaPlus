/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "playerrelations.h"

#include <algorithm>

#include "actorspritemanager.h"
#include "being.h"
#include "configuration.h"
#include "graphics.h"
#include "localplayer.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

#define PLAYER_IGNORE_STRATEGY_NOP "nop"
#define PLAYER_IGNORE_STRATEGY_EMOTE0 "emote0"
#define DEFAULT_IGNORE_STRATEGY PLAYER_IGNORE_STRATEGY_EMOTE0

#define NAME "name" // constant for xml serialisation
#define RELATION "relation" // constant for xml serialisation

#define IGNORE_EMOTE_TIME 100

class SortPlayersFunctor
{
    public:
        bool operator() (const std::string &str1, const std::string &str2)
        {
            std::string s1 = str1;
            std::string s2 = str2;
            toLower(s1);
            toLower(s2);
            if (s1 == s2)
                return str1 < str2;
            return s1 < s2;
        }
} playersSorter;

// (De)serialisation class
class PlayerConfSerialiser :
    public ConfigurationListManager<std::pair<std::string, PlayerRelation *>,
        std::map<std::string, PlayerRelation *> *>
{
    virtual ConfigurationObject *writeConfigItem(
        std::pair<std::string, PlayerRelation *> value,
        ConfigurationObject *cobj)
    {
        if (!cobj || !value.second)
            return nullptr;
        cobj->setValue(NAME, value.first);
        cobj->setValue(RELATION, toString(
                static_cast<int>(value.second->mRelation)));

        return cobj;
    }

    virtual std::map<std::string, PlayerRelation *> *
    readConfigItem(ConfigurationObject *cobj,
                   std::map<std::string, PlayerRelation *> *container)
    {
        if (!cobj)
            return container;
        std::string name = cobj->getValue(NAME, "");
        if (name.empty())
            return container;

        if (!(*container)[name])
        {
            int v = cobj->getValueInt(RELATION,
                static_cast<int>(PlayerRelation::NEUTRAL));

            (*container)[name] = new PlayerRelation(
                static_cast<PlayerRelation::Relation>(v));
        }
        // otherwise ignore the duplicate entry

        return container;
    }
};

static PlayerConfSerialiser player_conf_serialiser; // stateless singleton

const unsigned int PlayerRelation::RELATION_PERMISSIONS[RELATIONS_NR] = {
    /* NEUTRAL */     0, // we always fall back to the defaults anyway
    /* FRIEND  */     EMOTE | SPEECH_FLOAT | SPEECH_LOG | WHISPER | TRADE,
    /* DISREGARDED*/  EMOTE | SPEECH_FLOAT,
    /* IGNORED */     0,
    /* ERASED */      INVISIBLE,
    /* BLACKLISTED */ SPEECH_LOG | WHISPER,
    /* ENEMY2 */      EMOTE | SPEECH_FLOAT | SPEECH_LOG | WHISPER | TRADE
};

PlayerRelation::PlayerRelation(Relation relation)
{
    mRelation = relation;
}

PlayerRelationsManager::PlayerRelationsManager() :
    mPersistIgnores(false),
    mDefaultPermissions(PlayerRelation::DEFAULT),
    mIgnoreStrategy(0)
{
}

PlayerRelationsManager::~PlayerRelationsManager()
{
    delete_all(mIgnoreStrategies);

    for (std::map<std::string,
         PlayerRelation *>::const_iterator it = mRelations.begin();
         it != mRelations.end(); ++it)
    {
        delete it->second;
    }
}

void PlayerRelationsManager::clear()
{
    std::vector<std::string> *names = getPlayers();
    for (std::vector<std::string>::const_iterator
             it = names->begin(); it != names->end(); ++it)
    {
        removePlayer(*it);
    }
    delete names;
    names = 0;
}

#define PERSIST_IGNORE_LIST "persistent-player-list"
#define PLAYER_IGNORE_STRATEGY "player-ignore-strategy"
#define DEFAULT_PERMISSIONS "default-player-permissions"

int PlayerRelationsManager::getPlayerIgnoreStrategyIndex(
    const std::string &name)
{
    std::vector<PlayerIgnoreStrategy *> *strategies
        = getPlayerIgnoreStrategies();

    if (!strategies)
        return -1;

    for (unsigned int i = 0; i < strategies->size(); i++)
    {
        if ((*strategies)[i]->mShortName == name)
            return i;
    }

    return -1;
}

void PlayerRelationsManager::load(bool oldConfig)
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

    std::string ignore_strategy_name = cfg->getValue(PLAYER_IGNORE_STRATEGY,
                                                     DEFAULT_IGNORE_STRATEGY);
    int ignore_strategy_index = getPlayerIgnoreStrategyIndex(
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
        clear(); // Yes, we still keep them around in the config file
                 // until the next update.
    }

    for (std::list<PlayerRelationsListener *>::const_iterator
         it = mListeners.begin(); it != mListeners.end(); ++it)
    {
        (*it)->updateAll();
    }
}

void PlayerRelationsManager::store()
{
    serverConfig.setList<std::map<std::string,
        PlayerRelation *>::const_iterator,
        std::pair<std::string, PlayerRelation *>,
        std::map<std::string, PlayerRelation *> *>
        ("player",
         mRelations.begin(), mRelations.end(),
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
    for (std::list<PlayerRelationsListener *>::const_iterator
         it = mListeners.begin(); it != mListeners.end(); ++it)
    {
        (*it)->updatedPlayer(name);
    }

    if (actorSpriteManager)
    {
        Being* being = actorSpriteManager->findBeingByName(
            name, Being::PLAYER);

        if (being && being->getType() == Being::PLAYER)
            being->updateColors();
    }
}

unsigned int PlayerRelationsManager::checkPermissionSilently(
    const std::string &player_name, unsigned int flags)
{
    PlayerRelation *r = mRelations[player_name];
    if (!r)
    {
        return mDefaultPermissions & flags;
    }
    else
    {
        unsigned int permissions =
            PlayerRelation::RELATION_PERMISSIONS[r->mRelation];

        switch (r->mRelation)
        {
            case PlayerRelation::NEUTRAL:
                permissions = mDefaultPermissions;
                break;

            case PlayerRelation::FRIEND:
                permissions |= mDefaultPermissions; // widen
                break;

            case PlayerRelation::DISREGARDED:
            case PlayerRelation::IGNORED:
            case PlayerRelation::ERASED:
            case PlayerRelation::BLACKLISTED:
            default:
                permissions &= mDefaultPermissions; // narrow
        }

        return permissions & flags;
    }
}

bool PlayerRelationsManager::hasPermission(Being *being, unsigned int flags)
{
    if (!being)
        return false;

    if (being->getType() == ActorSprite::PLAYER)
        return hasPermission(being->getName(), flags) == flags;
    return true;
}

bool PlayerRelationsManager::hasPermission(const std::string &name,
                                           unsigned int flags)
{
    if (!actorSpriteManager)
        return false;

    unsigned int rejections = flags & ~checkPermissionSilently(name, flags);
    bool permitted = (rejections == 0);

    if (!permitted)
    {
        // execute `ignore' strategy, if possible
        if (mIgnoreStrategy)
        {
            Being *b = actorSpriteManager->findBeingByName(
                name, ActorSprite::PLAYER);

            if (b && b->getType() == ActorSprite::PLAYER)
                mIgnoreStrategy->ignore(b, rejections);
        }
    }

    return permitted;
}

void PlayerRelationsManager::setRelation(const std::string &player_name,
                                         PlayerRelation::Relation relation)
{
    if (!player_node || (relation != PlayerRelation::NEUTRAL
        && player_node->getName() == player_name))
    {
        return;
    }

    PlayerRelation *r = mRelations[player_name];
    if (!r)
        mRelations[player_name] = new PlayerRelation(relation);
    else
        r->mRelation = relation;

    signalUpdate(player_name);
}

std::vector<std::string> * PlayerRelationsManager::getPlayers()
{
    std::vector<std::string> *retval = new std::vector<std::string>();

    for (std::map<std::string,
         PlayerRelation *>::const_iterator it = mRelations.begin();
         it != mRelations.end(); ++it)
    {
        if (it->second)
            retval->push_back(it->first);
    }

    sort(retval->begin(), retval->end(), playersSorter);

    return retval;
}

void PlayerRelationsManager::removePlayer(const std::string &name)
{
    if (mRelations[name])
        delete mRelations[name];

    mRelations.erase(name);

    signalUpdate(name);
}


PlayerRelation::Relation PlayerRelationsManager::getRelation(
    const std::string &name)
{
    if (mRelations[name])
        return mRelations[name]->mRelation;

    return PlayerRelation::NEUTRAL;
}

////////////////////////////////////////
// defaults

unsigned int PlayerRelationsManager::getDefault() const
{
    return mDefaultPermissions;
}

void PlayerRelationsManager::setDefault(unsigned int permissions)
{
    mDefaultPermissions = permissions;

    store();
    signalUpdate("");
}

void PlayerRelationsManager::ignoreTrade(std::string name)
{
    if (name.empty())
        return;

    PlayerRelation::Relation relation = getRelation(name);

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

bool PlayerRelationsManager::checkBadRelation(std::string name)
{
    if (name.empty())
        return true;

    PlayerRelation::Relation relation = getRelation(name);

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


class PIS_nothing : public PlayerIgnoreStrategy
{
public:
    PIS_nothing()
    {
        mDescription = _("Completely ignore");
        mShortName = PLAYER_IGNORE_STRATEGY_NOP;
    }

    virtual void ignore(Being *being A_UNUSED, unsigned int flags A_UNUSED)
    {
    }
};

class PIS_dotdotdot : public PlayerIgnoreStrategy
{
public:
    PIS_dotdotdot()
    {
        mDescription = _("Print '...'");
        mShortName = "dotdotdot";
    }

    virtual void ignore(Being *being, unsigned int flags A_UNUSED)
    {
        if (!being)
            return;

        logger->log("ignoring: " + being->getName());
        being->setSpeech("...");
    }
};


class PIS_blinkname : public PlayerIgnoreStrategy
{
public:
    PIS_blinkname()
    {
        mDescription = _("Blink name");
        mShortName = "blinkname";
    }

    virtual void ignore(Being *being, unsigned int flags A_UNUSED)
    {
        if (!being)
            return;

        logger->log("ignoring: " + being->getName());
        being->flashName(200);
    }
};

class PIS_emote : public PlayerIgnoreStrategy
{
public:
    PIS_emote(Uint8 emote_nr, const std::string &description,
              const std::string &shortname) :
        mEmotion(emote_nr)
    {
        mDescription = description;
        mShortName = shortname;
    }

    virtual void ignore(Being *being, unsigned int flags A_UNUSED)
    {
        if (!being)
            return;

        being->setEmote(mEmotion, IGNORE_EMOTE_TIME);
    }
    Uint8 mEmotion;
};



std::vector<PlayerIgnoreStrategy *> *
PlayerRelationsManager::getPlayerIgnoreStrategies()
{
    if (mIgnoreStrategies.empty())
    {
        // not initialised yet?
        mIgnoreStrategies.push_back(new PIS_emote(FIRST_IGNORE_EMOTE,
                                    _("Floating '...' bubble"),
                                    PLAYER_IGNORE_STRATEGY_EMOTE0));
        mIgnoreStrategies.push_back(new PIS_emote(FIRST_IGNORE_EMOTE + 1,
                                    _("Floating bubble"),
                                    "emote1"));
        mIgnoreStrategies.push_back(new PIS_nothing());
        mIgnoreStrategies.push_back(new PIS_dotdotdot());
        mIgnoreStrategies.push_back(new PIS_blinkname());
    }
    return &mIgnoreStrategies;
}

bool PlayerRelationsManager::isGoodName(std::string name)
{
    bool status(false);

    const int size = name.size();

    if (size < 3 || mRelations[name])
        return true;

    status = checkName(name);

    return status;
}

bool PlayerRelationsManager::isGoodName(Being *being)
{
    bool status(false);

    if (!being)
        return false;
    if (being->getGoodStatus() != -1)
        return (being->getGoodStatus() == 1);

    const std::string name = being->getName();
    const int size = name.size();

    if (size < 3 || mRelations[name])
        return true;

    status = checkName(name);

    being->setGoodStatus(status ? 1 : 0);
    return status;
}

bool PlayerRelationsManager::checkName(const std::string &name) const
{
    const int size = name.size();
    std::string check = config.getStringValue("unsecureChars");

    if (name.substr(0, 1) == " " || name.substr(size - 1, 1) == " ")
        return false;
    else if (check.empty())
        return true;
    else if (name.find_first_of(check) != std::string::npos)
        return false;
    else
        return true;
}

PlayerRelationsManager player_relations;
