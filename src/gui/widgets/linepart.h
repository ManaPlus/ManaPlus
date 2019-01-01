/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_WIDGETS_LINEPART_H
#define GUI_WIDGETS_LINEPART_H

#include "gui/color.h"

#include <string>

#include "localconsts.h"

class Image;

class LinePart final
{
    public:
        LinePart(const int x, const int y, const Color &color,
                 const Color &color2, const std::string &text,
                 const bool bold) :
            mX(x),
            mY(y),
            mColor(color),
            mColor2(color2),
            mText(text),
            mType(0),
            mImage(nullptr),
            mBold(bold)
        {
        }

        LinePart(const int x, const int y, const Color &color,
                 const Color &color2, Image *const image) :
            mX(x),
            mY(y),
            mColor(color),
            mColor2(color2),
            mText(),
            mType(1),
            mImage(image),
            mBold(false)
        {
        }

        LinePart(const LinePart &l) :
            mX(l.mX),
            mY(l.mY),
            mColor(l.mColor),
            mColor2(l.mColor2),
            mText(l.mText),
            mType(l.mType),
            mImage(l.mImage),
            mBold(l.mBold)
        {
        }

        LinePart &operator=(const LinePart &l)
        {
            mX = l.mX;
            mY = l.mY;
            mColor = l.mColor;
            mColor2 = l.mColor2;
            mText = l.mText;
            mType = l.mType;
            mImage = l.mImage;
            mBold = l.mBold;
            return *this;
        }

        A_DEFAULT_COPY(LinePart)

        ~LinePart();

        int mX;
        int mY;
        Color mColor;
        Color mColor2;
        std::string mText;
        unsigned char mType;
        Image *mImage;
        bool mBold;
};

#endif  // GUI_WIDGETS_LINEPART_H
