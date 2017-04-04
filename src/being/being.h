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

#ifndef BEING_BEING_H
#define BEING_BEING_H

#include "position.h"

#include "enums/being/badgeindex.h"
#include "enums/being/reachable.h"

#include "enums/gui/usercolorid.h"

#include "enums/simpletypes/move.h"

#include "resources/beinginfo.h"
#include "resources/beingslot.h"

#include "being/actorsprite.h"

#include "enums/being/attacktype.h"
#include "enums/being/beingaction.h"
#include "enums/being/gender.h"

#include "listeners/configlistener.h"

#include "localconsts.h"

static const int DEFAULT_BEING_WIDTH = 32;
static const int DEFAULT_BEING_HEIGHT = 32;

class AnimatedSprite;
class BeingCacheEntry;
class CastingEffect;
class Color;
class Equipment;
class FlashText;
class Guild;
class ItemInfo;
class Particle;
class Party;
class SpeechBubble;
class Text;

struct ChatObject;
struct HorseInfo;
struct SkillInfo;
struct ParticleInfo;

extern volatile time_t cur_time;

struct NextSoundInfo final
{
    NextSoundInfo() noexcept2 :
        sound(nullptr),
        x(0),
        y(0),
        time(0)
    {
    }

    A_DELETE_COPY(NextSoundInfo)

    const SoundInfo *sound;
    int x;
    int y;
    int time;
};

