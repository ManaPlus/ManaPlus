/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "localplayer.h"

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "effectmanager.h"
#include "flooritem.h"
#include "graphics.h"
#include "guild.h"
#include "item.h"
#include "keyboardconfig.h"
#include "logger.h"
#include "map.h"
#include "maplayer.h"
#include "party.h"
#include "particle.h"
#include "playerinfo.h"
#include "playerrelations.h"
#include "simpleanimation.h"
#include "sound.h"
#include "statuseffect.h"
#include "dropshortcut.h"

#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/ministatuswindow.h"
#include "gui/okdialog.h"
#include "gui/outfitwindow.h"
#include "gui/shopwindow.h"
#include "gui/skilldialog.h"
#include "gui/socialwindow.h"
#include "gui/theme.h"
#include "gui/userpalette.h"
#include "gui/viewport.h"

#include "gui/widgets/chattab.h"

#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/guildhandler.h"
#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"
#include "net/playerhandler.h"
#include "net/specialhandler.h"
#include "net/tradehandler.h"

#include "resources/animation.h"
#include "resources/imageset.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "mumblemanager.h"

#include "debug.h"

// This is the minimal delay between to permitted
// setDestination() calls using the keyboard.
// TODO: This can fine tuned later on when running is added...
const short walkingKeyboardDelay = 1000;
const short awayLimitTimer = 60;

LocalPlayer *player_node = nullptr;

extern std::list<BeingCacheEntry*> beingInfoCache;
extern OkDialog *weightNotice;
extern int weightNoticeTime;
extern MiniStatusWindow *miniStatusWindow;
extern SkillDialog *skillDialog;

LocalPlayer::LocalPlayer(int id, int subtype):
    Being(id, PLAYER, subtype, nullptr),
    mTargetTime(-1),
    mLastTarget(-1),
    mTarget(nullptr),
    mPlayerFollowed(""),
    mPlayerImitated(""),
    mPickUpTarget(nullptr),
    mGoingToTarget(false),
    mKeepAttacking(false),
    mLastAction(-1),
    mWalkingDir(0),
    mPathSetByMouse(false),
    mLocalWalkTime(-1),
    mMessageTime(0),
    mAwayDialog(nullptr),
    mAfkTime(0),
    mAwayMode(false),
    mPseudoAwayMode(false),
    mShowNavigePath(false),
    mDrawPath(false),
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
    mLastHitFrom(""),
    mWaitFor(""),
    mAdvertTime(0),
    mBlockAdvert(false),
    mNextStep(false)
{
    logger->log1("LocalPlayer::LocalPlayer");

    mAttackRange = 0;

    listen(CHANNEL_ATTRIBUTES);
    mLevel = 1;

    mAdvanced = true;

    mAwayListener = new AwayListener();

    mUpdateName = true;

    mTextColor = &Theme::getThemeColor(Theme::PLAYER);
    if (userPalette)
        mNameColor = &userPalette->getColor(UserPalette::SELF);
    else
        mNameColor = nullptr;

    mLastTargetX = 0;
    mLastTargetY = 0;

    mInvertDirection = 0;
    mCrazyMoveType = config.getIntValue("crazyMoveType");
    mCrazyMoveState = 0;
    mAttackWeaponType = config.getIntValue("attackWeaponType");
    mQuickDropCounter = config.getIntValue("quickDropCounter");
    mMoveState = 0;
    mDisableCrazyMove = false;
    mPickUpType = config.getIntValue("pickUpType");
    mMagicAttackType = config.getIntValue("magicAttackType");
    mPvpAttackType = config.getIntValue("pvpAttackType");
    mMoveToTargetType = config.getIntValue("moveToTargetType");
    mDisableGameModifiers = config.getBoolValue("disableGameModifiers");
    mTargetDeadPlayers = config.getBoolValue("targetDeadPlayers");
    mAttackType = config.getIntValue("attackType");
    mFollowMode = config.getIntValue("followMode");
    mImitationMode = config.getIntValue("imitationMode");
    mIsServerBuggy = serverConfig.getValueBool("enableBuggyServers", true);
    mSyncPlayerMove = config.getBoolValue("syncPlayerMove");
    mDrawPath = config.getBoolValue("drawPath");
    mServerAttack = config.getBoolValue("serverAttack");
    mAttackMoving = config.getBoolValue("attackMoving");
    mShowJobExp = config.getBoolValue("showJobExp");
    mEnableAdvert = config.getBoolValue("enableAdvert");
    mTradebot = config.getBoolValue("tradebot");
    mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");

    mPingSendTick = 0;
    mWaitPing = false;
    mPingTime = 0;

    PlayerInfo::setStatBase(WALK_SPEED, static_cast<int>(getWalkSpeed().x));
    PlayerInfo::setStatMod(WALK_SPEED, 0);

    loadHomes();

    config.addListener("showownname", this);
    config.addListener("targetDeadPlayers", this);
    serverConfig.addListener("enableBuggyServers", this);
    config.addListener("syncPlayerMove", this);
    config.addListener("drawPath", this);
    config.addListener("serverAttack", this);
    config.addListener("attackMoving", this);
    config.addListener("showJobExp", this);
    config.addListener("enableAdvert", this);
    config.addListener("tradebot", this);
    config.addListener("targetOnlyReachable", this);
    setShowName(config.getBoolValue("showownname"));
}

LocalPlayer::~LocalPlayer()
{
    logger->log1("LocalPlayer::~LocalPlayer");

    config.removeListeners(this);
    serverConfig.removeListener("enableBuggyServers", this);

    if (mAwayDialog)
    {
        sound.volumeRestore();
        delete mAwayDialog;
        mAwayDialog = nullptr;
    }
    delete mAwayListener;
    mAwayListener = nullptr;
}

