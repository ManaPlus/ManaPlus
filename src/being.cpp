/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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
#include "beingcacheentry.h"
#include "beingequipbackend.h"
#include "client.h"
#include "effectmanager.h"
#include "guild.h"
#include "particle.h"
#include "party.h"
#include "playerrelations.h"
#include "soundmanager.h"
#include "text.h"

#include "gui/equipmentwindow.h"
#include "gui/socialwindow.h"
#include "gui/speechbubble.h"
#include "gui/sdlfont.h"
#include "gui/skilldialog.h"

#include "net/charserverhandler.h"
#include "net/gamehandler.h"
#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/playerhandler.h"

#include "resources/avatardb.h"
#include "resources/emotedb.h"
#include "resources/iteminfo.h"
#include "resources/monsterdb.h"
#include "resources/npcdb.h"
#include "resources/petdb.h"
#include "resources/resourcemanager.h"

#include "gui/widgets/langtab.h"

#include "utils/gettext.h"

#include <cmath>

#include "debug.h"

const unsigned int CACHE_SIZE = 50;

int Being::mNumberOfHairstyles = 1;
int Being::mNumberOfRaces = 1;

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
bool Being::mHideErased = false;
bool Being::mMoveNames = false;
int Being::mAwayEffect = -1;

std::list<BeingCacheEntry*> beingInfoCache;
typedef std::map<int, Guild*>::const_iterator GuildsMapCIter;
typedef std::map<int, int>::const_iterator IntMapCIter;

Being::Being(const int id, const Type type, const uint16_t subtype,
             Map *const map) :
    ActorSprite(id),
    mNextSound(),
    mInfo(BeingInfo::unknown),
    mActionTime(0),
    mEmotionSprite(nullptr),
    mEmotionTime(0),
    mSpeechTime(0),
    mAnimationEffect(nullptr),
    mAttackSpeed(350),
    mAction(STAND),
    mSubType(0xFFFF),
    mDirection(DOWN),
    mDirectionDelayed(0),
    mSpriteDirection(DIRECTION_DOWN),
    mSpriteAction(SpriteAction::STAND),
    mName(),
    mRaceName(),
    mPartyName(),
    mGuildName(),
    mDispName(nullptr),
    mNameColor(nullptr),
    mShowName(false),
    mEquippedWeapon(nullptr),
    mPath(),
    mSpeech(),
    mText(nullptr),
    mTextColor(nullptr),
    mLevel(0),
    mDest(),
    mSpriteIDs(),
    mSpriteColors(),
    mSpriteColorsIds(),
    mGender(GENDER_UNSPECIFIED),
    mGuilds(),
    mParty(nullptr),
    mIsGM(false),
    mAttackRange(1),
    mType(type),
    mSpeechBubble(new SpeechBubble),
    mWalkSpeed(Net::getPlayerHandler()->getDefaultWalkSpeed()),
    mX(0),
    mY(0),
    mDamageTaken(0),
    mHP(0),
    mMaxHP(0),
    mDistance(0),
    mIsReachable(REACH_UNKNOWN),
    mGoodStatus(-1),
    mMoveTime(0),
    mAttackTime(0),
    mTalkTime(0),
    mOtherTime(0),
    mTestTime(cur_time),
    mErased(false),
    mEnemy(false),
    mIp(""),
    mAttackDelay(0),
    mMinHit(0),
    mMaxHit(0),
    mCriticalHit(0),
    mPvpRank(0),
    mSpriteRemap(new int[20]),
    mSpriteHide(new int[20]),
    mComment(""),
    mGotComment(false),
    mAdvanced(false),
    mShop(false),
    mAway(false),
    mInactive(false),
    mNumber(100),
    mHairColor(0),
    mPet(nullptr),
    mPetId(0),
    mOwner(nullptr),
    mSpecialParticle(nullptr)
{
    for (int f = 0; f < 20; f ++)
    {
        mSpriteRemap[f] = f;
        mSpriteHide[f] = 0;
    }

    setMap(map);
    setSubtype(subtype);

    if (mType == PLAYER)
        mShowName = config.getBoolValue("visiblenames");
    else if (mType != NPC)
        mGotComment = true;

    config.addListener("visiblenames", this);

    reReadConfig();

    if (mType == NPC)
        setShowName(true);
    else
        setShowName(mShowName);

    updateColors();
    updatePercentHP();
}

Being::~Being()
{
    config.removeListener("visiblenames", this);

    delete [] mSpriteRemap;
    mSpriteRemap = nullptr;
    delete [] mSpriteHide;
    mSpriteHide = nullptr;

    delete mSpeechBubble;
    mSpeechBubble = nullptr;
    delete mDispName;
    mDispName = nullptr;
    delete mText;
    mText = nullptr;

    delete mEmotionSprite;
    mEmotionSprite = nullptr;
    delete mAnimationEffect;
    mAnimationEffect = nullptr;

    if (mOwner)
        mOwner->setPet(nullptr);
    if (mPet)
        mPet->setOwner(nullptr);
}

void Being::setSubtype(const uint16_t subtype)
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
            mYDiff = mInfo->getSortOffsetY();
        }
    }
    else if (mType == NPC)
    {
        mInfo = NPCDB::get(mSubType);
        if (mInfo)
        {
            setupSpriteDisplay(mInfo->getDisplay(), false);
            mYDiff = mInfo->getSortOffsetY();
        }
    }
    else if (mType == AVATAR)
    {
        mInfo = AvatarDB::get(mSubType);
        if (mInfo)
            setupSpriteDisplay(mInfo->getDisplay(), false);
    }
    else if (mType == PET)
    {
        mInfo = PETDB::get(mId);
        if (mInfo)
        {
            setupSpriteDisplay(mInfo->getDisplay(), false);
            mYDiff = mInfo->getSortOffsetY();
        }
    }
    else if (mType == PLAYER)
    {
        int id = -100 - subtype;

        // Prevent showing errors when sprite doesn't exist
        if (!ItemDB::exists(id))
        {
            id = -100;
            // TRANSLATORS: default race name
            setRaceName(_("Human"));
        }
        else
        {
            const ItemInfo &info = ItemDB::get(id);
            setRaceName(info.getName());
        }

        if (Net::getCharServerHandler())
            setSprite(Net::getCharServerHandler()->baseSprite(), id);
    }
}

