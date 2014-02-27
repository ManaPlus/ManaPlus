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

#ifndef BEING_BEING_H
#define BEING_BEING_H

#include "resources/beinginfo.h"

#include <map>

#include "localconsts.h"

static const unsigned int FIRST_IGNORE_EMOTE = 14;
static const unsigned int STATUS_EFFECTS = 32;

static const unsigned int SPEECH_TIME = 500;
static const unsigned int SPEECH_MIN_TIME = 200;
static const unsigned int SPEECH_MAX_TIME = 800;

static const int DEFAULT_BEING_WIDTH = 32;
static const int DEFAULT_BEING_HEIGHT = 32;

class AnimatedSprite;
class BeingCacheEntry;
class Color;
class Equipment;
class FlashText;
class Guild;
class ItemInfo;
class Particle;
class Party;
class SpeechBubble;
class Text;

struct ParticleInfo;

extern volatile int cur_time;

enum Gender
{
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_UNSPECIFIED = 2,
    GENDER_OTHER = 3
};


struct NextSoundInfo
{
    NextSoundInfo() :
        sound(nullptr),
        x(0),
        y(0),
        time(0)
    {
    }

    const SoundInfo *sound;
    int x;
    int y;
    int time;
};

class Being : public ActorSprite, public ConfigListener
{
    public:
        friend class BeingEquipBackend;
        friend class LocalPlayer;

        enum FLAGS
        {
            FLAG_SHOP = 1,
            FLAG_AWAY = 2,
            FLAG_INACTIVE = 4,
            FLAG_GENDER_OTHER = 32,
            FLAG_GM = 64,
            FLAG_GENDER_MALE = 128,
            FLAG_SPECIAL = 128 + 64
        };

        /**
         * Action the being is currently performing
         * WARNING: Has to be in sync with the same enum in the Being class
         * of the server!
         */
        enum Action
        {
            STAND = 0,
            MOVE,
            ATTACK,
            SIT,
            DEAD,
            HURT,
            SPAWN
        };

        enum Speech
        {
            NO_SPEECH = 0,
            TEXT_OVERHEAD,
            NO_NAME_IN_BUBBLE,
            NAME_IN_BUBBLE
        };

        enum AttackType
        {
            HIT = 0x00,
            CRITICAL = 0x0a,
            MULTI = 0x08,
            REFLECT = 0x04,
            FLEE = 0x0b,
            SKILL = 0xff,
            MISS = 0xffff   // pseudo value for miss attacks
        };

        enum Reachable
        {
            REACH_UNKNOWN = 0,
            REACH_YES = 1,
            REACH_NO = 2
        };

        /**
         * Directions, to be used as bitmask values
         */
        enum BeingDirection
        {
            DOWN = 1,
            LEFT = 2,
            UP = 4,
            RIGHT = 8
        };

        /**
         * Constructor.
         *
         * @param id      a unique being id
         * @param subtype partly determines the type of the being
         * @param map     the map the being is on
         */
        Being(const int id, const Type type, const uint16_t subtype,
              Map *const map);

        A_DELETE_COPY(Being)

        virtual ~Being();

        Type getType() const A_WARN_UNUSED
        { return mType; }

        /**
         * Removes all path nodes from this being.
         */
        void clearPath();

        /**
         * Returns the time spent in the current action.
         */
        int getActionTime() const A_WARN_UNUSED
        { return mActionTime; }

        /**
         * Set the current action time.
         * @see Ea::BeingHandler that set it to tick time.
         */
        void setActionTime(const int actionTime)
        { mActionTime = actionTime; }

        /**
         * Makes this being take the next tile of its path.
         * TODO: Used by eAthena only?
         */
        virtual void nextTile();

        /**
         * Get the current X pixel offset.
         * TODO: Used by eAthena only?
         */
        int getXOffset() const A_WARN_UNUSED
        { return getOffset(LEFT, RIGHT); }

        /**
         * Get the current Y pixel offset.
         * TODO: Used by eAthena only?
         */
        int getYOffset() const A_WARN_UNUSED
        { return getOffset(UP, DOWN); }

        /**
         * Creates a path for the being from current position to ex and ey
         */
        void setDestination(const int dstX, const int dstY);

        /**
         * Returns the destination for this being.
         */
        const Vector &getDestination() const A_WARN_UNUSED
        { return mDest; }

