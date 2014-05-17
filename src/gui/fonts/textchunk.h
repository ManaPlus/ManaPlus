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

#ifndef GUI_FONTS_TEXTCHUNK_H
#define GUI_FONTS_TEXTCHUNK_H

#include "gui/color.h"

#include <string>

#include <SDL_ttf.h>

#include "localconsts.h"

class Color;
class Image;

class TextChunk final
{
    public:
        TextChunk(const std::string &text0,
                  const Color &color0,
                  const Color &color1);

        A_DELETE_COPY(TextChunk)

        ~TextChunk();

        bool operator==(const TextChunk &chunk) const;

        void generate(TTF_Font *const font, const float alpha);

        Image *img;
        std::string text;
        Color color;
        Color color2;
        TextChunk *prev;
        TextChunk *next;
};

#ifdef UNITTESTS
extern int sdlTextChunkCnt;
#endif

#endif  // GUI_FONTS_TEXTCHUNK_H
