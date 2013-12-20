/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/sdlfont.h"

#include "logger.h"
#include "main.h"

#include "render/sdlgraphics.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"
#include "resources/surfaceimagehelper.h"

#include "utils/paths.h"
#include "utils/physfsrwops.h"
#include "utils/sdlcheckutils.h"
#include "utils/stringutils.h"
#include "utils/timer.h"

#include <guichan/exception.hpp>

#include "debug.h"

const unsigned int CACHE_SIZE = 256;
const unsigned int CACHE_SIZE_SMALL1 = 2;
const unsigned int CACHE_SIZE_SMALL2 = 50;
const unsigned int CACHE_SIZE_SMALL3 = 170;
const unsigned int CLEAN_TIME = 7;
const int OUTLINE_SIZE = 1;

bool SDLFont::mSoftMode(false);

char *strBuf = nullptr;

#ifdef UNITTESTS
int sdlTextChunkCnt = 0;
#endif

SDLTextChunkSmall::SDLTextChunkSmall(const std::string &text0,
                                     const gcn::Color &color0,
                                     const gcn::Color &color1) :
    text(text0),
    color(color0),
    color2(color1)
{
}

SDLTextChunkSmall::SDLTextChunkSmall(const SDLTextChunkSmall &old) :
    text(old.text),
    color(old.color),
    color2(old.color2)
{
}

bool SDLTextChunkSmall::operator==(const SDLTextChunkSmall &chunk) const
{
    return (chunk.text == text && chunk.color == color
            && chunk.color2 == color2);
}

bool SDLTextChunkSmall::operator<(const SDLTextChunkSmall &chunk) const
{
    if (chunk.text != text)
        return chunk.text > text;

    const gcn::Color &c = chunk.color;
    if (c.r != color.r)
        return c.r > color.r;
    if (c.g != color.g)
        return c.g > color.g;
    if (c.b != color.b)
        return c.b > color.b;

    const gcn::Color &c2 = chunk.color2;
    if (c2.r != color2.r)
        return c2.r > color2.r;
    if (c2.g != color2.g)
        return c2.g > color2.g;
    if (c2.b != color2.b)
        return c2.b > color2.b;

    if (c.a != color.a && SDLFont::mSoftMode)
        return c.a > color.a;

    return false;
}

SDLTextChunk::SDLTextChunk(const std::string &text0, const gcn::Color &color0,
                           const gcn::Color &color1) :
    img(nullptr),
    text(text0),
    color(color0),
    color2(color1),
    prev(nullptr),
    next(nullptr)
{
#ifdef UNITTESTS
    sdlTextChunkCnt ++;
#endif
}

SDLTextChunk::~SDLTextChunk()
{
    delete img;
    img = nullptr;
#ifdef UNITTESTS
    sdlTextChunkCnt --;
#endif
}

bool SDLTextChunk::operator==(const SDLTextChunk &chunk) const
{
    return (chunk.text == text && chunk.color == color
            && chunk.color2 == color2);
}

void SDLTextChunk::generate(TTF_Font *const font, const float alpha)
{
    BLOCK_START("SDLTextChunk::generate")
    SDL_Color sdlCol;
    sdlCol.b = static_cast<uint8_t>(color.b);
    sdlCol.r = static_cast<uint8_t>(color.r);
    sdlCol.g = static_cast<uint8_t>(color.g);
#ifdef USE_SDL2
    sdlCol.a = 255;
#else
    sdlCol.unused = 0;
#endif

    getSafeUtf8String(text, strBuf);

    SDL_Surface *surface = MTTF_RenderUTF8_Blended(
        font, strBuf, sdlCol);

    if (!surface)
    {
        img = nullptr;
        BLOCK_END("SDLTextChunk::generate")
        return;
    }

    const int width = surface->w;
    const int height = surface->h;

    if (color.r != color2.r || color.g != color2.g
        || color.b != color2.b)
    {   // outlining
        SDL_Color sdlCol2;
        SDL_Surface *const background = imageHelper->create32BitSurface(
            width, height);
        if (!background)
        {
            img = nullptr;
            MSDL_FreeSurface(surface);
            BLOCK_END("SDLTextChunk::generate")
            return;
        }
        sdlCol2.b = static_cast<uint8_t>(color2.b);
        sdlCol2.r = static_cast<uint8_t>(color2.r);
        sdlCol2.g = static_cast<uint8_t>(color2.g);
#ifdef USE_SDL2
        sdlCol2.a = 255;
#else
        sdlCol2.unused = 0;
#endif
        SDL_Surface *const surface2 = MTTF_RenderUTF8_Blended(
            font, strBuf, sdlCol2);
        if (!surface2)
        {
            img = nullptr;
            MSDL_FreeSurface(surface);
            BLOCK_END("SDLTextChunk::generate")
            return;
        }
        SDL_Rect rect =
        {
            OUTLINE_SIZE,
            0,
            static_cast<Uint16>(surface->w),
            static_cast<Uint16>(surface->h)
        };
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.x = -OUTLINE_SIZE;
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.x = 0;
        rect.y = -OUTLINE_SIZE;
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.y = OUTLINE_SIZE;
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.x = 0;
        rect.y = 0;
        SurfaceImageHelper::combineSurface(surface, nullptr,
            background, &rect);
        MSDL_FreeSurface(surface);
        MSDL_FreeSurface(surface2);
        surface = background;
    }
    img = imageHelper->createTextSurface(
        surface, width, height, alpha);
    MSDL_FreeSurface(surface);

    BLOCK_END("SDLTextChunk::generate")
}


