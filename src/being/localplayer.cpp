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

#include "being/localplayer.h"

#include "actormanager.h"
#include "animatedsprite.h"
#include "client.h"
#include "configuration.h"
#include "dropshortcut.h"
#include "gamemodifiers.h"
#include "guild.h"
#include "item.h"
#include "party.h"
#include "settings.h"
#include "soundconsts.h"
#include "soundmanager.h"
#include "statuseffect.h"
#include "resources/map/walklayer.h"

#include "being/attributes.h"
#include "being/beingflag.h"
#include "being/pickup.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "particle/particle.h"

#include "input/keyboardconfig.h"

#include "gui/gui.h"
#include "gui/popupmanager.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/gmtab.h"
#include "gui/widgets/tabs/whispertab.h"

#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/packetlimiter.h"
#include "net/pethandler.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "resources/iteminfo.h"
#include "resources/itemslot.h"
#include "resources/mapitemtype.h"

#include "resources/db/emotedb.h"
#include "resources/db/weaponsdb.h"

#include "resources/map/map.h"
#include "resources/map/mapitem.h"
#include "resources/map/speciallayer.h"

#include "listeners/awaylistener.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/timer.h"

#ifdef USE_MUMBLE
#include "mumblemanager.h"
#endif

#include <climits>

#include "debug.h"

static const int16_t awayLimitTimer = 60;
static const int MAX_TICK_VALUE = INT_MAX / 2;

typedef std::map<int, Guild*>::const_iterator GuildMapCIter;

LocalPlayer *localPlayer = nullptr;

class SkillDialog;

extern std::list<BeingCacheEntry*> beingInfoCache;
extern OkDialog *weightNotice;
extern int weightNoticeTime;
extern MiniStatusWindow *miniStatusWindow;
extern SkillDialog *skillDialog;

LocalPlayer::LocalPlayer(const int id, const uint16_t subtype) :
    Being(id, ActorType::Player, subtype, nullptr),
    AttributeListener(),
    StatListener(),
    mGMLevel(0),
    mCrazyMoveState(0),
    mMoveState(0),
    mLastTargetX(0),
    mLastTargetY(0),
    mHomes(),
    mTarget(nullptr),
    mPlayerFollowed(),
    mPlayerImitated(),
    mNextDestX(0),
    mNextDestY(0),
    mPickUpTarget(nullptr),
    mLastAction(-1),
    mStatusEffectIcons(),
    mMessages(),
    mMessageTime(0),
    mAwayListener(new AwayListener),
    mAwayDialog(nullptr),
    mPingSendTick(0),
    mPingTime(0),
    mAfkTime(0),
    mActivityTime(0),
    mNavigateX(0),
    mNavigateY(0),
    mNavigateId(0),
    mCrossX(0),
    mCrossY(0),
    mOldX(0),
    mOldY(0),
    mOldTileX(0),
    mOldTileY(0),
    mNavigatePath(),
    mLastHitFrom(),
    mWaitFor(),
    mAdvertTime(0),
    mTestParticle(nullptr),
    mTestParticleName(),
    mTestParticleTime(0),
    mTestParticleHash(0L),
    mWalkingDir(0),
    mUpdateName(true),
    mBlockAdvert(false),
    mTargetDeadPlayers(config.getBoolValue("targetDeadPlayers")),
    mServerAttack(config.getBoolValue("serverAttack")),
    mEnableAdvert(config.getBoolValue("enableAdvert")),
    mTradebot(config.getBoolValue("tradebot")),
    mTargetOnlyReachable(config.getBoolValue("targetOnlyReachable")),
    mIsServerBuggy(serverConfig.getValueBool("enableBuggyServers", true)),
    mSyncPlayerMove(config.getBoolValue("syncPlayerMove")),
    mDrawPath(config.getBoolValue("drawPath")),
    mAttackMoving(config.getBoolValue("attackMoving")),
    mAttackNext(config.getBoolValue("attackNext")),
    mShowJobExp(config.getBoolValue("showJobExp")),
    mShowServerPos(config.getBoolValue("showserverpos")),
    mNextStep(false),
    mDisableCrazyMove(false),
    mGoingToTarget(false),
    mKeepAttacking(false),
    mPathSetByMouse(false),
    mWaitPing(false),
    mShowNavigePath(false)
{
    logger->log1("LocalPlayer::LocalPlayer");

    mAttackRange = 0;
    mLevel = 1;
    mAdvanced = true;
    mTextColor = &theme->getColor(Theme::PLAYER, 255);
    if (userPalette)
        mNameColor = &userPalette->getColor(UserPalette::SELF);
    else
        mNameColor = nullptr;

    PlayerInfo::setStatBase(Attributes::WALK_SPEED,
        static_cast<int>(getWalkSpeed().x));
    PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);

    loadHomes();

    config.addListener("showownname", this);
    config.addListener("targetDeadPlayers", this);
    serverConfig.addListener("enableBuggyServers", this);
    config.addListener("syncPlayerMove", this);
    config.addListener("drawPath", this);
    config.addListener("serverAttack", this);
    config.addListener("attackMoving", this);
    config.addListener("attackNext", this);
    config.addListener("showJobExp", this);
    config.addListener("enableAdvert", this);
    config.addListener("tradebot", this);
    config.addListener("targetOnlyReachable", this);
    config.addListener("showserverpos", this);
    setShowName(config.getBoolValue("showownname"));
}

LocalPlayer::~LocalPlayer()
{
    logger->log1("LocalPlayer::~LocalPlayer");

    config.removeListeners(this);
    serverConfig.removeListener("enableBuggyServers", this);

    navigateClean();
    mCrossX = 0;
    mCrossY = 0;

    updateNavigateList();

    if (mAwayDialog)
    {
        soundManager.volumeRestore();
        delete2(mAwayDialog)
    }
    delete2(mAwayListener);
}

void LocalPlayer::logic()
{
    BLOCK_START("LocalPlayer::logic")
#ifdef USE_MUMBLE
    if (mumbleManager)
        mumbleManager->setPos(mX, mY, mDirection);
#endif

    // Actions are allowed once per second
    if (get_elapsed_time(mLastAction) >= 1000)
        mLastAction = -1;

    if (mActivityTime == 0 || mLastAction != -1)
        mActivityTime = cur_time;

    if ((mAction != BeingAction::MOVE || mNextStep) && !mNavigatePath.empty())
    {
        mNextStep = false;
        int dist = 5;
        if (!mSyncPlayerMove)
            dist = 20;

        if ((mNavigateX || mNavigateY) &&
            ((mCrossX + dist >= mX && mCrossX <= mX + dist
            && mCrossY + dist >= mY && mCrossY <= mY + dist)
            || (!mCrossX && !mCrossY)))
        {
            const Path::const_iterator i = mNavigatePath.begin();
            if ((*i).x == mX && (*i).y == mY)
                mNavigatePath.pop_front();
            else
                setDestination((*i).x, (*i).y);
        }
    }

    // Show XP messages
    if (!mMessages.empty())
    {
        if (mMessageTime == 0)
        {
            MessagePair info = mMessages.front();

            if (particleEngine && gui)
            {
                particleEngine->addTextRiseFadeOutEffect(
                    info.first,
                    getPixelX(),
                    getPixelY() - 48,
                    &userPalette->getColor(info.second),
                    gui->getInfoParticleFont(), true);
            }

            mMessages.pop_front();
            mMessageTime = 30;
        }
        mMessageTime--;
    }

    if (mTarget)
    {
        if (mTarget->getType() == ActorType::Npc)
        {
            // NPCs are always in range
            mTarget->setTargetType(TargetCursorType::IN_RANGE);
        }
        else
        {
            // Find whether target is in range
            const int rangeX = static_cast<int>(
                abs(mTarget->getTileX() - getTileX()));
            const int rangeY = static_cast<int>(
                abs(mTarget->getTileY() - getTileY()));
            const int attackRange = getAttackRange();
            const TargetCursorType::Type targetType
                = rangeX > attackRange || rangeY > attackRange
                ? TargetCursorType::NORMAL : TargetCursorType::IN_RANGE;
            mTarget->setTargetType(targetType);

            if (!mTarget->isAlive() && (!mTargetDeadPlayers
                || mTarget->getType() != ActorType::Player))
            {
                stopAttack(true);
            }

            if (mKeepAttacking && mTarget)
                attack(mTarget, true);
        }
    }

    Being::logic();
    BLOCK_END("LocalPlayer::logic")
}

void LocalPlayer::slowLogic()
{
    BLOCK_START("LocalPlayer::slowLogic")
    const int time = cur_time;
    if (weightNotice && weightNoticeTime < time)
    {
        weightNotice->scheduleDelete();
        weightNotice = nullptr;
        weightNoticeTime = 0;
    }

    if (!Net::getServerFeatures()->havePlayerStatusUpdate()
        && mEnableAdvert
        && !mBlockAdvert
        && mAdvertTime < cur_time)
    {
        uint8_t smile = BeingFlag::SPECIAL;
        if (mTradebot && shopWindow && !shopWindow->isShopEmpty())
            smile |= BeingFlag::SHOP;

        if (settings.awayMode || settings.pseudoAwayMode)
            smile |= BeingFlag::AWAY;

        if (mInactive)
            smile |= BeingFlag::INACTIVE;

        if (emote(smile))
            mAdvertTime = time + 60;
        else
            mAdvertTime = time + 30;
    }

    if (mTestParticleTime != time && !mTestParticleName.empty())
    {
        const unsigned long hash = UpdaterWindow::getFileHash(
            mTestParticleName);
        if (hash != mTestParticleHash)
        {
            setTestParticle(mTestParticleName, false);
            mTestParticleHash = hash;
        }
        mTestParticleTime = time;
    }

    BLOCK_END("LocalPlayer::slowLogic")
}

void LocalPlayer::setAction(const BeingAction::Action &action,
                            const int attackType)
{
    if (action == BeingAction::DEAD)
    {
        if (!mLastHitFrom.empty())
        {
            // TRANSLATORS: chat message after death
            debugMsg(strprintf(_("You were killed by %s"),
                mLastHitFrom.c_str()));
            mLastHitFrom.clear();
        }
        setTarget(nullptr);
    }

    Being::setAction(action, attackType);
#ifdef USE_MUMBLE
    if (mumbleManager)
        mumbleManager->setAction(static_cast<int>(action));
#endif
}

