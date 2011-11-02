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

#include "being.h"

#include "actorspritemanager.h"
#include "animatedsprite.h"
#include "client.h"
#include "configuration.h"
#include "effectmanager.h"
#include "graphics.h"
#include "guild.h"
#include "item.h"
#include "localplayer.h"
#include "logger.h"
#include "map.h"
#include "particle.h"
#include "party.h"
#include "playerrelations.h"
#include "simpleanimation.h"
#include "sound.h"
#include "sprite.h"
#include "text.h"
#include "statuseffect.h"

#include "gui/buydialog.h"
#include "gui/buyselldialog.h"
#include "gui/equipmentwindow.h"
#include "gui/gui.h"
#include "gui/npcdialog.h"
#include "gui/npcpostdialog.h"
#include "gui/selldialog.h"
#include "gui/socialwindow.h"
#include "gui/speechbubble.h"
#include "gui/theme.h"
#include "gui/sdlfont.h"
#include "gui/userpalette.h"

#include "net/charhandler.h"
#include "net/gamehandler.h"
#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/playerhandler.h"

#include "resources/beinginfo.h"
#include "resources/colordb.h"
#include "resources/emotedb.h"
#include "resources/image.h"
#include "resources/itemdb.h"
#include "resources/iteminfo.h"
#include "resources/monsterdb.h"
#include "resources/npcdb.h"
#include "resources/resourcemanager.h"

#include "gui/widgets/chattab.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"

#include <cmath>

#include "debug.h"

#define CACHE_SIZE 50

static const int DEFAULT_BEING_WIDTH = 32;
static const int DEFAULT_BEING_HEIGHT = 32;

class BeingCacheEntry
{
    public:
        BeingCacheEntry(int id):
            mId(id),
            mName(""),
            mPartyName(""),
            mGuildName(""),
            mLevel(0),
            mPvpRank(0),
            mTime(0),
            mIp(""),
            mIsAdvanced(false),
            mFlags(0)
        {
        }

        int getId() const
        { return mId; }

        /**
         * Returns the name of the being.
         */
        const std::string &getName() const
        { return mName; }

        /**
         * Sets the name for the being.
         *
         * @param name The name that should appear.
         */
        void setName(const std::string &name)
        { mName = name; }

        /**
         * Following are set from the server (mainly for players)
         */
        void setPartyName(const std::string &name)
        { mPartyName = name; }

        void setGuildName(const std::string &name)
        { mGuildName = name; }

        const std::string &getPartyName() const
        { return mPartyName; }

        const std::string &getGuildName() const
        { return mGuildName; }

        void setLevel(int n)
        { mLevel = n; }

        int getLevel() const
        { return mLevel; }

        void setTime(int n)
        { mTime = n; }

        int getTime() const
        { return mTime; }

        unsigned getPvpRank() const
        { return mPvpRank; }

        void setPvpRank(int r)
        { mPvpRank = r; }

        std::string getIp() const
        { return mIp; }

        void setIp(std::string ip)
        { mIp = ip; }

        bool isAdvanced() const
        { return mIsAdvanced; }

        void setAdvanced(bool a)
        { mIsAdvanced = a; }

        int getFlags() const
        { return mFlags; }

        void setFlags(int flags)
        { mFlags = flags; }

    protected:
        int mId;                        /**< Unique sprite id */
        std::string mName;              /**< Name of character */
        std::string mPartyName;
        std::string mGuildName;
        int mLevel;
        unsigned int mPvpRank;
        int mTime;
        std::string mIp;
        bool mIsAdvanced;
        int mFlags;
};


int Being::mNumberOfHairstyles = 1;

int Being::mUpdateConfigTime = 0;
unsigned int Being::mConfLineLim = 0;
int Being::mSpeechType = 0;
bool Being::mHighlightMapPortals = false;
bool Being::mHighlightMonsterAttackRange = false;
bool Being::mLowTraffic = true;
bool Being::mDrawHotKeys = true;
bool Being::mShowBattleEvents = false;
bool Being::mShowMobHP = false;
bool Being::mShowOwnHP = false;
bool Being::mShowGender = false;
bool Being::mShowLevel = false;
bool Being::mShowPlayersStatus = false;
bool Being::mEnableReorderSprites = true;

std::list<BeingCacheEntry*> beingInfoCache;


// TODO: mWalkTime used by eAthena only
Being::Being(int id, Type type, Uint16 subtype, Map *map):
    ActorSprite(id),
    mInfo(BeingInfo::unknown),
    mActionTime(0),
    mEmotion(0), mEmotionTime(0),
    mSpeechTime(0),
    mAttackType(1),
    mAttackSpeed(350),
    mAction(STAND),
    mSubType(0xFFFF),
    mDirection(DOWN),
    mDirectionDelayed(0),
    mSpriteDirection(DIRECTION_DOWN),
    mDispName(0),
    mShowName(false),
    mEquippedWeapon(NULL),
    mText(0),
    mLevel(0),
    mGender(GENDER_UNSPECIFIED),
    mParty(0),
    mIsGM(false),
    mAttackRange(1),
    mType(type),
    mX(0), mY(0),
    mDamageTaken(0),
    mHP(0), mMaxHP(0),
    mDistance(0),
    mIsReachable(REACH_UNKNOWN),
    mGoodStatus(-1),
    mErased(false),
    mEnemy(false),
    mIp(""),
    mAttackDelay(0),
    mMinHit(0),
    mMaxHit(0),
    mCriticalHit(0),
    mPvpRank(0),
    mComment(""),
    mGotComment(false),
    mAdvanced(false),
    mShop(false),
    mAway(false),
    mInactive(false)
{
    mSpriteRemap = new int[20];
    mSpriteHide = new int[20];

    for (int f = 0; f < 20; f ++)
    {
        mSpriteRemap[f] = f;
        mSpriteHide[f] = 0;
    }

    setMap(map);
    setSubtype(subtype);

    mSpeechBubble = new SpeechBubble;

    mWalkSpeed = Net::getPlayerHandler()->getDefaultWalkSpeed();

    if (mType == PLAYER)
        mShowName = config.getBoolValue("visiblenames");
    else if (mType != NPC)
        mGotComment = true;

    config.addListener("visiblenames", this);

    mEnableReorderSprites = config.getBoolValue("enableReorderSprites");

    if (mType == NPC)
        setShowName(true);
    else
        setShowName(mShowName);

    updateColors();
    resetCounters();
}

Being::~Being()
{
    config.removeListener("visiblenames", this);

    delete[] mSpriteRemap;
    mSpriteRemap = 0;
    delete[] mSpriteHide;
    mSpriteHide = 0;

    delete mSpeechBubble;
    mSpeechBubble = 0;
    delete mDispName;
    mDispName = 0;
    delete mText;
    mText = 0;
}

void Being::setSubtype(Uint16 subtype)
{
    if (!mInfo)
        return;

    if (subtype == mSubType)
        return;

    mSubType = subtype;

    if (mType == MONSTER)
    {
        mInfo = MonsterDB::get(mSubType);
        if (mInfo)
        {
            setName(mInfo->getName());
            setupSpriteDisplay(mInfo->getDisplay());
        }
    }
    else if (mType == NPC)
    {
        mInfo = NPCDB::get(mSubType);
        if (mInfo)
            setupSpriteDisplay(mInfo->getDisplay(), false);
    }
    else if (mType == PLAYER)
    {
        int id = -100 - subtype;

        // Prevent showing errors when sprite doesn't exist
        if (!ItemDB::exists(id))
            id = -100;

        setSprite(Net::getCharHandler()->baseSprite(), id);
    }
}

ActorSprite::TargetCursorSize Being::getTargetCursorSize() const
{
    if (!mInfo)
        return ActorSprite::TC_SMALL;

    return mInfo->getTargetCursorSize();
}

int Being::getTargetOffsetX() const
{
    if (!mInfo)
        return 0;

    return mInfo->getTargetOffsetX();
}

int Being::getTargetOffsetY() const
{
    if (!mInfo)
        return 0;

    return mInfo->getTargetOffsetY();
}