        /**
         * Returns the tile x coord
         */
        int getTileX() const A_WARN_UNUSED
        { return mX; }

        /**
         * Returns the tile y coord
         */
        int getTileY() const A_WARN_UNUSED
        { return mY; }

        /**
         * Sets the tile x and y coord
         */
        void setTileCoords(const int x, const int y);

        /**
         * Puts a "speech balloon" above this being for the specified amount
         * of time.
         *
         * @param text The text that should appear.
         * @param time The amount of time the text should stay in milliseconds.
         */
        void setSpeech(const std::string &text,
                       const std::string &channel = "",
                       int time = 0);

        /**
         * Puts a damage bubble above this being.
         *
         * @param attacker the attacking being
         * @param damage the amount of damage recieved (0 means miss)
         * @param type the attack type
         * @param id skill id
         */
        void takeDamage(Being *const attacker, const int damage,
                        const AttackType type, const int attackId = 1);

        /**
         * Handles an attack of another being by this being.
         *
         * @param victim the victim being
         * @param damage the amount of damage dealt (0 means miss)
         * @param type the attack type
         */
        void handleAttack(Being *const victim, const int damage,
                          const int attackId = 1);

        void handleSkill(Being *const victim, const int damage,
                         const int skillId, const int skillLevel);

        const ItemInfo *getEquippedWeapon() const A_WARN_UNUSED
        { return mEquippedWeapon; }

        /**
         * Returns the name of the being.
         */
        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        /**
         * Sets the name for the being.
         *
         * @param name The name that should appear.
         */
        void setName(const std::string &name);

        bool getShowName() const A_WARN_UNUSED
        { return mShowName; }

        void setShowName(const bool doShowName);

        /**
         * Sets the name of the party the being is in. Shown in BeingPopup.
         */
        void setPartyName(const std::string &name)
        { mPartyName = name; }

        const std::string &getPartyName() const A_WARN_UNUSED
        { return mPartyName; }

        const std::string &getGuildName() const A_WARN_UNUSED
        { return mGuildName; }

        /**
         * Sets the name of the primary guild the being is in. Shown in
         * BeingPopup (eventually).
         */
        void setGuildName(const std::string &name);

        void setGuildPos(const std::string &pos);

        /**
         * Adds a guild to the being.
         */
        void addGuild(Guild *const guild);

        /**
         * Removers a guild from the being.
         */
        void removeGuild(const int id);

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        const Guild *getGuild(const std::string &guildName)
                              const A_WARN_UNUSED;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        const Guild *getGuild(const int id) const A_WARN_UNUSED;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        Guild *getGuild() const A_WARN_UNUSED;

        /**
         * Returns all guilds the being is in.
         */
        const std::map<int, Guild*> &getGuilds() const A_WARN_UNUSED
        { return mGuilds; }

        /**
         * Removes all guilds the being is in.
         */
        void clearGuilds();

        /**
         * Get number of guilds the being belongs to.
         */
        int16_t getNumberOfGuilds() const A_WARN_UNUSED
        { return static_cast<int16_t>(mGuilds.size()); }

        bool isInParty() const A_WARN_UNUSED
        { return mParty; }

        void setParty(Party *const party);

        void setGuild(Guild *const guild);

        void updateGuild();

        Party *getParty() const A_WARN_UNUSED
        { return mParty; }

        int getSpritesCount() const A_WARN_UNUSED
        { return static_cast<int>(size()); }

        /**
         * Sets visible equipments for this being.
         */
        void setSprite(const unsigned int slot, const int id,
                       std::string color = "",
                       const unsigned char colorId = 1,
                       const bool isWeapon = false,
                       const bool isTempSprite = false);

        void updateSprite(const unsigned int slot, const int id,
                          std::string color = "",
                          const unsigned char colorId = 1,
                          const bool isWeapon = false,
                          const bool isTempSprite = false);

        void setSpriteID(const unsigned int slot, const int id);

        void setSpriteColor(const unsigned int slot,
                            const std::string &color = "");

        /**
         * Get the number of hairstyles implemented
         */
        static int getNumOfHairstyles() A_WARN_UNUSED
        { return mNumberOfHairstyles; }

        /**
         * Get the number of races implemented
         */
        static int getNumOfRaces() A_WARN_UNUSED
        { return mNumberOfRaces; }

