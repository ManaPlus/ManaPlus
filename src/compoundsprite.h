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

#ifndef COMPOUNDSPRITE_H
#define COMPOUNDSPRITE_H

#include "sprite.h"

#include <list>
#include <vector>

class Image;

typedef std::list <void*> VectorPointers;

class CompoundItem final
{
    public:
        CompoundItem();

        ~CompoundItem();

//        float alpha;
        VectorPointers data;
        Image *image;
        Image *alphaImage;
};

class CompoundSprite : public Sprite
{
public:
    typedef std::vector<Sprite*>::iterator SpriteIterator;
    typedef std::vector<Sprite*>::const_iterator SpriteConstIterator;

    CompoundSprite();

    ~CompoundSprite();

    virtual bool reset() override;

    virtual bool play(std::string action) override;

    virtual bool update(int time) override;

    virtual bool draw(Graphics *graphics, int posX, int posY) const override;

    /**
     * Gets the width in pixels of the first sprite in the list.
     */
    virtual int getWidth() const override;

    /**
     * Gets the height in pixels of the first sprite in the list.
     */
    virtual int getHeight() const override;

    virtual const Image *getImage() const override;

    virtual bool setSpriteDirection(const SpriteDirection direction) override;

    int getNumberOfLayers() const;

    unsigned int getCurrentFrame() const override;

    unsigned int getFrameCount() const override;

    size_t size() const
    { return mSprites.size(); }

    bool empty() const
    { return mSprites.empty(); }

    void addSprite(Sprite *const sprite);

    void setSprite(const int layer, Sprite *const sprite);

    Sprite *getSprite(int layer) const
    { return mSprites.at(layer); }

    void removeSprite(const int layer);

    void clear();

    void ensureSize(size_t layerCount);

    virtual void drawSprites(Graphics* graphics,
                             int posX, int posY) const;

    virtual void drawSpritesSDL(Graphics* graphics,
                                int posX, int posY) const;

    /**
     * Returns the curent frame in the current animation of the given layer.
     */
    virtual unsigned int getCurrentFrame(unsigned int layer);

    /**
     * Returns the frame count in the current animation of the given layer.
     */
    virtual unsigned int getFrameCount(unsigned int layer);

    virtual void setAlpha(float alpha) override;

    bool updateNumber(const unsigned num) override;

    static void setEnableDelay(bool b)
    { mEnableDelay = b; }

private:
    void redraw() const;

    void updateImages() const;

    bool updateFromCache() const;

    void initCurrentCacheItem() const;

    typedef std::list<CompoundItem*> ImagesCache;
    mutable ImagesCache imagesCache;
    mutable CompoundItem *mCacheItem;

    mutable Image *mImage;
    mutable Image *mAlphaImage;

    mutable int mOffsetX, mOffsetY;

    mutable bool mNeedsRedraw;

    bool mEnableAlphaFix;
    bool mDisableAdvBeingCaching;
    bool mDisableBeingCaching;
    std::vector<Sprite*> mSprites;
    mutable int mNextRedrawTime;
    static bool mEnableDelay;
};

#endif // COMPOUNDSPRITE_H