class Being notfinal : public ActorSprite,
                       public ConfigListener
{
    public:
        friend class ActorManager;
        friend class BeingEquipBackend;
        friend class LocalPlayer;

        /**
         * Constructor.
         *
         * @param id      a unique being id
         * @param subtype partly determines the type of the being
         * @param map     the map the being is on
         */
        Being(const BeingId id,
              const ActorTypeT type,
              const BeingTypeId subtype,
              Map *const map);

        A_DELETE_COPY(Being)

        virtual ~Being();

        ActorTypeT getType() const restrict2 noexcept2 override final
                           A_WARN_UNUSED
        { return mType; }

        /**
         * Removes all path nodes from this being.
         */
        void clearPath() restrict2;

        /**
         * Returns the time spent in the current action.
         */
        int getActionTime() const restrict2 noexcept2 A_WARN_UNUSED
        { return mActionTime; }

        /**
         * Set the current action time.
         * @see Ea::BeingHandler that set it to tick time.
         */
        void setActionTime(const int actionTime) restrict2 noexcept2
        { mActionTime = actionTime; }

        /**
         * Makes this being take the next tile of its path.
         */
        virtual void nextTile() restrict2;

        /**
         * Creates a path for the being from current position to ex and ey
         */
        void setDestination(const int dstX,
                            const int dstY) restrict2;

        void setCachedDestination(const int dstX,
                                  const int dstY) restrict2 noexcept2
        { mCachedX = dstX; mCachedY = dstY; }

        int getCachedX() const noexcept2 A_WARN_UNUSED
        { return mCachedX; }

        int getCachedY() const noexcept2 A_WARN_UNUSED
        { return mCachedY; }

        /**
         * Returns the destination for this being.
         */
        const Vector &getDestination() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDest; }

        /**
         * Returns the tile x coord
         */
        int getTileX() const restrict2 noexcept2 override A_WARN_UNUSED
        { return mX; }

        /**
         * Returns the tile y coord
         */
        int getTileY() const restrict2 noexcept2 override A_WARN_UNUSED
        { return mY; }

        /**
         * Sets the tile x and y coord
         */
        void setTileCoords(const int x, const int y) restrict2;

        /**
         * Puts a "speech balloon" above this being for the specified amount
         * of time.
         *
         * @param text The text that should appear.
         * @param time The amount of time the text should stay in milliseconds.
         */
        void setSpeech(const std::string &restrict text,
                       const std::string &restrict channel = "",
                       int time = 0) restrict2;

        /**
         * Puts a damage bubble above this being.
         *
         * @param attacker the attacking being
         * @param damage the amount of damage recieved (0 means miss)
         * @param type the attack type
         * @param attackId the attack id
         * @param level the skill level
         */
        void takeDamage(Being *restrict const attacker,
                        const int damage,
                        const AttackTypeT type,
                        const int attackId = 1,
                        const int level = 1) restrict2;

        /**
         * Handles an attack of another being by this being.
         *
         * @param victim the victim being
         * @param damage the amount of damage dealt (0 means miss)
         * @param attackId the attack id
         */
        void handleAttack(Being *restrict const victim,
                          const int damage,
                          const int attackId = 1) restrict2;

        void handleSkillCasting(Being *restrict const victim,
                                const int skillId,
                                const int skillLevel) restrict2;

        void handleSkill(Being *restrict const victim,
                         const int damage,
                         const int skillId,
                         const int skillLevel) restrict2;

        const ItemInfo *getEquippedWeapon() const restrict2 A_WARN_UNUSED
        { return mEquippedWeapon; }

        /**
         * Returns the name of the being.
         */
        const std::string &getName() const restrict2 noexcept2 A_WARN_UNUSED
        { return mName; }

        const std::string &getExtName() const restrict2 noexcept2 A_WARN_UNUSED
        { return mExtName; }

        /**
         * Sets the name for the being.
         *
         * @param name The name that should appear.
         */
        void setName(const std::string &restrict name) restrict2;

        bool getShowName() const noexcept2 A_WARN_UNUSED
        { return mShowName; }

        void setShowName(const bool doShowName) restrict2;

        /**
         * Sets the name of the party the being is in. Shown in BeingPopup.
         */
        void setPartyName(const std::string &restrict name) restrict2;

        const std::string &getPartyName() const restrict2 noexcept2
                                        A_WARN_UNUSED
        { return mPartyName; }

        const std::string &getGuildName() const restrict2 noexcept2
                                        A_WARN_UNUSED
        { return mGuildName; }

        /**
         * Sets the name of the primary guild the being is in. Shown in
         * BeingPopup (eventually).
         */
        void setGuildName(const std::string &restrict name) restrict2;

        void setGuildPos(const std::string &restrict pos) restrict2 A_CONST;

        /**
         * Adds a guild to the being.
         */
        void addGuild(Guild *restrict const guild) restrict2;

        /**
         * Removers a guild from the being.
         */
        void removeGuild(const int id) restrict2;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        const Guild *getGuild(const std::string &restrict guildName)
                              const restrict2 A_WARN_UNUSED;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        const Guild *getGuild(const int id) const restrict2 A_WARN_UNUSED;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        Guild *getGuild() const restrict2 A_WARN_UNUSED;

        /**
         * Returns all guilds the being is in.
         */
        const std::map<int, Guild*> &getGuilds() const restrict2 noexcept2
                                               A_WARN_UNUSED
        { return mGuilds; }

        /**
         * Removes all guilds the being is in.
         */
        void clearGuilds() restrict2;

        /**
         * Get number of guilds the being belongs to.
         */
        int16_t getNumberOfGuilds() const restrict2 A_WARN_UNUSED
        { return CAST_S16(mGuilds.size()); }

        bool isInParty() const restrict2 noexcept2 A_WARN_UNUSED
        { return mParty; }

        void setParty(Party *restrict const party) restrict2;

        void setGuild(Guild *restrict const guild) restrict2;

        void updateGuild() restrict2;

        Party *getParty() const restrict2 noexcept2 A_WARN_UNUSED
        { return mParty; }

        void setSprite(const unsigned int slot,
                       const int id,
                       std::string color,
                       ItemColor colorId) restrict2;

        void setSpriteColor(const unsigned int slot,
                            const int id,
                            const std::string &color) restrict2;

        void setSpriteColorId(const unsigned int slot,
                              const int id,
                              ItemColor colorId) restrict2;

        void setSpriteCards(const unsigned int slot,
                            const int id,
                            const CardsList &restrict cards) restrict2;

        void setSpriteId(const unsigned int slot,
                         const int id) restrict2;

        void unSetSprite(const unsigned int slot) restrict2;

        void setWeaponId(const int id) restrict2;

        void setTempSprite(const unsigned int slot,
                           const int id) restrict2;

        void setHairTempSprite(const unsigned int slot,
                               const int id) restrict2;

        void updateSprite(const unsigned int slot,
                          const int id,
                          const std::string &restrict color = "")
                          restrict2;

        void setHairColorSpriteID(const unsigned int slot,
                                  const int id) restrict2;

        void setSpriteColor(const unsigned int slot,
                            const std::string &restrict color = "") restrict2;

        /**
         * Get the number of layers used to draw the being
         */
        int getNumberOfLayers() const restrict2 override A_WARN_UNUSED
        { return CompoundSprite::getNumberOfLayers(); }

        /**
         * Performs being logic.
         */
        void logic() restrict2 override;

        void botLogic() restrict2;

        /**
         * Draws the speech text above the being.
         */
        void drawSpeech(const int offsetX,
                        const int offsetY) restrict2;

        /**
         * Draws the emotion picture above the being.
         */
        void drawEmotion(Graphics *restrict const graphics,
                         const int offsetX,
                         const int offsetY) const restrict2 A_NONNULL(2);

        BeingTypeId getSubType() const restrict2 noexcept2 A_WARN_UNUSED
        { return mSubType; }

         /**
          * Set Being's subtype (mostly for view for monsters and NPCs)
          */
        void setSubtype(const BeingTypeId subtype,
                        const uint16_t look) restrict2;

        const BeingInfo *getInfo() const restrict2 noexcept2 A_WARN_UNUSED
        { return mInfo; }

        TargetCursorSizeT getTargetCursorSize() const restrict2 override final
                                              A_WARN_UNUSED;

        int getTargetOffsetX() const restrict2 override A_WARN_UNUSED
        {
            if (!mInfo)
                return 0;
            return mInfo->getTargetOffsetX();
        }

        int getTargetOffsetY() const restrict2 override A_WARN_UNUSED
        {
            if (!mInfo)
                return 0;
            return mInfo->getTargetOffsetY();
        }

        /**
         * Gets the way the object is blocked by other objects.
         */
        virtual unsigned char getBlockWalkMask() const restrict2 A_WARN_UNUSED
        {
            if (!mInfo)
                return 0;
            return mInfo->getBlockWalkMask();
        }

        /**
         * Gets the way the monster blocks pathfinding for other objects
         */
        BlockTypeT getBlockType() const restrict2 override A_WARN_UNUSED
        {
            if (!mInfo)
                return BlockType::NONE;
            return mInfo->getBlockType();
        }

        /**
         * Sets the walk speed in pixels per second.
         */
        void setWalkSpeed(const int speed) restrict2
        { mWalkSpeed = speed; mSpeed = speed; }

        /**
         * Gets the walk speed in pixels per second.
         */
        int getWalkSpeed() const restrict2 noexcept2 A_WARN_UNUSED
        { return mWalkSpeed; }

        /**
         * Sets the attack speed.
         */
        void setAttackSpeed(const int speed) restrict2 noexcept2
        { mAttackSpeed = speed; }

        /**
         * Gets the attack speed.
         */
        int getAttackSpeed() const restrict2 noexcept2 A_WARN_UNUSED
        { return mAttackSpeed; }

        /**
         * Sets the current action.
         */
        virtual void setAction(const BeingActionT &restrict action,
                               const int attackType) restrict2;

        /**
         * Get the being's action currently performed.
         */
        BeingActionT getCurrentAction() const restrict2 noexcept2 A_WARN_UNUSED
        { return mAction; }

        /**
         * Returns whether this being is still alive.
         */
        bool isAlive() const restrict2 noexcept2 A_WARN_UNUSED
        { return mAction != BeingAction::DEAD; }

        /**
         * Returns the current direction.
         */
        uint8_t getDirection() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDirection; }

        /**
         * Sets the current direction.
         */
        virtual void setDirection(const uint8_t direction) restrict2;

        void setDirectionDelayed(const uint8_t direction) restrict2 noexcept2
        { mDirectionDelayed = direction; }

        uint8_t getDirectionDelayed() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDirectionDelayed; }

        /**
         * Returns the direction the being is facing.
         */
        SpriteDirection::Type getSpriteDirection() const restrict2 noexcept2
                                                 A_WARN_UNUSED
        { return mSpriteDirection; }

        void setPixelPositionF(const Vector &restrict pos) restrict2 override;

        /**
         * Overloaded method provided for convenience.
         *
         * @see setPosition(const Vector &pos)
         */
        inline void setPixelPositionF(const float x,
                                      const float y,
                                      const float z = 0.0F) restrict2 A_INLINE
        { setPixelPositionF(Vector(x, y, z)); }

        /**
         * Returns the horizontal size of the current base sprite of the being.
         */
        int getWidth() const restrict2 override final A_WARN_UNUSED
        { return std::max(CompoundSprite::getWidth(), DEFAULT_BEING_WIDTH); }

        /**
         * Returns the vertical size of the current base sprite of the being.
         */
        int getHeight() const restrict2 override final A_WARN_UNUSED
        { return std::max(CompoundSprite::getHeight(), DEFAULT_BEING_HEIGHT); }

        /**
         * Shoots a missile particle from this being, to target being
         */
        void fireMissile(Being *restrict const target,
                         const std::string &restrict particle) const restrict2;

        /**
         * Returns the path this being is following. An empty path is returned
         * when this being isn't following any path currently.
         */
        const Path &getPath() const restrict2 noexcept2 A_WARN_UNUSED
        { return mPath; }

        int getDistance() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDistance; }

        void setDistance(const int n) restrict2 noexcept2
        { mDistance = n; }

        /**
         * Set the Emoticon type and time displayed above
         * the being.
         */
        void setEmote(const uint8_t emotion,
                      const int emote_time) restrict2;

        void setState(const uint8_t state) restrict2;

        void drawPlayerSprites(Graphics *restrict const  graphics,
                               const int posX,
                               const int posY) const
                               restrict2 A_NONNULL(2);

        void drawSpritesSDL(Graphics *restrict const graphics,
                            const int posX,
                            const int posY) const
                            restrict2 override final A_NONNULL(2);

        void drawHpBar(Graphics *restrict const graphics,
                       const int maxHP,
                       const int hp,
                       const int damage,
                       const UserColorIdT color1,
                       const UserColorIdT color2,
                       const int x,
                       const int y,
                       const int width,
                       const int height) const restrict2 A_NONNULL(2);

        void optionChanged(const std::string &restrict value)
                           restrict2 override;

        void flashName(const int time) restrict2;

        int getDamageTaken() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDamageTaken; }

        void setDamageTaken(const int damage) restrict2 noexcept2
        { mDamageTaken = damage; }

        void updateName() restrict2;

        void setLevel(const int n) restrict2 noexcept2
        { mLevel = n; }

        virtual int getLevel() const restrict2 A_WARN_UNUSED
        { return mLevel; }

        void setReachable(const ReachableT n) restrict2 noexcept2
        { mReachable = n; }

        ReachableT getReachable() const restrict2 noexcept2 A_WARN_UNUSED
        { return mReachable; }

        static void reReadConfig();

        static BeingCacheEntry* getCacheEntry(const BeingId id) A_WARN_UNUSED;

        void addToCache() const restrict2;

        bool updateFromCache() restrict2;

        /**
         * Sets the gender of this being.
         */
        virtual void setGender(const GenderT gender) restrict2;

        GenderT getGender() const restrict2 noexcept2 A_WARN_UNUSED
        { return mGender; }

        /**
         * Return sprite sit action for current environment.
         */
        std::string getSitAction() const restrict2 A_WARN_UNUSED;

        std::string getCastAction() const restrict2 A_WARN_UNUSED;

        std::string getCastAction(const SkillInfo *restrict const skill) const
                                  restrict2 A_WARN_UNUSED;

        std::string getMoveAction() const restrict2 A_WARN_UNUSED;

        std::string getDeadAction() const restrict2 A_WARN_UNUSED;

        std::string getStandAction() const restrict2 A_WARN_UNUSED;

        std::string getSpawnAction() const restrict2 A_WARN_UNUSED;

        std::string getWeaponAttackAction(const ItemInfo *restrict const
                                          weapon) const
                                          restrict2 A_WARN_UNUSED;

        std::string getAttackAction() const restrict2 A_WARN_UNUSED;

        std::string getAttackAction(const Attack *restrict const attack) const
                                    restrict2 A_WARN_UNUSED;

        /**
         * Whether or not this player is a GM.
         */
        bool isGM() const restrict2 noexcept2 A_WARN_UNUSED
        { return mIsGM; }

        /**
         * Triggers whether or not to show the name as a GM name.
         */
        void setGM(const bool gm) restrict2;

        bool canTalk() const restrict2 noexcept2 A_WARN_UNUSED
        { return mType == ActorType::Npc; }

        void talkTo() const restrict2;

        void draw(Graphics *restrict const graphics,
                  const int offsetX,
                  const int offsetY) const
                  restrict2 override final A_NONNULL(2);

        void drawPlayer(Graphics *restrict const graphics,
                        const int offsetX,
                        const int offsetY) const
                        restrict2 A_NONNULL(2);

        void drawOther(Graphics *restrict const graphics,
                       const int offsetX,
                       const int offsetY) const
                       restrict2 A_NONNULL(2);

        void drawMonster(Graphics *restrict const graphics,
                         const int offsetX,
                         const int offsetY) const
                         restrict2 A_NONNULL(2);

        void drawPortal(Graphics *restrict const graphics,
                        const int offsetX,
                        const int offsetY) const
                        restrict2 A_NONNULL(2);

        void drawBasic(Graphics *restrict const graphics,
                       const int x,
                       const int y) const restrict2 A_NONNULL(2);

        void setMoveTime() restrict2 noexcept2
        { mMoveTime = cur_time; }

        void setAttackTime() restrict2 noexcept2
        { mAttackTime = cur_time; }

        void setTalkTime() restrict2 noexcept2
        { mTalkTime = cur_time; }

        void setTestTime() restrict2 noexcept2
        { mTestTime = cur_time; }

        void setOtherTime() restrict2 noexcept2
        { mOtherTime = cur_time; }

        time_t getMoveTime() const restrict2 noexcept2
        { return mMoveTime; }

        time_t getAttackTime() const restrict2 noexcept2
        { return mAttackTime; }

        time_t getTalkTime() const restrict2 noexcept2
        { return mTalkTime; }

        time_t getTestTime() const restrict2 noexcept2
        { return mTestTime; }

        time_t getOtherTime() const restrict2 noexcept2
        { return mOtherTime; }

        void resetCounters() restrict2;

        void updateColors() restrict2;

        void setEnemy(const bool n) restrict2 noexcept2
        { mEnemy = n; }

        const std::string &getIp() const restrict2 noexcept2 A_WARN_UNUSED
        { return mIp; }

        void setIp(const std::string &restrict ip) restrict2 noexcept2
        { mIp = ip; }

        unsigned int getPvpRank() const restrict2 noexcept2 A_WARN_UNUSED
        { return mPvpRank; }

        void setPvpRank(const unsigned int rank) restrict2 noexcept2
        { mPvpRank = rank; }

        void setHP(const int n) restrict2;

        void setMaxHP(const int hp) restrict2;

        int getHP() const restrict2 noexcept2 A_WARN_UNUSED
        { return mHP; }

        int getMaxHP() const restrict2 noexcept2 A_WARN_UNUSED
        { return mMaxHP; }

        uint8_t calcDirection(const int dstX,
                              const int dstY) const restrict2 A_WARN_UNUSED;

        uint8_t calcDirection() const restrict2 A_WARN_UNUSED;

        void setAttackDelay(const int n) restrict2 noexcept2
        { mAttackDelay = n; }

        int getAttackDelay() const restrict2 noexcept2 A_WARN_UNUSED
        { return mAttackDelay; }

        int getMinHit() const restrict2 noexcept2 A_WARN_UNUSED
        { return mMinHit; }

        void setMinHit(const int n) restrict2 noexcept2
        { mMinHit = n; }

        int getMaxHit() const restrict2 noexcept2 A_WARN_UNUSED
        { return mMaxHit; }

        void setMaxHit(const int n) restrict2 noexcept2
        { mMaxHit = n; }

        int getCriticalHit() const restrict2 A_WARN_UNUSED
        { return mCriticalHit; }

        void setCriticalHit(const int n) restrict2 noexcept2
        { mCriticalHit = n; }

        void updateHit(const int amount) restrict2;

        Equipment *getEquipment() restrict2 A_WARN_UNUSED;

        void undressItemById(const int id) restrict2;

        int getGoodStatus() const restrict2 noexcept2 A_WARN_UNUSED
        { return mGoodStatus; }

        void setGoodStatus(const int n) restrict2 noexcept2
        { mGoodStatus = n; }

        std::string getGenderSign() const restrict2 A_WARN_UNUSED;

        std::string getGenderSignWithSpace() const restrict2 A_WARN_UNUSED;

        void updateComment() restrict2;

        const std::string getComment() const restrict2 noexcept2 A_WARN_UNUSED
        { return mComment; }

        void setComment(const std::string &restrict n) restrict2 noexcept2
        { mComment = n; }

        static void clearCache();

        static std::string loadComment(const std::string &restrict name,
                                       const ActorTypeT &restrict type)
                                       A_WARN_UNUSED;

        static void saveComment(const std::string &restrict name,
                                const std::string &restrict comment,
                                const ActorTypeT &restrict type);

        bool isAdvanced() const restrict2 noexcept2 A_WARN_UNUSED
        { return mAdvanced; }

        void setAdvanced(const bool n) restrict2
        { mAdvanced = n; addToCache(); }

        bool isBuyShopEnabled() const restrict2 A_WARN_UNUSED;

        bool isSellShopEnabled() const restrict2 A_WARN_UNUSED;

        void enableShop(const bool b) restrict2;

        /**
         * Sets the attack range.
         */
        void setAttackRange(const int range) restrict2 noexcept2
        { mAttackRange = range; }

