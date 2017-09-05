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

#ifndef RESOURCES_SPRITE_ANIMATEDSPRITE_H
#define RESOURCES_SPRITE_ANIMATEDSPRITE_H

#include "resources/sprite/sprite.h"

class Animation;
class AnimationDelayLoad;
struct Frame;

/**
 * Animates a sprite by adding playback state.
 */
class AnimatedSprite final : public Sprite
{
    public:
        /**
         * Constructor.
         * @param sprite the sprite to animate
         */
        explicit AnimatedSprite(SpriteDef *restrict const sprite);

        A_DELETE_COPY(AnimatedSprite)

        /**
         * An helper function, which will request the sprite to animate
         * from the resource manager.
         *
         * @param filename the file of the sprite to animate
         * @param variant  the sprite variant
         */
        static AnimatedSprite *load(const std::string &restrict filename,
                                    const int variant = 0) A_WARN_UNUSED;

        static AnimatedSprite *delayedLoad(const std::string &restrict
                                           filename,
                                           const int variant = 0)
                                           A_WARN_UNUSED;

        static AnimatedSprite *clone(const AnimatedSprite *restrict const
                                     anim);

        ~AnimatedSprite();

        bool reset() restrict2 override final;

        bool play(const std::string &restrict spriteAction)
                  restrict2 override final;

        bool update(const int time) restrict2 override final;

        void draw(Graphics *restrict const graphics,
                  const int posX,
                  const int posY) const restrict2 override final A_NONNULL(2);

        void drawRescaled(Graphics *restrict const graphics,
                          const int posX,
                          const int posY,
                          const int dx,
                          const int dy) const restrict2 A_NONNULL(2);

        void drawRaw(Graphics *restrict const graphics,
                     const int posX,
                     const int posY) const restrict2 A_NONNULL(2);

        int getWidth() const restrict2 override final A_WARN_UNUSED;

        int getHeight() const restrict2 override final A_WARN_UNUSED;

        const Image* getImage() const restrict2 noexcept2 override final
                              A_WARN_UNUSED;

        bool setSpriteDirection(const SpriteDirection::Type direction)
                                restrict2 override final;

        int getNumberOfLayers() const restrict2 noexcept2 A_WARN_UNUSED
        { return 1; }

        std::string getIdPath() const restrict2 A_WARN_UNUSED;

        unsigned int getCurrentFrame() const restrict2 noexcept2 override final
                                     A_WARN_UNUSED
        { return mFrameIndex; }

        unsigned int getFrameCount() const
                                   restrict2 override final A_WARN_UNUSED;

        void setAlpha(float alpha) restrict2 override final;

        const void *getHash() const restrict2 override final A_WARN_UNUSED;

        bool updateNumber(const unsigned num) restrict2 override final;

        void clearDelayLoad() restrict2 noexcept2
        { mDelayLoad = nullptr; }

        void setSprite(SpriteDef *restrict const sprite) restrict2 noexcept2
        { mSprite = sprite; }

        bool isTerminated() const restrict2 noexcept2
        { return mTerminated; }

        constexpr2 static void setEnableCache(const bool b) noexcept2
        { mEnableCache = b; }

        void setLastTime(const int time) noexcept2
        { mLastTime = time; }

#ifdef UNITTESTS
        SpriteDef *getSprite() restrict2
        { return mSprite; }

        const Frame *getFrame() const restrict2
        { return mFrame; }

        const Animation *getAnimation() const restrict2
        { return mAnimation; }

        unsigned int getFrameIndex() const restrict2
        { return mFrameIndex; }

        unsigned int getFrameTime() const restrict2
        { return mFrameTime; }
#endif  // UNITTESTS

#ifdef DEBUG_ANIMATIONS
        void setSpriteName(const std::string &restrict name) noexcept2
        { mSpriteName = name; }

        std::string getSpriteName() const noexcept2 A_WARN_UNUSED
        { return mSpriteName; }
#endif  // DEBUG_ANIMATIONS

    private:
        bool updateCurrentAnimation(const unsigned int dt) restrict2;

        void setDelayLoad(const std::string &restrict filename,
                          const int variant) restrict2;

#ifdef DEBUG_ANIMATIONS
        std::string mSpriteName;
#endif  // DEBUG_ANIMATIONS

        SpriteDirection::Type mDirection;  /**< The sprite direction. */
        int mLastTime;                 /**< The last time update was called. */

        unsigned int mFrameIndex;      /**< The index of the current frame. */
        unsigned int mFrameTime;       /**< The time since start of frame. */

        /**< The sprite definition. */
        SpriteDef *restrict mSprite;
        /**< The currently active action. */
        const Action *restrict mAction;
        /**< The currently active animation. */
        const Animation *restrict mAnimation;
        /**< The currently active frame. */
        const Frame *restrict mFrame;
        unsigned mNumber;
        unsigned mNumber1;
        AnimationDelayLoad *mDelayLoad;
        bool mTerminated;
        static bool mEnableCache;
};

#endif  // RESOURCES_SPRITE_ANIMATEDSPRITE_H
