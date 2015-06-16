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

#include "being/being.h"

#include "actormanager.h"
#include "animatedsprite.h"
#include "beingequipbackend.h"
#include "configuration.h"
#include "effectmanager.h"
#include "guild.h"
#include "party.h"
#include "settings.h"
#include "soundmanager.h"
#include "text.h"

#include "being/beingcacheentry.h"
#include "being/beingflag.h"
#include "being/beingspeech.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "enums/being/beingdirection.h"

#include "particle/particleinfo.h"

#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/fonts/font.h"

#include "gui/popups/speechbubble.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"

#include "net/charserverhandler.h"
#include "net/gamehandler.h"
#include "net/npchandler.h"
#include "net/packetlimiter.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "resources/attack.h"
#include "resources/chatobject.h"
#include "resources/emoteinfo.h"
#include "resources/emotesprite.h"
#include "resources/horseinfo.h"
#include "resources/iteminfo.h"
#include "resources/spriteaction.h"

#include "resources/db/avatardb.h"
#include "resources/db/emotedb.h"
#include "resources/db/homunculusdb.h"
#include "resources/db/horsedb.h"
#include "resources/db/itemdb.h"
#include "resources/db/mercenarydb.h"
#include "resources/db/monsterdb.h"
#include "resources/db/npcdb.h"
#include "resources/db/petdb.h"

#include "resources/map/map.h"

#include "gui/widgets/createwidget.h"
#include "gui/widgets/skilldata.h"
#include "gui/widgets/skillinfo.h"

#include "gui/widgets/tabs/chat/langtab.h"

#include "utils/delete2.h"
#include "utils/files.h"
#include "utils/gettext.h"
#include "utils/timer.h"

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
Move Being::mMoveNames = Move_false;
bool Being::mUseDiagonal = true;
int Being::mAwayEffect = -1;

std::list<BeingCacheEntry*> beingInfoCache;
typedef std::map<int, Guild*>::const_iterator GuildsMapCIter;
typedef std::map<int, int>::const_iterator IntMapCIter;

Being::Being(const BeingId id,
             const ActorTypeT type,
             const BeingTypeId subtype,
             Map *const map) :
    ActorSprite(id),
    mNextSound(),
    mInfo(BeingInfo::unknown),
    mEmotionSprite(nullptr),
    mAnimationEffect(nullptr),
    mSpriteAction(SpriteAction::STAND),
    mName(),
    mRaceName(),
    mPartyName(),
    mGuildName(),
    mSpeech(),
    mDispName(nullptr),
    mNameColor(nullptr),
    mEquippedWeapon(nullptr),
    mPath(),
    mText(nullptr),
    mTextColor(nullptr),
    mDest(),
    mSpriteColors(),
    mSpriteIDs(),
    mSpriteColorsIds(),
    mSpriteParticles(),
    mGuilds(),
    mParty(nullptr),
    mActionTime(0),
    mEmotionTime(0),
    mSpeechTime(0),
    mAttackSpeed(350),
    mLevel(0),
    mAttackRange(1),
    mLastAttackX(0),
    mLastAttackY(0),
    mPreStandTime(0),
    mGender(Gender::UNSPECIFIED),
    mAction(BeingAction::STAND),
    mSubType(fromInt(0xFFFF, BeingTypeId)),
    mDirection(BeingDirection::DOWN),
    mDirectionDelayed(0),
    mSpriteDirection(SpriteDirection::DOWN),
    mShowName(false),
    mIsGM(false),
    mType(type),
    mSpeechBubble(nullptr),
    mWalkSpeed(playerHandler->getDefaultWalkSpeed()),
    mSpeed(playerHandler->getDefaultWalkSpeed().x),
    mIp(),
    mSpriteRemap(new int[20]),
    mSpriteHide(new int[20]),
    mSpriteDraw(new int[20]),
    mComment(),
#ifdef EATHENA_SUPPORT
    mBuyBoard(),
    mSellBoard(),
#endif
    mPets(),
    mOwner(nullptr),
    mSpecialParticle(nullptr),
#ifdef EATHENA_SUPPORT
    mChat(nullptr),
    mHorseInfo(nullptr),
    mHorseSprite(nullptr),
#endif
    mX(0),
    mY(0),
    mSortOffsetY(0),
    mOffsetY(0),
    mFixedOffsetY(0),
    mOldHeight(0),
    mDamageTaken(0),
    mHP(0),
    mMaxHP(0),
    mDistance(0),
    mReachable(Reachable::REACH_UNKNOWN),
    mGoodStatus(-1),
    mMoveTime(0),
    mAttackTime(0),
    mTalkTime(0),
    mOtherTime(0),
    mTestTime(cur_time),
    mAttackDelay(0),
    mMinHit(0),
    mMaxHit(0),
    mCriticalHit(0),
    mPvpRank(0),
    mNumber(100),
    mUsageCounter(1),
    mKarma(0),
    mManner(0),
    mAreaSize(11),
    mLook(0U),
    mHairColor(0),
    mErased(false),
    mEnemy(false),
    mGotComment(false),
    mAdvanced(false),
    mShop(false),
    mAway(false),
    mInactive(false),
    mNeedPosUpdate(true),
    mPetAi(true)
{
    for (int f = 0; f < 20; f ++)
    {
        mSpriteRemap[f] = f;
        mSpriteHide[f] = 0;
        mSpriteDraw[f] = 0;
    }

    setMap(map);
    setSubtype(subtype, 0);

    if (mType == ActorType::Player
#ifdef EATHENA_SUPPORT
        || mType == ActorType::Mercenary
        || mType == ActorType::Pet
        || mType == ActorType::Homunculus
#endif
    )
    {
        mShowName = config.getBoolValue("visiblenames");
    }
    else if (mType != ActorType::Npc)
    {
        mGotComment = true;
    }

    if (mType == ActorType::Portal)
        mShowName = false;

    config.addListener("visiblenames", this);

    reReadConfig();

    if (mType == ActorType::Npc)
        setShowName(true);
    else
        setShowName(mShowName);

    updateColors();
    updatePercentHP();
}

Being::~Being()
{
    config.removeListener("visiblenames", this);
    CHECKLISTENERS

    delete [] mSpriteRemap;
    mSpriteRemap = nullptr;
    delete [] mSpriteHide;
    mSpriteHide = nullptr;
    delete [] mSpriteDraw;
    mSpriteDraw = nullptr;

    delete2(mSpeechBubble);
    delete2(mDispName);
    delete2(mText);
    delete2(mEmotionSprite);
    delete2(mAnimationEffect);
#ifdef EATHENA_SUPPORT
    delete2(mChat);
#endif

    if (mOwner)
    {
        if (mType == ActorType::LocalPet)
            mOwner->unassignPet(this);

        mOwner = nullptr;
    }
    FOR_EACH (std::vector<Being*>::iterator, it, mPets)
    {
        Being *pet = *it;
        if (pet)
        {
            pet->setOwner(nullptr);
            actorManager->erase(pet);
            delete pet;
        }
    }
    mPets.clear();

    removeAllItemsParticles();
}

void Being::createSpeechBubble()
{
    CREATEWIDGETV0(mSpeechBubble, SpeechBubble);
}

void Being::setSubtype(const BeingTypeId subtype,
                       const uint16_t look)
{
    if (!mInfo)
        return;

    if (subtype == mSubType && mLook == look)
        return;

    mSubType = subtype;
    mLook = look;

    if (mType == ActorType::Monster)
    {
        mInfo = MonsterDB::get(mSubType);
        if (mInfo)
        {
            setName(mInfo->getName());
            setupSpriteDisplay(mInfo->getDisplay(),
                ForceDisplay_true,
                0,
                mInfo->getColor(mLook));
            mYDiff = mInfo->getSortOffsetY();
        }
    }
#ifdef EATHENA_SUPPORT
    if (mType == ActorType::Pet)
    {
        mInfo = PETDB::get(mSubType);
        if (mInfo)
        {
            setName(mInfo->getName());
            setupSpriteDisplay(mInfo->getDisplay(),
                ForceDisplay_true,
                0,
                mInfo->getColor(mLook));
            mYDiff = mInfo->getSortOffsetY();
        }
    }
    else if (mType == ActorType::Mercenary)
    {
        mInfo = MercenaryDB::get(mSubType);
        if (mInfo)
        {
            setName(mInfo->getName());
            setupSpriteDisplay(mInfo->getDisplay(),
                ForceDisplay_true,
                0,
                mInfo->getColor(mLook));
            mYDiff = mInfo->getSortOffsetY();
        }
    }
    if (mType == ActorType::Homunculus)
    {
        mInfo = HomunculusDB::get(mSubType);
        if (mInfo)
        {
            setName(mInfo->getName());
            setupSpriteDisplay(mInfo->getDisplay(),
                ForceDisplay_true,
                0,
                mInfo->getColor(mLook));
            mYDiff = mInfo->getSortOffsetY();
        }
    }
#endif
    else if (mType == ActorType::Npc)
    {
        mInfo = NPCDB::get(mSubType);
        if (mInfo)
        {
            setupSpriteDisplay(mInfo->getDisplay(), ForceDisplay_false);
            mYDiff = mInfo->getSortOffsetY();
        }
    }
    else if (mType == ActorType::Avatar)
    {
        mInfo = AvatarDB::get(mSubType);
        if (mInfo)
            setupSpriteDisplay(mInfo->getDisplay(), ForceDisplay_false);
    }
    else if (mType == ActorType::LocalPet)
    {
        mInfo = PETDB::get(fromInt(mId, BeingTypeId));
        if (mInfo)
        {
            setName(mInfo->getName());
            setupSpriteDisplay(mInfo->getDisplay(), ForceDisplay_false);
            mYDiff = mInfo->getSortOffsetY();
            const int speed = mInfo->getWalkSpeed();
            if (!speed)
                setWalkSpeed(playerHandler->getDefaultWalkSpeed());
            else
                setWalkSpeed(Vector(speed, speed, 0));
        }
    }
    else if (mType == ActorType::Player)
    {
        int id = -100 - toInt(subtype, int);

        // Prevent showing errors when sprite doesn't exist
        if (!ItemDB::exists(id))
        {
            id = -100;
            // TRANSLATORS: default race name
            setRaceName(_("Human"));
            if (charServerHandler)
                setSprite(charServerHandler->baseSprite(), id);
        }
        else
        {
            const ItemInfo &info = ItemDB::get(id);
            setRaceName(info.getName());
            if (charServerHandler)
            {
                setSprite(charServerHandler->baseSprite(),
                    id, info.getColor(mLook));
            }
        }
    }
}