        /**
         * Get the number of layers used to draw the being
         */
        int getNumberOfLayers() const A_WARN_UNUSED
        { return CompoundSprite::getNumberOfLayers(); }

        /**
         * Performs being logic.
         */
        virtual void logic() override;

        void petLogic();

        /**
         * Draws the speech text above the being.
         */
        void drawSpeech(const int offsetX, const int offsetY);

        /**
         * Draws the emotion picture above the being.
         */
        void drawEmotion(Graphics *const graphics, const int offsetX,
                         const int offsetY) const;

        uint16_t getSubType() const
        { return mSubType; }

         /**
          * Set Being's subtype (mostly for view for monsters and NPCs)
          */
        void setSubtype(const uint16_t subtype, const uint8_t look);

        const BeingInfo *getInfo() const A_WARN_UNUSED
        { return mInfo; }

        TargetCursorSize getTargetCursorSize() const override final
                                             A_WARN_UNUSED;

        int getTargetOffsetX() const A_WARN_UNUSED
        {
            if (!mInfo)
                return 0;
            return mInfo->getTargetOffsetX();
        }

        int getTargetOffsetY() const A_WARN_UNUSED
        {
            if (!mInfo)
                return 0;
            return mInfo->getTargetOffsetY();
        }

        /**
         * Gets the way the object is blocked by other objects.
         */
        virtual unsigned char getWalkMask() const A_WARN_UNUSED
        {
            if (!mInfo)
                return 0;
            return mInfo->getWalkMask();
        }

        /**
         * Gets the way the monster blocks pathfinding for other objects
         */
        Map::BlockType getBlockType() const A_WARN_UNUSED
        {
            if (!mInfo)
                return Map::BLOCKTYPE_NONE;
            return mInfo->getBlockType();
        }

        /**
         * Sets the walk speed in pixels per second.
         */
        void setWalkSpeed(Vector speed)
        { mWalkSpeed = speed; mSpeed = speed.x; }

        /**
         * Gets the walk speed in pixels per second.
         */
        Vector getWalkSpeed() const A_WARN_UNUSED
        { return mWalkSpeed; }

        /**
         * Sets the attack speed.
         * @todo In what unit?
         */
        void setAttackSpeed(const int speed)
        { mAttackSpeed = speed; }

        /**
         * Gets the attack speed.
         * @todo In what unit?
         */
        int getAttackSpeed() const A_WARN_UNUSED
        { return mAttackSpeed; }

        /**
         * Sets the current action.
         */
        virtual void setAction(const Action &action, const int attackType);

        /**
         * Get the being's action currently performed.
         */
        Action getCurrentAction() const A_WARN_UNUSED
        { return mAction; }

        /**
         * Returns whether this being is still alive.
         */
        bool isAlive() const A_WARN_UNUSED
        { return mAction != DEAD; }

        /**
         * Returns the current direction.
         */
        uint8_t getDirection() const A_WARN_UNUSED
        { return mDirection; }

        /**
         * Sets the current direction.
         */
        virtual void setDirection(const uint8_t direction);

        virtual void setDirectionDelayed(const uint8_t direction)
        { mDirectionDelayed = direction; }

        uint8_t getDirectionDelayed() const A_WARN_UNUSED
        { return mDirectionDelayed; }

        /**
         * Returns the direction the being is facing.
         */
        SpriteDirection getSpriteDirection() const A_WARN_UNUSED
        { return static_cast<SpriteDirection>(mSpriteDirection); }

        void setPosition(const Vector &pos);

        /**
         * Overloaded method provided for convenience.
         *
         * @see setPosition(const Vector &pos)
         */
        inline void setPosition(const float x, const float y,
                                const float z = 0.0F)
        { setPosition(Vector(x, y, z)); }

        /**
         * Returns the horizontal size of the current base sprite of the being.
         */
        virtual int getWidth() const override final A_WARN_UNUSED
        { return std::max(CompoundSprite::getWidth(), DEFAULT_BEING_WIDTH); }

        /**
         * Returns the vertical size of the current base sprite of the being.
         */
        virtual int getHeight() const override final A_WARN_UNUSED
        { return std::max(CompoundSprite::getHeight(), DEFAULT_BEING_HEIGHT); }

        /**
         * Returns the being's pixel radius used to detect collisions.
         */
        virtual int getCollisionRadius() const A_WARN_UNUSED
        { return 16; }