TextChunkList::TextChunkList() :
    start(nullptr),
    end(nullptr),
    size(0),
    search(),
    searchWidth()
{
}

void TextChunkList::insertFirst(SDLTextChunk *const item)
{
    SDLTextChunk *const oldFirst = start;
    if (start)
        start->prev = item;
    item->prev = nullptr;
    if (oldFirst)
        item->next = oldFirst;
    else
        end = item;
    start = item;
    size ++;
    search[SDLTextChunkSmall(item->text, item->color, item->color2)] = item;
    searchWidth[item->text] = item;
}

void TextChunkList::moveToFirst(SDLTextChunk *item)
{
    if (item == start)
        return;

    SDLTextChunk *oldPrev = item->prev;
    if (oldPrev)
        oldPrev->next = item->next;
    SDLTextChunk *oldNext = item->next;
    if (oldNext)
        oldNext->prev = item->prev;
    else
        end = oldPrev;
    SDLTextChunk *const oldFirst = start;
    if (start)
        start->prev = item;
    item->prev = nullptr;
    item->next = oldFirst;
    start = item;
}

void TextChunkList::removeBack()
{
    SDLTextChunk *oldEnd = end;
    if (oldEnd)
    {
        end = oldEnd->prev;
        if (end)
            end->next = nullptr;
        else
            start = nullptr;
        search.erase(SDLTextChunkSmall(oldEnd->text,
            oldEnd->color, oldEnd->color2));
        searchWidth.erase(oldEnd->text);
        delete oldEnd;
        size --;
    }
}

void TextChunkList::removeBack(int n)
{
    SDLTextChunk *item = end;
    while (n && item)
    {
        n --;
        SDLTextChunk *oldEnd = item;
        item = item->prev;
        search.erase(SDLTextChunkSmall(oldEnd->text,
            oldEnd->color, oldEnd->color2));
        searchWidth.erase(oldEnd->text);
        delete oldEnd;
        size --;
    }
    if (item)
    {
        item->next = nullptr;
        end = item;
    }
    else
    {
        start = nullptr;
        end = nullptr;
    }
}

void TextChunkList::clear()
{
    search.clear();
    searchWidth.clear();
    SDLTextChunk *item = start;
    while (item)
    {
        SDLTextChunk *item2 = item->next;
        delete item;
        item = item2;
    }
    start = nullptr;
    end = nullptr;
    size = 0;
}

static int fontCounter;

SDLFont::SDLFont(std::string filename,
                 const int size,
                 const int style) :
    mFont(nullptr),
    mCreateCounter(0),
    mDeleteCounter(0),
    mCleanTime(cur_time + CLEAN_TIME)
{
    if (fontCounter == 0)
    {
        mSoftMode = imageHelper->useOpenGL() == RENDER_SOFTWARE;
        if (TTF_Init() == -1)
        {
            throw GCN_EXCEPTION("Unable to initialize SDL_ttf: " +
                std::string(TTF_GetError()));
        }
    }

    if (!fontCounter)
    {
        strBuf = new char[65535];
        memset(strBuf, 0, 65535);
    }

    ++fontCounter;

    fixDirSeparators(filename);
    mFont = openFont(filename.c_str(), size);

    if (!mFont)
    {
        logger->log("Error finding font " + filename);
        std::string backFile("fonts/dejavusans.ttf");
        mFont = openFont(fixDirSeparators(backFile).c_str(), size);
        if (!mFont)
        {
            throw GCN_EXCEPTION("SDLSDLFont::SDLSDLFont: " +
                                std::string(TTF_GetError()));
        }
    }

    TTF_SetFontStyle(mFont, style);
}

SDLFont::~SDLFont()
{
    TTF_CloseFont(mFont);
    mFont = nullptr;
    --fontCounter;
    clear();

    if (fontCounter == 0)
    {
        TTF_Quit();
        delete []strBuf;
    }
}

