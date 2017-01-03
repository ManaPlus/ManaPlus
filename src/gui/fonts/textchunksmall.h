/*
 *  The ManaPlus Client
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

#ifndef GUI_FONTS_TEXTCHUNKSMALL_H
#define GUI_FONTS_TEXTCHUNKSMALL_H

#include "gui/color.h"

#include <string>

#include "localconsts.h"

class TextChunkSmall final
{
    public:
        TextChunkSmall(const std::string &restrict text0,
                       const Color &restrict color0,
                       const Color &restrict color1);

        TextChunkSmall(const TextChunkSmall &restrict old);

        bool operator==(const TextChunkSmall &restrict chunk) const restrict2;
        bool operator<(const TextChunkSmall &restrict chunk) const restrict2;
        TextChunkSmall &operator=(const TextChunkSmall &restrict chunk)
                                  restrict2;
        std::string text;
        Color color;
        Color color2;
};
#endif  // GUI_FONTS_TEXTCHUNKSMALL_H