void LocalPlayer::logic()
{
    if (mumbleManager)
        mumbleManager->setPos(mX, mY, mDirection);

    // Actions are allowed once per second
    if (get_elapsed_time(mLastAction) >= 1000)
        mLastAction = -1;

    if (mActivityTime == 0 || mLastAction != -1)
        mActivityTime = cur_time;

    if ((mAction != MOVE || mNextStep) && !mNavigatePath.empty())
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
            for (Path::const_iterator i = mNavigatePath.begin(),
                 i_end = mNavigatePath.end(); i != i_end; ++i)
            {
                if ((*i).x == mX && (*i).y == mY)
                {
                    mNavigatePath.pop_front();
                    break;
                }
                moveTo((*i).x, (*i).y);
                break;
            }
        }
    }

    if (weightNotice && weightNoticeTime < cur_time)
    {
        weightNotice->scheduleDelete();
        weightNotice = nullptr;
        weightNoticeTime = 0;
    }

    // Show XP messages
    if (!mMessages.empty())
    {
        if (mMessageTime == 0)
        {
            MessagePair info = mMessages.front();

            if (particleEngine)
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

    PlayerInfo::logic();

    // Targeting allowed 4 times a second
    if (get_elapsed_time(mLastTarget) >= 250)
        mLastTarget = -1;

//    // Remove target if its been on a being for more than a minute
    if (get_elapsed_time(mTargetTime) >= 60000)
    {
        mTargetTime = tick_time;
//        setTarget(nullptr);
        mLastTarget = -1;
    }

    if (mTarget)
    {
        if (mTarget->getType() == ActorSprite::NPC)
        {
            // NPCs are always in range
            mTarget->setTargetType(TCT_IN_RANGE);
        }
        else
        {
            // Find whether target is in range
            // TODO: Make this nicer, probably using getPosition() only
#ifdef MANASERV_SUPPORT
            const int rangeX =
                (Net::getNetworkType() == ServerInfo::MANASERV) ?
                static_cast<int>(abs(static_cast<int>(mTarget->getPosition().x
                - getPosition().x))) :
                static_cast<int>(abs(mTarget->getTileX() - getTileX()));
            const int rangeY =
                (Net::getNetworkType() == ServerInfo::MANASERV) ?
                static_cast<int>(abs(static_cast<int>(mTarget->getPosition().y
                - getPosition().y))) :
                static_cast<int>(abs(mTarget->getTileY() - getTileY()));
#else
            const int rangeX = static_cast<int>(
                abs(mTarget->getTileX() - getTileX()));
            const int rangeY = static_cast<int>(
                abs(mTarget->getTileY() - getTileY()));
#endif
            const int attackRange = getAttackRange();
            const TargetCursorType targetType = rangeX > attackRange ||
                                                rangeY > attackRange ?
                                                TCT_NORMAL : TCT_IN_RANGE;
            mTarget->setTargetType(targetType);

            if (!mTarget->isAlive() && (!mTargetDeadPlayers
                || mTarget->getType() != Being::PLAYER))
            {
                stopAttack();
            }

            if (mKeepAttacking && mTarget)
                attack(mTarget, true);
        }
    }

    if (serverVersion < 4 && mEnableAdvert && !mBlockAdvert
        && mAdvertTime < cur_time)
    {
        Uint8 smile = FLAG_SPECIAL;
        if (mTradebot && shopWindow && !shopWindow->isShopEmpty())
            smile += FLAG_SHOP;

        if (mAwayMode || mPseudoAwayMode)
            smile += FLAG_AWAY;

        if (mInactive)
            smile += FLAG_INACTIVE;

        if (emote(smile))
            mAdvertTime = cur_time + 60;
        else
            mAdvertTime = cur_time + 30;
    }
    Being::logic();
}

void LocalPlayer::setAction(Action action, int attackType)
{
    if (action == DEAD)
    {
        mLastTarget = -1;
        if (!mLastHitFrom.empty())
        {
            debugMsg(strprintf(_("You were killed by %s"),
                mLastHitFrom.c_str()));
            mLastHitFrom = "";
        }
        setTarget(nullptr);
    }

    Being::setAction(action, attackType);
    if (mumbleManager)
        mumbleManager->setAction(static_cast<int>(action));
}

void LocalPlayer::setGMLevel(int level)
{
    mGMLevel = level;

    if (level > 0)
    {
        setGM(true);
        if (chatWindow)
            chatWindow->loadGMCommands();
    }
}


Position LocalPlayer::getNextWalkPosition(unsigned char dir)
{
    // Compute where the next tile will be set.
    int dx = 0, dy = 0;
    if (dir & Being::UP)
        dy--;
    if (dir & Being::DOWN)
        dy++;
    if (dir & Being::LEFT)
        dx--;
    if (dir & Being::RIGHT)
        dx++;

    Vector pos = getPosition();

    // If no map or no direction is given, give back the current player position
    if (!mMap || (!dx && !dy))
        return Position(static_cast<int>(pos.x), static_cast<int>(pos.y));

    // Get the current tile pos and its offset
    int tileX = static_cast<int>(pos.x) / mMap->getTileWidth();
    int tileY = static_cast<int>(pos.y) / mMap->getTileHeight();
    int offsetX = static_cast<int>(pos.x) % mMap->getTileWidth();
    int offsetY = static_cast<int>(pos.y) % mMap->getTileHeight();

    // Get the walkability of every surrounding tiles.
    bool wTopLeft = mMap->getWalk(tileX - 1, tileY - 1, getWalkMask());
    bool wTop = mMap->getWalk(tileX, tileY - 1, getWalkMask());
    bool wTopRight = mMap->getWalk(tileX + 1, tileY - 1, getWalkMask());
    bool wLeft = mMap->getWalk(tileX - 1, tileY, getWalkMask());
    bool wRight = mMap->getWalk(tileX + 1, tileY, getWalkMask());
    bool wBottomLeft = mMap->getWalk(tileX - 1, tileY + 1, getWalkMask());
    bool wBottom = mMap->getWalk(tileX, tileY + 1, getWalkMask());
    bool wBottomRight = mMap->getWalk(tileX + 1, tileY + 1, getWalkMask());

    // Make diagonals unwalkable when both straight directions are blocking
    if (!wTop)
    {
        if (!wRight)
            wTopRight = false;
        if (!wLeft)
            wTopLeft = false;
    }
    if (!wBottom)
    {
        if (!wRight)
            wBottomRight = false;
        if (!wLeft)
            wBottomLeft = false;
    }

    // We'll make tests for each desired direction

    // Handle diagonal cases by setting the way back to a straight direction
    // when necessary.
    if (dx && dy)
    {
        // Going top-right
        if (dx > 0 && dy < 0)
        {
            if (!wTopRight)
            {
                // Choose a straight direction when diagonal target is blocked
                if (!wTop && wRight)
                {
                    dy = 0;
                }
                else if (wTop && !wRight)
                {
                    dx = 0;
                }
                else if (!wTop && !wRight)
                {
                    return Position(tileX * 32 + 32 - getCollisionRadius(),
                                    tileY * 32 + getCollisionRadius());
                }
                else // Both straight direction are walkable
                {
                    // Go right when below the corner
                    if (offsetY >= (offsetX / mMap->getTileHeight()
                        - (offsetX / mMap->getTileWidth()
                        * mMap->getTileHeight()) ))
                    {
                        dy = 0;
                    }
                    else // Go up otherwise
                    {
                        dx = 0;
                    }
                }
            }
            else // The diagonal is walkable
            {
                return mMap->checkNodeOffsets(getCollisionRadius(),
                        getWalkMask(), Position(static_cast<int>(pos.x) + 32,
                                                static_cast<int>(pos.y) - 32));
            }
        }

        // Going top-left
        if (dx < 0 && dy < 0)
        {
            if (!wTopLeft)
            {
                // Choose a straight direction when diagonal target is blocked
                if (!wTop && wLeft)
                {
                    dy = 0;
                }
                else if (wTop && !wLeft)
                {
                    dx = 0;
                }
                else if (!wTop && !wLeft)
                {
                    return Position(tileX * 32 + getCollisionRadius(),
                                    tileY * 32 + getCollisionRadius());
                }
                else // Both straight direction are walkable
                {
                    // Go left when below the corner
                    if (offsetY >= (offsetX / mMap->getTileWidth()
                        * mMap->getTileHeight()))
                    {
                        dy = 0;
                    }
                    else // Go up otherwise
                    {
                        dx = 0;
                    }
                }
            }
            else // The diagonal is walkable
            {
                return mMap->checkNodeOffsets(getCollisionRadius(),
                        getWalkMask(), Position(static_cast<int>(pos.x) - 32,
                                                static_cast<int>(pos.y) - 32));
            }
        }

        // Going bottom-left
        if (dx < 0 && dy > 0)
        {
            if (!wBottomLeft)
            {
                // Choose a straight direction when diagonal target is blocked
                if (!wBottom && wLeft)
                {
                    dy = 0;
                }
                else if (wBottom && !wLeft)
                {
                    dx = 0;
                }
                else if (!wBottom && !wLeft)
                {
                    return Position(tileX * 32 + getCollisionRadius(),
                                    tileY * 32 + 32 - getCollisionRadius());
                }
                else // Both straight direction are walkable
                {
                    // Go down when below the corner
                    if (offsetY >= (offsetX / mMap->getTileHeight()
                        - (offsetX / mMap->getTileWidth()
                        * mMap->getTileHeight())))
                    {
                        dx = 0;
                    }
                    else // Go left otherwise
                    {
                        dy = 0;
                    }
                }
            }
            else // The diagonal is walkable
            {
                return mMap->checkNodeOffsets(getCollisionRadius(),
                        getWalkMask(), Position(static_cast<int>(pos.x) - 32,
                                                static_cast<int>(pos.y) + 32));
            }
        }

        // Going bottom-right
        if (dx > 0 && dy > 0)
        {
            if (!wBottomRight)
            {
                // Choose a straight direction when diagonal target is blocked
                if (!wBottom && wRight)
                {
                    dy = 0;
                }
                else if (wBottom && !wRight)
                {
                    dx = 0;
                }
                else if (!wBottom && !wRight)
                {
                    return Position(tileX * 32 + 32 - getCollisionRadius(),
                                    tileY * 32 + 32 - getCollisionRadius());
                }
                else // Both straight direction are walkable
                {
                    // Go down when below the corner
                    if (offsetY >= (offsetX / mMap->getTileWidth()
                        * mMap->getTileHeight()))
                    {
                        dx = 0;
                    }
                    else // Go right otherwise
                    {
                        dy = 0;
                    }
                }
            }
            else // The diagonal is walkable
            {
                return mMap->checkNodeOffsets(getCollisionRadius(),
                        getWalkMask(), Position(static_cast<int>(pos.x) + 32,
                                                static_cast<int>(pos.y) + 32));
            }
        }

    } // End of diagonal cases

    // Straight directions
    // Right direction
    if (dx > 0 && !dy)
    {
        // If the straight destination is blocked,
        // Make the player go the closest possible.
        if (!wRight)
        {
            return Position(tileX * 32 + 32 - getCollisionRadius(),
                            static_cast<int>(pos.y));
        }
        else
        {
            if (!wTopRight)
            {
                // If we're going to collide with the top-right corner
                if (offsetY - getCollisionRadius() < 0)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + 32 - getCollisionRadius(),
                                    tileY * 32 + getCollisionRadius());

                }
            }

            if (!wBottomRight)
            {
                // If we're going to collide with the bottom-right corner
                if (offsetY + getCollisionRadius() > 32)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + 32 - getCollisionRadius(),
                                    tileY * 32 + 32 - getCollisionRadius());

                }
            }
            // If the way is clear, step up one checked tile ahead.
            return mMap->checkNodeOffsets(getCollisionRadius(), getWalkMask(),
                    Position(static_cast<int>(pos.x) + 32,
                             static_cast<int>(pos.y)));
        }
    }

    // Left direction
    if (dx < 0 && !dy)
    {
        // If the straight destination is blocked,
        // Make the player go the closest possible.
        if (!wLeft)
        {
            return Position(tileX * 32 + getCollisionRadius(),
                            static_cast<int>(pos.y));
        }
        else
        {
            if (!wTopLeft)
            {
                // If we're going to collide with the top-left corner
                if (offsetY - getCollisionRadius() < 0)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + getCollisionRadius(),
                                    tileY * 32 + getCollisionRadius());

                }
            }

            if (!wBottomLeft)
            {
                // If we're going to collide with the bottom-left corner
                if (offsetY + getCollisionRadius() > 32)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + getCollisionRadius(),
                                    tileY * 32 + 32 - getCollisionRadius());

                }
            }
            // If the way is clear, step up one checked tile ahead.
            return mMap->checkNodeOffsets(getCollisionRadius(), getWalkMask(),
                    Position(static_cast<int>(pos.x) - 32,
                             static_cast<int>(pos.y)));
        }
    }

    // Up direction
    if (!dx && dy < 0)
    {
        // If the straight destination is blocked,
        // Make the player go the closest possible.
        if (!wTop)
        {
            return Position(static_cast<int>(pos.x),
                            tileY * 32 + getCollisionRadius());
        }
        else
        {
            if (!wTopLeft)
            {
                // If we're going to collide with the top-left corner
                if (offsetX - getCollisionRadius() < 0)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + getCollisionRadius(),
                                    tileY * 32 + getCollisionRadius());

                }
            }

            if (!wTopRight)
            {
                // If we're going to collide with the top-right corner
                if (offsetX + getCollisionRadius() > 32)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + 32 - getCollisionRadius(),
                                    tileY * 32 + getCollisionRadius());

                }
            }
            // If the way is clear, step up one checked tile ahead.
            return mMap->checkNodeOffsets(getCollisionRadius(), getWalkMask(),
                    Position(static_cast<int>(pos.x),
                             static_cast<int>(pos.y) - 32));
        }
    }

    // Down direction
    if (!dx && dy > 0)
    {
        // If the straight destination is blocked,
        // Make the player go the closest possible.
        if (!wBottom)
        {
            return Position(static_cast<int>(pos.x),
                            tileY * 32 + 32 - getCollisionRadius());
        }
        else
        {
            if (!wBottomLeft)
            {
                // If we're going to collide with the bottom-left corner
                if (offsetX - getCollisionRadius() < 0)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + getCollisionRadius(),
                                    tileY * 32 + 32 - getCollisionRadius());

                }
            }

            if (!wBottomRight)
            {
                // If we're going to collide with the bottom-right corner
                if (offsetX + getCollisionRadius() > 32)
                {
                    // We make the player corrects its offset
                    // before going further
                    return Position(tileX * 32 + 32 - getCollisionRadius(),
                                    tileY * 32 + 32 - getCollisionRadius());

                }
            }
            // If the way is clear, step up one checked tile ahead.
            return mMap->checkNodeOffsets(getCollisionRadius(), getWalkMask(),
                    Position(static_cast<int>(pos.x),
                             static_cast<int>(pos.y) + 32));
        }
    }

    // Return the current position if everything else has failed.
    return Position(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void LocalPlayer::nextTile(unsigned char dir A_UNUSED = 0)
{
#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        if (Party::getParty(1))
        {
            PartyMember *pm = Party::getParty(1)->getMember(getName());
            if (pm)
            {
                pm->setX(mX);
                pm->setY(mY);
            }
        }

        // TODO: Fix picking up when reaching target (this method is obsolete)
        // TODO: Fix holding walking button to keep walking smoothly
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

        // TODO: Fix automatically walking within range of target, when wanted
        if (mGoingToTarget && mTarget && withinAttackRange(mTarget))
        {
            mAction = Being::STAND;
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
            if (mNavigatePath.empty() || mAction != MOVE)
                setAction(STAND);
            else
                mNextStep = true;
        }
        else
        {
            Being::nextTile();
        }
    }
#ifdef MANASERV_SUPPORT
    else
    {
        if (!mMap || !dir)
            return;

        const Vector &pos = getPosition();
        Position destination = getNextWalkPosition(dir);

        if (static_cast<int>(pos.x) != destination.x
            || static_cast<int>(pos.y) != destination.y)
        {
            setDestination(destination.x, destination.y);
        }
        else if (dir != mDirection)
        {
            // If the being can't move, just change direction

            if (!Client::limitPackets(PACKET_DIRECTION))
                return;

            Net::getPlayerHandler()->setDirection(dir);
            setDirection(dir);
        }
    }
#endif
}

bool LocalPlayer::checkInviteRights(const std::string &guildName)
{
    Guild *guild = getGuild(guildName);
    if (guild)
        return guild->getInviteRights();

    return false;
}

void LocalPlayer::inviteToGuild(Being *being)
{
    if (!being || being->getType() != PLAYER)
        return;

    // TODO: Allow user to choose which guild to invite being to
    // For now, just invite to the first guild you have permissions to invite with
    std::map<int, Guild*>::const_iterator itr = mGuilds.begin();
    std::map<int, Guild*>::const_iterator itr_end = mGuilds.end();
    for (; itr != itr_end; ++itr)
    {
        if (itr->second && checkInviteRights(itr->second->getName()))
        {
            Net::getGuildHandler()->invite(itr->second->getId(), being);
            return;
        }
    }
}