TargetCursorSizeT Being::getTargetCursorSize() const
{
    if (!mInfo)
        return TargetCursorSize::SMALL;

    return mInfo->getTargetCursorSize();
}

void Being::setPosition(const Vector &pos)
{
    Actor::setPosition(pos);

    updateCoords();

    if (mText)
    {
        mText->adviseXY(static_cast<int>(pos.x),
            static_cast<int>(pos.y) - getHeight() - mText->getHeight() - 6,
            mMoveNames);
    }
}

void Being::setDestination(const int dstX, const int dstY)
{
    // We can't calculate anything without a map anyway.
    if (!mMap)
        return;

    setPath(mMap->findPath(mX, mY, dstX, dstY, getBlockWalkMask()));
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

    if (mAction != BeingAction::MOVE && mAction != BeingAction::DEAD)
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

    if (!channel.empty() && (!langChatTab || langChatTab->getChannelName()
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

    if (!time)
    {
        const size_t sz = mSpeech.size();
        if (sz < 200)
            time = static_cast<int>(SPEECH_TIME - 300 + (3 * sz));
    }

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
        position = mSpeech.find("@@");

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
    if (speech == BeingSpeech::TEXT_OVERHEAD && userPalette)
    {
        delete mText;
        mText = new Text(mSpeech,
            getPixelX(),
            getPixelY() - getHeight(),
            Graphics::CENTER,
            &userPalette->getColor(UserColorId::PARTICLE),
            Speech_true);
    }
    else
    {
        const bool isShowName = (speech == BeingSpeech::NAME_IN_BUBBLE);
        if (!mSpeechBubble)
            createSpeechBubble();
        mSpeechBubble->setCaption(isShowName ? mName : "");
        mSpeechBubble->setText(mSpeech, isShowName);
    }
}

void Being::takeDamage(Being *const attacker,
                       const int amount,
                       const AttackTypeT type,
                       const int attackId,
                       const int level)
{
    if (!userPalette || !attacker)
        return;

    BLOCK_START("Being::takeDamage1")

    Font *font = nullptr;
    // TRANSLATORS: hit or miss message in attacks
    const std::string damage = amount ? toString(amount)
        : type == AttackType::FLEE ? _("dodge") : _("miss");
    const Color *color;

    if (gui)
        font = gui->getInfoParticleFont();

    // Selecting the right color
    if (type == AttackType::CRITICAL || type == AttackType::FLEE)
    {
        if (type == AttackType::CRITICAL)
            attacker->setCriticalHit(amount);

        if (attacker == localPlayer)
        {
            color = &userPalette->getColor(
                UserColorId::HIT_LOCAL_PLAYER_CRITICAL);
        }
        else
        {
            color = &userPalette->getColor(UserColorId::HIT_CRITICAL);
        }
    }
    else if (!amount)
    {
        if (attacker == localPlayer)
        {
            // This is intended to be the wrong direction to visually
            // differentiate between hits and misses
            color = &userPalette->getColor(UserColorId::HIT_LOCAL_PLAYER_MISS);
        }
        else
        {
            color = &userPalette->getColor(UserColorId::MISS);
        }
    }
    else if (mType == ActorType::Monster
#ifdef EATHENA_SUPPORT
             || mType == ActorType::Mercenary
             || mType == ActorType::Pet
             || mType == ActorType::Homunculus
#endif
    )
    {
        if (attacker == localPlayer)
        {
            color = &userPalette->getColor(
                UserColorId::HIT_LOCAL_PLAYER_MONSTER);
        }
        else
        {
            color = &userPalette->getColor(
                UserColorId::HIT_PLAYER_MONSTER);
        }
    }
    else if (mType == ActorType::Player && attacker != localPlayer
             && this == localPlayer)
    {
        // here player was attacked by other player. mark him as enemy.
        color = &userPalette->getColor(UserColorId::HIT_PLAYER_PLAYER);
        attacker->setEnemy(true);
        attacker->updateColors();
    }
    else
    {
        color = &userPalette->getColor(UserColorId::HIT_MONSTER_PLAYER);
    }

    if (chatWindow && mShowBattleEvents)
    {
        if (this == localPlayer)
        {
            if (attacker->mType == ActorType::Player || amount)
            {
                chatWindow->battleChatLog(strprintf("%s : Hit you  -%d",
                    attacker->getName().c_str(), amount),
                    ChatMsgType::BY_OTHER);
            }
        }
        else if (attacker == localPlayer && amount)
        {
            chatWindow->battleChatLog(strprintf("%s : You hit %s -%d",
                attacker->getName().c_str(), getName().c_str(), amount),
                ChatMsgType::BY_PLAYER);
        }
    }
    if (font && particleEngine)
    {
        // Show damage number
        particleEngine->addTextSplashEffect(damage,
            getPixelX(), getPixelY() - 16, color, font, true);
    }
    BLOCK_END("Being::takeDamage1")
    BLOCK_START("Being::takeDamage2")

    if (type != AttackType::SKILL)
        attacker->updateHit(amount);

    if (amount > 0)
    {
        if (localPlayer && localPlayer == this)
            localPlayer->setLastHitFrom(attacker->getName());

        mDamageTaken += amount;
        if (mInfo)
        {
            playSfx(mInfo->getSound(ItemSoundEvent::HURT),
                this,
                false,
                mX,
                mY);

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

        if (mType == ActorType::Monster)
        {
            updatePercentHP();
            updateName();
        }
        else if (mType == ActorType::Player && socialWindow && getName() != "")
        {
            socialWindow->updateAvatar(getName());
        }

        if (effectManager)
        {
            const int hitEffectId = getHitEffect(attacker,
                type,
                attackId,
                level);
            if (hitEffectId >= 0)
                effectManager->trigger(hitEffectId, this);
        }
    }
    else
    {
        if (effectManager)
        {
            int hitEffectId = -1;
            if (type == AttackType::SKILL)
            {
                hitEffectId = getHitEffect(attacker,
                    AttackType::SKILLMISS,
                    attackId,
                    level);
            }
            else
            {
                hitEffectId = getHitEffect(attacker,
                    AttackType::MISS,
                    attackId,
                    level);
            }
            if (hitEffectId >= 0)
                effectManager->trigger(hitEffectId, this);
        }
    }
    BLOCK_END("Being::takeDamage2")
}

int Being::getHitEffect(const Being *const attacker,
                        const AttackTypeT type,
                        const int attackId,
                        const int level) const
{
    if (!effectManager)
        return 0;

    BLOCK_START("Being::getHitEffect")
    // Init the particle effect path based on current
    // weapon or default.
    int hitEffectId = 0;
    if (type == AttackType::SKILL || type == AttackType::SKILLMISS)
    {
        SkillData *const data = skillDialog->getSkillDataByLevel(
            attackId, level);
        if (!data)
            return -1;
        if (type == AttackType::SKILL)
        {
            hitEffectId = data->hitEffectId;
            if (hitEffectId == -1)
                hitEffectId = paths.getIntValue("skillHitEffectId");
        }
        else
        {
            hitEffectId = data->missEffectId;
            if (hitEffectId == -1)
                hitEffectId = paths.getIntValue("skillMissEffectId");
        }
    }
    else
    {
        if (attacker)
        {
            const ItemInfo *const attackerWeapon
                = attacker->getEquippedWeapon();
            if (attackerWeapon && attacker->getType() == ActorType::Player)
            {
                if (type == AttackType::MISS)
                    hitEffectId = attackerWeapon->getMissEffectId();
                else if (type != AttackType::CRITICAL)
                    hitEffectId = attackerWeapon->getHitEffectId();
                else
                    hitEffectId = attackerWeapon->getCriticalHitEffectId();
            }
            else if (attacker->getType() == ActorType::Monster)
            {
                const BeingInfo *const info = attacker->getInfo();
                if (info)
                {
                    const Attack *const atk = info->getAttack(attackId);
                    if (atk)
                    {
                        if (type == AttackType::MISS)
                            hitEffectId = atk->mMissEffectId;
                        else if (type != AttackType::CRITICAL)
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
    BLOCK_END("Being::getHitEffect")
    return hitEffectId;
}

int Being::getDefaultEffectId(const AttackTypeT &type)
{
    if (type == AttackType::MISS)
        return paths.getIntValue("missEffectId");
    else if (type != AttackType::CRITICAL)
        return paths.getIntValue("hitEffectId");
    else
        return paths.getIntValue("criticalHitEffectId");
}

void Being::handleAttack(Being *const victim, const int damage,
                         const int attackId)
{
    if (!victim || !mInfo)
        return;

    BLOCK_START("Being::handleAttack")

    if (this != localPlayer)
        setAction(BeingAction::ATTACK, attackId);

    mLastAttackX = victim->getTileX();
    mLastAttackY = victim->getTileY();

    if (mType == ActorType::Player && mEquippedWeapon)
        fireMissile(victim, mEquippedWeapon->getMissileParticleFile());
    else if (mInfo->getAttack(attackId))
        fireMissile(victim, mInfo->getAttack(attackId)->mMissileParticle);

    reset();
    mActionTime = tick_time;

    if (!serverFeatures->haveAttackDirections() && this != localPlayer)
    {
        const uint8_t dir = calcDirection(victim->getTileX(),
            victim->getTileY());
        if (dir)
            setDirection(dir);
    }

    if (damage && victim->mType == ActorType::Player
        && victim->mAction == BeingAction::SIT)
    {
        victim->setAction(BeingAction::STAND, 0);
    }

    if (mType == ActorType::Player)
    {
        if (mSpriteIDs.size() >= 10)
        {
            // here 10 is weapon slot
            int weaponId = mSpriteIDs[10];
            if (!weaponId)
                weaponId = -100 - toInt(mSubType, int);
            const ItemInfo &info = ItemDB::get(weaponId);
            playSfx(info.getSound(
                (damage > 0) ? ItemSoundEvent::HIT : ItemSoundEvent::MISS),
                victim,
                true,
                mX, mY);
        }
    }
    else
    {
        playSfx(mInfo->getSound((damage > 0) ?
            ItemSoundEvent::HIT : ItemSoundEvent::MISS), victim, true, mX, mY);
    }
    BLOCK_END("Being::handleAttack")
}

void Being::handleSkill(Being *const victim, const int damage,
                        const int skillId, const int skillLevel)
{
    if (!victim || !mInfo || !skillDialog)
        return;

    if (this != localPlayer)
        setAction(BeingAction::ATTACK, 1);

    const SkillInfo *const skill = skillDialog->getSkill(skillId);
    const SkillData *const data = skill
        ? skill->getData1(skillLevel) : nullptr;
    if (data)
        fireMissile(victim, data->particle);

    reset();
    mActionTime = tick_time;

    if (!serverFeatures->haveAttackDirections() && this != localPlayer)
    {
        const uint8_t dir = calcDirection(victim->getTileX(),
            victim->getTileY());
        if (dir)
            setDirection(dir);
    }
    if (damage && victim->mType == ActorType::Player
        && victim->mAction == BeingAction::SIT)
    {
        victim->setAction(BeingAction::STAND, 0);
    }
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
            ItemSoundEvent::HIT : ItemSoundEvent::MISS), victim, true, mX, mY);
    }
}

void Being::setName(const std::string &name)
{
    if (mType == ActorType::Npc)
    {
        mName = name.substr(0, name.find('#', 0));
        showName();
    }
    else
    {
        if (mType == ActorType::Portal)
            mName = name.substr(0, name.find('#', 0));
        else
            mName = name;

        if (getShowName())
            showName();
    }
}

void Being::setShowName(const bool doShowName)
{
    if (mShowName == doShowName)
        return;

    mShowName = doShowName;

    if (doShowName)
        showName();
    else
        delete2(mDispName)
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

    if (this == localPlayer && socialWindow)
        socialWindow->addTab(guild);
}

void Being::removeGuild(const int id)
{
    if (this == localPlayer && socialWindow)
        socialWindow->removeTab(mGuilds[id]);

    if (mGuilds[id])
        mGuilds[id]->removeMember(getName());
    mGuilds.erase(id);
}

const Guild *Being::getGuild(const std::string &guildName) const
{
    FOR_EACH (GuildsMapCIter, itr, mGuilds)
    {
        const Guild *const guild = itr->second;
        if (guild && guild->getName() == guildName)
            return guild;
    }

    return nullptr;
}

const Guild *Being::getGuild(const int id) const
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
            if (this == localPlayer && socialWindow)
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

    if (this == localPlayer && socialWindow)
    {
        if (old)
            socialWindow->removeTab(old);

        if (party)
            socialWindow->addTab(party);
    }
}

void Being::updateGuild()
{
    if (!localPlayer)
        return;

    Guild *const guild = localPlayer->getGuild();
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

    if (this == localPlayer && socialWindow)
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

    BLOCK_START("Being::fireMissile")

    Particle *const target = particleEngine->createChild();

    if (!target)
    {
        BLOCK_END("Being::fireMissile")
        return;
    }

    Particle *const missile = target->addEffect(
        particle, getPixelX(), getPixelY());

    if (missile)
    {
        target->moveBy(Vector(0.0F, 0.0F, 32.0F));
        target->setLifetime(1000);
        victim->controlParticle(target);

        missile->setDestination(target, 7, 0);
        missile->setDieDistance(8);
        missile->setLifetime(900);
    }
    BLOCK_END("Being::fireMissile")
}

std::string Being::getSitAction() const
{
    if (mRiding)
        return SpriteAction::SITRIDE;
    if (mMap)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if (mask & BlockMask::GROUNDTOP)
            return SpriteAction::SITTOP;
        else if (mask & BlockMask::AIR)
            return SpriteAction::SITSKY;
        else if (mask & BlockMask::WATER)
            return SpriteAction::SITWATER;
    }
    return SpriteAction::SIT;
}


std::string Being::getMoveAction() const
{
    if (mRiding)
        return SpriteAction::RIDE;
    if (mMap)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if (mask & BlockMask::AIR)
            return SpriteAction::FLY;
        else if (mask & BlockMask::WATER)
            return SpriteAction::SWIM;
    }
    return SpriteAction::MOVE;
}

std::string Being::getWeaponAttackAction(const ItemInfo *const weapon) const
{
    if (!weapon)
        return SpriteAction::ATTACK;

    if (mRiding)
        return weapon->getRideAttackAction();
    if (mMap)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if (mask & BlockMask::AIR)
            return weapon->getSkyAttackAction();
        else if (mask & BlockMask::WATER)
            return weapon->getWaterAttackAction();
    }
    return weapon->getAttackAction();
}

std::string Being::getAttackAction(const Attack *const attack1) const
{
    if (!attack1)
        return SpriteAction::ATTACK;

    if (mMap)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if (mask & BlockMask::AIR)
            return attack1->mSkyAction;
        else if (mask & BlockMask::WATER)
            return attack1->mWaterAction;
    }
    return attack1->mAction;
}

