/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef BEING_ACTORSPRITE_H
#define BEING_ACTORSPRITE_H

#include "being/actor.h"
#include "being/compoundsprite.h"

#include "const/resources/map/map.h"

#include "enums/being/actortype.h"
#include "enums/being/targetcursorsize.h"
#include "enums/being/targetcursortype.h"

#include "enums/simpletypes/beingid.h"
#include "enums/simpletypes/enable.h"
#include "enums/simpletypes/forcedisplay.h"
#include "enums/simpletypes/isstart.h"

#include "enums/resources/displaytype.h"

#include "enums/resources/map/blocktype.h"

#include "particle/particlelist.h"
#include "particle/particlevector.h"

#include "localconsts.h"

class AnimatedSprite;
class StatusEffect;
class ActorSpriteListener;

struct SpriteDisplay;

class ActorSprite notfinal : public CompoundSprite, public Actor
{
    public:
        explicit ActorSprite(const BeingId id);

        A_DELETE_COPY(ActorSprite)

        ~ActorSprite() override;

        BeingId getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

        void setId(const BeingId id) noexcept2
        { mId = id; }

        /**
         * Returns the type of the ActorSprite.
         */
        virtual ActorTypeT getType() const noexcept2 A_WARN_UNUSED
        { return ActorType::Unknown; }

        virtual void logic();

        void setMap(Map *const map) override;

        /**
         * Gets the way the object blocks pathfinding for other objects
         */
        virtual BlockTypeT getBlockType() const A_WARN_UNUSED
        { return BlockType::NONE; }

        /**
         * Take control of a particle. Particle can be auto deleted.
         */
        void controlAutoParticle(Particle *const particle);

        /**
         * Take control of a particle. Owner must remove particle by self.
         */
        void controlCustomParticle(Particle *const particle);

        /**
         * Returns the required size of a target cursor for this being.
         */
        virtual TargetCursorSizeT getTargetCursorSize() const A_WARN_UNUSED
        { return TargetCursorSize::MEDIUM; }

        virtual int getTargetOffsetX() const A_WARN_UNUSED
        { return 0; }

        virtual int getTargetOffsetY() const A_WARN_UNUSED
        { return 0; }

        /**
         * Sets the target animation for this actor.
         */
        void setTargetType(const TargetCursorTypeT type);

        /**
         * Untargets the actor.
         */
        void untarget()
        { mUsedTargetCursor = nullptr; }

        void setStatusEffect(const int32_t index,
                             const Enable active,
                             const IsStart start);

        void setStatusEffectOpitons(const uint32_t option,
                                    const uint32_t opt1,
                                    const uint32_t opt2,
                                    const uint32_t opt3);

        void setStatusEffectOpitons(const uint32_t option,
                                    const uint32_t opt1,
                                    const uint32_t opt2);

        void setStatusEffectOpiton0(const uint32_t option);

        void setAlpha(const float alpha) override final
        { CompoundSprite::setAlpha(alpha); }

        float getAlpha() const override final A_WARN_UNUSED
        { return CompoundSprite::getAlpha(); }

        int getWidth() const override A_WARN_UNUSED
        { return CompoundSprite::getWidth(); }

        int getHeight() const override A_WARN_UNUSED
        { return CompoundSprite::getHeight(); }

        static void load();

        static void unload();

        /**
         * Add an ActorSprite listener.
         */
        void addActorSpriteListener(ActorSpriteListener *const listener);

        /**
         * Remove an ActorSprite listener.
         */
        void removeActorSpriteListener(ActorSpriteListener *const listener);

        int getActorX() const A_WARN_UNUSED
        { return getPixelX() - mapTileSize / 2; }

        int getActorY() const A_WARN_UNUSED
        { return getPixelY() - mapTileSize; }

        void setPoison(const bool b)
        { mPoison = b; }

        bool getPoison() const A_WARN_UNUSED
        { return mPoison; }

        void setHaveCart(const bool b)
        { mHaveCart = b; }

        bool getHaveCart() const A_WARN_UNUSED
        { return mHaveCart; }

        virtual void setRiding(const bool b)
        { mHorseId = b ? 1 : 0; }

        virtual void setTrickDead(const bool b)
        { mTrickDead = b; }

        bool isTrickDead() const A_WARN_UNUSED
        { return mTrickDead; }

        const std::set<int32_t> &getStatusEffects() const A_WARN_UNUSED
        { return mStatusEffects; }

        std::string getStatusEffectsString() const;

        virtual void stopCast(const bool b A_UNUSED)
        { }

        size_t getParticlesCount() const
        {
            return mStatusParticleEffects.usedSize() +
                mChildParticleEffects.size();
        }

        void controlParticleDeleted(const Particle *const particle);

    protected:
        /**
         * A status effect block is a 16 bit mask of status effects. We assign
         * each such flag a block ID of offset + bitnr.
         *
         * These are NOT the same as the status effect indices.
         */
        void setStatusEffectBlock(const int offset,
                                  const uint16_t newEffects);

        /**
         * Notify self that a status effect has flipped.
         * The new flag is passed.
         */
        virtual void updateStatusEffect(const int32_t index,
                                        const Enable newStatus,
                                        const IsStart start);

        /**
         * Handle an update to a status or stun effect
         *
         * \param The StatusEffect to effect
         * \param effectId -1 for stun, otherwise the effect index
         */
        virtual void handleStatusEffect(const StatusEffect *const effect,
                                        const int32_t effectId,
                                        const Enable newStatus,
                                        const IsStart start);

        void setupSpriteDisplay(const SpriteDisplay &display,
                                const ForceDisplay forceDisplay,
                                const DisplayTypeT displayType,
                                const std::string &color);

        /** Load the target cursors into memory */
        static void initTargetCursor();

        /** Remove the target cursors from memory */
        static void cleanupTargetCursors();

        /** Animated target cursors. */
        static AnimatedSprite *targetCursor
            [CAST_SIZE(TargetCursorType::NUM_TCT)]
            [CAST_SIZE(TargetCursorSize::NUM_TC)];

        static bool loaded;

        /**< set of active status effects */
        std::set<int32_t> mStatusEffects;

        ParticleVector mStatusParticleEffects;
        ParticleList mChildParticleEffects;
        int mHorseId;
        BeingId mId;

        /** Target cursor being used */
        AnimatedSprite *mUsedTargetCursor;

        typedef std::list<ActorSpriteListener*> ActorSpriteListeners;
        typedef ActorSpriteListeners::iterator ActorSpriteListenerIterator;
        ActorSpriteListeners mActorSpriteListeners;

        int mCursorPaddingX;
        int mCursorPaddingY;

        /** Reset particle status effects on next redraw? */
        bool mMustResetParticles;
        bool mPoison;
        bool mHaveCart;
        bool mTrickDead;
};

#endif  // BEING_ACTORSPRITE_H
