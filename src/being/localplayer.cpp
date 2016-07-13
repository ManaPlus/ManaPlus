/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "being/localplayer.h"

#include "actormanager.h"
#include "configuration.h"
#include "gamemodifiers.h"
#include "guild.h"
#include "party.h"
#include "settings.h"
#include "soundmanager.h"
#include "statuseffect.h"

#include "being/beingflag.h"
#include "being/crazymoves.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "const/sound.h"

#include "enums/being/beingdirection.h"

#include "enums/resources/map/mapitemtype.h"

#include "particle/particleengine.h"

#include "input/keyboardconfig.h"

#include "gui/gui.h"
#include "gui/userpalette.h"
#include "gui/popupmanager.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/chat/whispertab.h"

#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/inventoryhandler.h"
#include "net/packetlimiter.h"
#include "net/playerhandler.h"
#include "net/serverfeatures.h"

#include "resources/iteminfo.h"
#include "resources/itemslot.h"

#include "resources/db/weaponsdb.h"

#include "resources/item/item.h"

#include "resources/map/map.h"
#include "resources/map/mapitem.h"
#include "resources/map/speciallayer.h"
#include "resources/map/walklayer.h"

#include "listeners/awaylistener.h"

#include "resources/sprite/animatedsprite.h"

#include "net/net.h"

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

LocalPlayer::LocalPlayer(const BeingId id,
                         const BeingTypeId subtype) :
    Being(id, ActorType::Player, subtype, nullptr),
    AttributeListener(),
    PlayerDeathListener(),
    StatListener(),
    mGMLevel(0),
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
    mNavigateId(BeingId_zero),
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
    mSyncPlayerMoveDistance(config.getBoolValue("syncPlayerMoveDistance")),
    mWalkingDir(0),
    mUpdateName(true),
    mBlockAdvert(false),
    mTargetDeadPlayers(config.getBoolValue("targetDeadPlayers")),
    mServerAttack(fromBool(config.getBoolValue("serverAttack"), Keep)),
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
    mGoingToTarget(false),
    mKeepAttacking(false),
    mPathSetByMouse(false),
    mWaitPing(false),
    mShowNavigePath(false),
    mAllowRename(false),
    mXpToNextLevel(0)
{
    logger->log1("LocalPlayer::LocalPlayer");

    mAttackRange = 0;
    mLevel = 1;
    mAdvanced = true;
    mTextColor = &theme->getColor(ThemeColorId::PLAYER, 255);
    if (userPalette)
        mNameColor = &userPalette->getColor(UserColorId::SELF);
    else
        mNameColor = nullptr;

    PlayerInfo::setStatBase(Attributes::WALK_SPEED,
        getWalkSpeed());
    PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);

    loadHomes();

    config.addListener("showownname", this);
    config.addListener("targetDeadPlayers", this);
    serverConfig.addListener("enableBuggyServers", this);
    config.addListener("syncPlayerMove", this);
    config.addListener("syncPlayerMoveDistance", this);
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
    
    mXpToNextLevel = PlayerInfo::getAttribute(Attributes::EXP_NEEDED) - PlayerInfo::getAttribute(Attributes::EXP);
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
                    mPixelX,
                    mPixelY - 48,
                    &userPalette->getColor(info.second),
                    gui->getInfoParticleFont(),
                    true);
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
            const int rangeX = CAST_S32(
                abs(mTarget->mX - mX));
            const int rangeY = CAST_S32(
                abs(mTarget->mY - mY));
            const int attackRange = getAttackRange();
            const TargetCursorTypeT targetType
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

    if (serverFeatures &&
        !serverFeatures->havePlayerStatusUpdate() &&
        mEnableAdvert &&
        !mBlockAdvert &&
        mAdvertTime < cur_time)
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

void LocalPlayer::setAction(const BeingActionT &action,
                            const int attackType)
{
    if (action == BeingAction::DEAD)
    {
        if (!mLastHitFrom.empty() &&
            !serverFeatures->haveKillerId())
        {
            // TRANSLATORS: chat message after death
            debugMsg(strprintf(_("You were killed by %s."),
                mLastHitFrom.c_str()));
            mLastHitFrom.clear();
        }
        setTarget(nullptr);
    }

    Being::setAction(action, attackType);
#ifdef USE_MUMBLE
    if (mumbleManager)
        mumbleManager->setAction(CAST_S32(action));
#endif
}

