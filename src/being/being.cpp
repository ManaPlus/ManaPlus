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

#include "being/being.h"

#include "actormanager.h"
#include "beingequipbackend.h"
#include "configuration.h"
#include "effectmanager.h"
#include "guild.h"
#include "itemcolormanager.h"
#include "party.h"
#include "settings.h"
#include "soundmanager.h"
#include "text.h"

#include "being/beingcacheentry.h"
#include "being/beingflag.h"
#include "being/beingspeech.h"
#include "being/castingeffect.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"
#include "being/homunculusinfo.h"
#include "being/mercenaryinfo.h"

#include "const/utils/timer.h"

#include "const/resources/spriteaction.h"

#include "enums/being/beingdirection.h"

#include "enums/resources/map/blockmask.h"

#include "fs/files.h"

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
#include "net/homunculushandler.h"
#include "net/mercenaryhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/packetlimiter.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "particle/particleinfo.h"

#include "resources/attack.h"
#include "resources/chatobject.h"
#include "resources/emoteinfo.h"
#include "resources/emotesprite.h"
#include "resources/horseinfo.h"
#include "resources/iteminfo.h"

#include "resources/db/avatardb.h"
#include "resources/db/badgesdb.h"
#include "resources/db/groupdb.h"
#include "resources/db/elementaldb.h"
#include "resources/db/emotedb.h"
#include "resources/db/homunculusdb.h"
#include "resources/db/horsedb.h"
#include "resources/db/languagedb.h"
#include "resources/db/mercenarydb.h"
#include "resources/db/monsterdb.h"
#include "resources/db/npcdb.h"
#include "resources/db/petdb.h"
#include "resources/db/skillunitdb.h"

#include "resources/image/image.h"

#include "resources/item/item.h"

#include "resources/map/map.h"

#include "resources/skill/skilldata.h"
#include "resources/skill/skillinfo.h"

#include "resources/sprite/animatedsprite.h"

#include "gui/widgets/createwidget.h"

#include "gui/widgets/tabs/chat/langtab.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/likely.h"
#include "utils/stdmove.h"
#include "utils/timer.h"

#include "debug.h"

const unsigned int CACHE_SIZE = 50;

time_t Being::mUpdateConfigTime = 0;
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
BadgeDrawType::Type Being::mShowBadges = BadgeDrawType::Top;
int Being::mAwayEffect = -1;
VisibleNamePos::Type Being::mVisibleNamePos = VisibleNamePos::Buttom;

std::list<BeingCacheEntry*> beingInfoCache;
typedef std::map<int, Guild*>::const_iterator GuildsMapCIter;
typedef std::map<int, int>::const_iterator IntMapCIter;

static const unsigned int SPEECH_TIME = 500;
static const unsigned int SPEECH_MIN_TIME = 200;
static const unsigned int SPEECH_MAX_TIME = 800;

#define for_each_badges() \
    for (int f = 0; f < BadgeIndex::BadgeIndexSize; f++)

#define for_each_horses(name) \
    FOR_EACH (STD_VECTOR<AnimatedSprite*>::const_iterator, it, name)

Being::Being(const BeingId id,
             const ActorTypeT type) :
    ActorSprite(id),
    mNextSound(),
    mInfo(BeingInfo::unknown),
    mEmotionSprite(nullptr),
    mAnimationEffect(nullptr),
    mCastingEffect(nullptr),
    mBadges(),
    mSpriteAction(SpriteAction::STAND),
    mName(),
    mExtName(),
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
    mSlots(),
    mSpriteParticles(),
    mGuilds(),
    mParty(nullptr),
    mActionTime(0),
    mEmotionTime(0),
    mSpeechTime(0),
    mAttackSpeed(350),
    mLevel(0),
    mGroupId(0),
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
    mWalkSpeed(playerHandler != nullptr ?
               playerHandler->getDefaultWalkSpeed() : 1),
    mSpeed(playerHandler != nullptr ?
           playerHandler->getDefaultWalkSpeed() : 1),
    mIp(),
    mSpriteRemap(new int[20]),
    mSpriteHide(new int[20]),
    mSpriteDraw(new int[20]),
    mComment(),
    mBuyBoard(),
    mSellBoard(),
    mOwner(nullptr),
    mSpecialParticle(nullptr),
    mChat(nullptr),
    mHorseInfo(nullptr),
    mDownHorseSprites(),
    mUpHorseSprites(),
    mSpiritParticles(),
    mX(0),
    mY(0),
    mCachedX(0),
    mCachedY(0),
    mSortOffsetY(0),
    mPixelOffsetY(0),
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
    mSpiritBalls(0U),
    mUsageCounter(1),
    mKarma(0),
    mManner(0),
    mAreaSize(11),
    mCastEndTime(0),
    mLanguageId(-1),
    mBadgesX(0),
    mBadgesY(0),
    mCreatorId(BeingId_zero),
    mTeamId(0U),
    mLook(0U),
    mBadgesCount(0U),
    mHairColor(ItemColor_zero),
    mErased(false),
    mEnemy(false),
    mGotComment(false),
    mAdvanced(false),
    mShop(false),
    mAway(false),
    mInactive(false),
    mNeedPosUpdate(true),
    mBotAi(true)
{
    for (int f = 0; f < 20; f ++)
    {
        mSpriteRemap[f] = f;
        mSpriteHide[f] = 0;
        mSpriteDraw[f] = 0;
    }

    for_each_badges()
        mBadges[f] = nullptr;
}

void Being::postInit(const BeingTypeId subtype,
                     Map *const map)
{
    setMap(map);
    setSubtype(subtype, 0);

    VisibleName::Type showName1 = VisibleName::Hide;

    switch (mType)
    {
        case ActorType::Player:
        case ActorType::Mercenary:
        case ActorType::Pet:
        case ActorType::Homunculus:
        case ActorType::Elemental:
            showName1 = static_cast<VisibleName::Type>(
                config.getIntValue("visiblenames"));
            break;
        case ActorType::Portal:
        case ActorType::SkillUnit:
            showName1 = VisibleName::Hide;
            break;
        default:
        case ActorType::Unknown:
        case ActorType::Npc:
        case ActorType::Monster:
        case ActorType::FloorItem:
        case ActorType::Avatar:
            break;
    }

    if (mType != ActorType::Npc)
        mGotComment = true;

    config.addListener("visiblenames", this);

    reReadConfig();

    if (mType == ActorType::Npc ||
        showName1 == VisibleName::Show)
    {
        setShowName(true);
    }

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

    for_each_badges()
        delete2(mBadges[f]);

    delete2(mSpeechBubble);
    delete2(mDispName);
    delete2(mText);
    delete2(mEmotionSprite);
    delete2(mAnimationEffect);
    delete2(mCastingEffect);
    mBadgesCount = 0;
    delete2(mChat);
    removeHorse();

    removeAllItemsParticles();
    mSpiritParticles.clear();
}

void Being::createSpeechBubble() restrict2
{
    CREATEWIDGETV0(mSpeechBubble, SpeechBubble);
}

void Being::setSubtype(const BeingTypeId subtype,
                       const uint16_t look) restrict2
{
    if (mInfo == nullptr)
        return;

    if (subtype == mSubType && mLook == look)
        return;

    mSubType = subtype;
    mLook = look;

    switch (mType)
    {
        case ActorType::Monster:
            mInfo = MonsterDB::get(mSubType);
            if (mInfo != nullptr)
            {
                setName(mInfo->getName());
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_true,
                    DisplayType::Item,
                    mInfo->getColor(fromInt(mLook, ItemColor)));
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::Pet:
            mInfo = PETDB::get(mSubType);
            if (mInfo != nullptr)
            {
                setName(mInfo->getName());
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_true,
                    DisplayType::Item,
                    mInfo->getColor(fromInt(mLook, ItemColor)));
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::Mercenary:
            mInfo = MercenaryDB::get(mSubType);
            if (mInfo != nullptr)
            {
                setName(mInfo->getName());
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_true,
                    DisplayType::Item,
                    mInfo->getColor(fromInt(mLook, ItemColor)));
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::Homunculus:
            mInfo = HomunculusDB::get(mSubType);
            if (mInfo != nullptr)
            {
                setName(mInfo->getName());
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_true,
                    DisplayType::Item,
                    mInfo->getColor(fromInt(mLook, ItemColor)));
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::SkillUnit:
            mInfo = SkillUnitDb::get(mSubType);
            if (mInfo != nullptr)
            {
                setName(mInfo->getName());
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_false,
                    DisplayType::Item,
                    mInfo->getColor(fromInt(mLook, ItemColor)));
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::Elemental:
            mInfo = ElementalDb::get(mSubType);
            if (mInfo != nullptr)
            {
                setName(mInfo->getName());
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_false,
                    DisplayType::Item,
                    mInfo->getColor(fromInt(mLook, ItemColor)));
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::Npc:
            mInfo = NPCDB::get(mSubType);
            if (mInfo != nullptr)
            {
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_false,
                    DisplayType::Item,
                    std::string());
                mYDiff = mInfo->getSortOffsetY();
            }
            break;
        case ActorType::Avatar:
            mInfo = AvatarDB::get(mSubType);
            if (mInfo != nullptr)
            {
                setupSpriteDisplay(mInfo->getDisplay(),
                    ForceDisplay_false,
                    DisplayType::Item,
                    std::string());
            }
            break;
        case ActorType::Player:
        {
            int id = -100 - toInt(subtype, int);
            // Prevent showing errors when sprite doesn't exist
            if (!ItemDB::exists(id))
            {
                id = -100;
                // TRANSLATORS: default race name
                setRaceName(_("Human"));
                if (charServerHandler != nullptr)
                {
                    setSpriteId(charServerHandler->baseSprite(),
                        id);
                }
            }
            else
            {
                const ItemInfo &restrict info = ItemDB::get(id);
                setRaceName(info.getName());
                if (charServerHandler != nullptr)
                {
                    setSpriteColor(charServerHandler->baseSprite(),
                        id,
                        info.getColor(fromInt(mLook, ItemColor)));
                }
            }
            break;
        }
        case ActorType::Portal:
            break;
        case ActorType::Unknown:
        case ActorType::FloorItem:
        default:
            reportAlways("Wrong being type %d in setSubType",
                CAST_S32(mType));
            break;
    }
}

TargetCursorSizeT Being::getTargetCursorSize() const restrict2
{
    if (mInfo == nullptr)
        return TargetCursorSize::SMALL;

    return mInfo->getTargetCursorSize();
}

void Being::setPixelPositionF(const Vector &restrict pos) restrict2
{
    Actor::setPixelPositionF(pos);

    updateCoords();

    if (mText != nullptr)
    {
        mText->adviseXY(CAST_S32(pos.x),
            CAST_S32(pos.y) - getHeight() - mText->getHeight() - 9,
            mMoveNames);
    }
}

void Being::setDestination(const int dstX,
                           const int dstY) restrict2
{
    if (mMap == nullptr)
        return;

    setPath(mMap->findPath(mX, mY, dstX, dstY, getBlockWalkMask()));
}

void Being::clearPath() restrict2
{
    mPath.clear();
}

void Being::setPath(const Path &restrict path) restrict2
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

void Being::setSpeech(const std::string &restrict text,
                      const std::string &restrict channel,
                      int time) restrict2
{
    if (userPalette == nullptr)
        return;

    if (!channel.empty() &&
        ((langChatTab == nullptr) || langChatTab->getChannelName() != channel))
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

    if (time == 0)
    {
        const size_t sz = mSpeech.size();
        if (sz < 200)
            time = CAST_S32(SPEECH_TIME - 300 + (3 * sz));
    }

    if (time < CAST_S32(SPEECH_MIN_TIME))
        time = CAST_S32(SPEECH_MIN_TIME);

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
        mSpeechTime = time <= CAST_S32(SPEECH_MAX_TIME)
            ? time : CAST_S32(SPEECH_MAX_TIME);
    }

    const int speech = mSpeechType;
    if (speech == BeingSpeech::TEXT_OVERHEAD)
    {
        delete mText;
        mText = nullptr;
        mText = new Text(mSpeech,
            mPixelX,
            mPixelY - getHeight(),
            Graphics::CENTER,
            &userPalette->getColor(UserColorId::PARTICLE),
            Speech_true);
        mText->adviseXY(mPixelX,
            (mY + 1) * mapTileSize - getHeight() - mText->getHeight() - 9,
            mMoveNames);
    }
    else
    {
        const bool isShowName = (speech == BeingSpeech::NAME_IN_BUBBLE);
        if (mSpeechBubble == nullptr)
            createSpeechBubble();
        if (mSpeechBubble != nullptr)
        {
            mSpeechBubble->setCaption(isShowName ? mName : "");
            mSpeechBubble->setText(mSpeech, isShowName);
        }
    }
}

