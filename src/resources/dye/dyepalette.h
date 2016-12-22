/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef RESOURCES_DYE_DYEPALETTE_H
#define RESOURCES_DYE_DYEPALETTE_H

#include "resources/dye/dyecolor.h"

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
        DyePalette(const std::string &restrict pallete,
                   const uint8_t blockSize);

        A_DELETE_COPY(DyePalette)

        /**
         * Gets a pixel color depending on its intensity. First color is
         * implicitly black (0, 0, 0).
         */
        void getColor(const unsigned int intensity,
                      unsigned int (&restrict color)[3]) const restrict2;

        /**
         * Gets a pixel color depending on its intensity.
         */
        void getColor(double intensity,
                      int (&restrict color)[3]) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        void replaceSColor(uint32_t *restrict pixels,
                           const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        void replaceSColorDefault(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceSColorSimd(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceSColorSse2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceSColorAvx2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

#ifdef SIMD_SUPPORTED
        /**
         * replace colors for SDL for S dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceSColorSimd(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceSColorSimd(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceSColorSse2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceSColorAvx2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;
#endif  // SIMD_SUPPORTED

        /**
         * replace colors for SDL for A dye.
         */
        void replaceAColor(uint32_t *restrict pixels,
                           const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        void replaceAColorDefault(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceAColorSimd(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceAColorSse2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceAColorAvx2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

#ifdef SIMD_SUPPORTED
        /**
         * replace colors for SDL for A dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceAColorSimd(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceAColorSimd(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;
        /**
         * replace colors for SDL for A dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceAColorSse2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceAColorAvx2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;
#endif  // SIMD_SUPPORTED

#ifdef USE_OPENGL
        /**
         * replace colors for OpenGL for S dye.
         */
        void replaceSOGLColor(uint32_t *restrict pixels,
                              const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for S dye.
         */
        void replaceSOGLColorDefault(uint32_t *restrict pixels,
                                     const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for S dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceSOGLColorSimd(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

#ifdef SIMD_SUPPORTED
        /**
         * replace colors for OpenGL for S dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceSOGLColorSimd(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
#endif  // SIMD_SUPPORTED

        /**
         * replace colors for OpenGL for A dye.
         */
        void replaceAOGLColor(uint32_t *restrict pixels,
                              const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for A dye.
         */
        void replaceAOGLColorDefault(uint32_t *restrict pixels,
                                     const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for A dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceAOGLColorSimd(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for A dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceAOGLColorSse2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for A dye.
         */
        FUNCTION_SIMD_DEFAULT
        void replaceAOGLColorAvx2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

#ifdef SIMD_SUPPORTED
        /**
         * replace colors for OpenGL for A dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceAOGLColorSimd(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
        /**
         * replace colors for OpenGL for A dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceAOGLColorSimd(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
        /**
         * replace colors for OpenGL for A dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceAOGLColorSse2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
        /**
         * replace colors for OpenGL for A dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceAOGLColorAvx2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
#endif  // SIMD_SUPPORTED
#endif  // USE_OPENGL

        static unsigned int hexDecode(const signed char c)
                                      noexcept2 A_CONST A_WARN_UNUSED;

        static void hexToColor(const std::string &restrict hexStr,
                               const uint8_t blockSize,
                               DyeColor &color) noexcept2;

#ifndef UNITTESTS
    private:
#endif  // UNITTESTS
        std::vector<DyeColor> mColors;
};

#endif  // RESOURCES_DYE_DYEPALETTE_H