#define getSpriteAction(func, action) \
    std::string Being::get##func##Action() const \
{ \
    if (mRiding) \
        return SpriteAction::action##RIDE; \
    if (mMap) \
    { \
        const unsigned char mask = mMap->getBlockMask(mX, mY); \
        if (mask & BlockMask::AIR) \
            return SpriteAction::action##SKY; \
        else if (mask & BlockMask::WATER) \
            return SpriteAction::action##WATER; \
    } \
    return SpriteAction::action; \
}

getSpriteAction(Dead, DEAD)
getSpriteAction(Stand, STAND)
getSpriteAction(Spawn, SPAWN)

void Being::setAction(const BeingActionT &action, const int attackId)
{
    std::string currentAction = SpriteAction::INVALID;

    switch (action)
    {
        case BeingAction::MOVE:
            if (mInfo)
            {
                playSfx(mInfo->getSound(
                    ItemSoundEvent::MOVE), nullptr, true, mX, mY);
            }
            currentAction = getMoveAction();
            // Note: When adding a run action,
            // Differentiate walk and run with action name,
            // while using only the ACTION_MOVE.
            break;
        case BeingAction::SIT:
            currentAction = getSitAction();
            if (mInfo)
            {
                ItemSoundEvent::Type event;
                if (currentAction == SpriteAction::SITTOP)
                    event = ItemSoundEvent::SITTOP;
                else
                    event = ItemSoundEvent::SIT;
                playSfx(mInfo->getSound(event), nullptr, true, mX, mY);
            }
            break;
        case BeingAction::ATTACK:
            if (mEquippedWeapon)
            {
                currentAction = getWeaponAttackAction(mEquippedWeapon);
                reset();
            }
            else
            {
                if (!mInfo || !mInfo->getAttack(attackId))
                    break;

                currentAction = getAttackAction(mInfo->getAttack(attackId));
                reset();

                // attack particle effect
                if (Particle::enabled && effectManager)
                {
                    const int effectId = mInfo->getAttack(attackId)->mEffectId;
                    if (effectId >= 0)
                    {
                        effectManager->triggerDirection(effectId,
                            this,
                            mSpriteDirection);
                    }
                }
            }
            break;
        case BeingAction::HURT:
            if (mInfo)
            {
                playSfx(mInfo->getSound(ItemSoundEvent::HURT),
                    this, false, mX, mY);
            }
            break;
        case BeingAction::DEAD:
            currentAction = getDeadAction();
            if (mInfo)
            {
                playSfx(mInfo->getSound(ItemSoundEvent::DIE),
                    this,
                    false,
                    mX, mY);
                if (mType == ActorType::Monster || mType == ActorType::Npc)
                    mYDiff = mInfo->getDeadSortOffsetY();
            }
            break;
        case BeingAction::STAND:
            currentAction = getStandAction();
            break;
        case BeingAction::SPAWN:
            if (mInfo)
            {
                playSfx(mInfo->getSound(ItemSoundEvent::SPAWN),
                    nullptr, true, mX, mY);
            }
            currentAction = getSpawnAction();
            break;
        case BeingAction::PRESTAND:
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
#ifdef EATHENA_SUPPORT
        if (mHorseSprite)
            mHorseSprite->play(currentAction);
#endif
        mAction = action;
    }

    if (currentAction != SpriteAction::MOVE
        && currentAction != SpriteAction::FLY
        && currentAction != SpriteAction::SWIM)
    {
        mActionTime = tick_time;
    }
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

    SpriteDirection::Type dir;
    if (mFaceDirection & BeingDirection::UP)
    {
        if (mFaceDirection & BeingDirection::LEFT)
            dir = SpriteDirection::UPLEFT;
        else if (mFaceDirection & BeingDirection::RIGHT)
            dir = SpriteDirection::UPRIGHT;
        else
            dir = SpriteDirection::UP;
    }
    else if (mFaceDirection & BeingDirection::DOWN)
    {
        if (mFaceDirection & BeingDirection::LEFT)
            dir = SpriteDirection::DOWNLEFT;
        else if (mFaceDirection & BeingDirection::RIGHT)
            dir = SpriteDirection::DOWNRIGHT;
        else
            dir = SpriteDirection::DOWN;
    }
    else if (mFaceDirection & BeingDirection::RIGHT)
    {
        dir = SpriteDirection::RIGHT;
    }
    else
    {
        dir = SpriteDirection::LEFT;
    }
    mSpriteDirection = dir;

    CompoundSprite::setSpriteDirection(dir);
    if (mEmotionSprite)
        mEmotionSprite->setSpriteDirection(dir);
    if (mAnimationEffect)
        mAnimationEffect->setSpriteDirection(dir);
#ifdef EATHENA_SUPPORT
    if (mHorseSprite)
        mHorseSprite->setSpriteDirection(dir);
#endif
    recalcSpritesOrder();
}