void Being::takeDamage(Being *restrict const attacker,
                       const int amount,
                       const AttackTypeT type,
                       const int attackId,
                       const int level) restrict2
{
    if (userPalette == nullptr || attacker == nullptr)
        return;

    BLOCK_START("Being::takeDamage1")

    Font *font = nullptr;
    const std::string damage = amount != 0 ? toString(amount) :
        // TRANSLATORS: dodge or miss message in attacks
        type == AttackType::FLEE ? _("dodge") : _("miss");
    const Color *color;

    if (gui != nullptr)
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
    else if (amount == 0)
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
    else if (mType == ActorType::Monster ||
             mType == ActorType::Mercenary ||
             mType == ActorType::Pet ||
             mType == ActorType::Homunculus ||
             mType == ActorType::SkillUnit)
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
    else if (mType == ActorType::Player &&
             attacker != localPlayer &&
             this == localPlayer)
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

    if (chatWindow != nullptr && mShowBattleEvents)
    {
        if (this == localPlayer)
        {
            if (attacker->mType == ActorType::Player || (amount != 0))
            {
                ChatWindow::battleChatLog(strprintf("%s : Hit you  -%d",
                    attacker->getName().c_str(), amount),
                    ChatMsgType::BY_OTHER);
            }
        }
        else if (attacker == localPlayer && (amount != 0))
        {
            ChatWindow::battleChatLog(strprintf("%s : You hit %s -%d",
                attacker->mName.c_str(),
                mName.c_str(),
                amount),
                ChatMsgType::BY_PLAYER);
        }
    }
    if (font != nullptr && particleEngine != nullptr)
    {
        // Show damage number
        particleEngine->addTextSplashEffect(damage,
            mPixelX,
            mPixelY - 16,
            color,
            font,
            true);
    }
    BLOCK_END("Being::takeDamage1")
    BLOCK_START("Being::takeDamage2")

    if (type != AttackType::SKILL)
        attacker->updateHit(amount);

    if (amount > 0)
    {
        if ((localPlayer != nullptr) && localPlayer == this)
            localPlayer->setLastHitFrom(attacker->mName);

        mDamageTaken += amount;
        if (mInfo != nullptr)
        {
            playSfx(mInfo->getSound(ItemSoundEvent::HURT),
                this,
                false,
                mX,
                mY);

            if (!mInfo->isStaticMaxHP())
            {
                if ((mHP == 0) && mInfo->getMaxHP() < mDamageTaken)
                    mInfo->setMaxHP(mDamageTaken);
            }
        }
        if ((mHP != 0) && isAlive())
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
        else if (mType == ActorType::Player &&
                 (socialWindow != nullptr) &&
                 !mName.empty())
        {
            socialWindow->updateAvatar(mName);
        }

        if (effectManager != nullptr)
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
        if (effectManager != nullptr)
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

int Being::getHitEffect(const Being *restrict const attacker,
                        const AttackTypeT type,
                        const int attackId,
                        const int level) const restrict2
{
    if (effectManager == nullptr)
        return 0;

    BLOCK_START("Being::getHitEffect")
    // Init the particle effect path based on current
    // weapon or default.
    int hitEffectId = 0;
    if (type == AttackType::SKILL || type == AttackType::SKILLMISS)
    {
        const SkillData *restrict const data =
            skillDialog->getSkillDataByLevel(attackId, level);
        if (data == nullptr)
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
        if (attacker != nullptr)
        {
            const ItemInfo *restrict const attackerWeapon
                = attacker->getEquippedWeapon();
            if (attackerWeapon != nullptr &&
                attacker->getType() == ActorType::Player)
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
                const BeingInfo *restrict const info = attacker->getInfo();
                if (info != nullptr)
                {
                    const Attack *restrict const atk =
                        info->getAttack(attackId);
                    if (atk != nullptr)
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

int Being::getDefaultEffectId(const AttackTypeT &restrict type)
{
    if (type == AttackType::MISS)
        return paths.getIntValue("missEffectId");
    else if (type != AttackType::CRITICAL)
        return paths.getIntValue("hitEffectId");
    else
        return paths.getIntValue("criticalHitEffectId");
}

void Being::handleAttack(Being *restrict const victim,
                         const int damage,
                         const int attackId) restrict2
{
    if ((victim == nullptr) || (mInfo == nullptr))
        return;

    BLOCK_START("Being::handleAttack")

    if (this != localPlayer)
        setAction(BeingAction::ATTACK, attackId);

    mLastAttackX = victim->mX;
    mLastAttackY = victim->mY;

    if (mType == ActorType::Player && (mEquippedWeapon != nullptr))
        fireMissile(victim, mEquippedWeapon->getMissileConst());
    else if (mInfo->getAttack(attackId) != nullptr)
        fireMissile(victim, mInfo->getAttack(attackId)->mMissile);

    reset();
    mActionTime = tick_time;

    if (Net::getNetworkType() == ServerType::TMWATHENA &&
        this != localPlayer)
    {
        const uint8_t dir = calcDirection(victim->mX,
            victim->mY);
        if (dir != 0u)
            setDirection(dir);
    }

    if ((damage != 0) && victim->mType == ActorType::Player
        && victim->mAction == BeingAction::SIT)
    {
        victim->setAction(BeingAction::STAND, 0);
    }

    if (mType == ActorType::Player)
    {
        if (mSlots.size() >= 10)
        {
            // here 10 is weapon slot
            int weaponId = mSlots[10].spriteId;
            if (weaponId == 0)
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

void Being::handleSkillCasting(Being *restrict const victim,
                               const int skillId,
                               const int skillLevel) restrict2
{
    if ((victim == nullptr) || (mInfo == nullptr) || (skillDialog == nullptr))
        return;

    setAction(BeingAction::CAST, skillId);

    const SkillData *restrict const data = skillDialog->getSkillDataByLevel(
        skillId,
        skillLevel);

    if (data != nullptr)
    {
        effectManager->triggerDefault(data->castingSrcEffectId,
            this,
            paths.getIntValue("skillCastingSrcEffectId"));
        effectManager->triggerDefault(data->castingDstEffectId,
            victim,
            paths.getIntValue("skillCastingDstEffectId"));
        fireMissile(victim, data->castingMissile);
    }
}

void Being::handleSkill(Being *restrict const victim,
                        const int damage,
                        const int skillId,
                        const int skillLevel) restrict2
{
    if ((victim == nullptr) || (mInfo == nullptr) || (skillDialog == nullptr))
        return;

    const SkillInfo *restrict const skill = skillDialog->getSkill(skillId);
    const SkillData *restrict const data = skill != nullptr
        ? skill->getData1(skillLevel) : nullptr;
    if (data != nullptr)
    {
        effectManager->triggerDefault(data->srcEffectId,
            this,
            paths.getIntValue("skillSrcEffectId"));
        effectManager->triggerDefault(data->dstEffectId,
            victim,
            paths.getIntValue("skillDstEffectId"));
        fireMissile(victim, data->missile);
    }

    if (this != localPlayer && (skill != nullptr))
    {
        const SkillType::SkillType type = skill->type;
        if ((type & SkillType::Attack) != 0 ||
            (type & SkillType::Ground) != 0)
        {
            setAction(BeingAction::ATTACK, 1);
        }
        else
        {
            setAction(BeingAction::STAND, 1);
        }
    }

    reset();
    mActionTime = tick_time;

    if (Net::getNetworkType() == ServerType::TMWATHENA &&
        this != localPlayer)
    {
        const uint8_t dir = calcDirection(victim->mX,
            victim->mY);
        if (dir != 0u)
            setDirection(dir);
    }
    if ((damage != 0) && victim->mType == ActorType::Player
        && victim->mAction == BeingAction::SIT)
    {
        victim->setAction(BeingAction::STAND, 0);
    }
    if (data != nullptr)
    {
        if (damage > 0)
            playSfx(data->soundHit, victim, true, mX, mY);
        else
            playSfx(data->soundMiss, victim, true, mX, mY);
    }
    else
    {
        playSfx(mInfo->getSound((damage > 0) ?
            ItemSoundEvent::HIT : ItemSoundEvent::MISS),
            victim,
            true,
            mX, mY);
    }
}

void Being::showNameBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Name]);
    if (show &&
        !mName.empty() &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string badge = BadgesDB::getNameBadge(mName);
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Name] = AnimatedSprite::load(
                paths.getStringValue("badges") + badge);
        }
    }
}

void Being::setName(const std::string &restrict name) restrict2
{
    mExtName = name;
    if (mType == ActorType::Npc)
    {
        mName = name.substr(0, name.find('#', 0));
        showName();
    }
    else if (mType == ActorType::Player)
    {
        if (mName != name)
        {
            mName = name;
            showNameBadge(!mName.empty());
        }
        if (getShowName())
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

void Being::setShowName(const bool doShowName) restrict2
{
    if (mShowName == doShowName)
        return;

    mShowName = doShowName;

    if (doShowName)
        showName();
    else
        delete2(mDispName)
}

void Being::showGuildBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Guild]);
    if (show &&
        !mGuildName.empty() &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string badge = BadgesDB::getGuildBadge(mGuildName);
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Guild] = AnimatedSprite::load(
                paths.getStringValue("badges") + badge);
        }
    }
}

void Being::setGuildName(const std::string &restrict name) restrict2
{
    if (mGuildName != name)
    {
        mGuildName = name;
        showGuildBadge(!mGuildName.empty());
        updateBadgesCount();
    }
}

void Being::setGuildPos(const std::string &restrict pos A_UNUSED) restrict2
{
}

void Being::addGuild(Guild *restrict const guild) restrict2
{
    if (guild == nullptr)
        return;

    mGuilds[guild->getId()] = guild;

    if (this == localPlayer && (socialWindow != nullptr))
        socialWindow->addTab(guild);
}

void Being::removeGuild(const int id) restrict2
{
    if (this == localPlayer && (socialWindow != nullptr))
        socialWindow->removeTab(mGuilds[id]);

    if (mGuilds[id] != nullptr)
        mGuilds[id]->removeMember(mName);
    mGuilds.erase(id);
}

const Guild *Being::getGuild(const std::string &restrict guildName) const
                             restrict2
{
    FOR_EACH (GuildsMapCIter, itr, mGuilds)
    {
        const Guild *restrict const guild = itr->second;
        if ((guild != nullptr) && guild->getName() == guildName)
            return guild;
    }

    return nullptr;
}

const Guild *Being::getGuild(const int id) const restrict2
{
    const std::map<int, Guild*>::const_iterator itr = mGuilds.find(id);
    if (itr != mGuilds.end())
        return itr->second;

    return nullptr;
}

Guild *Being::getGuild() const restrict2
{
    const std::map<int, Guild*>::const_iterator itr = mGuilds.begin();
    if (itr != mGuilds.end())
        return itr->second;

    return nullptr;
}

void Being::clearGuilds() restrict2
{
    FOR_EACH (GuildsMapCIter, itr, mGuilds)
    {
        Guild *const guild = itr->second;

        if (guild != nullptr)
        {
            if (this == localPlayer && (socialWindow != nullptr))
                socialWindow->removeTab(guild);

            guild->removeMember(mId);
        }
    }

    mGuilds.clear();
}

void Being::setParty(Party *restrict const party) restrict2
{
    if (party == mParty)
        return;

    Party *const old = mParty;
    mParty = party;

    if (old != nullptr)
        old->removeMember(mId);

    if (party != nullptr)
        party->addMember(mId, mName);

    updateColors();

    if (this == localPlayer && (socialWindow != nullptr))
    {
        if (old != nullptr)
            socialWindow->removeTab(old);

        if (party != nullptr)
            socialWindow->addTab(party);
    }
}

void Being::updateGuild() restrict2
{
    if (localPlayer == nullptr)
        return;

    Guild *restrict const guild = localPlayer->getGuild();
    if (guild == nullptr)
    {
        clearGuilds();
        updateColors();
        return;
    }
    if (guild->getMember(mName) != nullptr)
    {
        setGuild(guild);
        if (!guild->getName().empty())
            setGuildName(guild->getName());
    }
    updateColors();
}

void Being::setGuild(Guild *restrict const guild) restrict2
{
    Guild *restrict const old = getGuild();
    if (guild == old)
        return;

    clearGuilds();
    addGuild(guild);

    if (old != nullptr)
        old->removeMember(mName);

    updateColors();

    if (this == localPlayer && (socialWindow != nullptr))
    {
        if (old != nullptr)
            socialWindow->removeTab(old);

        if (guild != nullptr)
            socialWindow->addTab(guild);
    }
}

void Being::fireMissile(Being *restrict const victim,
                        const MissileInfo &restrict missile) const restrict2
{
    BLOCK_START("Being::fireMissile")

    if (victim == nullptr ||
        missile.particle.empty() ||
        particleEngine == nullptr)
    {
        BLOCK_END("Being::fireMissile")
        return;
    }

    Particle *restrict const target = particleEngine->createChild();

    if (target == nullptr)
    {
        BLOCK_END("Being::fireMissile")
        return;
    }

    Particle *restrict const missileParticle = target->addEffect(
        missile.particle,
        mPixelX,
        mPixelY);

    target->moveBy(Vector(0.0F, 0.0F, missile.z));
    target->setLifetime(missile.lifeTime);
    victim->controlAutoParticle(target);

    if (missileParticle != nullptr)
    {
        missileParticle->setDestination(target, missile.speed, 0.0F);
        missileParticle->setDieDistance(missile.dieDistance);
        missileParticle->setLifetime(missile.lifeTime);
    }
    BLOCK_END("Being::fireMissile")
}

std::string Being::getSitAction() const restrict2
{
    if (mHorseId != 0)
        return SpriteAction::SITRIDE;
    if (mMap != nullptr)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if ((mask & BlockMask::GROUNDTOP) != 0)
            return SpriteAction::SITTOP;
        else if ((mask & BlockMask::AIR) != 0)
            return SpriteAction::SITSKY;
        else if ((mask & BlockMask::WATER) != 0)
            return SpriteAction::SITWATER;
    }
    return SpriteAction::SIT;
}


std::string Being::getMoveAction() const restrict2
{
    if (mHorseId != 0)
        return SpriteAction::RIDE;
    if (mMap != nullptr)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if ((mask & BlockMask::AIR) != 0)
            return SpriteAction::FLY;
        else if ((mask & BlockMask::WATER) != 0)
            return SpriteAction::SWIM;
    }
    return SpriteAction::MOVE;
}

std::string Being::getWeaponAttackAction(const ItemInfo *restrict const weapon)
                                         const restrict2
{
    if (weapon == nullptr)
        return getAttackAction();

    if (mHorseId != 0)
        return weapon->getRideAttackAction();
    if (mMap != nullptr)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if ((mask & BlockMask::AIR) != 0)
            return weapon->getSkyAttackAction();
        else if ((mask & BlockMask::WATER) != 0)
            return weapon->getWaterAttackAction();
    }
    return weapon->getAttackAction();
}