void LocalPlayer::setGMLevel(const int level)
{
    mGMLevel = level;

    if (level > 0)
    {
        setGM(true);
        if (chatWindow)
        {
            chatWindow->loadGMCommands();
            if (!gmChatTab && config.getBoolValue("enableGmTab"))
                gmChatTab = new GmTab(chatWindow);
        }
    }
}

void LocalPlayer::nextTile(unsigned char dir A_UNUSED = 0)
{
    const Party *const party = Party::getParty(1);
    if (party)
    {
        PartyMember *const pm = party->getMember(getName());
        if (pm)
        {
            pm->setX(mX);
            pm->setY(mY);
        }
    }

    if (mPath.empty())
    {
        if (mPickUpTarget)
            pickUp(mPickUpTarget);

        if (mWalkingDir)
            startWalking(mWalkingDir);
    }
    else if (mPath.size() == 1)
    {
        if (mPickUpTarget)
            pickUp(mPickUpTarget);
    }

    if (mGoingToTarget && mTarget && withinAttackRange(mTarget))
    {
        mAction = BeingAction::STAND;
        attack(mTarget, true);
        mGoingToTarget = false;
        mPath.clear();
        return;
    }
    else if (mGoingToTarget && !mTarget)
    {
        mGoingToTarget = false;
        mPath.clear();
    }

    if (mPath.empty())
    {
        if (mNavigatePath.empty() || mAction != BeingAction::MOVE)
            setAction(BeingAction::STAND);
        else
            mNextStep = true;
    }
    else
    {
        Being::nextTile();
    }
}

bool LocalPlayer::pickUp(FloorItem *const item)
{
    if (!item)
        return false;

    if (!PacketLimiter::limitPackets(PACKET_PICKUP))
        return false;

    const int dx = item->getTileX() - mX;
    const int dy = item->getTileY() - mY;
    int dist = 6;

    const unsigned int pickUpType = settings.pickUpType;
    if (pickUpType >= 4 && pickUpType <= 6)
        dist = 4;

    if (dx * dx + dy * dy < dist)
    {
        if (actorManager && actorManager->checkForPickup(item))
        {
            PlayerInfo::pickUpItem(item, true);
            mPickUpTarget = nullptr;
        }
    }
    else if (pickUpType >= 4 && pickUpType <= 6)
    {
        const Vector &playerPos = getPosition();
        const Path debugPath = mMap->findPath(
            static_cast<int>(playerPos.x - mapTileSize / 2) / mapTileSize,
            static_cast<int>(playerPos.y - mapTileSize) / mapTileSize,
            item->getTileX(), item->getTileY(), getBlockWalkMask(), 0);
        if (!debugPath.empty())
            navigateTo(item->getTileX(), item->getTileY());
        else
            setDestination(item->getTileX(), item->getTileY());

        mPickUpTarget = item;
        mPickUpTarget->addActorSpriteListener(this);
    }
    return true;
}

void LocalPlayer::actorSpriteDestroyed(const ActorSprite &actorSprite)
{
    if (mPickUpTarget == &actorSprite)
        mPickUpTarget = nullptr;
}

Being *LocalPlayer::getTarget() const
{
    return mTarget;
}

void LocalPlayer::setTarget(Being *const target)
{
    if (target == this && target)
        return;

    if (target == mTarget)
        return;

    Being *oldTarget = nullptr;
    if (mTarget)
    {
        mTarget->untarget();
        oldTarget = mTarget;
    }

    if (mTarget && mTarget->getType() == ActorType::Monster)
        mTarget->setShowName(false);

    mTarget = target;

    if (oldTarget)
        oldTarget->updateName();

    if (mTarget)
    {
        mLastTargetX = mTarget->getTileX();
        mLastTargetY = mTarget->getTileY();
        mTarget->updateName();
    }

    if (target && target->getType() == ActorType::Monster)
        target->setShowName(true);
}

void LocalPlayer::setDestination(const int x, const int y)
{
    mActivityTime = cur_time;

    if (settings.attackType == 0 || !mAttackMoving)
        mKeepAttacking = false;

    // Only send a new message to the server when destination changes
    if (x != mDest.x || y != mDest.y)
    {
        if (settings.moveType != 1)
        {
            Net::getPlayerHandler()->setDestination(x, y, mDirection);
            Being::setDestination(x, y);
        }
        else
        {
            uint8_t newDir = 0;
            if (mDirection & BeingDirection::UP)
                newDir |= BeingDirection::DOWN;
            if (mDirection & BeingDirection::LEFT)
                newDir |= BeingDirection::RIGHT;
            if (mDirection & BeingDirection::DOWN)
                newDir |= BeingDirection::UP;
            if (mDirection & BeingDirection::RIGHT)
                newDir |= BeingDirection::LEFT;

            Net::getPlayerHandler()->setDestination(x, y, newDir);

//            if (PacketLimiter::limitPackets(PACKET_DIRECTION))
            {
                setDirection(newDir);
                Net::getPlayerHandler()->setDirection(newDir);
            }

            Being::setDestination(x, y);
            Net::getPlayerHandler()->setDestination(x, y, mDirection);
        }
    }
}

void LocalPlayer::setWalkingDir(const unsigned char dir)
{
    // This function is called by Game::handleInput()
    mWalkingDir = dir;

    // If we're not already walking, start walking.
    if (mAction != BeingAction::MOVE && dir)
        startWalking(dir);
}

void LocalPlayer::startWalking(const unsigned char dir)
{
    // This function is called by setWalkingDir(),
    // but also by nextTile() for TMW-Athena...
    if (!mMap || !dir)
        return;

    mPickUpTarget = nullptr;
    if (mAction == BeingAction::MOVE && !mPath.empty())
    {
        // Just finish the current action, otherwise we get out of sync
        Being::setDestination(mX, mY);
        return;
    }

    int dx = 0, dy = 0;
    if (dir & BeingDirection::UP)
        dy--;
    if (dir & BeingDirection::DOWN)
        dy++;
    if (dir & BeingDirection::LEFT)
        dx--;
    if (dir & BeingDirection::RIGHT)
        dx++;

    const unsigned char blockWalkMask = getBlockWalkMask();
    // Prevent skipping corners over colliding tiles
    if (dx && !mMap->getWalk(mX + dx, mY, blockWalkMask))
        dx = 0;
    if (dy && !mMap->getWalk(mX, mY + dy, blockWalkMask))
        dy = 0;

    // Choose a straight direction when diagonal target is blocked
    if (dx && dy && !mMap->getWalk(mX + dx, mY + dy, blockWalkMask))
        dx = 0;

    // Walk to where the player can actually go
    if ((dx || dy) && mMap->getWalk(mX + dx, mY + dy, blockWalkMask))
    {
        setDestination(mX + dx, mY + dy);
    }
    else if (dir != mDirection)
    {
        // If the being can't move, just change direction

//            if (PacketLimiter::limitPackets(PACKET_DIRECTION))
        {
            Net::getPlayerHandler()->setDirection(dir);
            setDirection(dir);
        }
    }
}

void LocalPlayer::stopWalking(const bool sendToServer)
{
    if (mAction == BeingAction::MOVE && mWalkingDir)
    {
        mWalkingDir = 0;
        mPickUpTarget = nullptr;

        setDestination(static_cast<int>(getPosition().x),
                       static_cast<int>(getPosition().y));
        if (sendToServer)
        {
            Net::getPlayerHandler()->setDestination(
                    static_cast<int>(getPosition().x),
                    static_cast<int>(getPosition().y), -1);
        }
        setAction(BeingAction::STAND);
    }

    // No path set anymore, so we reset the path by mouse flag
    mPathSetByMouse = false;

    clearPath();
    navigateClean();
}

bool LocalPlayer::toggleSit() const
{
    if (!PacketLimiter::limitPackets(PACKET_SIT))
        return false;

    BeingAction::Action newAction;
    switch (mAction)
    {
        case BeingAction::STAND:
        case BeingAction::SPAWN:
            newAction = BeingAction::SIT;
            break;
        case BeingAction::SIT:
            newAction = BeingAction::STAND;
            break;
        case BeingAction::MOVE:
        case BeingAction::ATTACK:
        case BeingAction::DEAD:
        case BeingAction::HURT:
        default:
            return true;
    }

    Net::getPlayerHandler()->changeAction(newAction);
    return true;
}

bool LocalPlayer::updateSit() const
{
    if (!PacketLimiter::limitPackets(PACKET_SIT))
        return false;

    Net::getPlayerHandler()->changeAction(mAction);
    return true;
}

bool LocalPlayer::emote(const uint8_t emotion)
{
    if (!PacketLimiter::limitPackets(PACKET_EMOTE))
        return false;

    Net::getPlayerHandler()->emote(emotion);
    return true;
}

void LocalPlayer::attack(Being *const target, const bool keep,
                         const bool dontChangeEquipment)
{
    mKeepAttacking = keep;

    if (!target || target->getType() == ActorType::Npc)
        return;

    if (mTarget != target)
        setTarget(target);

    const int dist_x = target->getTileX() - mX;
    const int dist_y = target->getTileY() - mY;

    // Must be standing or sitting to attack
    if (mAction != BeingAction::STAND && mAction != BeingAction::SIT)
        return;

    if (abs(dist_y) >= abs(dist_x))
    {
        if (dist_y > 0)
            setDirection(BeingDirection::DOWN);
        else
            setDirection(BeingDirection::UP);
    }
    else
    {
        if (dist_x > 0)
            setDirection(BeingDirection::RIGHT);
        else
            setDirection(BeingDirection::LEFT);
    }

    mActionTime = tick_time;

    if (target->getType() != ActorType::Player
        || checAttackPermissions(target))
    {
        setAction(BeingAction::ATTACK);

        if (!PacketLimiter::limitPackets(PACKET_ATTACK))
            return;

        if (!dontChangeEquipment)
            changeEquipmentBeforeAttack(target);

        Net::getPlayerHandler()->attack(target->getId(), mServerAttack);
    }

    if (!keep)
        stopAttack();
}

void LocalPlayer::stopAttack(const bool keepAttack)
{
    if (!PacketLimiter::limitPackets(PACKET_STOPATTACK))
        return;

    if (mServerAttack && mAction == BeingAction::ATTACK)
        Net::getPlayerHandler()->stopAttack();

    untarget();
    if (!keepAttack || !mAttackNext)
        mKeepAttacking = false;
}