uint8_t Being::calcDirection() const
{
    uint8_t dir = 0;
    if (mDest.x > mX)
        dir |= BeingDirection::RIGHT;
    else if (mDest.x < mX)
        dir |= BeingDirection::LEFT;
    if (mDest.y > mY)
        dir |= BeingDirection::DOWN;
    else if (mDest.y < mY)
        dir |= BeingDirection::UP;
    return dir;
}

uint8_t Being::calcDirection(const int dstX, const int dstY) const
{
    uint8_t dir = 0;
    if (dstX > mX)
        dir |= BeingDirection::RIGHT;
    else if (dstX < mX)
        dir |= BeingDirection::LEFT;
    if (dstY > mY)
        dir |= BeingDirection::DOWN;
    else if (dstY < mY)
        dir |= BeingDirection::UP;
    return dir;
}

void Being::nextTile()
{
    if (mPath.empty())
    {
        mAction = BeingAction::PRESTAND;
        mPreStandTime = tick_time;
        return;
    }

    const Position pos = mPath.front();
    mPath.pop_front();

    const uint8_t dir = calcDirection(pos.x, pos.y);
    if (dir)
        setDirection(dir);

    if (!mMap || !mMap->getWalk(pos.x, pos.y, getBlockWalkMask()))
    {
        setAction(BeingAction::STAND, 0);
        return;
    }

    mActionTime += static_cast<int>(mSpeed / 10);
    if ((mType != ActorType::Player || mUseDiagonal)
        && mX != pos.x && mY != pos.y)
    {
        mSpeed = static_cast<float>(mWalkSpeed.x * 1.4F);
    }
    else
    {
        mSpeed = mWalkSpeed.x;
    }

    if (mX != pos.x || mY != pos.y)
        mOldHeight = mMap->getHeightOffset(mX, mY);
    mX = pos.x;
    mY = pos.y;
    const uint8_t height = mMap->getHeightOffset(mX, mY);
    mOffsetY = height - mOldHeight;
    mFixedOffsetY = height;
    mNeedPosUpdate = true;
    setAction(BeingAction::MOVE, 0);
}