std::string Being::getAttackAction(const Attack *restrict const attack1) const
                                   restrict2
{
    if (attack1 == nullptr)
        return getAttackAction();

    if (mHorseId != 0)
        return attack1->mRideAction;
    if (mMap != nullptr)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if ((mask & BlockMask::AIR) != 0)
            return attack1->mSkyAction;
        else if ((mask & BlockMask::WATER) != 0)
            return attack1->mWaterAction;
    }
    return attack1->mAction;
}

std::string Being::getCastAction(const SkillInfo *restrict const skill) const
                                 restrict2
{
    if (skill == nullptr)
        return getCastAction();

    if (mHorseId != 0)
        return skill->castingRideAction;
    if (mMap != nullptr)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if ((mask & BlockMask::AIR) != 0)
            return skill->castingSkyAction;
        else if ((mask & BlockMask::WATER) != 0)
            return skill->castingWaterAction;
    }
    return skill->castingAction;
}

#define getSpriteAction(func, action) \
    std::string Being::get##func##Action() const restrict2\
{ \
    if (mHorseId != 0) \
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

getSpriteAction(Attack, ATTACK)
getSpriteAction(Cast, CAST)
getSpriteAction(Dead, DEAD)
getSpriteAction(Spawn, SPAWN)

std::string Being::getStandAction() const restrict2
{
    if (mHorseId != 0)
        return SpriteAction::STANDRIDE;
    if (mMap != nullptr)
    {
        const unsigned char mask = mMap->getBlockMask(mX, mY);
        if (mTrickDead)
        {
            if ((mask & BlockMask::AIR) != 0)
                return SpriteAction::DEADSKY;
            else if ((mask & BlockMask::WATER) != 0)
                return SpriteAction::DEADWATER;
            else
                return SpriteAction::DEAD;
        }
        if ((mask & BlockMask::AIR) != 0)
            return SpriteAction::STANDSKY;
        else if ((mask & BlockMask::WATER) != 0)
            return SpriteAction::STANDWATER;
    }
    return SpriteAction::STAND;
}

void Being::setAction(const BeingActionT &restrict action,
                      const int attackId) restrict2
{
    std::string currentAction = SpriteAction::INVALID;

    switch (action)
    {
        case BeingAction::MOVE:
            if (mInfo != nullptr)
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
            if (mInfo != nullptr)
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
            if (mEquippedWeapon != nullptr)
            {
                currentAction = getWeaponAttackAction(mEquippedWeapon);
                reset();
            }
            else
            {
                if (mInfo == nullptr || mInfo->getAttack(attackId) == nullptr)
                    break;

                currentAction = getAttackAction(mInfo->getAttack(attackId));
                reset();

                // attack particle effect
                if (ParticleEngine::enabled && (effectManager != nullptr))
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
        case BeingAction::CAST:
            if (skillDialog != nullptr)
            {
                const SkillInfo *restrict const info =
                    skillDialog->getSkill(attackId);
                currentAction = getCastAction(info);
            }
            break;
        case BeingAction::HURT:
            if (mInfo != nullptr)
            {
                playSfx(mInfo->getSound(ItemSoundEvent::HURT),
                    this, false, mX, mY);
            }
            break;
        case BeingAction::DEAD:
            currentAction = getDeadAction();
            if (mInfo != nullptr)
            {
                playSfx(mInfo->getSound(ItemSoundEvent::DIE),
                    this,
                    false,
                    mX, mY);
                if (mType == ActorType::Monster ||
                    mType == ActorType::Npc ||
                    mType == ActorType::SkillUnit)
                {
                    mYDiff = mInfo->getDeadSortOffsetY();
                }
            }
            break;
        case BeingAction::STAND:
            currentAction = getStandAction();
            break;
        case BeingAction::SPAWN:
            if (mInfo != nullptr)
            {
                playSfx(mInfo->getSound(ItemSoundEvent::SPAWN),
                    nullptr, true, mX, mY);
            }
            currentAction = getSpawnAction();
            break;
        case BeingAction::PRESTAND:
            break;
        default:
            logger->log("Being::setAction unknown action: "
                + toString(CAST_U32(action)));
            break;
    }

    if (currentAction != SpriteAction::INVALID)
    {
        mSpriteAction = currentAction;
        play(currentAction);
        if (mEmotionSprite != nullptr)
            mEmotionSprite->play(currentAction);
        if (mAnimationEffect != nullptr)
            mAnimationEffect->play(currentAction);
        for_each_badges()
        {
            AnimatedSprite *const sprite = mBadges[f];
            if (sprite != nullptr)
                sprite->play(currentAction);
        }
        for_each_horses(mDownHorseSprites)
            (*it)->play(currentAction);
        for_each_horses(mUpHorseSprites)
            (*it)->play(currentAction);
        mAction = action;
    }

    if (currentAction != SpriteAction::MOVE
        && currentAction != SpriteAction::FLY
        && currentAction != SpriteAction::SWIM)
    {
        mActionTime = tick_time;
    }
}

void Being::setDirection(const uint8_t direction) restrict2
{
    if (mDirection == direction)
        return;

    mDirection = direction;

    mDirectionDelayed = 0;

    // if the direction does not change much, keep the common component
    int mFaceDirection = mDirection & direction;
    if (mFaceDirection == 0)
        mFaceDirection = direction;

    SpriteDirection::Type dir;
    if ((mFaceDirection & BeingDirection::UP) != 0)
    {
        if ((mFaceDirection & BeingDirection::LEFT) != 0)
            dir = SpriteDirection::UPLEFT;
        else if ((mFaceDirection & BeingDirection::RIGHT) != 0)
            dir = SpriteDirection::UPRIGHT;
        else
            dir = SpriteDirection::UP;
    }
    else if ((mFaceDirection & BeingDirection::DOWN) != 0)
    {
        if ((mFaceDirection & BeingDirection::LEFT) != 0)
            dir = SpriteDirection::DOWNLEFT;
        else if ((mFaceDirection & BeingDirection::RIGHT) != 0)
            dir = SpriteDirection::DOWNRIGHT;
        else
            dir = SpriteDirection::DOWN;
    }
    else if ((mFaceDirection & BeingDirection::RIGHT) != 0)
    {
        dir = SpriteDirection::RIGHT;
    }
    else
    {
        dir = SpriteDirection::LEFT;
    }
    mSpriteDirection = dir;

    CompoundSprite::setSpriteDirection(dir);
    if (mEmotionSprite != nullptr)
        mEmotionSprite->setSpriteDirection(dir);
    if (mAnimationEffect != nullptr)
        mAnimationEffect->setSpriteDirection(dir);

    for_each_badges()
    {
        AnimatedSprite *const sprite = mBadges[f];
        if (sprite != nullptr)
            sprite->setSpriteDirection(dir);
    }

    for_each_horses(mDownHorseSprites)
        (*it)->setSpriteDirection(dir);
    for_each_horses(mUpHorseSprites)
        (*it)->setSpriteDirection(dir);
    recalcSpritesOrder();
}

uint8_t Being::calcDirection() const restrict2
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

uint8_t Being::calcDirection(const int dstX,
                             const int dstY) const restrict2
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

void Being::nextTile() restrict2
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
    if (dir != 0u)
        setDirection(dir);

    if (mMap == nullptr ||
        !mMap->getWalk(pos.x, pos.y, getBlockWalkMask()))
    {
        setAction(BeingAction::STAND, 0);
        return;
    }

    mActionTime += mSpeed / 10;
    if ((mType != ActorType::Player || mUseDiagonal)
        && mX != pos.x && mY != pos.y)
    {
        mSpeed = mWalkSpeed * 14 / 10;
    }
    else
    {
        mSpeed = mWalkSpeed;
    }

    if (mX != pos.x || mY != pos.y)
    {
        mOldHeight = mMap->getHeightOffset(mX, mY);
        if (mReachable == Reachable::REACH_NO &&
            mMap->getBlockMask(mX, mY) != mMap->getBlockMask(pos.x, pos.y))
        {
            mReachable = Reachable::REACH_UNKNOWN;
        }
    }
    mX = pos.x;
    mY = pos.y;
    const uint8_t height = mMap->getHeightOffset(mX, mY);
    mPixelOffsetY = height - mOldHeight;
    mFixedOffsetY = height;
    mNeedPosUpdate = true;
    setAction(BeingAction::MOVE, 0);
}

void Being::logic() restrict2
{
    BLOCK_START("Being::logic")
    if (A_UNLIKELY(mSpeechTime != 0))
    {
        mSpeechTime--;
        if (mSpeechTime == 0 && mText != nullptr)
            delete2(mText)
    }

    if (A_UNLIKELY(mOwner != nullptr))
    {
        if (mType == ActorType::Homunculus ||
            mType == ActorType::Mercenary)
        {
            botLogic();
        }
    }

    const int time = tick_time * MILLISECONDS_IN_A_TICK;
    if (mEmotionSprite != nullptr)
        mEmotionSprite->update(time);
    for_each_horses(mDownHorseSprites)
        (*it)->update(time);
    for_each_horses(mUpHorseSprites)
        (*it)->update(time);

    if (A_UNLIKELY(mCastEndTime != 0 && mCastEndTime < tick_time))
    {
        mCastEndTime = 0;
        delete2(mCastingEffect);
    }

    if (A_UNLIKELY(mAnimationEffect))
    {
        mAnimationEffect->update(time);
        if (mAnimationEffect->isTerminated())
            delete2(mAnimationEffect)
    }
    if (A_UNLIKELY(mCastingEffect))
    {
        mCastingEffect->update(time);
        if (mCastingEffect->isTerminated())
            delete2(mCastingEffect)
    }
    for_each_badges()
    {
        AnimatedSprite *restrict const sprite = mBadges[f];
        if (sprite != nullptr)
            sprite->update(time);
    }

    int frameCount = CAST_S32(getFrameCount());

    switch (mAction)
    {
        case BeingAction::STAND:
        case BeingAction::SIT:
        case BeingAction::DEAD:
        case BeingAction::HURT:
        case BeingAction::SPAWN:
        case BeingAction::CAST:
        default:
            break;

        case BeingAction::MOVE:
        {
            if (get_elapsed_time(mActionTime) >= mSpeed)
                nextTile();
            break;
        }

        case BeingAction::ATTACK:
        {
            if (mActionTime == 0)
                break;

            int curFrame = 0;
            if (mAttackSpeed != 0)
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
        const int xOffset = getOffset<BeingDirection::LEFT,
            BeingDirection::RIGHT>();
        const int yOffset = getOffset<BeingDirection::UP,
            BeingDirection::DOWN>();
        int offset = xOffset;
        if (offset == 0)
            offset = yOffset;

        if ((xOffset == 0) && (yOffset == 0))
            mNeedPosUpdate = false;

        const int halfTile = mapTileSize / 2;
        const float offset2 = static_cast<float>(
            mPixelOffsetY * abs(offset)) / 2;
//        mSortOffsetY = (mOldHeight - mFixedOffsetY + mPixelOffsetY)
//            * halfTile - offset2;
        mSortOffsetY = 0;
        const float yOffset3 = (mY + 1) * mapTileSize + yOffset
            - (mOldHeight + mPixelOffsetY) * halfTile + offset2;

        // Update pixel coordinates
        setPixelPositionF(static_cast<float>(mX * mapTileSize
            + mapTileSize / 2 + xOffset), yOffset3);
    }

    if (A_UNLIKELY(mEmotionSprite))
    {
        mEmotionTime--;
        if (mEmotionTime == 0)
            delete2(mEmotionSprite)
    }

    ActorSprite::logic();

    if (frameCount < 10)
        frameCount = 10;

    if (A_UNLIKELY(!isAlive() &&
        mSpeed != 0 &&
        gameHandler->removeDeadBeings() &&
        get_elapsed_time(mActionTime) / mSpeed >= frameCount))
    {
        if (mType != ActorType::Player && (actorManager != nullptr))
            actorManager->destroy(this);
    }

    const SoundInfo *restrict const sound = mNextSound.sound;
    if (A_UNLIKELY(sound))
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

void Being::botLogic() restrict2
{
    if ((mOwner == nullptr) || (mMap == nullptr) || (mInfo == nullptr))
        return;

    const int time = tick_time;
    const int thinkTime = mInfo->getThinkTime();
    if (abs(CAST_S32(mMoveTime) - time) < thinkTime)
        return;

    mMoveTime = time;

    int dstX = mOwner->mX;
    int dstY = mOwner->mY;
    const int warpDist = mInfo->getWarpDist();
    const int divX = abs(dstX - mX);
    const int divY = abs(dstY - mY);

    if (divX >= warpDist || divY >= warpDist)
    {
        if (mType == ActorType::Homunculus)
            homunculusHandler->moveToMaster();
        else
            mercenaryHandler->moveToMaster();
        mBotAi = true;
        return;
    }
    if (!mBotAi)
        return;
    if (mAction == BeingAction::MOVE)
    {
        if (mOwner->mAction == BeingAction::MOVE)
        {
            updateBotFollow(dstX, dstY,
                divX, divY);
        }
        return;
    }

    switch (mOwner->mAction)
    {
        case BeingAction::MOVE:
        case BeingAction::PRESTAND:
            updateBotFollow(dstX, dstY,
                divX, divY);
            break;
        case BeingAction::STAND:
        case BeingAction::SPAWN:
            botFixOffset(dstX, dstY);
            moveBotTo(dstX, dstY);
            break;
        case BeingAction::ATTACK:
        {
            const Being *const target = localPlayer->getTarget();
            if (target == nullptr)
                return;
            const BeingId targetId = target->getId();
            if (mType == ActorType::Homunculus)
            {
                homunculusHandler->attack(targetId,
                    Keep_true);
            }
            else
            {
                mercenaryHandler->attack(targetId,
                    Keep_true);
            }
            break;
        }
        case BeingAction::SIT:
            botFixOffset(dstX, dstY);
            moveBotTo(dstX, dstY);
            break;
        case BeingAction::DEAD:
            botFixOffset(dstX, dstY);
            moveBotTo(dstX, dstY);
            break;
        case BeingAction::CAST:
        case BeingAction::HURT:
        default:
            break;
    }
}

void Being::botFixOffset(int &restrict dstX,
                         int &restrict dstY) const
{
    if ((mInfo == nullptr) || (mOwner == nullptr))
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
        case BeingAction::CAST:
        default:
            offsetX1 = mInfo->getTargetOffsetX();
            offsetY1 = mInfo->getTargetOffsetY();
            break;
    }

    int offsetX = offsetX1;
    int offsetY = offsetY1;
    switch (mOwner->mDirection)
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
    if (mMap != nullptr)
    {
        if (!mMap->getWalk(dstX, dstY, getBlockWalkMask()))
        {
            dstX = mOwner->mX;
            dstY = mOwner->mY;
        }
    }
}

void Being::updateBotFollow(int dstX,
                            int dstY,
                            const int divX,
                            const int divY)
{
    const int followDist = mInfo->getStartFollowDist();
    const int dist = mInfo->getFollowDist();
    if (divX > followDist || divY > followDist)
    {
        if (dist > 0)
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
        botFixOffset(dstX, dstY);
        moveBotTo(dstX, dstY);
    }
}

void Being::moveBotTo(int dstX,
                      int dstY)
{
    const int dstX0 = mOwner->mX;
    const int dstY0 = mOwner->mY;
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
        if (mType == ActorType::Homunculus)
            homunculusHandler->move(dstX, dstY);
        else
            mercenaryHandler->move(dstX, dstY);
        return;
    }
    updateBotDirection(dstX, dstY);
}

void Being::updateBotDirection(const int dstX,
                               const int dstY)
{
    int directionType = 0;
    switch (mOwner->getCurrentAction())
    {
        case BeingAction::STAND:
        case BeingAction::MOVE:
        case BeingAction::HURT:
        case BeingAction::SPAWN:
        case BeingAction::CAST:
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
            newDir = mOwner->mDirection;
            break;

        case 2:
        {
            const int dstX0 = mOwner->mX;
            const int dstY0 = mOwner->mY;
            if (dstX > dstX0)
                newDir |= BeingDirection::LEFT;
            else if (dstX < dstX0)
                newDir |= BeingDirection::RIGHT;
            if (dstY > dstY0)
                newDir |= BeingDirection::UP;
            else if (dstY < dstY0)
                newDir |= BeingDirection::DOWN;
            break;
        }
        case 3:
        {
            const int dstX0 = mOwner->mX;
            const int dstY0 = mOwner->mY;
            if (dstX > dstX0)
                newDir |= BeingDirection::RIGHT;
            else if (dstX < dstX0)
                newDir |= BeingDirection::LEFT;
            if (dstY > dstY0)
                newDir |= BeingDirection::DOWN;
            else if (dstY < dstY0)
                newDir |= BeingDirection::UP;
            break;
        }
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
    if ((newDir != 0u) && newDir != mDirection)
    {
        if (mType == ActorType::Homunculus)
            homunculusHandler->setDirection(newDir);
        else
            mercenaryHandler->setDirection(newDir);
    }
}

void Being::updateBadgesPosition()
{
    const int px = mPixelX - mapTileSize / 2;
    const int py = mPixelY - mapTileSize * 2 - mapTileSize;
    if (mShowBadges != BadgeDrawType::Hide &&
        mBadgesCount != 0u)
    {
        if (mDispName != nullptr &&
            gui != nullptr)
        {
            if (mShowBadges == BadgeDrawType::Right)
            {
                const Font *restrict const font = gui->getFont();
                mBadgesX = mDispName->getX() + mDispName->getWidth();
                mBadgesY = mDispName->getY() - font->getHeight();
            }
            else if (mShowBadges == BadgeDrawType::Bottom)
            {
                mBadgesX = px + 8 - mBadgesCount * 8;
                if (mVisibleNamePos == VisibleNamePos::Buttom)
                {
                    mBadgesY = mDispName->getY();
                }
                else
                {
                    mBadgesY = py + settings.playerNameOffset + 16;
                }
            }
            else
            {
                mBadgesX = px + 8 - mBadgesCount * 8;
                if (mVisibleNamePos == VisibleNamePos::Top)
                    mBadgesY = py - mDispName->getHeight();
                else
                    mBadgesY = py;
            }
        }
        else
        {
            if (mShowBadges == BadgeDrawType::Right)
            {
                mBadgesX = px + settings.playerBadgeAtRightOffset;
                mBadgesY = py;
            }
            else if (mShowBadges == BadgeDrawType::Bottom)
            {
                mBadgesX = px + 8 - mBadgesCount * 8;
                const int height = settings.playerNameOffset;
                if (mVisibleNamePos == VisibleNamePos::Buttom)
                    mBadgesY = py + height;
                else
                    mBadgesY = py + height + 16;
            }
            else
            {
                mBadgesX = px + 8 - mBadgesCount * 8;
                mBadgesY = py;
            }
        }
    }
}

void Being::drawEmotion(Graphics *restrict const graphics,
                        const int offsetX,
                        const int offsetY) const restrict2
{
    if (mErased)
        return;

    const int px = mPixelX - offsetX - mapTileSize / 2;
    const int py = mPixelY - offsetY - mapTileSize * 2 - mapTileSize;
    if (mAnimationEffect != nullptr)
        mAnimationEffect->draw(graphics, px, py);
    if (mShowBadges != BadgeDrawType::Hide &&
        mBadgesCount != 0u)
    {
        int x = mBadgesX - offsetX;
        const int y = mBadgesY - offsetY;
        for_each_badges()
        {
            const AnimatedSprite *restrict const sprite = mBadges[f];
            if (sprite != nullptr)
            {
                sprite->draw(graphics, x, y);
                x += 16;
            }
        }
    }
    if (mEmotionSprite != nullptr)
        mEmotionSprite->draw(graphics, px, py);
}

void Being::drawSpeech(const int offsetX,
                       const int offsetY) restrict2
{
    if (mErased)
        return;
    if (mSpeech.empty())
        return;

    const int px = mPixelX - offsetX;
    const int py = mPixelY - offsetY;
    const int speech = mSpeechType;

    // Draw speech above this being
    if (mSpeechTime == 0)
    {
        if (mSpeechBubble != nullptr &&
            mSpeechBubble->mVisible == Visible_true)
        {
            mSpeechBubble->setVisible(Visible_false);
        }
        mSpeech.clear();
    }
    else if (mSpeechTime > 0 && (speech == BeingSpeech::NAME_IN_BUBBLE ||
             speech == BeingSpeech::NO_NAME_IN_BUBBLE))
    {
        delete2(mText)

        if (mSpeechBubble != nullptr)
        {
            mSpeechBubble->setPosition(px - (mSpeechBubble->getWidth() / 2),
                py - getHeight() - (mSpeechBubble->getHeight()));
            mSpeechBubble->setVisible(Visible_true);
            mSpeechBubble->requestMoveToBackground();
        }
    }
    else if (mSpeechTime > 0 && speech == BeingSpeech::TEXT_OVERHEAD)
    {
        if (mSpeechBubble != nullptr)
            mSpeechBubble->setVisible(Visible_false);

        if ((mText == nullptr) && (userPalette != nullptr))
        {
            mText = new Text(mSpeech,
                mPixelX,
                mPixelY - getHeight(),
                Graphics::CENTER,
                &theme->getColor(ThemeColorId::BUBBLE_TEXT, 255),
                Speech_true);
            mText->adviseXY(mPixelX,
                (mY + 1) * mapTileSize - getHeight() - mText->getHeight() - 9,
                mMoveNames);
        }
    }
    else if (speech == BeingSpeech::NO_SPEECH)
    {
        if (mSpeechBubble != nullptr)
            mSpeechBubble->setVisible(Visible_false);
        delete2(mText)
    }
}

template<signed char pos, signed char neg>
int Being::getOffset() const restrict2
{
    // Check whether we're walking in the requested direction
    if (mAction != BeingAction::MOVE || !(mDirection & (pos | neg)))
        return 0;

    int offset = 0;

    if (mMap && mSpeed)
    {
        const int time = get_elapsed_time(mActionTime);
        offset = (pos == BeingDirection::LEFT &&
            neg == BeingDirection::RIGHT) ?
            (time * mMap->getTileWidth() / mSpeed)
            : (time * mMap->getTileHeight() / mSpeed);
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

void Being::updateCoords() restrict2
{
    if (mDispName != nullptr)
    {
        int offsetX = mPixelX;
        int offsetY = mPixelY;
        if (mInfo != nullptr)
        {
            offsetX += mInfo->getNameOffsetX();
            offsetY += mInfo->getNameOffsetY();
        }
        // Monster names show above the sprite instead of below it
        if (mType == ActorType::Monster ||
            mVisibleNamePos == VisibleNamePos::Top)
        {
            offsetY += - settings.playerNameOffset - mDispName->getHeight();
        }
        mDispName->adviseXY(offsetX, offsetY, mMoveNames);
    }
    updateBadgesPosition();
}

void Being::optionChanged(const std::string &restrict value) restrict2
{
    if (mType == ActorType::Player && value == "visiblenames")
    {
        setShowName(config.getIntValue("visiblenames") == VisibleName::Show);
        updateBadgesPosition();
    }
}

void Being::flashName(const int time) restrict2
{
    if (mDispName != nullptr)
        mDispName->flash(time);
}

std::string Being::getGenderSignWithSpace() const restrict2
{
    const std::string &restrict str = getGenderSign();
    if (str.empty())
        return str;
    else
        return std::string(" ").append(str);
}

std::string Being::getGenderSign() const restrict2
{
    std::string str;
    if (mShowGender)
    {
        if (getGender() == Gender::FEMALE)
            str = "\u2640";
        else if (getGender() == Gender::MALE)
            str = "\u2642";
    }
    if (mShowPlayersStatus &&
        mShowBadges == BadgeDrawType::Hide)
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

void Being::showName() restrict2
{
    if (mName.empty())
        return;

    delete2(mDispName);

    if (mHideErased && playerRelations.getRelation(mName) == Relation::ERASED)
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
    if ((localPlayer != nullptr) && localPlayer->getTarget() == this
        && mType != ActorType::Monster)
    {
        font = boldFont;
    }
    else if (mType == ActorType::Player
             && !playerRelations.isGoodName(this) && (gui != nullptr))
    {
        font = gui->getSecureFont();
    }

    if (mInfo != nullptr)
    {
        mDispName = new FlashText(displayName,
            mPixelX + mInfo->getNameOffsetX(),
            mPixelY + mInfo->getNameOffsetY(),
            Graphics::CENTER,
            mNameColor,
            font);
    }
    else
    {
        mDispName = new FlashText(displayName,
            mPixelX,
            mPixelY,
            Graphics::CENTER,
            mNameColor,
            font);
    }

    updateCoords();
}

void Being::setDefaultNameColor(const UserColorIdT defaultColor) restrict2
{
    switch (mTeamId)
    {
        case 0:
        default:
            mNameColor = &userPalette->getColor(defaultColor);
            break;
        case 1:
            mNameColor = &userPalette->getColor(UserColorId::TEAM1);
            break;
        case 2:
            mNameColor = &userPalette->getColor(UserColorId::TEAM2);
            break;
        case 3:
            mNameColor = &userPalette->getColor(UserColorId::TEAM3);
            break;
    }
}

void Being::updateColors()
{
    if (userPalette != nullptr)
    {
        if (mType == ActorType::Monster)
        {
            setDefaultNameColor(UserColorId::MONSTER);
            mTextColor = &userPalette->getColor(UserColorId::MONSTER);
        }
        else if (mType == ActorType::Npc)
        {
            setDefaultNameColor(UserColorId::NPC);
            mTextColor = &userPalette->getColor(UserColorId::NPC);
        }
        else if (mType == ActorType::Pet)
        {
            setDefaultNameColor(UserColorId::PET);
            mTextColor = &userPalette->getColor(UserColorId::PET);
        }
        else if (mType == ActorType::Homunculus)
        {
            setDefaultNameColor(UserColorId::HOMUNCULUS);
            mTextColor = &userPalette->getColor(UserColorId::HOMUNCULUS);
        }
        else if (mType == ActorType::SkillUnit)
        {
            setDefaultNameColor(UserColorId::SKILLUNIT);
            mTextColor = &userPalette->getColor(UserColorId::SKILLUNIT);
        }
        else if (this == localPlayer)
        {
            mNameColor = &userPalette->getColor(UserColorId::SELF);
            mTextColor = &theme->getColor(ThemeColorId::PLAYER, 255);
        }
        else
        {
            mTextColor = &theme->getColor(ThemeColorId::PLAYER, 255);

            if (playerRelations.getRelation(mName) != Relation::ERASED)
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
            else if ((mParty != nullptr) && (localPlayer != nullptr)
                     && mParty == localPlayer->getParty())
            {
                mNameColor = &userPalette->getColor(UserColorId::PARTY);
            }
            else if ((localPlayer != nullptr) && (getGuild() != nullptr)
                     && getGuild() == localPlayer->getGuild())
            {
                mNameColor = &userPalette->getColor(UserColorId::GUILD);
            }
            else if (playerRelations.getRelation(mName) == Relation::FRIEND)
            {
                mNameColor = &userPalette->getColor(UserColorId::FRIEND);
            }
            else if (playerRelations.getRelation(mName) ==
                     Relation::DISREGARDED
                     || playerRelations.getRelation(mName) ==
                     Relation::BLACKLISTED)
            {
                mNameColor = &userPalette->getColor(UserColorId::DISREGARDED);
            }
            else if (playerRelations.getRelation(mName)
                     == Relation::IGNORED ||
                     playerRelations.getRelation(mName) == Relation::ENEMY2)
            {
                mNameColor = &userPalette->getColor(UserColorId::IGNORED);
            }
            else if (playerRelations.getRelation(mName) == Relation::ERASED)
            {
                mNameColor = &userPalette->getColor(UserColorId::ERASED);
            }
            else
            {
                setDefaultNameColor(UserColorId::PC);
            }
        }

        if (mDispName != nullptr)
            mDispName->setColor(mNameColor);
    }
}

void Being::updateSprite(const unsigned int slot,
                         const int id,
                         const std::string &restrict color) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    if ((slot != 0u) && mSlots[slot].spriteId == id)
        return;
    setSpriteColor(slot,
        id,
        color);
}

// set sprite id, reset colors, reset cards
void Being::setSpriteId(const unsigned int slot,
                        const int id) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        const int id1 = mSlots[slot].spriteId;
        if (id1 != 0)
            removeItemParticles(id1);
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;
        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!filename.empty())
        {
            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"), filename));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);
        mSpriteDraw[slot] = id;

        addItemParticles(id, info.getDisplay());

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }

    BeingSlot &beingSlot = mSlots[slot];
    beingSlot.spriteId = id;
    beingSlot.color.clear();
    beingSlot.colorId = ItemColor_one;
    beingSlot.cardsId = CardsList(nullptr);
    recalcSpritesOrder();
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->updateBeing(this);
}

// reset sprite id, reset colors, reset cards
void Being::unSetSprite(const unsigned int slot) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    removeSprite(slot);
    mSpriteDraw[slot] = 0;

    BeingSlot &beingSlot = mSlots[slot];
    const int id1 = beingSlot.spriteId;
    if (id1 != 0)
        removeItemParticles(id1);

    beingSlot.spriteId = 0;
    beingSlot.color.clear();
    beingSlot.colorId = ItemColor_one;
    beingSlot.cardsId = CardsList(nullptr);
    recalcSpritesOrder();
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->updateBeing(this);
}

// set sprite id, use color string, reset cards
void Being::setSpriteColor(const unsigned int slot,
                           const int id,
                           const std::string &color) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    // disabled for now, because it may broke replace/reorder sprites logic
//    if (slot && mSlots[slot].spriteId == id)
//        return;

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        const int id1 = mSlots[slot].spriteId;
        if (id1 != 0)
            removeItemParticles(id1);
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;
        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!filename.empty())
        {
            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"),
                combineDye(filename, color)));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);
        mSpriteDraw[slot] = id;

        addItemParticles(id, info.getDisplay());

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }

    BeingSlot &beingSlot = mSlots[slot];
    beingSlot.spriteId = id;
    beingSlot.color = color;
    beingSlot.colorId = ItemColor_one;
    beingSlot.cardsId = CardsList(nullptr);
    recalcSpritesOrder();
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->updateBeing(this);
}

