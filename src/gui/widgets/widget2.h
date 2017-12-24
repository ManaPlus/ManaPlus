/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_WIDGET2_H
#define GUI_WIDGETS_WIDGET2_H

#include "const/gui/theme.h"

#include "enums/render/rendertype.h"

#include "gui/themecolorsidoperators.h"
#include "gui/theme.h"

#include "localconsts.h"

class Widget;

class Widget2 notfinal
{
    public:
        A_DEFAULT_COPY(Widget2)

        virtual ~Widget2()
        {
        }

        inline const Color &getThemeColor(const ThemeColorIdT type,
                                          const unsigned int alpha)
                                          const A_WARN_UNUSED A_INLINE
        {
            return theme->getColor(type + mPaletteOffset, alpha);
        }

        inline const Color &getThemeCharColor(const signed char c,
                                              bool &valid)
                                              const A_WARN_UNUSED A_INLINE
        {
            if (theme == nullptr)
                return Palette::BLACK;
            const ThemeColorIdT colorId = theme->getIdByChar(c, valid);
            if (valid)
                return theme->getColor(colorId + mPaletteOffset, 255U);
            return Palette::BLACK;
        }

        virtual void setWidget2(const Widget2 *const widget)
        {
            mPaletteOffset = widget != nullptr ? widget->mPaletteOffset : 0;
        }

        void setPalette(int palette)
        {
            mPaletteOffset = palette * CAST_S32(
                ThemeColorId::THEME_COLORS_END);
            checkPalette();
            setWidget2(this);
        }

        void checkPalette()
        {
            if (mPaletteOffset < 0 ||
                mPaletteOffset >= THEME_PALETTES * CAST_S32(
                ThemeColorId::THEME_COLORS_END))
            {
                mPaletteOffset = 0;
            }
        }

        void setForegroundColor2(const Color &color) noexcept2
        {
            mForegroundColor2 = color;
        }

        Widget* getWindow() const noexcept2 A_WARN_UNUSED
        {
            return mWindow;
        }

        virtual void setWindow(Widget *const window)
        {
            mWindow = window;
        }

    protected:
        explicit Widget2(const Widget2 *const widget) :
            mPaletteOffset(widget != nullptr ? widget->mPaletteOffset : 0),
            mWindow(widget != nullptr ? widget->getWindow() : nullptr),
            mForegroundColor2()
        {
            checkPalette();
        }

        int mPaletteOffset;
        Widget *mWindow;
        Color mForegroundColor2;
};

extern RenderType openGLMode;

#endif  // GUI_WIDGETS_WIDGET2_H