void Being::logic()
{
    BLOCK_START("Being::logic")
    // Reduce the time that speech is still displayed
    if (mSpeechTime > 0)
        mSpeechTime--;

    // Remove text and speechbubbles if speech boxes aren't being used
    if (mSpeechTime == 0 && mText)
        delete2(mText)

    const int time = tick_time * MILLISECONDS_IN_A_TICK;
    if (mEmotionSprite)
        mEmotionSprite->update(time);
#ifdef EATHENA_SUPPORT
    if (mHorseSprite)
        mHorseSprite->update(time);
#endif

    if (mAnimationEffect)
    {
        mAnimationEffect->update(time);
        if (mAnimationEffect->isTerminated())
            delete2(mAnimationEffect)
    }

    int frameCount = static_cast<int>(getFrameCount());

    switch (mAction)
    {
        case BeingAction::STAND:
        case BeingAction::SIT:
        case BeingAction::DEAD:
        case BeingAction::HURT:
        case BeingAction::SPAWN:
        default:
            break;

        case BeingAction::MOVE:
        {
            if (static_cast<float>(get_elapsed_time(
                mActionTime)) >= mSpeed)
            {
                nextTile();
            }
            break;
        }

        case BeingAction::ATTACK:
        {
            if (!mActionTime)
                break;

            int curFrame = 0;
            if (mAttackSpeed)
            {
                curFrame = (get_elapsed_time(mActionTime) * frameCount)
                    / mAttackSpeed;
            }

            if (this == localPlayer && curFrame >= frameCount)
                nextTile();

            break;
        }

        case BeingAction::PRESTAND:
        {
            if (get_elapsed_time1(mPreStandTime) > 10)
                setAction(BeingAction::STAND, 0);
            break;
        }
    }

    if (mAction == BeingAction::MOVE || mNeedPosUpdate)
    {
        const int xOffset = getOffset(
            BeingDirection::LEFT, BeingDirection::RIGHT);
        const int yOffset = getOffset(
            BeingDirection::UP, BeingDirection::DOWN);
        int offset = xOffset;
        if (!offset)
            offset = yOffset;

        if (!xOffset && !yOffset)
            mNeedPosUpdate = false;

/*
        mSortOffsetY = (mOldHeight * mapTileSize / 2)
            + (mOffsetY * mapTileSize / 2)
            * (mapTileSize - abs(offset)) / mapTileSize - mFixedOffsetY * mapTileSize / 2;
        const int yOffset2 = yOffset - mSortOffsetY - mFixedOffsetY * mapTileSize / 2;
*/
        const int halfTile = mapTileSize / 2;
        const float offset2 = static_cast<float>(mOffsetY * abs(offset)) / 2;
        mSortOffsetY = (mOldHeight - mFixedOffsetY + mOffsetY)
            * halfTile - offset2;
        const float yOffset3 = (mY + 1) * mapTileSize + yOffset
            - (mOldHeight + mOffsetY) * halfTile + offset2;

        // Update pixel coordinates
        setPosition(static_cast<float>(mX * mapTileSize
            + mapTileSize / 2 + xOffset), yOffset3);
    }

    if (mEmotionSprite)
    {
        mEmotionTime--;
        if (mEmotionTime == 0)
            delete2(mEmotionSprite)
    }

    ActorSprite::logic();

    if (frameCount < 10)
        frameCount = 10;

    if (!isAlive() && mSpeed && gameHandler->removeDeadBeings()
        && static_cast<int> ((static_cast<float>(get_elapsed_time(mActionTime))
        / mSpeed)) >= frameCount)
    {
        if (mType != ActorType::Player && actorManager)
            actorManager->destroy(this);
    }

    FOR_EACH (std::vector<Being*>::iterator, it, mPets)
    {
        Being *const pet = *it;
        if (pet)
            pet->petLogic();
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

void Being::petLogic()
{
    if (!mOwner || !mMap || !mInfo)
        return;

    const int time = tick_time;
    const int thinkTime = mInfo->getThinkTime();
    if (abs(static_cast<int>(mMoveTime) - time) < thinkTime)
        return;

    mMoveTime = time;

    const int dstX0 = mOwner->getTileX();
    const int dstY0 = mOwner->getTileY();
    int dstX = dstX0;
    int dstY = dstY0;
    const int followDist = mInfo->getStartFollowDist();
    const int warpDist = mInfo->getWarpDist();
    const int dist = mInfo->getFollowDist();
    const int divX = abs(dstX - mX);
    const int divY = abs(dstY - mY);

    if (divX >= warpDist || divY >= warpDist)
    {
        setAction(BeingAction::STAND, 0);
        fixPetSpawnPos(dstX, dstY);
        setTileCoords(dstX, dstY);
        mPetAi = true;
    }
    else if (!followDist || divX > followDist || divY > followDist)
    {
        if (!mPetAi)
            return;
        if (!dist)
        {
            fixPetSpawnPos(dstX, dstY);
        }
        else
        {
            if (divX > followDist)
            {
                if (dstX > mX + dist)
                    dstX -= dist;
                else if (dstX + dist <= mX)
                    dstX += dist;
            }
            else
            {
                dstX = mX;
            }
            if (divY > followDist)
            {
                if (dstY > mY + dist)
                    dstY -= dist;
                else if (dstX + dist <= mX)
                    dstY += dist;
            }
            else
            {
                dstY = mY;
            }
        }

        const unsigned char blockWalkMask = getBlockWalkMask();
        if (!mMap->getWalk(dstX, dstY, blockWalkMask))
        {
            if (dstX != dstX0)
            {
                dstX = dstX0;
                if (!mMap->getWalk(dstX, dstY, blockWalkMask))
                    dstY = dstY0;
            }
            else if (dstY != dstY0)
            {
                dstY = dstY0;
                if (!mMap->getWalk(dstX, dstY, blockWalkMask))
                    dstX = dstX0;
            }
        }
        if (mX != dstX || mY != dstY)
        {
            setPath(mMap->findPath(mX, mY, dstX, dstY, blockWalkMask));
            return;
        }
    }
    if (!mPetAi)
        return;

    if (mOwner->getCurrentAction() != BeingAction::ATTACK)
    {
        if (mAction == BeingAction::ATTACK)
            setAction(BeingAction::STAND, 0);
    }
    else
    {
        if (mAction == BeingAction::STAND || mAction == BeingAction::ATTACK)
            setAction(BeingAction::ATTACK, 0);
    }

    if (mAction == BeingAction::STAND || mAction == BeingAction::ATTACK)
    {
        int directionType = 0;
        switch (mOwner->getCurrentAction())
        {
            case BeingAction::STAND:
            case BeingAction::MOVE:
            case BeingAction::HURT:
            case BeingAction::SPAWN:
            case BeingAction::PRESTAND:
            default:
                directionType = mInfo->getDirectionType();
                break;
            case BeingAction::SIT:
                directionType = mInfo->getSitDirectionType();
                break;
            case BeingAction::DEAD:
                directionType = mInfo->getDeadDirectionType();
                break;
            case BeingAction::ATTACK:
                directionType = mInfo->getAttackDirectionType();
                break;
        }

        uint8_t newDir = 0;
        switch (directionType)
        {
            case 0:
            default:
                return;

            case 1:
                newDir = mOwner->getDirection();
                break;

            case 2:
                if (dstX > dstX0)
                    newDir |= BeingDirection::LEFT;
                else if (dstX < dstX0)
                    newDir |= BeingDirection::RIGHT;
                if (dstY > dstY0)
                    newDir |= BeingDirection::UP;
                else if (dstY < dstY0)
                    newDir |= BeingDirection::DOWN;
                break;

            case 3:
                if (dstX > dstX0)
                    newDir |= BeingDirection::RIGHT;
                else if (dstX < dstX0)
                    newDir |= BeingDirection::LEFT;
                if (dstY > dstY0)
                    newDir |= BeingDirection::DOWN;
                else if (dstY < dstY0)
                    newDir |= BeingDirection::UP;
                break;

            case 4:
            {
                const int dstX2 = mOwner->getLastAttackX();
                const int dstY2 = mOwner->getLastAttackY();
                if (dstX > dstX2)
                    newDir |= BeingDirection::LEFT;
                else if (dstX < dstX2)
                    newDir |= BeingDirection::RIGHT;
                if (dstY > dstY2)
                    newDir |= BeingDirection::UP;
                else if (dstY < dstY2)
                    newDir |= BeingDirection::DOWN;
                break;
            }
        }
        if (newDir && newDir != getDirection())
            setDirection(newDir);
    }
}

void Being::drawEmotion(Graphics *const graphics, const int offsetX,
                        const int offsetY) const
{
    const int px = getPixelX() - offsetX - mapTileSize / 2;
    const int py = getPixelY() - offsetY - mapTileSize * 2 - mapTileSize;
    if (mEmotionSprite)
        mEmotionSprite->draw(graphics, px, py);
    if (mAnimationEffect)
        mAnimationEffect->draw(graphics, px, py);
}

void Being::drawSpeech(const int offsetX, const int offsetY)
{
    if (mSpeech.empty())
        return;

    const int px = getPixelX() - offsetX;
    const int py = getPixelY() - offsetY;
    const int speech = mSpeechType;

    // Draw speech above this being
    if (mSpeechTime == 0)
    {
        if (mSpeechBubble && mSpeechBubble->isVisibleLocal())
            mSpeechBubble->setVisible(Visible_false);
        mSpeech.clear();
    }
    else if (mSpeechTime > 0 && (speech == BeingSpeech::NAME_IN_BUBBLE ||
             speech == BeingSpeech::NO_NAME_IN_BUBBLE))
    {
        delete2(mText)

        if (mSpeechBubble)
        {
            mSpeechBubble->setPosition(px - (mSpeechBubble->getWidth() / 2),
                py - getHeight() - (mSpeechBubble->getHeight()));
            mSpeechBubble->setVisible(Visible_true);
            mSpeechBubble->requestMoveToBackground();
        }
    }
    else if (mSpeechTime > 0 && speech == BeingSpeech::TEXT_OVERHEAD)
    {
        if (mSpeechBubble)
            mSpeechBubble->setVisible(Visible_false);

        if (!mText && userPalette)
        {
            mText = new Text(mSpeech,
                getPixelX(),
                getPixelY() - getHeight(),
                Graphics::CENTER,
                &theme->getColor(ThemeColorId::BUBBLE_TEXT, 255),
                Speech_true);
        }
    }
    else if (speech == BeingSpeech::NO_SPEECH)
    {
        if (mSpeechBubble)
            mSpeechBubble->setVisible(Visible_false);
        delete2(mText)
    }
}

int Being::getOffset(const signed char pos, const signed char neg) const
{
    // Check whether we're walking in the requested direction
    if (mAction != BeingAction::MOVE ||  !(mDirection & (pos | neg)))
        return 0;

    int offset = 0;

    if (mMap)
    {
        const int time = get_elapsed_time(mActionTime);
        offset = (pos == BeingDirection::LEFT
            && neg == BeingDirection::RIGHT) ?
            static_cast<int>((static_cast<float>(time)
            * static_cast<float>(mMap->getTileWidth())) / mSpeed) :
            static_cast<int>((static_cast<float>(time)
            * static_cast<float>(mMap->getTileHeight())) / mSpeed);
    }

    // We calculate the offset _from_ the _target_ location
    offset -= mapTileSize;
    if (offset > 0)
        offset = 0;

    // Going into negative direction? Invert the offset.
    if (mDirection & pos)
        offset = -offset;

    if (offset > mapTileSize)
        offset = mapTileSize;
    if (offset < -mapTileSize)
        offset = -mapTileSize;

    return offset;
}

void Being::updateCoords()
{
    if (!mDispName)
        return;

    int offsetX = getPixelX();
    int offsetY = getPixelY();
    if (mInfo)
    {
        offsetX += mInfo->getNameOffsetX();
        offsetY += mInfo->getNameOffsetY();
    }
    // Monster names show above the sprite instead of below it
    if (mType == ActorType::Monster)
        offsetY += - getHeight() - mDispName->getHeight();

    mDispName->adviseXY(offsetX, offsetY, mMoveNames);
}

void Being::optionChanged(const std::string &value)
{
    if (mType == ActorType::Player && value == "visiblenames")
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
        if (getGender() == Gender::FEMALE)
            str = "\u2640";
        else if (getGender() == Gender::MALE)
            str = "\u2642";
    }
    if (mShowPlayersStatus)
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

    delete2(mDispName);

    if (mHideErased && player_relations.getRelation(mName) == Relation::ERASED)
        return;

    std::string displayName(mName);

    if (mType != ActorType::Monster && (mShowGender || mShowLevel))
    {
        displayName.append(" ");
        if (mShowLevel && getLevel() != 0)
            displayName.append(toString(getLevel()));

        displayName.append(getGenderSign());
    }

    if (mType == ActorType::Monster)
    {
        if (config.getBoolValue("showMonstersTakedDamage"))
            displayName.append(", ").append(toString(getDamageTaken()));
    }

    Font *font = nullptr;
    if (localPlayer && localPlayer->getTarget() == this
        && mType != ActorType::Monster)
    {
        font = boldFont;
    }
    else if (mType == ActorType::Player
             && !player_relations.isGoodName(this) && gui)
    {
        font = gui->getSecureFont();
    }

    if (mInfo)
    {
        mDispName = new FlashText(displayName,
            getPixelX() + mInfo->getNameOffsetX(),
            getPixelY() + mInfo->getNameOffsetY(),
            Graphics::CENTER, mNameColor, font);
    }
    else
    {
        mDispName = new FlashText(displayName, getPixelX(), getPixelY(),
            Graphics::CENTER, mNameColor, font);
    }

    updateCoords();
}

void Being::updateColors()
{
    if (userPalette)
    {
        if (mType == ActorType::Monster)
        {
            mNameColor = &userPalette->getColor(UserColorId::MONSTER);
            mTextColor = &userPalette->getColor(UserColorId::MONSTER);
        }
        else if (mType == ActorType::Npc)
        {
            mNameColor = &userPalette->getColor(UserColorId::NPC);
            mTextColor = &userPalette->getColor(UserColorId::NPC);
        }
        else if (this == localPlayer)
        {
            mNameColor = &userPalette->getColor(UserColorId::SELF);
            mTextColor = &theme->getColor(ThemeColorId::PLAYER, 255);
        }
        else
        {
            mTextColor = &theme->getColor(ThemeColorId::PLAYER, 255);

            if (player_relations.getRelation(mName) != Relation::ERASED)
                mErased = false;
            else
                mErased = true;

            if (mIsGM)
            {
                mTextColor = &userPalette->getColor(UserColorId::GM);
                mNameColor = &userPalette->getColor(UserColorId::GM);
            }
            else if (mEnemy)
            {
                mNameColor = &userPalette->getColor(UserColorId::ENEMY);
            }
            else if (mParty && localPlayer
                     && mParty == localPlayer->getParty())
            {
                mNameColor = &userPalette->getColor(UserColorId::PARTY);
            }
            else if (localPlayer && getGuild()
                     && getGuild() == localPlayer->getGuild())
            {
                mNameColor = &userPalette->getColor(UserColorId::GUILD);
            }
            else if (player_relations.getRelation(mName) == Relation::FRIEND)
            {
                mNameColor = &userPalette->getColor(UserColorId::FRIEND);
            }
            else if (player_relations.getRelation(mName) ==
                     Relation::DISREGARDED
                     || player_relations.getRelation(mName) ==
                     Relation::BLACKLISTED)
            {
                mNameColor = &userPalette->getColor(UserColorId::DISREGARDED);
            }
            else if (player_relations.getRelation(mName)
                     == Relation::IGNORED ||
                     player_relations.getRelation(mName)== Relation::ENEMY2)
            {
                mNameColor = &userPalette->getColor(UserColorId::IGNORED);
            }
            else if (player_relations.getRelation(mName) == Relation::ERASED)
            {
                mNameColor = &userPalette->getColor(UserColorId::ERASED);
            }
            else
            {
                mNameColor = &userPalette->getColor(UserColorId::PC);
            }
        }

        if (mDispName)
            mDispName->setColor(mNameColor);
    }
}

void Being::updateSprite(const unsigned int slot, const int id,
                         std::string color, const unsigned char colorId,
                         const bool isWeapon, const bool isTempSprite)
{
    if (slot >= charServerHandler->maxSprite())
        return;

    if (slot >= static_cast<unsigned int>(mSpriteIDs.size()))
        mSpriteIDs.resize(slot + 1, 0);

    if (slot && mSpriteIDs[slot] == id)
        return;
    setSprite(slot, id, color, colorId, isWeapon, isTempSprite);
}

void Being::setSprite(const unsigned int slot, const int id,
                      std::string color, const unsigned char colorId,
                      const bool isWeapon, const bool isTempSprite)
{
    if (slot >= charServerHandler->maxSprite())
        return;

    if (slot >= static_cast<unsigned int>(size()))
        ensureSize(slot + 1);

    if (slot >= static_cast<unsigned int>(mSpriteIDs.size()))
        mSpriteIDs.resize(slot + 1, 0);

    if (slot >= static_cast<unsigned int>(mSpriteColors.size()))
        mSpriteColors.resize(slot + 1, "");

    if (slot >= static_cast<unsigned int>(mSpriteColorsIds.size()))
        mSpriteColorsIds.resize(slot + 1, 1);

    // disabled for now, because it may broke replace/reorder sprites logic
//    if (slot && mSpriteIDs[slot] == id)
//        return;

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        if (isWeapon)
            mEquippedWeapon = nullptr;
        const int id1 = mSpriteIDs[slot];
        if (id1)
        {
            const ItemInfo &info = ItemDB::get(id1);
            if (!isTempSprite && mMap && mType == ActorType::Player)
            {
                const BeingId pet = fromInt(info.getPet(), BeingId);
                if (pet != BeingId_zero)
                    removePet(pet);
            }
            removeItemParticles(id1);
        }
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &filename = info.getSprite(
            mGender, mSubType);
        AnimatedSprite *equipmentSprite = nullptr;

        if (!isTempSprite && mType == ActorType::Player)
        {
            const BeingId pet = fromInt(info.getPet(), BeingId);
            if (pet != BeingId_zero)
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
        mSpriteDraw[slot] = id;

        addItemParticles(id, info.getDisplay());

        if (isWeapon)
            mEquippedWeapon = &ItemDB::get(id);

        setAction(mAction, 0);
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
    std::vector<unsigned char>::const_iterator it3 = mSpriteColorsIds.begin();
    const std::vector<unsigned char>::const_iterator
        it3_end = mSpriteColorsIds.end();

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
    while (ItemDB::get(-hairstyles).getSprite(Gender::MALE,
           BeingTypeId_zero) != paths.getStringValue("spriteErrorFile"))
    {
        hairstyles ++;
    }
    mNumberOfHairstyles = hairstyles;

    int races = 100;
    while (ItemDB::get(-races).getSprite(Gender::MALE, BeingTypeId_zero) !=
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
        mMoveNames = fromBool(config.getBoolValue("moveNames"), Move);
        mUseDiagonal = config.getBoolValue("useDiagonalSpeed");

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
            if (!serverFeatures->haveVending())
                mShop = ((flags & BeingFlag::SHOP) != 0);
            mAway = ((flags & BeingFlag::AWAY) != 0);
            mInactive = ((flags & BeingFlag::INACTIVE) != 0);
            if (mShop || mAway || mInactive)
                updateName();
        }
        else
        {
            if (!serverFeatures->haveVending())
                mShop = false;
            mAway = false;
            mInactive = false;
        }

        updateAwayEffect();
        if (mType == ActorType::Player)
            updateColors();
        return true;
    }
    return false;
}

void Being::addToCache() const
{
    if (localPlayer == this)
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
        if (!serverFeatures->haveVending() && mShop)
            flags += BeingFlag::SHOP;
        if (mAway)
            flags += BeingFlag::AWAY;
        if (mInactive)
            flags += BeingFlag::INACTIVE;
        entry->setFlags(flags);
    }
    else
    {
        entry->setFlags(0);
    }
}

