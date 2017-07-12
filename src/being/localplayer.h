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

#ifndef BEING_LOCALPLAYER_H
#define BEING_LOCALPLAYER_H

#include "being/being.h"

#include "enums/being/pickup.h"
#include "enums/being/visiblename.h"

#include "enums/simpletypes/allowsort.h"
#include "enums/simpletypes/keep.h"

#include "listeners/actorspritelistener.h"
#include "listeners/attributelistener.h"
#include "listeners/playerdeathlistener.h"
#include "listeners/statlistener.h"

#include "localconsts.h"

class AwayListener;
class ChatTab;
class FloorItem;
class Map;
class OkDialog;

/**
 * The local player character.
 */
class LocalPlayer final : public Being,
                          public ActorSpriteListener,
                          public AttributeListener,
                          public PlayerDeathListener,
                          public StatListener
{
    public:
        /**
         * Constructor.
         */
        explicit LocalPlayer(const BeingId id,
                             const BeingTypeId subType = BeingTypeId_zero);

        A_DELETE_COPY(LocalPlayer)

        /**
         * Destructor.
         */
        ~LocalPlayer();

        void logic() override final;

        void slowLogic();

        void setAction(const BeingActionT &action,
                       const int attackType = 0) override final;

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
        void nextTile() override final
        { nextTile(0); }

        void nextTile(unsigned char dir);

        bool pickUp(FloorItem *const item);

        /**
         * Called when an ActorSprite has been destroyed.
         * @param actorSprite the ActorSprite being destroyed.
         */
        void actorSpriteDestroyed(const ActorSprite &actorSprite)
                                  override final;

        /**
         * Gets the attack range.
         */
        int getAttackRange() const A_WARN_UNUSED;

        int getAttackRange2() const A_WARN_UNUSED;

        void attack(Being *const target = nullptr, const bool keep = false,
                    const bool dontChangeEquipment = false);

        void attack2(Being *const target = nullptr, const bool keep = false,
                     const bool dontChangeEquipment = false);

        void setGroupId(const int id) override final;

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

        Being *setNewTarget(const ActorTypeT type,
                            const AllowSort allowSort);

        /**
         * Sets a new destination for this being to walk to.
         */
        void setDestination(const int x, const int y);

        /**
         * Sets a new direction to keep walking in.
         */
        void setWalkingDir(const unsigned char dir);

        /**
         * Gets the walking direction
         */
        unsigned char getWalkingDir() const noexcept2 A_WARN_UNUSED
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
        void pickedUp(const ItemInfo &itemInfo,
                      const int amount,
                      const ItemColor color,
                      const BeingId floorItemId,
                      const PickupT fail);

        int getLevel() const override final A_WARN_UNUSED;

        /** Tells that the path has been set by mouse. */
        void pathSetByMouse()
        { mPathSetByMouse = true; }

        /** Tells if the path has been set by mouse. */
        bool isPathSetByMouse() const noexcept2 A_WARN_UNUSED
        { return mPathSetByMouse; }

        unsigned int getMoveState() const noexcept2 A_WARN_UNUSED
        { return mMoveState; }

        void setMoveState(const unsigned int n) noexcept2
        { mMoveState = n; }

        std::string getPingTime() const A_WARN_UNUSED;

        void tryPingRequest();

#ifdef TMWA_SUPPORT
        void magicAttack() const;
#endif  // TMWA_SUPPORT

        void specialMove(const unsigned char direction);

        void moveByDirection(const unsigned char dir);

        bool pickUpItems(int pickUpType = 0);

        void move(const int dX, const int dY);

        void moveToTarget(int dist = -1);

        void moveToHome();

        bool isReachable(Being *const being,
                         const int maxCost = 0) A_WARN_UNUSED;

        bool isReachable(const int x, const int y,
                         const bool allowCollision) const A_WARN_UNUSED;

        void setHome();

        void pingRequest();

        void pingResponse();

        void setAway(const std::string &message) const;

        static void setPseudoAway(const std::string &message);

        void setHalfAway(const bool n)
        { mInactive = n; }

        bool getHalfAway() const noexcept2 A_WARN_UNUSED
        { return mInactive; }

        void afkRespond(ChatTab *const tab, const std::string &nick);

        static void setAfkMessage(std::string message);

        bool navigateTo(const int x, const int y);

        void navigateClean();

        void imitateEmote(const Being *const being,
                          const unsigned char action) const;

        void imitateAction(const Being *const being,
                           const BeingActionT &action);

        void imitateDirection(const Being *const being,
                              const unsigned char dir);

        void imitateOutfit(const Being *const player,
                           const int sprite = -1) const;

        void followMoveTo(const Being *const being, const int x, const int y);

        void followMoveTo(const Being *const being, const int x1, const int y1,
                          const int x2, const int y2);

        bool allowAction() A_WARN_UNUSED;

        void setRealPos(const int x, const int y);

        bool isServerBuggy() const noexcept2 A_WARN_UNUSED
        { return mIsServerBuggy; }

        void fixPos();

        /**
         * Sets the map the being is on
         */
        void setMap(Map *const map) override final;

        void addMessageToQueue(const std::string &message,
                               const UserColorIdT color
                               = UserColorId::EXP_INFO);

        /**
         * Called when a option (set with config.addListener()) is changed
         */
        void optionChanged(const std::string &value) override final;

        void playerDeath() override final;

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

        int getNextDestX() const noexcept2 A_WARN_UNUSED
        { return mNextDestX; }

        int getNextDestY() const noexcept2 A_WARN_UNUSED
        { return mNextDestY; }

        void respawn();

        const FloorItem *getPickUpTarget() const noexcept2 A_WARN_UNUSED
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
        const std::string &getFollow() const noexcept2 A_WARN_UNUSED
        { return mPlayerFollowed; }

        /**
         * Get the playername imitated by the current player.
         */
        const std::string &getImitate() const noexcept2 A_WARN_UNUSED
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
        bool getCheckNameSetting() const noexcept2 A_WARN_UNUSED
        { return mUpdateName; }

        void fixAttackTarget();

        void updateNavigateList();

        int getPathLength(const Being *const being) const A_WARN_UNUSED;

        void targetMoved() const A_CONST;

        void setLastHitFrom(const std::string &n)
        { mLastHitFrom = n; }

        void waitFor(const std::string &nick);

        void checkNewName(Being *const being);

        unsigned char getBlockWalkMask() const override final
                                       A_CONST A_WARN_UNUSED;

        void saveHomes();

        void removeHome();

        void stopAdvert();

        void addXpMessage(const int change);

        void addHomunXpMessage(const int change);

        void addJobMessage(const int change);

        void addHpMessage(const int change);

        void addSpMessage(const int change);

        static bool checAttackPermissions(const Being *const target)
                                          A_WARN_UNUSED;

        void updateStatus() const;

        void setTestParticle(const std::string &fileName,
                             const bool updateHash = true);

        int getLastAttackX() const override final
        { return mTarget != nullptr ? mTarget->mX : mLastAttackX; }

        int getLastAttackY() const override final
        { return mTarget != nullptr ? mTarget->mY : mLastAttackY; }

        void attributeChanged(const AttributesT id,
                              const int oldVal,
                              const int newVal) override final;

        void statChanged(const AttributesT id,
                         const int oldVal1,
                         const int oldVal2) override final;

        void updateMusic() const;

        void setAfkTime(const int v) noexcept2
        { mAfkTime = v; }

        void setAwayDialog(OkDialog *const dialog)
        { mAwayDialog = dialog; }

        AwayListener *getAwayListener() const noexcept2 A_WARN_UNUSED
        { return mAwayListener; }

        void setRename(const bool r) noexcept2
        { mAllowRename = r; }

        bool getRename() const noexcept2
        { return mAllowRename; }

        bool canMove() const;

        void freezeMoving(const int timeWaitTicks);

        void failMove(const int x,
                      const int y);

    protected:
        void updateCoords() override final;

        void handleStatusEffect(const StatusEffect *const effect,
                                const int32_t effectId,
                                const Enable newStatus,
                                const IsStart start) override final;

        void startWalking(const unsigned char dir);

        void changeEquipmentBeforeAttack(const Being *const target) const;

#ifdef TMWA_SUPPORT
        static void tryMagic(const std::string &spell,
                             const int baseMagic,
                             const int schoolMagic,
                             const int mana);
#endif  // TMWA_SUPPORT

        void loadHomes();

        // move state. used if mMoveType == 2
        unsigned int mMoveState;

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

        int mLastAction;      // Time stamp of the last action, -1 if none.

        std::vector<int32_t> mStatusEffectIcons;

        typedef std::pair<std::string, UserColorIdT> MessagePair;
        /** Queued messages*/
        std::list<MessagePair> mMessages;
        int mMessageTime;
        AwayListener *mAwayListener;
        OkDialog *mAwayDialog;

        int mPingSendTick;
        time_t mPingTime;
        time_t mAfkTime;
        time_t mActivityTime;
        int mNavigateX;
        int mNavigateY;
        BeingId mNavigateId;
        int mCrossX;
        int mCrossY;
        int mOldX;
        int mOldY;
        int mOldTileX;
        int mOldTileY;
        Path mNavigatePath;

        std::string mLastHitFrom;
        std::string mWaitFor;
        time_t mAdvertTime;
        Particle *mTestParticle;
        std::string mTestParticleName;
        time_t mTestParticleTime;
        unsigned long mTestParticleHash;
        int mSyncPlayerMoveDistance;
        int mUnfreezeTime;
        unsigned char mWalkingDir;  // The direction the player is walking in.
        /** Whether or not the name settings have changed */
        bool mUpdateName;
        bool mBlockAdvert;
        bool mTargetDeadPlayers;
        Keep mServerAttack;
        VisibleName::Type mVisibleNames;
        bool mEnableAdvert;
        bool mTradebot;
        bool mTargetOnlyReachable;
        bool mIsServerBuggy;
        bool mSyncPlayerMove;
        bool mDrawPath;
        bool mAttackMoving;
        bool mAttackNext;
        bool mShowJobExp;
        bool mShowServerPos;
        bool mNextStep;
        // temporary disable crazy moves in moves
        bool mGoingToTarget;
        // Whether or not to continue to attack
        bool mKeepAttacking;
        // Tells if the path was set using mouse
        bool mPathSetByMouse;
        bool mWaitPing;
        bool mShowNavigePath;
        bool mAllowRename;
        bool mFreezed;
};

extern LocalPlayer *localPlayer;

#endif  // BEING_LOCALPLAYER_H
