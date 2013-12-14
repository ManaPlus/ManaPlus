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

#ifndef SIMPLEANIMATION_H
#define SIMPLEANIMATION_H

#include "utils/xml.h"

#include "localconsts.h"

class Animation;
class Graphics;
class Image;
class ImageSet;

struct Frame;

/**
 * This class is a leightweight alternative to the AnimatedSprite class.
 * It hosts a looping animation without actions and directions.
 */
class SimpleAnimation final
{
    public:
        /**
         * Creates a simple animation with an already created \a animation.
         * Takes ownership over the given animation.
         */
        explicit SimpleAnimation(Animation *const animation);

        /**
         * Creates a simple animation that creates its animation from XML Data.
         */
        explicit SimpleAnimation(const XmlNodePtr animationNode,
                                 const std::string& dyePalettes);

        A_DELETE_COPY(SimpleAnimation)

        ~SimpleAnimation();

        void setFrame(int frame);

        int getLength() const A_WARN_UNUSED;

        bool update(const int timePassed);

        void draw(Graphics *const graphics,
                  const int posX, const int posY) const;

        /**
         * Resets the animation.
         */
        void reset();

        Image *getCurrentImage() const A_WARN_UNUSED;

    private:
        void initializeAnimation(const XmlNodePtr animationNode,
                                 const std::string&
                                 dyePalettes = std::string());

        /** The hosted animation. */
        Animation *mAnimation;

        /** Time in game ticks the current frame is shown. */
        int mAnimationTime;

        /** Index of current animation phase. */
        int mAnimationPhase;

        /** Current animation phase. */
        const Frame *mCurrentFrame;

        /**  Tell whether the animation is ready */
        bool mInitialized;

        ImageSet *mImageSet;
};

#endif  // SIMPLEANIMATION_H
