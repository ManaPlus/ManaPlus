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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GUI_FONT_H
#define GUI_FONT_H

#include "gui/color.h"

#include <SDL_ttf.h>

#include <map>
#include <string>

#include "localconsts.h"

class Color;
class Graphics;
class Image;

const unsigned int CACHES_NUMBER = 256;

class SDLTextChunkSmall
{
    public:
        SDLTextChunkSmall(const std::string &text0, const Color &color0,
                          const Color &color1);

        SDLTextChunkSmall(const SDLTextChunkSmall &old);

        bool operator==(const SDLTextChunkSmall &chunk) const;
        bool operator<(const SDLTextChunkSmall &chunk) const;

        std::string text;
        Color color;
        Color color2;
};

class SDLTextChunk final
{
    public:
        SDLTextChunk(const std::string &text0, const Color &color0,
                     const Color &color1);

        A_DELETE_COPY(SDLTextChunk)

        ~SDLTextChunk();

        bool operator==(const SDLTextChunk &chunk) const;

        void generate(TTF_Font *const font, const float alpha);

        Image *img;
        std::string text;
        Color color;
        Color color2;
        SDLTextChunk *prev;
        SDLTextChunk *next;
};


class TextChunkList final
{
    public:
        TextChunkList();

        A_DELETE_COPY(TextChunkList)

        void insertFirst(SDLTextChunk *const item);

        void moveToFirst(SDLTextChunk *const item);

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
class Font final
{
    public:
        Font(std::string filename,
             const int size,
             const int style = 0);

        A_DELETE_COPY(Font)

        ~Font();

        void loadFont(std::string filename,
                      const int size,
                      const int style = 0);

        int getWidth(const std::string &text) const A_WARN_UNUSED;

        int getHeight() const A_WARN_UNUSED;

        const TextChunkList *getCache() const A_WARN_UNUSED;

        /**
         * @see Font::drawString
         */
        void drawString(Graphics *const graphics,
                        const std::string &text,
                        const int x, const int y);

        void clear();

        void doClean();

        void slowLogic(const int rnd);

        int getCreateCounter() const A_WARN_UNUSED
        { return mCreateCounter; }

        int getDeleteCounter() const A_WARN_UNUSED
        { return mDeleteCounter; }

        int getStringIndexAt(const std::string& text,
                             const int x) const A_WARN_UNUSED;

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

#endif  // GUI_FONT_H
