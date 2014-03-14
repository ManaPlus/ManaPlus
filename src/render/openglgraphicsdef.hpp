/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

public:
    void setColor(const Color &color) override final
    {
        mColor = color;
        mColor2 = color;
        mColorAlpha = (color.a != 255);
    }

    void setColorAll(const Color &color, const Color &color2)
    {
        mColor = color;
        mColor2 = color2;
        mColorAlpha = (color.a != 255);
    }

    void drawRectangle(const Rect &rect,
                       const bool filled);

    static void dumpSettings();

    void prepareScreenshot() override final;

    int getMemoryUsage() A_WARN_UNUSED;

    void updateTextureFormat();

    bool isAllowScale() const override final
    { return true; }

    static void bindTexture(const GLenum target, const GLuint texture);

    static GLuint mLastImage;

protected:
    void setTexturingAndBlending(const bool enable);

private:
    void inline setColorAlpha(const float alpha);

    void inline restoreColor();
