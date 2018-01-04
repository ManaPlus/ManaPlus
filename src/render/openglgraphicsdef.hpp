/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#define RENDER_OPENGLGRAPHICSDEF_HPP

public:
    void drawRectangle(const Rect &restrict rect,
                       const bool filled) restrict2;

    void drawNet(const int x1, const int y1,
                 const int x2, const int y2,
                 const int width, const int height) restrict2 override final;

    static void dumpSettings();

    int getMemoryUsage() restrict2 A_WARN_UNUSED;

    void updateTextureFormat() restrict2;

    bool isAllowScale() const restrict2 noexcept2 override final
    { return true; }

    void clearScreen() const restrict2 override final;

    void deleteArrays() restrict2 override final;

    static void bindTexture(const GLenum target, const GLuint texture);

    static GLuint mTextureBinded;

protected:
    void enableTexturingAndBlending() restrict2;

    void disableTexturingAndBlending() restrict2;

private:
    void inline setColorAlpha(const float alpha) restrict2 A_INLINE;

    void inline restoreColor() restrict2 A_INLINE;