void LocalPlayer::untarget()
{
    if (mAction == BeingAction::ATTACK)
        setAction(BeingAction::STAND);

    if (mTarget)
        setTarget(nullptr);
}

void LocalPlayer::pickedUp(const ItemInfo &itemInfo, const int amount,
                           const unsigned char color, const int floorItemId,
                           const unsigned char fail)
{
    if (fail)
    {
        if (actorManager && floorItemId)
        {
            FloorItem *const item = actorManager->findItem(floorItemId);
            if (item)
            {
                if (!item->getShowMsg())
                    return;
                item->setShowMsg(false);
            }
        }
        const char* msg = nullptr;
        switch (fail)
        {
            case Pickup::BAD_ITEM:
                // TRANSLATORS: pickup error message
                msg = N_("Tried to pick up nonexistent item.");
                break;
            case Pickup::TOO_HEAVY:
                // TRANSLATORS: pickup error message
                msg = N_("Item is too heavy.");
                break;
            case Pickup::TOO_FAR:
                // TRANSLATORS: pickup error message
                msg = N_("Item is too far away.");
                break;
            case Pickup::INV_FULL:
                // TRANSLATORS: pickup error message
                msg = N_("Inventory is full.");
                break;
            case Pickup::STACK_FULL:
                // TRANSLATORS: pickup error message
                msg = N_("Stack is too big.");
                break;
            case Pickup::DROP_STEAL:
                // TRANSLATORS: pickup error message
                msg = N_("Item belongs to someone else.");
                break;
            default:
                // TRANSLATORS: pickup error message
                msg = N_("Unknown problem picking up item.");
                break;
        }
        if (localChatTab && config.getBoolValue("showpickupchat"))
            localChatTab->chatLog(gettext(msg), ChatMsgType::BY_SERVER);

        if (mMap && config.getBoolValue("showpickupparticle"))
        {
            // Show pickup notification
            addMessageToQueue(gettext(msg), UserPalette::PICKUP_INFO);
        }
    }
    else
    {
        std::string str;
        if (Net::getServerFeatures()->haveItemColors())
            str = itemInfo.getName(color);
        else
            str = itemInfo.getName();

        if (config.getBoolValue("showpickupchat") && localChatTab)
        {
            // TRANSLATORS: %d is number,
            // [@@%d|%s@@] - here player can see link to item
            localChatTab->chatLog(strprintf(ngettext("You picked up %d "
                "[@@%d|%s@@].", "You picked up %d [@@%d|%s@@].", amount),
                amount, itemInfo.getId(), str.c_str()),
                ChatMsgType::BY_SERVER);
        }

        if (mMap && config.getBoolValue("showpickupparticle"))
        {
            // Show pickup notification
            if (amount > 1)
            {
                addMessageToQueue(strprintf("%d x %s", amount,
                    str.c_str()), UserPalette::PICKUP_INFO);
            }
            else
            {
                addMessageToQueue(str, UserPalette::PICKUP_INFO);
            }
        }
    }
}

int LocalPlayer::getAttackRange() const
{
    if (mAttackRange > -1)
    {
        return mAttackRange;
    }
    else
    {
        const Item *const weapon = PlayerInfo::getEquipment(
            ItemSlot::FIGHT1_SLOT);
        if (weapon)
        {
            const ItemInfo &info = weapon->getInfo();
            return info.getAttackRange();
        }
        return 48;  // unarmed range
    }
}

bool LocalPlayer::withinAttackRange(const Being *const target,
                                    const bool fixDistance,
                                    const int addRange) const
{
    if (!target)
        return false;

    int range = getAttackRange() + addRange;
    int dx;
    int dy;

    if (fixDistance && range == 1)
        range = 2;

    dx = static_cast<int>(abs(target->getTileX() - mX));
    dy = static_cast<int>(abs(target->getTileY() - mY));
    return !(dx > range || dy > range);
}

void LocalPlayer::setGotoTarget(Being *const target)
{
    if (!target)
        return;

    mPickUpTarget = nullptr;
    setTarget(target);
    mGoingToTarget = true;
    setDestination(target->getTileX(), target->getTileY());
}

void LocalPlayer::handleStatusEffect(const StatusEffect *const effect,
                                     const int effectId)
{
    Being::handleStatusEffect(effect, effectId);

    if (effect)
    {
        effect->deliverMessage();
        effect->playSFX();

        AnimatedSprite *const sprite = effect->getIcon();

        if (!sprite)
        {
            // delete sprite, if necessary
            for (size_t i = 0; i < mStatusEffectIcons.size(); )
            {
                if (mStatusEffectIcons[i] == effectId)
                {
                    mStatusEffectIcons.erase(mStatusEffectIcons.begin() + i);
                    if (miniStatusWindow)
                        miniStatusWindow->eraseIcon(static_cast<int>(i));
                }
                else
                {
                    i++;
                }
            }
        }
        else
        {
            // replace sprite or append
            bool found = false;
            const size_t sz = mStatusEffectIcons.size();
            for (size_t i = 0; i < sz; i++)
            {
                if (mStatusEffectIcons[i] == effectId)
                {
                    if (miniStatusWindow)
                        miniStatusWindow->setIcon(static_cast<int>(i), sprite);
                    found = true;
                    break;
                }
            }

            if (!found)
            { // add new
                const int offset = static_cast<int>(mStatusEffectIcons.size());
                if (miniStatusWindow)
                    miniStatusWindow->setIcon(offset, sprite);
                mStatusEffectIcons.push_back(effectId);
            }
        }
    }
}

void LocalPlayer::addMessageToQueue(const std::string &message,
                                    const int color)
{
    if (mMessages.size() < 20)
        mMessages.push_back(MessagePair(message, color));
}

void LocalPlayer::optionChanged(const std::string &value)
{
    if (value == "showownname")
        setShowName(config.getBoolValue("showownname"));
    else if (value == "targetDeadPlayers")
        mTargetDeadPlayers = config.getBoolValue("targetDeadPlayers");
    else if (value == "enableBuggyServers")
        mIsServerBuggy = serverConfig.getBoolValue("enableBuggyServers");
    else if (value == "syncPlayerMove")
        mSyncPlayerMove = config.getBoolValue("syncPlayerMove");
    else if (value == "drawPath")
        mDrawPath = config.getBoolValue("drawPath");
    else if (value == "serverAttack")
        mServerAttack = config.getBoolValue("serverAttack");
    else if (value == "attackMoving")
        mAttackMoving = config.getBoolValue("attackMoving");
    else if (value == "attackNext")
        mAttackNext = config.getBoolValue("attackNext");
    else if (value == "showJobExp")
        mShowJobExp = config.getBoolValue("showJobExp");
    else if (value == "enableAdvert")
        mEnableAdvert = config.getBoolValue("enableAdvert");
    else if (value == "tradebot")
        mTradebot = config.getBoolValue("tradebot");
    else if (value == "targetOnlyReachable")
        mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
    else if (value == "showserverpos")
        mShowServerPos = config.getBoolValue("showserverpos");
}

void LocalPlayer::statChanged(const int id,
                              const int oldVal1,
                              const int oldVal2)
{
    if (!mShowJobExp || id != Net::getPlayerHandler()->getJobLocation())
        return;

    const std::pair<int, int> exp = PlayerInfo::getStatExperience(id);
    if (oldVal1 > exp.first || !oldVal2)
        return;

    const int change = exp.first - oldVal1;
    if (change != 0 && mMessages.size() < 20)
    {
        if (!mMessages.empty())
        {
            MessagePair pair = mMessages.back();
            // TRANSLATORS: this is normal experience
            if (pair.first.find(strprintf(" %s", _("xp")))
                == pair.first.size() - strlen(_("xp")) - 1)
            {
                mMessages.pop_back();
                // TRANSLATORS: this is job experience
                pair.first.append(strprintf(", %d %s", change, _("job")));
                mMessages.push_back(pair);
            }
            else
            {
                // TRANSLATORS: this is job experience
                addMessageToQueue(strprintf("%d %s", change, _("job")));
            }
        }
        else
        {
            // TRANSLATORS: this is job experience
            addMessageToQueue(strprintf("%d %s", change, _("job")));
        }
    }
}

void LocalPlayer::attributeChanged(const int id,
                                   const int oldVal,
                                   const int newVal)
{
    switch (id)
    {
        case Attributes::EXP:
        {
            if (oldVal > newVal)
                break;

            const int change = newVal - oldVal;
            if (change != 0)
            {
                // TRANSLATORS: get xp message
                addMessageToQueue(strprintf("%d %s", change, _("xp")));
            }
            break;
        }
        case Attributes::LEVEL:
            mLevel = newVal;
            break;
        default:
            break;
    }
}

void LocalPlayer::move(const int dX, const int dY)
{
    mPickUpTarget = nullptr;
    setDestination(mX + dX, mY + dY);
}

void LocalPlayer::moveToTarget(int dist)
{
    bool gotPos(false);
    Path debugPath;

    const Vector &playerPos = getPosition();
    unsigned int limit(0);

    if (dist == -1)
    {
        dist = settings.moveToTargetType;
        if (dist != 0)
        {
            const bool broken = Net::getServerFeatures()
                ->haveBrokenPlayerAttackDistance();
            switch (dist)
            {
                case 10:
                    dist = mAttackRange;
                    if (dist == 1 && broken)
                        dist = 2;
                    break;
                case 11:
                    dist = mAttackRange - 1;
                    if (dist < 1)
                        dist = 1;
                    if (dist == 1 && broken)
                        dist = 2;
                    break;
                default:
                    break;
            }
        }
    }

    if (mTarget)
    {
        if (mMap)
        {
            debugPath = mMap->findPath(static_cast<int>(
                playerPos.x - mapTileSize / 2) / mapTileSize,
                static_cast<int>(playerPos.y - mapTileSize) / mapTileSize,
                mTarget->getTileX(), mTarget->getTileY(), getBlockWalkMask(), 0);
        }

        const size_t sz = debugPath.size();
        if (sz < static_cast<size_t>(dist))
            return;
        limit = static_cast<int>(sz) - dist;
        gotPos = true;
    }
    else if (mNavigateX || mNavigateY)
    {
        debugPath = mNavigatePath;
        limit = dist;
        gotPos = true;
    }

    if (gotPos)
    {
        if (dist == 0)
        {
            if (mTarget)
                navigateTo(mTarget->getTileX(), mTarget->getTileY());
        }
        else
        {
            Position pos(0, 0);
            unsigned int f = 0;

            for (Path::const_iterator i = debugPath.begin(),
                 i_end = debugPath.end();
                 i != i_end && f < limit; ++i, f++)
            {
                pos = (*i);
            }
            navigateTo(pos.x, pos.y);
        }
    }
    else if (mLastTargetX || mLastTargetY)
    {
        navigateTo(mLastTargetX, mLastTargetY);
    }
}

