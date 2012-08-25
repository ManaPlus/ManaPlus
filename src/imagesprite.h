/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef IMAGESPRITE_H
#define IMAGESPRITE_H

#include "sprite.h"

#include "resources/image.h"

class Graphics;

class ImageSprite : public Sprite
{
public:
    ImageSprite(Image *image);

    ~ImageSprite();

    bool reset()
    { return false; }

    bool play(std::string action A_UNUSED)
    { return false; }

    bool update(int time A_UNUSED)
    { return false; }

    bool draw(Graphics* graphics, int posX, int posY) const;

    int getWidth() const
    { return mImage ? mImage->getWidth() : 0; }

    int getHeight() const
    { return mImage ? mImage->getHeight() : 0; }

    const Image* getImage() const
    { return mImage; }

    virtual bool setSpriteDirection(SpriteDirection direction A_UNUSED)
    { return false; }

    int getNumberOfLayers()
    { return 1; }

    unsigned int getCurrentFrame() const
    { return 0; }

    unsigned int getFrameCount() const
    { return 1; }

    bool updateNumber(const unsigned num A_UNUSED)
    { return false; }

private:
    Image *mImage;
};

#endif // IMAGESPRITE_H