// set sprite id, use color id, reset cards
void Being::setSpriteColorId(const unsigned int slot,
                             const int id,
                             ItemColor colorId) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    // disabled for now, because it may broke replace/reorder sprites logic
//    if (slot && mSlots[slot].spriteId == id)
//        return;

    std::string color;

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        const int id1 = mSlots[slot].spriteId;
        if (id1 != 0)
            removeItemParticles(id1);
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;
        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!filename.empty())
        {
            color = info.getDyeColorsString(colorId);
            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"),
                combineDye(filename, color)));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);
        mSpriteDraw[slot] = id;

        addItemParticles(id, info.getDisplay());

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }

    BeingSlot &beingSlot = mSlots[slot];
    beingSlot.spriteId = id;
    beingSlot.color = STD_MOVE(color);
    beingSlot.colorId = colorId;
    beingSlot.cardsId = CardsList(nullptr);
    recalcSpritesOrder();
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->updateBeing(this);
}

// set sprite id, colors from cards, cards
void Being::setSpriteCards(const unsigned int slot,
                           const int id,
                           const CardsList &cards) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    // disabled for now, because it may broke replace/reorder sprites logic
//    if (slot && mSlots[slot].spriteId == id)
//        return;

    ItemColor colorId = ItemColor_one;
    std::string color;

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        const int id1 = mSlots[slot].spriteId;
        if (id1 != 0)
            removeItemParticles(id1);
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;
        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!cards.isEmpty())
            colorId = ItemColorManager::getColorFromCards(cards);

        if (!filename.empty())
        {
            if (color.empty())
                color = info.getDyeColorsString(colorId);

            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"),
                combineDye(filename, color)));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);
        mSpriteDraw[slot] = id;

        addItemParticlesCards(id,
            info.getDisplay(),
            cards);

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }

    BeingSlot &beingSlot = mSlots[slot];
    beingSlot.spriteId = id;
    beingSlot.color = STD_MOVE(color);
    beingSlot.colorId = colorId;
    beingSlot.cardsId = CardsList(cards);
    recalcSpritesOrder();
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->updateBeing(this);
}

