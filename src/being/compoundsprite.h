/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef BEING_COMPOUNDSPRITE_H
#define BEING_COMPOUNDSPRITE_H

#include "resources/sprite/sprite.h"

#include "utils/vector.h"

#include <list>

#include "localconsts.h"

class CompoundItem;
class Image;

class CompoundSprite notfinal : public Sprite
{
    public:
        typedef std::vector<Sprite*>::iterator SpriteIterator;
        typedef std::vector<Sprite*>::const_iterator SpriteConstIterator;

        CompoundSprite();

        A_DELETE_COPY(CompoundSprite)

        virtual ~CompoundSprite();

        bool reset() override final;

        bool play(const std::string &action) override final;

        bool update(const int time) override final;

        void drawSimple(Graphics *const graphics,
                        const int posX,
                        const int posY) const A_NONNULL(2);

        /**
         * Gets the width in pixels of the first sprite in the list.
         */
        int getWidth() const override A_WARN_UNUSED;

        /**
         * Gets the height in pixels of the first sprite in the list.
         */
        int getHeight() const override A_WARN_UNUSED;

        const Image *getImage() const override final A_WARN_UNUSED;

        bool setSpriteDirection(const SpriteDirection::Type direction)
                                override final;

        int getNumberOfLayers() const A_WARN_UNUSED;

        unsigned int getCurrentFrame() const override final A_WARN_UNUSED;

        unsigned int getFrameCount() const override final A_WARN_UNUSED;

        void addSprite(Sprite *const sprite);

        void setSprite(const size_t layer, Sprite *const sprite);

        void removeSprite(const int layer);

        void clear();

        void ensureSize(const size_t layerCount);

        void drawSprites(Graphics *const graphics,
                         const int posX,
                         const int posY) const;

        virtual void drawSpritesSDL(Graphics *const graphics,
                                    const int posX,
                                    const int posY) const;

        void setAlpha(float alpha) override;

        bool updateNumber(const unsigned num) override final;

        constexpr2 static void setEnableDelay(bool b) noexcept2
        { mEnableDelay = b; }

        int getLastTime() const noexcept2 A_WARN_UNUSED
        { return mLastTime; }

        int getStartTime() const noexcept2 A_WARN_UNUSED
        { return mStartTime; }

        std::vector<Sprite*> mSprites;

    protected:
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
        int mStartTime;
        int mLastTime;
#ifndef USE_SDL2
        mutable int mNextRedrawTime;
#endif  // USE_SDL2

        static bool mEnableDelay;
        mutable bool mNeedsRedraw;
        bool mEnableAlphaFix;
        bool mDisableAdvBeingCaching;
        bool mDisableBeingCaching;
};

#endif  // BEING_COMPOUNDSPRITE_H