void LocalPlayer::setGMLevel(const int level)
{
    mGMLevel = level;

    if (level > 0)
    {
        setGM(true);
        if (statusWindow)
            statusWindow->updateLevelLabel();
        if (chatWindow)
        {
            chatWindow->loadGMCommands();
            chatWindow->showGMTab();
        }
    }
}

void LocalPlayer::nextTile(unsigned char dir A_UNUSED = 0)
{
    const Party *const party = Party::getParty(1);
    if (party)
    {
        PartyMember *const pm = party->getMember(mName);
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

#ifdef EATHENA_SUPPORT
    PlayerInfo::updateMoveAI();
#endif
}

bool LocalPlayer::pickUp(FloorItem *const item)
{
    if (!item)
        return false;

    if (!PacketLimiter::limitPackets(PacketType::PACKET_PICKUP))
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
            PlayerInfo::pickUpItem(item, Sfx_true);
            mPickUpTarget = nullptr;
        }
    }
    else if (pickUpType >= 4 && pickUpType <= 6)
    {
        const Path debugPath = mMap->findPath(
            (mPixelX - mapTileSize / 2) / mapTileSize,
            (mPixelY - mapTileSize) / mapTileSize,
            item->getTileX(),
            item->getTileY(),
            getBlockWalkMask(),
            0);
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
        mLastTargetX = mTarget->mX;
        mLastTargetY = mTarget->mY;
        mTarget->updateName();
    }

    if (target && target->getType() == ActorType::Monster)
        target->setShowName(true);
}

Being *LocalPlayer::setNewTarget(const ActorTypeT type,
                                 const AllowSort allowSort)
{
    if (actorManager)
    {
        Being *const target = actorManager->findNearestLivingBeing(
            localPlayer, 20, type, allowSort);

        if (target && target != mTarget)
            setTarget(target);

        return target;
    }
    return nullptr;
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
            playerHandler->setDestination(x, y, mDirection);
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

            playerHandler->setDestination(x, y, newDir);

//            if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
            {
                setDirection(newDir);
                playerHandler->setDirection(newDir);
            }

            Being::setDestination(x, y);
            playerHandler->setDestination(x, y, mDirection);
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

//            if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
        {
            playerHandler->setDirection(dir);
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
        setDestination(mPixelX,
            mPixelY);
        if (sendToServer)
        {
            playerHandler->setDestination(
                mPixelX,
                mPixelY,
                -1);
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
    if (!PacketLimiter::limitPackets(PacketType::PACKET_SIT))
        return false;

    BeingActionT newAction;
    switch (mAction)
    {
        case BeingAction::STAND:
        case BeingAction::PRESTAND:
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
        case BeingAction::CAST:
        default:
            return true;
    }

    playerHandler->changeAction(newAction);
    return true;
}

bool LocalPlayer::updateSit() const
{
    if (!PacketLimiter::limitPackets(PacketType::PACKET_SIT))
        return false;

    playerHandler->changeAction(mAction);
    return true;
}

bool LocalPlayer::emote(const uint8_t emotion)
{
    if (!PacketLimiter::limitPackets(PacketType::PACKET_EMOTE))
        return false;

    playerHandler->emote(emotion);
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

    const int dist_x = target->mX - mX;
    const int dist_y = target->mY - mY;

    // Must be standing or sitting or casting to attack
    if (mAction != BeingAction::STAND &&
        mAction != BeingAction::SIT &&
        mAction != BeingAction::CAST)
    {
        return;
    }

    if (!serverFeatures->haveAttackDirections())
    {
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
    }

    mActionTime = tick_time;

    if (target->getType() != ActorType::Player
        || checAttackPermissions(target))
    {
        setAction(BeingAction::ATTACK);

        if (!PacketLimiter::limitPackets(PacketType::PACKET_ATTACK))
            return;

        if (!dontChangeEquipment)
            changeEquipmentBeforeAttack(target);

        const BeingId targetId = target->getId();
        playerHandler->attack(targetId, mServerAttack);
#ifdef EATHENA_SUPPORT
        PlayerInfo::updateAttackAi(targetId, mServerAttack);
#endif
    }

    if (!keep)
        stopAttack();
}

void LocalPlayer::stopAttack(const bool keepAttack)
{
    if (!PacketLimiter::limitPackets(PacketType::PACKET_STOPATTACK))
        return;

    if (mServerAttack == Keep_true && mAction == BeingAction::ATTACK)
        playerHandler->stopAttack();

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

void LocalPlayer::pickedUp(const ItemInfo &itemInfo,
                           const int amount,
                           const ItemColor color,
                           const BeingId floorItemId,
                           const PickupT fail)
{
    if (fail != Pickup::OKAY)
    {
        if (actorManager && floorItemId != BeingId_zero)
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
            case Pickup::MAX_AMOUNT:
                // TRANSLATORS: pickup error message
                msg = N_("You can't pickup this amount of items.");
                break;
            case Pickup::STACK_AMOUNT:
                // TRANSLATORS: pickup error message
                msg = N_("Your item stack has max amount.");
                break;
            case Pickup::OKAY:
                break;
            default:
            case Pickup::UNKNOWN:
                // TRANSLATORS: pickup error message
                msg = N_("Unknown problem picking up item.");
                break;
        }
        if (localChatTab && config.getBoolValue("showpickupchat"))
            localChatTab->chatLog(gettext(msg), ChatMsgType::BY_SERVER);

        if (mMap && config.getBoolValue("showpickupparticle"))
        {
            // Show pickup notification
            addMessageToQueue(gettext(msg), UserColorId::PICKUP_INFO);
        }
    }
    else
    {
        std::string str;
        if (serverFeatures->haveItemColors())
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
                    str.c_str()), UserColorId::PICKUP_INFO);
            }
            else
            {
                addMessageToQueue(str, UserColorId::PICKUP_INFO);
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

    dx = CAST_S32(abs(target->mX - mX));
    dy = CAST_S32(abs(target->mY - mY));
    return !(dx > range || dy > range);
}

void LocalPlayer::setGotoTarget(Being *const target)
{
    if (!target)
        return;

    mPickUpTarget = nullptr;
    setTarget(target);
    mGoingToTarget = true;
    navigateTo(target->mX,
        target->mY);
}

void LocalPlayer::handleStatusEffect(const StatusEffect *const effect,
                                     const int32_t effectId,
                                     const IsStart start)
{
    Being::handleStatusEffect(effect,
        effectId,
        start);

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
                        miniStatusWindow->eraseIcon(CAST_S32(i));
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
                        miniStatusWindow->setIcon(CAST_S32(i), sprite);
                    found = true;
                    break;
                }
            }

            if (!found)
            {   // add new
                const int offset = CAST_S32(mStatusEffectIcons.size());
                if (miniStatusWindow)
                    miniStatusWindow->setIcon(offset, sprite);
                mStatusEffectIcons.push_back(effectId);
            }
        }
    }
}