ActorSprite::TargetCursorSize Being::getTargetCursorSize() const
{
    if (!mInfo)
        return ActorSprite::TC_SMALL;

    return mInfo->getTargetCursorSize();
}

void Being::setPosition(const Vector &pos)
{
    Actor::setPosition(pos);

    updateCoords();

    if (mText)
    {
        mText->adviseXY(static_cast<int>(pos.x), static_cast<int>(pos.y)
            - getHeight() - mText->getHeight() - 6, mMoveNames);
    }
}

void Being::setDestination(const int dstX, const int dstY)
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

#ifdef MANASERV_SUPPORT
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
        static_cast<unsigned char>(getWalkMask()));

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
#endif
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

void Being::setSpeech(const std::string &text, const std::string &channel,
                      int time)
{
    if (!userPalette)
        return;

    if (!channel.empty() && (langChatTab && langChatTab->getChannelName()
        != channel))
    {
        return;
    }

    // Remove colors
    mSpeech = removeColors(text);

    // Trim whitespace
    trim(mSpeech);

    const unsigned int lineLim = mConfLineLim;
    if (lineLim > 0 && mSpeech.length() > lineLim)
        mSpeech = mSpeech.substr(0, lineLim);

    trim(mSpeech);
    if (mSpeech.empty())
        return;

    const size_t sz = mSpeech.size();
    if (!time && sz < 200)
        time = static_cast<int>(SPEECH_TIME - 300 + (3 * sz));

    if (time < static_cast<int>(SPEECH_MIN_TIME))
        time = static_cast<int>(SPEECH_MIN_TIME);

    // Check for links
    size_t start = mSpeech.find('[');
    size_t e = mSpeech.find(']', start);

    while (start != std::string::npos && e != std::string::npos)
    {
        // Catch multiple embeds and ignore them so it doesn't crash the client.
        while ((mSpeech.find('[', start + 1) != std::string::npos) &&
               (mSpeech.find('[', start + 1) < e))
        {
            start = mSpeech.find('[', start + 1);
        }

        size_t position = mSpeech.find('|');
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
    {
        mSpeechTime = time <= static_cast<int>(SPEECH_MAX_TIME)
            ? time : static_cast<int>(SPEECH_MAX_TIME);
    }

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

void Being::takeDamage(Being *const attacker, const int amount,
                       const AttackType type, const int attackId)
{
    if (!userPalette || !attacker)
        return;

    gcn::Font *font = nullptr;
    // TRANSLATORS: hit or miss message in attacks
    const std::string damage = amount ? toString(amount) : type == FLEE ?
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

    if (type != SKILL)
        attacker->updateHit(amount);

    if (amount > 0)
    {
        if (player_node && player_node == this)
            player_node->setLastHitFrom(attacker->getName());

        mDamageTaken += amount;
        if (mInfo)
        {
            playSfx(mInfo->getSound(SOUND_EVENT_HURT), this, false, mX, mY);

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
        {
            updatePercentHP();
            updateName();
        }
        else if (mType == PLAYER && socialWindow && getName() != "")
        {
            socialWindow->updateAvatar(getName());
        }

        if (effectManager)
        {
            const int hitEffectId = getHitEffect(attacker, type, attackId);
            if (hitEffectId >= 0)
                effectManager->trigger(hitEffectId, this);
        }
    }
    else
    {
        if (effectManager)
        {
            const int hitEffectId = getHitEffect(attacker,
                MISS, attackId);
            if (hitEffectId >= 0)
                effectManager->trigger(hitEffectId, this);
        }
    }
}

int Being::getHitEffect(const Being *const attacker,
                        const AttackType type, const int attackId) const
{
    if (!effectManager)
        return 0;

    // Init the particle effect path based on current
    // weapon or default.
    int hitEffectId = 0;
    if (type != SKILL)
    {
        if (attacker)
        {
            const ItemInfo *attackerWeapon = attacker->getEquippedWeapon();
            if (attackerWeapon && attacker->getType() == PLAYER)
            {
                if (type == MISS)
                    hitEffectId = attackerWeapon->getMissEffectId();
                else if (type != CRITICAL)
                    hitEffectId = attackerWeapon->getHitEffectId();
                else
                    hitEffectId = attackerWeapon->getCriticalHitEffectId();
            }
            else if (attacker->getType() == MONSTER)
            {
                const BeingInfo *const info = attacker->getInfo();
                if (info)
                {
                    const Attack *atk = info->getAttack(attackId);
                    if (atk)
                    {
                        if (type == MISS)
                            hitEffectId = atk->mMissEffectId;
                        else if (type != CRITICAL)
                            hitEffectId = atk->mHitEffectId;
                        else
                            hitEffectId = atk->mCriticalHitEffectId;
                    }
                    else
                    {
                        hitEffectId = getDefaultEffectId(type);
                    }
                }
            }
            else
            {
                hitEffectId = getDefaultEffectId(type);
            }
        }
        else
        {
            hitEffectId = getDefaultEffectId(type);
        }
    }
    else
    {
        // move skills effects to +100000 in effects list
        hitEffectId = attackId + 100000;
    }
    return hitEffectId;
}

int Being::getDefaultEffectId(const int type)
{
    if (type == MISS)
        return paths.getIntValue("missEffectId");
    else if (type != CRITICAL)
        return paths.getIntValue("hitEffectId");
    else
        return paths.getIntValue("criticalHitEffectId");
}

void Being::handleAttack(Being *const victim, const int damage,
                         const int attackId)
{
    if (!victim || !mInfo)
        return;

    if (this != player_node)
        setAction(Being::ATTACK, attackId);

    if (mType == PLAYER && mEquippedWeapon)
        fireMissile(victim, mEquippedWeapon->getMissileParticleFile());
    else if (mInfo->getAttack(attackId))
        fireMissile(victim, mInfo->getAttack(attackId)->mMissileParticle);

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        reset();
        mActionTime = tick_time;
    }

    if (this != player_node)
    {
        const uint8_t dir = calcDirection(victim->getTileX(),
            victim->getTileY());
        if (dir)
            setDirection(dir);
    }
    if (damage && victim->mType == PLAYER && victim->mAction == SIT)
        victim->setAction(STAND);

    if (mType == PLAYER)
    {
        if (mSpriteIDs.size() >= 10)
        {
            // here 10 is weapon slot
            int weaponId = mSpriteIDs[10];
            if (!weaponId)
                weaponId = -100 - mSubType;
            const ItemInfo &info = ItemDB::get(weaponId);
            playSfx(info.getSound((damage > 0) ?
                SOUND_EVENT_HIT : SOUND_EVENT_MISS), victim, true, mX, mY);
        }
    }
    else
    {
        playSfx(mInfo->getSound((damage > 0) ?
            SOUND_EVENT_HIT : SOUND_EVENT_MISS), victim, true, mX, mY);
    }
}

void Being::handleSkill(Being *const victim, const int damage,
                        const int skillId, const int skillLevel)
{
    if (!victim || !mInfo || !skillDialog)
        return;

    if (this != player_node)
        setAction(Being::ATTACK, 1);

    SkillInfo *const skill = skillDialog->getSkill(skillId);
    const SkillData *const data = skill
        ? skill->getData1(skillLevel) : nullptr;
    if (data)
        fireMissile(victim, data->particle);

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        reset();
        mActionTime = tick_time;
    }

    if (this != player_node)
    {
        const uint8_t dir = calcDirection(victim->getTileX(),
            victim->getTileY());
        if (dir)
            setDirection(dir);
    }
    if (damage && victim->mType == PLAYER && victim->mAction == SIT)
        victim->setAction(STAND);
    if (data)
    {
        if (damage > 0)
            playSfx(data->soundHit, victim, true, mX, mY);
        else
            playSfx(data->soundMiss, victim, true, mX, mY);
    }
    else
    {
        playSfx(mInfo->getSound((damage > 0) ?
            SOUND_EVENT_HIT : SOUND_EVENT_MISS), victim, true, mX, mY);
    }
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

void Being::setShowName(const bool doShowName)
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
        mDispName = nullptr;
    }
}

