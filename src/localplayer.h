/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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
#include "client.h"
#include "game.h"
#include "listener.h"

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

class AwayListener : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &event);
};

/**
 * The local player character.
 */
class LocalPlayer : public Being, public ActorSpriteListener,
        public Mana::Listener
{
    public:
        /**
         * Constructor.
         */
        LocalPlayer(int id = 65535, int subtype = 0);

        /**
         * Destructor.
         */
        ~LocalPlayer();

        virtual void logic();

        virtual void setAction(Action action, int attackType = 0);

        /**
         * Compute the next pathnode location when walking using keyboard.
         * used by nextTile().
         */
        Position getNextWalkPosition(unsigned char dir);

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
        bool checkInviteRights(const std::string &guildName);

        /**
         * Invite a player to join guild
         */
        void inviteToGuild(Being *being);

//        void clearInventory();
//        void setInvItem(int index, int id, int amount);

        bool pickUp(FloorItem *item);

        /**
         * Called when an ActorSprite has been destroyed.
         * @param actorSprite the ActorSprite being destroyed.
         */
        void actorSpriteDestroyed(const ActorSprite &actorSprite);

        /**
         * Sets the attack range.
         */
        void setAttackRange(int range) { mAttackRange = range; }

        /**
         * Gets the attack range.
         */
        int getAttackRange();

        void attack(Being *target = NULL, bool keep = false,
                    bool dontChangeEquipment = false);

        void attack2(Being *target = NULL, bool keep = false,
                     bool dontChangeEquipment = false);

        void setGMLevel(int level);

        int getGMLevel()
        { return mGMLevel; }

        void stopAttack();

        /**
         * Overridden to do nothing. The attacks of the local player are
         * displayed as soon as the player attacks, not when the server says
         * the player does.
         *
         * @param victim the victim being
         * @param damage the amount of damage dealt (0 means miss)
         * @param type the attack type
         */
        //virtual void handleAttack(Being *victim, int damage, AttackType type) {}
        virtual void handleAttack()
        { }

        /**
         * Returns the current target of the player. Returns 0 if no being is
         * currently targeted.
         */
        Being *getTarget() const;

        /**
         * Sets the target being of the player.
         */
        void setTarget(Being *target);

        /**
         * Sets a new destination for this being to walk to.
         */
        virtual void setDestination(int x, int y);

        /**
         * Sets a new direction to keep walking in.
         */
        void setWalkingDir(unsigned char dir);

        /**
         * Gets the walking direction
         */
        unsigned char getWalkingDir() const
        { return mWalkingDir; }

        /**
         * Sets going to being to attack
         */
        void setGotoTarget(Being *target);

        /**
         * Returns whether the target is in range to attack
         */
        bool withinAttackRange(Being *target, bool fixDistance = false,
                               int addRange = 0);

        /**
         * Stops the player dead in his tracks
         */
        void stopWalking(bool sendToServer = true);

        bool toggleSit();
        bool updateSit();
        bool emote(Uint8 emotion);

        /**
         * Shows item pickup notifications.
         */
        void pickedUp(const ItemInfo &itemInfo, int amount);

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

        int getInvertDirection()
        { return mInvertDirection; }

        void setInvertDirection(int n)
        { mInvertDirection = n; }

        void invertDirection();

        int getAttackWeaponType()
        { return mAttackWeaponType; }

        int getAttackType()
        { return mAttackType; }

        int getFollowMode()
        { return mFollowMode; }

        int getImitationMode()
        { return mImitationMode; }

        void changeAttackWeaponType();

        void changeAttackType();

        void changeFollowMode();

        void changeImitationMode();

        void changePickUpType();

        int getCrazyMoveType()
        { return mCrazyMoveType ; }

        int getPickUpType()
        { return mPickUpType ; }

        int getQuickDropCounter()
        { return mQuickDropCounter ; }

        void changeQuickDropCounter();

        int getMoveState()
        { return mMoveState ; }

        void setMoveState(int n)
        { mMoveState = n ; }

        void switchMagicAttack();

        int getMagicAttackType()
        { return mMagicAttackType ; }

        int getMoveToTargetType()
        { return mMoveToTargetType ; }

        int getDisableGameModifiers()
        { return mDisableGameModifiers ; }

        int getPingTime()
        { return mPingTime ; }

        void tryPingRequest();

        void changeMoveToTargetType();

        void switchGameModifiers();

        void magicAttack();

        void specialMove(unsigned char direction);

        void moveByDirection(unsigned char dir);

        bool pickUpItems(int pickUpType = 0);

        void changeCrazyMoveType();

        void crazyMove();

        void moveTo(int x, int y);

        void move(int dX, int dY);

        void moveToTarget(unsigned int dist = -1);

        void moveToHome();

        void debugMsg(std::string str);

//        int getSkillLv(int id);

        bool isReachable(int x, int y, int maxCost = 0);

        bool isReachable(Being *being, int maxCost = 0);

        void setHome();

        void pingRequest();

        void pingResponse();

        void changeAwayMode();

        bool getAwayMode()
        { return mAwayMode; }

        void setAway(const std::string &message);

        void afkRespond(ChatTab *tab, const std::string &nick);

        void navigateTo(int x, int y);

        void navigateTo(Being *being);

        void naviageClean();

        void updateCoords();

        void imitateEmote(Being* being, unsigned char emote);

        void imitateAction(Being *being, Being::Action action);

        void imitateDirection(Being *being, unsigned char dir);

        void imitateOutfit(Being *player, int sprite = -1);

        void followMoveTo(Being *being, int x, int y);

        void followMoveTo(Being *being, int x1, int y1, int x2, int y2);

        bool allowAction();

        bool allowMove();

        void setRealPos(int x, int y);

        bool isServerBuggy()
        { return mIsServerBuggy; }

        void fixPos(int maxDist = 1);

        /**
         * Sets the map the being is on
         */
        void setMap(Map *map);

        void addMessageToQueue(const std::string &message,
                               int color = UserPalette::EXP_INFO);

        /**
         * Called when a option (set with config.addListener()) is changed
         */
        void optionChanged(const std::string &value);

        void event(Channels channel, const Mana::Event &event);

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
        void setNextDest(int x, int y);


        int getNextDestX() const
        { return mNextDestX; }

        int getNextDestY() const
        { return mNextDestY; }

        void respawn();

        FloorItem *getPickUpTarget()
        { return mPickUpTarget; }

        void unSetPickUpTarget()
        { mPickUpTarget = 0; }

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
        void setCheckNameSetting(bool checked)
        { mUpdateName = checked; }

        /**
         * Gets if the engine has to check
         * if the Player Name is to be displayed.
         */
        bool getCheckNameSetting() const
        { return mUpdateName; }

        void fixAttackTarget();

        void updateNavigateList();

        int getPathLength(Being* being);

        void targetMoved();

        void setLastHitFrom(std::string n)
        { mLastHitFrom = n; }

        void waitFor(std::string nick);

        void checkNewName(Being *being);

    protected:
        /** Whether or not the name settings have changed */
        bool mUpdateName;

        virtual void handleStatusEffect(StatusEffect *effect, int effectId);

        void startWalking(unsigned char dir);

        void changeEquipmentBeforeAttack(Being* target);

        void tryMagic(std::string spell, int baseMagic,
                      int schoolMagic, int mana);

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

        void saveHomes();

        bool mInStorage;      /**< Whether storage is currently accessible */

        int mAttackRange;

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

        bool mShowNavigePath;
        bool mIsServerBuggy;
        bool mSyncPlayerMove;
        bool mDrawPath;
        bool mAttackMoving;
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
};

extern LocalPlayer *player_node;

#endif
