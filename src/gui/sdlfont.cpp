/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/sdlfont.h"

#include "client.h"
#include "graphics.h"
#include "log.h"
#include "main.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/stringutils.h"

#include <guichan/exception.hpp>

const unsigned int CACHE_SIZE = 256;
const unsigned int CACHE_SIZE_SMALL1 = 2;
const unsigned int CACHE_SIZE_SMALL2 = 50;
const unsigned int CACHE_SIZE_SMALL3 = 170;
const unsigned int CLEAN_TIME = 5;

char *strBuf;

class SDLTextChunk
{
    public:
        SDLTextChunk(const std::string &text, const gcn::Color &color) :
            img(0), text(text), color(color)
        {
        }

        ~SDLTextChunk()
        {
            delete img;
            img = 0;
        }

        bool operator==(const SDLTextChunk &chunk) const
        {
            return (chunk.text == text && chunk.color == color);
        }

        void generate(TTF_Font *font)
        {
            SDL_Color sdlCol;
            sdlCol.b = static_cast<Uint8>(color.b);
            sdlCol.r = static_cast<Uint8>(color.r);
            sdlCol.g = static_cast<Uint8>(color.g);

            getSafeUtf8String(text, strBuf);

//            SDL_Surface *surface = TTF_RenderUTF8_Solid(
            SDL_Surface *surface = TTF_RenderUTF8_Blended(
                    font, strBuf, sdlCol);

            if (!surface)
            {
                img = 0;
                return;
            }

            img = Image::load(surface);

            SDL_FreeSurface(surface);
        }

        Image *img;
        std::string text;
        gcn::Color color;
};

typedef std::list<SDLTextChunk>::iterator CacheIterator;

static int fontCounter;

SDLFont::SDLFont(const std::string &filename, int size, int style) :
    mCreateCounter(0),
    mDeleteCounter(0)
{
    ResourceManager *resman = ResourceManager::getInstance();

    if (fontCounter == 0 && TTF_Init() == -1)
    {
        throw GCN_EXCEPTION("Unable to initialize SDL_ttf: " +
            std::string(TTF_GetError()));
    }

    if (!fontCounter)
    {
        strBuf = new char[65535];
        memset(strBuf, 0, 65535);
    }

    ++fontCounter;
    mFont = TTF_OpenFont(resman->getPath(filename).c_str(), size);

    if (!mFont)
    {
        logger->log("Error finding font " + filename);
        mFont = TTF_OpenFont(resman->getPath(
            "fonts/dejavusans.ttf").c_str(), size);
        if (!mFont)
        {
            throw GCN_EXCEPTION("SDLSDLFont::SDLSDLFont: " +
                                std::string(TTF_GetError()));
        }
    }

    TTF_SetFontStyle(mFont, style);
    mCleanTime = cur_time + CLEAN_TIME;
}

SDLFont::~SDLFont()
{
    TTF_CloseFont(mFont);
    mFont = 0;
    --fontCounter;

    if (fontCounter == 0)
    {
        TTF_Quit();
        delete []strBuf;
    }
}

void SDLFont::loadFont(const std::string &filename, int size, int style)
{
    ResourceManager *resman = ResourceManager::getInstance();

    if (fontCounter == 0 && TTF_Init() == -1)
    {
        logger->log("Unable to initialize SDL_ttf: " +
                    std::string(TTF_GetError()));
        return;
    }

    TTF_Font *font = TTF_OpenFont(resman->getPath(filename).c_str(), size);

    if (!font)
    {
        logger->log("SDLSDLFont::SDLSDLFont: " +
                    std::string(TTF_GetError()));
        return;
    }

    if (mFont)
        TTF_CloseFont(mFont);

    mFont = font;
    TTF_SetFontStyle(mFont, style);
    clear();
}

void SDLFont::clear()
{
    for (unsigned short f = 0; f < static_cast<unsigned short>(
        CACHES_NUMBER); f ++)
    {
        mCache[static_cast<unsigned short>(f)].clear();
    }
}

