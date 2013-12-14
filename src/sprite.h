/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef SPRITE_H
#define SPRITE_H

#include "resources/spritedef.h"

#include "localconsts.h"

class Graphics;
class Image;

class Sprite
{
    public:
        virtual ~Sprite()
        { }

        /**
         * Resets the sprite.
         *
         * @returns true if the sprite changed, false otherwise
         */
        virtual bool reset() = 0;

        /**
         * Plays an action using the current direction.
         *
         * @returns true if the sprite changed, false otherwise
         */
        virtual bool play(const std::string &action) = 0;

        /**
         * Inform the animation of the passed time so that it can output the
         * correct animation frame.
         *
         * @returns true if the sprite changed, false otherwise
         */
        virtual bool update(const int time) = 0;

        /**
         * Draw the current animation frame at the coordinates given in screen
         * pixels.
         */
        virtual void draw(Graphics *const graphics,
                          const int posX, const int posY) const = 0;

        /**
         * Gets the width in pixels of the image of the current frame
         */
        virtual int getWidth() const A_WARN_UNUSED = 0;

        /**
         * Gets the height in pixels of the image of the current frame
         */
        virtual int getHeight() const A_WARN_UNUSED = 0;

        /**
         * Returns a reference to the current image being drawn.
         */
        virtual const Image* getImage() const A_WARN_UNUSED = 0;

        /**
         * Sets the direction.
         *
         * @returns true if the sprite changed, false otherwise
         */
        virtual bool setSpriteDirection(const SpriteDirection direction) = 0;

        /**
         * Sets the alpha value of the animated sprite
         */
        virtual void setAlpha(float alpha)
        { mAlpha = alpha; }

        /**
         * Returns the current alpha opacity of the animated sprite.
         */
        virtual float getAlpha() const A_WARN_UNUSED
        { return mAlpha; }

        /**
         * Returns the current frame number for the sprite.
         */
        virtual unsigned int getCurrentFrame() const A_WARN_UNUSED = 0;

        /**
         * Returns the frame count for the sprite.
         */
        virtual unsigned int getFrameCount() const A_WARN_UNUSED = 0;

        virtual const void *getHash() const A_WARN_UNUSED
        { return nullptr; }

        virtual const void *getHash2() const A_WARN_UNUSED
        { return this; }

        virtual bool updateNumber(const unsigned num) = 0;

    protected:
        Sprite() :
            mAlpha()
        {
        }

        float mAlpha;                  /**< The alpha opacity used to draw */
};

#endif  // SPRITE_H