void Being::setGuildName(const std::string &name)
{
    mGuildName = name;
}

void Being::setGuildPos(const std::string &pos A_UNUSED)
{
}

void Being::addGuild(Guild *const guild)
{
    if (!guild)
        return;

    mGuilds[guild->getId()] = guild;

    if (this == player_node && socialWindow)
        socialWindow->addTab(guild);
}

void Being::removeGuild(const int id)
{
    if (this == player_node && socialWindow)
        socialWindow->removeTab(mGuilds[id]);

    if (mGuilds[id])
        mGuilds[id]->removeMember(getName());
    mGuilds.erase(id);
}

Guild *Being::getGuild(const std::string &guildName) const
{
    FOR_EACH (GuildsMapCIter, itr, mGuilds)
    {
        Guild *const guild = itr->second;
        if (guild && guild->getName() == guildName)
            return guild;
    }

    return nullptr;
}

Guild *Being::getGuild(const int id) const
{
    const std::map<int, Guild*>::const_iterator itr = mGuilds.find(id);
    if (itr != mGuilds.end())
        return itr->second;

    return nullptr;
}

Guild *Being::getGuild() const
{
    const std::map<int, Guild*>::const_iterator itr = mGuilds.begin();
    if (itr != mGuilds.end())
        return itr->second;

    return nullptr;
}

void Being::clearGuilds()
{
    FOR_EACH (GuildsMapCIter, itr, mGuilds)
    {
        Guild *const guild = itr->second;

        if (guild)
        {
            if (this == player_node && socialWindow)
                socialWindow->removeTab(guild);

            guild->removeMember(mId);
        }
    }

    mGuilds.clear();
}