unsigned char Being::getWalkMask() const
{
    if (!mInfo)
        return 0;

    return mInfo->getWalkMask();
}

Map::BlockType Being::getBlockType() const
{
    if (!mInfo)
        return Map::BLOCKTYPE_NONE;

    return mInfo->getBlockType();
}

void Being::setPosition(const Vector &pos)
{
    Actor::setPosition(pos);

    updateCoords();

    if (mText)
    {
        mText->adviseXY(static_cast<int>(pos.x), static_cast<int>(pos.y)
            - getHeight() - mText->getHeight() - 6);
    }
}

void Being::setDestination(int dstX, int dstY)
{
    // We can't calculate anything without a map anyway.
    if (!mMap)
        return;

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        setPath(mMap->findPath(mX, mY, dstX, dstY, getWalkMask()));
        return;
    }

    // Don't handle flawed destinations from server...
    if (dstX == 0 || dstY == 0)
        return;

    // If the destination is unwalkable, don't bother trying to get there
    if (!mMap->getWalk(dstX / 32, dstY / 32))
        return;

    Position dest = mMap->checkNodeOffsets(getCollisionRadius(), getWalkMask(),
                                           dstX, dstY);
    Path thisPath = mMap->findPixelPath(static_cast<int>(mPos.x),
        static_cast<int>(mPos.y), dest.x, dest.y,
        static_cast<int>(getCollisionRadius()),
        static_cast<int>(getWalkMask()));

    if (thisPath.empty())
    {
        // If there is no path but the destination is on the same walkable tile,
        // we accept it.
        if (static_cast<int>(mPos.x) / 32 == dest.x / 32
            && static_cast<int>(mPos.y) / 32 == dest.y / 32)
        {
            mDest.x = static_cast<float>(dest.x);
            mDest.y = static_cast<float>(dest.y);
        }
        setPath(Path());
        return;
    }

    // The destination is valid, so we set it.
    mDest.x = static_cast<float>(dest.x);
    mDest.y = static_cast<float>(dest.y);

    setPath(thisPath);
}

void Being::clearPath()
{
    mPath.clear();
}

void Being::setPath(const Path &path)
{
    mPath = path;
    if (mPath.empty())
        return;

#ifdef MANASERV_SUPPORT
    if ((Net::getNetworkType() != ServerInfo::MANASERV) &&
        mAction != MOVE && mAction != DEAD)
#else
    if (mAction != MOVE && mAction != DEAD)
#endif
    {
        nextTile();
        mActionTime = tick_time;
    }
}

void Being::setSpeech(const std::string &text, int time)
{
    if (!userPalette)
        return;

    // Remove colors
    mSpeech = removeColors(text);

    // Trim whitespace
    trim(mSpeech);

    unsigned int lineLim = mConfLineLim;
    if (lineLim > 0 && mSpeech.length() > lineLim)
        mSpeech = mSpeech.substr(0, lineLim);

    trim(mSpeech);
    if (mSpeech.empty())
        return;

    if (!time && mSpeech.size() < 200)
        time = SPEECH_TIME - 300 + (3 * mSpeech.size());

    if (time < SPEECH_MIN_TIME)
        time = SPEECH_MIN_TIME;

    // Check for links
    std::string::size_type start = mSpeech.find('[');
    std::string::size_type e = mSpeech.find(']', start);

    while (start != std::string::npos && e != std::string::npos)
    {
        // Catch multiple embeds and ignore them so it doesn't crash the client.
        while ((mSpeech.find('[', start + 1) != std::string::npos) &&
               (mSpeech.find('[', start + 1) < e))
        {
            start = mSpeech.find('[', start + 1);
        }

        std::string::size_type position = mSpeech.find('|');
        if (mSpeech[start + 1] == '@' && mSpeech[start + 2] == '@')
        {
            mSpeech.erase(e, 1);
            mSpeech.erase(start, (position - start) + 1);
        }
        position = mSpeech.find('@');

        while (position != std::string::npos)
        {
            mSpeech.erase(position, 2);
            position = mSpeech.find('@');
        }

        start = mSpeech.find('[', start + 1);
        e = mSpeech.find(']', start);
    }

    if (!mSpeech.empty())
        mSpeechTime = time <= SPEECH_MAX_TIME ? time : SPEECH_MAX_TIME;

    const int speech = mSpeechType;
    if (speech == TEXT_OVERHEAD && userPalette)
    {
        delete mText;

        mText = new Text(mSpeech,
                         getPixelX(), getPixelY() - getHeight(),
                         gcn::Graphics::CENTER,
                         &userPalette->getColor(UserPalette::PARTICLE),
                         true);
    }
}

void Being::takeDamage(Being *attacker, int amount, AttackType type)
{
    if (!userPalette || !attacker)
        return;

    gcn::Font *font = 0;
    std::string damage = amount ? toString(amount) : type == FLEE ?
            _("dodge") : _("miss");
    const gcn::Color *color;

    if (gui)
        font = gui->getInfoParticleFont();

    // Selecting the right color
    if (type == CRITICAL || type == FLEE)
    {
        if (type == CRITICAL)
            attacker->setCriticalHit(amount);

        if (attacker == player_node)
        {
            color = &userPalette->getColor(
                UserPalette::HIT_LOCAL_PLAYER_CRITICAL);
        }
        else
        {
            color = &userPalette->getColor(UserPalette::HIT_CRITICAL);
        }
    }
    else if (!amount)
    {
        if (attacker == player_node)
        {
            // This is intended to be the wrong direction to visually
            // differentiate between hits and misses
            color = &userPalette->getColor(UserPalette::HIT_LOCAL_PLAYER_MISS);
        }
        else
        {
            color = &userPalette->getColor(UserPalette::MISS);
        }
    }
    else if (mType == MONSTER)
    {
        if (attacker == player_node)
        {
            color = &userPalette->getColor(
                UserPalette::HIT_LOCAL_PLAYER_MONSTER);
        }
        else
        {
            color = &userPalette->getColor(
                UserPalette::HIT_PLAYER_MONSTER);
        }
    }
    else if (mType == PLAYER && attacker != player_node
             && this == player_node)
    {
        // here player was attacked by other player. mark him as enemy.
        color = &userPalette->getColor(UserPalette::HIT_PLAYER_PLAYER);
        attacker->setEnemy(true);
        attacker->updateColors();
    }
    else
    {
        color = &userPalette->getColor(UserPalette::HIT_MONSTER_PLAYER);
    }

    if (chatWindow && mShowBattleEvents)
    {
        if (this == player_node)
        {
            if (attacker->mType == PLAYER || amount)
            {
                chatWindow->battleChatLog(strprintf("%s : Hit you  -%d",
                    attacker->getName().c_str(), amount), BY_OTHER);
            }
        }
        else if (attacker == player_node && amount)
        {
            chatWindow->battleChatLog(strprintf("%s : You hit %s -%d",
                attacker->getName().c_str(), getName().c_str(), amount),
                BY_PLAYER);
        }
    }
    if (font && particleEngine)
    {
        // Show damage number
        particleEngine->addTextSplashEffect(damage,
            getPixelX(), getPixelY() - 16, color, font, true);
    }

    attacker->updateHit(amount);

    if (amount > 0)
    {
        if (player_node && player_node == this)
            player_node->setLastHitFrom(attacker->getName());

        mDamageTaken += amount;
        if (mInfo)
        {
            sound.playSfx(mInfo->getSound(SOUND_EVENT_HURT),
                attacker->getTileX(), attacker->getTileY());

            if (!mInfo->isStaticMaxHP())
            {
                if (!mHP && mInfo->getMaxHP() < mDamageTaken)
                    mInfo->setMaxHP(mDamageTaken);
            }
        }
        if (mHP && isAlive())
        {
            mHP -= amount;
            if (mHP < 0)
                mHP = 0;
        }

        if (mType == MONSTER)
            updateName();
        else if (mType == PLAYER && socialWindow && getName() != "")
            socialWindow->updateAvatar(getName());

        if (effectManager)
        {
            // Init the particle effect path based on current
            // weapon or default.
            int hitEffectId = 0;
            const ItemInfo *attackerWeapon = attacker->getEquippedWeapon();
            if (attacker->mType == PLAYER && attackerWeapon)
            {
                if (type != CRITICAL)
                    hitEffectId = attackerWeapon->getHitEffectId();
                else
                    hitEffectId = attackerWeapon->getCriticalHitEffectId();
            }
            else
            {
                if (type != CRITICAL)
                    hitEffectId = paths.getIntValue("hitEffectId");
                else
                    hitEffectId = paths.getIntValue("criticalHitEffectId");
            }
            effectManager->trigger(hitEffectId, this);
        }
    }
}

