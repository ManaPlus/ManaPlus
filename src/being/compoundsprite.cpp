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

#include "being/compoundsprite.h"

#include "configuration.h"
#include "game.h"

#ifdef USE_OPENGL
#include "main.h"
#endif

#include "map.h"
#include "sdlshared.h"

#include "render/surfacegraphics.h"

#include "resources/image.h"
#include "resources/imagehelper.h"

#include "utils/dtor.h"
#include "utils/sdlcheckutils.h"
#include "utils/timer.h"

#include <SDL_endian.h>

#include "debug.h"

static const int BUFFER_WIDTH = 100;
static const int BUFFER_HEIGHT = 100;

static const unsigned cache_max_size = 10;
static const unsigned cache_clean_part = 3;
bool CompoundSprite::mEnableDelay = true;

CompoundSprite::CompoundSprite() :
    imagesCache(),
    mCacheItem(nullptr),
    mImage(nullptr),
    mAlphaImage(nullptr),
    mOffsetX(0),
    mOffsetY(0),
    mSprites(),
    mNextRedrawTime(0),
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
        if (*it)
            ret |= (*it)->reset();
    }
    mNeedsRedraw |= ret;
    return ret;
}

bool CompoundSprite::play(const std::string &action)
{
    bool ret = false;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it)
            ret |= (*it)->play(action);
    }
    mNeedsRedraw |= ret;
    return ret;
}

bool CompoundSprite::update(const int time)
{
    bool ret = false;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it)
            ret |= (*it)->update(time);
    }
    mNeedsRedraw |= ret;
    return ret;
}

void CompoundSprite::draw(Graphics *const graphics,
                          const int posX, const int posY) const
{
    FUNC_BLOCK("CompoundSprite::draw", 1)
    if (mNeedsRedraw)
        updateImages();

    if (mSprites.empty())  // Nothing to draw
        return;

    if (mAlpha == 1.0F && mImage)
    {
        DRAW_IMAGE(graphics, mImage, posX + mOffsetX, posY + mOffsetY);
    }
    else if (mAlpha && mAlphaImage)
    {
        mAlphaImage->setAlpha(mAlpha);
        DRAW_IMAGE(graphics, mAlphaImage,
            posX + mOffsetX, posY + mOffsetY);
    }
    else
    {
        drawSprites(graphics, posX, posY);
    }
}

void CompoundSprite::drawSprites(Graphics *const graphics,
                                 int posX, int posY) const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it)
        {
            (*it)->setAlpha(mAlpha);
            (*it)->draw(graphics, posX, posY);
        }
    }
}

void CompoundSprite::drawSpritesSDL(Graphics *const graphics,
                                    int posX, int posY) const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it)
            (*it)->draw(graphics, posX, posY);
    }
}

int CompoundSprite::getWidth() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        const Sprite *const base = *it;
        if (base)
            return base->getWidth();
    }

    return 0;
}

int CompoundSprite::getHeight() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        const Sprite *const base = *it;
        if (base)
            return base->getHeight();
    }

    return 0;
}

const Image *CompoundSprite::getImage() const
{
    return mImage;
}

bool CompoundSprite::setSpriteDirection(const SpriteDirection direction)
{
    bool ret = false;
    FOR_EACH (SpriteIterator, it, mSprites)
    {
        if (*it)
            ret |= (*it)->setSpriteDirection(direction);
    }
    mNeedsRedraw |= ret;
    return ret;
}

int CompoundSprite::getNumberOfLayers() const
{
    if (mImage || mAlphaImage)
        return 1;
    else
        return static_cast<int>(size());
}

unsigned int CompoundSprite::getCurrentFrame() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it)
            return (*it)->getCurrentFrame();
    }
    return 0;
}

unsigned int CompoundSprite::getFrameCount() const
{
    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it)
            return (*it)->getFrameCount();
    }
    return 0;
}

void CompoundSprite::addSprite(Sprite *const sprite)
{
    mSprites.push_back(sprite);
    mNeedsRedraw = true;
}

void CompoundSprite::setSprite(const int layer, Sprite *const sprite)
{
    // Skip if it won't change anything
    if (mSprites.at(layer) == sprite)
        return;

    delete mSprites.at(layer);
    mSprites[layer] = sprite;
    mNeedsRedraw = true;
}

void CompoundSprite::removeSprite(const int layer)
{
    // Skip if it won't change anything
    if (!mSprites.at(layer))
        return;

    delete mSprites.at(layer);
    mSprites.at(layer) = nullptr;
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
    delete mCacheItem;
    mCacheItem = nullptr;
}