void LocalPlayer::moveToHome()
{
    mPickUpTarget = nullptr;
    if ((mX != mCrossX || mY != mCrossY) && mCrossX && mCrossY)
    {
        setDestination(mCrossX, mCrossY);
    }
    else if (mMap)
    {
        const std::map<std::string, Vector>::const_iterator iter =
            mHomes.find(mMap->getProperty("_realfilename"));

        if (iter != mHomes.end())
        {
            const Vector pos = mHomes[(*iter).first];
            if (mX == pos.x && mY == pos.y)
            {
                Net::getPlayerHandler()->setDestination(
                        static_cast<int>(pos.x),
                        static_cast<int>(pos.y),
                        static_cast<int>(mDirection));
            }
            else
            {
                navigateTo(static_cast<int>(pos.x), static_cast<int>(pos.y));
            }
        }
    }
}

void LocalPlayer::changeEquipmentBeforeAttack(const Being *const target) const
{
    if (settings.attackWeaponType == 1
        || !target
        || !PlayerInfo::getInventory())
    {
        return;
    }

    bool allowSword = false;
    const int dx = target->getTileX() - mX;
    const int dy = target->getTileY() - mY;
    const Item *item = nullptr;

    if (dx * dx + dy * dy > 80)
        return;

    if (dx * dx + dy * dy < 8)
        allowSword = true;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    // if attack distance for sword
    if (allowSword)
    {
        // searching swords
        const WeaponsInfos &swords = WeaponsDB::getSwords();
        FOR_EACH (WeaponsInfosIter, it, swords)
        {
            item = inv->findItem(*it, 0);
            if (item)
                break;
        }

        // no swords
        if (!item)
            return;

        // if sword not equiped
        if (!item->isEquipped())
            PlayerInfo::equipItem(item, true);

        // if need equip shield too
        if (settings.attackWeaponType == 3)
        {
            // searching shield
            const WeaponsInfos &shields = WeaponsDB::getShields();
            FOR_EACH (WeaponsInfosIter, it, shields)
            {
                item = inv->findItem(*it, 0);
                if (item)
                    break;
            }
            if (item && !item->isEquipped())
                PlayerInfo::equipItem(item, true);
        }
    }
    // big distance. allowed only bow
    else
    {
        // searching bow
        const WeaponsInfos &bows = WeaponsDB::getBows();
        FOR_EACH (WeaponsInfosIter, it, bows)
        {
            item = inv->findItem(*it, 0);
            if (item)
                break;
        }

        // no bow
        if (!item)
            return;

        if (!item->isEquipped())
            PlayerInfo::equipItem(item, true);
    }
}

void LocalPlayer::crazyMove()
{
    const bool oldDisableCrazyMove = mDisableCrazyMove;
    mDisableCrazyMove = true;
    switch (settings.crazyMoveType)
    {
        case 1:
            crazyMove1();
            break;
        case 2:
            crazyMove2();
            break;
        case 3:
            crazyMove3();
            break;
        case 4:
            crazyMove4();
            break;
        case 5:
            crazyMove5();
            break;
        case 6:
            crazyMove6();
            break;
        case 7:
            crazyMove7();
            break;
        case 8:
            crazyMove8();
            break;
        case 9:
            crazyMove9();
            break;
        case 10:
            crazyMoveA();
            break;
        default:
            break;
    }
    mDisableCrazyMove = oldDisableCrazyMove;
}

void LocalPlayer::crazyMove1()
{
    if (mAction == BeingAction::MOVE)
        return;

//    if (!PacketLimiter::limitPackets(PACKET_DIRECTION))
//        return;

    if (mDirection == BeingDirection::UP)
    {
        setWalkingDir(BeingDirection::UP);
        setDirection(BeingDirection::LEFT);
        Net::getPlayerHandler()->setDirection(BeingDirection::LEFT);
    }
    else if (mDirection == BeingDirection::LEFT)
    {
        setWalkingDir(BeingDirection::LEFT);
        setDirection(BeingDirection::DOWN);
        Net::getPlayerHandler()->setDirection(BeingDirection::DOWN);
    }
    else if (mDirection == BeingDirection::DOWN)
    {
        setWalkingDir(BeingDirection::DOWN);
        setDirection(BeingDirection::RIGHT);
        Net::getPlayerHandler()->setDirection(BeingDirection::RIGHT);
    }
    else if (mDirection == BeingDirection::RIGHT)
    {
        setWalkingDir(BeingDirection::RIGHT);
        setDirection(BeingDirection::UP);
        Net::getPlayerHandler()->setDirection(BeingDirection::UP);
    }
}

void LocalPlayer::crazyMove2()
{
    if (mAction == BeingAction::MOVE)
        return;

//    if (!PacketLimiter::limitPackets(PACKET_DIRECTION))
//        return;

    if (mDirection == BeingDirection::UP)
    {
        setWalkingDir(BeingDirection::UP | BeingDirection::LEFT);
        setDirection(BeingDirection::RIGHT);
        Net::getPlayerHandler()->setDirection(
            BeingDirection::DOWN | BeingDirection::RIGHT);
    }
    else if (mDirection == BeingDirection::RIGHT)
    {
        setWalkingDir(BeingDirection::UP | BeingDirection::RIGHT);
        setDirection(BeingDirection::DOWN);
        Net::getPlayerHandler()->setDirection(
            BeingDirection::DOWN | BeingDirection::LEFT);
    }
    else if (mDirection == BeingDirection::DOWN)
    {
        setWalkingDir(BeingDirection::DOWN | BeingDirection::RIGHT);
        setDirection(BeingDirection::LEFT);
        Net::getPlayerHandler()->setDirection(
            BeingDirection::UP | BeingDirection::LEFT);
    }
    else if (mDirection == BeingDirection::LEFT)
    {
        setWalkingDir(BeingDirection::DOWN | BeingDirection::LEFT);
        setDirection(BeingDirection::UP);
        Net::getPlayerHandler()->setDirection(
            BeingDirection::UP | BeingDirection::RIGHT);
    }
}

void LocalPlayer::crazyMove3()
{
    if (mAction == BeingAction::MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            move(1, 1);
            mCrazyMoveState = 1;
            break;
        case 1:
            move(1, -1);
            mCrazyMoveState = 2;
            break;
        case 2:
            move(-1, -1);
            mCrazyMoveState = 3;
            break;
        case 3:
            move(-1, 1);
            mCrazyMoveState = 0;
            break;
        default:
            break;
    }

//    if (!PacketLimiter::limitPackets(PACKET_DIRECTION))
//        return;

    setDirection(BeingDirection::DOWN);
    Net::getPlayerHandler()->setDirection(BeingDirection::DOWN);
}

void LocalPlayer::crazyMove4()
{
    if (mAction == BeingAction::MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            move(7, 0);
            mCrazyMoveState = 1;
        break;
        case 1:
            move(-7, 0);
            mCrazyMoveState = 0;
        break;
        default:
        break;
    }
}

void LocalPlayer::crazyMove5()
{
    if (mAction == BeingAction::MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            move(0, 7);
            mCrazyMoveState = 1;
            break;
        case 1:
            move(0, -7);
            mCrazyMoveState = 0;
            break;
        default:
            break;
    }
}

void LocalPlayer::crazyMove6()
{
    if (mAction == BeingAction::MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            move(3, 0);
            mCrazyMoveState = 1;
            break;
        case 1:
            move(2, -2);
            mCrazyMoveState = 2;
            break;
        case 2:
            move(0, -3);
            mCrazyMoveState = 3;
            break;
        case 3:
            move(-2, -2);
            mCrazyMoveState = 4;
            break;
        case 4:
            move(-3, 0);
            mCrazyMoveState = 5;
            break;
        case 5:
            move(-2, 2);
            mCrazyMoveState = 6;
            break;
        case 6:
            move(0, 3);
            mCrazyMoveState = 7;
            break;
        case 7:
            move(2, 2);
            mCrazyMoveState = 0;
            break;
        default:
            break;
    }
}

void LocalPlayer::crazyMove7()
{
    if (mAction == BeingAction::MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            move(1, 1);
            mCrazyMoveState = 1;
            break;
        case 1:
            move(-1, 1);
            mCrazyMoveState = 2;
            break;
        case 2:
            move(-1, -1);
            mCrazyMoveState = 3;
            break;
        case 3:
            move(1, -1);
            mCrazyMoveState = 0;
            break;
        default:
            break;
    }
}

void LocalPlayer::crazyMove8()
{
    if (mAction == BeingAction::MOVE || !mMap)
        return;
    int idx = 0;
    const int dist = 1;

// look
//      up, ri,do,le
    static const int movesX[][4] =
    {
        {-1,  0,  1,  0},   // move left
        { 0,  1,  0, -1},   // move up
        { 1,  0, -1,  0},   // move right
        { 0, -1,  0,  1}    // move down
    };

// look
//      up, ri,do,le
    static const int movesY[][4] =
    {
        { 0, -1,  0,  1},   // move left
        {-1,  0,  1,  0},   // move up
        { 0,  1,  0, -1},   // move right
        { 1,  0, -1,  0}    // move down
    };

    if (mDirection == BeingDirection::UP)
        idx = 0;
    else if (mDirection == BeingDirection::RIGHT)
        idx = 1;
    else if (mDirection == BeingDirection::DOWN)
        idx = 2;
    else if (mDirection == BeingDirection::LEFT)
        idx = 3;


    int mult = 1;
    const unsigned char blockWalkMask = getBlockWalkMask();
    if (mMap->getWalk(mX + movesX[idx][0],
        mY + movesY[idx][0], blockWalkMask))
    {
        while (mMap->getWalk(mX + movesX[idx][0] * mult,
               mY + movesY[idx][0] * mult,
               getBlockWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][0] * (mult - 1), movesY[idx][0] * (mult - 1));
    }
    else if (mMap->getWalk(mX + movesX[idx][1],
             mY + movesY[idx][1], blockWalkMask))
    {
        while (mMap->getWalk(mX + movesX[idx][1] * mult,
               mY + movesY[idx][1] * mult,
               getBlockWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][1] * (mult - 1), movesY[idx][1] * (mult - 1));
    }
    else if (mMap->getWalk(mX + movesX[idx][2],
             mY + movesY[idx][2], blockWalkMask))
    {
        while (mMap->getWalk(mX + movesX[idx][2] * mult,
               mY + movesY[idx][2] * mult,
               getBlockWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][2] * (mult - 1), movesY[idx][2] * (mult - 1));
    }
    else if (mMap->getWalk(mX + movesX[idx][3],
             mY + movesY[idx][3], blockWalkMask))
    {
        while (mMap->getWalk(mX + movesX[idx][3] * mult,
               mY + movesY[idx][3] * mult,
               getBlockWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][3] * (mult - 1), movesY[idx][3] * (mult - 1));
    }
}