void SDLFont::drawString(gcn::Graphics *graphics,
                         const std::string &text,
                         int x, int y)
{
    if (text.empty())
        return;

    Graphics *g = dynamic_cast<Graphics *>(graphics);

    gcn::Color col = g->getColor();
    const float alpha = static_cast<float>(col.a) / 255.0f;

    /* The alpha value is ignored at string generation so avoid caching the
     * same text with different alpha values.
     */
    col.a = 255;

    SDLTextChunk chunk(text, col);

    unsigned char chr = text[0];
    std::list<SDLTextChunk> *cache = &mCache[chr];

    bool found = false;

#ifdef DEBUG_FONT
    int cnt = 0;
#endif

    for (CacheIterator i = cache->begin(); i != cache->end(); ++i)
    {
        if (chunk == (*i))
        {
            // Raise priority: move it to front
            cache->splice(cache->begin(), *cache, i);
            found = true;
            break;
        }
#ifdef DEBUG_FONT
        cnt ++;
#endif
    }
#ifdef DEBUG_FONT
    logger->log("drawString: " + text + ", iterations: " + toString(cnt));
#endif

    // Surface not found
    if (!found)
    {
        if (cache->size() >= CACHE_SIZE)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter ++;
#endif
            cache->pop_back();
        }
#ifdef DEBUG_FONT_COUNTERS
        mCreateCounter ++;
#endif
        cache->push_front(chunk);
        cache->front().generate(mFont);

        if (!mCleanTime)
        {
            mCleanTime = cur_time + CLEAN_TIME;
        }
        else if (mCleanTime < cur_time)
        {
            doClean();
            mCleanTime = cur_time + CLEAN_TIME;
        }
    }

    if (cache->front().img)
    {
        cache->front().img->setAlpha(alpha);
        g->drawImage(cache->front().img, x, y);
    }

}

void SDLFont::createSDLTextChunk(SDLTextChunk *chunk)
{
    if (!chunk || chunk->text.empty())
        return;

    const float alpha = static_cast<float>(chunk->color.a) / 255.0f;
    chunk->color.a = 255;
    chunk->generate(mFont);
    if (chunk->img)
        chunk->img->setAlpha(alpha);
}

int SDLFont::getWidth(const std::string &text) const
{
    if (text.empty())
        return 0;

    unsigned char chr = text[0];
    std::list<SDLTextChunk> *cache = &mCache[chr];

#ifdef DEBUG_FONT
    int cnt = 0;
#endif

    for (CacheIterator i = cache->begin(); i != cache->end(); ++i)
    {
        if (i->text == text)
        {
            // Raise priority: move it to front
            // Assumption is that TTF::draw will be called next
            cache->splice(cache->begin(), *cache, i);
            if (i->img)
                return i->img->getWidth();
            else
                return 0;
        }
#ifdef DEBUG_FONT
        cnt ++;
#endif
    }

#ifdef DEBUG_FONT
    logger->log("getWidth: " + text + ", iterations: " + toString(cnt));
#endif

    int w, h;
    getSafeUtf8String(text, strBuf);
    TTF_SizeUTF8(mFont, strBuf, &w, &h);
    return w;
}

int SDLFont::getHeight() const
{
    return TTF_FontHeight(mFont);
}

void SDLFont::doClean()
{
    for (int f = 0; f < CACHES_NUMBER; f ++)
    {
        std::list<SDLTextChunk> *cache = &mCache[f];
        const unsigned size = cache->size();
#ifdef DEBUG_FONT_COUNTERS
        logger->log("ptr: %d, size: %d", f, size);
#endif
        if (size > CACHE_SIZE_SMALL3)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter += 100;
#endif
            for (int d = 0; d < 100; d ++)
                cache->pop_back();
#ifdef DEBUG_FONT_COUNTERS
            logger->log("delete3");
#endif
        }
        else if (size > CACHE_SIZE_SMALL2)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter += 20;
#endif
            for (int d = 0; d < 20; d ++)
                cache->pop_back();
#ifdef DEBUG_FONT_COUNTERS
            logger->log("delete2");
#endif
        }
        else if (size > CACHE_SIZE_SMALL1)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter ++;
#endif
            cache->pop_back();
#ifdef DEBUG_FONT_COUNTERS
            logger->log("delete1");
#endif
        }
    }
}