bool LocalPlayer::pickUp(FloorItem *item)
{
    if (!item)
        return false;

    if (!Client::limitPackets(PACKET_PICKUP))
        return false;

    int dx = item->getTileX() - mX;
    int dy = item->getTileY() - mY;
    int dist = 6;

    if (mPickUpType >= 4 && mPickUpType <= 6)
        dist = 4;

    if (dx * dx + dy * dy < dist)
    {
        Net::getPlayerHandler()->pickUp(item);
        mPickUpTarget = nullptr;
    }
    else if (mPickUpType >= 4 && mPickUpType <= 6)
    {
#ifdef MANASERV_SUPPORT
        if (Net::getNetworkType() == ServerInfo::MANASERV)
        {
            setDestination(item->getPixelX() + 16, item->getPixelY() + 16);
            mPickUpTarget = item;
            mPickUpTarget->addActorSpriteListener(this);
        }
        else
#endif
        {
            const Vector &playerPos = getPosition();
            Path debugPath = mMap->findPath(
                    static_cast<int>(playerPos.x - 16) / 32,
                    static_cast<int>(playerPos.y - 32) / 32,
                    item->getTileX(), item->getTileY(), getWalkMask(), 0);
            if (!debugPath.empty())
                navigateTo(item->getTileX(), item->getTileY());
            else
                setDestination(item->getTileX(), item->getTileY());

            mPickUpTarget = item;
            mPickUpTarget->addActorSpriteListener(this);
        }
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

void LocalPlayer::setTarget(Being *target)
{
    if ((mLastTarget != -1 || target == this) && target)
        return;

    if (target)
        mLastTarget = tick_time;

    if (target == mTarget)
        return;

    if (target || mAction == ATTACK)
    {
        mTargetTime = tick_time;
    }
    else
    {
        mKeepAttacking = false;
        mTargetTime = -1;
    }

    Being *oldTarget = nullptr;
    if (mTarget)
    {
        mTarget->untarget();
        oldTarget = mTarget;
    }

    if (mTarget && mTarget->getType() == ActorSprite::MONSTER)
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

    if (target && target->getType() == ActorSprite::MONSTER)
        target->setShowName(true);
}

void LocalPlayer::setDestination(int x, int y)
{
    mActivityTime = cur_time;

    if (getAttackType() == 0 || !mAttackMoving)
        mKeepAttacking = false;

    // Only send a new message to the server when destination changes
    if (x != mDest.x || y != mDest.y)
    {
        if (mInvertDirection != 1)
        {
            Net::getPlayerHandler()->setDestination(x, y, mDirection);
            Being::setDestination(x, y);
        }
        else if (mInvertDirection == 1)
        {
            Uint8 newDir = 0;
            if (mDirection & UP)
                newDir |= DOWN;
            if (mDirection & LEFT)
                newDir |= RIGHT;
            if (mDirection & DOWN)
                newDir |= UP;
            if (mDirection & RIGHT)
                newDir |= LEFT;

            Net::getPlayerHandler()->setDestination(x, y, newDir);

            if (Client::limitPackets(PACKET_DIRECTION))
            {
                setDirection(newDir);
                Net::getPlayerHandler()->setDirection(newDir);
            }

            Being::setDestination(x, y);
        }
        else
        {
#ifdef MANASERV_SUPPORT
            // Manaserv:
            // If the destination given to being class is accepted,
            // we inform the Server.
            if ((x == mDest.x && y == mDest.y)
                || Net::getNetworkType() != ServerInfo::MANASERV)
#endif
            {
                Net::getPlayerHandler()->setDestination(x, y, mDirection);
            }
        }
    }
}

void LocalPlayer::setWalkingDir(unsigned char dir)
{
    // This function is called by Game::handleInput()

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        // First if player is pressing key for the direction he is already
        // going, do nothing more...

        // Else if he is pressing a key, and its different from what he has
        // been pressing, stop (do not send this stop to the server) and
        // start in the new direction
        if (dir && (dir != getWalkingDir()))
            stopWalking(false);

        // Else, he is not pressing a key,
        // and the current path hasn't been sent by mouse,
        // then, stop (sending to server).
        else if (!dir)
        {
            if (!mPathSetByMouse)
                stopWalking(true);
            return;
        }

        // If the delay to send another walk message to the server hasn't expired,
        // don't do anything or we could get disconnected for spamming the server
        if (get_elapsed_time(mLocalWalkTime) < walkingKeyboardDelay)
            return;
    }
#endif

    mWalkingDir = dir;

    // If we're not already walking, start walking.
    if (mAction != MOVE && dir)
    {
        startWalking(dir);
    }
#ifdef MANASERV_SUPPORT
    else if (mAction == MOVE && (Net::getNetworkType()
             == ServerInfo::MANASERV))
    {
        nextTile(dir);
    }
#endif
}

void LocalPlayer::startWalking(unsigned char dir)
{
    // This function is called by setWalkingDir(),
    // but also by nextTile() for TMW-Athena...
    if (!mMap || !dir)
        return;

    mPickUpTarget = nullptr;
    if (mAction == MOVE && !mPath.empty())
    {
        // Just finish the current action, otherwise we get out of sync
#ifdef MANASERV_SUPPORT
        if (Net::getNetworkType() == ServerInfo::MANASERV)
        {
            const Vector &pos = getPosition();
            Being::setDestination(static_cast<int>(pos.x),
                                  static_cast<int>(pos.y));
        }
        else
#endif
        {
            Being::setDestination(mX, mY);
        }
        return;
    }

    int dx = 0, dy = 0;
    if (dir & UP)
        dy--;
    if (dir & DOWN)
        dy++;
    if (dir & LEFT)
        dx--;
    if (dir & RIGHT)
        dx++;

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() != ServerInfo::MANASERV)
#endif
    {
        // Prevent skipping corners over colliding tiles
        if (dx && !mMap->getWalk(mX + dx, mY, getWalkMask()))
            dx = 0;
        if (dy && !mMap->getWalk(mX, mY + dy, getWalkMask()))
            dy = 0;

        // Choose a straight direction when diagonal target is blocked
        if (dx && dy && !mMap->getWalk(mX + dx, mY + dy, getWalkMask()))
            dx = 0;

        // Walk to where the player can actually go
        if ((dx || dy) && mMap->getWalk(mX + dx, mY + dy, getWalkMask()))
        {
            setDestination(mX + dx, mY + dy);
        }
        else if (dir != mDirection)
        {
            // If the being can't move, just change direction

            if (Client::limitPackets(PACKET_DIRECTION))
            {
                Net::getPlayerHandler()->setDirection(dir);
                setDirection(dir);
            }
        }
    }
#ifdef MANASERV_SUPPORT
    else
    {
        nextTile(dir);
    }
#endif
}

void LocalPlayer::stopWalking(bool sendToServer)
{
    if (mAction == MOVE && mWalkingDir)
    {
        mWalkingDir = 0;
        mLocalWalkTime = 0;
        mPickUpTarget = nullptr;

        setDestination(static_cast<int>(getPosition().x),
                       static_cast<int>(getPosition().y));
        if (sendToServer)
        {
            Net::getPlayerHandler()->setDestination(
                    static_cast<int>(getPosition().x),
                    static_cast<int>(getPosition().y));
        }
        setAction(STAND);
    }

    // No path set anymore, so we reset the path by mouse flag
    mPathSetByMouse = false;

    clearPath();
    navigateClean();
}

bool LocalPlayer::toggleSit()
{
    if (!Client::limitPackets(PACKET_SIT))
        return false;

    Being::Action newAction;
    switch (mAction)
    {
        case STAND:
        case SPAWN:
            newAction = SIT;
            break;
        case SIT:
            newAction = STAND;
            break;
        case MOVE:
        case ATTACK:
        case DEAD:
        case HURT:
        default:
            return true;
    }

    Net::getPlayerHandler()->changeAction(newAction);
    return true;
}

bool LocalPlayer::updateSit()
{
    if (!Client::limitPackets(PACKET_SIT))
        return false;

    Net::getPlayerHandler()->changeAction(mAction);
    return true;
}

bool LocalPlayer::emote(Uint8 emotion)
{
    if (!Client::limitPackets(PACKET_EMOTE))
        return false;

    Net::getPlayerHandler()->emote(emotion);
    return true;
}

void LocalPlayer::attack(Being *target, bool keep, bool dontChangeEquipment)
{
#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        if (mLastAction != -1)
            return;

        // Can only attack when standing still
        if (mAction != STAND && mAction != ATTACK)
            return;
    }
#endif

    mKeepAttacking = keep;

    if (!target || target->getType() == ActorSprite::NPC)
        return;

    if (mTarget != target || !mTarget)
    {
        mLastTarget = -1;
        setTarget(target);
    }

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        Vector plaPos = this->getPosition();
        Vector tarPos = mTarget->getPosition();
        int dist_x = static_cast<int>(plaPos.x - tarPos.x);
        int dist_y = static_cast<int>(plaPos.y - tarPos.y);

        if (abs(dist_y) >= abs(dist_x))
        {
            if (dist_y < 0)
                setDirection(DOWN);
            else
                setDirection(UP);
        }
        else
        {
            if (dist_x < 0)
                setDirection(RIGHT);
            else
                setDirection(LEFT);
        }

        mLastAction = tick_time;
    }
    else
#endif
    {
        int dist_x = target->getTileX() - mX;
        int dist_y = target->getTileY() - mY;

        // Must be standing or sitting to attack
        if (mAction != STAND && mAction != SIT)
            return;

        if (abs(dist_y) >= abs(dist_x))
        {
            if (dist_y > 0)
                setDirection(DOWN);
            else
                setDirection(UP);
        }
        else
        {
            if (dist_x > 0)
                setDirection(RIGHT);
            else
                setDirection(LEFT);
        }

        mActionTime = tick_time;
        mTargetTime = tick_time;
    }

    if (target->getType() != Being::PLAYER || checAttackPermissions(target))
    {
        setAction(ATTACK);

        if (mEquippedWeapon)
        {
            std::string soundFile = mEquippedWeapon->getSound(
                EQUIP_EVENT_STRIKE);
            if (!soundFile.empty())
                sound.playSfx(soundFile);
        }
        else
        {
            sound.playSfx(paths.getValue("attackSfxFile", "fist-swish.ogg"));
        }

        if (!Client::limitPackets(PACKET_ATTACK))
            return;

        if (!dontChangeEquipment && target)
            changeEquipmentBeforeAttack(target);

        Net::getPlayerHandler()->attack(target->getId(), mServerAttack);
    }

#ifdef MANASERV_SUPPORT
    if ((Net::getNetworkType() != ServerInfo::MANASERV) && !keep)
#else
    if (!keep)
#endif
        stopAttack();
}

void LocalPlayer::stopAttack()
{
    if (!Client::limitPackets(PACKET_STOPATTACK))
        return;

    if (mServerAttack && mAction == ATTACK)
        Net::getPlayerHandler()->stopAttack();

    untarget();
}

void LocalPlayer::untarget()
{
    if (mAction == ATTACK)
        setAction(STAND);

    if (mTarget)
        setTarget(nullptr);

    mKeepAttacking = false;
    mLastTarget = -1;
}

