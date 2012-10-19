/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef GUI_WIDGET2_H
#define GUI_WIDGET2_H

#include "gui/theme.h"

class Widget2
{
    public:
        Widget2() :
            mPalette(1)
        {
        }

        Widget2(const Widget2 *const widget) :
            mPalette(widget ? widget->mPalette : 1)
        {
        }

        virtual ~Widget2()
        {
        }

        inline const gcn::Color &getThemeColor(const int type,
                                               const int alpha = 255) const
        {
            return Theme::getThemeColor(type * mPalette, alpha);
        }

        inline const gcn::Color &getThemeCharColor(const signed char c,
                                                   bool &valid) const
        {
            const int colorId = Theme::getIdByChar(c, valid);
            if (valid)
                return Theme::getThemeColor(colorId * mPalette);
            else
                return Palette::BLACK;
        }

        virtual void setWidget2(const Widget2 *const widget)
        {
            mPalette = widget ? widget->mPalette : 1;
        }

    private:
        int mPalette;
};

#endif