BeingCacheEntry* Being::getCacheEntry(const BeingId id)
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


void Being::setGender(const GenderT gender)
{
    if (gender != mGender)
    {
        mGender = gender;

        // Reload all subsprites
        for (unsigned int i = 0;
             i < static_cast<unsigned int>(mSpriteIDs.size());
             i++)
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
    if (!PacketLimiter::limitPackets(PacketType::PACKET_NPC_TALK))
        return;

    npcHandler->talk(mId);
}

void Being::draw(Graphics *const graphics,
                 const int offsetX,
                 const int offsetY) const
{
    if (!mErased)
    {
        const int px = getActorX() + offsetX;
        const int py = getActorY() + offsetY;
#ifdef EATHENA_SUPPORT
        if (mHorseInfo)
        {
            AnimatedSprite *const sprite = mHorseInfo->sprite;
            if (sprite)
            {
                sprite->draw(graphics,
                    px + mHorseInfo->offsetX,
                    py + mHorseInfo->offsetY);
            }
        }
#endif
        ActorSprite::draw1(graphics, px, py);
        drawSpriteAt(graphics, px, py);
    }
}

void Being::drawSprites(Graphics *const graphics,
                        int posX, int posY) const
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
                           int posX, int posY) const
{
    const unsigned int sz = static_cast<unsigned int>(size());
    for (unsigned int f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        const Sprite *const sprite = getSprite(mSpriteRemap[f]);
        if (sprite)
            sprite->draw(graphics, posX, posY);
    }
}

void Being::drawSpriteAt(Graphics *const graphics,
                         const int x, const int y) const
{
    CompoundSprite::draw(graphics, x, y);

    if (!userPalette)
        return;

    if (mHighlightMapPortals &&
        mMap &&
        mSubType == fromInt(45, BeingTypeId) &&
        !mMap->getHasWarps())
    {
        graphics->setColor(userPalette->
                getColorWithAlpha(UserColorId::PORTAL_HIGHLIGHT));

        graphics->fillRectangle(Rect(x, y,
            mapTileSize, mapTileSize));

        if (mDrawHotKeys && !mName.empty())
        {
            Font *const font = gui->getFont();
            if (font)
            {
                const Color &color = userPalette->getColor(UserColorId::BEING);
                font->drawString(graphics, color, color, mName, x, y);
            }
        }
    }

    if (mHighlightMonsterAttackRange && mType == ActorType::Monster
        && isAlive())
    {
        int attackRange;
        if (mAttackRange)
            attackRange = mapTileSize * mAttackRange;
        else
            attackRange = mapTileSize;

        graphics->setColor(userPalette->getColorWithAlpha(
            UserColorId::MONSTER_ATTACK_RANGE));

        graphics->fillRectangle(Rect(
            x - attackRange, y - attackRange,
            2 * attackRange + mapTileSize, 2 * attackRange + mapTileSize));
    }

    if (mShowMobHP && mInfo && localPlayer && localPlayer->getTarget() == this
        && mType == ActorType::Monster)
    {
        // show hp bar here
        int maxHP = mMaxHP;
        if (!maxHP)
            maxHP = mInfo->getMaxHP();

        drawHpBar(graphics,
                  maxHP,
                  mHP,
                  mDamageTaken,
                  UserColorId::MONSTER_HP,
                  UserColorId::MONSTER_HP2,
                  x - 50 + mapTileSize / 2 + mInfo->getHpBarOffsetX(),
                  y + mapTileSize - 6 + mInfo->getHpBarOffsetY(),
                  2 * 50,
                  4);
    }
    if (mShowOwnHP
        && mInfo
        && localPlayer == this
        && mAction != BeingAction::DEAD)
    {
        drawHpBar(graphics, PlayerInfo::getAttribute(Attributes::MAX_HP),
                  PlayerInfo::getAttribute(Attributes::HP), 0,
                  UserColorId::PLAYER_HP, UserColorId::PLAYER_HP2,
                  x - 50 + mapTileSize / 2 + mInfo->getHpBarOffsetX(),
                  y + mapTileSize - 6 + mInfo->getHpBarOffsetY(),
                  2 * 50, 4);
    }
}

