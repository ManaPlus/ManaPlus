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

#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "sprite.h"

#include <string>

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
        explicit AnimatedSprite(SpriteDef *const sprite);

        A_DELETE_COPY(AnimatedSprite)

        /**
         * An helper function, which will request the sprite to animate
         * from the resource manager.
         *
         * @param filename the file of the sprite to animate
         * @param variant  the sprite variant
         */
        static AnimatedSprite *load(const std::string &filename,
                                    const int variant = 0) A_WARN_UNUSED;

        static AnimatedSprite *delayedLoad(const std::string &filename,
                                           const int variant = 0)
                                           A_WARN_UNUSED;

        static AnimatedSprite *clone(const AnimatedSprite *const anim);

        ~AnimatedSprite();

        bool reset() override final;

        bool play(const std::string &action) override final;

        bool update(const int time) override final;

        void draw(Graphics *const graphics,
                  const int posX, const int posY) const override final;

        int getWidth() const A_WARN_UNUSED;

        int getHeight() const A_WARN_UNUSED;

        const Image* getImage() const A_WARN_UNUSED;

        bool setSpriteDirection(const SpriteDirection direction);

        int getNumberOfLayers() const A_WARN_UNUSED
        { return 1; }

        std::string getIdPath() const A_WARN_UNUSED;

        unsigned int getCurrentFrame() const A_WARN_UNUSED;

        unsigned int getFrameCount() const A_WARN_UNUSED;

        void setAlpha(float alpha) override final;

        const void *getHash() const override final A_WARN_UNUSED;

        bool updateNumber(const unsigned num);

        void clearDelayLoad();

        void setSprite(SpriteDef *const sprite)
        { mSprite = sprite; }

        bool isTerminated() const
        { return mTerminated; }

        static void setEnableCache(const bool b)
        { mEnableCache = b; }

#ifdef UNITTESTS
        SpriteDef *getSprite()
        { return mSprite; }

        const Frame *getFrame() const
        { return mFrame; }

        const Animation *getAnimation() const
        { return mAnimation; }

        unsigned int getFrameIndex() const
        { return mFrameIndex; }

        unsigned int getFrameTime() const
        { return mFrameTime; }
#endif

    private:
        bool updateCurrentAnimation(const unsigned int dt);

        void setDelayLoad(const std::string &filename, const int variant);

        SpriteDirection mDirection;    /**< The sprite direction. */
        int mLastTime;                 /**< The last time update was called. */

        unsigned int mFrameIndex;      /**< The index of the current frame. */
        unsigned int mFrameTime;       /**< The time since start of frame. */

        SpriteDef *mSprite;            /**< The sprite definition. */
        const Action *mAction;         /**< The currently active action. */
        const Animation *mAnimation;   /**< The currently active animation. */
        const Frame *mFrame;           /**< The currently active frame. */
        unsigned mNumber;
        unsigned mNumber1;
        AnimationDelayLoad *mDelayLoad;
        bool mTerminated;
        static bool mEnableCache;
};

#endif  // ANIMATEDSPRITE_H