void Being::handleAttack(Being *victim, int damage,
                         AttackType type A_UNUSED)
{
    if (!victim || !mInfo)
        return;

    if (this != player_node)
        setAction(Being::ATTACK, 1);

    if (mType == PLAYER && mEquippedWeapon)
        fireMissile(victim, mEquippedWeapon->getMissileParticleFile());
    else if (mInfo->getAttack(mAttackType))
        fireMissile(victim, mInfo->getAttack(mAttackType)->missileParticle);

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        reset();
        mActionTime = tick_time;
    }

    if (this != player_node)
    {
        Uint8 dir = calcDirection(victim->getTileX(), victim->getTileY());
        if (dir)
            setDirection(dir);
    }
    if (damage && victim->mType == PLAYER && victim->mAction == SIT)
        victim->setAction(STAND);

    sound.playSfx(mInfo->getSound((damage > 0) ?
                  SOUND_EVENT_HIT : SOUND_EVENT_MISS), mX, mY);
}

void Being::setName(const std::string &name)
{
    if (mType == NPC)
    {
        mName = name.substr(0, name.find('#', 0));
        showName();
    }
    else
    {
        mName = name;

        if (mType == PLAYER && getShowName())
            showName();
    }
}

void Being::setShowName(bool doShowName)
{
    if (mShowName == doShowName)
        return;

    mShowName = doShowName;

    if (doShowName)
    {
        showName();
    }
    else
    {
        delete mDispName;
        mDispName = 0;
    }
}

void Being::setGuildName(const std::string &name)
{
    mGuildName = name;
}

void Being::setGuildPos(const std::string &pos A_UNUSED)
{
}

void Being::addGuild(Guild *guild)
{
    if (!guild)
        return;

    mGuilds[guild->getId()] = guild;

    if (this == player_node && socialWindow)
        socialWindow->addTab(guild);
}

void Being::removeGuild(int id)
{
    if (this == player_node && socialWindow)
        socialWindow->removeTab(mGuilds[id]);

    if (mGuilds[id])
        mGuilds[id]->removeMember(getName());
    mGuilds.erase(id);
}

Guild *Being::getGuild(const std::string &guildName) const
{
    std::map<int, Guild*>::const_iterator itr, itr_end = mGuilds.end();
    for (itr = mGuilds.begin(); itr != itr_end; ++itr)
    {
        Guild *guild = itr->second;
        if (guild && guild->getName() == guildName)
            return guild;
    }

    return 0;
}

Guild *Being::getGuild(int id) const
{
    std::map<int, Guild*>::const_iterator itr;
    itr = mGuilds.find(id);
    if (itr != mGuilds.end())
        return itr->second;

    return 0;
}

Guild *Being::getGuild() const
{
    std::map<int, Guild*>::const_iterator itr;
    itr = mGuilds.begin();
    if (itr != mGuilds.end())
        return itr->second;

    return 0;
}

void Being::clearGuilds()
{
    std::map<int, Guild*>::const_iterator itr, itr_end = mGuilds.end();
    for (itr = mGuilds.begin(); itr != itr_end; ++itr)
    {
        Guild *guild = itr->second;

        if (guild)
        {
            if (this == player_node && socialWindow)
                socialWindow->removeTab(guild);

            guild->removeMember(mId);
        }
    }

    mGuilds.clear();
}

void Being::setParty(Party *party)
{
    if (party == mParty)
        return;

    Party *old = mParty;
    mParty = party;

    if (old)
        old->removeMember(mId);

    if (party)
        party->addMember(mId, mName);

    updateColors();

    if (this == player_node && socialWindow)
    {
        if (old)
            socialWindow->removeTab(old);

        if (party)
            socialWindow->addTab(party);
    }
}

void Being::updateGuild()
{
    if (!player_node)
        return;

    Guild *guild = player_node->getGuild();
    if (!guild)
    {
        clearGuilds();
        updateColors();
        return;
    }
    if (guild->getMember(getName()))
    {
        setGuild(guild);
        if (!guild->getName().empty())
            mGuildName = guild->getName();
    }
    updateColors();
}

void Being::setGuild(Guild *guild)
{
    Guild *old = getGuild();
    if (guild == old)
        return;

    clearGuilds();
    addGuild(guild);

    if (old)
        old->removeMember(mName);

//    if (guild)
//        guild->addMember(mId, mName);

    updateColors();

    if (this == player_node && socialWindow)
    {
        if (old)
            socialWindow->removeTab(old);

        if (guild)
            socialWindow->addTab(guild);
    }
}

void Being::fireMissile(Being *victim, const std::string &particle)
{
    if (!victim || particle.empty() || !particleEngine)
        return;

    Particle *target = particleEngine->createChild();

    if (!target)
        return;

    Particle *missile = target->addEffect(particle, getPixelX(), getPixelY());

    if (missile)
    {
        target->moveBy(Vector(0.0f, 0.0f, 32.0f));
        target->setLifetime(1000);
        victim->controlParticle(target);

        missile->setDestination(target, 7, 0);
        missile->setDieDistance(8);
        missile->setLifetime(900);
    }

}

void Being::setAction(Action action, int attackType A_UNUSED)
{
    std::string currentAction = SpriteAction::INVALID;

    switch (action)
    {
        case MOVE:
            if (mInfo)
                sound.playSfx(mInfo->getSound(SOUND_EVENT_MOVE), mX, mY);
            currentAction = SpriteAction::MOVE;
            // Note: When adding a run action,
            // Differentiate walk and run with action name,
            // while using only the ACTION_MOVE.
            break;
        case SIT:
            currentAction = SpriteAction::SIT;
            if (mInfo)
                sound.playSfx(mInfo->getSound(SOUND_EVENT_SIT), mX, mY);
            break;
        case ATTACK:
            if (mEquippedWeapon)
            {
                currentAction = mEquippedWeapon->getAttackAction();
                reset();
            }
            else
            {
                mAttackType = attackType;
                if (!mInfo || !mInfo->getAttack(attackType))
                    break;

                currentAction = mInfo->getAttack(attackType)->action;
                reset();

#ifdef MANASERV_SUPPORT
                if (Net::getNetworkType() == ServerInfo::MANASERV
                    && mInfo->getAttack(attackType))
                {
                    //attack particle effect
                    std::string particleEffect = mInfo->getAttack(attackType)
                                                 ->particleEffect;
                    if (!particleEffect.empty() && Particle::enabled)
                    {
                        int rotation = 0;
                        switch (mSpriteDirection)
                        {
                            case DIRECTION_DOWN: rotation = 0; break;
                            case DIRECTION_LEFT: rotation = 90; break;
                            case DIRECTION_UP: rotation = 180; break;
                            case DIRECTION_RIGHT: rotation = 270; break;
                            default: break;
                        }
                        ;
                        if (particleEngine)
                        {
                            Particle *p = particleEngine->addEffect(
                                particleEffect, 0, 0, rotation);
                            if (p)
                                controlParticle(p);
                        }
                    }
                }
#endif
            }

            break;
        case HURT:
            if (mInfo)
                sound.playSfx(mInfo->getSound(SOUND_EVENT_HURT), mX, mY);
            //currentAction = SpriteAction::HURT;// Buggy: makes the player stop
                                            // attacking and unable to attack
                                            // again until he moves.
                                            // TODO: fix this!
            break;
        case DEAD:
            currentAction = SpriteAction::DEAD;
            if (mInfo)
                sound.playSfx(mInfo->getSound(SOUND_EVENT_DIE), mX, mY);
            break;
        case STAND:
            currentAction = SpriteAction::STAND;
            break;
        case SPAWN:
            if (mInfo)
                sound.playSfx(mInfo->getSound(SOUND_EVENT_SPAWN), mX, mY);
            currentAction = SpriteAction::SPAWN;
            break;
        default:
            logger->log("Being::setAction unknown action: "
                + toString(static_cast<unsigned>(action)));
            break;
    }

    if (currentAction != SpriteAction::INVALID)
    {
        play(currentAction);
        mAction = action;
    }

    if (currentAction != SpriteAction::MOVE)
        mActionTime = tick_time;
}