        /**
         * Shoots a missile particle from this being, to target being
         */
        void fireMissile(Being *const target,
                         const std::string &particle) const;

        /**
         * Returns the path this being is following. An empty path is returned
         * when this being isn't following any path currently.
         */
        const Path &getPath() const A_WARN_UNUSED
        { return mPath; }

        int getDistance() const A_WARN_UNUSED
        { return mDistance; }

        void setDistance(const int n)
        { mDistance = n; }

        /**
         * Set the Emoticon type and time displayed above
         * the being.
         */
        void setEmote(const uint8_t emotion, const int emote_time);

        void setState(const uint8_t state);

        virtual void drawSprites(Graphics *const  graphics,
                                 int posX, int posY) const override final;

        virtual void drawSpritesSDL(Graphics *const graphics,
                                    int posX, int posY) const override final;

        void drawHpBar(Graphics *const graphics, const int x, const int y,
                       const int maxHP, const int hp, const int damage,
                       const int color1, const int color2, const int width,
                       const int height) const;

        static void load();

        virtual void optionChanged(const std::string &value) override;

        void flashName(const int time);

        int getDamageTaken() const A_WARN_UNUSED
        { return mDamageTaken; }

        void setDamageTaken(const int damage)
        { mDamageTaken = damage; }

        void updateName();

        void setLevel(const int n)
        { mLevel = n; }

        virtual int getLevel() const A_WARN_UNUSED
        { return mLevel; }

        void setIsReachable(const int n)
        { mIsReachable = n; }

        int isReachable() const A_WARN_UNUSED
        { return mIsReachable; }

        static void reReadConfig();

        static BeingCacheEntry* getCacheEntry(const int id) A_WARN_UNUSED;

        void addToCache() const;

        bool updateFromCache();

        /**
         * Sets the gender of this being.
         */
        virtual void setGender(const Gender gender);

        Gender getGender() const A_WARN_UNUSED
        { return mGender; }

        /**
         * Return sprite sit action for current environment.
         */
        std::string getSitAction() const A_WARN_UNUSED;

        std::string getMoveAction() const A_WARN_UNUSED;

        std::string getDeadAction() const A_WARN_UNUSED;

        std::string getStandAction() const A_WARN_UNUSED;

        std::string getSpawnAction() const A_WARN_UNUSED;

        std::string getWeaponAttackAction(const ItemInfo *const weapon) const;

        std::string getAttackAction(const Attack *const attack) const;

        /**
         * Whether or not this player is a GM.
         */
        bool isGM() const A_WARN_UNUSED
        { return mIsGM; }

        /**
         * Triggers whether or not to show the name as a GM name.
         */
        void setGM(const bool gm);

        bool canTalk() const A_WARN_UNUSED
        { return mType == NPC; }

        void talkTo() const;

        void draw(Graphics *const graphics,
                  const int offsetX, const int offsetY) const override final;

        void drawSpriteAt(Graphics *const graphics,
                          const int x, const int y) const;

        void setMoveTime()
        { mMoveTime = cur_time; }

        void setAttackTime()
        { mAttackTime = cur_time; }

        void setTalkTime()
        { mTalkTime = cur_time; }

        void setTestTime()
        { mTestTime = cur_time; }

        void setOtherTime()
        { mOtherTime = cur_time; }

        unsigned int getMoveTime() const
        { return mMoveTime; }

        unsigned int getAttackTime() const
        { return mAttackTime; }

        unsigned int getTalkTime() const
        { return mTalkTime; }

        unsigned int getTestTime() const
        { return mTestTime; }

        unsigned int getOtherTime() const
        { return mOtherTime; }

        void resetCounters();

        void updateColors();

        void setEnemy(const bool n)
        { mEnemy = n; }

        const std::string &getIp() const A_WARN_UNUSED
        { return mIp; }

        void setIp(std::string ip)
        { mIp = ip; }

        unsigned int getPvpRank() const A_WARN_UNUSED
        { return mPvpRank; }

        void setPvpRank(const unsigned int rank)
        { mPvpRank = rank; }

        void setHP(const int n);

        void setMaxHP(const int hp);

        int getHP() const A_WARN_UNUSED
        { return mHP; }

        uint8_t calcDirection(const int dstX,
                              const int dstY) const A_WARN_UNUSED;

        uint8_t calcDirection() const A_WARN_UNUSED;

