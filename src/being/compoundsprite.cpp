/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "being/compoundsprite.h"

#include "configuration.h"

#include "sdlshared.h"

#include "being/compounditem.h"

#include "render/surfacegraphics.h"

#if defined(USE_OPENGL) || !defined(USE_SDL2)
#include "resources/imagehelper.h"
#endif  // USE_OPENGL
#include "resources/image/image.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/foreach.h"
#include "utils/likely.h"
#include "utils/sdlcheckutils.h"

#ifndef USE_SDL2
#include "game.h"

#include "const/resources/map/map.h"

#include "resources/map/map.h"

#include "utils/timer.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER
PRAGMA48(GCC diagnostic pop)
#endif  // USE_SDL2

#include "debug.h"

#ifndef USE_SDL2
static const int BUFFER_WIDTH = 100;
static const int BUFFER_HEIGHT = 100;
static const unsigned cache_max_size = 10;
static const unsigned cache_clean_part = 3;
#endif  // USE_SDL2

bool CompoundSprite::mEnableDelay = true;

CompoundSprite::CompoundSprite() :
    Sprite(),
    mSprites(),
    imagesCache(),
    mCacheItem(nullptr),
    mImage(nullptr),
    mAlphaImage(nullptr),
    mOffsetX(0),
    mOffsetY(0),
    mStartTime(0),
    mLastTime(0),
#ifndef USE_SDL2
    mNextRedrawTime(0),
#endif  // USE_SDL2
    mNeedsRedraw(false),
    mEnableAlphaFix(config.getBoolValue("enableAlphaFix")),
    mDisableAdvBeingCaching(config.getBoolValue("disableAdvBeingCaching")),
    mDisableBeingCaching(config.getBoolValue("disableBeingCaching"))
{
    mAlpha = 1.0F;
}

CompoundSprite::~CompoundSprite()
{
    clear();
    mImage = nullptr;
    mAlphaImage = nullptr;
}

bool CompoundSprite::reset()
{
    bool ret = false;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it != nullptr)
            ret |= (*it)->reset();
    }
    if (ret)
        mLastTime = 0;
    mNeedsRedraw |= ret;
    return ret;
}

bool CompoundSprite::play(const std::string &action)
{
    bool ret = false;
    bool ret2 = true;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it != nullptr)
        {
            const bool tmpVal = (*it)->play(action);
            ret |= tmpVal;
            ret2 &= tmpVal;
        }
    }
    mNeedsRedraw |= ret;
    if (ret2)
        mLastTime = 0;
    return ret;
}

bool CompoundSprite::update(const int time)
{
    bool ret = false;
    if (A_UNLIKELY(mLastTime == 0))
        mStartTime = time;
    mLastTime = time;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it != nullptr)
            ret |= (*it)->update(time);
    }
    mNeedsRedraw |= ret;
    return ret;
}

void CompoundSprite::drawSimple(Graphics *const graphics,
                                const int posX,
                                const int posY) const
{
    FUNC_BLOCK("CompoundSprite::draw", 1)
    if (mNeedsRedraw)
        updateImages();

    if (mSprites.empty())  // Nothing to draw
        return;

    if (mAlpha == 1.0F && (mImage != nullptr))
    {
        graphics->drawImage(mImage, posX + mOffsetX, posY + mOffsetY);
    }
    else if ((mAlpha != 0.0f) && (mAlphaImage != nullptr))
    {
        mAlphaImage->setAlpha(mAlpha);
        graphics->drawImage(mAlphaImage,
            posX + mOffsetX, posY + mOffsetY);
    }
    else
    {
        CompoundSprite::drawSprites(graphics, posX, posY);
    }
}

void CompoundSprite::drawSprites(Graphics *const graphics,
                                 const int posX,
                                 const int posY) const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it != nullptr)
        {
            (*it)->setAlpha(mAlpha);
            (*it)->draw(graphics, posX, posY);
        }
    }
}

void CompoundSprite::drawSpritesSDL(Graphics *const graphics,
                                    const int posX,
                                    const int posY) const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it != nullptr)
            (*it)->draw(graphics, posX, posY);
    }
}

int CompoundSprite::getWidth() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        const Sprite *const base = *it;
        if (base != nullptr)
            return base->getWidth();
    }

    return 0;
}