void Being::setDirection(Uint8 direction)
{
    if (mDirection == direction)
        return;

    mDirection = direction;

    mDirectionDelayed = 0;

    // if the direction does not change much, keep the common component
    int mFaceDirection = mDirection & direction;
    if (!mFaceDirection)
        mFaceDirection = direction;

    SpriteDirection dir;
    if (mFaceDirection & UP)
    {
        if (mFaceDirection & LEFT)
            dir = DIRECTION_UPLEFT;
        else if (mFaceDirection & RIGHT)
            dir = DIRECTION_UPRIGHT;
        else
            dir = DIRECTION_UP;
    }
    else if (mFaceDirection & DOWN)
    {
        if (mFaceDirection & LEFT)
            dir = DIRECTION_DOWNLEFT;
        else if (mFaceDirection & RIGHT)
            dir = DIRECTION_DOWNRIGHT;
        else
            dir = DIRECTION_DOWN;
    }
    else if (mFaceDirection & RIGHT)
    {
        dir = DIRECTION_RIGHT;
    }
    else
    {
        dir = DIRECTION_LEFT;
    }
    mSpriteDirection = dir;

    CompoundSprite::setSpriteDirection(dir);
    recalcSpritesOrder();
}

Uint8 Being::calcDirection() const
{
    Uint8 dir = 0;
    if (mDest.x > mX)
        dir |= RIGHT;
    else if (mDest.x < mX)
        dir |= LEFT;
    if (mDest.y > mY)
        dir |= DOWN;
    else if (mDest.y < mY)
        dir |= UP;
    return dir;
}

Uint8 Being::calcDirection(int dstX, int dstY) const
{
    Uint8 dir = 0;
    if (dstX > mX)
        dir |= RIGHT;
    else if (dstX < mX)
        dir |= LEFT;
    if (dstY > mY)
        dir |= DOWN;
    else if (dstY < mY)
        dir |= UP;
    return dir;
}

void Being::nextTile()
{
    if (mPath.empty())
    {
        setAction(STAND);
        return;
    }

    Position pos = mPath.front();
    mPath.pop_front();

    Uint8 dir = calcDirection(pos.x, pos.y);
    if (dir)
        setDirection(static_cast<Uint8>(dir));

    if (!mMap || !mMap->getWalk(pos.x, pos.y, getWalkMask()))
    {
        setAction(STAND);
        return;
    }

    mX = pos.x;
    mY = pos.y;
    setAction(MOVE);
    mActionTime += static_cast<int>(mWalkSpeed.x / 10);
}

int Being::getCollisionRadius() const
{
    // FIXME: Get this from XML file
    return 16;
}

void Being::logic()
{
    // Reduce the time that speech is still displayed
    if (mSpeechTime > 0)
        mSpeechTime--;

    // Remove text and speechbubbles if speech boxes aren't being used
    if (mSpeechTime == 0 && mText)
    {
        delete mText;
        mText = 0;
    }

    int frameCount = static_cast<int>(getFrameCount());
#ifdef MANASERV_SUPPORT
    if ((Net::getNetworkType() == ServerInfo::MANASERV) && (mAction != DEAD))
    {
        const Vector dest = (mPath.empty()) ?
            mDest : Vector(static_cast<float>(mPath.front().x),
                           static_cast<float>(mPath.front().y));

        // This is a hack that stops NPCs from running off the map...
        if (mDest.x <= 0 && mDest.y <= 0)
            return;

        // The Vector representing the difference between current position
        // and the next destination path node.
        Vector dir = dest - mPos;

        const float nominalLength = dir.length();

        // When we've not reached our destination, move to it.
        if (nominalLength > 0.0f && !mWalkSpeed.isNull())
        {
            // The deplacement of a point along a vector is calculated
            // using the Unit Vector (â) multiplied by the point speed.
            // â = a / ||a|| (||a|| is the a length.)
            // Then, diff = (dir/||dir||) * speed.
            const Vector normalizedDir = dir.normalized();
            Vector diff(normalizedDir.x * mWalkSpeed.x,
                        normalizedDir.y * mWalkSpeed.y);

            // Test if we don't miss the destination by a move too far:
            if (diff.length() > nominalLength)
            {
                setPosition(mPos + dir);

                // Also, if the destination is reached, try to get the next
                // path point, if existing.
                if (!mPath.empty())
                    mPath.pop_front();
            }
            // Otherwise, go to it using the nominal speed.
            else
            {
                setPosition(mPos + diff);
            }

            if (mAction != MOVE)
                setAction(MOVE);

            // Update the player sprite direction.
            // N.B.: We only change this if the distance is more than one pixel.
            if (nominalLength > 1.0f)
            {
                int direction = 0;
                const float dx = std::abs(dir.x);
                float dy = std::abs(dir.y);

                // When not using mouse for the player, we slightly prefer
                // UP and DOWN position, especially when walking diagonally.
                if (player_node && this == player_node &&
                    !player_node->isPathSetByMouse())
                {
                    dy = dy + 2;
                }

                if (dx > dy)
                     direction |= (dir.x > 0) ? RIGHT : LEFT;
                else
                     direction |= (dir.y > 0) ? DOWN : UP;

                setDirection(static_cast<Uint8>(direction));
            }
        }
        else if (!mPath.empty())
        {
            // If the current path node has been reached,
            // remove it and go to the next one.
            mPath.pop_front();
        }
        else if (mAction == MOVE)
        {
            setAction(STAND);
        }
    }
    else
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        switch (mAction)
        {
            case STAND:
            case SIT:
            case DEAD:
            case HURT:
            case SPAWN:
            default:
               break;

            case MOVE:
            {
                if (getWalkSpeed().x && static_cast<int> ((static_cast<float>(
                    get_elapsed_time(mActionTime)) * static_cast<float>(
                    frameCount)) / static_cast<float>(getWalkSpeed().x))
                    >= frameCount)
                {
                    nextTile();
                }
                break;
            }

            case ATTACK:
            {
                std::string particleEffect("");

                if (!mActionTime)
                    break;

                int curFrame = 0;
                if (mAttackSpeed)
                {
                    curFrame = (get_elapsed_time(mActionTime) * frameCount)
                        / mAttackSpeed;
                }

                //attack particle effect
                if (mEquippedWeapon)
                {
                    particleEffect = mEquippedWeapon->getParticleEffect();

                    if (!particleEffect.empty() &&
                        findSameSubstring(particleEffect,
                        paths.getStringValue("particles")).empty())
                    {
                        particleEffect = paths.getStringValue("particles")
                                         + particleEffect;
                    }
                }
                else if (mInfo && mInfo->getAttack(mAttackType))
                {
                    particleEffect = mInfo->getAttack(mAttackType)
                        ->particleEffect;
                }

                if (particleEngine && !particleEffect.empty()
                    && Particle::enabled && curFrame == 1)
                {
                    int rotation = 0;

                    switch (mDirection)
                    {
                        case DOWN: rotation = 0; break;
                        case LEFT: rotation = 90; break;
                        case UP: rotation = 180; break;
                        case RIGHT: rotation = 270; break;
                        default: break;
                    }
                    Particle *p = particleEngine->addEffect(particleEffect,
                        0, 0, rotation);
                        controlParticle(p);
                }

                if (this == player_node && curFrame >= frameCount)
                    nextTile();

                break;
            }
        }

        // Update pixel coordinates
        setPosition(static_cast<float>(mX * 32 + 16 + getXOffset()),
                    static_cast<float>(mY * 32 + 32 + getYOffset()));
    }

    if (mEmotion != 0)
    {
        mEmotionTime--;
        if (mEmotionTime == 0)
            mEmotion = 0;
    }

    ActorSprite::logic();

