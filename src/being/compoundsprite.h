/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef BEING_COMPOUNDSPRITE_H
#define BEING_COMPOUNDSPRITE_H

#include "sprite.h"

#include <list>
#include <vector>

#include "localconsts.h"

class Image;

typedef std::list <const void*> VectorPointers;

class CompoundItem final
{
    public:
        CompoundItem();

        A_DELETE_COPY(CompoundItem)

        ~CompoundItem();

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

    A_DELETE_COPY(CompoundSprite)

    virtual ~CompoundSprite();

    virtual bool reset() override final;

    virtual bool play(const std::string &action) override final;

    virtual bool update(const int time) override final;

    virtual void draw(Graphics *const graphics,
                      const int posX, const int posY) const override;

    /**
     * Gets the width in pixels of the first sprite in the list.
     */
    virtual int getWidth() const override A_WARN_UNUSED;

    /**
     * Gets the height in pixels of the first sprite in the list.
     */
    virtual int getHeight() const override A_WARN_UNUSED;

    virtual const Image *getImage() const override final A_WARN_UNUSED;

    virtual bool setSpriteDirection(const SpriteDirection direction)
                                    override final;

    int getNumberOfLayers() const A_WARN_UNUSED;

    unsigned int getCurrentFrame() const override final A_WARN_UNUSED;

    unsigned int getFrameCount() const override final A_WARN_UNUSED;

    size_t size() const A_WARN_UNUSED
    { return mSprites.size(); }

    bool empty() const A_WARN_UNUSED
    { return mSprites.empty(); }

    void addSprite(Sprite *const sprite);

    void setSprite(const int layer, Sprite *const sprite);

    Sprite *getSprite(int layer) const A_WARN_UNUSED
    { return mSprites.at(layer); }

    void removeSprite(const int layer);

    void clear();

    void ensureSize(size_t layerCount);

    virtual void drawSprites(Graphics *const graphics,
                             int posX, int posY) const;

    virtual void drawSpritesSDL(Graphics *const graphics,
                                int posX, int posY) const;

    /**
     * Returns the curent frame in the current animation of the given layer.
     */
    virtual unsigned int getCurrentFrame(unsigned int layer)
                                         const A_WARN_UNUSED;

    /**
     * Returns the frame count in the current animation of the given layer.
     */
    virtual unsigned int getFrameCount(unsigned int layer) A_WARN_UNUSED;

    virtual void setAlpha(float alpha) override;

    bool updateNumber(const unsigned num) override final;

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

    mutable int mOffsetX;
    mutable int mOffsetY;
    std::vector<Sprite*> mSprites;
    mutable int mNextRedrawTime;
    static bool mEnableDelay;
    mutable bool mNeedsRedraw;
    bool mEnableAlphaFix;
    bool mDisableAdvBeingCaching;
    bool mDisableBeingCaching;
};

#endif  // BEING_COMPOUNDSPRITE_H