void Being::setWeaponId(const int id) restrict2
{
    if (id == 0)
        mEquippedWeapon = nullptr;
    else
        mEquippedWeapon = &ItemDB::get(id);
}

void Being::setTempSprite(const unsigned int slot,
                          const int id) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    BeingSlot &beingSlot = mSlots[slot];

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        const int id1 = beingSlot.spriteId;
        if (id1 != 0)
            removeItemParticles(id1);
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;

        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!filename.empty())
        {
            ItemColor colorId = ItemColor_one;
            const CardsList &cards = beingSlot.cardsId;
            if (!cards.isEmpty())
                colorId = ItemColorManager::getColorFromCards(cards);
            std::string color = beingSlot.color;
            if (color.empty())
                color = info.getDyeColorsString(colorId);

            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"),
                combineDye(filename, color)));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);
        mSpriteDraw[slot] = id;

        // +++ here probably need use existing cards
        addItemParticles(id, info.getDisplay());

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }
}

void Being::setHairTempSprite(const unsigned int slot,
                              const int id) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    const CardsList &cards = mSlots[slot].cardsId;

    // id = 0 means unequip
    if (id == 0)
    {
        removeSprite(slot);
        mSpriteDraw[slot] = 0;

        const int id1 = mSlots[slot].spriteId;
        if (id1 != 0)
            removeItemParticles(id1);
    }
    else
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;
        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!filename.empty())
        {
            ItemColor colorId = ItemColor_one;
            if (!cards.isEmpty())
                colorId = ItemColorManager::getColorFromCards(cards);

            std::string color = info.getDyeColorsString(mHairColor);
            if (color.empty())
                color = info.getDyeColorsString(colorId);

            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"),
                combineDye(filename, color)));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);
        mSpriteDraw[slot] = id;

        addItemParticles(id, info.getDisplay());

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }
}

void Being::setHairColorSpriteID(const unsigned int slot,
                                 const int id) restrict2
{
    BeingSlot &beingSlot = mSlots[slot];
    setSpriteColor(slot,
        id,
        beingSlot.color);
}

void Being::setSpriteColor(const unsigned int slot,
                           const std::string &restrict color) restrict2
{
    if (charServerHandler == nullptr || slot >= charServerHandler->maxSprite())
        return;

    if (slot >= CAST_U32(mSprites.size()))
        ensureSize(slot + 1);

    if (slot >= CAST_U32(mSlots.size()))
        mSlots.resize(slot + 1, BeingSlot());

    // disabled for now, because it may broke replace/reorder sprites logic
//    if (slot && mSlots[slot].spriteId == id)
//        return;

    BeingSlot &beingSlot = mSlots[slot];
    const int id = beingSlot.spriteId;

    // id = 0 means unequip
    if (id != 0)
    {
        const ItemInfo &info = ItemDB::get(id);
        const std::string &restrict filename = info.getSprite(
            mGender, mSubType);
        int lastTime = 0;
        int startTime = 0;
        AnimatedSprite *restrict equipmentSprite = nullptr;

        if (!filename.empty())
        {
            equipmentSprite = AnimatedSprite::delayedLoad(
                pathJoin(paths.getStringValue("sprites"),
                combineDye(filename, color)));
        }

        if (equipmentSprite != nullptr)
        {
            equipmentSprite->setSpriteDirection(getSpriteDirection());
            startTime = getStartTime();
            lastTime = getLastTime();
        }

        CompoundSprite::setSprite(slot, equipmentSprite);

        setAction(mAction, 0);
        if (equipmentSprite != nullptr)
        {
            if (lastTime > 0)
            {
                equipmentSprite->setLastTime(startTime);
                equipmentSprite->update(lastTime);
            }
        }
    }

    beingSlot.color = color;
    beingSlot.colorId = ItemColor_one;
    if (beingEquipmentWindow != nullptr)
        beingEquipmentWindow->updateBeing(this);
}

void Being::setHairStyle(const unsigned int slot,
                         const int id) restrict2
{
    if (id != 0)
    {
        setSpriteColor(slot,
            id,
            ItemDB::get(id).getDyeColorsString(mHairColor));
    }
    else
    {
        setSpriteColor(slot,
            0,
            std::string());
    }
}

void Being::setHairColor(const unsigned int slot,
                         const ItemColor color) restrict2
{
    mHairColor = color;
    BeingSlot &beingSlot = mSlots[slot];
    const int id = beingSlot.spriteId;
    if (id != 0)
    {
        setSpriteColor(slot,
            id,
            ItemDB::get(id).getDyeColorsString(color));
    }
}

void Being::setSpriteSlot(const unsigned int slot,
                          const BeingSlot &beingSlot)
{
    mSlots[slot] = beingSlot;
}

void Being::dumpSprites() const restrict2
{
    STD_VECTOR<BeingSlot>::const_iterator it1 = mSlots.begin();
    const STD_VECTOR<BeingSlot>::const_iterator it1_end = mSlots.end();

    logger->log("sprites");
    for (; it1 != it1_end;
         ++ it1)
    {
        logger->log("%d,%s,%d",
            (*it1).spriteId,
            (*it1).color.c_str(),
            toInt((*it1).colorId, int));
    }
}

void Being::updateName() restrict2
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
        mShowBadges = static_cast<BadgeDrawType::Type>(
            config.getIntValue("showBadges"));
        mVisibleNamePos = static_cast<VisibleNamePos::Type>(
            config.getIntValue("visiblenamespos"));

        mUpdateConfigTime = cur_time;
    }
    BLOCK_END("Being::reReadConfig")
}

bool Being::updateFromCache() restrict2
{
    const BeingCacheEntry *restrict const entry =
        Being::getCacheEntry(getId());

    if ((entry != nullptr) && entry->getTime() + 120 >= cur_time)
    {
        if (!entry->getName().empty())
            setName(entry->getName());
        setPartyName(entry->getPartyName());
        setGuildName(entry->getGuildName());
        setLevel(entry->getLevel());
        setPvpRank(entry->getPvpRank());
        setIp(entry->getIp());
        setTeamId(entry->getTeamId());

        mAdvanced = entry->isAdvanced();
        if (mAdvanced)
        {
            const int flags = entry->getFlags();
            if ((serverFeatures != nullptr) &&
                Net::getNetworkType() == ServerType::TMWATHENA)
            {
                mShop = ((flags & BeingFlag::SHOP) != 0);
            }
            mAway = ((flags & BeingFlag::AWAY) != 0);
            mInactive = ((flags & BeingFlag::INACTIVE) != 0);
            if (mShop || mAway || mInactive)
                updateName();
        }
        else
        {
            if (Net::getNetworkType() == ServerType::TMWATHENA)
                mShop = false;
            mAway = false;
            mInactive = false;
        }

        showShopBadge(mShop);
        showInactiveBadge(mInactive);
        showAwayBadge(mAway);
        updateAwayEffect();
        if (mType == ActorType::Player || (mTeamId != 0u))
            updateColors();
        return true;
    }
    return false;
}

void Being::addToCache() const restrict2
{
    if (localPlayer == this)
        return;

    BeingCacheEntry *entry = Being::getCacheEntry(getId());
    if (entry == nullptr)
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

    entry->setName(mName);
    entry->setLevel(getLevel());
    entry->setPartyName(getPartyName());
    entry->setGuildName(getGuildName());
    entry->setTime(cur_time);
    entry->setPvpRank(getPvpRank());
    entry->setIp(getIp());
    entry->setAdvanced(isAdvanced());
    entry->setTeamId(getTeamId());
    if (isAdvanced())
    {
        int flags = 0;
        if (Net::getNetworkType() == ServerType::TMWATHENA && mShop)
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
        if (*i == nullptr)
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


void Being::setGender(const GenderT gender) restrict2
{
    if (charServerHandler == nullptr)
        return;

    if (gender != mGender)
    {
        mGender = gender;

        const unsigned int sz = CAST_U32(mSlots.size());

        if (sz > CAST_U32(mSprites.size()))
            ensureSize(sz);

        // Reload all subsprites
        for (unsigned int i = 0;
             i < sz;
             i++)
        {
            BeingSlot &beingSlot = mSlots[i];
            const int id = beingSlot.spriteId;
            if (id != 0)
            {
                const ItemInfo &info = ItemDB::get(id);
                const std::string &restrict filename = info.getSprite(
                    mGender, mSubType);
                int lastTime = 0;
                int startTime = 0;
                AnimatedSprite *restrict equipmentSprite = nullptr;

                if (!filename.empty())
                {
                    equipmentSprite = AnimatedSprite::delayedLoad(
                        pathJoin(paths.getStringValue("sprites"),
                        combineDye(filename, beingSlot.color)));
                }

                if (equipmentSprite != nullptr)
                {
                    equipmentSprite->setSpriteDirection(getSpriteDirection());
                    startTime = getStartTime();
                    lastTime = getLastTime();
                }

                CompoundSprite::setSprite(i, equipmentSprite);
                setAction(mAction, 0);
                if (equipmentSprite != nullptr)
                {
                    if (lastTime > 0)
                    {
                        equipmentSprite->setLastTime(startTime);
                        equipmentSprite->update(lastTime);
                    }
                }

                if (beingEquipmentWindow != nullptr)
                    beingEquipmentWindow->updateBeing(this);
            }
        }

        updateName();
    }
}

void Being::showGmBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Gm]);
    if (show &&
        mIsGM &&
        mShowBadges != BadgeDrawType::Hide &&
        GroupDb::getShowBadge(mGroupId))
    {
        const std::string &gmBadge = GroupDb::getBadge(mGroupId);
        if (!gmBadge.empty())
        {
            mBadges[BadgeIndex::Gm] = AnimatedSprite::load(
                paths.getStringValue("badges") + gmBadge);
        }
    }
    updateBadgesCount();
    updateBadgesPosition();
}