//    int frameCount = static_cast<int>(getFrameCount());
    if (frameCount < 10)
        frameCount = 10;

    if (!isAlive() && getWalkSpeed().x
        && Net::getGameHandler()->removeDeadBeings()
        && static_cast<int> ((static_cast<float>(get_elapsed_time(mActionTime))
        / static_cast<float>(getWalkSpeed().x)))
        >= static_cast<int>(frameCount))
    {
        if (mType != PLAYER && actorSpriteManager)
            actorSpriteManager->destroy(this);
    }
}

void Being::drawEmotion(Graphics *graphics, int offsetX, int offsetY)
{
    if (!mEmotion)
        return;

    const int px = getPixelX() - offsetX - 16;
    const int py = getPixelY() - offsetY - 64 - 32;
    const int emotionIndex = mEmotion - 1;

    if (emotionIndex >= 0 && emotionIndex <= EmoteDB::getLast())
    {
        if (EmoteDB::getAnimation(emotionIndex))
            EmoteDB::getAnimation(emotionIndex)->draw(graphics, px, py);
    }
}

void Being::drawSpeech(int offsetX, int offsetY)
{
    if (!mSpeechBubble || mSpeech.empty())
        return;

    const int px = getPixelX() - offsetX;
    const int py = getPixelY() - offsetY;
    const int speech = mSpeechType;

    // Draw speech above this being
    if (mSpeechTime == 0)
    {
        if (mSpeechBubble->isVisible())
            mSpeechBubble->setVisible(false);
    }
    else if (mSpeechTime > 0 && (speech == NAME_IN_BUBBLE ||
             speech == NO_NAME_IN_BUBBLE))
    {
        const bool isShowName = (speech == NAME_IN_BUBBLE);

        delete mText;
        mText = 0;

        mSpeechBubble->setCaption(isShowName ? mName : "", mTextColor);

        mSpeechBubble->setText(mSpeech, isShowName);
        mSpeechBubble->setPosition(px - (mSpeechBubble->getWidth() / 2),
            py - getHeight() - (mSpeechBubble->getHeight()));
        mSpeechBubble->setVisible(true);
    }
    else if (mSpeechTime > 0 && speech == TEXT_OVERHEAD)
    {
        mSpeechBubble->setVisible(false);

        if (!mText && userPalette)
        {
            mText = new Text(mSpeech, getPixelX(), getPixelY() - getHeight(),
                gcn::Graphics::CENTER, &userPalette->getColor(
                UserPalette::PARTICLE), true);
        }
    }
    else if (speech == NO_SPEECH)
    {
        mSpeechBubble->setVisible(false);

        delete mText;
        mText = 0;
    }
}

/** TODO: eAthena only */
int Being::getOffset(char pos, char neg) const
{
    // Check whether we're walking in the requested direction
    if (mAction != MOVE ||  !(mDirection & (pos | neg)))
        return 0;

    int offset = 0;

    if (mMap)
    {
        offset = (pos == LEFT && neg == RIGHT) ?
            static_cast<int>((static_cast<float>(get_elapsed_time(mActionTime))
             * static_cast<float>(mMap->getTileWidth()))
             / static_cast<float>(mWalkSpeed.x)) :
            static_cast<int>((static_cast<float>(get_elapsed_time(mActionTime))
            * static_cast<float>(mMap->getTileHeight()))
            / static_cast<float>(mWalkSpeed.y));
    }

    // We calculate the offset _from_ the _target_ location
    offset -= 32;
    if (offset > 0)
        offset = 0;

    // Going into negative direction? Invert the offset.
    if (mDirection & pos)
        offset = -offset;

    if (offset > 32)
        offset = 32;
    if (offset < -32)
        offset = -32;

    return offset;
}

int Being::getWidth() const
{
    return std::max(CompoundSprite::getWidth(), DEFAULT_BEING_WIDTH);
}

int Being::getHeight() const
{
    return std::max(CompoundSprite::getHeight(), DEFAULT_BEING_HEIGHT);
}

void Being::updateCoords()
{
    if (!mDispName)
        return;

    // Monster names show above the sprite instead of below it
    if (mType == MONSTER)
    {
        mDispName->adviseXY(getPixelX(),
            getPixelY() - getHeight() - mDispName->getHeight());
    }
    else
    {
        mDispName->adviseXY(getPixelX(), getPixelY());
    }
}

void Being::optionChanged(const std::string &value)
{
    if (mType == PLAYER && value == "visiblenames")
        setShowName(config.getBoolValue("visiblenames"));
}

void Being::flashName(int time)
{
    if (mDispName)
        mDispName->flash(time);
}

std::string Being::getGenderSignWithSpace() const
{
    const std::string &str = getGenderSign();
    if (str.empty())
        return str;
    else
        return " " + str;
}

std::string Being::getGenderSign() const
{
    std::string str;
    if (mShowGender)
    {
        if (getGender() == GENDER_FEMALE)
            str = "\u2640";
        else if (getGender() == GENDER_MALE)
            str = "\u2642";
    }
    if (mShowPlayersStatus && mAdvanced)
    {
        if (mShop)
            str += "$";
        if (mAway)
        {
            // TRANSLATORS: this away status writed in player nick
            str += _("A");
        }
        else if (mInactive)
        {
            // TRANSLATORS: this inactive status writed in player nick
            str += _("I");
        }
    }
    return str;
}

void Being::showName()
{
    delete mDispName;
    mDispName = 0;
    std::string mDisplayName(mName);

    if (mType != MONSTER && (mShowGender || mShowLevel))
    {
        mDisplayName += " ";
        if (mShowLevel && getLevel() != 0)
            mDisplayName += toString(getLevel());

        mDisplayName += getGenderSign();
    }

    if (mType == MONSTER)
    {
        if (config.getBoolValue("showMonstersTakedDamage"))
            mDisplayName += ", " + toString(getDamageTaken());
    }

    gcn::Font *font = 0;
    if (player_node && player_node->getTarget() == this
        && mType != MONSTER)
    {
        font = boldFont;
    }
    else if (mType == PLAYER && !player_relations.isGoodName(this) && gui)
    {
        font = gui->getSecureFont();
    }

    if (mDisplayName.empty())
        return;

    mDispName = new FlashText(mDisplayName, getPixelX(), getPixelY(),
                              gcn::Graphics::CENTER, mNameColor, font);

    updateCoords();
}

void Being::updateColors()
{
    if (userPalette)
    {
        if (mType == MONSTER)
        {
            mNameColor = &userPalette->getColor(UserPalette::MONSTER);
            mTextColor = &userPalette->getColor(UserPalette::MONSTER);
        }
        else if (mType == NPC)
        {
            mNameColor = &userPalette->getColor(UserPalette::NPC);
            mTextColor = &userPalette->getColor(UserPalette::NPC);
        }
        else if (this == player_node)
        {
            mNameColor = &userPalette->getColor(UserPalette::SELF);
            mTextColor = &Theme::getThemeColor(Theme::PLAYER);
        }
        else
        {
            mTextColor = &userPalette->getColor(Theme::PLAYER);

            mErased = false;

            if (mIsGM)
            {
                mTextColor = &userPalette->getColor(UserPalette::GM);
                mNameColor = &userPalette->getColor(UserPalette::GM);
            }
            else if (mEnemy)
            {
                mNameColor = &userPalette->getColor(UserPalette::MONSTER);
            }
            else if (mParty && mParty == player_node->getParty())
            {
                mNameColor = &userPalette->getColor(UserPalette::PARTY);
            }
            else if (getGuild() && getGuild() == player_node->getGuild())
            {
                mNameColor = &userPalette->getColor(UserPalette::GUILD);
            }
            else if (player_relations.getRelation(mName) ==
                     PlayerRelation::FRIEND)
            {
                mNameColor = &userPalette->getColor(UserPalette::FRIEND);
            }
            else if (player_relations.getRelation(mName) ==
                     PlayerRelation::DISREGARDED
                     || player_relations.getRelation(mName) ==
                     PlayerRelation::BLACKLISTED)
            {
                mNameColor = &userPalette->getColor(UserPalette::DISREGARDED);
            }
            else if (player_relations.getRelation(mName) ==
                     PlayerRelation::IGNORED
                     || player_relations.getRelation(mName) ==
                     PlayerRelation::ENEMY2)
            {
                mNameColor = &userPalette->getColor(UserPalette::IGNORED);
            }
            else if (player_relations.getRelation(mName) ==
                     PlayerRelation::ERASED)
            {
                mNameColor = &userPalette->getColor(UserPalette::ERASED);
                mErased = true;
            }
            else
            {
                mNameColor = &userPalette->getColor(UserPalette::PC);
            }
        }

        if (mDispName)
            mDispName->setColor(mNameColor);
    }
}

