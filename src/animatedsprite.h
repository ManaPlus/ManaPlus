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

#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "sprite.h"

#include <map>
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
        AnimatedSprite(SpriteDef *const sprite);

        /**
         * An helper function, which will request the sprite to animate
         * from the resource manager.
         *
         * @param filename the file of the sprite to animate
         * @param variant  the sprite variant
         */
        static AnimatedSprite *load(const std::string &filename,
                                    int variant = 0);

        static AnimatedSprite *delayedLoad(const std::string &filename,
                                           const int variant = 0);

        virtual ~AnimatedSprite();

        bool reset();

        bool play(std::string action);

        bool update(int time);

        bool draw(Graphics* graphics, int posX, int posY) const;

        int getWidth() const;

        int getHeight() const;

        const Image* getImage() const;

        bool setSpriteDirection(const SpriteDirection direction);

        int getNumberOfLayers() const
        { return 1; }

        std::string getIdPath() const;

        unsigned int getCurrentFrame() const;

        unsigned int getFrameCount() const;

        virtual void setAlpha(float alpha);

        virtual void *getHash();

        bool updateNumber(const unsigned num);

        void clearDelayLoad();

        void setSprite(SpriteDef *const sprite)
        { mSprite = sprite; }

        static void setEnableCache(const bool b)
        { mEnableCache = b; }

    private:
        bool updateCurrentAnimation(unsigned int dt);

        void setDelayLoad(const std::string &filename, const int variant);

        SpriteDirection mDirection;    /**< The sprite direction. */
        int mLastTime;                 /**< The last time update was called. */

        unsigned int mFrameIndex;      /**< The index of the current frame. */
        unsigned int mFrameTime;       /**< The time since start of frame. */

        SpriteDef *mSprite;            /**< The sprite definition. */
        Action *mAction;               /**< The currently active action. */
        Animation *mAnimation;         /**< The currently active animation. */
        Frame *mFrame;                 /**< The currently active frame. */
        unsigned mNumber;
        unsigned mNumber1;
        AnimationDelayLoad *mDelayLoad;
        static bool mEnableCache;
};

#endif