void Being::setGM(const bool gm) restrict2
{
    if (mIsGM != gm)
    {
        mIsGM = gm;

        showGmBadge(mIsGM);
        updateColors();
    }
}

void Being::talkTo() const restrict2
{
    if (npcHandler == nullptr)
        return;

    if (!PacketLimiter::limitPackets(PacketType::PACKET_NPC_TALK))
    {
        // using workaround...
        if ((playerHandler != nullptr) &&
            PacketLimiter::limitPackets(PacketType::PACKET_ATTACK))
        {
            playerHandler->attack(mId, Keep_false);
        }
        return;
    }

    npcHandler->talk(this);
}

void Being::drawPlayer(Graphics *restrict const graphics,
                       const int offsetX,
                       const int offsetY) const restrict2
{
    if (!mErased)
    {
        // getActorX() + offsetX;
        const int px = mPixelX - mapTileSize / 2 + offsetX;
        // getActorY() + offsetY;
        const int py = mPixelY - mapTileSize + offsetY;
        if (mHorseInfo != nullptr)
        {
            HorseOffset &offset = mHorseInfo->offsets[mSpriteDirection];
            for_each_horses(mDownHorseSprites)
            {
                (*it)->draw(graphics,
                    px + offset.downOffsetX,
                    py + offset.downOffsetY);
            }

            drawBeingCursor(graphics, px, py);
            drawPlayerSpriteAt(graphics,
                px + offset.riderOffsetX,
                py + offset.riderOffsetY);

            for_each_horses(mUpHorseSprites)
            {
                (*it)->draw(graphics,
                    px + offset.upOffsetX,
                    py + offset.upOffsetY);
            }
        }
        else
        {
            drawBeingCursor(graphics, px, py);
            drawPlayerSpriteAt(graphics, px, py);
        }
    }
}

void Being::drawBeingCursor(Graphics *const graphics,
                            const int offsetX,
                            const int offsetY) const
{
    if (mUsedTargetCursor != nullptr)
    {
        mUsedTargetCursor->update(tick_time * MILLISECONDS_IN_A_TICK);
        if (mInfo == nullptr)
        {
            mUsedTargetCursor->draw(graphics,
                offsetX - mCursorPaddingX,
                offsetY - mCursorPaddingY);
        }
        else
        {
            mUsedTargetCursor->draw(graphics,
                offsetX + mInfo->getTargetOffsetX() - mCursorPaddingX,
                offsetY + mInfo->getTargetOffsetY() - mCursorPaddingY);
        }
    }
}

void Being::drawOther(Graphics *restrict const graphics,
                      const int offsetX,
                      const int offsetY) const restrict2
{
    // getActorX() + offsetX;
    const int px = mPixelX - mapTileSize / 2 + offsetX;
    // getActorY() + offsetY;
    const int py = mPixelY - mapTileSize + offsetY;
    drawBeingCursor(graphics, px, py);
    drawOtherSpriteAt(graphics, px, py);
}

void Being::drawMonster(Graphics *restrict const graphics,
                        const int offsetX,
                        const int offsetY) const restrict2
{
    // getActorX() + offsetX;
    const int px = mPixelX - mapTileSize / 2 + offsetX;
    // getActorY() + offsetY;
    const int py = mPixelY - mapTileSize + offsetY;
    drawBeingCursor(graphics, px, py);
    drawMonsterSpriteAt(graphics, px, py);
}

void Being::drawHomunculus(Graphics *restrict const graphics,
                           const int offsetX,
                           const int offsetY) const restrict2
{
    // getActorX() + offsetX;
    const int px = mPixelX - mapTileSize / 2 + offsetX;
    // getActorY() + offsetY;
    const int py = mPixelY - mapTileSize + offsetY;
    drawBeingCursor(graphics, px, py);
    drawHomunculusSpriteAt(graphics, px, py);
}

void Being::drawMercenary(Graphics *restrict const graphics,
                          const int offsetX,
                          const int offsetY) const restrict2
{
    // getActorX() + offsetX;
    const int px = mPixelX - mapTileSize / 2 + offsetX;
    // getActorY() + offsetY;
    const int py = mPixelY - mapTileSize + offsetY;
    drawBeingCursor(graphics, px, py);
    drawMercenarySpriteAt(graphics, px, py);
}

void Being::drawElemental(Graphics *restrict const graphics,
                          const int offsetX,
                          const int offsetY) const restrict2
{
    // getActorX() + offsetX;
    const int px = mPixelX - mapTileSize / 2 + offsetX;
    // getActorY() + offsetY;
    const int py = mPixelY - mapTileSize + offsetY;
    drawBeingCursor(graphics, px, py);
    drawElementalSpriteAt(graphics, px, py);
}

void Being::drawPortal(Graphics *restrict const graphics,
                       const int offsetX,
                       const int offsetY) const restrict2
{
    // getActorX() + offsetX;
    const int px = mPixelX - mapTileSize / 2 + offsetX;
    // getActorY() + offsetY;
    const int py = mPixelY - mapTileSize + offsetY;
    drawPortalSpriteAt(graphics, px, py);
}

void Being::draw(Graphics *restrict const graphics,
                 const int offsetX,
                 const int offsetY) const restrict2
{
    switch (mType)
    {
        case ActorType::Player:
            drawPlayer(graphics,
                offsetX,
                offsetY);
            break;
        case ActorType::Portal:
            drawPortal(graphics,
                offsetX,
                offsetY);
            break;
        case ActorType::Homunculus:
            drawHomunculus(graphics,
                offsetX,
                offsetY);
            break;
        case ActorType::Mercenary:
            drawMercenary(graphics,
                offsetX,
                offsetY);
            break;
        case ActorType::Elemental:
            drawElemental(graphics,
                offsetX,
                offsetY);
            break;
        case ActorType::Monster:
            drawMonster(graphics,
                offsetX,
                offsetY);
            break;
        case ActorType::Pet:
        case ActorType::SkillUnit:
        case ActorType::Unknown:
        case ActorType::Npc:
        case ActorType::FloorItem:
        case ActorType::Avatar:
        default:
            drawOther(graphics,
                offsetX,
                offsetY);
            break;
    }
}

void Being::drawPlayerSprites(Graphics *restrict const graphics,
                              const int posX,
                              const int posY) const restrict2
{
    const int sz = CompoundSprite::getNumberOfLayers();
    for (int f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        Sprite *restrict const sprite = mSprites[mSpriteRemap[f]];
        if (sprite != nullptr)
        {
            sprite->setAlpha(mAlpha);
            sprite->draw(graphics, posX, posY);
        }
    }
}

void Being::drawSpritesSDL(Graphics *restrict const graphics,
                           const int posX,
                           const int posY) const restrict2
{
    const size_t sz = mSprites.size();
    for (size_t f = 0; f < sz; f ++)
    {
        const int rSprite = mSpriteHide[mSpriteRemap[f]];
        if (rSprite == 1)
            continue;

        const Sprite *restrict const sprite = mSprites[mSpriteRemap[f]];
        if (sprite != nullptr)
            sprite->draw(graphics, posX, posY);
    }
}

void Being::drawBasic(Graphics *restrict const graphics,
                      const int x,
                      const int y) const restrict2
{
    drawCompound(graphics, x, y);
}

void Being::drawCompound(Graphics *const graphics,
                         const int posX,
                         const int posY) const
{
    FUNC_BLOCK("CompoundSprite::draw", 1)
    if (mNeedsRedraw)
        updateImages();

    if (mSprites.empty())  // Nothing to draw
        return;

    if (mAlpha == 1.0F && (mImage != nullptr))
    {
        graphics->drawImage(mImage,
            posX + mOffsetX,
            posY + mOffsetY);
    }
    else if ((mAlpha != 0.0f) && (mAlphaImage != nullptr))
    {
        mAlphaImage->setAlpha(mAlpha);
        graphics->drawImage(mAlphaImage,
            posX + mOffsetX,
            posY + mOffsetY);
    }
    else
    {
        Being::drawPlayerSprites(graphics, posX, posY);
    }
}

void Being::drawPlayerSpriteAt(Graphics *restrict const graphics,
                               const int x,
                               const int y) const restrict2
{
    drawCompound(graphics, x, y);

    if (mShowOwnHP &&
        (mInfo != nullptr) &&
        localPlayer == this &&
        mAction != BeingAction::DEAD)
    {
        drawHpBar(graphics,
            PlayerInfo::getAttribute(Attributes::PLAYER_MAX_HP),
            PlayerInfo::getAttribute(Attributes::PLAYER_HP),
            0,
            UserColorId::PLAYER_HP,
            UserColorId::PLAYER_HP2,
            x - 50 + mapTileSize / 2 + mInfo->getHpBarOffsetX(),
            y + mapTileSize - 6 + mInfo->getHpBarOffsetY(),
            2 * 50,
            4);
    }
}

void Being::drawOtherSpriteAt(Graphics *restrict const graphics,
                              const int x,
                              const int y) const restrict2
{
    CompoundSprite::drawSimple(graphics, x, y);
}

