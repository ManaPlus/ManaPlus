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
        void action(const gcn::ActionEvent &event) override;
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

        A_DELETE_COPY(LocalPlayer)

        /**
         * Destructor.
         */
        ~LocalPlayer();

        virtual void logic() override;

        void slowLogic();

        virtual void setAction(const Action &action,
                               const int attackType = 0) override;

        /**
         * Compute the next pathnode location when walking using keyboard.
         * used by nextTile().
         */
        Position getNextWalkPosition(const unsigned char dir)
                                     const A_WARN_UNUSED;

        /**
         * Adds a new tile to the path when walking.
         * @note Eathena
         * Also, when specified, it picks up an item at the end of a path
         * or attack target.
         */
        virtual void nextTile() override
        { nextTile(0); }

        virtual void nextTile(unsigned char dir);

        bool pickUp(FloorItem *const item);

        /**
         * Called when an ActorSprite has been destroyed.
         * @param actorSprite the ActorSprite being destroyed.
         */
        void actorSpriteDestroyed(const ActorSprite &actorSprite) override;

        /**
         * Gets the attack range.
         */
        int getAttackRange() const A_WARN_UNUSED;

        int getAttackRange2() const A_WARN_UNUSED;

        void attack(Being *const target = nullptr, const bool keep = false,
                    const bool dontChangeEquipment = false);

        void attack2(Being *const target = nullptr, const bool keep = false,
                     const bool dontChangeEquipment = false);

        void setGMLevel(const int level);

        int getGMLevel() const A_WARN_UNUSED
        { return mGMLevel; }

        void stopAttack(const bool keepAttack = false);

        void untarget();

        /**
         * Returns the current target of the player. Returns 0 if no being is
         * currently targeted.
         */
        Being *getTarget() const A_WARN_UNUSED;

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
        unsigned char getWalkingDir() const A_WARN_UNUSED
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
                               const int addRange = 0) const A_WARN_UNUSED;

        /**
         * Stops the player dead in his tracks
         */
        void stopWalking(const bool sendToServer = true);

        bool toggleSit() const;

        bool updateSit() const;

        static bool emote(const uint8_t emotion);

        /**
         * Shows item pickup notifications.
         */
        void pickedUp(const ItemInfo &itemInfo, const int amount,
                      const unsigned char color, const int floorItemId,
                      const unsigned char fail);

        int getLevel() const override A_WARN_UNUSED;

        /** Tells that the path has been set by mouse. */
        void pathSetByMouse()
        { mPathSetByMouse = true; }

        /** Tells if the path has been set by mouse. */
        bool isPathSetByMouse() const A_WARN_UNUSED
        { return mPathSetByMouse; }

        int getInvertDirection() const A_WARN_UNUSED
        { return mInvertDirection; }

        void setInvertDirection(const int n)
        { mInvertDirection = n; }

        void invertDirection();

        int getAttackWeaponType() const A_WARN_UNUSED
        { return mAttackWeaponType; }

        int getAttackType() const A_WARN_UNUSED
        { return mAttackType; }

        int getFollowMode() const A_WARN_UNUSED
        { return mFollowMode; }

        int getImitationMode() const A_WARN_UNUSED
        { return mImitationMode; }

        void changeAttackWeaponType();

        void changeAttackType();

        void changeFollowMode();

        void changeImitationMode();

        void changePickUpType();

        int getCrazyMoveType() const A_WARN_UNUSED
        { return mCrazyMoveType; }

        int getPickUpType() const A_WARN_UNUSED
        { return mPickUpType; }

        int getQuickDropCounter() const A_WARN_UNUSED
        { return mQuickDropCounter; }

        void setQuickDropCounter(const int n);

        void changeQuickDropCounter();

        int getMoveState() const A_WARN_UNUSED
        { return mMoveState; }

        void setMoveState(const int n)
        { mMoveState = n; }

        void switchMagicAttack();

        void switchPvpAttack();

        int getMagicAttackType() const A_WARN_UNUSED
        { return mMagicAttackType; }

        int getPvpAttackType() const A_WARN_UNUSED
        { return mPvpAttackType; }

        int getMoveToTargetType() const A_WARN_UNUSED
        { return mMoveToTargetType; }

        int getDisableGameModifiers() const A_WARN_UNUSED
        { return mDisableGameModifiers; }

        std::string getPingTime() const A_WARN_UNUSED;

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

        void debugMsg(const std::string &str) const;

        bool isReachable(Being *const being,
                         const int maxCost = 0) A_WARN_UNUSED;

        bool isReachable(const int x, const int y,
                         const int allowCollision) const A_WARN_UNUSED;

        void setHome();

        void pingRequest();

        void pingResponse();

        void changeAwayMode();

        void setAway(const std::string &message);

        void setPseudoAway(const std::string &message);

        bool getAway() const A_WARN_UNUSED
        { return mAwayMode; }

        bool getPseudoAway() const A_WARN_UNUSED
        { return mPseudoAwayMode; }

        void setHalfAway(const bool n)
        { mInactive = n; }

        bool getHalfAway() const A_WARN_UNUSED
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

        bool allowAction() A_WARN_UNUSED;

        void setRealPos(const int x, const int y);

        bool isServerBuggy() const A_WARN_UNUSED
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
        void optionChanged(const std::string &value) override;

        void processEvent(Channels channel,
                          const DepricatedEvent &event) override;

        /**
         * set a following player.
         */
        void setFollow(const std::string &player);

        /**
         * set an imitation player.
         */
        void setImitate(const std::string &player);

        /**
         * setting the next destination of the following, in case of warp
         */
        void setNextDest(const int x, const int y);

        int getNextDestX() const A_WARN_UNUSED
        { return mNextDestX; }

        int getNextDestY() const A_WARN_UNUSED
        { return mNextDestY; }

        void respawn();

        FloorItem *getPickUpTarget() const A_WARN_UNUSED
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
        const std::string &getFollow() const A_WARN_UNUSED
        { return mPlayerFollowed; }

        /**
         * Get the playername imitated by the current player.
         */
        const std::string &getImitate() const A_WARN_UNUSED
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
        bool getCheckNameSetting() const A_WARN_UNUSED
        { return mUpdateName; }

        void fixAttackTarget();

        void updateNavigateList();

        int getPathLength(const Being *const being) const A_WARN_UNUSED;

        void targetMoved() const;

        void setLastHitFrom(const std::string &n)
        { mLastHitFrom = n; }

        void waitFor(const std::string &nick);

        void checkNewName(Being *const being);

        void resetYellowBar();

        unsigned char getWalkMask() const override A_WARN_UNUSED;

        void saveHomes();

        void removeHome();

        void stopAdvert();

        bool checAttackPermissions(const Being *const target)
                                   const A_WARN_UNUSED;

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
        void updateCoords() override;

        /** Whether or not the name settings have changed */
        bool mUpdateName;

        virtual void handleStatusEffect(StatusEffect *const effect,
                                        const int effectId);

        void startWalking(const unsigned char dir);

        void changeEquipmentBeforeAttack(const Being *const target) const;

        static void tryMagic(const std::string &spell, const int baseMagic,
                             const int schoolMagic, const int mana);

        const char *getVarItem(const char *const *const arr,
                               const unsigned index,
                               const unsigned sz) const A_WARN_UNUSED;

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