void LocalPlayer::pickedUp(const ItemInfo &itemInfo, int amount,
                           unsigned char color, int floorItemId,
                           unsigned char fail)
{
    if (fail)
    {
        if (actorSpriteManager && floorItemId)
        {
            FloorItem *item = actorSpriteManager->findItem(floorItemId);
            if (item)
            {
                if (!item->getShowMsg())
                    return;
                item->setShowMsg(false);
            }
        }
        const char* msg;
        switch (fail)
        {
            case PICKUP_BAD_ITEM:
                msg = N_("Tried to pick up nonexistent item.");
                break;
            case PICKUP_TOO_HEAVY:
                msg = N_("Item is too heavy.");
                break;
            case PICKUP_TOO_FAR:
                msg = N_("Item is too far away.");
                break;
            case PICKUP_INV_FULL:
                msg = N_("Inventory is full.");
                break;
            case PICKUP_STACK_FULL:
                msg = N_("Stack is too big.");
                break;
            case PICKUP_DROP_STEAL:
                msg = N_("Item belongs to someone else.");
                break;
            default:
                msg = N_("Unknown problem picking up item.");
                break;
        }
        if (localChatTab && config.getBoolValue("showpickupchat"))
            localChatTab->chatLog(_(msg), BY_SERVER);

        if (mMap && config.getBoolValue("showpickupparticle"))
        {
            // Show pickup notification
            addMessageToQueue(_(msg), UserPalette::PICKUP_INFO);
        }
    }
    else
    {
        std::string str;
        if (serverVersion > 0)
            str = itemInfo.getName(color);
        else
            str = itemInfo.getName();

        if (config.getBoolValue("showpickupchat") && localChatTab)
        {
            // TRANSLATORS: %d is number,
            // [@@%d|%s@@] - here player can see link to item
            localChatTab->chatLog(strprintf(ngettext("You picked up %d "
                "[@@%d|%s@@].", "You picked up %d [@@%d|%s@@].", amount),
                amount, itemInfo.getId(), str.c_str()), BY_SERVER);
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

int LocalPlayer::getAttackRange()
{
    if (mAttackRange > -1)
    {
        return mAttackRange;
    }
    else
    {
        // TODO: Fix this to be more generic
        Item *weapon = PlayerInfo::getEquipment(EQUIP_FIGHT1_SLOT);
        if (weapon)
        {
            const ItemInfo &info = weapon->getInfo();
            return info.getAttackRange();
        }
        return 48; // unarmed range
    }
}

bool LocalPlayer::withinAttackRange(Being *target, bool fixDistance,
                                    int addRange)
{
    if (!target)
        return false;

    int range = getAttackRange() + addRange;
    int dx;
    int dy;

    if (fixDistance && range == 1)
        range = 2;

#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        const Vector &targetPos = target->getPosition();
        const Vector &pos = getPosition();
        dx = static_cast<int>(abs(static_cast<int>(targetPos.x - pos.x)));
        dy = static_cast<int>(abs(static_cast<int>(targetPos.y - pos.y)));

    }
    else
#endif
    {
        dx = static_cast<int>(abs(target->getTileX() - mX));
        dy = static_cast<int>(abs(target->getTileY() - mY));
    }
    return !(dx > range || dy > range);
}

void LocalPlayer::setGotoTarget(Being *target)
{
    mLastTarget = -1;

    if (!target)
        return;

    mPickUpTarget = nullptr;
#ifdef MANASERV_SUPPORT
    if (Net::getNetworkType() == ServerInfo::MANASERV)
    {
        mTarget = target;
        mGoingToTarget = true;
        const Vector &targetPos = target->getPosition();
        setDestination(static_cast<int>(targetPos.x),
            static_cast<int>(targetPos.y));
    }
    else
#endif
    {
        setTarget(target);
        mGoingToTarget = true;
        setDestination(target->getTileX(), target->getTileY());
    }
}

void LocalPlayer::handleStatusEffect(StatusEffect *effect, int effectId)
{
    Being::handleStatusEffect(effect, effectId);

    if (effect)
    {
        effect->deliverMessage();
        effect->playSFX();

        AnimatedSprite *sprite = effect->getIcon();

        if (!sprite)
        {
            // delete sprite, if necessary
            for (unsigned int i = 0; i < mStatusEffectIcons.size(); )
            {
                if (mStatusEffectIcons[i] == effectId)
                {
                    mStatusEffectIcons.erase(mStatusEffectIcons.begin() + i);
                    if (miniStatusWindow)
                        miniStatusWindow->eraseIcon(i);
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

            for (unsigned int i = 0; i < mStatusEffectIcons.size(); i++)
            {
                if (mStatusEffectIcons[i] == effectId)
                {
                    if (miniStatusWindow)
                        miniStatusWindow->setIcon(i, sprite);
                    found = true;
                    break;
                }
            }

            if (!found)
            { // add new
                int offset = static_cast<int>(mStatusEffectIcons.size());
                if (miniStatusWindow)
                    miniStatusWindow->setIcon(offset, sprite);
                mStatusEffectIcons.push_back(effectId);
            }
        }
    }
}

void LocalPlayer::addMessageToQueue(const std::string &message, int color)
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
    else if (value == "showJobExp")
        mShowJobExp = config.getBoolValue("showJobExp");
    else if (value == "enableAdvert")
        mEnableAdvert = config.getBoolValue("enableAdvert");
    else if (value == "tradebot")
        mTradebot = config.getBoolValue("tradebot");
    else if (value == "targetOnlyReachable")
        mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
}

void LocalPlayer::processEvent(Channels channel,
                               const Event &event)
{
    if (channel == CHANNEL_ATTRIBUTES)
    {
        if (event.getName() == EVENT_UPDATEATTRIBUTE)
        {
            switch (event.getInt("id"))
            {
                case EXP:
                {
                    if (event.getInt("oldValue") > event.getInt("newValue"))
                        break;

                    int change = event.getInt("newValue")
                                 - event.getInt("oldValue");

                    if (change != 0)
                        addMessageToQueue(strprintf("%d %s", change, _("xp")));
                    break;
                }
                case LEVEL:
                    mLevel = event.getInt("newValue");
                    break;
                default:
                    break;
            };
        }
        else if (event.getName() == EVENT_UPDATESTAT)
        {
            if (!mShowJobExp)
                return;

            int id = event.getInt("id");
            if (id == Net::getPlayerHandler()->getJobLocation())
            {
                std::pair<int, int> exp
                    = PlayerInfo::getStatExperience(id);
                if (event.getInt("oldValue1") > exp.first
                    || !event.getInt("oldValue2"))
                {
                    return;
                }

                int change = exp.first - event.getInt("oldValue1");
                if (change != 0 && mMessages.size() < 20)
                {
                    if (!mMessages.empty())
                    {
                        MessagePair pair = mMessages.back();
                        // TRANSLATORS: this is normal experience
                        if (pair.first.find(strprintf(" %s",
                            _("xp"))) == pair.first.size()
                            - strlen(_("xp")) - 1)
                        {
                            mMessages.pop_back();
                            // TRANSLATORS: this is job experience
                            pair.first += strprintf (", %d %s",
                                change, _("job"));
                            mMessages.push_back(pair);
                        }
                        else
                        {
                            // TRANSLATORS: this is job experience
                            addMessageToQueue(strprintf("%d %s",
                                change, _("job")));
                        }
                    }
                    else
                    {
                        // TRANSLATORS: this is job experience
                        addMessageToQueue(strprintf(
                            "%d %s", change, _("job")));
                    }
                }
            }
        }
    }
}

void LocalPlayer::moveTo(int x, int y)
{
    setDestination(x, y);
}

void LocalPlayer::move(int dX, int dY)
{
    mPickUpTarget = nullptr;
    moveTo(mX + dX, mY + dY);
}

void LocalPlayer::moveToTarget(unsigned int dist)
{
    bool gotPos(false);
    Path debugPath;

    Vector targetPos(-1, -1);
    const Vector &playerPos = getPosition();
    unsigned int limit(0);

    if (static_cast<int>(dist) == -1)
    {
        dist = mMoveToTargetType;
        if (mMoveToTargetType == 0)
        {
            dist = 0;
        }
        else
        {
            switch (mMoveToTargetType)
            {
                case 1:
                    dist = 1;
                    break;
                case 2:
                    dist = 2;
                    break;
                case 3:
                    dist = 3;
                    break;
                case 4:
                    dist = 5;
                    break;
                case 5:
                    dist = 7;
                    break;
                case 6:
                case 7:
                    dist = mAttackRange;
                    if (dist == 1 && serverVersion < 1)
                        dist = 2;
                default:
                    break;
            }
        }
    }

    if (mTarget)
    {
        if (mMap)
        {
            debugPath = mMap->findPath(static_cast<int>(playerPos.x - 16) / 32,
                static_cast<int>(playerPos.y - 32) / 32,
                mTarget->getTileX(), mTarget->getTileY(), getWalkMask(), 0);
        }

        if (debugPath.size() < dist)
            return;
        limit = static_cast<int>(debugPath.size()) - dist;
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
                navigateTo(mTarget);
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
        moveTo(mCrossX, mCrossY);
    }
    else if (mMap)
    {
        std::map<std::string, Vector>::const_iterator iter =
            mHomes.find(mMap->getProperty("_realfilename"));

        if (iter != mHomes.end())
        {
            Vector pos = mHomes[(*iter).first];
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

static const unsigned invertDirectionSize = 5;

void LocalPlayer::changeMode(unsigned *var, unsigned limit, const char *conf,
                             std::string (LocalPlayer::*func)(), unsigned def,
                             bool save)
{
    if (!var)
        return;

    (*var) ++;
    if (*var >= limit)
        *var = def;
    if (save)
        config.setValue(conf, *var);
    if (miniStatusWindow)
        miniStatusWindow->updateStatus();
    const std::string str = (this->*func)();
    if (str.size() > 4)
        debugMsg(str.substr(4));
}

void LocalPlayer::invertDirection()
{
    mMoveState = 0;
    changeMode(&mInvertDirection, invertDirectionSize, "invertMoveDirection",
        &LocalPlayer::getInvertDirectionString, 0, false);
}

static const char *invertDirectionStrings[] =
{
    N_("(D) default moves"),
    N_("(I) invert moves"),
    N_("(c) moves with some crazy moves"),
    N_("(C) moves with crazy moves"),
    N_("(d) double normal + crazy"),
    N_("(?) unknown move")
};

std::string LocalPlayer::getInvertDirectionString()
{
    return gettext(getVarItem(&invertDirectionStrings[0],
        mInvertDirection, invertDirectionSize));
}

static const unsigned crazyMoveTypeSize = 11;

void LocalPlayer::changeCrazyMoveType()
{
    mCrazyMoveState = 0;
    changeMode(&mCrazyMoveType, crazyMoveTypeSize, "crazyMoveType",
        &LocalPlayer::getCrazyMoveTypeString, 1);
}

std::string LocalPlayer::getCrazyMoveTypeString()
{
    if (mCrazyMoveType < crazyMoveTypeSize - 1)
    {
        return strprintf(_("(%d) crazy move number %d"),
            mCrazyMoveType, mCrazyMoveType);
    }
    else if (mCrazyMoveType == crazyMoveTypeSize - 1)
    {
        return _("(a) custom crazy move");
    }
    else
    {
        return _("(?) crazy move");
    }
}

static const unsigned moveToTargetTypeSize = 8;

void LocalPlayer::changeMoveToTargetType()
{
    changeMode(&mMoveToTargetType, moveToTargetTypeSize, "moveToTargetType",
        &LocalPlayer::getMoveToTargetTypeString);
}

static const char *moveToTargetTypeStrings[] =
{
    N_("(0) default moves to target"),
    N_("(1) moves to target in distance 1"),
    N_("(2) moves to target in distance 2"),
    N_("(3) moves to target in distance 3"),
    N_("(5) moves to target in distance 5"),
    N_("(7) moves to target in distance 7"),
    N_("(A) moves to target in attack range"),
    N_("(a) archer attack range"),
    N_("(?) move to target")
};

std::string LocalPlayer::getMoveToTargetTypeString()
{
    return gettext(getVarItem(&moveToTargetTypeStrings[0],
        mMoveToTargetType, moveToTargetTypeSize));
}

static const unsigned followModeSize = 4;

void LocalPlayer::changeFollowMode()
{
    changeMode(&mFollowMode, followModeSize, "followMode",
        &LocalPlayer::getFollowModeString);
}

static const char *followModeStrings[] =
{
    N_("(D) default follow"),
    N_("(R) relative follow"),
    N_("(M) mirror follow"),
    N_("(P) pet follow"),
    N_("(?) unknown follow")
};

std::string LocalPlayer::getFollowModeString()
{
    return gettext(getVarItem(&followModeStrings[0],
        mFollowMode, followModeSize));
}

const unsigned attackWeaponTypeSize = 4;

void LocalPlayer::changeAttackWeaponType()
{
    changeMode(&mAttackWeaponType, attackWeaponTypeSize, "attackWeaponType",
        &LocalPlayer::getAttackWeaponTypeString, 1);
}

static const char *attackWeaponTypeStrings[] =
{
    N_("(?) attack"),
    N_("(D) default attack"),
    N_("(s) switch attack without shield"),
    N_("(S) switch attack with shield"),
    N_("(?) attack")
};

std::string LocalPlayer::getAttackWeaponTypeString()
{
    return gettext(getVarItem(&attackWeaponTypeStrings[0],
        mAttackWeaponType, attackWeaponTypeSize));
}

const unsigned attackTypeSize = 4;

void LocalPlayer::changeAttackType()
{
    changeMode(&mAttackType, attackTypeSize, "attackType",
        &LocalPlayer::getAttackTypeString);
}

static const char *attackTypeStrings[] =
{
    N_("(D) default attack"),
    N_("(G) go and attack"),
    N_("(A) go, attack, pickup"),
    N_("(d) without auto attack"),
    N_("(?) attack")
};

std::string LocalPlayer::getAttackTypeString()
{
    return gettext(getVarItem(&attackTypeStrings[0],
        mAttackType, attackTypeSize));
}

const unsigned quickDropCounterSize = 31;

void LocalPlayer::changeQuickDropCounter()
{
    changeMode(&mQuickDropCounter, quickDropCounterSize, "quickDropCounter",
        &LocalPlayer::getQuickDropCounterString, 1);
}

std::string LocalPlayer::getQuickDropCounterString()
{
    if (mQuickDropCounter > 9)
    {
        return strprintf("(%c) drop counter %d",
            'a' + mQuickDropCounter - 10, mQuickDropCounter);
    }
    else
    {
        return strprintf("(%d) drop counter %d",
            mQuickDropCounter, mQuickDropCounter);
    }
}

void LocalPlayer::setQuickDropCounter(int n)
{
    if (n < 1 || n >= static_cast<signed>(quickDropCounterSize))
        return;
    mQuickDropCounter = n;
    config.setValue("quickDropCounter", mQuickDropCounter);
    if (miniStatusWindow)
        miniStatusWindow->updateStatus();
}

const unsigned pickUpTypeSize = 7;

void LocalPlayer::changePickUpType()
{
    changeMode(&mPickUpType, pickUpTypeSize, "pickUpType",
        &LocalPlayer::getPickUpTypeString);
}

static const char *pickUpTypeStrings[] =
{
    N_("(S) small pick up 1x1 cells"),
    N_("(D) default pick up 2x1 cells"),
    N_("(F) forward pick up 2x3 cells"),
    N_("(3) pick up 3x3 cells"),
    N_("(g) go and pick up in distance 4"),
    N_("(G) go and pick up in distance 8"),
    N_("(A) go and pick up in max distance"),
    N_("(?) pick up")
};

std::string LocalPlayer::getPickUpTypeString()
{
    return gettext(getVarItem(&pickUpTypeStrings[0],
        mPickUpType, pickUpTypeSize));
}

const unsigned debugPathSize = 5;

static const char *debugPathStrings[] =
{
    N_("(N) normal map view"),
    N_("(D) debug map view"),
    N_("(u) ultra map view"),
    N_("(U) ultra map view 2"),
    N_("(e) empty map view"),
    N_("(b) black & white map view")
};

std::string LocalPlayer::getDebugPathString()
{
    return gettext(getVarItem(&debugPathStrings[0],
        viewport->getDebugPath(), debugPathSize));
}

const unsigned magicAttackSize = 5;

void LocalPlayer::switchMagicAttack()
{
    changeMode(&mMagicAttackType, magicAttackSize, "magicAttackType",
        &LocalPlayer::getMagicAttackString);
}

static const char *magicAttackStrings[] =
{
    N_("(f) use #flar for magic attack"),
    N_("(c) use #chiza for magic attack"),
    N_("(I) use #ingrav for magic attack"),
    N_("(F) use #frillyar for magic attack"),
    N_("(U) use #upmarmu for magic attack"),
    N_("(?) magic attack")
};

std::string LocalPlayer::getMagicAttackString()
{
    return gettext(getVarItem(&magicAttackStrings[0],
        mMagicAttackType, magicAttackSize));
}

const unsigned pvpAttackSize = 4;

void LocalPlayer::switchPvpAttack()
{
    changeMode(&mPvpAttackType, pvpAttackSize, "pvpAttackType",
        &LocalPlayer::getPvpAttackString);
}

static const char *pvpAttackStrings[] =
{
    N_("(a) attack all players"),
    N_("(f) attack all except friends"),
    N_("(b) attack bad relations"),
    N_("(d) don't attack players"),
    N_("(?) pvp attack")
};

std::string LocalPlayer::getPvpAttackString()
{
    return gettext(getVarItem(&pvpAttackStrings[0],
        mPvpAttackType, pvpAttackSize));
}

const unsigned imitationModeSize = 2;

void LocalPlayer::changeImitationMode()
{
    changeMode(&mImitationMode, imitationModeSize, "imitationMode",
        &LocalPlayer::getImitationModeString);
}

static const char *imitationModeStrings[] =
{
    N_("(D) default imitation"),
    N_("(O) outfits imitation"),
    N_("(?) imitation")
};

std::string LocalPlayer::getImitationModeString()
{
    return gettext(getVarItem(&imitationModeStrings[0],
        mImitationMode, imitationModeSize));
}

const unsigned awayModeSize = 2;

void LocalPlayer::changeAwayMode()
{
    mAwayMode = !mAwayMode;
    mAfkTime = 0;
    mInactive = false;
    updateName();
    if (miniStatusWindow)
        miniStatusWindow->updateStatus();
    if (mAwayMode)
    {
        if (chatWindow)
            chatWindow->clearAwayLog();

        cancelFollow();
        navigateClean();
        if (outfitWindow)
            outfitWindow->wearAwayOutfit();
        mAwayDialog = new OkDialog(_("Away"),
            config.getStringValue("afkMessage"),
            DIALOG_SILENCE, true, false);
        mAwayDialog->addActionListener(mAwayListener);
        sound.volumeOff();
    }
    else
    {
        mAwayDialog = nullptr;
        sound.volumeRestore();
        if (chatWindow)
        {
            chatWindow->displayAwayLog();
            chatWindow->clearAwayLog();
        }
    }
}

static const char *awayModeStrings[] =
{
    N_("(O) on keyboard"),
    N_("(A) away"),
    N_("(?) away")
};

std::string LocalPlayer::getAwayModeString()
{
    return gettext(getVarItem(&awayModeStrings[0],
        mAwayMode, awayModeSize));
}

const unsigned cameraModeSize = 2;

static const char *cameraModeStrings[] =
{
    N_("(G) game camera mode"),
    N_("(F) free camera mode"),
    N_("(?) away")
};

std::string LocalPlayer::getCameraModeString()
{
    return gettext(getVarItem(&cameraModeStrings[0],
        viewport->getCameraMode(), cameraModeSize));
}

const unsigned gameModifiersSize = 2;

void LocalPlayer::switchGameModifiers()
{
    mDisableGameModifiers = !mDisableGameModifiers;
    config.setValue("disableGameModifiers", mDisableGameModifiers);
    miniStatusWindow->updateStatus();

    const std::string str = getGameModifiersString();
    if (str.size() > 4)
        debugMsg(str.substr(4));
}

static const char *gameModifiersStrings[] =
{
    _("Game modifiers are enabled"),
    _("Game modifiers are disabled"),
    _("Game modifiers are unknown")
};

std::string LocalPlayer::getGameModifiersString()
{
    return gettext(getVarItem(&gameModifiersStrings[0],
        mDisableGameModifiers, gameModifiersSize));
}


void LocalPlayer::changeEquipmentBeforeAttack(Being* target)
{
    if (mAttackWeaponType == 1 || !target || !PlayerInfo::getInventory())
        return;

    bool allowSword = false;
    int dx = target->getTileX() - mX;
    int dy = target->getTileY() - mY;
    Item *item = nullptr;

    if (dx * dx + dy * dy > 80)
        return;

    if (dx * dx + dy * dy < 8)
        allowSword = true;

    const Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    //if attack distance for sword
    if (allowSword)
    {
        //finding sword
        item = inv->findItem(571, 0);

        if (!item)
            item = inv->findItem(570, 0);

        if (!item)
            item = inv->findItem(579, 0);

        if (!item)
            item = inv->findItem(536, 0);

        //no swords
        if (!item)
            return;

        //if sword not equiped
        if (!item->isEquipped())
            Net::getInventoryHandler()->equipItem(item);

        //if need equip shield too
        if (mAttackWeaponType == 3)
        {
            //finding shield
            item = inv->findItem(601, 0);
            if (!item)
                item = inv->findItem(602, 0);
            if (item && !item->isEquipped())
                Net::getInventoryHandler()->equipItem(item);
        }

    }
    //big distance. allowed only bow
    else
    {
        //finding bow
        item = inv->findItem(545, 0);

        if (!item)
            item = inv->findItem(530, 0);

        //no bow
        if (!item)
            return;

        if (!item->isEquipped())
            Net::getInventoryHandler()->equipItem(item);
    }
}

void LocalPlayer::crazyMove()
{
    bool oldDisableCrazyMove = mDisableCrazyMove;
    mDisableCrazyMove = true;
    switch (mCrazyMoveType)
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
    if (mAction == MOVE)
        return;

    if (!Client::limitPackets(PACKET_DIRECTION))
        return;

    if (mDirection == Being::UP)
    {
        setWalkingDir(Being::UP);
        setDirection(Being::LEFT);
        Net::getPlayerHandler()->setDirection(Being::LEFT);
    }
    else if (mDirection == Being::LEFT)
    {
        setWalkingDir(Being::LEFT);
        setDirection(Being::DOWN);
        Net::getPlayerHandler()->setDirection(Being::DOWN);
    }
    else if (mDirection == Being::DOWN)
    {
        setWalkingDir(Being::DOWN);
        setDirection(Being::RIGHT);
        Net::getPlayerHandler()->setDirection(Being::RIGHT);
    }
    else if (mDirection == Being::RIGHT)
    {
        setWalkingDir(Being::RIGHT);
        setDirection(Being::UP);
        Net::getPlayerHandler()->setDirection(Being::UP);
    }
}

void LocalPlayer::crazyMove2()
{
    if (mAction == MOVE)
        return;

    if (!Client::limitPackets(PACKET_DIRECTION))
        return;

    if (mDirection == Being::UP)
    {
        setWalkingDir(Being::UP | Being::LEFT);
        setDirection(Being::RIGHT);
        Net::getPlayerHandler()->setDirection(Being::DOWN | Being::RIGHT);
    }
    else if (mDirection == Being::RIGHT)
    {
        setWalkingDir(Being::UP | Being::RIGHT);
        setDirection(Being::DOWN);
        Net::getPlayerHandler()->setDirection(Being::DOWN | Being::LEFT);
    }
    else if (mDirection == Being::DOWN)
    {
        setWalkingDir(Being::DOWN | Being::RIGHT);
        setDirection(Being::LEFT);
        Net::getPlayerHandler()->setDirection(Being::UP | Being::LEFT);
    }
    else if (mDirection == Being::LEFT)
    {
        setWalkingDir(Being::DOWN | Being::LEFT);
        setDirection(Being::UP);
        Net::getPlayerHandler()->setDirection(Being::UP | Being::RIGHT);
    }
}

void LocalPlayer::crazyMove3()
{
    if (mAction == MOVE)
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

    if (!Client::limitPackets(PACKET_DIRECTION))
        return;

    setDirection(Being::DOWN);
    Net::getPlayerHandler()->setDirection(Being::DOWN);
}

void LocalPlayer::crazyMove4()
{
    if (mAction == MOVE)
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
    if (mAction == MOVE)
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
    if (mAction == MOVE)
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
    if (mAction == MOVE)
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
    if (mAction == MOVE || !mMap)
        return;
    int idx = 0;
    int dist = 1;

// look
//      up, ri,do,le
    static const int movesX[][4] =
    {
        {-1,  0,  1,  0},   //move left
        { 0,  1,  0, -1},   //move up
        { 1,  0, -1,  0},   //move right
        { 0, -1,  0,  1}    //move down
    };

// look
//      up, ri,do,le
    static const int movesY[][4] =
    {
        { 0, -1,  0,  1},   //move left
        {-1,  0,  1,  0},   //move up
        { 0,  1,  0, -1},   //move right
        { 1,  0, -1,  0}    //move down
    };

    if (mDirection == Being::UP)
        idx = 0;
    else if (mDirection == Being::RIGHT)
        idx = 1;
    else if (mDirection == Being::DOWN)
        idx = 2;
    else if (mDirection == Being::LEFT)
        idx = 3;


    int mult = 1;
    if (mMap->getWalk(mX + movesX[idx][0],
        mY + movesY[idx][0], getWalkMask()))
    {
        while (mMap->getWalk(mX + movesX[idx][0] * mult,
               mY + movesY[idx][0] * mult,
               getWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][0] * (mult - 1), movesY[idx][0] * (mult - 1));
    }
    else if (mMap->getWalk(mX + movesX[idx][1],
             mY + movesY[idx][1], getWalkMask()))
    {
        while (mMap->getWalk(mX + movesX[idx][1] * mult,
               mY + movesY[idx][1] * mult,
               getWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][1] * (mult - 1), movesY[idx][1] * (mult - 1));
    }
    else if (mMap->getWalk(mX + movesX[idx][2],
             mY + movesY[idx][2], getWalkMask()))
    {
        while (mMap->getWalk(mX + movesX[idx][2] * mult,
               mY + movesY[idx][2] * mult,
               getWalkMask()) && mult <= dist)
        {
            mult ++;
        }
        move(movesX[idx][2] * (mult - 1), movesY[idx][2] * (mult - 1));
    }
    else if (mMap->getWalk(mX + movesX[idx][3],
             mY + movesY[idx][3], getWalkMask()))
    {
        while (mMap->getWalk(mX + movesX[idx][3] * mult,
               mY + movesY[idx][3] * mult,
               getWalkMask()) && mult <= dist)
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

    if (mAction == MOVE)
        return;

    switch (mCrazyMoveState)
    {
        case 0:
            switch (mDirection)
            {
                case UP   : dy = -1; break;
                case DOWN : dy = 1; break;
                case LEFT : dx = -1; break;
                case RIGHT: dx = 1; break;
                default: break;
            }
            move(dx, dy);
            mCrazyMoveState = 1;
            break;
        case 1:
            mCrazyMoveState = 2;
            if (!allowAction())
                return;
            Net::getPlayerHandler()->changeAction(SIT);
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
    std::string mMoveProgram(config.getStringValue("crazyMoveProgram"));

    if (mAction == MOVE)
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

            char param = mMoveProgram[mCrazyMoveState++];
            if (param == '?')
            {
                char cmd[] = {'l', 'r', 'u', 'd'};
                srand(tick_time);
                param = cmd[rand() % 4];
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
                case 'f':
                    switch (mDirection)
                    {
                        case UP   : dy = -1; break;
                        case DOWN : dy = 1; break;
                        case LEFT : dx = -1; break;
                        case RIGHT: dx = 1; break;
                        default: break;
                    }
                    move(dx, dy);
                    break;
                case 'b':
                    switch (mDirection)
                    {
                        case UP   : dy = 1; break;
                        case DOWN : dy = -1; break;
                        case LEFT : dx = 1; break;
                        case RIGHT: dx = -1; break;
                        default: break;
                    }
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
            char param = mMoveProgram[mCrazyMoveState++];
            if (param == '?')
            {
                char cmd[] = {'l', 'r', 'u', 'd'};
                srand(tick_time);
                param = cmd[rand() % 4];
            }
            switch (param)
            {
                case 'd':

                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(Being::DOWN);
                        Net::getPlayerHandler()->setDirection(Being::DOWN);
                    }
                    break;
                case 'u':
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(Being::UP);
                        Net::getPlayerHandler()->setDirection(Being::UP);
                    }
                    break;
                case 'l':
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(Being::LEFT);
                        Net::getPlayerHandler()->setDirection(Being::LEFT);
                    }
                    break;
                case 'r':
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        setDirection(Being::RIGHT);
                        Net::getPlayerHandler()->setDirection(Being::RIGHT);
                    }
                    break;
                case 'L':
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        Uint8 dir = 0;
                        switch (mDirection)
                        {
                            case UP    : dir = Being::LEFT; break;
                            case DOWN  : dir = Being::RIGHT; break;
                            case LEFT  : dir = Being::DOWN; break;
                            case RIGHT : dir = Being::UP; break;
                            default: break;
                        }
                        setDirection(dir);
                        Net::getPlayerHandler()->setDirection(dir);
                    }
                    break;
                case 'R':
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        Uint8 dir = 0;
                        switch (mDirection)
                        {
                            case UP    : dir = Being::RIGHT; break;
                            case DOWN  : dir = Being::LEFT; break;
                            case LEFT  : dir = Being::UP; break;
                            case RIGHT : dir = Being::DOWN; break;
                            default: break;
                        }
                        setDirection(dir);
                        Net::getPlayerHandler()->setDirection(dir);
                    }
                    break;
                case 'b':
                    if (Client::limitPackets(PACKET_DIRECTION))
                    {
                        Uint8 dir = 0;
                        switch (mDirection)
                        {
                            case UP    : dir = Being::DOWN; break;
                            case DOWN  : dir = Being::UP; break;
                            case LEFT  : dir = Being::RIGHT; break;
                            case RIGHT : dir = Being::LEFT; break;
                            default: break;
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
    else if (mMoveProgram[mCrazyMoveState] == 'e')
    {
        mCrazyMoveState ++;
        char emo = mMoveProgram[mCrazyMoveState];
        if (emo == '?')
        {
            srand(tick_time);
            emote(static_cast<unsigned char>(1 + (rand() % 13)));
        }
        else
        {
            if (emo >= '0' && emo <= '9')
                emote(static_cast<unsigned char>(emo - '0' + 1));
            else if (emo >= 'a' && emo <= 'd')
                emote(static_cast<unsigned char>(emo - 'a' + 11));
        }

        mCrazyMoveState ++;
    }
    else
    {
        mCrazyMoveState ++;
    }

    if (mCrazyMoveState >= mMoveProgram.length())
        mCrazyMoveState = 0;
}

bool LocalPlayer::isReachable(int x, int y, int maxCost)
{
    if (!mMap)
        return false;

    if (x - 1 <= mX && x + 1 >= mX
        && y - 1 <= mY && y + 1 >= mY )
    {
        return true;
    }

    const Vector &playerPos = getPosition();

    Path debugPath = mMap->findPath(
        static_cast<int>(playerPos.x - 16) / 32,
        static_cast<int>(playerPos.y - 32) / 32,
        x, y, getWalkMask(), maxCost);

    return !debugPath.empty();
}

bool LocalPlayer::isReachable(Being *being, int maxCost)
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

    Path debugPath = mMap->findPath(
        static_cast<int>(playerPos.x - 16) / 32,
        static_cast<int>(playerPos.y - 32) / 32,
        being->getTileX(), being->getTileY(), getWalkMask(), maxCost);

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

bool LocalPlayer::pickUpItems(int pickUpType)
{
    if (!actorSpriteManager)
        return false;

    bool status = false;
    int x = mX;
    int y = mY;

    // first pick up item on player position
    FloorItem *item =
        actorSpriteManager->findItem(x, y);
    if (item)
    {
        status = pickUp(item);
        //status = true;
    }

    if (pickUpType == 0)
        pickUpType = mPickUpType;

    if (pickUpType == 0)
        return status;

    int x1, y1, x2, y2;
    switch (pickUpType)
    {
        case 1:
            switch (mDirection)
            {
                case UP   : --y; break;
                case DOWN : ++y; break;
                case LEFT : --x; break;
                case RIGHT: ++x; break;
                default: break;
            }
            item = actorSpriteManager->findItem(x, y);
            if (item)
                status = pickUp(item);
            break;
        case 2:
            switch (mDirection)
            {
                case UP   : x1 = x - 1; y1 = y - 1; x2 = x + 1; y2 = y; break;
                case DOWN : x1 = x - 1; y1 = y; x2 = x + 1; y2 = y + 1; break;
                case LEFT : x1 = x - 1; y1 = y - 1; x2 = x; y2 = y + 1; break;
                case RIGHT: x1 = x; y1 = y - 1; x2 = x + 1; y2 = y + 1; break;
                default: x1 = x; x2 = x; y1 = y; y2 = y; break;
            }
            if (actorSpriteManager->pickUpAll(x1, y1, x2, y2))
                status = true;
            break;
        case 3:
            if (actorSpriteManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
                status = true;
            break;

        case 4:
            if (!actorSpriteManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
            {
                if (actorSpriteManager->pickUpNearest(x, y, 4))
                    status = true;
            }
            else
            {
                status = true;
            }
            break;

        case 5:
            if (!actorSpriteManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
            {
                if (actorSpriteManager->pickUpNearest(x, y, 8))
                    status = true;
            }
            else
            {
                status = true;
            }
            break;

        case 6:
            if (!actorSpriteManager->pickUpAll(x - 1, y - 1, x + 1, y + 1))
            {
                if (actorSpriteManager->pickUpNearest(x, y, 90))
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


void LocalPlayer::moveByDirection(unsigned char dir)
{
    int dx = 0, dy = 0;
#ifdef MANASERV_SUPPORT
    if (dir & UP)
        dy -= 32;
    if (dir & DOWN)
        dy += 32;
    if (dir & LEFT)
        dx -= 32;
    if (dir & RIGHT)
        dx += 32;
#else
    if (dir & UP)
        dy--;
    if (dir & DOWN)
        dy++;
    if (dir & LEFT)
        dx--;
    if (dir & RIGHT)
        dx++;
#endif

    move(dx, dy);
}

void LocalPlayer::specialMove(unsigned char direction)
{
    if (direction && (mNavigateX || mNavigateY))
        navigateClean();

    if (direction && (getInvertDirection() >= 2
        && getInvertDirection() <= 4)
        && !mIsServerBuggy)
    {
        if (mAction == MOVE)
            return;

        int max;

        if (getInvertDirection() == 2)
            max = 5;
        else if (getInvertDirection() == 4)
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

void LocalPlayer::debugMsg(std::string str)
{
    if (debugChatTab)
        debugChatTab->chatLog(str);
}

void LocalPlayer::magicAttack()
{
    if (!chatWindow || !isAlive()
        || !Net::getPlayerHandler()->canUseMagic())
    {
        return;
    }

    if (!Client::limitPackets(PACKET_CHAT))
        return;

    switch (mMagicAttackType)
    {
        //flar W00
        case 0:
            tryMagic("#flar", 1, 0, 10);
            break;
        //chiza W01
        case 1:
            tryMagic("#chiza", 1, 0,  9);
            break;
        //ingrav W10
        case 2:
            tryMagic("#ingrav", 2, 2,  20);
            break;
        //frillyar W11
        case 3:
            tryMagic("#frillyar", 2, 2, 25);
            break;
        //upmarmu W12
        case 4:
            tryMagic("#upmarmu", 2, 2, 20);
            break;
        default:
            break;
    }
}

void LocalPlayer::tryMagic(std::string spell, int baseMagic,
                           int schoolMagic, int mana)
{
    if (!chatWindow)
        return;

    if (PlayerInfo::getStatEffective(340) >= baseMagic
        && PlayerInfo::getStatEffective(342) >= schoolMagic)
    {
        if (PlayerInfo::getAttribute(MP) >= mana)
        {
            if (!Client::limitPackets(PACKET_CHAT))
                return;

            chatWindow->localChatInput(spell);
        }
    }
}

void LocalPlayer::loadHomes()
{
    std::string homeStr = serverConfig.getValue("playerHomes",
            "maps/018-1.tmx 71 76 maps/013-3.tmx 71 24");
    std::string buf;
    std::stringstream ss(homeStr);

    while (ss >> buf)
    {
        Vector pos;
        ss >> pos.x;
        ss >> pos.y;
        mHomes[buf] = pos;
    }

}

void LocalPlayer::setMap(Map *map)
{
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
}

void LocalPlayer::setHome()
{
    if (!mMap || !socialWindow)
        return;

    SpecialLayer *specialLayer = mMap->getSpecialLayer();

    if (!specialLayer)
        return;

    std::string key = mMap->getProperty("_realfilename");
    Vector pos = mHomes[key];

    if (mAction == SIT)
    {
        std::map<std::string, Vector>::const_iterator iter = mHomes.find(key);

        if (iter != mHomes.end())
        {
            socialWindow->removePortal(static_cast<int>(pos.x),
                static_cast<int>(pos.y));
        }

        if (iter != mHomes.end() && mX == static_cast<int>(pos.x)
            && mY == static_cast<int>(pos.y))
        {
            mMap->updatePortalTile("", MapItem::EMPTY,
                static_cast<int>(pos.x), static_cast<int>(pos.y));

            mHomes.erase(key);
            socialWindow->removePortal(static_cast<int>(pos.x),
                static_cast<int>(pos.y));
        }
        else
        {
            if (specialLayer && iter != mHomes.end())
            {
                specialLayer->setTile(static_cast<int>(pos.x),
                    static_cast<int>(pos.y), MapItem::EMPTY);
            }

            pos.x = static_cast<float>(mX);
            pos.y = static_cast<float>(mY);
            mHomes[key] = pos;
            mMap->updatePortalTile("home", MapItem::HOME,
                                   mX, mY);
            socialWindow->addPortal(mX, mY);
        }
        MapItem *mapItem = specialLayer->getTile(mX, mY);
        if (mapItem)
        {
            int idx = socialWindow->getPortalIndex(mX, mY);
            mapItem->setName(keyboard.getKeyShortString(
                outfitWindow->keyName(idx)));
        }
        saveHomes();
    }
    else
    {
        MapItem *mapItem = specialLayer->getTile(mX, mY);
        int type = 0;

        std::map<std::string, Vector>::iterator iter = mHomes.find(key);
        if (iter != mHomes.end() && mX == pos.x && mY == pos.y)
        {
            mHomes.erase(key);
            saveHomes();
        }

        if (!mapItem || mapItem->getType() == MapItem::EMPTY)
        {
            if (mDirection & UP)
                type = MapItem::ARROW_UP;
            else if (mDirection & LEFT)
                type = MapItem::ARROW_LEFT;
            else if (mDirection & DOWN)
                type = MapItem::ARROW_DOWN;
            else if (mDirection & RIGHT)
                type = MapItem::ARROW_RIGHT;
        }
        else
        {
            type = MapItem::EMPTY;
        }
        mMap->updatePortalTile("", type, mX, mY);

        if (type != MapItem::EMPTY)
        {
            socialWindow->addPortal(mX, mY);
            mapItem = specialLayer->getTile(mX, mY);
            if (mapItem)
            {
                int idx = socialWindow->getPortalIndex(mX, mY);
                mapItem->setName(keyboard.getKeyShortString(
                    outfitWindow->keyName(idx)));
            }
        }
        else
        {
            specialLayer->setTile(mX, mY, MapItem::EMPTY);
            socialWindow->removePortal(mX, mY);
        }
    }
}

void LocalPlayer::saveHomes()
{
    std::string homeStr;
    std::stringstream ss(homeStr);

    for (std::map<std::string, Vector>::const_iterator iter = mHomes.begin();
         iter != mHomes.end(); ++iter )
    {
        Vector pos = (*iter).second;

        if (iter != mHomes.begin())
            ss << " ";
        ss << (*iter).first << " " << pos.x << " " << pos.y;
    }

    serverConfig.setValue("playerHomes", ss.str());
}

void LocalPlayer::pingRequest()
{
    if (mWaitPing == true && mPingSendTick != 0)
    {
        if (tick_time >= mPingSendTick
            && (tick_time - mPingSendTick) > 1000)
        {
            return;
        }
    }

    mPingSendTick = tick_time;
    mWaitPing = true;
    Net::getBeingHandler()->requestNameById(getId());
}

void LocalPlayer::pingResponse()
{
    if (mWaitPing == true && mPingSendTick > 0)
    {
        mWaitPing = false;
        if (tick_time < mPingSendTick)
        {
            mPingSendTick = 0;
            mPingTime = 0;
        }
        else
        {
            mPingTime = (tick_time - mPingSendTick) * 10;
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


void LocalPlayer::setAway(const std::string &message)
{
    if (!message.empty())
        config.setValue("afkMessage", message);
    changeAwayMode();
    updateStatus();
}

void LocalPlayer::setPseudoAway(const std::string &message)
{
    if (!message.empty())
        config.setValue("afkMessage", message);
    mPseudoAwayMode = !mPseudoAwayMode;
}

void LocalPlayer::afkRespond(ChatTab *tab, const std::string &nick)
{
    if (mAwayMode)
    {
        if (mAfkTime == 0
            || cur_time < mAfkTime
            || cur_time - mAfkTime > awayLimitTimer)
        {
            std::string msg = "*AFK*: "
                    + config.getStringValue("afkMessage");

            if (!tab)
            {
                Net::getChatHandler()->privateMessage(nick, msg);
                if (localChatTab)
                {
                    localChatTab->chatLog(getName() + " : " + msg,
                        ACT_WHISPER, false);
                }
            }
            else
            {
                if (tab->getNoAway())
                    return;
                Net::getChatHandler()->privateMessage(nick, msg);
                tab->chatLog(getName(), msg);
            }
            mAfkTime = cur_time;
        }
    }
}

bool LocalPlayer::navigateTo(int x, int y)
{
    if (!mMap)
        return false;

    SpecialLayer *tmpLayer = mMap->getTempLayer();
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
        static_cast<int>(playerPos.x - 16) / 32,
        static_cast<int>(playerPos.y - 32) / 32,
        x, y, getWalkMask(), 0);

    if (mDrawPath)
        tmpLayer->addRoad(mNavigatePath);
    return !mNavigatePath.empty();
}

void LocalPlayer::navigateTo(Being *being)
{
    if (!mMap || !being)
        return;

    SpecialLayer *tmpLayer = mMap->getTempLayer();
    if (!tmpLayer)
        return;

    const Vector &playerPos = getPosition();
    mShowNavigePath = true;
    mOldX = static_cast<int>(playerPos.x);
    mOldY = static_cast<int>(playerPos.y);
    mOldTileX = mX;
    mOldTileY = mY;
    mNavigateX = being->getTileX();
    mNavigateY = being->getTileY();

    mNavigatePath = mMap->findPath(
        static_cast<int>(playerPos.x - 16) / 32,
        static_cast<int>(playerPos.y - 32) / 32,
        being->getTileX(), being->getTileY(),
        getWalkMask(), 0);

    if (mDrawPath)
        tmpLayer->addRoad(mNavigatePath);
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

    SpecialLayer *tmpLayer = mMap->getTempLayer();
    if (!tmpLayer)
        return;

    tmpLayer->clean();
}

void LocalPlayer::updateCoords()
{
    Being::updateCoords();

    const Vector &playerPos = getPosition();

    if (mX != mOldTileX || mY != mOldTileY)
    {
        if (socialWindow)
            socialWindow->updatePortals();
        if (viewport)
            viewport->hideBeingPopup();
        if (mMap)
        {
            std::string str = mMap->getObjectData(mX, mY,
                MapItem::MUSIC);
            if (str.empty())
                str = mMap->getMusicFile();
            if (str != sound.getCurrentMusicFile())
            {
                if (str.empty())
                    sound.fadeOutMusic();
                else
                    sound.fadeOutAndPlayMusic(str);
            }
        }
    }

    if (mShowNavigePath)
    {
        if (mMap && (mX != mOldTileX || mY != mOldTileY))
        {
            SpecialLayer *tmpLayer = mMap->getTempLayer();
            if (!tmpLayer)
                return;

            int x = static_cast<int>(playerPos.x - 16) / 32;
            int y = static_cast<int>(playerPos.y - 32) / 32;
            if (mNavigateId)
            {
                if (!actorSpriteManager)
                {
                    navigateClean();
                    return;
                }

                Being* being = actorSpriteManager->findBeing(mNavigateId);
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
                     i_end = mNavigatePath.end(); i != i_end; ++i)
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

void LocalPlayer::targetMoved()
{
/*
    if (mKeepAttacking)
    {
        if (mTarget && mServerAttack)
        {
            logger->log("LocalPlayer::targetMoved0");
            if (!Client::limitPackets(PACKET_ATTACK))
                return;
            logger->log("LocalPlayer::targetMoved");
            Net::getPlayerHandler()->attack(mTarget->getId(), mServerAttack);
        }
    }
*/
}

int LocalPlayer::getPathLength(Being* being)
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
        Path debugPath = mMap->findPath(
            static_cast<int>(playerPos.x - 16) / 32,
            static_cast<int>(playerPos.y - 32) / 32,
            being->getTileX(), being->getTileY(),
            getWalkMask(), 0);
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

int LocalPlayer::getAttackRange2()
{
    int range = getAttackRange();
    if (range == 1)
        range = 2;
    return range;
}

void LocalPlayer::attack2(Being *target, bool keep, bool dontChangeEquipment)
{
    if (!dontChangeEquipment && target)
        changeEquipmentBeforeAttack(target);

    if ((!target || getAttackType() == 0 || getAttackType() == 3)
        || (withinAttackRange(target, serverVersion < 1,
        serverVersion < 1 ? 1 : 0)
        && getPathLength(target) <= getAttackRange2()))
    {
        attack(target, keep);
        if (getAttackType() == 2)
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
        if (getAttackType() == 2)
        {
            if (pickUpItems())
                return;
        }
        setTarget(target);
        if (target && target->getType() != Being::NPC)
        {
            mKeepAttacking = true;
            moveToTarget();
        }
    }
}

void LocalPlayer::setFollow(std::string player)
{
    mPlayerFollowed = player;
    if (!mPlayerFollowed.empty())
        debugMsg(_("Follow: ") + player);
    else
        debugMsg(_("Follow canceled"));
}

void LocalPlayer::setImitate(std::string player)
{
    mPlayerImitated = player;
    if (!mPlayerImitated.empty())
        debugMsg(_("Imitation: ") + player);
    else
        debugMsg(_("Imitation canceled"));
}

void LocalPlayer::cancelFollow()
{
    if (!mPlayerFollowed.empty())
        debugMsg(_("Follow canceled"));
    if (!mPlayerImitated.empty())
        debugMsg(_("Imitation canceled"));
    mPlayerFollowed = "";
    mPlayerImitated = "";
}

void LocalPlayer::imitateEmote(Being* being, unsigned char action)
{
    if (!being)
        return;

    std::string player_imitated = getImitate();
    if (!player_imitated.empty() && being->getName() == player_imitated)
        emote(action);
}

void LocalPlayer::imitateAction(Being *being, Being::Action action)
{
    if (!being)
        return;

    std::string player_imitated = getImitate();
    if (!player_imitated.empty() && being->getName() == player_imitated)
    {
        setAction(action);
        Net::getPlayerHandler()->changeAction(action);
    }
}

void LocalPlayer::imitateDirection(Being *being, unsigned char dir)
{
    if (!being)
        return;

    std::string player_imitated = getImitate();
    if (!player_imitated.empty() && being->getName() == player_imitated)
    {
        if (!Client::limitPackets(PACKET_DIRECTION))
            return;

        if (mFollowMode == 2)
        {
            Uint8 dir2 = 0;
            if (dir & Being::LEFT)
                dir2 |= Being::RIGHT;
            else if (dir & Being::RIGHT)
                dir2 |= Being::LEFT;
            if (dir & Being::UP)
                dir2 |= Being::DOWN;
            else if (dir & Being::DOWN)
                dir2 |= Being::UP;

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

void LocalPlayer::imitateOutfit(Being *player, int sprite)
{
    if (!player)
        return;

    std::string player_imitated = getImitate();
    if (mImitationMode == 1 && !player_imitated.empty()
        && player->getName() == player_imitated)
    {
        if (sprite < 0 || sprite >= player->getNumberOfLayers())
            return;

        AnimatedSprite *equipmentSprite = dynamic_cast<AnimatedSprite *>(player
                ->getSprite(sprite));

        if (equipmentSprite)
        {
//            logger->log("have equipmentSprite");
            Inventory *inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            std::string path = equipmentSprite->getIdPath();
            if (path.empty())
                return;

//            logger->log("idPath: " + path);

            Item *item = inv->findItemBySprite(path, player->getGender());
//            if (item)
//            {
//                logger->log("got item");
//                if (item->isEquipped())
//                    logger->log("isEquipped");
//            }

            if (item && !item->isEquipped())
                Net::getInventoryHandler()->equipItem(item);
        }
        else
        {
//            logger->log("have unequip %d", sprite);

            int equipmentSlot = Net::getInventoryHandler()
                ->convertFromServerSlot(sprite);
//            logger->log("equipmentSlot: " + toString(equipmentSlot));
            if (equipmentSlot == EQUIP_PROJECTILE_SLOT)
                return;

            Item *item = PlayerInfo::getEquipment(equipmentSlot);
            if (item)
            {
//                logger->log("unequiping");
                Net::getInventoryHandler()->unequipItem(item);
            }
        }
    }
}

void LocalPlayer::followMoveTo(Being *being, int x, int y)
{
    if (being && !mPlayerFollowed.empty()
        && being->getName() == mPlayerFollowed)
    {
        mPickUpTarget = nullptr;
        setDestination(x, y);
    }
}

void LocalPlayer::followMoveTo(Being *being, int x1, int y1, int x2, int y2)
{
    if (!being)
        return;

    mPickUpTarget = nullptr;
    if (!mPlayerFollowed.empty() && being->getName() == mPlayerFollowed)
    {
        switch (mFollowMode)
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
                    if (actorSpriteManager)
                    {
                        Being *b = actorSpriteManager->findBeingByName(
                            mPlayerFollowed, Being::PLAYER);
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

void LocalPlayer::setNextDest(int x, int y)
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

bool LocalPlayer::allowMove()
{
    if (mIsServerBuggy)
    {
        if (mAction == MOVE)
            return false;
    }
    return true;
}

void LocalPlayer::fixPos(int maxDist)
{
    if (!mCrossX && !mCrossY)
        return;

    int dx = abs(mX - mCrossX);
    int dy = abs(mY - mCrossY);
    int dest = (dx * dx) + (dy * dy);

    if (dest > maxDist && mActivityTime
        && (cur_time < mActivityTime || cur_time - mActivityTime > 2))
    {
        mActivityTime = cur_time;
        moveTo(mCrossX, mCrossY);
    }
}

void LocalPlayer::setRealPos(int x, int y)
{
    if (!mMap)
        return;

    SpecialLayer *layer = mMap->getTempLayer();
    if (layer)
    {
        fixPos(1);

        if ((mCrossX || mCrossY) && layer->getTile(mCrossX, mCrossY)
            && layer->getTile(mCrossX, mCrossY)->getType() == MapItem::CROSS)
        {
            layer->setTile(mCrossX, mCrossY, MapItem::EMPTY);
        }

        if (!layer->getTile(x, y)
            || layer->getTile(x, y)->getType() == MapItem::EMPTY)
        {
            if (getTileX() != x && getTileY() != y)
                layer->setTile(x, y, MapItem::CROSS);
        }

        mCrossX = x;
        mCrossY = y;
    }
}
void LocalPlayer::fixAttackTarget()
{
    if (!mMap || !mTarget)
        return;

    if (getMoveToTargetType() == 7 || !getAttackType()
        || !config.getBoolValue("autofixPos"))
    {
        return;
    }

    const Vector &playerPos = getPosition();
    Path debugPath = mMap->findPath(
        static_cast<int>(playerPos.x - 16) / 32,
        static_cast<int>(playerPos.y - 32) / 32,
        mTarget->getTileX(), mTarget->getTileY(),
        getWalkMask(), 0);

    if (!debugPath.empty())
    {
        Path::const_iterator i = debugPath.begin();
        moveTo((*i).x, (*i).y);
    }
}

void LocalPlayer::respawn()
{
    navigateClean();
}

int LocalPlayer::getTargetTime()
{
    if (mTargetTime != -1)
        return get_elapsed_time(mTargetTime);
    else
        return -1;
}

int LocalPlayer::getLevel() const
{
    return PlayerInfo::getAttribute(LEVEL);
}

void LocalPlayer::updateNavigateList()
{
    if (mMap)
    {
        std::map<std::string, Vector>::const_iterator iter =
                mHomes.find(mMap->getProperty("_realfilename"));

        if (iter != mHomes.end())
        {
            Vector pos = mHomes[(*iter).first];
            if (pos.x && pos.y)
            {
                mMap->addPortalTile("home", MapItem::HOME,
                    static_cast<int>(pos.x), static_cast<int>(pos.y));
            }
        }
    }
}

void LocalPlayer::waitFor(std::string nick)
{
    mWaitFor = nick;
}

void LocalPlayer::checkNewName(Being *being)
{
    if (!being)
        return;

    const std::string nick = being->getName();
    if (being->getType() == ActorSprite::PLAYER)
    {
        const Guild *guild = getGuild();
        if (guild)
        {
            const GuildMember *gm = guild->getMember(nick);
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
            ChatTab *tab = chatWindow->getWhisperTab(nick);
            if (tab)
                tab->setTabColor(&Theme::getThemeColor(Theme::WHISPER));
        }
    }

    if (!mWaitFor.empty() && mWaitFor == nick)
    {
        debugMsg(_("You see ") + mWaitFor);
        sound.playGuiSound(SOUND_INFO);
        mWaitFor = "";
    }
}

void LocalPlayer::resetYellowBar()
{
    mInvertDirection = 0;
    mCrazyMoveType = config.resetIntValue("crazyMoveType");
    mMoveToTargetType = config.resetIntValue("moveToTargetType");
    mFollowMode = config.resetIntValue("followMode");
    mAttackWeaponType = config.resetIntValue("attackWeaponType");
    mAttackType = config.resetIntValue("attackType");
    mMagicAttackType = config.resetIntValue("magicAttackType");
    mPvpAttackType = config.resetIntValue("pvpAttackType");
    mQuickDropCounter = config.resetIntValue("quickDropCounter");
    mPickUpType = config.resetIntValue("pickUpType");
    if (viewport)
    {
        viewport->setDebugPath(0);
        if (viewport->getCameraMode())
            viewport->toggleCameraMode();
    }
    if (mMap)
        mMap->setDebugFlags(0);
    mImitationMode = config.resetIntValue("imitationMode");
    mDisableGameModifiers = config.resetBoolValue("disableGameModifiers");

    if (miniStatusWindow)
        miniStatusWindow->updateStatus();
}

unsigned char LocalPlayer::getWalkMask() const
{
    // for now blocking all types of collisions
    return Map::BLOCKMASK_WALL | Map::BLOCKMASK_AIR | Map::BLOCKMASK_WATER;
}

void LocalPlayer::removeHome()
{
    if (!mMap)
        return;

    std::string key = mMap->getProperty("_realfilename");
    std::map<std::string, Vector>::iterator iter = mHomes.find(key);

    if (iter != mHomes.end())
        mHomes.erase(key);
}

void LocalPlayer::stopAdvert()
{
    mBlockAdvert = true;
}

bool LocalPlayer::checAttackPermissions(Being *target)
{
    if (!target)
        return false;

    switch (mPvpAttackType)
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


const char *LocalPlayer::getVarItem(const char **arr, unsigned index,
                                    unsigned sz)
{
    if (index < sz)
        return arr[index];
    return arr[sz];
}

void LocalPlayer::updateStatus()
{
    if (serverVersion >= 4 && mEnableAdvert)
    {
        Uint8 status = 0;
        if (mTradebot && shopWindow && !shopWindow->isShopEmpty())
            status += FLAG_SHOP;

        if (mAwayMode || mPseudoAwayMode)
            status += FLAG_AWAY;

        if (mInactive)
            status += FLAG_INACTIVE;

        Net::getPlayerHandler()->updateStatus(status);
    }
}

void AwayListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && player_node && player_node->getAway())
    {
        player_node->changeAwayMode();
        player_node->updateStatus();
        if (outfitWindow)
            outfitWindow->unwearAwayOutfit();
        if (miniStatusWindow)
            miniStatusWindow->updateStatus();
    }
}