void LocalPlayer::addMessageToQueue(const std::string &message,
                                    const UserColorIdT color)
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
    else if (value == "syncPlayerMoveDistance")
        mSyncPlayerMoveDistance = config.getIntValue("syncPlayerMoveDistance");
    else if (value == "drawPath")
        mDrawPath = config.getBoolValue("drawPath");
    else if (value == "serverAttack")
        mServerAttack = fromBool(config.getBoolValue("serverAttack"), Keep);
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

void LocalPlayer::addJobMessage(const int change)
{
    if (change != 0 && mMessages.size() < 20)
    {
        if (!mMessages.empty())
        {
            MessagePair pair = mMessages.back();
            // TRANSLATORS: this is normal experience
            if (pair.first.find(strprintf(" %s", _("xp"))) ==
                // TRANSLATORS: this is normal experience
                pair.first.size() - strlen(_("xp")) - 1)
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

void LocalPlayer::addXpMessage(const int change)
{
    if (change != 0 && mMessages.size() < 20)
    {
        // TRANSLATORS: get xp message
        addMessageToQueue(strprintf("%d %s", change, _("xp")));
    }
}

void LocalPlayer::addHpMessage(const int change)
{
    if (change != 0 && mMessages.size() < 20)
    {
        // TRANSLATORS: get hp message
        addMessageToQueue(strprintf("%d %s", change, _("hp")));
    }
}

void LocalPlayer::addSpMessage(const int change)
{
    if (change != 0 && mMessages.size() < 20)
    {
        // TRANSLATORS: get hp message
        addMessageToQueue(strprintf("%d %s", change, _("mana")));
    }
}

void LocalPlayer::statChanged(const AttributesT id,
                              const int oldVal1,
                              const int oldVal2)
{
    if (!mShowJobExp ||
        id != Attributes::JOB ||
        serverFeatures->haveExpPacket())
    {
        return;
    }

    const std::pair<int, int> exp = PlayerInfo::getStatExperience(id);
    if (oldVal1 > exp.first || !oldVal2)
        return;

    const int change = exp.first - oldVal1;
    addJobMessage(change);
}

void LocalPlayer::attributeChanged(const AttributesT id,
                                   const int oldVal,
                                   const int newVal)
{
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (id)
    {
        case Attributes::EXP:
        {
            if (serverFeatures->haveExpPacket())
                break;
                
            int change = 0;
            if (oldVal > newVal)
            {
                if (mXpToNextLevel > 0)
                {
                	change = mXpToNextLevel + newVal;
                }
                else
                {
                	break;
                }
            }
            else
            {
            	change = newVal - oldVal;
            }
 
            if (change != 0)
            {
                // TRANSLATORS: get xp message
                addXpMessage(change);
            }
            mXpToNextLevel = PlayerInfo::getAttribute(Attributes::EXP_NEEDED) - newVal;
            break;
        }
        case Attributes::LEVEL:
            mLevel = newVal;
            break;
        case Attributes::HP:
            if (oldVal != 0 && newVal == 0)
                PlayerDeathListener::distributeEvent();
            break;
        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)
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

    unsigned int limit(0);

    if (dist == -1)
    {
        dist = settings.moveToTargetType;
        if (dist != 0)
        {
            const bool broken = serverFeatures
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
            debugPath = mMap->findPath(
                (mPixelX - mapTileSize / 2) / mapTileSize,
                (mPixelY - mapTileSize) / mapTileSize,
                mTarget->mX,
                mTarget->mY,
                getBlockWalkMask(),
                0);
        }

        const size_t sz = debugPath.size();
        if (sz < CAST_SIZE(dist))
            return;
        limit = CAST_S32(sz) - dist;
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
                navigateTo(mTarget->mX, mTarget->mY);
        }
        else
        {
            Position pos(0, 0);
            unsigned int f = 0;

            for (Path::const_iterator i = debugPath.begin(),
                 i_fend = debugPath.end();
                 i != i_fend && f < limit; ++i, f++)
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
                playerHandler->setDestination(
                        CAST_S32(pos.x),
                        CAST_S32(pos.y),
                        CAST_S32(mDirection));
            }
            else
            {
                navigateTo(CAST_S32(pos.x), CAST_S32(pos.y));
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
    const int dx = target->mX - mX;
    const int dy = target->mY - mY;
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
            item = inv->findItem(*it, ItemColor_zero);
            if (item)
                break;
        }

        // no swords
        if (!item)
            return;

        // if sword not equiped
        if (item->isEquipped() == Equipped_false)
            PlayerInfo::equipItem(item, Sfx_true);

        // if need equip shield too
        if (settings.attackWeaponType == 3)
        {
            // searching shield
            const WeaponsInfos &shields = WeaponsDB::getShields();
            FOR_EACH (WeaponsInfosIter, it, shields)
            {
                item = inv->findItem(*it, ItemColor_zero);
                if (item)
                    break;
            }
            if (item && item->isEquipped() == Equipped_false)
                PlayerInfo::equipItem(item, Sfx_true);
        }
    }
    // big distance. allowed only bow
    else
    {
        // searching bow
        const WeaponsInfos &bows = WeaponsDB::getBows();
        FOR_EACH (WeaponsInfosIter, it, bows)
        {
            item = inv->findItem(*it, ItemColor_zero);
            if (item)
                break;
        }

        // no bow
        if (!item)
            return;

        if (item->isEquipped() == Equipped_false)
            PlayerInfo::equipItem(item, Sfx_true);
    }
}

bool LocalPlayer::isReachable(Being *const being,
                              const int maxCost)
{
    if (!being || !mMap)
        return false;

    if (being->getReachable() == Reachable::REACH_NO)
        return false;

    if (being->mX == mX &&
        being->mY == mY)
    {
        being->setDistance(0);
        being->setReachable(Reachable::REACH_YES);
        return true;
    }
    else if (being->mX - 1 <= mX &&
             being->mX + 1 >= mX &&
             being->mY - 1 <= mY &&
             being->mY + 1 >= mY)
    {
        being->setDistance(1);
        being->setReachable(Reachable::REACH_YES);
        return true;
    }

    const Path debugPath = mMap->findPath(
        (mPixelX - mapTileSize / 2) / mapTileSize,
        (mPixelY - mapTileSize) / mapTileSize,
        being->mX,
        being->mY,
        getBlockWalkMask(),
        maxCost);

    being->setDistance(CAST_S32(debugPath.size()));
    if (!debugPath.empty())
    {
        being->setReachable(Reachable::REACH_YES);
        return true;
    }
    else
    {
        being->setReachable(Reachable::REACH_NO);
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
            crazyMoves->crazyMove();
        }
    }
    else
    {
        setWalkingDir(direction);
    }
}

void LocalPlayer::magicAttack() const
{
#ifdef EATHENA_SUPPORT
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return;
#endif
    if (!chatWindow || !isAlive()
        || !playerHandler->canUseMagic())
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
            if (!PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
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
            socialWindow->removePortal(CAST_S32(pos.x),
                CAST_S32(pos.y));
        }

        if (iter != mHomes.end() && mX == CAST_S32(pos.x)
            && mY == CAST_S32(pos.y))
        {
            mMap->updatePortalTile("", MapItemType::EMPTY,
                CAST_S32(pos.x), CAST_S32(pos.y));

            mHomes.erase(key);
            socialWindow->removePortal(CAST_S32(pos.x),
                CAST_S32(pos.y));
        }
        else
        {
            if (iter != mHomes.end())
            {
                specialLayer->setTile(CAST_S32(pos.x),
                    CAST_S32(pos.y), MapItemType::EMPTY);
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
         iter_fend = mHomes.end();
         iter != iter_fend;
         ++iter)
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
    GameModifiers::changeAwayMode(true);
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
        serverConfig.setValue("afkMessage", message);
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
            std::string str(serverConfig.getValue("afkMessage",
                "I am away from keyboard."));
            if (str.find("'NAME'") != std::string::npos)
                replaceAll(str, "'NAME'", nick);

            std::string msg("*AFK*: " + str);

            if (config.getIntValue("afkFormat") == 1)
                msg = "*" + msg + "*";

            if (!tab)
            {
                chatHandler->privateMessage(nick, msg);
                if (localChatTab)
                {
                    localChatTab->chatLog(std::string(mName).append(
                        " : ").append(msg),
                        ChatMsgType::ACT_WHISPER,
                        IgnoreRecord_false);
                }
            }
            else
            {
                if (tab->getNoAway())
                    return;
                chatHandler->privateMessage(nick, msg);
                tab->chatLog(mName, msg);
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

    mShowNavigePath = true;
    mOldX = mPixelX;
    mOldY = mPixelY;
    mOldTileX = mX;
    mOldTileY = mY;
    mNavigateX = x;
    mNavigateY = y;
    mNavigateId = BeingId_zero;

    mNavigatePath = mMap->findPath(
        (mPixelX - mapTileSize / 2) / mapTileSize,
        (mPixelY - mapTileSize) / mapTileSize,
        x,
        y,
        getBlockWalkMask(),
        0);

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
    mNavigateId = BeingId_zero;

    mNavigatePath.clear();

    const SpecialLayer *const tmpLayer = mMap->getTempLayer();
    if (!tmpLayer)
        return;

    tmpLayer->clean();
}

void LocalPlayer::updateMusic() const
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

    // probably map not loaded.
    if (!mPixelX || !mPixelY)
        return;

    if (mX != mOldTileX || mY != mOldTileY)
    {
        if (socialWindow)
            socialWindow->updatePortals();
        if (popupManager)
            popupManager->hideBeingPopup();
        updateMusic();
    }

    if (mMap && (mX != mOldTileX || mY != mOldTileY))
    {
        SpecialLayer *const tmpLayer = mMap->getTempLayer();
        if (!tmpLayer)
            return;

        const int x = (mPixelX - mapTileSize / 2) / mapTileSize;
        const int y = (mPixelY - mapTileSize) / mapTileSize;
        if (mNavigateId != BeingId_zero)
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
            mNavigateX = being->mX;
            mNavigateY = being->mY;
        }

        if (mNavigateX == x && mNavigateY == y)
        {
            navigateClean();
            return;
        }
        else
        {
            for (Path::const_iterator i = mNavigatePath.begin(),
                 i_fend = mNavigatePath.end();
                 i != i_fend;
                 ++i)
            {
                if ((*i).x == mX && (*i).y == mY)
                {
                    mNavigatePath.pop_front();
                    fixPos();
                    break;
                }
            }

            if (mDrawPath && mShowNavigePath)
            {
                tmpLayer->clean();
                tmpLayer->addRoad(mNavigatePath);
            }
        }
    }
    mOldX = mPixelX;
    mOldY = mPixelY;
    mOldTileX = mX;
    mOldTileY = mY;
}

void LocalPlayer::targetMoved() const
{
/*
    if (mKeepAttacking)
    {
        if (mTarget && mServerAttack == Keep_true)
        {
            logger->log("LocalPlayer::targetMoved0");
            if (!PacketLimiter::limitPackets(PacketType::PACKET_ATTACK))
                return;
            logger->log("LocalPlayer::targetMoved");
            playerHandler->attack(mTarget->getId(), mServerAttack);
        }
    }
*/
}

int LocalPlayer::getPathLength(const Being *const being) const
{
    if (!mMap || !being)
        return 0;

    if (being->mX == mX && being->mY == mY)
        return 0;

    if (being->mX - 1 <= mX &&
        being->mX + 1 >= mX &&
        being->mY - 1 <= mY &&
        being->mY + 1 >= mY)
    {
        return 1;
    }

    if (mTargetOnlyReachable)
    {
        const Path debugPath = mMap->findPath(
            (mPixelX - mapTileSize / 2) / mapTileSize,
            (mPixelY - mapTileSize) / mapTileSize,
            being->mX,
            being->mY,
            getBlockWalkMask(),
            0);
        return CAST_S32(debugPath.size());
    }
    else
    {
        const int dx = CAST_S32(abs(being->mX - mX));
        const int dy = CAST_S32(abs(being->mY - mY));
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

    const bool broken = serverFeatures
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
    if (!player_imitated.empty() && being->mName == player_imitated)
        emote(action);
}

void LocalPlayer::imitateAction(const Being *const being,
                                const BeingActionT &action)
{
    if (!being)
        return;

    if (!mPlayerImitated.empty() && being->mName == mPlayerImitated)
    {
        setAction(action);
        playerHandler->changeAction(action);
    }
}

void LocalPlayer::imitateDirection(const Being *const being,
                                   const unsigned char dir)
{
    if (!being)
        return;

    if (!mPlayerImitated.empty() && being->mName == mPlayerImitated)
    {
        if (!PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
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
            playerHandler->setDirection(dir2);
        }
        else
        {
            setDirection(dir);
            playerHandler->setDirection(dir);
        }
    }
}

void LocalPlayer::imitateOutfit(const Being *const player,
                                const int sprite) const
{
    if (!player)
        return;

    if (settings.imitationMode == 1 &&
        !mPlayerImitated.empty() &&
        player->mName == mPlayerImitated)
    {
        if (sprite < 0 || sprite >= player->getNumberOfLayers())
            return;

        const AnimatedSprite *const equipmentSprite
            = dynamic_cast<const AnimatedSprite *const>(
            player->mSprites[sprite]);

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
            if (item && item->isEquipped() == Equipped_false)
                PlayerInfo::equipItem(item, Sfx_false);
        }
        else
        {
//            logger->log("have unequip %d", sprite);
            const int equipmentSlot = inventoryHandler
                ->convertFromServerSlot(sprite);
//            logger->log("equipmentSlot: " + toString(equipmentSlot));
            if (equipmentSlot == inventoryHandler->getProjectileSlot())
                return;

            const Item *const item = PlayerInfo::getEquipment(equipmentSlot);
            if (item)
            {
//                logger->log("unequiping");
                PlayerInfo::unequipItem(item, Sfx_false);
            }
        }
    }
}

void LocalPlayer::followMoveTo(const Being *const being,
                               const int x, const int y)
{
    if (being &&
        !mPlayerFollowed.empty() &&
        being->mName == mPlayerFollowed)
    {
        mPickUpTarget = nullptr;
        navigateTo(x, y);
    }
}

void LocalPlayer::followMoveTo(const Being *const being,
                               const int x1, const int y1,
                               const int x2, const int y2)
{
    if (!being)
        return;

    mPickUpTarget = nullptr;
    if (!mPlayerFollowed.empty() &&
        being->mName == mPlayerFollowed)
    {
        switch (settings.followMode)
        {
            case 0:
                navigateTo(x1, y1);
                setNextDest(x2, y2);
                break;
            case 1:
                if (x1 != x2 || y1 != y2)
                {
                    navigateTo(mX + x2 - x1, mY + y2 - y1);
                    setNextDest(mX + x2 - x1, mY + y2 - y1);
                }
                break;
            case 2:
                if (x1 != x2 || y1 != y2)
                {
                    navigateTo(mX + x1 - x2, mY + y1 - y2);
                    setNextDest(mX + x1 - x2, mY + y1 - y2);
                }
                break;
            case 3:
                if (!mTarget || mTarget->mName != mPlayerFollowed)
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

void LocalPlayer::fixPos()
{
    if (!mCrossX && !mCrossY)
        return;

    const int dx = abs(mX - mCrossX);
    const int dy = abs(mY - mCrossY);
    const int dist = dx > dy ? dx : dy;
    const int time = cur_time;
    const int maxDist = mSyncPlayerMove ? mSyncPlayerMoveDistance : 7;

    if (dist > maxDist)
    {
        mActivityTime = time;
        setTileCoords(mCrossX, mCrossY);
// alternative way to fix, move to real position
//        setDestination(mCrossX, mCrossY);
    }
}

void LocalPlayer::setRealPos(const int x, const int y)
{
    if (!mMap)
        return;

    SpecialLayer *const layer = mMap->getTempLayer();
    if (layer)
    {
        if ((mCrossX || mCrossY) &&
            layer->getTile(mCrossX, mCrossY) &&
            layer->getTile(mCrossX, mCrossY)->getType() == MapItemType::CROSS)
        {
            layer->setTile(mCrossX, mCrossY, MapItemType::EMPTY);
        }

        if (mShowServerPos)
        {
            const MapItem *const mapItem = layer->getTile(x, y);

            if (!mapItem || mapItem->getType() == MapItemType::EMPTY)
            {
                if (mX != x && mY != y)
                    layer->setTile(x, y, MapItemType::CROSS);
            }
        }

        if (mCrossX != x || mCrossY != y)
        {
            mCrossX = x;
            mCrossY = y;
            fixPos();
        }
    }
    if (mMap->isCustom())
        mMap->setWalk(x, y);
}
void LocalPlayer::fixAttackTarget()
{
    if (!mMap || !mTarget)
        return;

    if (settings.moveToTargetType == 11 || !settings.attackType
        || !config.getBoolValue("autofixPos"))
    {
        return;
    }

    const Path debugPath = mMap->findPath(
        (mPixelX - mapTileSize / 2) / mapTileSize,
        (mPixelY - mapTileSize) / mapTileSize,
        mTarget->mX,
        mTarget->mY,
        getBlockWalkMask(),
        0);

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
                    CAST_S32(pos.x), CAST_S32(pos.y));
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

    const std::string &nick = being->mName;
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
            return !(player_relations.getRelation(target->mName)
                 == Relation::FRIEND);
        case 2:
            return player_relations.checkBadRelation(target->mName);
        default:
        case 3:
            return false;
    }
}

void LocalPlayer::updateStatus() const
{
    if (serverFeatures->havePlayerStatusUpdate() && mEnableAdvert)
    {
        uint8_t status = 0;
        if (!serverFeatures->haveVending())
        {
            if (mTradebot && shopWindow && !shopWindow->isShopEmpty())
                status |= BeingFlag::SHOP;
        }
        if (settings.awayMode || settings.pseudoAwayMode)
            status |= BeingFlag::AWAY;

        if (mInactive)
            status |= BeingFlag::INACTIVE;

        playerHandler->updateStatus(status);
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

void LocalPlayer::playerDeath()
{
    if (mAction != BeingAction::DEAD)
    {
        setAction(BeingAction::DEAD, 0);
        recalcSpritesOrder();
    }
}

bool LocalPlayer::canMove() const
{
    return mAction != BeingAction::DEAD &&
        (serverFeatures->haveMoveWhileSit() ||
        mAction != BeingAction::SIT);
}