TTF_Font *SDLFont::openFont(const char *const name, const int size)
{
// disabled for now because some systems like gentoo cant use it
//#ifdef USE_SDL2
//    SDL_RWops *const rw = MPHYSFSRWOPS_openRead(name);
//    if (!rw)
//        return nullptr;
//    return TTF_OpenFontIndexRW(rw, 1, size, 0);
//#else
    return TTF_OpenFontIndex(ResourceManager::getInstance()->getPath(
        name).c_str(), size, 0);
//#endif
}

void SDLFont::loadFont(std::string filename,
                       const int size,
                       const int style)
{
    if (fontCounter == 0 && TTF_Init() == -1)
    {
        logger->log("Unable to initialize SDL_ttf: " +
                    std::string(TTF_GetError()));
        return;
    }

    fixDirSeparators(filename);
    TTF_Font *const font = openFont(filename.c_str(), size);

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
    for (size_t f = 0; f < CACHES_NUMBER; f ++)
        mCache[f].clear();
}

void SDLFont::drawString(gcn::Graphics *const graphics,
                         const std::string &text,
                         const int x, const int y)
{
    BLOCK_START("SDLFont::drawString")
    if (text.empty())
    {
        BLOCK_END("SDLFont::drawString")
        return;
    }

    Graphics *const g = dynamic_cast<Graphics *const>(graphics);
    if (!g)
        return;

    gcn::Color col = g->getColor();
    const gcn::Color &col2 = g->getColor2();
    const float alpha = static_cast<float>(col.a) / 255.0F;

    /* The alpha value is ignored at string generation so avoid caching the
     * same text with different alpha values.
     */
    col.a = 255;

    const unsigned char chr = text[0];
    TextChunkList *const cache = &mCache[chr];

    std::map<SDLTextChunkSmall, SDLTextChunk*> &search = cache->search;
    std::map<SDLTextChunkSmall, SDLTextChunk*>::iterator i
        = search.find(SDLTextChunkSmall(text, col, col2));
    if (i != search.end())
    {
        SDLTextChunk *const chunk2 = (*i).second;
        cache->moveToFirst(chunk2);
        Image *const image = chunk2->img;
        if (image)
        {
            image->setAlpha(alpha);
            DRAW_IMAGE(g, image, x, y);
        }
    }
    else
    {
        if (cache->size >= CACHE_SIZE)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter ++;
#endif
            cache->removeBack();
        }
#ifdef DEBUG_FONT_COUNTERS
        mCreateCounter ++;
#endif
        SDLTextChunk *chunk2 = new SDLTextChunk(text, col, col2);

        chunk2->generate(mFont, alpha);
        cache->insertFirst(chunk2);

        const Image *const image = chunk2->img;
        if (image)
            DRAW_IMAGE(g, image, x, y);
    }
    BLOCK_END("SDLFont::drawString")
}

void SDLFont::slowLogic(const int rnd)
{
    BLOCK_START("SDLFont::slowLogic")
    if (!mCleanTime)
    {
        mCleanTime = cur_time + CLEAN_TIME + rnd;
    }
    else if (mCleanTime < cur_time)
    {
        doClean();
        mCleanTime = cur_time + CLEAN_TIME + rnd;
    }
    BLOCK_END("SDLFont::slowLogic")
}

int SDLFont::getWidth(const std::string &text) const
{
    if (text.empty())
        return 0;

    const unsigned char chr = text[0];
    TextChunkList *const cache = &mCache[chr];

    std::map<std::string, SDLTextChunk*> &search = cache->searchWidth;
    std::map<std::string, SDLTextChunk*>::iterator i = search.find(text);
    if (i != search.end())
    {
        SDLTextChunk *const chunk = (*i).second;
        cache->moveToFirst(chunk);
        const Image *const image = chunk->img;
        if (image)
            return image->getWidth();
        else
            return 0;
    }

    // if string was not drawed
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
    for (unsigned int f = 0; f < CACHES_NUMBER; f ++)
    {
        TextChunkList *const cache = &mCache[f];
        const size_t size = cache->size;
#ifdef DEBUG_FONT_COUNTERS
        logger->log("ptr: %d, size: %d", f, size);
#endif
        if (size > CACHE_SIZE_SMALL3)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter += 100;
#endif
            cache->removeBack(100);
#ifdef DEBUG_FONT_COUNTERS
            logger->log("delete3");
#endif
        }
        else if (size > CACHE_SIZE_SMALL2)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter += 20;
#endif
            cache->removeBack(20);
#ifdef DEBUG_FONT_COUNTERS
            logger->log("delete2");
#endif
        }
        else if (size > CACHE_SIZE_SMALL1)
        {
#ifdef DEBUG_FONT_COUNTERS
            mDeleteCounter ++;
#endif
            cache->removeBack();
#ifdef DEBUG_FONT_COUNTERS
            logger->log("delete1");
#endif
        }
    }
}

const TextChunkList *SDLFont::getCache() const
{
    return mCache;
}