void Being::setParty(Party *const party)
{
    if (party == mParty)
        return;

    Party *const old = mParty;
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

    Guild *const guild = player_node->getGuild();
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

void Being::setGuild(Guild *const guild)
{
    Guild *const old = getGuild();
    if (guild == old)
        return;

    clearGuilds();
    addGuild(guild);

    if (old)
        old->removeMember(mName);

    updateColors();

    if (this == player_node && socialWindow)
    {
        if (old)
            socialWindow->removeTab(old);

        if (guild)
            socialWindow->addTab(guild);
    }
}

void Being::fireMissile(Being *const victim, const std::string &particle) const
{
    if (!victim || particle.empty() || !particleEngine)
        return;

    Particle *const target = particleEngine->createChild();

    if (!target)
        return;

    Particle *const missile = target->addEffect(
        particle, getPixelX(), getPixelY());

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

std::string Being::getSitAction() const
{
    if (serverVersion < 0)
    {
        return SpriteAction::SIT;
    }
    else
    {
        if (mMap && !mMap->getWalk(mX, mY, Map::BLOCKMASK_GROUNDTOP))
            return SpriteAction::SITTOP;
        return SpriteAction::SIT;
    }
}

void Being::setAction(const Action &action, const int attackId)
{
    std::string currentAction = SpriteAction::INVALID;

    switch (action)
    {
        case MOVE:
            if (mInfo)
            {
                playSfx(mInfo->getSound(
                    SOUND_EVENT_MOVE), nullptr, true, mX, mY);
            }
            currentAction = SpriteAction::MOVE;
            // Note: When adding a run action,
            // Differentiate walk and run with action name,
            // while using only the ACTION_MOVE.
            break;
        case SIT:
            currentAction = getSitAction();
            if (mInfo)
            {
                SoundEvent event;
                if (currentAction == SpriteAction::SITTOP)
                    event = SOUND_EVENT_SITTOP;
                else
                    event = SOUND_EVENT_SIT;
                playSfx(mInfo->getSound(event), nullptr, true, mX, mY);
            }
            break;
        case ATTACK:
            if (mEquippedWeapon)
            {
                currentAction = mEquippedWeapon->getAttackAction();
                reset();
            }
            else
            {
                if (!mInfo || !mInfo->getAttack(attackId))
                    break;

                currentAction = mInfo->getAttack(attackId)->mAction;
                reset();

                // attack particle effect
                if (Particle::enabled)
                {
                    const int effectId = mInfo->getAttack(attackId)->mEffectId;

                    int rotation;
                    switch (mSpriteDirection)
                    {
                        case DIRECTION_DOWN:
                        default:
                            rotation = 0;
                            break;
                        case DIRECTION_LEFT:
                            rotation = 90;
                            break;
                        case DIRECTION_UP:
                            rotation = 180;
                            break;
                        case DIRECTION_RIGHT:
                            rotation = 270;
                            break;
                    }
                    if (Particle::enabled && effectManager && effectId >= 0)
                        effectManager->trigger(effectId, this, rotation);
                }
            }
            break;
        case HURT:
            if (mInfo)
            {
                playSfx(mInfo->getSound(SOUND_EVENT_HURT),
                    this, false, mX, mY);
            }
            break;
        case DEAD:
            currentAction = SpriteAction::DEAD;
            if (mInfo)
            {
                playSfx(mInfo->getSound(SOUND_EVENT_DIE), this, true, mX, mY);
                if (mType == MONSTER || mType == NPC)
                    mYDiff = mInfo->getDeadSortOffsetY();
            }
            break;
        case STAND:
            currentAction = SpriteAction::STAND;
            break;
        case SPAWN:
            if (mInfo)
            {
                playSfx(mInfo->getSound(SOUND_EVENT_SPAWN),
                    nullptr, true, mX, mY);
            }
            currentAction = SpriteAction::SPAWN;
            break;
        default:
            logger->log("Being::setAction unknown action: "
                + toString(static_cast<unsigned>(action)));
            break;
    }

    if (currentAction != SpriteAction::INVALID)
    {
        mSpriteAction = currentAction;
        play(currentAction);
        if (mEmotionSprite)
            mEmotionSprite->play(currentAction);
        if (mAnimationEffect)
            mAnimationEffect->play(currentAction);
        mAction = action;
    }

    if (currentAction != SpriteAction::MOVE)
        mActionTime = tick_time;
}

void Being::setDirection(const uint8_t direction)
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
    mSpriteDirection = static_cast<uint8_t>(dir);

    CompoundSprite::setSpriteDirection(dir);
    if (mEmotionSprite)
        mEmotionSprite->setSpriteDirection(dir);
    if (mAnimationEffect)
        mAnimationEffect->setSpriteDirection(dir);
    recalcSpritesOrder();
}

uint8_t Being::calcDirection() const
{
    uint8_t dir = 0;
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

uint8_t Being::calcDirection(const int dstX, const int dstY) const
{
    uint8_t dir = 0;
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

    const Position pos = mPath.front();
    mPath.pop_front();

    const uint8_t dir = calcDirection(pos.x, pos.y);
    if (dir)
        setDirection(dir);

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

void Being::logic()
{
    BLOCK_START("Being::logic")
    // Reduce the time that speech is still displayed
    if (mSpeechTime > 0)
        mSpeechTime--;

    // Remove text and speechbubbles if speech boxes aren't being used
    if (mSpeechTime == 0 && mText)
    {
        delete mText;
        mText = nullptr;
    }

    const int time = tick_time * MILLISECONDS_IN_A_TICK;
    if (mEmotionSprite)
        mEmotionSprite->update(time);

    if (mAnimationEffect)
    {
        mAnimationEffect->update(time);
        if (mAnimationEffect->isTerminated())
        {
            delete mAnimationEffect;
            mAnimationEffect = nullptr;
        }
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
        {
            BLOCK_END("Being::logic")
            return;
        }

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

                setDirection(static_cast<uint8_t>(direction));
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
                    frameCount)) / getWalkSpeed().x)
                    >= frameCount)
                {
                    nextTile();
                }
                break;
            }

            case ATTACK:
            {
//                std::string particleEffect("");

                if (!mActionTime)
                    break;

                int curFrame = 0;
                if (mAttackSpeed)
                {
                    curFrame = (get_elapsed_time(mActionTime) * frameCount)
                        / mAttackSpeed;
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

    if (mEmotionSprite)
    {
        mEmotionTime--;
        if (mEmotionTime == 0)
        {
            delete mEmotionSprite;
            mEmotionSprite = nullptr;
        }
    }

    ActorSprite::logic();

    if (frameCount < 10)
        frameCount = 10;

    if (!isAlive() && getWalkSpeed().x
        && Net::getGameHandler()->removeDeadBeings()
        && static_cast<int> ((static_cast<float>(get_elapsed_time(mActionTime))
        / static_cast<float>(getWalkSpeed().x))) >= frameCount)
    {
        if (mType != PLAYER && actorSpriteManager)
            actorSpriteManager->destroy(this);
    }

    const SoundInfo *const sound = mNextSound.sound;
    if (sound)
    {
        const int time2 = tick_time;
        if (time2 > mNextSound.time)
        {
            soundManager.playSfx(sound->sound, mNextSound.x, mNextSound.y);

            mNextSound.sound = nullptr;
            mNextSound.time = time2 + sound->delay;
        }
    }

    BLOCK_END("Being::logic")
}

void Being::drawEmotion(Graphics *const graphics, const int offsetX,
                        const int offsetY)
{
    const int px = getPixelX() - offsetX - 16;
    const int py = getPixelY() - offsetY - 64 - 32;
    if (mEmotionSprite)
        mEmotionSprite->draw(graphics, px, py);
    if (mAnimationEffect)
        mAnimationEffect->draw(graphics, px, py);
}

void Being::drawSpeech(const int offsetX, const int offsetY)
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
        mText = nullptr;

        mSpeechBubble->setCaption(isShowName ? mName : "");

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
                gcn::Graphics::CENTER, &Theme::getThemeColor(
                Theme::BUBBLE_TEXT), true);
        }
    }
    else if (speech == NO_SPEECH)
    {
        mSpeechBubble->setVisible(false);

        delete mText;
        mText = nullptr;
    }
}

