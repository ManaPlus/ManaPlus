/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GCN_FONT_HPP
#define GCN_FONT_HPP

#include <string>

#include "localconsts.h"

namespace gcn
{
    class Graphics;

    /**
     * Interface for a font.
     *
     * @see ImageFont
     */
    class Font
    {
    public:
        /**
         * Destructor.
         */
        virtual ~Font()
        { }

        /**
         * Gets the width of a string. The width of a string is not necesserily
         * the sum of all the widths of it's glyphs.
         *
         * @param text The string to return the width of.
         * @return The width of a string.
         */
        virtual int getWidth(const std::string& text) const A_WARN_UNUSED = 0;

        /**
         * Gets the height of the glyphs in the font.
         *
         * @return The height of the glyphs int the font.
         */
        virtual int getHeight() const A_WARN_UNUSED = 0;

        /**
         * Gets a string index in a string providing an x coordinate.
         * Used to retrive a string index (for a character in a
         * string) at a certain x position. It is especially useful
         * when a mouse clicks in a TextField and you want to know which
         * character was clicked.
         *
         * @return A string index in a string providing an x coordinate.
         */
        virtual int getStringIndexAt(const std::string& text,
                                     const int x) const A_WARN_UNUSED;

        /**
         * Draws a string.
         *
         * NOTE: You normally won't use this function to draw text since
         *       Graphics contains better functions for drawing text.
         *
         * @param graphics A Graphics object to use for drawing.
         * @param text The string to draw.
         * @param x The x coordinate where to draw the string.
         * @param y The y coordinate where to draw the string.
         */
        virtual void drawString(Graphics* graphics, const std::string& text,
                                int x, int y) = 0;
    };
}  // namespace gcn

#endif  // end GCN_FONT_HPP