        void setAttackDelay(const int n)
        { mAttackDelay = n; }

        int getAttackDelay() const A_WARN_UNUSED
        { return mAttackDelay; }

        int getMinHit() const A_WARN_UNUSED
        { return mMinHit; }

        void setMinHit(const int n)
        { mMinHit = n; }

        int getMaxHit() const A_WARN_UNUSED
        { return mMaxHit; }

        void setMaxHit(const int n)
        { mMaxHit = n; }

        int getCriticalHit() const A_WARN_UNUSED
        { return mCriticalHit; }

        void setCriticalHit(const int n)
        { mCriticalHit = n; }

        void updateHit(const int amount);

        Equipment *getEquipment() A_WARN_UNUSED;

        void undressItemById(const int id);

        int getGoodStatus() const A_WARN_UNUSED
        { return mGoodStatus; }

        void setGoodStatus(const int n)
        { mGoodStatus = n; }

        std::string getGenderSign() const A_WARN_UNUSED;

        std::string getGenderSignWithSpace() const A_WARN_UNUSED;

        void updateComment();

        const std::string getComment() const A_WARN_UNUSED
        { return mComment; }

        void setComment(std::string n)
        { mComment = n; }

        static void clearCache();

        static std::string loadComment(const std::string &name,
                                       const int type) A_WARN_UNUSED;

        static void saveComment(const std::string &restrict name,
                                const std::string &restrict comment,
                                const int type);

        bool isAdvanced() const A_WARN_UNUSED
        { return mAdvanced; }

        void setAdvanced(const bool n)
        { mAdvanced = n; addToCache(); }

        bool isShopEnabled() const A_WARN_UNUSED
        { return mShop; }

        void enableShop(const bool b)
        { mShop = b; }

        /**
         * Sets the attack range.
         */
        void setAttackRange(const int range)
        { mAttackRange = range; }

        void attack(Being *target = nullptr, bool keep = false,
                    bool dontChangeEquipment = false);

        void attack2(Being *target = nullptr, bool keep = false,
                     bool dontChangeEquipment = false);

        void updatePercentHP();

        void setRaceName(std::string name)
        { mRaceName = name; }

        std::string getRaceName() const A_WARN_UNUSED
        { return mRaceName; }

        int getSpriteID(const int slot) const A_WARN_UNUSED;

        void setHairStyle(const unsigned int slot, const int id);

        void setHairColor(const unsigned int slot,
                          const unsigned char color);

        void setHairColor(const unsigned char color)
        { mHairColor = color; }

        unsigned char getHairColor() const A_WARN_UNUSED
        { return mHairColor; }

        void recalcSpritesOrder();

        int getHitEffect(const Being *const attacker,
                         const AttackType type,
                         const int attackId) const A_WARN_UNUSED;

        Cursor::Cursor getHoverCursor() const A_WARN_UNUSED
        { return mInfo ? mInfo->getHoverCursor() : Cursor::CURSOR_POINTER; }

        void addAfkEffect();

        void removeAfkEffect();

        void updateAwayEffect();

        void addSpecialEffect(const int effect);

        void removeSpecialEffect();

        void addEffect(const std::string &name);

        void addPet(const int id);

        void removePet(const int id);

        void updatePets();

        void fixPetSpawnPos(int &dstX, int &dstY) const;

        const std::vector<Being*> &getPets() const
        { return mPets; }

        Being *getFirstPet()
        { return mPets.empty() ? nullptr : mPets[0]; }

        void setOwner(Being *const owner)
        { mOwner = owner; }

        void unassignPet(const Being *const pet);

        void removeAllPets();

        Being *findChildPet(const int id);

        void playSfx(const SoundInfo &sound, Being *const being,
                     const bool main, const int x, const int y) const;

        int getLook() const
        { return mLook; }

        void setLook(const int look);

        static uint8_t genderToInt(const Gender sex) A_WARN_UNUSED;

        static Gender intToGender(const uint8_t sex) A_WARN_UNUSED;

        NextSoundInfo mNextSound;

        /**
         * Sets the new path for this being.
         */
        void setPath(const Path &path);

        int getSortPixelY() const A_WARN_UNUSED
        { return static_cast<int>(mPos.y) - mYDiff - mSortOffsetY; }

        void setMap(Map *const map);

        void recreateItemParticles();

        void incUsage()
        { mUsageCounter ++; }