void Being::drawHpBar(Graphics *const graphics,
                      const int maxHP,
                      const int hp,
                      const int damage,
                      const UserColorIdT color1,
                      const UserColorIdT color2,
                      const int x,
                      const int y,
                      const int width,
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

    if (!serverFeatures->haveServerHp())
    {   // old servers
        if ((!damage && (this != localPlayer || hp == maxHP))
            || (!hp && maxHP == damage))
        {
            graphics->setColor(userPalette->getColorWithAlpha(color1));
            graphics->fillRectangle(Rect(
                x, y, dx, height));
            return;
        }
        else if (width - dx <= 0)
        {
            graphics->setColor(userPalette->getColorWithAlpha(color2));
            graphics->fillRectangle(Rect(
                x, y, width, height));
            return;
        }
    }
    else
    {
        if (hp == maxHP)
        {
            graphics->setColor(userPalette->getColorWithAlpha(color1));
            graphics->fillRectangle(Rect(
                x, y, dx, height));
            return;
        }
        else if (width - dx <= 0)
        {
            graphics->setColor(userPalette->getColorWithAlpha(color2));
            graphics->fillRectangle(Rect(
                x, y, width, height));
            return;
        }
    }

    graphics->setColor(userPalette->getColorWithAlpha(color1));
    graphics->fillRectangle(Rect(
        x, y, dx, height));

    graphics->setColor(userPalette->getColorWithAlpha(color2));
    graphics->fillRectangle(Rect(x + dx, y, width - dx, height));
}