/*
        void attack(Being *restrict target = nullptr,
                    bool keep = false,
                    bool dontChangeEquipment = false) restrict2;

        void attack2(Being *restrict target = nullptr,
                     bool keep = false,
                     bool dontChangeEquipment = false) restrict2;
*/

        void updatePercentHP() restrict2;

        void setRaceName(const std::string &restrict name) restrict2 noexcept2
        { mRaceName = name; }

        std::string getRaceName() const restrict2 noexcept2 A_WARN_UNUSED
        { return mRaceName; }

        int getSpriteID(const int slot) const restrict2 A_WARN_UNUSED;

        const BeingSlot &getSpriteSlot(const int slot) const
                                       restrict2 A_WARN_UNUSED;

        ItemColor getSpriteColor(const int slot) const restrict2 A_WARN_UNUSED;

        void setHairStyle(const unsigned int slot,
                          const int id) restrict2;

        void setHairColor(const unsigned int slot,
                          const ItemColor color) restrict2;

        void setHairColor(const ItemColor color) restrict2 noexcept2
        { mHairColor = color; }

        void setSpriteSlot(const unsigned int slot,
                           const BeingSlot &beingSlot);

        ItemColor getHairColor() const noexcept2 A_WARN_UNUSED
        { return mHairColor; }

        void recalcSpritesOrder() restrict2;

        int getHitEffect(const Being *restrict const attacker,
                         const AttackTypeT type,
                         const int attackId,
                         const int level) const restrict2 A_WARN_UNUSED;

        CursorT getHoverCursor() const restrict2 A_WARN_UNUSED
        { return mInfo ? mInfo->getHoverCursor() : Cursor::CURSOR_POINTER; }

        void addAfkEffect() restrict2;

        void removeAfkEffect() restrict2;

        void updateAwayEffect() restrict2;

        void addSpecialEffect(const int effect) restrict2;

        void removeSpecialEffect() restrict2;

        void addEffect(const std::string &restrict name) restrict2;

        void setOwner(Being *restrict const owner) restrict2 noexcept2
        { mOwner = owner; }

        Being *getOwner() const restrict2 noexcept2
        { return mOwner; }

        void playSfx(const SoundInfo &sound,
                     Being *const being,
                     const bool main,
                     const int x, const int y) const restrict2;

        uint16_t getLook() const restrict2 noexcept2
        { return mLook; }

        void setLook(const uint16_t look) restrict2;

        constexpr2 static uint8_t genderToInt(const GenderT sex)
                                              A_CONST A_WARN_UNUSED
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

        constexpr2 static GenderT intToGender(const uint8_t sex)
                                              A_CONST A_WARN_UNUSED
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

        NextSoundInfo mNextSound;

        /**
         * Sets the new path for this being.
         */
        void setPath(const Path &restrict path) restrict2;

        int getSortPixelY() const restrict2 override A_WARN_UNUSED
        { return CAST_S32(mPos.y) - mYDiff - mSortOffsetY; }

        void setMap(Map *restrict const map) restrict2 override;

        void recreateItemParticles() restrict2;

        void incUsage() restrict2 noexcept2
        { mUsageCounter ++; }

        int decUsage() restrict2 noexcept2
        { return --mUsageCounter; }

        virtual int getLastAttackX() const restrict2
        { return mLastAttackX; }

        virtual int getLastAttackY() const restrict2
        { return mLastAttackY; }

        void drawHomunculus(Graphics *restrict const graphics,
                            const int offsetX,
                            const int offsetY) const
                            restrict2 A_NONNULL(2);

        void drawMercenary(Graphics *restrict const graphics,
                           const int offsetX,
                           const int offsetY) const
                           restrict2 A_NONNULL(2);

        void drawElemental(Graphics *restrict const graphics,
                           const int offsetX,
                           const int offsetY) const
                           restrict2 A_NONNULL(2);

        void setTrickDead(const bool b) restrict2 override final;

        void setChat(ChatObject *restrict const obj) restrict2;

        ChatObject *getChat() const restrict2 noexcept2
        { return mChat; }

        void setRiding(const bool b) restrict2 override final;

        void setHorse(const int horseId) restrict2;

        void removeHorse() restrict2;

        void setSellBoard(const std::string &restrict text) restrict2;

        std::string getSellBoard() const restrict2 noexcept2 A_WARN_UNUSED
        { return mSellBoard; }

        void setBuyBoard(const std::string &restrict text) restrict2;

        std::string getBuyBoard() const restrict2 noexcept2 A_WARN_UNUSED
        { return mBuyBoard; }

        void setSpiritBalls(const unsigned int balls) restrict2;

        unsigned int getSpiritBalls() const restrict2 noexcept2 A_WARN_UNUSED
        { return mSpiritBalls; }

        void stopCast(const bool b) override final;

        void setCreatorId(const BeingId id)
        { mCreatorId = id; }

        BeingId getCreatorId() const noexcept2 A_WARN_UNUSED
        { return mCreatorId; }

        void setKarma(const int karma) restrict2 noexcept2
        { mKarma = karma; }

        int getKarma() const restrict2 noexcept2 A_WARN_UNUSED
        { return mKarma; }

        void setManner(const int manner) restrict2 noexcept2
        { mManner = manner; }

        int getManner() const restrict2 noexcept2 A_WARN_UNUSED
        { return mManner; }

        void disableBotAi() restrict2 noexcept2
        { mBotAi = false; }

        void enableBotAi() restrict2 noexcept2
        { mBotAi = true; }

        int getAreaSize() const restrict2 noexcept2 A_WARN_UNUSED
        { return mAreaSize; }

        void setAreaSize(const int areaSize) restrict2 noexcept2
        { mAreaSize = areaSize; }

        void setTeamId(const uint16_t teamId) restrict2;

        void showTeamBadge(const bool show) restrict2;

        void showGuildBadge(const bool show) restrict2;

        void showGmBadge(const bool show) restrict2;

        void showPartyBadge(const bool show) restrict2;

        void showNameBadge(const bool show) restrict2;

        void showShopBadge(const bool show) restrict2;

        void showInactiveBadge(const bool show) restrict2;

        void showAwayBadge(const bool show) restrict2;

        void showBadges(const bool show) restrict2;

        void setLanguageId(const int lang) restrict2 noexcept2;

        uint16_t getTeamId() const restrict2 noexcept2 A_WARN_UNUSED
        { return mTeamId; }

        void serverRemove() restrict2 noexcept2;

        void addCast(const int dstX,
                     const int dstY,
                     const int skillId,
                     const int skillLevel,
                     const int range,
                     const int waitTimeTicks);

        void fixDirectionOffsets(int &offsetX,
                                 int &offsetY) const;

    protected:
        void drawPlayerSpriteAt(Graphics *restrict const graphics,
                                const int x,
                                const int y) const restrict2 A_NONNULL(2);

        void drawOtherSpriteAt(Graphics *restrict const graphics,
                               const int x,
                               const int y) const restrict2 A_NONNULL(2);

        void drawPortalSpriteAt(Graphics *restrict const graphics,
                                const int x,
                                const int y) const restrict2 A_NONNULL(2);

        void drawMonsterSpriteAt(Graphics *restrict const graphics,
                                 const int x,
                                 const int y) const restrict2 A_NONNULL(2);

        void drawHomunculusSpriteAt(Graphics *restrict const graphics,
                                    const int x,
                                    const int y) const restrict2 A_NONNULL(2);

        void drawMercenarySpriteAt(Graphics *restrict const graphics,
                                   const int x,
                                   const int y) const restrict2 A_NONNULL(2);

        void drawElementalSpriteAt(Graphics *restrict const graphics,
                                   const int x,
                                   const int y) const restrict2 A_NONNULL(2);

        void drawCompound(Graphics *const graphics,
                          const int posX,
                          const int posY) const A_NONNULL(2);

        /**
         * Updates name's location.
         */
        virtual void updateCoords() restrict2;

        void showName() restrict2;

        void addItemParticles(const int id,
                              const SpriteDisplay &restrict display) restrict2;

        void addItemParticlesCards(const int id,
                                   const SpriteDisplay &restrict display,
                                   const CardsList &restrict cards) restrict2;

        void removeAllItemsParticles() restrict2;

        void removeItemParticles(const int id) restrict2;

        void createSpeechBubble() restrict2;

        void setDefaultNameColor(const UserColorIdT defaultColor) restrict2;

        void updateBadgesCount() restrict2;

        static int getDefaultEffectId(const AttackTypeT &restrict type);

        BeingInfo *restrict mInfo;
        AnimatedSprite *restrict mEmotionSprite;
        AnimatedSprite *restrict mAnimationEffect;
        CastingEffect *restrict mCastingEffect;
        AnimatedSprite *restrict mBadges[BadgeIndex::BadgeIndexSize];

        std::string mSpriteAction;
        std::string mName;              /**< Name of being */
        std::string mExtName;           /**< Full name of being */
        std::string mRaceName;
        std::string mPartyName;
        std::string mGuildName;
        std::string mSpeech;

        /**
         * Holds a text object when the being displays it's name, 0 otherwise
         */
        FlashText *restrict mDispName;
        const Color *restrict mNameColor;

        /** Engine-related infos about weapon. */
        const ItemInfo *restrict mEquippedWeapon;

        Path mPath;
        Text *restrict mText;
        const Color *restrict mTextColor;

        Vector mDest;  /**< destination coordinates. */

        typedef std::map<int, ParticleInfo*> SpriteParticleInfo;
        typedef SpriteParticleInfo::iterator SpriteParticleInfoIter;

        std::vector<BeingSlot> mSlots;
        SpriteParticleInfo mSpriteParticles;

        // Character guild information
        std::map<int, Guild*> mGuilds;
        Party *mParty;

        int mActionTime;      /**< Time spent in current action */
        int mEmotionTime;     /**< Time until emotion disappears */

        /** Time until the last speech sentence disappears */
        int mSpeechTime;
        int mAttackSpeed;     /**< Attack speed */

        int mLevel;
        int mAttackRange;
        int mLastAttackX;
        int mLastAttackY;

        int mPreStandTime;

        GenderT mGender;
        BeingActionT mAction;
        BeingTypeId mSubType;       /**< Subtype (graphical view, basically) */
        uint8_t mDirection;         /**< Facing direction */
        uint8_t mDirectionDelayed;  /**< Facing direction */
        SpriteDirection::Type mSpriteDirection;  /**< Facing direction */
        bool mShowName;
        bool mIsGM;

    protected:
        /**
         * Calculates the offset in the given directions.
         * If walking in direction 'neg' the value is negated.
         */
        template<signed char pos, signed char neg>
        int getOffset() const restrict2 A_WARN_UNUSED;

        int searchSlotValue(const std::vector<int> &restrict slotRemap,
                            const int val) const restrict2 A_WARN_UNUSED;

        static void searchSlotValueItr(std::vector<int>::iterator &restrict it,
                                       int &idx,
                                       std::vector<int> &restrict slotRemap,
                                       const int val);

        void addSpiritBalls(const unsigned int balls,
                            const int effectId) restrict2;

        void removeSpiritBalls(const unsigned int balls) restrict2;

        void dumpSprites() const restrict2;

        void drawBeingCursor(Graphics *const graphics,
                             const int offsetX,
                             const int offsetY) const A_NONNULL(2);

        void drawCasting(Graphics *const graphics,
                         const int offsetX,
                         const int offsetY) const A_NONNULL(2);

        void updateBotFollow(int dstX,
                             int dstY,
                             const int divX,
                             const int divY);

        void moveBotTo(int dstX,
                       int dstY);

        void updateBotDirection(const int dstX,
                                const int dstY);

        void botFixOffset(int &restrict dstX,
                          int &restrict dstY) const;

        const ActorTypeT mType;

        /** Speech Bubble components */
        SpeechBubble *restrict mSpeechBubble;

        /**
         * Walk speed for x and y movement values.
         * In pixels per second.
         * @see MILLISECONDS_IN_A_TICK
         */
        int mWalkSpeed;
        int mSpeed;
        std::string mIp;
        int *restrict mSpriteRemap A_NONNULLPOINTER;
        int *restrict mSpriteHide A_NONNULLPOINTER;
        int *restrict mSpriteDraw A_NONNULLPOINTER;
        std::string mComment;
        std::string mBuyBoard;
        std::string mSellBoard;
        Being *restrict mOwner;
        Particle *restrict mSpecialParticle;
        ChatObject *restrict mChat;
        HorseInfo *restrict mHorseInfo;
        std::vector<AnimatedSprite*> mDownHorseSprites;
        std::vector<AnimatedSprite*> mUpHorseSprites;
        std::vector<Particle*> mSpiritParticles;

        int mX;             // position in tiles
        int mY;             // position in tiles
        int mCachedX;
        int mCachedY;
        int mSortOffsetY;  // caculated offset in pixels based on mPixelOffsetY
        int mPixelOffsetY;  // tile height offset in pixels
                            // calculated between tiles
        int mFixedOffsetY;  // fixed tile height offset in pixels for tile
        uint8_t mOldHeight;

        int mDamageTaken;
        int mHP;
        int mMaxHP;
        int mDistance;
        ReachableT mReachable;
        int mGoodStatus;

        static time_t mUpdateConfigTime;
        static unsigned int mConfLineLim;
        static int mSpeechType;
        static bool mHighlightMapPortals;
        static bool mHighlightMonsterAttackRange;
        static bool mLowTraffic;
        static bool mDrawHotKeys;
        static bool mShowBattleEvents;
        static bool mShowMobHP;
        static bool mShowOwnHP;
        static bool mShowGender;
        static bool mShowLevel;
        static bool mShowPlayersStatus;
        static bool mEnableReorderSprites;
        static bool mHideErased;
        static Move mMoveNames;
        static bool mUseDiagonal;
        static uint8_t mShowBadges;
        static int mAwayEffect;

        time_t mMoveTime;
        time_t mAttackTime;
        time_t mTalkTime;
        time_t mOtherTime;
        time_t mTestTime;
        int mAttackDelay;
        int mMinHit;
        int mMaxHit;
        int mCriticalHit;
        unsigned int mPvpRank;
        unsigned int mNumber;
        unsigned int mSpiritBalls;
        int mUsageCounter;
        int mKarma;
        int mManner;
        int mAreaSize;
        int mCastEndTime;
        int mLanguageId;
        BeingId mCreatorId;
        uint16_t mTeamId;
        uint16_t mLook;
        uint16_t mBadgesCount;
        ItemColor mHairColor;
        bool mErased;
        bool mEnemy;
        bool mGotComment;
        bool mAdvanced;
        bool mShop;
        bool mAway;
        bool mInactive;
        bool mNeedPosUpdate;
        bool mBotAi;
};

extern std::list<BeingCacheEntry*> beingInfoCache;

#endif  // BEING_BEING_H