void CompoundSprite::ensureSize(size_t layerCount)
{
    // Skip if it won't change anything
    if (mSprites.size() >= layerCount)
        return;

//    resize(layerCount, nullptr);
    mSprites.resize(layerCount);
}

/**
 * Returns the curent frame in the current animation of the given layer.
 */
unsigned int CompoundSprite::getCurrentFrame(unsigned int layer) const
{
    if (layer >= mSprites.size())
        return 0;

    const Sprite *const s = getSprite(layer);
    if (s)
        return s->getCurrentFrame();

    return 0;
}

/**
 * Returns the frame count in the current animation of the given layer.
 */
unsigned int CompoundSprite::getFrameCount(unsigned int layer)
{
    if (layer >= mSprites.size())
        return 0;

    const Sprite *const s = getSprite(layer);
    if (s)
        return s->getFrameCount();

    return 0;
}

void CompoundSprite::redraw() const
{
#ifndef USE_SDL2

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
    const int amask = 0x000000ff;
#else
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
    const int amask = 0xff000000;
#endif

    SDL_Surface *const surface = MSDL_CreateRGBSurface(SDL_HWSURFACE,
        BUFFER_WIDTH, BUFFER_HEIGHT, 32, rmask, gmask, bmask, amask);

    if (!surface)
        return;

    SurfaceGraphics *graphics = new SurfaceGraphics();
    graphics->setBlitMode(SurfaceGraphics::BLIT_GFX);
    graphics->setTarget(surface);
    graphics->_beginDraw();

    int tileX = mapTileSize / 2;
    int tileY = mapTileSize;

    const Game *const game = Game::instance();
    if (game)
    {
        const Map *const map = game->getCurrentMap();
        if (map)
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

    delete graphics;
    graphics = nullptr;

    SDL_Surface *const surfaceA = MSDL_CreateRGBSurface(SDL_HWSURFACE,
        BUFFER_WIDTH, BUFFER_HEIGHT, 32, rmask, gmask, bmask, amask);

#ifdef USE_SDL2
    SDL_SetSurfaceAlphaMod(surface, 255);
#else
    SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
#endif
    SDL_BlitSurface(surface, nullptr, surfaceA, nullptr);

    delete mImage;
    delete mAlphaImage;

    mImage = imageHelper->load(surface);
    MSDL_FreeSurface(surface);

    if (ImageHelper::mEnableAlpha)
    {
        mAlphaImage = imageHelper->load(surfaceA);
        MSDL_FreeSurface(surfaceA);
    }
    else
    {
        mAlphaImage = nullptr;
    }
#endif
}

void CompoundSprite::setAlpha(float alpha)
{
    if (alpha != mAlpha)
    {
#ifdef USE_OPENGL
        if (mEnableAlphaFix && imageHelper->useOpenGL() == RENDER_SOFTWARE
            && size() > 3)
#else
        if (mEnableAlphaFix && size() > 3)
#endif
        {
            FOR_EACH (SpriteConstIterator, it, mSprites)
            {
                if (*it)
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
#endif

    if (mEnableDelay)
    {
        if (get_elapsed_time1(mNextRedrawTime) < 10)
            return;
        mNextRedrawTime = tick_time;
    }
    mNeedsRedraw = false;

    if (!mDisableBeingCaching)
    {
        if (size() <= 3)
            return;

        if (!mDisableAdvBeingCaching)
        {
            if (updateFromCache())
                return;

            redraw();

            if (mImage)
                initCurrentCacheItem();
        }
        else
        {
            redraw();
        }
    }
#endif
}

bool CompoundSprite::updateFromCache() const
{
#ifndef USE_SDL2
//    static int hits = 0;
//    static int miss = 0;

    if (mCacheItem && mCacheItem->image)
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

    const size_t sz = size();
    FOR_EACH (ImagesCache::iterator, it, imagesCache)
    {
        CompoundItem *const ic = *it;
        if (ic && ic->data.size() == sz)
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
                if (*it1)
                    ptr1 = (*it1)->getHash();
                if (*it2)
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
#endif
    return false;
}

void CompoundSprite::initCurrentCacheItem() const
{
    delete mCacheItem;
    mCacheItem = new CompoundItem();
    mCacheItem->image = mImage;
    mCacheItem->alphaImage = mAlphaImage;
//    mCacheItem->alpha = mAlpha;

    FOR_EACH (SpriteConstIterator, it, mSprites)
    {
        if (*it)
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
        if (*it)
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
