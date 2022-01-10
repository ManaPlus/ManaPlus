/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RENDER_SDLGRAPHICS_H
#define RENDER_SDLGRAPHICS_H

#ifdef USE_SDL2
#include "render/sdl2graphics.h"
RENDER_SDL2GRAPHICS_H

#else

#include "render/graphics.h"

#include "localconsts.h"

/**
 * A central point of control for graphics.
 */
class SDLGraphics final : public Graphics
{
    public:
        /**
         * Constructor.
         */
        SDLGraphics();

        A_DELETE_COPY(SDLGraphics)

        /**
         * Destructor.
         */
        ~SDLGraphics() override final;

        #include "render/graphicsdef.hpp"
        RENDER_GRAPHICSDEF_HPP

        #include "render/softwaregraphicsdef.hpp"
        RENDER_SOFTWAREGRAPHICSDEF_HPP

    protected:
        int SDL_FakeUpperBlit(const SDL_Surface *restrict const src,
                              SDL_Rect *restrict const srcrect,
                              const SDL_Surface *restrict const dst,
                              SDL_Rect *restrict dstrect) const restrict2;

        void drawHLine(int x1, int y, int x2) restrict2;

        void drawVLine(int x, int y1, int y2) restrict2;

        uint32_t mOldPixel;
        unsigned int mOldAlpha;
};

#endif  // USE_SDL2
#endif  // RENDER_SDLGRAPHICS_H