void LocalPlayer::crazyMove9()
{
    int dx = 0;
    int dy = 0;

    if (mAction == BeingAction::MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            switch (mDirection)
            {
                case BeingDirection::UP   : dy = -1; break;
                case BeingDirection::DOWN : dy = 1; break;
                case BeingDirection::LEFT : dx = -1; break;
                case BeingDirection::RIGHT: dx = 1; break;
                default: break;
            }
            move(dx, dy);
            mCrazyMoveState = 1;
            break;
        case 1:
            mCrazyMoveState = 2;
            if (!allowAction())
                return;
            Net::getPlayerHandler()->changeAction(BeingAction::SIT);
            break;
        case 2:
            mCrazyMoveState = 3;
            break;
        case 3:
            mCrazyMoveState = 0;
            break;
        default:
            break;
    }
}

void LocalPlayer::crazyMoveA()
{
    const std::string mMoveProgram(config.getStringValue("crazyMoveProgram"));

    if (mAction == BeingAction::MOVE)
        return;

    if (mMoveProgram.empty())
        return;

    if (mCrazyMoveState >= mMoveProgram.length())
        mCrazyMoveState = 0;

    // move command
    if (mMoveProgram[mCrazyMoveState] == 'm')
    {
        mCrazyMoveState ++;
        if (mCrazyMoveState < mMoveProgram.length())
        {
            int dx = 0;
            int dy = 0;

            signed char param = mMoveProgram[mCrazyMoveState++];
            if (param == '?')
            {
                const char cmd[] = {'l', 'r', 'u', 'd', 'L', 'R', 'U', 'D'};
                srand(tick_time);
                param = cmd[rand() % 8];
            }
            switch (param)
            {
                case 'd':
                    move(0, 1);
                    break;
                case 'u':
                    move(0, -1);
                    break;
                case 'l':
                    move(-1, 0);
                    break;
                case 'r':
                    move(1, 0);
                    break;
                case 'D':
                    move(1, 1);
                    break;
                case 'U':
                    move(-1, -1);
                    break;
                case 'L':
                    move(-1, 1);
                    break;
                case 'R':
                    move(1, -1);
                    break;
                case 'f':
                    if (mDirection & BeingDirection::UP)
                        dy = -1;
                    else if (mDirection & BeingDirection::DOWN)
                        dy = 1;
                    if (mDirection & BeingDirection::LEFT)
                        dx = -1;
                    else if (mDirection & BeingDirection::RIGHT)
                        dx = 1;
                    move(dx, dy);
                    break;
                case 'b':
                    if (mDirection & BeingDirection::UP)
                        dy = 1;
                    else if (mDirection & BeingDirection::DOWN)
                        dy = -1;
                    if (mDirection & BeingDirection::LEFT)
                        dx = 1;
                    else if (mDirection & BeingDirection::RIGHT)
                        dx = -1;
                    move(dx, dy);
                    break;
                default:
                    break;
            }
        }
    }
    // direction command
    else if (mMoveProgram[mCrazyMoveState] == 'd')
    {
        mCrazyMoveState ++;

        if (mCrazyMoveState < mMoveProgram.length())
        {
            signed char param = mMoveProgram[mCrazyMoveState++];
            if (param == '?')
            {
                const char cmd[] = {'l', 'r', 'u', 'd'};
                srand(tick_time);
                param = cmd[rand() % 4];
            }
            switch (param)
            {
                case 'd':

//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(BeingDirection::DOWN);
                        Net::getPlayerHandler()->setDirection(
                            BeingDirection::DOWN);
                    }
                    break;
                case 'u':
//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(BeingDirection::UP);
                        Net::getPlayerHandler()->setDirection(
                            BeingDirection::UP);
                    }
                    break;
                case 'l':
//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(BeingDirection::LEFT);
                        Net::getPlayerHandler()->setDirection(
                            BeingDirection::LEFT);
                    }
                    break;
                case 'r':
//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(BeingDirection::RIGHT);
                        Net::getPlayerHandler()->setDirection(
                            BeingDirection::RIGHT);
                    }
                    break;
                case 'L':
//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        uint8_t dir = 0;
                        switch (mDirection)
                        {
                            case BeingDirection::UP:
                                dir = BeingDirection::LEFT;
                                break;
                            case BeingDirection::DOWN:
                                dir = BeingDirection::RIGHT;
                                break;
                            case BeingDirection::LEFT:
                                dir = BeingDirection::DOWN;
                                break;
                            case BeingDirection::RIGHT:
                                dir = BeingDirection::UP;
                                break;
                            default:
                                break;
                        }
                        setDirection(dir);
                        Net::getPlayerHandler()->setDirection(dir);
                    }
                    break;
                case 'R':
//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        uint8_t dir = 0;
                        switch (mDirection)
                        {
                            case BeingDirection::UP:
                                dir = BeingDirection::RIGHT;
                                break;
                            case BeingDirection::DOWN:
                                dir = BeingDirection::LEFT;
                                break;
                            case BeingDirection::LEFT:
                                dir = BeingDirection::UP;
                                break;
                            case BeingDirection::RIGHT:
                                dir = BeingDirection::DOWN;
                                break;
                            default:
                                break;
                        }
                        setDirection(dir);
                        Net::getPlayerHandler()->setDirection(dir);
                    }
                    break;
                case 'b':
//                    if (PacketLimiter::limitPackets(PACKET_DIRECTION))
                    {
                        uint8_t dir = 0;
                        switch (mDirection)
                        {
                            case BeingDirection::UP:
                                dir = BeingDirection::DOWN;
                                break;
                            case BeingDirection::DOWN:
                                dir = BeingDirection::UP;
                                break;
                            case BeingDirection::LEFT:
                                dir = BeingDirection::RIGHT;
                                break;
                            case BeingDirection::RIGHT:
                                dir = BeingDirection::LEFT;
                                break;
                            default:
                                break;
                        }
                        setDirection(dir);
                        Net::getPlayerHandler()->setDirection(dir);
                    }
                    break;
                case '0':
                    dropShortcut->dropFirst();
                    break;
                case 'a':
                    dropShortcut->dropItems();
                    break;
                default:
                    break;
            }
        }
    }
    // sit command
    else if (mMoveProgram[mCrazyMoveState] == 's')
    {
        mCrazyMoveState ++;
        if (toggleSit())
            mCrazyMoveState ++;
    }
    // wear outfits
    else if (mMoveProgram[mCrazyMoveState] == 'o')
    {
        mCrazyMoveState ++;
        if (mCrazyMoveState < mMoveProgram.length())
        {
            // wear next outfit
            if (mMoveProgram[mCrazyMoveState] == 'n')
            {
                mCrazyMoveState ++;
                outfitWindow->wearNextOutfit();
            }
            // wear previous outfit
            else if (mMoveProgram[mCrazyMoveState] == 'p')
            {
                mCrazyMoveState ++;
                outfitWindow->wearPreviousOutfit();
            }
        }
    }
    // pause
    else if (mMoveProgram[mCrazyMoveState] == 'w')
    {
        mCrazyMoveState ++;
    }
    // pick up
    else if (mMoveProgram[mCrazyMoveState] == 'p')
    {
        mCrazyMoveState ++;
        pickUpItems();
    }
    // emote
    else if (mMoveProgram[mCrazyMoveState] == 'e'
             || mMoveProgram[mCrazyMoveState] == 'E')
    {
        mCrazyMoveState ++;
        const signed char emo = mMoveProgram[mCrazyMoveState];
        unsigned char emoteId = 0;
        if (emo == '?')
        {
            srand(tick_time);
            emoteId = static_cast<unsigned char>(
                1 + (rand() % EmoteDB::size()));
        }
        else
        {
            if (emo >= '0' && emo <= '9')
                emoteId = static_cast<unsigned char>(emo - '0' + 1);
            else if (emo >= 'a' && emo <= 'z')
                emoteId = static_cast<unsigned char>(emo - 'a' + 11);
            else if (emo >= 'A' && emo <= 'Z')
                emoteId = static_cast<unsigned char>(emo - 'A' + 37);
        }
        if (mMoveProgram[mCrazyMoveState - 1] == 'e')
            emote(emoteId);
        else if (PacketLimiter::limitPackets(PACKET_CHAT))
            Net::getPetHandler()->emote(emoteId, 0);

        mCrazyMoveState ++;
    }
    else
    {
        mCrazyMoveState ++;
    }

    if (mCrazyMoveState >= mMoveProgram.length())
        mCrazyMoveState = 0;
}

bool LocalPlayer::isReachable(Being *const being,
                              const int maxCost)
{
    if (!being || !mMap)
        return false;

    if (being->isReachable() == Being::REACH_NO)
        return false;

    if (being->getTileX() == mX
        && being->getTileY() == mY)
    {
        being->setDistance(0);
        being->setIsReachable(Being::REACH_YES);
        return true;
    }
    else if (being->getTileX() - 1 <= mX
             && being->getTileX() + 1 >= mX
             && being->getTileY() - 1 <= mY
             && being->getTileY() + 1 >= mY)
    {
        being->setDistance(1);
        being->setIsReachable(Being::REACH_YES);
        return true;
    }

    const Vector &playerPos = getPosition();

    const Path debugPath = mMap->findPath(
        static_cast<int>(playerPos.x - mapTileSize / 2) / mapTileSize,
        static_cast<int>(playerPos.y - mapTileSize) / mapTileSize,
        being->getTileX(), being->getTileY(), getBlockWalkMask(), maxCost);

    being->setDistance(static_cast<int>(debugPath.size()));
    if (!debugPath.empty())
    {
        being->setIsReachable(Being::REACH_YES);
        return true;
    }
    else
    {
        being->setIsReachable(Being::REACH_NO);
        return false;
    }
}

