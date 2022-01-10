/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/fonts/textchunksmall.h"

#include "gui/fonts/font.h"

#include "debug.h"

TextChunkSmall::TextChunkSmall(const std::string &text0,
                               const Color &color0,
                               const Color &color1) :
    text(text0),
    color(color0),
    color2(color1)
{
}

TextChunkSmall::TextChunkSmall(const TextChunkSmall &old) :
    text(old.text),
    color(old.color),
    color2(old.color2)
{
}

TextChunkSmall &TextChunkSmall::operator=(const TextChunkSmall &chunk)
{
    text = chunk.text;
    color = chunk.color;
    color2 = chunk.color2;
    return *this;
}

bool TextChunkSmall::operator==(const TextChunkSmall &chunk) const
{
    return chunk.text == text &&
        chunk.color == color &&
        chunk.color2 == color2;
}

bool TextChunkSmall::operator<(const TextChunkSmall &chunk) const
{
    if (chunk.text != text)
        return chunk.text > text;

    const Color &restrict c = chunk.color;
    if (c.r != color.r)
        return c.r > color.r;
    if (c.g != color.g)
        return c.g > color.g;
    if (c.b != color.b)
        return c.b > color.b;

    const Color &restrict c2 = chunk.color2;
    if (c2.r != color2.r)
        return c2.r > color2.r;
    if (c2.g != color2.g)
        return c2.g > color2.g;
    if (c2.b != color2.b)
        return c2.b > color2.b;

    if (c.a != color.a && Font::mSoftMode)
        return c.a > color.a;

    return false;
}
