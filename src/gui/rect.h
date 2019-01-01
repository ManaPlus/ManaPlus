/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_RECT_H
#define GUI_RECT_H

#include "localconsts.h"

/**
  * Represents a rectangle.
  */
class Rect notfinal
{
    public:
        /**
          * Constructor. The default rectangle is an empty rectangle
          * at the coordinates (0,0).
          */
        constexpr Rect() :
            x(0),
            y(0),
            width(0),
            height(0)
        {
        }

        /**
          * Constructor.
          *
          * @param x_ The x coordinate of the rectangle.
          * @param y_ The y coordinate of the rectangle.
          * @param width_ The width of the rectangle.
          * @param height_ The height of the rectangle.
          */
        constexpr Rect(const int x_,
                       const int y_,
                       const int width_,
                       const int height_) :
            x(x_),
            y(y_),
            width(width_),
            height(height_)
        {
        }

        constexpr Rect(const Rect &r) :
            x(r.x),
            y(r.y),
            width(r.width),
            height(r.height)
        {
        }

        A_DEFAULT_COPY(Rect)

        Rect &operator=(const Rect &r)
        {
            x = r.x;
            y = r.y;
            width = r.width;
            height = r.height;
            return *this;
        }

        virtual ~Rect()
        { }

        /**
          * Sets the dimension of a rectangle.
          *
          * @param x0 The x coordinate of the rectangle.
          * @param y0 The y coordinate of the rectangle.
          * @param width0 The width of the rectangle.
          * @param height0 The height of the rectangle.
          */
        void setAll(const int x0,
                    const int y0,
                    const int width0,
                    const int height0)
        {
            x = x0;
            y = y0;
            width = width0;
            height = height0;
        }

        /**
          * Checks if another rectangle intersects with the rectangle.
          *
          * @param rectangle Another rectangle to check for intersection.
          * @return True if the rectangles intersect, false otherwise.
          */
        bool isIntersecting(const Rect& rectangle) const A_WARN_UNUSED
        {
            int x_ = x;
            int y_ = y;
            int width_ = width;
            int height_ = height;

            x_ -= rectangle.x;
            y_ -= rectangle.y;

            if (x_ < 0)
            {
                width_ += x_;
            }
            else if (x_ + width_ > rectangle.width)
            {
                width_ = rectangle.width - x_;
            }

            if (y_ < 0)
            {
                height_ += y_;
            }
            else if (y_ + height_ > rectangle.height)
            {
                height_ = rectangle.height - y_;
            }

            if (width_ <= 0 || height_ <= 0)
            {
                return false;
            }

            return true;
        }

        /**
          * Checks if a point is inside the rectangle
          *
          * @param x_ The x coordinate of the point.
          * @param y_ The y coordinate of the point.
          * @return True if the point is inside the rectangle.
          */
        bool isPointInRect(const int x_,
                           const int y_) const A_WARN_UNUSED
        {
            return x_ >= x
                && y_ >= y
                && x_ < x + width
                && y_ < y + height;
        }

        /**
          * Holds the x coordinate of the rectangle.
          */
        int x;

        /**
          * Holds the x coordinate of the rectangle.
          */
        int y;

        /**
          * Holds the width of the rectangle.
          */
        int width;

        /**
          * Holds the height of the rectangle.
          */
        int height;
};

#endif  // GUI_RECT_H