int CompoundSprite::getHeight() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        const Sprite *const base = *it;
        if (base != nullptr)
            return base->getHeight();
    }

    return 0;
}

const Image *CompoundSprite::getImage() const
{
    return mImage;
}

bool CompoundSprite::setSpriteDirection(const SpriteDirection::Type direction)
{
    bool ret = false;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it != nullptr)
            ret |= (*it)->setSpriteDirection(direction);
    }
    if (ret)
        mLastTime = 0;
    mNeedsRedraw |= ret;
    return ret;
}

int CompoundSprite::getNumberOfLayers() const
{
    if ((mImage != nullptr) || (mAlphaImage != nullptr))
        return 1;
    return CAST_S32(mSprites.size());
}

unsigned int CompoundSprite::getCurrentFrame() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it != nullptr)
            return (*it)->getCurrentFrame();
    }
    return 0;
}

unsigned int CompoundSprite::getFrameCount() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it != nullptr)
            return (*it)->getFrameCount();
    }
    return 0;
}

void CompoundSprite::addSprite(Sprite *const sprite)
{
    mSprites.push_back(sprite);
    mNeedsRedraw = true;
}

void CompoundSprite::setSprite(const size_t layer, Sprite *const sprite)
{
    // Skip if it won't change anything
    if (mSprites[layer] == sprite)
        return;

    delete mSprites[layer];
    mSprites[layer] = sprite;
    mNeedsRedraw = true;
}

void CompoundSprite::removeSprite(const int layer)
{
    // Skip if it won't change anything
    if (mSprites[layer] == nullptr)
        return;

    delete2(mSprites[layer]);
    mNeedsRedraw = true;
}

void CompoundSprite::clear()
{
    // Skip if it won't change anything
    if (!mSprites.empty())
    {
        delete_all(mSprites);
        mSprites.clear();
    }
    mNeedsRedraw = true;
    delete_all(imagesCache);
    imagesCache.clear();
    delete2(mCacheItem);
    mLastTime = 0;
}

void CompoundSprite::ensureSize(const size_t layerCount)
{
    // Skip if it won't change anything
    if (mSprites.size() >= layerCount)
        return;

//    resize(layerCount, nullptr);
    mSprites.resize(layerCount);
}

void CompoundSprite::redraw() const
{
#ifndef USE_SDL2

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
    const int amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
    const int amask = 0xff000000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    SDL_Surface *const surface = MSDL_CreateRGBSurface(SDL_HWSURFACE,
        BUFFER_WIDTH, BUFFER_HEIGHT, 32, rmask, gmask, bmask, amask);

    if (surface == nullptr)
        return;

    SurfaceGraphics *graphics = new SurfaceGraphics;
    graphics->setBlitMode(BlitMode::BLIT_GFX);
    graphics->setTarget(surface);
    graphics->beginDraw();

    int tileX = mapTileSize / 2;
    int tileY = mapTileSize;

    const Game *const game = Game::instance();
    if (game != nullptr)
    {
        const Map *const map = game->getCurrentMap();
        if (map != nullptr)
        {
            tileX = map->getTileWidth() / 2;
            tileY = map->getTileWidth();
        }
    }

    const int posX = BUFFER_WIDTH / 2 - tileX;
    const int posY = BUFFER_HEIGHT - tileY;

    mOffsetX = tileX - BUFFER_WIDTH / 2;
    mOffsetY = tileY - BUFFER_HEIGHT;

    drawSpritesSDL(graphics, posX, posY);

    delete2(graphics);

    SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);

    delete mAlphaImage;

    if (ImageHelper::mEnableAlpha)
    {
        SDL_Surface *const surfaceA = MSDL_CreateRGBSurface(SDL_HWSURFACE,
            BUFFER_WIDTH, BUFFER_HEIGHT, 32, rmask, gmask, bmask, amask);
        SDL_BlitSurface(surface, nullptr, surfaceA, nullptr);
        mAlphaImage = imageHelper->loadSurface(surfaceA);
        MSDL_FreeSurface(surfaceA);
    }
    else
    {
        mAlphaImage = nullptr;
    }

    delete mImage;
    mImage = imageHelper->loadSurface(surface);
    MSDL_FreeSurface(surface);