        int decUsage()
        { return --mUsageCounter; }

        virtual int getLastAttackX() const
        { return mLastAttackX; }

        virtual int getLastAttackY() const
        { return mLastAttackY; }

    protected:
        /**
         * Updates name's location.
         */
        virtual void updateCoords();

        void showName();

        void addItemParticles(const int id, const SpriteDisplay &display);

        void removeAllItemsParticles();

        void removeItemParticles(const int id);

        static int getDefaultEffectId(const int type);

        BeingInfo *mInfo;
        AnimatedSprite *mEmotionSprite;
        AnimatedSprite* mAnimationEffect;

        std::string mSpriteAction;
        std::string mName;              /**< Name of character */
        std::string mRaceName;
        std::string mPartyName;
        std::string mGuildName;
        std::string mSpeech;

        /**
         * Holds a text object when the being displays it's name, 0 otherwise
         */
        FlashText *mDispName;
        const Color *mNameColor;

        /** Engine-related infos about weapon. */
        const ItemInfo *mEquippedWeapon;

        static int mNumberOfHairstyles; /** Number of hair styles in use */
        static int mNumberOfRaces; /** Number of races in use */

        Path mPath;
        Text *mText;
        const Color *mTextColor;

        Vector mDest;  /**< destination coordinates. */

        typedef std::map<int, ParticleInfo*> SpriteParticleInfo;
        typedef SpriteParticleInfo::iterator SpriteParticleInfoIter;

        StringVect mSpriteColors;
        std::vector<int> mSpriteIDs;
        std::vector<int> mSpriteColorsIds;
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
        Gender mGender;
        Action mAction;       /**< Action the being is performing */
        uint16_t mSubType;      /**< Subtype (graphical view, basically) */
        uint8_t mDirection;               /**< Facing direction */
        uint8_t mDirectionDelayed;        /**< Facing direction */
        uint8_t mSpriteDirection;         /**< Facing direction */
        bool mShowName;
        bool mIsGM;

    private:
        /**
         * Calculates the offset in the given directions.
         * If walking in direction 'neg' the value is negated.
         * TODO: Used by eAthena only?
         */
        int getOffset(const signed char pos,
                      const signed char neg) const A_WARN_UNUSED;

        int searchSlotValue(const std::vector<int> &slotRemap,
                            const int val) const A_WARN_UNUSED;

        static void searchSlotValueItr(std::vector<int>::iterator &it,
                                       int &idx,
                                       std::vector<int> &slotRemap,
                                       const int val);

        void dumpSprites() const;

        const Type mType;

        /** Speech Bubble components */
        SpeechBubble *mSpeechBubble;

        /**
         * Walk speed for x and y movement values.
         * In pixels per second.
         * @see MILLISECONDS_IN_A_TICK
         */
        Vector mWalkSpeed;
        float mSpeed;
        std::string mIp;
        int *mSpriteRemap;
        int *mSpriteHide;
        int *mSpriteDraw;
        std::string mComment;
        std::vector<Being*> mPets;
        Being *mOwner;
        Particle *mSpecialParticle;

        int mX;            // position in tiles
        int mY;            // position in tiles
        int mSortOffsetY;  // caculated offset in pixels based on mOffsetY
        int mOffsetY;      // fixed tile height offset in pixels
        uint8_t mOldHeight;

        int mDamageTaken;
        int mHP;
        int mMaxHP;
        int mDistance;
        int mIsReachable; /**< 0 - unknown, 1 - reachable, 2 - not reachable*/
        int mGoodStatus;

        static int mUpdateConfigTime;
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
        static bool mMoveNames;
        static bool mUseDiagonal;
        static int mAwayEffect;

        unsigned int mMoveTime;
        unsigned int mAttackTime;
        unsigned int mTalkTime;
        unsigned int mOtherTime;
        unsigned int mTestTime;
        int mAttackDelay;
        int mMinHit;
        int mMaxHit;
        int mCriticalHit;
        unsigned int mPvpRank;
        unsigned int mNumber;
        int mLook;
        int mUsageCounter;
        unsigned char mHairColor;
        bool mErased;
        bool mEnemy;
        bool mGotComment;
        bool mAdvanced;
        bool mShop;
        bool mAway;
        bool mInactive;
};

extern std::list<BeingCacheEntry*> beingInfoCache;

#endif  // BEING_BEING_H
