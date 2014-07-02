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
    void drawRectangle(const Rect &rect,
                       const bool filled);

    void drawNet(const int x1, const int y1,
                 const int x2, const int y2,
                 const int width, const int height) override final;

    static void dumpSettings();

    void prepareScreenshot() override final;

    int getMemoryUsage() A_WARN_UNUSED;

    void updateTextureFormat();

    bool isAllowScale() const override final
    { return true; }

    void clearScreen() const override final;

    void deleteArrays() override final;

    static void bindTexture(const GLenum target, const GLuint texture);

    static GLuint mTextureBinded;

protected:
    void setTexturingAndBlending(const bool enable);

private:
    void inline setColorAlpha(const float alpha);

    void inline restoreColor();