void Being::drawMonsterSpriteAt(Graphics *restrict const graphics,
                                const int x,
                                const int y) const restrict2
{
    if (mHighlightMonsterAttackRange &&
        mType == ActorType::Monster &&
        mAction != BeingAction::DEAD)
    {
        if (userPalette == nullptr)
        {
            CompoundSprite::drawSimple(graphics, x, y);
            return;
        }

        int attackRange;
        if (mAttackRange != 0)
            attackRange = mapTileSize * mAttackRange;
        else
            attackRange = mapTileSize;

        graphics->setColor(userPalette->getColorWithAlpha(
            UserColorId::MONSTER_ATTACK_RANGE));

        graphics->fillRectangle(Rect(
            x - attackRange, y - attackRange,
            2 * attackRange + mapTileSize, 2 * attackRange + mapTileSize));
    }

    CompoundSprite::drawSimple(graphics, x, y);

    if (mShowMobHP &&
        (mInfo != nullptr) &&
        (localPlayer != nullptr) &&
        localPlayer->getTarget() == this &&
        mType == ActorType::Monster)
    {
        // show hp bar here
        int maxHP = mMaxHP;
        if (maxHP == 0)
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
}

void Being::drawHomunculusSpriteAt(Graphics *restrict const graphics,
                                   const int x,
                                   const int y) const restrict2
{
    if (mHighlightMonsterAttackRange &&
        mAction != BeingAction::DEAD)
    {
        if (userPalette == nullptr)
        {
            CompoundSprite::drawSimple(graphics, x, y);
            return;
        }

        int attackRange;
        if (mAttackRange != 0)
            attackRange = mapTileSize * mAttackRange;
        else
            attackRange = mapTileSize;

        graphics->setColor(userPalette->getColorWithAlpha(
            UserColorId::MONSTER_ATTACK_RANGE));

        graphics->fillRectangle(Rect(
            x - attackRange, y - attackRange,
            2 * attackRange + mapTileSize, 2 * attackRange + mapTileSize));
    }

    CompoundSprite::drawSimple(graphics, x, y);

    if (mShowMobHP &&
        (mInfo != nullptr))
    {
        const HomunculusInfo *const info = PlayerInfo::getHomunculus();
        if ((info != nullptr) &&
            mId == info->id)
        {
            // show hp bar here
            int maxHP = PlayerInfo::getStatBase(Attributes::HOMUN_MAX_HP);
            if (maxHP == 0)
                maxHP = mInfo->getMaxHP();

            drawHpBar(graphics,
                maxHP,
                PlayerInfo::getStatBase(Attributes::HOMUN_HP),
                mDamageTaken,
                UserColorId::HOMUN_HP,
                UserColorId::HOMUN_HP2,
                x - 50 + mapTileSize / 2 + mInfo->getHpBarOffsetX(),
                y + mapTileSize - 6 + mInfo->getHpBarOffsetY(),
                2 * 50,
                4);
        }
    }
}

void Being::drawMercenarySpriteAt(Graphics *restrict const graphics,
                                  const int x,
                                  const int y) const restrict2
{
    if (mHighlightMonsterAttackRange &&
        mAction != BeingAction::DEAD)
    {
        if (userPalette == nullptr)
        {
            CompoundSprite::drawSimple(graphics, x, y);
            return;
        }

        int attackRange;
        if (mAttackRange != 0)
            attackRange = mapTileSize * mAttackRange;
        else
            attackRange = mapTileSize;

        graphics->setColor(userPalette->getColorWithAlpha(
            UserColorId::MONSTER_ATTACK_RANGE));

        graphics->fillRectangle(Rect(
            x - attackRange, y - attackRange,
            2 * attackRange + mapTileSize, 2 * attackRange + mapTileSize));
    }

    CompoundSprite::drawSimple(graphics, x, y);

    if (mShowMobHP &&
        (mInfo != nullptr))
    {
        const MercenaryInfo *const info = PlayerInfo::getMercenary();
        if ((info != nullptr) &&
            mId == info->id)
        {
            // show hp bar here
            int maxHP = PlayerInfo::getStatBase(Attributes::MERC_MAX_HP);
            if (maxHP == 0)
                maxHP = mInfo->getMaxHP();

            drawHpBar(graphics,
                maxHP,
                PlayerInfo::getStatBase(Attributes::MERC_HP),
                mDamageTaken,
                UserColorId::MERC_HP,
                UserColorId::MERC_HP2,
                x - 50 + mapTileSize / 2 + mInfo->getHpBarOffsetX(),
                y + mapTileSize - 6 + mInfo->getHpBarOffsetY(),
                2 * 50,
                4);
        }
    }
}

void Being::drawElementalSpriteAt(Graphics *restrict const graphics,
                                  const int x,
                                  const int y) const restrict2
{
    if (mHighlightMonsterAttackRange &&
        mAction != BeingAction::DEAD)
    {
        if (userPalette == nullptr)
        {
            CompoundSprite::drawSimple(graphics, x, y);
            return;
        }

        int attackRange;
        if (mAttackRange != 0)
            attackRange = mapTileSize * mAttackRange;
        else
            attackRange = mapTileSize;

        graphics->setColor(userPalette->getColorWithAlpha(
            UserColorId::MONSTER_ATTACK_RANGE));

        graphics->fillRectangle(Rect(
            x - attackRange, y - attackRange,
            2 * attackRange + mapTileSize, 2 * attackRange + mapTileSize));
    }

    CompoundSprite::drawSimple(graphics, x, y);

    if (mShowMobHP &&
        (mInfo != nullptr))
    {
        if (mId == PlayerInfo::getElementalId())
        {
            // show hp bar here
            int maxHP = PlayerInfo::getStatBase(Attributes::ELEMENTAL_MAX_HP);
            if (maxHP == 0)
                maxHP = mInfo->getMaxHP();

            drawHpBar(graphics,
                maxHP,
                PlayerInfo::getStatBase(Attributes::ELEMENTAL_HP),
                mDamageTaken,
                UserColorId::ELEMENTAL_HP,
                UserColorId::ELEMENTAL_HP2,
                x - 50 + mapTileSize / 2 + mInfo->getHpBarOffsetX(),
                y + mapTileSize - 6 + mInfo->getHpBarOffsetY(),
                2 * 50,
                4);
        }
    }
}

void Being::drawPortalSpriteAt(Graphics *restrict const graphics,
                               const int x,
                               const int y) const restrict2
{
    if (mHighlightMapPortals &&
        (mMap != nullptr) &&
        !mMap->getHasWarps())
    {
        if (userPalette == nullptr)
        {
            CompoundSprite::drawSimple(graphics, x, y);
            return;
        }

        graphics->setColor(userPalette->
            getColorWithAlpha(UserColorId::PORTAL_HIGHLIGHT));

        graphics->fillRectangle(Rect(x, y,
            mapTileSize, mapTileSize));

        if (mDrawHotKeys && !mName.empty())
        {
            const Color &color = userPalette->getColor(UserColorId::BEING);
            gui->getFont()->drawString(graphics, color, color, mName, x, y);
        }
    }

    CompoundSprite::drawSimple(graphics, x, y);
}

void Being::drawHpBar(Graphics *restrict const graphics,
                      const int maxHP,
                      const int hp,
                      const int damage,
                      const UserColorIdT color1,
                      const UserColorIdT color2,
                      const int x,
                      const int y,
                      const int width,
                      const int height) const restrict2
{
    if (maxHP <= 0 || (userPalette == nullptr))
        return;

    float p;

    if (hp != 0)
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

    const int dx = static_cast<int>(static_cast<float>(width) / p);

#ifdef TMWA_SUPPORT
    if (!serverFeatures->haveServerHp())
    {   // old servers
        if ((damage == 0 && (this != localPlayer || hp == maxHP))
            || (hp == 0 && maxHP == damage))
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
#endif  // TMWA_SUPPORT
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

void Being::setHP(const int hp) restrict2
{
    mHP = hp;
    if (mMaxHP < mHP)
        mMaxHP = mHP;
    if (mType == ActorType::Monster)
        updatePercentHP();
}

void Being::setMaxHP(const int hp) restrict2
{
    mMaxHP = hp;
    if (mMaxHP < mHP)
        mMaxHP = mHP;
}

void Being::resetCounters() restrict2
{
    mMoveTime = 0;
    mAttackTime = 0;
    mTalkTime = 0;
    mOtherTime = 0;
    mTestTime = cur_time;
}

void Being::recalcSpritesOrder() restrict2
{
    if (!mEnableReorderSprites)
        return;

//    logger->log("recalcSpritesOrder");
    const size_t sz = mSprites.size();
    if (sz < 1)
        return;

    STD_VECTOR<int> slotRemap;
    IntMap itemSlotRemap;

    STD_VECTOR<int>::iterator it;
    int oldHide[20];
    bool updatedSprite[20];
    int dir = mSpriteDirection;
    if (dir < 0 || dir >= 9)
        dir = 0;
    // hack for allow different logic in dead player
    if (mAction == BeingAction::DEAD)
        dir = 9;

    const unsigned int hairSlot = charServerHandler->hairSprite();

    for (size_t slot = sz; slot < 20; slot ++)
    {
        oldHide[slot] = 0;
        updatedSprite[slot] = false;
    }

    for (size_t slot = 0; slot < sz; slot ++)
    {
        oldHide[slot] = mSpriteHide[slot];
        mSpriteHide[slot] = 0;
        updatedSprite[slot] = false;
    }

    size_t spriteIdSize = mSlots.size();
    if (reportTrue(spriteIdSize > 20))
        spriteIdSize = 20;

    for (size_t slot = 0; slot < sz; slot ++)
    {
        slotRemap.push_back(CAST_S32(slot));

        if (spriteIdSize <= slot)
            continue;

        const int id = mSlots[slot].spriteId;
        if (id == 0)
            continue;

        const ItemInfo &info = ItemDB::get(id);

        if (info.isRemoveSprites())
        {
            const SpriteToItemMap *restrict const spriteToItems
                = info.getSpriteToItemReplaceMap(dir);

            if (spriteToItems != nullptr)
            {
                FOR_EACHP (SpriteToItemMapCIter, itr, spriteToItems)
                {
                    const int remSprite = itr->first;
                    const IntMap &restrict itemReplacer = itr->second;
                    if (remSprite >= 0)
                    {   // slot known
                        if (itemReplacer.empty())
                        {
                            mSpriteHide[remSprite] = 1;
                        }
                        else if (mSpriteHide[remSprite] != 1)
                        {
                            IntMapCIter repIt = itemReplacer.find(
                                mSlots[remSprite].spriteId);
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
                                    if (CAST_U32(remSprite)
                                        != hairSlot)
                                    {
                                        setTempSprite(remSprite,
                                            repIt->second);
                                    }
                                    else
                                    {
                                        setHairTempSprite(remSprite,
                                            repIt->second);
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
                                if (mSlots[slot2].spriteId == repIt->first)
                                {
                                    mSpriteHide[slot2] = repIt->second;
                                    if (repIt->second != 1)
                                    {
                                        if (slot2 != hairSlot)
                                        {
                                            setTempSprite(slot2,
                                                repIt->second);
                                        }
                                        else
                                        {
                                            setHairTempSprite(slot2,
                                                repIt->second);
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
            const int id2 = mSlots[info.mDrawBefore[dir]].spriteId;
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
            const int id2 = mSlots[info.mDrawAfter[dir]].spriteId;
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

            if (CAST_S32(spriteIdSize) > val)
                id = mSlots[val].spriteId;

            int idx = -1;
            int idx1 = -1;
//            logger->log("item %d, id=%d", slot, id);
            int reorder = 0;
            const IntMapCIter orderIt = itemSlotRemap.find(id);
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
                const BeingSlot &beingSlot = mSlots[slot];
                const int id = beingSlot.spriteId;
                if (id == 0)
                    continue;

                updatedSprite[slot] = true;
                setTempSprite(slot,
                    id);
            }
        }
    }
    for (size_t slot = 0; slot < spriteIdSize; slot ++)
    {
        if (mSpriteHide[slot] == 0)
        {
            const BeingSlot &beingSlot = mSlots[slot];
            const int id = beingSlot.spriteId;
            if (updatedSprite[slot] == false &&
                mSpriteDraw[slot] != id)
            {
                setTempSprite(static_cast<unsigned int>(slot),
                    id);
            }
        }
    }
}

int Being::searchSlotValue(const STD_VECTOR<int> &restrict slotRemap,
                           const int val) const restrict2
{
    const size_t sz = mSprites.size();
    for (size_t slot = 0; slot < sz; slot ++)
    {
        if (slotRemap[slot] == val)
            return CAST_S32(slot);
    }
    return CompoundSprite::getNumberOfLayers() - 1;
}

void Being::searchSlotValueItr(STD_VECTOR<int>::iterator &restrict it,
                               int &restrict idx,
                               STD_VECTOR<int> &restrict slotRemap,
                               const int val)
{
//    logger->log("searching %d", val);
    it = slotRemap.begin();
    const STD_VECTOR<int>::iterator it_end = slotRemap.end();
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
}

void Being::updateHit(const int amount) restrict2
{
    if (amount > 0)
    {
        if ((mMinHit == 0) || amount < mMinHit)
            mMinHit = amount;
        if (amount != mCriticalHit && ((mMaxHit == 0) || amount > mMaxHit))
            mMaxHit = amount;
    }
}

Equipment *Being::getEquipment() restrict2
{
    Equipment *restrict const eq = new Equipment;
    Equipment::Backend *restrict const bk = new BeingEquipBackend(this);
    eq->setBackend(bk);
    return eq;
}

void Being::undressItemById(const int id) restrict2
{
    const size_t sz = mSlots.size();

    for (size_t f = 0; f < sz; f ++)
    {
        if (id == mSlots[f].spriteId)
        {
            unSetSprite(CAST_U32(f));
            break;
        }
    }
}

void Being::clearCache()
{
    delete_all(beingInfoCache);
    beingInfoCache.clear();
}

void Being::updateComment() restrict2
{
    if (mGotComment || mName.empty())
        return;

    mGotComment = true;
    mComment = loadComment(mName, mType);
}

std::string Being::loadComment(const std::string &restrict name,
                               const ActorTypeT &restrict type)
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
        case ActorType::Avatar:
        case ActorType::Mercenary:
        case ActorType::Homunculus:
        case ActorType::Pet:
        case ActorType::SkillUnit:
        case ActorType::Elemental:
        default:
            return "";
    }

    str = pathJoin(str, stringToHexPath(name), "comment.txt");
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
        case ActorType::Avatar:
        case ActorType::Unknown:
        case ActorType::Pet:
        case ActorType::Mercenary:
        case ActorType::Homunculus:
        case ActorType::SkillUnit:
        case ActorType::Elemental:
        default:
            return;
    }
    dir = pathJoin(dir, stringToHexPath(name));
    Files::saveTextFile(dir,
        "comment.txt",
        (name + "\n").append(comment));
}

void Being::setState(const uint8_t state) restrict2
{
    const bool shop = ((state & BeingFlag::SHOP) != 0);
    const bool away = ((state & BeingFlag::AWAY) != 0);
    const bool inactive = ((state & BeingFlag::INACTIVE) != 0);
    const bool needUpdate = (shop != mShop || away != mAway
        || inactive != mInactive);

    if (Net::getNetworkType() == ServerType::TMWATHENA)
        mShop = shop;
    mAway = away;
    mInactive = inactive;
    updateAwayEffect();
    showShopBadge(mShop);
    showInactiveBadge(mInactive);
    showAwayBadge(mAway);

    if (needUpdate)
    {
        if (shop || away || inactive)
            mAdvanced = true;
        updateName();
        addToCache();
    }
}

void Being::setEmote(const uint8_t emotion,
                     const int emote_time) restrict2
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
            if (info != nullptr)
            {
                const EmoteSprite *restrict const sprite =
                    info->sprites.front();
                if (sprite != nullptr)
                {
                    mEmotionSprite = AnimatedSprite::clone(sprite->sprite);
                    if (mEmotionSprite != nullptr)
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

        if (mEmotionSprite != nullptr)
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

void Being::updatePercentHP() restrict2
{
    if (mMaxHP == 0)
        return;
    BLOCK_START("Being::updatePercentHP")
    if (mHP != 0)
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

int Being::getSpriteID(const int slot) const restrict2
{
    if (slot < 0 || CAST_SIZE(slot) >= mSlots.size())
        return -1;

    return mSlots[slot].spriteId;
}

const BeingSlot &Being::getSpriteSlot(const int slot) const restrict2
{
    if (slot < 0 || CAST_SIZE(slot) >= mSlots.size())
        return *emptyBeingSlot;

    return mSlots[slot];
}

ItemColor Being::getSpriteColor(const int slot) const restrict2
{
    if (slot < 0 || CAST_SIZE(slot) >= mSlots.size())
        return ItemColor_one;

    return mSlots[slot].colorId;
}

void Being::addAfkEffect() restrict2
{
    addSpecialEffect(mAwayEffect);
}

void Being::removeAfkEffect() restrict2
{
    removeSpecialEffect();
}

void Being::addSpecialEffect(const int effect) restrict2
{
    if ((effectManager != nullptr) &&
        ParticleEngine::enabled &&
        (mSpecialParticle == nullptr) &&
        effect != -1)
    {
        mSpecialParticle = effectManager->triggerReturn(effect, this);
    }
}

void Being::removeSpecialEffect() restrict2
{
    if ((effectManager != nullptr) && (mSpecialParticle != nullptr))
    {
        mChildParticleEffects.removeLocally(mSpecialParticle);
        mSpecialParticle = nullptr;
    }
    delete2(mAnimationEffect);
}

void Being::updateAwayEffect() restrict2
{
    if (mAway)
        addAfkEffect();
    else
        removeAfkEffect();
}

void Being::addEffect(const std::string &restrict name) restrict2
{
    delete mAnimationEffect;
    mAnimationEffect = AnimatedSprite::load(
        paths.getStringValue("sprites") + name);
}

void Being::playSfx(const SoundInfo &sound,
                    Being *const being,
                    const bool main,
                    const int x, const int y) const restrict2
{
    BLOCK_START("Being::playSfx")

    if (being != nullptr)
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

void Being::setLook(const uint16_t look) restrict2
{
    if (mType == ActorType::Player)
        setSubtype(mSubType, look);
}

void Being::setTileCoords(const int x, const int y) restrict2
{
    mX = x;
    mY = y;
    if (mMap != nullptr)
    {
        mPixelOffsetY = 0;
        mFixedOffsetY = mPixelOffsetY;
        mOldHeight = mMap->getHeightOffset(mX, mY);
        mNeedPosUpdate = true;
    }
}

void Being::setMap(Map *restrict const map) restrict2
{
    mCastEndTime = 0;
    delete2(mCastingEffect);
    ActorSprite::setMap(map);
    if (mMap != nullptr)
    {
        mPixelOffsetY = mMap->getHeightOffset(mX, mY);
        mFixedOffsetY = mPixelOffsetY;
        mOldHeight = 0;
        mNeedPosUpdate = true;
    }
}

void Being::removeAllItemsParticles() restrict2
{
    FOR_EACH (SpriteParticleInfoIter, it, mSpriteParticles)
        delete (*it).second;
    mSpriteParticles.clear();
}

void Being::addItemParticles(const int id,
                             const SpriteDisplay &restrict display) restrict2
{
    const SpriteParticleInfoIter it = mSpriteParticles.find(id);
    ParticleInfo *restrict pi = nullptr;
    if (it == mSpriteParticles.end())
    {
        pi = new ParticleInfo;
        mSpriteParticles[id] = pi;
    }
    else
    {
        pi = (*it).second;
    }

    if ((pi == nullptr) || !pi->particles.empty())
        return;

    // setup particle effects
    if (ParticleEngine::enabled &&
        (particleEngine != nullptr))
    {
        FOR_EACH (StringVectCIter, itr, display.particles)
        {
            Particle *const p = particleEngine->addEffect(*itr, 0, 0);
            controlCustomParticle(p);
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

void Being::addItemParticlesCards(const int id,
                                  const SpriteDisplay &restrict display,
                                  const CardsList &cards) restrict2
{
    const SpriteParticleInfoIter it = mSpriteParticles.find(id);
    ParticleInfo *restrict pi = nullptr;
    if (it == mSpriteParticles.end())
    {
        pi = new ParticleInfo;
        mSpriteParticles[id] = pi;
    }
    else
    {
        pi = (*it).second;
    }

    if ((pi == nullptr) || !pi->particles.empty())
        return;

    // setup particle effects
    if (ParticleEngine::enabled &&
        (particleEngine != nullptr))
    {
        FOR_EACH (StringVectCIter, itr, display.particles)
        {
            Particle *const p = particleEngine->addEffect(*itr, 0, 0);
            controlCustomParticle(p);
            pi->files.push_back(*itr);
            pi->particles.push_back(p);
        }
        for (int f = 0; f < maxCards; f ++)
        {
            const int cardId = cards.cards[f];
            if (!Item::isItem(cardId))
                continue;
            const ItemInfo &info = ItemDB::get(cardId);
            const SpriteDisplay &restrict display2 = info.getDisplay();
            FOR_EACH (StringVectCIter, itr, display2.particles)
            {
                Particle *const p = particleEngine->addEffect(*itr, 0, 0);
                controlCustomParticle(p);
                pi->files.push_back(*itr);
                pi->particles.push_back(p);
            }
        }
    }
    else
    {
        FOR_EACH (StringVectCIter, itr, display.particles)
        {
            pi->files.push_back(*itr);
        }
        for (int f = 0; f < maxCards; f ++)
        {
            const int cardId = cards.cards[f];
            if (!Item::isItem(cardId))
                continue;
            const ItemInfo &info = ItemDB::get(cardId);
            const SpriteDisplay &restrict display2 = info.getDisplay();
            FOR_EACH (StringVectCIter, itr, display2.particles)
            {
                pi->files.push_back(*itr);
            }
        }
    }
}

void Being::removeItemParticles(const int id) restrict2
{
    const SpriteParticleInfoIter it = mSpriteParticles.find(id);
    if (it == mSpriteParticles.end())
        return;
    ParticleInfo *restrict const pi = (*it).second;
    if (pi != nullptr)
    {
        FOR_EACH (STD_VECTOR<Particle*>::const_iterator, itp, pi->particles)
            mChildParticleEffects.removeLocally(*itp);
        delete pi;
    }
    mSpriteParticles.erase(it);
}

void Being::recreateItemParticles() restrict2
{
    FOR_EACH (SpriteParticleInfoIter, it, mSpriteParticles)
    {
        ParticleInfo *restrict const pi = (*it).second;
        if ((pi != nullptr) && !pi->files.empty())
        {
            FOR_EACH (STD_VECTOR<Particle*>::const_iterator,
                      itp, pi->particles)
            {
                mChildParticleEffects.removeLocally(*itp);
            }

            FOR_EACH (STD_VECTOR<std::string>::const_iterator, str, pi->files)
            {
                Particle *const p = particleEngine->addEffect(
                    *str, 0, 0);
                controlCustomParticle(p);
                pi->particles.push_back(p);
            }
        }
    }
}

void Being::setTeamId(const uint16_t teamId) restrict2
{
    if (mTeamId != teamId)
    {
        mTeamId = teamId;
        showTeamBadge(mTeamId != 0);
        updateColors();
    }
}

void Being::showTeamBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Team]);
    if (show &&
        mTeamId != 0u &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string name = paths.getStringValue("badges") +
            paths.getStringValue(strprintf("team%dbadge",
            mTeamId));
        if (!name.empty())
            mBadges[BadgeIndex::Team] = AnimatedSprite::load(name);
    }
    updateBadgesCount();
    updateBadgesPosition();
}

void Being::showBadges(const bool show) restrict2
{
    showTeamBadge(show);
    showGuildBadge(show);
    showGmBadge(show);
    showPartyBadge(show);
    showNameBadge(show);
    showShopBadge(show);
    showInactiveBadge(show);
    showAwayBadge(show);
}

void Being::showPartyBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Party]);
    if (show &&
        !mPartyName.empty() &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string badge = BadgesDB::getPartyBadge(mPartyName);
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Party] = AnimatedSprite::load(
                paths.getStringValue("badges") + badge);
        }
    }
    updateBadgesCount();
    updateBadgesPosition();
}


void Being::setPartyName(const std::string &restrict name) restrict2
{
    if (mPartyName != name)
    {
        mPartyName = name;
        showPartyBadge(!mPartyName.empty());
    }
}

void Being::showShopBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Shop]);
    if (show &&
        mShop &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string badge = paths.getStringValue("shopbadge");
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Shop] = AnimatedSprite::load(
                paths.getStringValue("badges") + badge);
        }
    }
    updateBadgesCount();
    updateBadgesPosition();
}

void Being::showInactiveBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Inactive]);
    if (show &&
        mInactive &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string badge = paths.getStringValue("inactivebadge");
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Inactive] = AnimatedSprite::load(
                paths.getStringValue("badges") + badge);
        }
    }
    updateBadgesCount();
    updateBadgesPosition();
}

