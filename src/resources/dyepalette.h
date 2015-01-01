/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef RESOURCES_DYEPALETTE_H
#define RESOURCES_DYEPALETTE_H

#include "resources/dyecolor.h"

#include <string>
#include <vector>

#include "localconsts.h"

/**
 * Class for performing a linear interpolation between colors.
 */
class DyePalette final
{
    public:
        /**
         * Creates a palette based on the given string.
         * The string is either a file name or a sequence of hexadecimal RGB
         * values separated by ',' and starting with '#'.
         */
        DyePalette(const std::string &pallete, const uint8_t blockSize);

        A_DELETE_COPY(DyePalette)

        /**
         * Gets a pixel color depending on its intensity. First color is
         * implicitly black (0, 0, 0).
         */
        void getColor(const unsigned int intensity,
                      unsigned int color[3]) const;

        /**
         * Gets a pixel color depending on its intensity.
         */
        void getColor(double intensity, int color[3]) const;

        /**
         * replace colors for SDL for S dye.
         */
        void replaceSColor(uint32_t *restrict pixels, const int bufSize) const;

        /**
         * replace colors for SDL for S dye.
         */
        void replaceAColor(uint32_t *restrict pixels, const int bufSize) const;

        /**
         * replace colors for OpenGL for S dye.
         */
        void replaceSOGLColor(uint32_t *restrict pixels,
                              const int bufSize) const;

        /**
         * replace colors for OpenGL for A dye.
         */
        void replaceAOGLColor(uint32_t *restrict pixels,
                              const int bufSize) const;

        static unsigned int hexDecode(const signed char c) A_WARN_UNUSED;

    private:
        std::vector<DyeColor> mColors;
};

#endif  // RESOURCES_DYEPALETTE_H
