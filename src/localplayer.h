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

#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "actorspritelistener.h"
#include "being.h"
#include "listener.h"
#include "localconsts.h"

#include "gui/userpalette.h"

#include <guichan/actionlistener.hpp>

#include <memory>
#include <vector>

class ChatTab;
class FloorItem;
class ImageSet;
class Item;
class Map;
class OkDialog;
struct SkillInfo;

class AwayListener final : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &event);
};

/**
 * Reasons an item can fail to be picked up.
 */
enum
{
    PICKUP_OKAY = 0,
    PICKUP_BAD_ITEM,
    PICKUP_TOO_HEAVY,
    PICKUP_TOO_FAR,
    PICKUP_INV_FULL,
    PICKUP_STACK_FULL,
    PICKUP_DROP_STEAL
};

/**
 * The local player character.
 */
class LocalPlayer final : public Being,
                          public ActorSpriteListener,
                          public Listener
{
    public:
        /**
         * Constructor.
         */
        LocalPlayer(const int id = 65535, const int subtype = 0);

        /**
         * Destructor.
         */
        ~LocalPlayer();

        virtual void logic();

        void slowLogic();

        virtual void setAction(const Action &action, const int attackType = 0);

        /**
         * Compute the next pathnode location when walking using keyboard.
         * used by nextTile().
         */
        Position getNextWalkPosition(const unsigned char dir) const;

        /**
         * Adds a new tile to the path when walking.
         * @note Eathena
         * Also, when specified, it picks up an item at the end of a path
         * or attack target.
         */
        virtual void nextTile()
        { nextTile(0); }

        virtual void nextTile(unsigned char dir);

        /**
         * Check the player has permission to invite users to specific guild
         */
        bool checkInviteRights(const std::string &guildName) const;

        /**
         * Invite a player to join guild
         */
        void inviteToGuild(Being *const being);

//        void clearInventory();
//        void setInvItem(int index, int id, int amount);

        bool pickUp(FloorItem *const item);

        /**
         * Called when an ActorSprite has been destroyed.
         * @param actorSprite the ActorSprite being destroyed.
         */
        void actorSpriteDestroyed(const ActorSprite &actorSprite);

        /**
         * Gets the attack range.
         */
        int getAttackRange() const;

        int getAttackRange2() const;

        void attack(Being *const target = nullptr, const bool keep = false,
                    const bool dontChangeEquipment = false);

        void attack2(Being *const target = nullptr, const bool keep = false,
                     const bool dontChangeEquipment = false);

        void setGMLevel(const int level);

        int getGMLevel() const
        { return mGMLevel; }

        void stopAttack(const bool keepAttack = false);

        void untarget();

        /**
         * Returns the current target of the player. Returns 0 if no being is
         * currently targeted.
         */
        Being *getTarget() const;

        /**
         * Sets the target being of the player.
         */
        void setTarget(Being *const target);

        /**
         * Sets a new destination for this being to walk to.
         */
        virtual void setDestination(const int x, const int y);

        /**
         * Sets a new direction to keep walking in.
         */
        void setWalkingDir(const unsigned char dir);

        /**
         * Gets the walking direction
         */
        unsigned char getWalkingDir() const
        { return mWalkingDir; }

        /**
         * Sets going to being to attack
         */
        void setGotoTarget(Being *const target);

        /**
         * Returns whether the target is in range to attack
         */
        bool withinAttackRange(const Being *const target,
                               const bool fixDistance = false,
                               const int addRange = 0) const;

        /**
         * Stops the player dead in his tracks
         */
        void stopWalking(const bool sendToServer = true);

        bool toggleSit() const;

        bool updateSit();

        bool emote(const uint8_t emotion) const;

        /**
         * Shows item pickup notifications.
         */
        void pickedUp(const ItemInfo &itemInfo, const int amount,
                      const unsigned char color, const int floorItemId,
                      const unsigned char fail);

        int getLevel() const;

        int getTargetTime();

//        int getSkillPoints() const
//        { return mSkillPoints; }

//        void setSkillPoints(int points);

        /** Tells that the path has been set by mouse. */
        void pathSetByMouse()
        { mPathSetByMouse = true; }

        /** Tells if the path has been set by mouse. */
        bool isPathSetByMouse() const
        { return mPathSetByMouse; }

        int getInvertDirection() const
        { return mInvertDirection; }

        void setInvertDirection(const int n)
        { mInvertDirection = n; }

        void invertDirection();

        int getAttackWeaponType() const
        { return mAttackWeaponType; }

        int getAttackType() const
        { return mAttackType; }

        int getFollowMode() const
        { return mFollowMode; }

        int getImitationMode() const
        { return mImitationMode; }

        void changeAttackWeaponType();

        void changeAttackType();

        void changeFollowMode();

        void changeImitationMode();

        void changePickUpType();

        int getCrazyMoveType() const
        { return mCrazyMoveType; }

        int getPickUpType() const
        { return mPickUpType; }

        int getQuickDropCounter() const
        { return mQuickDropCounter; }

        void setQuickDropCounter(const int n);

        void changeQuickDropCounter();

        int getMoveState() const
        { return mMoveState; }

        void setMoveState(const int n)
        { mMoveState = n; }

        void switchMagicAttack();

        void switchPvpAttack();

        int getMagicAttackType() const
        { return mMagicAttackType; }

        int getPvpAttackType() const
        { return mPvpAttackType; }

        int getMoveToTargetType() const
        { return mMoveToTargetType; }

        int getDisableGameModifiers() const
        { return mDisableGameModifiers; }

        int getPingTime() const
        { return mPingTime; }

        void tryPingRequest();

        void changeMoveToTargetType();

        void switchGameModifiers();

        void magicAttack() const;

        void specialMove(const unsigned char direction);

        void moveByDirection(const unsigned char dir);

        bool pickUpItems(int pickUpType = 0);

        void changeCrazyMoveType();

        void crazyMove();

        void moveTo(const int x, const int y);

        void move(const int dX, const int dY);

        void moveToTarget(int dist = -1);

        void moveToHome();

        void debugMsg(std::string str) const;

//        int getSkillLv(int id);

        bool isReachable(const int x, const int y,
                         const int maxCost = 0) const;

        bool isReachable(Being *const being,
                         const int maxCost = 0);

        void setHome();

        void pingRequest();

        void pingResponse();

        void changeAwayMode();

        void setAway(const std::string &message);

        void setPseudoAway(const std::string &message);

        bool getAway() const
        { return mAwayMode; }

        bool getPseudoAway() const
        { return mPseudoAwayMode; }

        void setHalfAway(const bool n)
        { mInactive = n; }

        bool getHalfAway() const
        { return mInactive; }

        void afkRespond(ChatTab *const tab, const std::string &nick);

        bool navigateTo(const int x, const int y);

        void navigateTo(const Being *const being);

        void navigateClean();

        void imitateEmote(const Being *const being,
                          const unsigned char emote) const;

        void imitateAction(const Being *const being,
                           const Being::Action &action);

        void imitateDirection(const Being *const being,
                              const unsigned char dir);

        void imitateOutfit(Being *const player, const int sprite = -1) const;

        void followMoveTo(const Being *const being, const int x, const int y);

        void followMoveTo(const Being *const being, const int x1, const int y1,
                          const int x2, const int y2);

        bool allowAction();

        bool allowMove() const;

        void setRealPos(const int x, const int y);

        bool isServerBuggy() const
        { return mIsServerBuggy; }

        void fixPos(const int maxDist = 1);

        /**
         * Sets the map the being is on
         */
        void setMap(Map *const map);

        void addMessageToQueue(const std::string &message,
                               const int color = UserPalette::EXP_INFO);

        /**
         * Called when a option (set with config.addListener()) is changed
         */
        void optionChanged(const std::string &value);

        void processEvent(Channels channel, const DepricatedEvent &event);

        /**
         * set a following player.
         */
        void setFollow(std::string player);

        /**
         * set an imitation player.
         */
        void setImitate(std::string player);

        /**
         * setting the next destination of the following, in case of warp
         */
        void setNextDest(const int x, const int y);


        int getNextDestX() const
        { return mNextDestX; }

        int getNextDestY() const
        { return mNextDestY; }

        void respawn();

        FloorItem *getPickUpTarget() const
        { return mPickUpTarget; }

        void unSetPickUpTarget()
        { mPickUpTarget = nullptr; }

        /**
         * Stop following a player.
         */
        void cancelFollow();

        /**
         * Get the playername followed by the current player.
         */
        std::string getFollow() const
        { return mPlayerFollowed; }

        /**
         * Get the playername imitated by the current player.
         */
        std::string getImitate() const
        { return mPlayerImitated; }

        /**
         * Tells the engine whether to check
         * if the Player Name is to be displayed.
         */
        void setCheckNameSetting(const bool checked)
        { mUpdateName = checked; }

        /**
         * Gets if the engine has to check
         * if the Player Name is to be displayed.
         */
        bool getCheckNameSetting() const
        { return mUpdateName; }

        void fixAttackTarget();

        void updateNavigateList();

        int getPathLength(const Being *const being);

        void targetMoved() const;

        void setLastHitFrom(std::string n)
        { mLastHitFrom = n; }

        void waitFor(std::string nick);

        void checkNewName(Being *const being);

        void resetYellowBar();

        virtual unsigned char getWalkMask() const;

        void saveHomes();

        void removeHome();

        void stopAdvert();

        bool checAttackPermissions(const Being *const target) const;

        void updateStatus() const;

        std::string getInvertDirectionString();

        std::string getCrazyMoveTypeString();

        std::string getMoveToTargetTypeString();

        std::string getFollowModeString();

        std::string getAttackWeaponTypeString();

        std::string getAttackTypeString();

        std::string getQuickDropCounterString();

        std::string getPickUpTypeString();

        std::string getDebugPathString() const;

        std::string getMagicAttackString();

        std::string getPvpAttackString();

        std::string getImitationModeString();

        std::string getAwayModeString();

        std::string getCameraModeString() const;

        std::string getGameModifiersString();

    protected:
        void updateCoords();

        /** Whether or not the name settings have changed */
        bool mUpdateName;

        virtual void handleStatusEffect(StatusEffect *const effect,
                                        const int effectId);

        void startWalking(const unsigned char dir);

        void changeEquipmentBeforeAttack(const Being *const target) const;

        void tryMagic(const std::string &spell, const int baseMagic,
                      const int schoolMagic, const int mana) const;

        const char *getVarItem(const char **const arr, const unsigned index,
                               const unsigned sz) const;

        void changeMode(unsigned *const var, const unsigned limit,
                        const char *const conf,
                        std::string (LocalPlayer::*const func)(),
                        const unsigned def = 0,
                        const bool save = true);

        void crazyMove1();
        void crazyMove2();
        void crazyMove3();
        void crazyMove4();
        void crazyMove5();
        void crazyMove6();
        void crazyMove7();
        void crazyMove8();
        void crazyMove9();
        void crazyMoveA();

        void loadHomes();

        int mTargetTime;      /** How long the being has been targeted **/
        int mLastTarget;      /** Time stamp of last targeting action,
                                  -1 if none. */
        int mGMLevel;

        //move type
        unsigned int mInvertDirection;
        //crazy move type
        unsigned int mCrazyMoveType;
        //crazy move state
        unsigned int mCrazyMoveState;
        //attack weapon type
        unsigned int mAttackWeaponType;
        //quick drop counter
        unsigned int mQuickDropCounter;
        //move state. used if mInvertDirection == 2
        unsigned int mMoveState;
        //temporary disable crazy moves in moves
        bool mDisableCrazyMove;
        //pick up type 1x1, normal aka 2x1, forward aka 2x3, 3x3, 3x3 + 1
        unsigned int mPickUpType;
        //magic attack type
        unsigned int mMagicAttackType;
        //pvp attack type
        unsigned int mPvpAttackType;
        //type how move to target
        unsigned int mMoveToTargetType;
        unsigned int mAttackType;
        unsigned int mFollowMode;
        unsigned int mImitationMode;

        bool mDisableGameModifiers;

        int mLastTargetX;
        int mLastTargetY;

        std::map<std::string, Vector> mHomes;

        Being *mTarget;

        /** Follow system **/
        std::string mPlayerFollowed;
        std::string mPlayerImitated;
        int mNextDestX;
        int mNextDestY;

        FloorItem *mPickUpTarget;

        bool mGoingToTarget;
        bool mKeepAttacking;  /** Whether or not to continue to attack */
        int mLastAction;      /**< Time stamp of the last action, -1 if none.*/
        unsigned char mWalkingDir; /**< The direction the player is
                                        walking in. */
        bool mPathSetByMouse; /**< Tells if the path was set using mouse */

        std::vector<int> mStatusEffectIcons;

        int mLocalWalkTime;   /**< Timestamp used to control keyboard walk
                                  messages flooding */

        typedef std::pair<std::string, int> MessagePair;
        /** Queued messages*/
        std::list<MessagePair> mMessages;
        int mMessageTime;
        AwayListener *mAwayListener;
        OkDialog *mAwayDialog;

        int mPingSendTick;
        bool mWaitPing;
        int mPingTime;
        int mAfkTime;
        bool mAwayMode;
        bool mPseudoAwayMode;

        bool mShowNavigePath;
        bool mIsServerBuggy;
        bool mSyncPlayerMove;
        bool mDrawPath;
        bool mAttackMoving;
        bool mAttackNext;
        bool mShowJobExp;
        int mActivityTime;
        int mNavigateX;
        int mNavigateY;
        int mNavigateId;
        int mCrossX;
        int mCrossY;
        int mOldX;
        int mOldY;
        int mOldTileX;
        int mOldTileY;
        Path mNavigatePath;

        bool mTargetDeadPlayers;
        bool mServerAttack;
        std::string mLastHitFrom;
        std::string mWaitFor;
        int mAdvertTime;
        bool mBlockAdvert;
        bool mEnableAdvert;
        bool mTradebot;
        bool mTargetOnlyReachable;
        bool mNextStep;
};

extern LocalPlayer *player_node;

#endif