void Being::setSprite(unsigned int slot, int id, std::string color,
                      unsigned char colorId, bool isWeapon, bool isTempSprite)
{
    if (slot >= Net::getCharHandler()->maxSprite())
        return;

    if (slot >= size())
        ensureSize(slot + 1);

    if (slot >= mSpriteIDs.size())
        mSpriteIDs.resize(slot + 1, 0);

    if (slot >= mSpriteColors.size())
        mSpriteColors.resize(slot + 1, "");

    if (slot >= mSpriteColorsIds.size())
        mSpriteColorsIds.resize(slot + 1, 1);

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);

        if (isWeapon)
            mEquippedWeapon = NULL;
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        std::string filename = info.getSprite(mGender);
        AnimatedSprite *equipmentSprite = NULL;

        if (!filename.empty())
        {
            if (color.empty())
                color = info.getDyeColorsString(colorId);
            filename = combineDye(filename, color);

            equipmentSprite = AnimatedSprite::load(
                paths.getStringValue("sprites") + filename);
        }

        if (equipmentSprite)
            equipmentSprite->setSpriteDirection(getSpriteDirection());

        CompoundSprite::setSprite(slot, equipmentSprite);

        if (isWeapon)
            mEquippedWeapon = &ItemDB::get(id);

        setAction(mAction);
    }

    if (!isTempSprite)
    {
        mSpriteIDs[slot] = id;
        mSpriteColors[slot] = color;
        mSpriteColorsIds[slot] = colorId;
        recalcSpritesOrder();
        if (beingEquipmentWindow)
            beingEquipmentWindow->updateBeing(this);
    }
}

void Being::setSpriteID(unsigned int slot, int id)
{
    setSprite(slot, id, mSpriteColors[slot]);
}

void Being::setSpriteColor(unsigned int slot, const std::string &color)
{
    setSprite(slot, mSpriteIDs[slot], color);
}

int Being::getNumberOfLayers() const
{
    return CompoundSprite::getNumberOfLayers();
}

void Being::load()
{
    // Hairstyles are encoded as negative numbers. Count how far negative
    // we can go.
    int hairstyles = 1;

    while (ItemDB::get(-hairstyles).getSprite(GENDER_MALE) !=
           paths.getStringValue("spriteErrorFile"))
    {
        hairstyles ++;
    }

    mNumberOfHairstyles = hairstyles;
}

void Being::updateName()
{
    if (mShowName)
        showName();
}

void Being::reReadConfig()
{
    if (mUpdateConfigTime + 1 < cur_time)
    {
        mHighlightMapPortals = config.getBoolValue("highlightMapPortals");
        mConfLineLim = config.getIntValue("chatMaxCharLimit");
        mSpeechType = config.getIntValue("speech");
        mHighlightMonsterAttackRange =
            config.getBoolValue("highlightMonsterAttackRange");
        mLowTraffic = config.getBoolValue("lowTraffic");
        mDrawHotKeys = config.getBoolValue("drawHotKeys");
        mShowBattleEvents = config.getBoolValue("showBattleEvents");
        mShowMobHP = config.getBoolValue("showMobHP");
        mShowOwnHP = config.getBoolValue("showOwnHP");
        mShowGender = config.getBoolValue("showgender");
        mShowLevel = config.getBoolValue("showlevel");
        mShowPlayersStatus = config.getBoolValue("showPlayersStatus");
        mEnableReorderSprites = config.getBoolValue("enableReorderSprites");

        mUpdateConfigTime = cur_time;
    }
}

bool Being::updateFromCache()
{
    BeingCacheEntry *entry = Being::getCacheEntry(getId());

    if (entry && entry->getTime() + 120 >= cur_time)
    {
        if (!entry->getName().empty())
            setName(entry->getName());
        setPartyName(entry->getPartyName());
        setGuildName(entry->getGuildName());
        setLevel(entry->getLevel());
        setPvpRank(entry->getPvpRank());
        setIp(entry->getIp());

        mAdvanced = entry->isAdvanced();
        if (entry->isAdvanced())
        {
            int flags = entry->getFlags();
            mShop = (flags & FLAG_SHOP);
            mAway = (flags & FLAG_AWAY);
            mInactive = (flags & FLAG_INACTIVE);
            if (mShop || mAway || mInactive)
                updateName();
        }
        else
        {
            mShop = false;
            mAway = false;
            mInactive = false;
        }

        if (mType == PLAYER)
            updateColors();
        return true;
    }
    return false;
}

void Being::addToCache()
{
    if (player_node == this)
        return;

    BeingCacheEntry *entry = Being::getCacheEntry(getId());
    if (!entry)
    {
        entry = new BeingCacheEntry(getId());
        beingInfoCache.push_front(entry);

        if (beingInfoCache.size() >= CACHE_SIZE)
        {
            delete beingInfoCache.back();
            beingInfoCache.pop_back();
        }
    }
    if (!mLowTraffic)
        return;

    entry->setName(getName());
    entry->setLevel(getLevel());
    entry->setPartyName(getPartyName());
    entry->setGuildName(getGuildName());
    entry->setTime(cur_time);
    entry->setPvpRank(getPvpRank());
    entry->setIp(getIp());
    entry->setAdvanced(isAdvanced());
    if (isAdvanced())
    {
        int flags = 0;
        if (mShop)
            flags += FLAG_SHOP;
        if (mAway)
            flags += FLAG_AWAY;
        if (mInactive)
            flags += FLAG_INACTIVE;
        entry->setFlags(flags);
    }
    else
    {
        entry->setFlags(0);
    }
}

BeingCacheEntry* Being::getCacheEntry(int id)
{
    for (std::list<BeingCacheEntry*>::iterator i = beingInfoCache.begin();
         i != beingInfoCache.end(); ++i)
    {
        if (!*i)
            continue;

        if (id == (*i)->getId())
        {
            // Raise priority: move it to front
            if ((*i)->getTime() + 120 < cur_time)
            {
                beingInfoCache.splice(beingInfoCache.begin(),
                                      beingInfoCache, i);
            }
            return *i;
        }
    }
    return 0;
}


void Being::setGender(Gender gender)
{
    if (gender != mGender)
    {
        mGender = gender;

        // Reload all subsprites
        for (unsigned int i = 0; i < mSpriteIDs.size(); i++)
        {
            if (mSpriteIDs.at(i) != 0)
                setSprite(i, mSpriteIDs.at(i), mSpriteColors.at(i));
        }

        updateName();
    }
}

void Being::setGM(bool gm)
{
    mIsGM = gm;

    updateColors();
}

bool Being::canTalk()
{
    return mType == NPC;
}

void Being::talkTo()
{
    if (!Client::limitPackets(PACKET_NPC_TALK))
        return;

    Net::getNpcHandler()->talk(mId);
}

bool Being::isTalking()
{
    return NpcDialog::isActive() || BuyDialog::isActive() ||
           SellDialog::isActive() || BuySellDialog::isActive() ||
           NpcPostDialog::isActive();
}