bool LocalPlayer::isReachable(const int x, const int y,
                              const bool allowCollision) const
{
    const WalkLayer *const walk = mMap->getWalkLayer();
    if (!walk)
        return false;
    int num = walk->getDataAt(x, y);
    if (allowCollision && num < 0)
        num = -num;

    return walk->getDataAt(mX, mY) == num;
}

bool LocalPlayer::pickUpItems(int pickUpType)
{
    if (!actorManager)
        return false;

    bool status = false;
    int x = mX;
    int y = mY;

    // first pick up item on player position
    FloorItem *item =
        actorManager->findItem(x, y);
    if (item)
        status = pickUp(item);

    if (pickUpType == 0)
        pickUpType = settings.pickUpType;

    if (pickUpType == 0)
        return status;

    int x1, y1, x2, y2;
    switch (pickUpType)
    {
        case 1:
            switch (mDirection)
            {
                case BeingDirection::UP   : --y; break;
                case BeingDirection::DOWN : ++y; break;
                case BeingDirection::LEFT : --x; break;
                case BeingDirection::RIGHT: ++x; break;
                default: break;
            }
            item = actorManager->findItem(x, y);
            if (item)
                status = pickUp(item);
            break;
        case 2:
            switch (mDirection)
            {
                case BeingDirection::UP:
                    x1 = x - 1; y1 = y - 1; x2 = x + 1; y2 = y; break;
                case BeingDirection::DOWN:
                    x1 = x - 1; y1 = y; x2 = x + 1; y2 = y + 1; break;
                case BeingDirection::LEFT:
                    x1 = x - 1; y1 = y - 1; x2 = x; y2 = y + 1; break;
                case BeingDirection::RIGHT:
                    x1 = x; y1 = y - 1; x2 = x + 1; y2 = y + 1; break;
                default:
                    x1 = x; x2 = x; y1 = y; y2 = y; break;
            }
            if (actorManager->pickUpAll(x1, y1, x2, y2))
                status = true;
            break;
        case 3:
            if (actorManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
                status = true;
            break;

        case 4:
            if (!actorManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
            {
                if (actorManager->pickUpNearest(x, y, 4))
                    status = true;
            }
            else
            {
                status = true;
            }
            break;

        case 5:
            if (!actorManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
            {
                if (actorManager->pickUpNearest(x, y, 8))
                    status = true;
            }
            else
            {
                status = true;
            }
            break;

        case 6:
            if (!actorManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
            {
                if (actorManager->pickUpNearest(x, y, 90))
                    status = true;
            }
            else
            {
                status = true;
            }
            break;

        default:
            break;
    }
    return status;
}


void LocalPlayer::moveByDirection(const unsigned char dir)
{
    int dx = 0, dy = 0;
    if (dir & BeingDirection::UP)
        dy--;
    if (dir & BeingDirection::DOWN)
        dy++;
    if (dir & BeingDirection::LEFT)
        dx--;
    if (dir & BeingDirection::RIGHT)
        dx++;
    move(dx, dy);
}

void LocalPlayer::specialMove(const unsigned char direction)
{
    if (direction && (mNavigateX || mNavigateY))
        navigateClean();

    if (direction && (settings.moveType >= 2
        && settings.moveType <= 4))
    {
        if (mAction == BeingAction::MOVE)
            return;

        unsigned int max;

        if (settings.moveType == 2)
            max = 5;
        else if (settings.moveType == 4)
            max = 1;
        else
            max = 3;

        if (getMoveState() < max)
        {
            moveByDirection(direction);
            mMoveState ++;
        }
        else
        {
            mMoveState = 0;
            crazyMove();
        }
    }
    else
    {
        setWalkingDir(direction);
    }
}

void LocalPlayer::magicAttack() const
{
    if (!chatWindow || !isAlive()
        || !Net::getPlayerHandler()->canUseMagic())
    {
        return;
    }

    switch (settings.magicAttackType)
    {
        // flar W00
        case 0:
            tryMagic("#flar", 1, 0, 10);
            break;
        // chiza W01
        case 1:
            tryMagic("#chiza", 1, 0,  9);
            break;
        // ingrav W10
        case 2:
            tryMagic("#ingrav", 2, 2,  20);
            break;
        // frillyar W11
        case 3:
            tryMagic("#frillyar", 2, 2, 25);
            break;
        // upmarmu W12
        case 4:
            tryMagic("#upmarmu", 2, 2, 20);
            break;
        default:
            break;
    }
}

void LocalPlayer::tryMagic(const std::string &spell, const int baseMagic,
                           const int schoolMagic, const int mana)
{
    if (!chatWindow)
        return;

    if (PlayerInfo::getSkillLevel(340) >= baseMagic
        && PlayerInfo::getSkillLevel(342) >= schoolMagic)
    {
        if (PlayerInfo::getAttribute(Attributes::MP) >= mana)
        {
            if (!PacketLimiter::limitPackets(PACKET_CHAT))
                return;

            chatWindow->localChatInput(spell);
        }
    }
}

void LocalPlayer::loadHomes()
{
    std::string buf;
    std::stringstream ss(serverConfig.getValue("playerHomes", ""));

    while (ss >> buf)
    {
        Vector pos;
        ss >> pos.x;
        ss >> pos.y;
        mHomes[buf] = pos;
    }
}

void LocalPlayer::setMap(Map *const map)
{
    BLOCK_START("LocalPlayer::setMap")
    if (map)
    {
        if (socialWindow)
            socialWindow->updateActiveList();
    }
    navigateClean();
    mCrossX = 0;
    mCrossY = 0;

    Being::setMap(map);
    updateNavigateList();
    BLOCK_END("LocalPlayer::setMap")
}

void LocalPlayer::setHome()
{
    if (!mMap || !socialWindow)
        return;

    SpecialLayer *const specialLayer = mMap->getSpecialLayer();

    if (!specialLayer)
        return;

    const std::string key = mMap->getProperty("_realfilename");
    Vector pos = mHomes[key];

    if (mAction == BeingAction::SIT)
    {
        const std::map<std::string, Vector>::const_iterator
            iter = mHomes.find(key);

        if (iter != mHomes.end())
        {
            socialWindow->removePortal(static_cast<int>(pos.x),
                static_cast<int>(pos.y));
        }

        if (iter != mHomes.end() && mX == static_cast<int>(pos.x)
            && mY == static_cast<int>(pos.y))
        {
            mMap->updatePortalTile("", MapItemType::EMPTY,
                static_cast<int>(pos.x), static_cast<int>(pos.y));

            mHomes.erase(key);
            socialWindow->removePortal(static_cast<int>(pos.x),
                static_cast<int>(pos.y));
        }
        else
        {
            if (iter != mHomes.end())
            {
                specialLayer->setTile(static_cast<int>(pos.x),
                    static_cast<int>(pos.y), MapItemType::EMPTY);
            }

            pos.x = static_cast<float>(mX);
            pos.y = static_cast<float>(mY);
            mHomes[key] = pos;
            mMap->updatePortalTile("home", MapItemType::HOME,
                                   mX, mY);
            socialWindow->addPortal(mX, mY);
        }
        MapItem *const mapItem = specialLayer->getTile(mX, mY);
        if (mapItem)
        {
            const int idx = socialWindow->getPortalIndex(mX, mY);
            mapItem->setName(keyboard.getKeyShortString(
                outfitWindow->keyName(idx)));
        }
        saveHomes();
    }
    else
    {
        MapItem *mapItem = specialLayer->getTile(mX, mY);
        int type = 0;

        const std::map<std::string, Vector>::iterator iter = mHomes.find(key);
        if (iter != mHomes.end() && mX == pos.x && mY == pos.y)
        {
            mHomes.erase(key);
            saveHomes();
        }

        if (!mapItem || mapItem->getType() == MapItemType::EMPTY)
        {
            if (mDirection & BeingDirection::UP)
                type = MapItemType::ARROW_UP;
            else if (mDirection & BeingDirection::LEFT)
                type = MapItemType::ARROW_LEFT;
            else if (mDirection & BeingDirection::DOWN)
                type = MapItemType::ARROW_DOWN;
            else if (mDirection & BeingDirection::RIGHT)
                type = MapItemType::ARROW_RIGHT;
        }
        else
        {
            type = MapItemType::EMPTY;
        }
        mMap->updatePortalTile("", type, mX, mY);

        if (type != MapItemType::EMPTY)
        {
            socialWindow->addPortal(mX, mY);
            mapItem = specialLayer->getTile(mX, mY);
            if (mapItem)
            {
                const int idx = socialWindow->getPortalIndex(mX, mY);
                mapItem->setName(keyboard.getKeyShortString(
                    outfitWindow->keyName(idx)));
            }
        }
        else
        {
            specialLayer->setTile(mX, mY, MapItemType::EMPTY);
            socialWindow->removePortal(mX, mY);
        }
    }
}

void LocalPlayer::saveHomes()
{
    std::stringstream ss;

    for (std::map<std::string, Vector>::const_iterator iter = mHomes.begin(),
         iter_end = mHomes.end(); iter != iter_end; ++iter )
    {
        const Vector &pos = (*iter).second;

        if (iter != mHomes.begin())
            ss << " ";
        ss << (*iter).first << " " << pos.x << " " << pos.y;
    }

    serverConfig.setValue("playerHomes", ss.str());
}

void LocalPlayer::pingRequest()
{
    const int time = tick_time;
    if (mWaitPing == true && mPingSendTick != 0)
    {
        if (time >= mPingSendTick && (time - mPingSendTick) > 1000)
            return;
    }

    mPingSendTick = time;
    mWaitPing = true;
    beingHandler->requestNameById(getId());
}

std::string LocalPlayer::getPingTime() const
{
    std::string str;
    if (!mWaitPing)
    {
        if (!mPingTime)
            str = "?";
        else
            str = toString(mPingTime);
    }
    else
    {
        int time = tick_time;
        if (time > mPingSendTick)
            time -= mPingSendTick;
        else
            time += MAX_TICK_VALUE - mPingSendTick;
        if (time <= mPingTime)
            time = mPingTime;
        if (mPingTime != time)
            str = strprintf("%d (%d)", mPingTime, time);
        else
            str = toString(time);
    }
    return str;
}

void LocalPlayer::pingResponse()
{
    if (mWaitPing == true && mPingSendTick > 0)
    {
        mWaitPing = false;
        const int time = tick_time;
        if (time < mPingSendTick)
        {
            mPingSendTick = 0;
            mPingTime = 0;
        }
        else
        {
            mPingTime = (time - mPingSendTick) * 10;
        }
    }
}

void LocalPlayer::tryPingRequest()
{
    if (mPingSendTick == 0 || tick_time < mPingSendTick
        || (tick_time - mPingSendTick) > 200)
    {
        pingRequest();
    }
}


void LocalPlayer::setAway(const std::string &message) const
{
    setAfkMessage(message);
    GameModifiers::changeAwayMode();
    updateStatus();
}

void LocalPlayer::setAfkMessage(std::string message)
{
    if (!message.empty())
    {
        if (message.size() > 4 && message.substr(0, 4) == "/me ")
        {
            message = message.substr(4);
            config.setValue("afkFormat", 1);
        }
        else
        {
            config.setValue("afkFormat", 0);
        }
        config.setValue("afkMessage", message);
    }
}

void LocalPlayer::setPseudoAway(const std::string &message)
{
    setAfkMessage(message);
    settings.pseudoAwayMode = !settings.pseudoAwayMode;
}

void LocalPlayer::afkRespond(ChatTab *const tab, const std::string &nick)
{
    if (settings.awayMode)
    {
        const int time = cur_time;
        if (mAfkTime == 0 || time < mAfkTime
            || time - mAfkTime > awayLimitTimer)
        {
            std::string msg("*AFK*: "
                + config.getStringValue("afkMessage"));

            if (config.getIntValue("afkFormat") == 1)
                msg = "*" + msg + "*";

            if (!tab)
            {
                chatHandler->privateMessage(nick, msg);
                if (localChatTab)
                {
                    localChatTab->chatLog(std::string(getName()).append(
                        " : ").append(msg), ChatMsgType::ACT_WHISPER, false);
                }
            }
            else
            {
                if (tab->getNoAway())
                    return;
                chatHandler->privateMessage(nick, msg);
                tab->chatLog(getName(), msg);
            }
            mAfkTime = time;
        }
    }
}

bool LocalPlayer::navigateTo(const int x, const int y)
{
    if (!mMap)
        return false;

    SpecialLayer *const tmpLayer = mMap->getTempLayer();
    if (!tmpLayer)
        return false;

    const Vector &playerPos = getPosition();
    mShowNavigePath = true;
    mOldX = static_cast<int>(playerPos.x);
    mOldY = static_cast<int>(playerPos.y);
    mOldTileX = mX;
    mOldTileY = mY;
    mNavigateX = x;
    mNavigateY = y;
    mNavigateId = 0;

    mNavigatePath = mMap->findPath(
        static_cast<int>(playerPos.x - mapTileSize / 2) / mapTileSize,
        static_cast<int>(playerPos.y - mapTileSize) / mapTileSize,
        x, y, getBlockWalkMask(), 0);

    if (mDrawPath)
        tmpLayer->addRoad(mNavigatePath);
    return !mNavigatePath.empty();
}

void LocalPlayer::navigateClean()
{
    if (!mMap)
        return;

    mShowNavigePath = false;
    mOldX = 0;
    mOldY = 0;
    mOldTileX = 0;
    mOldTileY = 0;
    mNavigateX = 0;
    mNavigateY = 0;
    mNavigateId = 0;

    mNavigatePath.clear();

    const SpecialLayer *const tmpLayer = mMap->getTempLayer();
    if (!tmpLayer)
        return;

    tmpLayer->clean();
}

void LocalPlayer::updateMusic()
{
    if (mMap)
    {
        std::string str = mMap->getObjectData(mX, mY, MapItemType::MUSIC);
        if (str.empty())
            str = mMap->getMusicFile();
        if (str != soundManager.getCurrentMusicFile())
        {
            if (str.empty())
                soundManager.fadeOutMusic();
            else
                soundManager.fadeOutAndPlayMusic(str);
        }
    }
}

void LocalPlayer::updateCoords()
{
    Being::updateCoords();

    const Vector &playerPos = getPosition();
    // probably map not loaded.
    if (!playerPos.x || !playerPos.y)
        return;

    if (mX != mOldTileX || mY != mOldTileY)
    {
        if (socialWindow)
            socialWindow->updatePortals();
        if (popupManager)
            popupManager->hideBeingPopup();
        updateMusic();
    }

    if (mShowNavigePath)
    {
        if (mMap && (mX != mOldTileX || mY != mOldTileY))
        {
            SpecialLayer *const tmpLayer = mMap->getTempLayer();
            if (!tmpLayer)
                return;

            const int x = static_cast<int>(playerPos.x - mapTileSize / 2)
                / mapTileSize;
            const int y = static_cast<int>(playerPos.y - mapTileSize)
                / mapTileSize;
            if (mNavigateId)
            {
                if (!actorManager)
                {
                    navigateClean();
                    return;
                }

                const Being *const being = actorManager
                    ->findBeing(mNavigateId);
                if (!being)
                {
                    navigateClean();
                    return;
                }
                mNavigateX = being->getTileX();
                mNavigateY = being->getTileY();
            }

            if (mNavigateX == x && mNavigateY == y)
            {
                navigateClean();
                return;
            }
            else
            {
                for (Path::const_iterator i = mNavigatePath.begin(),
                     i_end = mNavigatePath.end(); i != i_end; ++ i)
                {
                    if ((*i).x == mX && (*i).y == mY)
                    {
                        mNavigatePath.pop_front();
                        break;
                    }
                }

                if (mDrawPath)
                {
                    tmpLayer->clean();
                    tmpLayer->addRoad(mNavigatePath);
                }
            }
        }
    }
    mOldX = static_cast<int>(playerPos.x);
    mOldY = static_cast<int>(playerPos.y);
    mOldTileX = mX;
    mOldTileY = mY;
}

void LocalPlayer::targetMoved() const
{
/*
    if (mKeepAttacking)
    {
        if (mTarget && mServerAttack)
        {
            logger->log("LocalPlayer::targetMoved0");
            if (!PacketLimiter::limitPackets(PACKET_ATTACK))
                return;
            logger->log("LocalPlayer::targetMoved");
            Net::getPlayerHandler()->attack(mTarget->getId(), mServerAttack);
        }
    }
*/
}

int LocalPlayer::getPathLength(const Being *const being) const
{
    if (!mMap || !being)
        return 0;

    const Vector &playerPos = getPosition();

    if (being->mX == mX && being->mY == mY)
        return 0;

    if (being->mX - 1 <= mX && being->mX + 1 >= mX
        && being->mY - 1 <= mY && being->mY + 1 >= mY)
    {
        return 1;
    }

    if (mTargetOnlyReachable)
    {
        const Path debugPath = mMap->findPath(
            static_cast<int>(playerPos.x - mapTileSize / 2) / mapTileSize,
            static_cast<int>(playerPos.y - mapTileSize) / mapTileSize,
            being->getTileX(), being->getTileY(),
            getBlockWalkMask(), 0);
        return static_cast<int>(debugPath.size());
    }
    else
    {
        const int dx = static_cast<int>(abs(being->mX - mX));
        const int dy = static_cast<int>(abs(being->mY - mY));
        if (dx > dy)
            return dx;
        return dy;
    }
}

int LocalPlayer::getAttackRange2() const
{
    int range = getAttackRange();
    if (range == 1)
        range = 2;
    return range;
}

void LocalPlayer::attack2(Being *const target, const bool keep,
                          const bool dontChangeEquipment)
{
    if (!dontChangeEquipment && target)
        changeEquipmentBeforeAttack(target);

    const bool broken = Net::getServerFeatures()
        ->haveBrokenPlayerAttackDistance();

    // probably need cache getPathLength(target)
    if ((!target || settings.attackType == 0 || settings.attackType == 3)
        || (withinAttackRange(target, broken, broken ? 1 : 0)
        && getPathLength(target) <= getAttackRange2()))
    {
        attack(target, keep);
        if (settings.attackType == 2)
        {
            if (!target)
            {
                if (pickUpItems())
                    return;
            }
            else
            {
                pickUpItems(3);
            }
        }
    }
    else if (!mPickUpTarget)
    {
        if (settings.attackType == 2)
        {
            if (pickUpItems())
                return;
        }
        setTarget(target);
        if (target->getType() != ActorType::Npc)
        {
            mKeepAttacking = true;
            moveToTarget();
        }
    }
}

void LocalPlayer::setFollow(const std::string &player)
{
    mPlayerFollowed = player;
    if (!mPlayerFollowed.empty())
    {
        // TRANSLATORS: follow command message
        std::string msg = strprintf(_("Follow: %s"), player.c_str());
        debugMsg(msg);
    }
    else
    {
        // TRANSLATORS: follow command message
        debugMsg(_("Follow canceled"));
    }
}

void LocalPlayer::setImitate(const std::string &player)
{
    mPlayerImitated = player;
    if (!mPlayerImitated.empty())
    {
        // TRANSLATORS: imitate command message
        std::string msg = strprintf(_("Imitation: %s"), player.c_str());
        debugMsg(msg);
    }
    else
    {
        // TRANSLATORS: imitate command message
        debugMsg(_("Imitation canceled"));
    }
}

void LocalPlayer::cancelFollow()
{
    if (!mPlayerFollowed.empty())
    {
        // TRANSLATORS: cancel follow message
        debugMsg(_("Follow canceled"));
    }
    if (!mPlayerImitated.empty())
    {
        // TRANSLATORS: cancel follow message
        debugMsg(_("Imitation canceled"));
    }
    mPlayerFollowed.clear();
    mPlayerImitated.clear();
}

void LocalPlayer::imitateEmote(const Being *const being,
                               const unsigned char action) const
{
    if (!being)
        return;

    std::string player_imitated = getImitate();
    if (!player_imitated.empty() && being->getName() == player_imitated)
        emote(action);
}

void LocalPlayer::imitateAction(const Being *const being,
                                const BeingAction::Action &action)
{
    if (!being)
        return;

    if (!mPlayerImitated.empty() && being->getName() == mPlayerImitated)
    {
        setAction(action);
        Net::getPlayerHandler()->changeAction(action);
    }
}

void LocalPlayer::imitateDirection(const Being *const being,
                                   const unsigned char dir)
{
    if (!being)
        return;

    if (!mPlayerImitated.empty() && being->getName() == mPlayerImitated)
    {
        if (!PacketLimiter::limitPackets(PACKET_DIRECTION))
            return;

        if (settings.followMode == 2)
        {
            uint8_t dir2 = 0;
            if (dir & BeingDirection::LEFT)
                dir2 |= BeingDirection::RIGHT;
            else if (dir & BeingDirection::RIGHT)
                dir2 |= BeingDirection::LEFT;
            if (dir & BeingDirection::UP)
                dir2 |= BeingDirection::DOWN;
            else if (dir & BeingDirection::DOWN)
                dir2 |= BeingDirection::UP;

            setDirection(dir2);
            Net::getPlayerHandler()->setDirection(dir2);
        }
        else
        {
            setDirection(dir);
            Net::getPlayerHandler()->setDirection(dir);
        }
    }
}

void LocalPlayer::imitateOutfit(const Being *const player,
                                const int sprite) const
{
    if (!player)
        return;

    if (settings.imitationMode == 1 && !mPlayerImitated.empty()
        && player->getName() == mPlayerImitated)
    {
        if (sprite < 0 || sprite >= player->getNumberOfLayers())
            return;

        const AnimatedSprite *const equipmentSprite
            = dynamic_cast<const AnimatedSprite *const>(
            player->getSprite(sprite));

        if (equipmentSprite)
        {
//            logger->log("have equipmentSprite");
            const Inventory *const inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            const std::string &path = equipmentSprite->getIdPath();
            if (path.empty())
                return;

//            logger->log("idPath: " + path);
            const Item *const item = inv->findItemBySprite(path,
                player->getGender(), player->getSubType());
            if (item && !item->isEquipped())
                PlayerInfo::equipItem(item, false);
        }
        else
        {
//            logger->log("have unequip %d", sprite);
            const int equipmentSlot = inventoryHandler
                ->convertFromServerSlot(sprite);
//            logger->log("equipmentSlot: " + toString(equipmentSlot));
            if (equipmentSlot == ItemSlot::PROJECTILE_SLOT)
                return;

            const Item *const item = PlayerInfo::getEquipment(equipmentSlot);
            if (item)
            {
//                logger->log("unequiping");
                PlayerInfo::unequipItem(item, false);
            }
        }
    }
}

void LocalPlayer::followMoveTo(const Being *const being,
                               const int x, const int y)
{
    if (being && !mPlayerFollowed.empty()
        && being->getName() == mPlayerFollowed)
    {
        mPickUpTarget = nullptr;
        setDestination(x, y);
    }
}

void LocalPlayer::followMoveTo(const Being *const being,
                               const int x1, const int y1,
                               const int x2, const int y2)
{
    if (!being)
        return;

    mPickUpTarget = nullptr;
    if (!mPlayerFollowed.empty() && being->getName() == mPlayerFollowed)
    {
        switch (settings.followMode)
        {
            case 0:
                setDestination(x1, y1);
                setNextDest(x2, y2);
                break;
            case 1:
                if (x1 != x2 || y1 != y2)
                {
                    setDestination(mX + x2 - x1, mY + y2 - y1);
                    setNextDest(mX + x2 - x1, mY + y2 - y1);
                }
                break;
            case 2:
                if (x1 != x2 || y1 != y2)
                {
                    setDestination(mX + x1 - x2, mY + y1 - y2);
                    setNextDest(mX + x1 - x2, mY + y1 - y2);
                }
                break;
            case 3:
                if (!mTarget || mTarget->getName() != mPlayerFollowed)
                {
                    if (actorManager)
                    {
                        Being *const b = actorManager->findBeingByName(
                            mPlayerFollowed, ActorType::Player);
                        setTarget(b);
                    }
                }
                moveToTarget();
                setNextDest(x2, y2);
                break;
            default:
                break;
        }
    }
}

void LocalPlayer::setNextDest(const int x, const int y)
{
    mNextDestX = x;
    mNextDestY = y;
}

bool LocalPlayer::allowAction()
{
    if (mIsServerBuggy)
    {
        if (mLastAction != -1)
            return false;
        mLastAction = tick_time;
    }
    return true;
}

/*
bool LocalPlayer::allowMove() const
{
    if (mIsServerBuggy)
    {
        if (mAction == MOVE)
            return false;
    }
    return true;
}
*/

void LocalPlayer::fixPos(const int maxDist)
{
    if (!mCrossX && !mCrossY)
        return;

    const int dx = abs(mX - mCrossX);
    const int dy = abs(mY - mCrossY);
    const int dest = (dx * dx) + (dy * dy);
    const int time = cur_time;

    if (dest > maxDist && mActivityTime
        && (time < mActivityTime || time - mActivityTime > 2))
    {
        mActivityTime = time;
        setDestination(mCrossX, mCrossY);
    }
}

void LocalPlayer::setRealPos(const int x, const int y)
{
    if (!mMap)
        return;

    SpecialLayer *const layer = mMap->getTempLayer();
    if (layer)
    {
        fixPos(1);

        if ((mCrossX || mCrossY)
            && layer->getTile(mCrossX, mCrossY)
            && layer->getTile(mCrossX, mCrossY)->getType()
            == MapItemType::CROSS)
        {
            layer->setTile(mCrossX, mCrossY, MapItemType::EMPTY);
        }

        if (mShowServerPos)
        {
            const MapItem *const mapItem = layer->getTile(x, y);

            if (!mapItem || mapItem->getType() == MapItemType::EMPTY)
            {
                if (getTileX() != x && getTileY() != y)
                    layer->setTile(x, y, MapItemType::CROSS);
            }
        }

        mCrossX = x;
        mCrossY = y;
    }
    if (mMap->isCustom())
        mMap->setWalk(x, y, true);
}
void LocalPlayer::fixAttackTarget()
{
    if (!mMap || !mTarget)
        return;

    if (settings.moveToTargetType == 7 || !settings.attackType
        || !config.getBoolValue("autofixPos"))
    {
        return;
    }

    const Vector &playerPos = getPosition();
    const Path debugPath = mMap->findPath(
        static_cast<int>(playerPos.x - mapTileSize / 2) / mapTileSize,
        static_cast<int>(playerPos.y - mapTileSize) / mapTileSize,
        mTarget->getTileX(), mTarget->getTileY(),
        getBlockWalkMask(), 0);

    if (!debugPath.empty())
    {
        const Path::const_iterator i = debugPath.begin();
        setDestination((*i).x, (*i).y);
    }
}

void LocalPlayer::respawn()
{
    navigateClean();
}

int LocalPlayer::getLevel() const
{
    return PlayerInfo::getAttribute(Attributes::LEVEL);
}

void LocalPlayer::updateNavigateList()
{
    if (mMap)
    {
        const std::map<std::string, Vector>::const_iterator iter =
            mHomes.find(mMap->getProperty("_realfilename"));

        if (iter != mHomes.end())
        {
            const Vector &pos = mHomes[(*iter).first];
            if (pos.x && pos.y)
            {
                mMap->addPortalTile("home", MapItemType::HOME,
                    static_cast<int>(pos.x), static_cast<int>(pos.y));
            }
        }
    }
}

void LocalPlayer::waitFor(const std::string &nick)
{
    mWaitFor = nick;
}

void LocalPlayer::checkNewName(Being *const being)
{
    if (!being)
        return;

    const std::string &nick = being->getName();
    if (being->getType() == ActorType::Player)
    {
        const Guild *const guild = getGuild();
        if (guild)
        {
            const GuildMember *const gm = guild->getMember(nick);
            if (gm)
            {
                const int level = gm->getLevel();
                if (level > 1 && being->getLevel() != level)
                {
                    being->setLevel(level);
                    being->updateName();
                }
            }
        }
        if (chatWindow)
        {
            WhisperTab *const tab = chatWindow->getWhisperTab(nick);
            if (tab)
                tab->setWhisperTabColors();
        }
    }

    if (!mWaitFor.empty() && mWaitFor == nick)
    {
        // TRANSLATORS: wait player/monster message
        debugMsg(strprintf(_("You see %s"), mWaitFor.c_str()));
        soundManager.playGuiSound(SOUND_INFO);
        mWaitFor.clear();
    }
}

unsigned char LocalPlayer::getBlockWalkMask() const
{
    // for now blocking all types of collisions
    return BlockMask::WALL | BlockMask::AIR | BlockMask::WATER;
}

void LocalPlayer::removeHome()
{
    if (!mMap)
        return;

    const std::string key = mMap->getProperty("_realfilename");
    const std::map<std::string, Vector>::iterator iter = mHomes.find(key);

    if (iter != mHomes.end())
        mHomes.erase(key);
}

void LocalPlayer::stopAdvert()
{
    mBlockAdvert = true;
}

bool LocalPlayer::checAttackPermissions(const Being *const target)
{
    if (!target)
        return false;

    switch (settings.pvpAttackType)
    {
        case 0:
            return true;
        case 1:
            return !(player_relations.getRelation(target->getName())
                == PlayerRelation::FRIEND);
        case 2:
            return player_relations.checkBadRelation(target->getName());
        default:
        case 3:
            return false;
    }
}

void LocalPlayer::updateStatus() const
{
    if (Net::getServerFeatures()->havePlayerStatusUpdate() && mEnableAdvert)
    {
        uint8_t status = 0;
        if (mTradebot && shopWindow && !shopWindow->isShopEmpty())
            status |= BeingFlag::SHOP;

        if (settings.awayMode || settings.pseudoAwayMode)
            status |= BeingFlag::AWAY;

        if (mInactive)
            status |= BeingFlag::INACTIVE;

        Net::getPlayerHandler()->updateStatus(status);
    }
}

void LocalPlayer::setTestParticle(const std::string &fileName,
                                  const bool updateHash)
{
    mTestParticleName = fileName;
    mTestParticleTime = cur_time;
    if (mTestParticle)
    {
        mChildParticleEffects.removeLocally(mTestParticle);
        mTestParticle = nullptr;
    }
    if (!fileName.empty())
    {
        mTestParticle = particleEngine->addEffect(fileName, 0, 0, 0);
        controlParticle(mTestParticle);
        if (updateHash)
            mTestParticleHash = UpdaterWindow::getFileHash(mTestParticleName);
    }
}