int Being::getOffset(const signed char pos, const signed char neg) const
{
    // Check whether we're walking in the requested direction
    if (mAction != MOVE ||  !(mDirection & (pos | neg)))
        return 0;

    int offset = 0;

    if (mMap)
    {
        const int time = get_elapsed_time(mActionTime);
        offset = (pos == LEFT && neg == RIGHT) ?
            static_cast<int>((static_cast<float>(time)
             * static_cast<float>(mMap->getTileWidth()))
             / static_cast<float>(mWalkSpeed.x)) :
            static_cast<int>((static_cast<float>(time)
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

void Being::updateCoords()
{
    if (!mDispName)
        return;

    // Monster names show above the sprite instead of below it
    if (mType == MONSTER)
    {
        mDispName->adviseXY(getPixelX(),
            getPixelY() - getHeight() - mDispName->getHeight(), mMoveNames);
    }
    else
    {
        mDispName->adviseXY(getPixelX(), getPixelY(), mMoveNames);
    }
}

void Being::optionChanged(const std::string &value)
{
    if (mType == PLAYER && value == "visiblenames")
        setShowName(config.getBoolValue("visiblenames"));
}

void Being::flashName(const int time)
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
        return std::string(" ").append(str);
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
            str.append("$");
        if (mAway)
        {
            // TRANSLATORS: this away status writed in player nick
            str.append(_("A"));
        }
        else if (mInactive)
        {
            // TRANSLATORS: this inactive status writed in player nick
            str.append(_("I"));
        }
    }
    return str;
}

void Being::showName()
{
    if (mName.empty())
        return;

    delete mDispName;
    mDispName = nullptr;

    if (mHideErased && player_relations.getRelation(mName) ==
        PlayerRelation::ERASED)
    {
        return;
    }

    std::string displayName(mName);

    if (mType != MONSTER && (mShowGender || mShowLevel))
    {
        displayName.append(" ");
        if (mShowLevel && getLevel() != 0)
            displayName.append(toString(getLevel()));

        displayName.append(getGenderSign());
    }

    if (mType == MONSTER)
    {
        if (config.getBoolValue("showMonstersTakedDamage"))
            displayName.append(", ").append(toString(getDamageTaken()));
    }

    gcn::Font *font = nullptr;
    if (player_node && player_node->getTarget() == this
        && mType != MONSTER)
    {
        font = boldFont;
    }
    else if (mType == PLAYER && !player_relations.isGoodName(this) && gui)
    {
        font = gui->getSecureFont();
    }

    mDispName = new FlashText(displayName, getPixelX(), getPixelY(),
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
            mTextColor = &Theme::getThemeColor(Theme::PLAYER);

            if (player_relations.getRelation(mName) != PlayerRelation::ERASED)
                mErased = false;
            else
                mErased = true;

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
            else if (player_node && getGuild()
                     && getGuild() == player_node->getGuild())
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

void Being::setSprite(const unsigned int slot, const int id,
                      std::string color, const unsigned char colorId,
                      const bool isWeapon, const bool isTempSprite)
{
    if (slot >= Net::getCharServerHandler()->maxSprite())
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
            mEquippedWeapon = nullptr;
        const ItemInfo &info = ItemDB::get(mSpriteIDs[slot]);
        if (mMap)
        {
            const int pet = info.getPet();
            if (pet)
                removePet();
        }
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string filename = info.getSprite(mGender, mSubType);
        AnimatedSprite *equipmentSprite = nullptr;

        if (mType == PLAYER)
        {
            const int pet = info.getPet();
            if (pet)
                addPet(pet);
        }

        if (!filename.empty())
        {
            if (color.empty())
                color = info.getDyeColorsString(colorId);

            equipmentSprite = AnimatedSprite::delayedLoad(
                paths.getStringValue("sprites").append(
                combineDye(filename, color)));
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

void Being::setSpriteID(const unsigned int slot, const int id)
{
    setSprite(slot, id, mSpriteColors[slot]);
}

void Being::setSpriteColor(const unsigned int slot, const std::string &color)
{
    setSprite(slot, mSpriteIDs[slot], color);
}

void Being::setHairStyle(const unsigned int slot, const int id)
{
//    dumpSprites();
    setSprite(slot, id, ItemDB::get(id).getDyeColorsString(mHairColor));
//    dumpSprites();
}

void Being::setHairColor(const unsigned int slot, const unsigned char color)
{
    mHairColor = color;
    setSprite(slot, mSpriteIDs[slot], ItemDB::get(
        getSpriteID(slot)).getDyeColorsString(color));
}

void Being::dumpSprites() const
{
    std::vector<int>::const_iterator it1 = mSpriteIDs.begin();
    const std::vector<int>::const_iterator it1_end = mSpriteIDs.end();
    StringVectCIter it2 = mSpriteColors.begin();
    const StringVectCIter it2_end = mSpriteColors.end();
    std::vector<int>::const_iterator it3 = mSpriteColorsIds.begin();
    const std::vector<int>::const_iterator it3_end = mSpriteColorsIds.end();

    logger->log("sprites");
    for (; it1 != it1_end && it2 != it2_end && it3 != it3_end;
         ++ it1, ++ it2, ++ it3)
    {
        logger->log("%d,%s,%d", *it1, (*it2).c_str(), *it3);
    }
}

void Being::load()
{
    // Hairstyles are encoded as negative numbers. Count how far negative
    // we can go.
    int hairstyles = 1;
    while (ItemDB::get(-hairstyles).getSprite(GENDER_MALE, 0) !=
           paths.getStringValue("spriteErrorFile"))
    {
        hairstyles ++;
    }
    mNumberOfHairstyles = hairstyles;

    int races = 100;
    while (ItemDB::get(-races).getSprite(GENDER_MALE, 0) !=
           paths.getStringValue("spriteErrorFile"))
    {
        races ++;
    }
    mNumberOfRaces = races - 100;
}

void Being::updateName()
{
    if (mShowName)
        showName();
}

void Being::reReadConfig()
{
    BLOCK_START("Being::reReadConfig")
    if (mUpdateConfigTime + 1 < cur_time)
    {
        mAwayEffect = paths.getIntValue("afkEffectId");
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
        mHideErased = config.getBoolValue("hideErased");
        mMoveNames = config.getBoolValue("moveNames");

        mUpdateConfigTime = cur_time;
    }
    BLOCK_END("Being::reReadConfig")
}

bool Being::updateFromCache()
{
    const BeingCacheEntry *const entry = Being::getCacheEntry(getId());

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
        if (mAdvanced)
        {
            const int flags = entry->getFlags();
            mShop = ((flags & FLAG_SHOP) != 0);
            mAway = ((flags & FLAG_AWAY) != 0);
            mInactive = ((flags & FLAG_INACTIVE) != 0);
            if (mShop || mAway || mInactive)
                updateName();
        }
        else
        {
            mShop = false;
            mAway = false;
            mInactive = false;
        }

        updateAwayEffect();
        if (mType == PLAYER)
            updateColors();
        return true;
    }
    return false;
}

void Being::addToCache() const
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

BeingCacheEntry* Being::getCacheEntry(const int id)
{
    FOR_EACH (std::list<BeingCacheEntry*>::iterator, i, beingInfoCache)
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
    return nullptr;
}


void Being::setGender(const Gender gender)
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

void Being::setGM(const bool gm)
{
    mIsGM = gm;

    updateColors();
}

void Being::talkTo() const
{
    if (!Client::limitPackets(PACKET_NPC_TALK))
        return;

    Net::getNpcHandler()->talk(mId);
}

bool Being::draw(Graphics *const graphics,
                 const int offsetX, const int offsetY) const
{
    bool res = true;
    if (!mErased)
        res = ActorSprite::draw(graphics, offsetX, offsetY);

    return res;
}

void Being::drawSprites(Graphics *const graphics,
                        const int posX, const int posY) const
{
    const int sz = getNumberOfLayers();
    for (int f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        Sprite *const sprite = getSprite(mSpriteRemap[f]);
        if (sprite)
        {
            sprite->setAlpha(mAlpha);
            sprite->draw(graphics, posX, posY);
        }
    }
}

void Being::drawSpritesSDL(Graphics *const graphics,
                           const int posX, const int posY) const
{
    const size_t sz = size();
    for (unsigned f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        const Sprite *const sprite = getSprite(mSpriteRemap[f]);
        if (sprite)
            sprite->draw(graphics, posX, posY);
    }
}

bool Being::drawSpriteAt(Graphics *const graphics,
                         const int x, const int y) const
{
    bool res = true;

    if (!mErased)
        res = ActorSprite::drawSpriteAt(graphics, x, y);

    if (!userPalette)
        return res;

    if (mHighlightMapPortals && mMap && mSubType == 45 && !mMap->getHasWarps())
    {
        graphics->setColor(userPalette->
                getColorWithAlpha(UserPalette::PORTAL_HIGHLIGHT));

        graphics->fillRectangle(gcn::Rectangle(x, y, 32, 32));

        if (mDrawHotKeys && !mName.empty())
        {
            gcn::Font *const font = gui->getFont();
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
        if (!maxHP)
            maxHP = mInfo->getMaxHP();

        drawHpBar(graphics, maxHP, mHP, mDamageTaken,
                  UserPalette::MONSTER_HP, UserPalette::MONSTER_HP2,
                  x - 50 + 16, y + 32 - 6, 2 * 50, 4);
    }
    if (mShowOwnHP && player_node == this && mAction != DEAD)
    {
        drawHpBar(graphics, PlayerInfo::getAttribute(PlayerInfo::MAX_HP),
                  PlayerInfo::getAttribute(PlayerInfo::HP), 0,
                  UserPalette::PLAYER_HP, UserPalette::PLAYER_HP2,
                  x - 50 + 16, y + 32 - 6, 2 * 50, 4);
    }
    return res;
}

void Being::drawHpBar(Graphics *const graphics, const int maxHP, const int hp,
                      const int damage, const int color1, const int color2,
                      const int x, const int y, const int width,
                      const int height) const
{
    if (maxHP <= 0 || !userPalette)
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

    const int dx = static_cast<const int>(static_cast<float>(width) / p);

    if (serverVersion < 1)
    {   // old servers
        if ((!damage && (this != player_node || hp == maxHP))
            || (!hp && maxHP == damage))
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
    }
    else
    {   // evol servers
        if (hp == maxHP)
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
    }

    graphics->setColor(userPalette->getColorWithAlpha(color1));
    graphics->fillRectangle(gcn::Rectangle(
        x, y, dx, height));

    graphics->setColor(userPalette->getColorWithAlpha(color2));
    graphics->fillRectangle(gcn::Rectangle(
        x + dx, y, width - dx, height));
}

void Being::setHP(const int hp)
{
    mHP = hp;
    if (mMaxHP < mHP)
        mMaxHP = mHP;
    if (mType == MONSTER)
        updatePercentHP();
}

void Being::setMaxHP(const int hp)
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
    const unsigned sz = static_cast<unsigned>(size());
    if (sz < 1)
        return;

    std::vector<int> slotRemap;
    std::map<int, int> itemSlotRemap;

    std::vector<int>::iterator it;
    int oldHide[20];
    int dir = mSpriteDirection;
    if (dir < 0 || dir >= 9)
        dir = 0;
    // hack for allow different logic in dead player
    if (mAction == DEAD)
        dir = 9;

    const unsigned int hairSlot = Net::getCharServerHandler()->hairSprite();

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

        const int id = mSpriteIDs[slot];
        if (!id)
            continue;

        const ItemInfo &info = ItemDB::get(id);

        if (info.isRemoveSprites())
        {
            SpriteToItemMap *const spriteToItems
                = info.getSpriteToItemReplaceMap(dir);

            if (spriteToItems)
            {
                FOR_EACHP (SpriteToItemMapCIter, itr, spriteToItems)
                {
                    const int remSprite = itr->first;
                    const std::map<int, int> &itemReplacer = itr->second;
                    if (remSprite >= 0)
                    {   // slot known
                        if (itemReplacer.empty())
                        {
                            mSpriteHide[remSprite] = 1;
                        }
                        else
                        {
                            std::map<int, int>::const_iterator repIt
                                = itemReplacer.find(mSpriteIDs[remSprite]);
                            if (repIt == itemReplacer.end())
                            {
                                repIt = itemReplacer.find(0);
                                if (repIt->second == 0)
                                    repIt = itemReplacer.end();
                            }
                            if (repIt != itemReplacer.end())
                            {
                                mSpriteHide[remSprite] = repIt->second;
                                if (repIt->second != 1)
                                {
                                    if (static_cast<unsigned>(remSprite)
                                        != hairSlot)
                                    {
                                        setSprite(remSprite, repIt->second,
                                            mSpriteColors[remSprite],
                                            1, false, true);
                                    }
                                    else
                                    {
                                        setSprite(remSprite, repIt->second,
                                            ItemDB::get(repIt->second)
                                            .getDyeColorsString(mHairColor),
                                            1, false, true);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {   // slot unknown. Search for real slot, this can be slow
                        FOR_EACH (IntMapCIter, repIt, itemReplacer)
                        {
                            for (unsigned slot2 = 0; slot2 < sz; slot2 ++)
                            {
                                if (mSpriteIDs[slot2] == repIt->first)
                                {
                                    mSpriteHide[slot2] = repIt->second;
                                    if (repIt->second != 1)
                                    {
                                        if (slot2 != hairSlot)
                                        {
                                            setSprite(slot2, repIt->second,
                                                mSpriteColors[slot2],
                                                1, false, true);
                                        }
                                        else
                                        {
                                            setSprite(slot2, repIt->second,
                                                ItemDB::get(repIt->second)
                                                .getDyeColorsString(
                                                mHairColor),
                                                1, false, true);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (info.mDrawBefore[dir] > 0)
        {
            const int id2 = mSpriteIDs[info.mDrawBefore[dir]];
            if (itemSlotRemap.find(id2) != itemSlotRemap.end())
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
        }
        else if (info.mDrawAfter[dir] > 0)
        {
            const int id2 = mSpriteIDs[info.mDrawAfter[dir]];
            if (itemSlotRemap.find(id2) != itemSlotRemap.end())
            {
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
//       logger->log("item slot->slot %d %d->%d", id, slot, itemSlotRemap[id]);
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
            const int slot = searchSlotValue(slotRemap, slot0);
            const int val = slotRemap.at(slot);
            int id = 0;

            if (static_cast<int>(mSpriteIDs.size()) > val)
                id = mSpriteIDs[val];

            int idx = -1;
            int idx1 = -1;
//            logger->log("item %d, id=%d", slot, id);
            int reorder = 0;
            const std::map<int, int>::const_iterator
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
            const int id = mSpriteIDs[slot];
            if (!id)
                continue;

            setSprite(slot, id, mSpriteColors[slot], 1, false, true);
        }
//        logger->log("slot %d = %d", slot, mSpriteRemap[slot]);
    }
}

int Being::searchSlotValue(const std::vector<int> &slotRemap,
                           const int val) const
{
    for (unsigned slot = 0; slot < size(); slot ++)
    {
        if (slotRemap[slot] == val)
            return slot;
    }
    return getNumberOfLayers() - 1;
}

void Being::searchSlotValueItr(std::vector<int>::iterator &it, int &idx,
                               std::vector<int> &slotRemap,
                               const int val) const
{
//    logger->log("searching %d", val);
    it = slotRemap.begin();
    const std::vector<int>::iterator it_end = slotRemap.end();
    idx = 0;
    while (it != it_end)
    {
//        logger->log("testing %d", *it);
        if (*it == val)
        {
//            logger->log("found at %d", idx);
            return;
        }
        ++ it;
        idx ++;
    }
//    logger->log("not found");
    idx = -1;
    return;
}

void Being::updateHit(const int amount)
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
    Equipment *const eq = new Equipment();
    Equipment::Backend *const bk = new BeingEquipBackend(this);
    eq->setBackend(bk);
    return eq;
}

void Being::undressItemById(const int id)
{
    const size_t sz = mSpriteIDs.size();

    for (size_t f = 0; f < sz; f ++)
    {
        if (id == mSpriteIDs[f])
        {
            setSprite(static_cast<unsigned int>(f), 0);
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

std::string Being::loadComment(const std::string &name, const int type)
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

    str.append(stringToHexPath(name)).append("/comment.txt");
    logger->log("load from: %s", str.c_str());
    StringVect lines;

    const ResourceManager *const resman = ResourceManager::getInstance();
    if (resman->existsLocal(str))
    {
        lines = resman->loadTextFileLocal(str);
        if (lines.size() >= 2)
            return lines[1];
    }
    return "";
}

void Being::saveComment(const std::string &name,
                        const std::string &comment, const int type)
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
    dir.append(stringToHexPath(name));
    const ResourceManager *const resman = ResourceManager::getInstance();
    resman->saveTextFile(dir, "comment.txt",
        (name + "\n").append(comment));
}

void Being::setState(const uint8_t state)
{
    const bool shop = ((state & FLAG_SHOP) != 0);
    const bool away = ((state & FLAG_AWAY) != 0);
    const bool inactive = ((state & FLAG_INACTIVE) != 0);
    const bool needUpdate = (shop != mShop || away != mAway
        || inactive != mInactive);

    mShop = shop;
    mAway = away;
    mInactive = inactive;
    updateAwayEffect();

    if (needUpdate)
    {
        if (shop || away || inactive)
            mAdvanced = true;
        updateName();
        addToCache();
    }
}

void Being::setEmote(const uint8_t emotion, const int emote_time)
{
    if ((emotion & FLAG_SPECIAL) == FLAG_SPECIAL)
    {
        setState(emotion);
        mAdvanced = true;
    }
    else
    {
        const int emotionIndex = emotion - 1;
        if (emotionIndex >= 0 && emotionIndex <= EmoteDB::getLast())
        {
            delete mEmotionSprite;
            mEmotionSprite = nullptr;
            const EmoteInfo *const info = EmoteDB::get2(emotionIndex, true);
            if (info)
            {
                const EmoteSprite *const sprite = info->sprites.front();
                if (sprite)
                {
                    mEmotionSprite = AnimatedSprite::clone(sprite->sprite);
                    if (mEmotionSprite)
                        mEmotionTime = info->time;
                    else
                        mEmotionTime = emote_time;
                }
            }
        }

        if (mEmotionSprite)
        {
            mEmotionSprite->play(mSpriteAction);
            mEmotionSprite->setSpriteDirection(static_cast<SpriteDirection>(
                mSpriteDirection));
        }
        else
        {
            mEmotionTime = 0;
        }
    }
}

void Being::updatePercentHP()
{
    if (!mMaxHP || !serverVersion)
        return;
    if (mHP)
    {
        const unsigned num = mHP * 100 / mMaxHP;
        if (num != mNumber)
        {
            mNumber = num;
            if (updateNumber(mNumber))
                setAction(mAction);
        }
    }
}

uint8_t Being::genderToInt(const Gender sex)
{
    switch (sex)
    {
        case GENDER_FEMALE:
        case GENDER_UNSPECIFIED:
        default:
            return 0;
        case GENDER_MALE:
            return 1;
        case GENDER_OTHER:
            return 3;
    }
}

Gender Being::intToGender(const uint8_t sex)
{
    switch (sex)
    {
        case 0:
        default:
            return GENDER_FEMALE;
        case 1:
            return GENDER_MALE;
        case 3:
            return GENDER_OTHER;
    }
}

int Being::getSpriteID(const int slot) const
{
    if (slot < 0 || static_cast<unsigned>(slot) >= mSpriteIDs.size())
        return -1;

    return mSpriteIDs[slot];
}

void Being::addAfkEffect()
{
    addSpecialEffect(mAwayEffect);
}

void Being::removeAfkEffect()
{
    removeSpecialEffect();
}

void Being::addSpecialEffect(const int effect)
{
    if (effectManager && Particle::enabled
        && !mSpecialParticle && effect != -1)
    {
        mSpecialParticle = effectManager->triggerReturn(effect, this);
    }
}

void Being::removeSpecialEffect()
{
    if (effectManager && mSpecialParticle)
    {
        mChildParticleEffects.removeLocally(mSpecialParticle);
        mSpecialParticle = nullptr;
    }
    delete mAnimationEffect;
    mAnimationEffect = nullptr;
}

void Being::updateAwayEffect()
{
    if (mAway)
        addAfkEffect();
    else
        removeAfkEffect();
}

void Being::addEffect(const std::string &name)
{
    delete mAnimationEffect;
    mAnimationEffect = AnimatedSprite::load(
        paths.getStringValue("sprites") + name);
}

void Being::addPet(const int id)
{
    if (!actorSpriteManager)
        return;

    removePet();
    Being *const being = actorSpriteManager->createBeing(
        id, ActorSprite::PET, 0);
    if (being)
    {
        being->setTileCoords(getTileX(), getTileY());
        being->setOwner(this);
        mPetId = id;
        mPet = being;
    }
}

void Being::removePet()
{
    if (!actorSpriteManager)
        return;

    mPetId = 0;
    if (mPet)
    {
        mPet->setOwner(nullptr);
        actorSpriteManager->destroy(mPet);
        mPet = nullptr;
    }
}

void Being::updatePets()
{
    removePet();
    FOR_EACH (std::vector<int>::const_iterator, it, mSpriteIDs)
    {
        const ItemInfo &info = ItemDB::get(*it);
        const int pet = info.getPet();
        if (pet)
        {
            addPet(pet);
            return;
        }
    }
}

void Being::playSfx(const SoundInfo &sound, Being *const being,
                    const bool main, const int x, const int y)
{
    if (being)
    {
        // here need add timer and delay sound
        const int time = tick_time;
        if (main)
        {
            being->mNextSound.sound = nullptr;
            being->mNextSound.time = time + sound.delay;
            being->mNextSound.sound = nullptr;
            soundManager.playSfx(sound.sound, x, y);
        }
        else if (mNextSound.time <= time)
        {   // old event sound time is gone. we can play new sound
            being->mNextSound.sound = nullptr;
            being->mNextSound.time = time + sound.delay;
            being->mNextSound.sound = nullptr;
            soundManager.playSfx(sound.sound, x, y);
        }
        else
        {   // old event sound in progress. need save sound and wait
            being->mNextSound.sound = &sound;
            being->mNextSound.x = x;
            being->mNextSound.y = y;
        }
    }
    else
    {
        soundManager.playSfx(sound.sound, x, y);
    }
}