bool Being::draw(Graphics *graphics, int offsetX, int offsetY) const
{
    bool res = true;
    if (!mErased)
        res = ActorSprite::draw(graphics, offsetX, offsetY);

    return res;
}

void Being::drawSprites(Graphics* graphics, int posX, int posY) const
{
    const int sz = getNumberOfLayers();
    for (int f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        Sprite *sprite = getSprite(mSpriteRemap[f]);
        if (sprite)
        {
            sprite->setAlpha(mAlpha);
            sprite->draw(graphics, posX, posY);
        }
    }
}

void Being::drawSpritesSDL(Graphics* graphics, int posX, int posY) const
{
    const unsigned sz = size();
    for (unsigned f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        Sprite *sprite = getSprite(mSpriteRemap[f]);
        if (sprite)
            sprite->draw(graphics, posX, posY);
    }
}

bool Being::drawSpriteAt(Graphics *graphics, int x, int y) const
{
    bool res = true;

    if (!mErased)
        res = ActorSprite::drawSpriteAt(graphics, x, y);

    if (mHighlightMapPortals && mMap && mSubType == 45 && !mMap->getHasWarps())
    {
        graphics->setColor(userPalette->
                getColorWithAlpha(UserPalette::PORTAL_HIGHLIGHT));

        graphics->fillRectangle(gcn::Rectangle(x, y, 32, 32));

        if (mDrawHotKeys && !mName.empty())
        {
            gcn::Font *font = gui->getFont();
            if (font)
            {
                graphics->setColor(userPalette->getColor(UserPalette::BEING));
                font->drawString(graphics, mName, x, y);
            }
        }
    }

    if (mHighlightMonsterAttackRange && mType == ActorSprite::MONSTER
        && isAlive())
    {
        int attackRange;
        if (mAttackRange)
            attackRange = 32 * mAttackRange;
        else
            attackRange = 32;

        graphics->setColor(userPalette->getColorWithAlpha(
            UserPalette::MONSTER_ATTACK_RANGE));

        graphics->fillRectangle(gcn::Rectangle(
            x - attackRange, y - attackRange,
            2 * attackRange + 32, 2 * attackRange + 32));
    }

    if (mShowMobHP && mInfo && player_node && player_node->getTarget() == this
        && mType == MONSTER)
    {
        // show hp bar here
        int maxHP = mMaxHP;
        if (!maxHP && mInfo)
            maxHP = mInfo->getMaxHP();

        drawHpBar(graphics, maxHP, mHP, mDamageTaken,
                  UserPalette::MONSTER_HP, UserPalette::MONSTER_HP2,
                  x - 50 + 16, y + 32 - 6, 2 * 50, 4);
    }
    if (mShowOwnHP && player_node == this && mAction != DEAD)
    {
        drawHpBar(graphics, PlayerInfo::getAttribute(MAX_HP),
                  PlayerInfo::getAttribute(HP), 0,
                  UserPalette::PLAYER_HP, UserPalette::PLAYER_HP2,
                  x - 50 + 16, y + 32 - 6, 2 * 50, 4);
    }
    return res;
}

void Being::drawHpBar(Graphics *graphics, int maxHP, int hp, int damage,
                      int color1, int color2, int x, int y,
                      int width, int height) const
{
    if (maxHP <= 0)
        return;

    if (!hp && maxHP < hp)
        return;

    float p;

    if (hp)
    {
        p = static_cast<float>(maxHP) / static_cast<float>(hp);
    }
    else if (maxHP != damage)
    {
        p = static_cast<float>(maxHP)
            / static_cast<float>(maxHP - damage);
    }
    else
    {
        p = 1;
    }

    if (p <= 0 || p > width)
        return;

    int dx = static_cast<int>(static_cast<float>(width) / p);

    if (!damage || (!hp && maxHP == damage))
    {
        graphics->setColor(userPalette->getColorWithAlpha(color1));

        graphics->fillRectangle(gcn::Rectangle(
            x, y, dx, height));
        return;
    }
    else if (width - dx <= 0)
    {
        graphics->setColor(userPalette->getColorWithAlpha(color2));

        graphics->fillRectangle(gcn::Rectangle(
            x, y, width, height));
        return;
    }

    graphics->setColor(userPalette->getColorWithAlpha(color1));

    graphics->fillRectangle(gcn::Rectangle(
        x, y, dx, height));

    graphics->setColor(userPalette->getColorWithAlpha(color2));

    graphics->fillRectangle(gcn::Rectangle(
        x + dx, y, width - dx, height));
}

void Being::setHP(int hp)
{
    mHP = hp;
    if (mMaxHP < mHP)
        mMaxHP = mHP;
}

void Being::setMaxHP(int hp)
{
    mMaxHP = hp;
    if (mMaxHP < mHP)
        mMaxHP = mHP;
}

void Being::resetCounters()
{
    mMoveTime = 0;
    mAttackTime = 0;
    mTalkTime = 0;
    mOtherTime = 0;
    mTestTime = cur_time;
}

