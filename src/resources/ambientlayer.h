/*
 *  The ManaPlus Client
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

#ifndef RESOURCES_AMBIENTLAYER_H
#define RESOURCES_AMBIENTLAYER_H

#include "localconsts.h"

class Graphics;
class Image;
class Map;

class AmbientLayer final
{
    public:
        friend class Map;

        /**
         * Constructor.
         *
         * @param img       the image this overlay displays
         * @param parallax  scroll factor based on camera position
         * @param speedX    scrolling speed in x-direction
         * @param speedY    scrolling speed in y-direction
         * @param keepRatio  rescale the image to keep
         * the same ratio than in 800x600 resolution mode.
         */
        AmbientLayer(Image *const img,
                     const float parallax, const float parallaxY,
                     const float speedX, const float speedY,
                     const bool keepRatio, const int mask);

        A_DELETE_COPY(AmbientLayer)

        ~AmbientLayer();

        void update(const int timePassed, const float dx, const float dy);

        void draw(Graphics *const graphics, const int x, const int y) const;

    private:
        Image *mImage;
        float mParallaxX;
        float mParallaxY;
        float mPosX;             /**< Current layer X position. */
        float mPosY;             /**< Current layer Y position. */
        float mSpeedX;           /**< Scrolling speed in X direction. */
        float mSpeedY;           /**< Scrolling speed in Y direction. */
        int mMask;
        bool mKeepRatio;         /**< Keep overlay ratio on every resolution */
};

#endif  // RESOURCES_AMBIENTLAYER_H