void Being::showAwayBadge(const bool show) restrict2
{
    delete2(mBadges[BadgeIndex::Away]);
    if (show &&
        mAway &&
        mShowBadges != BadgeDrawType::Hide)
    {
        const std::string badge = paths.getStringValue("awaybadge");
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Away] = AnimatedSprite::load(
                paths.getStringValue("badges") + badge);
        }
    }
    updateBadgesCount();
    updateBadgesPosition();
}

void Being::updateBadgesCount() restrict2
{
    mBadgesCount = 0;
    for_each_badges()
    {
        if (mBadges[f] != nullptr)
            mBadgesCount ++;
    }
}

void Being::setChat(ChatObject *restrict const obj) restrict2
{
    delete mChat;
    mChat = obj;
}

void Being::setSellBoard(const std::string &restrict text) restrict2
{
    mShop = !text.empty() || !mBuyBoard.empty();
    mSellBoard = text;
    updateName();
    showShopBadge(mShop);
}

void Being::setBuyBoard(const std::string &restrict text) restrict2
{
    mShop = !text.empty() || !mSellBoard.empty();
    mBuyBoard = text;
    updateName();
    showShopBadge(mShop);
}

void Being::enableShop(const bool b) restrict2
{
    mShop = b;
    updateName();
    showShopBadge(mShop);
}

bool Being::isBuyShopEnabled() const restrict2
{
    return mShop && (Net::getNetworkType() == ServerType::TMWATHENA ||
        !mBuyBoard.empty());
}

bool Being::isSellShopEnabled() const restrict2
{
    return mShop && (Net::getNetworkType() == ServerType::TMWATHENA ||
        !mSellBoard.empty());
}

void Being::serverRemove() restrict2 noexcept2
{
    // remove some flags what can survive player remove and next visible
    mTrickDead = false;
}

void Being::addCast(const int dstX,
                    const int dstY,
                    const int skillId,
                    const int skillLevel,
                    const int range,
                    const int waitTimeTicks)
{
    if (waitTimeTicks <= 0)
    {
        mCastEndTime = 0;
        return;
    }
    mCastEndTime = tick_time + waitTimeTicks;
    SkillData *const data = skillDialog->getSkillDataByLevel(
        skillId,
        skillLevel);
    delete2(mCastingEffect);
    if (data != nullptr)
    {
        const std::string castingAnimation = data->castingAnimation;
        mCastingEffect = new CastingEffect(skillId,
            skillLevel,
            castingAnimation,
            dstX,
            dstY,
            range);
        mCastingEffect->setMap(mMap);
    }
    else
    {
        reportAlways("Want to draw casting for unknown skill %d",
            skillId);
    }
}

void Being::removeHorse() restrict2
{
    delete_all(mUpHorseSprites);
    mUpHorseSprites.clear();
    delete_all(mDownHorseSprites);
    mDownHorseSprites.clear();
}

void Being::setRiding(const bool b) restrict2
{
    if (serverFeatures->haveExtendedRiding())
        return;

    if (b == (mHorseId != 0))
        return;
    if (b)
        setHorse(1);
    else
        setHorse(0);
}

void Being::setHorse(const int horseId) restrict2
{
    if (mHorseId == horseId)
        return;
    mHorseId = horseId;
    setAction(mAction, 0);
    removeHorse();
    if (mHorseId != 0)
    {
        mHorseInfo = HorseDB::get(horseId);
        if (mHorseInfo != nullptr)
        {
            FOR_EACH (SpriteRefs, it, mHorseInfo->downSprites)
            {
                const SpriteReference *restrict const ref = *it;
                AnimatedSprite *const sprite = AnimatedSprite::load(
                    ref->sprite,
                    ref->variant);
                mDownHorseSprites.push_back(sprite);
                sprite->play(mSpriteAction);
                sprite->setSpriteDirection(mSpriteDirection);
            }
            FOR_EACH (SpriteRefs, it, mHorseInfo->upSprites)
            {
                const SpriteReference *restrict const ref = *it;
                AnimatedSprite *const sprite = AnimatedSprite::load(
                    ref->sprite,
                    ref->variant);
                mUpHorseSprites.push_back(sprite);
                sprite->play(mSpriteAction);
                sprite->setSpriteDirection(mSpriteDirection);
            }
        }
    }
    else
    {
        mHorseInfo = nullptr;
    }
}

void Being::setTrickDead(const bool b) restrict2
{
    if (b != mTrickDead)
    {
        mTrickDead = b;
        setAction(mAction, 0);
    }
}

void Being::setSpiritBalls(const unsigned int balls) restrict2
{
    if (!ParticleEngine::enabled)
    {
        mSpiritBalls = balls;
        return;
    }

    if (balls > mSpiritBalls)
    {
        const int effectId = paths.getIntValue("spiritEffectId");
        if (effectId != -1)
            addSpiritBalls(balls - mSpiritBalls, effectId);
    }
    else if (balls < mSpiritBalls)
    {
        removeSpiritBalls(mSpiritBalls - balls);
    }
    mSpiritBalls = balls;
}

void Being::addSpiritBalls(const unsigned int balls,
                           const int effectId) restrict2
{
    if (effectManager == nullptr)
        return;
    for (unsigned int f = 0; f < balls; f ++)
    {
        Particle *const particle = effectManager->triggerReturn(
            effectId,
            this);
        mSpiritParticles.push_back(particle);
    }
}

void Being::removeSpiritBalls(const unsigned int balls) restrict2
{
    if (particleEngine == nullptr)
        return;
    for (unsigned int f = 0; f < balls && !mSpiritParticles.empty(); f ++)
    {
        const Particle *restrict const particle = mSpiritParticles.back();
        mChildParticleEffects.removeLocally(particle);
        mSpiritParticles.pop_back();
    }
}

void Being::stopCast(const bool b)
{
    if (b && mAction == BeingAction::CAST)
        setAction(BeingAction::STAND, 0);
}

void Being::fixDirectionOffsets(int &offsetX,
                                int &offsetY) const
{
    const uint8_t dir = mDirection;
    if ((dir & BeingDirection::DOWN) != 0)
    {
        // do nothing
    }
    else if ((dir & BeingDirection::UP) != 0)
    {
        offsetX = -offsetX;
        offsetY = -offsetY;
    }
    else if ((dir & BeingDirection::LEFT) != 0)
    {
        const int tmp = offsetY;
        offsetY = offsetX;
        offsetX = -tmp;
    }
    else if ((dir & BeingDirection::RIGHT) != 0)
    {
        const int tmp = offsetY;
        offsetY = -offsetX;
        offsetX = tmp;
    }
}

void Being::setLanguageId(const int lang) restrict2 noexcept2
{
    if (lang != mLanguageId)
    {
        delete2(mBadges[BadgeIndex::Lang]);
        const std::string &badge = LanguageDb::getIcon(lang);
        if (!badge.empty())
        {
            mBadges[BadgeIndex::Lang] = AnimatedSprite::load(pathJoin(
                paths.getStringValue("languageIcons"),
                badge));
        }

        mLanguageId = lang;
    }
    updateBadgesCount();
    updateBadgesPosition();
}

Being *Being::createBeing(const BeingId id,
                          const ActorTypeT type,
                          const BeingTypeId subType,
                          Map *const map)
{
    Being *const being = new Being(id,
        type);
    being->postInit(subType,
        map);
    return being;
}
