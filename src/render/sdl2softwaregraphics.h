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

#ifndef RENDER_SDL2SOFTWAREGRAPHICS_H
#define RENDER_SDL2SOFTWAREGRAPHICS_H

#ifdef USE_SDL2

#include "render/graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;
class MapLayer;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SDL2SoftwareGraphics final : public Graphics
{
    public:
        /**
         * Constructor.
         */
        SDL2SoftwareGraphics();

        A_DELETE_COPY(SDL2SoftwareGraphics)

        /**
         * Destructor.
         */
        ~SDL2SoftwareGraphics();

        void setRendererFlags(const uint32_t flags) override final
        { mRendererFlags = flags; }

        #include "render/graphicsdef.hpp"

        #include "render/softwaregraphicsdef.hpp"

        bool resizeScreen(const int width, const int height) override final;

    protected:
        int SDL_FakeUpperBlit(const SDL_Surface *const src,
                              SDL_Rect *const srcrect,
                              const SDL_Surface *const dst,
                              SDL_Rect *dstrect) const;

        void drawHLine(int x1, int y, int x2);

        void drawVLine(int x, int y1, int y2);

        uint32_t mRendererFlags;
        SDL_Surface *mSurface;
        uint32_t mOldPixel;
        unsigned int mOldAlpha;
};

#endif  // USE_SDL2
#endif  // RENDER_SDL2SOFTWAREGRAPHICS_H
