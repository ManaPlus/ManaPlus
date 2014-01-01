/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 * Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GCN_IMAGE_HPP
#define GCN_IMAGE_HPP

#include <string>

#include "guichan/platform.hpp"

#include "localconsts.h"

namespace gcn
{
    class Color;

    /**
     * Holds an image. To be able to use this class you must first set an
     * ImageLoader in Image by calling
     * @code Image::setImageLoader(myImageLoader) @endcode
     * The function is static. If this is not done, the constructor taking a
     * filename will throw an exception. The ImageLoader you use must be
     * compatible with the Graphics object you use.
     *
     * EXAMPLE: If you use SDLGraphics you should use SDLImageLoader.
     *          Otherwise your program might crash in a most bizarre way.
     * @see AllegroImageLoader, HGEImageLoader, OpenLayerImageLoader,
     *      OpenGLAllegroImageLoader, OpenGLSDLImageLoader, SDLImageLoader
     * @since 0.1.0
     */
    class GCN_CORE_DECLSPEC Image
    {
    public:
        /**
         * Constructor.
         */
        Image();

        /**
         * Destructor.
         */
        virtual ~Image();

        /**
         * Frees an image.
         *
         * @since 0.5.0
         */
        virtual void free() = 0;

        /**
         * Gets the width of the image.
         *
         * @return The width of the image.
         *
         * @since 0.1.0
         */
        virtual int getWidth() const A_WARN_UNUSED = 0;

        /**
         * Gets the height of the image.
         *
         * @return The height of the image.
         *
         * @since 0.1.0
         */
        virtual int getHeight() const A_WARN_UNUSED = 0;

        /**
         * Gets the color of a pixel at coordinate (x, y) in the image.
         *
         * IMPORTANT: Only guaranteed to work before the image has been
         *            converted to display format.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return The color of the pixel.
         *
         * @since 0.5.0
         */
        virtual Color getPixel(int x, int y) A_WARN_UNUSED = 0;

        /**
         * Puts a pixel with a certain color at coordinate (x, y).
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @param color The color of the pixel to put.
         * @since 0.5.0
         */
        virtual void putPixel(int x, int y, const Color& color) = 0;

        /**
         * Converts the image, if possible, to display format.
         *
         * IMPORTANT: Only guaranteed to work before the image has been
         *            converted to display format.
         * @since 0.5.0
         */
        virtual void convertToDisplayFormat() = 0;
    };
}  // namespace gcn

#endif  // end GCN_IMAGE_HPP