void Being::recalcSpritesOrder()
{
    if (!mEnableReorderSprites)
        return;

//    logger->log("recalcSpritesOrder");
    unsigned sz = static_cast<unsigned>(size());
    if (sz < 1)
        return;

    std::vector<int> slotRemap;
    std::map<int, int> itemSlotRemap;

//    logger->log("preparation start");
    std::vector<int>::iterator it;
    int oldHide[20];
    int dir = mSpriteDirection;
    if (dir < 0 || dir >= 9)
        dir = 0;

    for (unsigned slot = 0; slot < sz; slot ++)
    {
        oldHide[slot] = mSpriteHide[slot];
        mSpriteHide[slot] = 0;
    }

    for (unsigned slot = 0; slot < sz; slot ++)
    {
        slotRemap.push_back(slot);

        if (mSpriteIDs.size() <= slot)
            continue;

        int id = mSpriteIDs[slot];
        if (!id)
            continue;

        const ItemInfo &info = ItemDB::get(id);

        if (info.isRemoveSprites())
        {
            SpriteToItemMap *spriteToItems = info.getSpriteToItemReplaceMap(
                mSpriteDirection);

            if (spriteToItems)
            {
                SpriteToItemMap::const_iterator itr;

                for (itr = spriteToItems->begin();
                     itr != spriteToItems->end(); ++itr)
                {
                    int remSprite = itr->first;
                    const std::map<int, int> &itemReplacer = itr->second;
                    if (itemReplacer.empty())
                    {
                        mSpriteHide[remSprite] = 1;
                    }
                    else
                    {
                        std::map<int, int>::const_iterator repIt
                            = itemReplacer.find(mSpriteIDs[remSprite]);
                        if (repIt != itemReplacer.end())
                        {
                            mSpriteHide[remSprite] = repIt->second;
                            if (repIt->second != 1)
                            {
                                setSprite(remSprite, repIt->second,
                                    mSpriteColors[remSprite],
                                    1, false, true);
                            }
                        }
                    }
                }
            }
        }

        if (info.mDrawBefore[dir] > 0)
        {
            int id2 = mSpriteIDs[info.mDrawBefore[dir]];
            std::map<int, int>::const_iterator
                orderIt = itemSlotRemap.find(id2);
            if (orderIt != itemSlotRemap.end())
            {
//                logger->log("found duplicate (before)");
                const ItemInfo &info2 = ItemDB::get(id2);
                if (info.mDrawPriority[dir] < info2.mDrawPriority[dir])
                {
//                    logger->log("old more priority");
                    continue;
                }
                else
                {
//                    logger->log("new more priority");
                    itemSlotRemap.erase(id2);
                }
            }

            itemSlotRemap[id] = -info.mDrawBefore[dir];
//            logger->log("item slot->slot %d %d->%d", id, slot, itemSlotRemap[id]);
        }
        else if (info.mDrawAfter[dir] > 0)
        {
            int id2 = mSpriteIDs[info.mDrawAfter[dir]];
            std::map<int, int>::const_iterator
                orderIt = itemSlotRemap.find(id2);
            if (orderIt != itemSlotRemap.end())
            {
//                logger->log("found duplicate (after)");
                const ItemInfo &info2 = ItemDB::get(id2);
                if (info.mDrawPriority[dir] < info2.mDrawPriority[dir])
                {
//                    logger->log("old more priority");
                    continue;
                }
                else
                {
//                    logger->log("new more priority");
                    itemSlotRemap.erase(id2);
                }
            }

            itemSlotRemap[id] = info.mDrawAfter[dir];
//            logger->log("item slot->slot %d %d->%d", id, slot, itemSlotRemap[id]);
        }
    }
//    logger->log("preparation end");

    int lastRemap = 0;
    unsigned cnt = 0;

    while (cnt < 15 && lastRemap >= 0)
    {
        lastRemap = -1;
        cnt ++;
//        logger->log("iteration");

        for (unsigned slot0 = 0; slot0 < sz; slot0 ++)
        {
            int slot = searchSlotValue(slotRemap, slot0);
            int val = slotRemap.at(slot);
            int id = 0;

            if ((int)mSpriteIDs.size() > val)
                id = mSpriteIDs[val];

            int idx = -1;
            int idx1 = -1;
//            logger->log("item %d, id=%d", slot, id);
            int reorder = 0;
            std::map<int, int>::const_iterator
                orderIt = itemSlotRemap.find(id);
            if (orderIt != itemSlotRemap.end())
                reorder = orderIt->second;

            if (reorder < 0)
            {
//                logger->log("move item %d before %d", slot, -reorder);
                searchSlotValueItr(it, idx, slotRemap, -reorder);
                if (it == slotRemap.end())
                    return;
                searchSlotValueItr(it, idx1, slotRemap, val);
                if (it == slotRemap.end())
                    return;
                lastRemap = idx1;
                if (idx1 + 1 != idx)
                {
                    slotRemap.erase(it);
                    searchSlotValueItr(it, idx, slotRemap, -reorder);
                    slotRemap.insert(it, val);
                }
            }
            else if (reorder > 0)
            {
//                logger->log("move item %d after %d", slot, reorder);
                searchSlotValueItr(it, idx, slotRemap, reorder);
                searchSlotValueItr(it, idx1, slotRemap, val);
                if (it == slotRemap.end())
                    return;
                lastRemap = idx1;
                if (idx1 != idx + 1)
                {
                    slotRemap.erase(it);
                    searchSlotValueItr(it, idx, slotRemap, reorder);
                    if (it != slotRemap.end())
                    {
                        ++ it;
                        if (it != slotRemap.end())
                            slotRemap.insert(it, val);
                        else
                            slotRemap.push_back(val);
                    }
                    else
                    {
                        slotRemap.push_back(val);
                    }
                }
            }
        }
    }

//    logger->log("after remap");
    for (unsigned slot = 0; slot < sz; slot ++)
    {
        mSpriteRemap[slot] = slotRemap[slot];
        if (oldHide[slot] != 0 && oldHide[slot] != 1 && mSpriteHide[slot] == 0)
        {
            int id = mSpriteIDs[slot];
            if (!id)
                continue;

            setSprite(slot, id, mSpriteColors[slot], 1, false, true);
        }
//        logger->log("slot %d = %d", slot, mSpriteRemap[slot]);
    }
}

int Being::searchSlotValue(std::vector<int> &slotRemap, int val)
{
    for (unsigned slot = 0; slot < size(); slot ++)
    {
        if (slotRemap[slot] == val)
            return slot;
    }
    return getNumberOfLayers() - 1;
}

void Being::searchSlotValueItr(std::vector<int>::iterator &it, int &idx,
                               std::vector<int> &slotRemap, int val)
{
//    logger->log("searching %d", val);
    it = slotRemap.begin();
    idx = 0;
    while(it != slotRemap.end())
    {
//        logger->log("testing %d", *it);
        if (*it == val)
        {
//            logger->log("found at %d", idx);
            return;
        }
        it ++;
        idx ++;
    }
//    logger->log("not found");
    idx = -1;
    return;
}

void Being::updateHit(int amount)
{
    if (amount > 0)
    {
        if (!mMinHit || amount < mMinHit)
            mMinHit = amount;
        if (amount != mCriticalHit && (!mMaxHit || amount > mMaxHit))
            mMaxHit = amount;
    }
}

Equipment *Being::getEquipment()
{
    Equipment *eq = new Equipment();
    Equipment::Backend *bk = new BeingEquipBackend(this);
    eq->setBackend(bk);
    return eq;
}

void Being::undressItemById(int id)
{
    int sz = mSpriteIDs.size();

    for (int f = 0; f < sz; f ++)
    {
        if (id == mSpriteIDs[f])
        {
            setSprite(f, 0);
            break;
        }
    }
}

void Being::clearCache()
{
    delete_all(beingInfoCache);
    beingInfoCache.clear();
}

void Being::updateComment()
{
    if (mGotComment || mName.empty())
        return;

    mGotComment = true;
    mComment = loadComment(mName, mType);
}

std::string Being::loadComment(const std::string &name, int type)
{
    std::string str;
    switch (type)
    {
        case PLAYER:
            str = Client::getUsersDirectory();
            break;
        case NPC:
            str = Client::getNpcsDirectory();
            break;
        default:
            return "";
    }

    str += stringToHexPath(name) + "/comment.txt";
    logger->log("load from: %s", str.c_str());
    std::vector<std::string> lines;

    ResourceManager *resman = ResourceManager::getInstance();
    if (resman->existsLocal(str))
    {
        lines = resman->loadTextFileLocal(str);
        if (lines.size() >= 2)
            return lines[1];
    }
    return "";
}

void Being::saveComment(const std::string &name,
                        const std::string &comment, int type)
{
    std::string dir;
    switch (type)
    {
        case PLAYER:
            dir = Client::getUsersDirectory();
            break;
        case NPC:
            dir = Client::getNpcsDirectory();
            break;
        default:
            return;
    }
    dir += stringToHexPath(name);
//    logger->log("save to: %s", dir.c_str());
    ResourceManager *resman = ResourceManager::getInstance();
    resman->saveTextFile(dir, "comment.txt", name + "\n" + comment);
}

void Being::setEmote(Uint8 emotion, int emote_time)
{
    if ((emotion & FLAG_SPECIAL) == FLAG_SPECIAL)
    {
        mAdvanced = true;
        bool shop = (emotion & FLAG_SHOP);
        bool away = (emotion & FLAG_AWAY);
        bool inactive = (emotion & FLAG_INACTIVE);
        bool needUpdate = (shop != mShop || away != mAway
            || inactive != mInactive);

        mShop = shop;
        mAway = away;
        mInactive = inactive;

        if (needUpdate)
        {
            updateName();
            addToCache();
        }
//        logger->log("flags: %d", emotion - FLAG_SPECIAL);
    }
    else
    {
        mEmotion = emotion;
        mEmotionTime = emote_time;
    }
}

BeingEquipBackend::BeingEquipBackend(Being *being):
    mBeing(being)
{
    memset(mEquipment, 0, sizeof(mEquipment));
    if (being)
    {
        int sz = being->mSpriteIDs.size();

        for (int f = 0; f < sz; f ++)
        {
            int idx = Net::getInventoryHandler()->convertFromServerSlot(f);
            int id = being->mSpriteIDs[f];
            if (id > 0 && idx >= 0 && idx < EQUIPMENT_SIZE)
            {
                mEquipment[idx] = new Item(id, 1, 0,
                    being->mSpriteColorsIds[f], true, true);
            }
        }
    }
}

BeingEquipBackend::~BeingEquipBackend()
{
    clear();
}

void BeingEquipBackend::clear()
{
    for (int i = 0; i < EQUIPMENT_SIZE; i++)
    {
        delete mEquipment[i];
        mEquipment[i] = 0;
    }
}

void BeingEquipBackend::setEquipment(int index, Item *item)
{
    mEquipment[index] = item;
}

Item *BeingEquipBackend::getEquipment(int index) const
{
    if (index < 0 || index >= EQUIPMENT_SIZE)
        return 0;
    return mEquipment[index];
}