#endif  // USE_SDL2
}

void CompoundSprite::setAlpha(float alpha)
{
    if (alpha != mAlpha)
    {
        if (mEnableAlphaFix &&
#ifdef USE_OPENGL
            imageHelper->useOpenGL() == RENDER_SOFTWARE &&
#endif  // USE_OPENGL
            mSprites.size() > 3U)
        {
            FOR_EACH (SpriteConstIterator, it, mSprites)
            {
                if (*it != nullptr)
                    (*it)->setAlpha(alpha);
            }
        }
        mAlpha = alpha;
    }
}

void CompoundSprite::updateImages() const
{
#ifndef USE_SDL2
#ifdef USE_OPENGL
    if (imageHelper->useOpenGL() != RENDER_SOFTWARE)
        return;
#endif  // USE_OPENGL

    if (mEnableDelay)
    {
        if (get_elapsed_time1(mNextRedrawTime) < 10)
            return;
        mNextRedrawTime = tick_time;
    }
    mNeedsRedraw = false;

    if (!mDisableBeingCaching)
    {
        if (mSprites.size() <= 3U)
            return;

        if (!mDisableAdvBeingCaching)
        {
            if (updateFromCache())
                return;

            redraw();

            if (mImage != nullptr)
                initCurrentCacheItem();
        }
        else
        {
            redraw();
        }
    }
#endif  // USE_SDL2
}

bool CompoundSprite::updateFromCache() const
{
#ifndef USE_SDL2
//    static int hits = 0;
//    static int miss = 0;

    if ((mCacheItem != nullptr) && (mCacheItem->image != nullptr))
    {
        imagesCache.push_front(mCacheItem);
        mCacheItem = nullptr;
        if (imagesCache.size() > cache_max_size)
        {
            for (unsigned f = 0; f < cache_clean_part; f ++)
            {
                CompoundItem *item = imagesCache.back();
                imagesCache.pop_back();
                delete item;
            }
        }
    }

//    logger->log("cache size: %d, hit %d, miss %d",
//        (int)imagesCache.size(), hits, miss);

    const size_t sz = mSprites.size();
    FOR_EACH (ImagesCache::iterator, it, imagesCache)
    {
        CompoundItem *const ic = *it;
        if ((ic != nullptr) && ic->data.size() == sz)
        {
            bool fail(false);
            VectorPointers::const_iterator it2 = ic->data.begin();
            const VectorPointers::const_iterator it2_end = ic->data.end();

            for (SpriteConstIterator it1 = mSprites.begin(),
                 it1_end = mSprites.end();
                 it1 != it1_end && it2 != it2_end;
                 ++ it1, ++ it2)
            {
                const void *ptr1 = nullptr;
                const void *ptr2 = nullptr;
                if (*it1 != nullptr)
                    ptr1 = (*it1)->getHash();
                if (*it2 != nullptr)
                    ptr2 = *it2;
                if (ptr1 != ptr2)
                {
                    fail = true;
                    break;
                }
            }
            if (!fail)
            {
//                hits ++;
                mImage = (*it)->image;
                mAlphaImage = (*it)->alphaImage;
                imagesCache.erase(it);
                mCacheItem = ic;
                return true;
            }
        }
    }
    mImage = nullptr;
    mAlphaImage = nullptr;
//    miss++;
#endif  // USE_SDL2
    return false;
}

void CompoundSprite::initCurrentCacheItem() const
{
    delete mCacheItem;
    mCacheItem = new CompoundItem;
    mCacheItem->image = mImage;
    mCacheItem->alphaImage = mAlphaImage;
//    mCacheItem->alpha = mAlpha;

    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it != nullptr)
            mCacheItem->data.push_back((*it)->getHash());
        else
            mCacheItem->data.push_back(nullptr);
    }
}

bool CompoundSprite::updateNumber(const unsigned num)
{
    bool res(false);
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it != nullptr)
        {
            if ((*it)->updateNumber(num))
                res = true;
        }
    }
    return res;
}

CompoundItem::CompoundItem() :
    data(),
    image(nullptr),
    alphaImage(nullptr)
{
}

CompoundItem::~CompoundItem()
{
    delete image;
    delete alphaImage;
}
