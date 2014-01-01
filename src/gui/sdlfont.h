/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_SDLFONT_H
#define GUI_SDLFONT_H

#include <guichan/color.hpp>
#include <guichan/font.hpp>

#include <SDL_ttf.h>

#include <map>
#include <string>

#include "localconsts.h"

class Image;

const unsigned int CACHES_NUMBER = 256;

class SDLTextChunkSmall
{
    public:
        SDLTextChunkSmall(const std::string &text0, const gcn::Color &color0,
                          const gcn::Color &color1);

        SDLTextChunkSmall(const SDLTextChunkSmall &old);

        bool operator==(const SDLTextChunkSmall &chunk) const;
        bool operator<(const SDLTextChunkSmall &chunk) const;

        std::string text;
        gcn::Color color;
        gcn::Color color2;
};

class SDLTextChunk final
{
    public:
        SDLTextChunk(const std::string &text0, const gcn::Color &color0,
                     const gcn::Color &color1);

        A_DELETE_COPY(SDLTextChunk)

        ~SDLTextChunk();

        bool operator==(const SDLTextChunk &chunk) const;

        void generate(TTF_Font *const font, const float alpha);

        Image *img;
        std::string text;
        gcn::Color color;
        gcn::Color color2;
        SDLTextChunk *prev;
        SDLTextChunk *next;
};


class TextChunkList final
{
    public:
        TextChunkList();

        A_DELETE_COPY(TextChunkList)

        void insertFirst(SDLTextChunk *const item);

        void moveToFirst(SDLTextChunk *item);

        void removeBack();

        void removeBack(int n);

        void clear();

        SDLTextChunk *start;
        SDLTextChunk *end;
        uint32_t size;
        std::map<SDLTextChunkSmall, SDLTextChunk*> search;
        std::map<std::string, SDLTextChunk*> searchWidth;
};

/**
 * A wrapper around SDL_ttf for allowing the use of TrueType fonts.
 *
 * <b>NOTE:</b> This class initializes SDL_ttf as necessary.
 */
class SDLFont final : public gcn::Font
{
    public:
        SDLFont(std::string filename,
                const int size,
                const int style = 0);

        A_DELETE_COPY(SDLFont)

        ~SDLFont();

        void loadFont(std::string filename,
                      const int size,
                      const int style = 0);

        int getWidth(const std::string &text) const override
                     final A_WARN_UNUSED;

        int getHeight() const override final A_WARN_UNUSED;

        const TextChunkList *getCache() const A_WARN_UNUSED;

        /**
         * @see Font::drawString
         */
        void drawString(gcn::Graphics *const graphics,
                        const std::string &text,
                        const int x, const int y) override final;

        void clear();

        void doClean();

        void slowLogic(const int rnd);

        int getCreateCounter() const A_WARN_UNUSED
        { return mCreateCounter; }

        int getDeleteCounter() const A_WARN_UNUSED
        { return mDeleteCounter; }

        static bool mSoftMode;

    private:
        static TTF_Font *openFont(const char *const name, const int size);

        TTF_Font *mFont;
        unsigned mCreateCounter;
        unsigned mDeleteCounter;

        // Word surfaces cache
        int mCleanTime;
        mutable TextChunkList mCache[CACHES_NUMBER];
};

#ifdef UNITTESTS
extern int sdlTextChunkCnt;
#endif

#endif  // GUI_SDLFONT_H