void Being::setHP(const int hp)
{
    mHP = hp;
    if (mMaxHP < mHP)
        mMaxHP = mHP;
    if (mType == ActorType::Monster)
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
    const size_t sz = size();
    if (sz < 1)
        return;

    std::vector<int> slotRemap;
    std::map<int, int> itemSlotRemap;

    std::vector<int>::iterator it;
    int oldHide[20];
    bool updatedSprite[20];
    int dir = mSpriteDirection;
    if (dir < 0 || dir >= 9)
        dir = 0;
    // hack for allow different logic in dead player
    if (mAction == BeingAction::DEAD)
        dir = 9;

    const unsigned int hairSlot = charServerHandler->hairSprite();

    for (size_t slot = 0; slot < sz; slot ++)
    {
        oldHide[slot] = mSpriteHide[slot];
        mSpriteHide[slot] = 0;
        updatedSprite[slot] = false;
    }

    const size_t spriteIdSize = mSpriteIDs.size();

    for (size_t slot = 0; slot < sz; slot ++)
    {
        slotRemap.push_back(static_cast<int>(slot));

        if (spriteIdSize <= slot)
            continue;

        const int id = mSpriteIDs[slot];
        if (!id)
            continue;

        const ItemInfo &info = ItemDB::get(id);

        if (info.isRemoveSprites())
        {
            const SpriteToItemMap *const spriteToItems
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
                        else if (mSpriteHide[remSprite] != 1)
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
                                    updatedSprite[remSprite] = true;
                                }
                            }
                        }
                    }
                    else
                    {   // slot unknown. Search for real slot, this can be slow
                        FOR_EACH (IntMapCIter, repIt, itemReplacer)
                        {
                            for (unsigned int slot2 = 0; slot2 < sz; slot2 ++)
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
                                        updatedSprite[slot2] = true;
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

        for (unsigned int slot0 = 0; slot0 < sz; slot0 ++)
        {
            const int slot = searchSlotValue(slotRemap, slot0);
            const int val = slotRemap.at(slot);
            int id = 0;

            if (static_cast<int>(spriteIdSize) > val)
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
    for (unsigned int slot = 0; slot < sz; slot ++)
    {
        mSpriteRemap[slot] = slotRemap[slot];
        if (mSpriteHide[slot] == 0)
        {
            if (oldHide[slot] != 0 && oldHide[slot] != 1)
            {
                const int id = mSpriteIDs[slot];
                if (!id)
                    continue;

                updatedSprite[slot] = true;
                setSprite(slot, id, mSpriteColors[slot], 1, false, true);
            }
        }
    }
    for (unsigned slot = 0; slot < spriteIdSize; slot ++)
    {
        if (mSpriteHide[slot] == 0)
        {
            const int id = mSpriteIDs[slot];
            if (updatedSprite[slot] == false && mSpriteDraw[slot] != id)
                setSprite(slot, id, mSpriteColors[slot], 1, false, true);
        }
    }
}

int Being::searchSlotValue(const std::vector<int> &slotRemap,
                           const int val) const
{
    const size_t sz = size();
    for (size_t slot = 0; slot < sz; slot ++)
    {
        if (slotRemap[slot] == val)
            return static_cast<int>(slot);
    }
    return getNumberOfLayers() - 1;
}

void Being::searchSlotValueItr(std::vector<int>::iterator &it, int &idx,
                               std::vector<int> &slotRemap,
                               const int val)
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

std::string Being::loadComment(const std::string &name,
                               const ActorTypeT &type)
{
    std::string str;
    switch (type)
    {
        case ActorType::Player:
            str = settings.usersDir;
            break;
        case ActorType::Npc:
            str = settings.npcsDir;
            break;
        case ActorType::Unknown:
        case ActorType::Monster:
        case ActorType::FloorItem:
        case ActorType::Portal:
        case ActorType::LocalPet:
        case ActorType::Avatar:
#ifdef EATHENA_SUPPORT
        case ActorType::Mercenary:
        case ActorType::Homunculus:
        case ActorType::Pet:
#endif
        default:
            return "";
    }

    str.append(stringToHexPath(name)).append("/comment.txt");
    if (Files::existsLocal(str))
    {
        StringVect lines;
        Files::loadTextFileLocal(str, lines);
        if (lines.size() >= 2)
            return lines[1];
    }
    return std::string();
}

void Being::saveComment(const std::string &restrict name,
                        const std::string &restrict comment,
                        const ActorTypeT &restrict type)
{
    std::string dir;
    switch (type)
    {
        case ActorType::Player:
            dir = settings.usersDir;
            break;
        case ActorType::Npc:
            dir = settings.npcsDir;
            break;
        case ActorType::Monster:
        case ActorType::FloorItem:
        case ActorType::Portal:
        case ActorType::LocalPet:
        case ActorType::Avatar:
        case ActorType::Unknown:
#ifdef EATHENA_SUPPORT
        case ActorType::Pet:
        case ActorType::Mercenary:
        case ActorType::Homunculus:
#endif
        default:
            return;
    }
    dir.append(stringToHexPath(name));
    Files::saveTextFile(dir,
        "comment.txt",
        (name + "\n").append(comment));
}

void Being::setState(const uint8_t state)
{
    const bool shop = ((state & BeingFlag::SHOP) != 0);
    const bool away = ((state & BeingFlag::AWAY) != 0);
    const bool inactive = ((state & BeingFlag::INACTIVE) != 0);
    const bool needUpdate = (shop != mShop || away != mAway
        || inactive != mInactive);

    if (!serverFeatures->haveVending())
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
    if ((emotion & BeingFlag::SPECIAL) == BeingFlag::SPECIAL)
    {
        setState(emotion);
        mAdvanced = true;
    }
    else
    {
        const int emotionIndex = emotion - 1;
        if (emotionIndex >= 0 && emotionIndex <= EmoteDB::getLast())
        {
            delete2(mEmotionSprite)
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
                const int effectId = info->effectId;
                if (effectId >= 0)
                {
                    effectManager->trigger(effectId, this);
                }
            }
        }

        if (mEmotionSprite)
        {
            mEmotionSprite->play(mSpriteAction);
            mEmotionSprite->setSpriteDirection(mSpriteDirection);
        }
        else
        {
            mEmotionTime = 0;
        }
    }
}

void Being::updatePercentHP()
{
    if (!mMaxHP)
        return;
    BLOCK_START("Being::updatePercentHP")
    if (mHP)
    {
        const unsigned num = mHP * 100 / mMaxHP;
        if (num != mNumber)
        {
            mNumber = num;
            if (updateNumber(mNumber))
                setAction(mAction, 0);
        }
    }
    BLOCK_END("Being::updatePercentHP")
}

uint8_t Being::genderToInt(const GenderT sex)
{
    switch (sex)
    {
        case Gender::FEMALE:
        case Gender::UNSPECIFIED:
        default:
            return 0;
        case Gender::MALE:
            return 1;
        case Gender::OTHER:
            return 3;
    }
}

GenderT Being::intToGender(const uint8_t sex)
{
    switch (sex)
    {
        case 0:
        default:
            return Gender::FEMALE;
        case 1:
            return Gender::MALE;
        case 3:
            return Gender::OTHER;
    }
}

int Being::getSpriteID(const int slot) const
{
    if (slot < 0 || static_cast<size_t>(slot) >= mSpriteIDs.size())
        return -1;

    return mSpriteIDs[slot];
}

unsigned char Being::getSpriteColor(const int slot) const
{
    if (slot < 0 || static_cast<size_t>(slot) >= mSpriteColorsIds.size())
        return 1;

    return mSpriteColorsIds[slot];
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
    delete2(mAnimationEffect);
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

void Being::addPet(const BeingId id)
{
    if (!actorManager || !config.getBoolValue("usepets"))
        return;

    Being *const pet = findChildPet(id);
    if (pet)
    {
        pet->incUsage();
        return;
    }

    Being *const being = actorManager->createBeing(
        id, ActorType::LocalPet, BeingTypeId_zero);
    if (being)
    {
        being->setOwner(this);
        mPets.push_back(being);
        int dstX = mX;
        int dstY = mY;
        being->fixPetSpawnPos(dstX, dstY);
        being->setTileCoords(dstX, dstY);
    }
}

Being *Being::findChildPet(const BeingId id)
{
    FOR_EACH (std::vector<Being*>::iterator, it, mPets)
    {
        Being *const pet = *it;
        if (pet && pet->mId == id)
            return pet;
    }
    return nullptr;
}

void Being::removePet(const BeingId id)
{
    if (!actorManager)
        return;

    FOR_EACH (std::vector<Being*>::iterator, it, mPets)
    {
        Being *const pet = *it;
        if (pet && pet->mId == id)
        {
            if (!pet->decUsage())
            {
                pet->setOwner(nullptr);
                actorManager->erase(pet);
                mPets.erase(it);
                delete pet;
                return;
            }
        }
    }
}

void Being::removeAllPets()
{
    FOR_EACH (std::vector<Being*>::iterator, it, mPets)
    {
        Being *const pet = *it;
        if (pet)
        {
            pet->setOwner(nullptr);
            actorManager->erase(pet);
            delete pet;
        }
    }
    mPets.clear();
}

void Being::updatePets()
{
    removeAllPets();
    FOR_EACH (std::vector<int>::const_iterator, it, mSpriteIDs)
    {
        const int id = *it;
        if (!id)
            continue;
        const ItemInfo &info = ItemDB::get(id);
        const BeingId pet = fromInt(info.getPet(), BeingId);
        if (pet != BeingId_zero)
            addPet(pet);
    }
}

void Being::unassignPet(const Being *const pet1)
{
    FOR_EACH (std::vector<Being*>::iterator, it, mPets)
    {
        if (*it == pet1)
        {
            mPets.erase(it);
            return;
        }
    }
}

void Being::fixPetSpawnPos(int &dstX, int &dstY) const
{
    if (!mInfo || !mOwner)
        return;

    int offsetX1;
    int offsetY1;
    switch (mOwner->getCurrentAction())
    {
        case BeingAction::SIT:
            offsetX1 = mInfo->getSitOffsetX();
            offsetY1 = mInfo->getSitOffsetY();
            break;

        case BeingAction::MOVE:
            offsetX1 = mInfo->getMoveOffsetX();
            offsetY1 = mInfo->getMoveOffsetY();
            break;

        case BeingAction::DEAD:
            offsetX1 = mInfo->getDeadOffsetX();
            offsetY1 = mInfo->getDeadOffsetY();
            break;

        case BeingAction::ATTACK:
            offsetX1 = mInfo->getAttackOffsetX();
            offsetY1 = mInfo->getAttackOffsetY();
            break;

        case BeingAction::SPAWN:
        case BeingAction::HURT:
        case BeingAction::STAND:
        case BeingAction::PRESTAND:
        default:
            offsetX1 = mInfo->getTargetOffsetX();
            offsetY1 = mInfo->getTargetOffsetY();
            break;
    }

    int offsetX = offsetX1;
    int offsetY = offsetY1;
    switch (mOwner->getDirection())
    {
        case BeingDirection::LEFT:
            offsetX = -offsetY1;
            offsetY = offsetX1;
            break;
        case BeingDirection::RIGHT:
            offsetX = offsetY1;
            offsetY = -offsetX1;
            break;
        case BeingDirection::UP:
            offsetY = -offsetY;
            offsetX = -offsetX;
            break;
        default:
        case BeingDirection::DOWN:
            break;
    }
    dstX += offsetX;
    dstY += offsetY;
    if (mMap)
    {
        if (!mMap->getWalk(dstX, dstY, getBlockWalkMask()))
        {
            dstX = mOwner->getTileX();
            dstY = mOwner->getTileY();
        }
    }
}

void Being::playSfx(const SoundInfo &sound,
                    Being *const being,
                    const bool main,
                    const int x, const int y) const
{
    BLOCK_START("Being::playSfx")

    if (being)
    {
        // here need add timer and delay sound
        const int time = tick_time;
        if (main)
        {
            being->mNextSound.sound = nullptr;
            being->mNextSound.time = time + sound.delay;
            soundManager.playSfx(sound.sound, x, y);
        }
        else if (mNextSound.time <= time)
        {   // old event sound time is gone. we can play new sound
            being->mNextSound.sound = nullptr;
            being->mNextSound.time = time + sound.delay;
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
    BLOCK_END("Being::playSfx")
}

void Being::setLook(const uint16_t look)
{
    if (mType == ActorType::Player)
        setSubtype(mSubType, look);
}

void Being::setTileCoords(const int x, const int y)
{
    mX = x;
    mY = y;
    if (mMap)
    {
        mOffsetY = mMap->getHeightOffset(mX, mY);
        mFixedOffsetY = mOffsetY;
        mNeedPosUpdate = true;
    }
}

void Being::setMap(Map *const map)
{
    ActorSprite::setMap(map);
    if (mMap)
    {
        mOffsetY = mMap->getHeightOffset(mX, mY);
        mFixedOffsetY = mOffsetY;
        mOldHeight = 0;
        mNeedPosUpdate = true;
    }
}

void Being::removeAllItemsParticles()
{
    FOR_EACH (SpriteParticleInfoIter, it, mSpriteParticles)
        delete (*it).second;
    mSpriteParticles.clear();
}

void Being::addItemParticles(const int id, const SpriteDisplay &display)
{
    SpriteParticleInfoIter it = mSpriteParticles.find(id);
    ParticleInfo *pi = nullptr;
    if (it == mSpriteParticles.end())
    {
        pi = new ParticleInfo();
        mSpriteParticles[id] = pi;
    }
    else
    {
        pi = (*it).second;
    }

    if (!pi->particles.empty())
        return;

    // setup particle effects
    if (Particle::enabled && particleEngine)
    {
        FOR_EACH (StringVectCIter, itr, display.particles)
        {
            Particle *const p = particleEngine->addEffect(*itr, 0, 0);
            controlParticle(p);
            pi->files.push_back(*itr);
            pi->particles.push_back(p);
        }
    }
    else
    {
        FOR_EACH (StringVectCIter, itr, display.particles)
            pi->files.push_back(*itr);
    }
}

void Being::removeItemParticles(const int id)
{
    SpriteParticleInfoIter it = mSpriteParticles.find(id);
    if (it == mSpriteParticles.end())
        return;
    ParticleInfo *const pi = (*it).second;
    if (pi)
    {
        FOR_EACH (std::vector<Particle*>::const_iterator, itp, pi->particles)
            mChildParticleEffects.removeLocally(*itp);
        delete pi;
    }
    mSpriteParticles.erase(it);
}

void Being::recreateItemParticles()
{
    FOR_EACH (SpriteParticleInfoIter, it, mSpriteParticles)
    {
        ParticleInfo *const pi = (*it).second;
        if (pi && !pi->files.empty())
        {
            FOR_EACH (std::vector<Particle*>::const_iterator,
                      itp, pi->particles)
            {
                mChildParticleEffects.removeLocally(*itp);
            }

            FOR_EACH (std::vector<std::string>::const_iterator, str, pi->files)
            {
                Particle *const p = particleEngine->addEffect(*str, 0, 0);
                controlParticle(p);
                pi->particles.push_back(p);
            }
        }
    }
}

#ifdef EATHENA_SUPPORT
void Being::setChat(ChatObject *const obj)
{
    delete mChat;
    mChat = obj;
}

void Being::setSellBoard(const std::string &text)
{
    mShop = !text.empty() || !mBuyBoard.empty();
    mSellBoard = text;
    updateName();
}

void Being::setBuyBoard(const std::string &text)
{
    mShop = !text.empty() || !mSellBoard.empty();
    mBuyBoard = text;
    updateName();
}
#endif

void Being::enableShop(const bool b)
{
    mShop = b;
    updateName();
}

bool Being::isBuyShopEnabled() const
{
#ifdef EATHENA_SUPPORT
    return mShop && (!serverFeatures->haveVending() || !mBuyBoard.empty());
#else
    return mShop;
#endif
}

bool Being::isSellShopEnabled() const
{
#ifdef EATHENA_SUPPORT
    return mShop && (!serverFeatures->haveVending() || !mSellBoard.empty());
#else
    return mShop;
#endif
}

#ifdef EATHENA_SUPPORT
void Being::setRiding(const bool b)
{
    if (b == mRiding)
        return;
    mRiding = b;
    setAction(mAction, 0);
    if (b)
    {
        mHorseInfo = HorseDB::get(1);
        mHorseSprite = mHorseInfo->sprite;
    }
    else
    {
        mHorseInfo = nullptr;
        mHorseSprite = nullptr;
    }
}
#endif
