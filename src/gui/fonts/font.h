/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_FONTS_FONT_H
#define GUI_FONTS_FONT_H

#include "gui/fonts/textchunklist.h"

#include <SDL_ttf.h>

#include "localconsts.h"

class Graphics;

const unsigned int CACHES_NUMBER = 256;

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
                      const int style = 0) restrict2;

        int getWidth(const std::string &text) const restrict2 A_WARN_UNUSED;

        int getHeight() const restrict2 A_WARN_UNUSED;

        const TextChunkList *getCache() const restrict2 noexcept2
                                      A_CONST A_WARN_UNUSED;

        /**
         * @see Font::drawString
         */
        void drawString(Graphics *restrict const graphics,
                        Color col,
                        const Color &restrict col2,
                        const std::string &restrict text,
                        const int x,
                        const int y) restrict2 A_NONNULL(2);

        void clear() restrict2;

        void doClean() restrict2;

        void slowLogic(const int rnd) restrict2;

        unsigned int getCreateCounter() const restrict2 noexcept2 A_WARN_UNUSED
        { return mCreateCounter; }

        unsigned int getDeleteCounter() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDeleteCounter; }

        int getStringIndexAt(const std::string &restrict text,
                             const int x) const restrict2 A_WARN_UNUSED;

        void generate(TextChunk &restrict chunk) restrict2;

        void insertChunk(TextChunk *const chunk) restrict2;

        static bool mSoftMode;

    private:
        static TTF_Font *openFont(const char *restrict const name,
                                  const int size);

        TTF_Font *restrict mFont;
        unsigned int mCreateCounter;
        unsigned int mDeleteCounter;

        // Word surfaces cache
        time_t mCleanTime;
        mutable TextChunkList mCache[CACHES_NUMBER];
};

#ifdef UNITTESTS
extern int textChunkCnt;
#endif

#endif  // GUI_FONTS_FONT_H
