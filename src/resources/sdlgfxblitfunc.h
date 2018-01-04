/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2018  The ManaPlus Developers
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

/*

SDL_gfxBlitFunc: custom blitters (part of SDL_gfx library)

LGPL (c) A. Schiffler

*/

#ifndef RESOURCE_SDLGFXBLITFUNC_H
#define RESOURCE_SDLGFXBLITFUNC_H

struct SDL_Surface;
struct SDL_Rect;

// src surface can be any format (most time 32 bit surface with any masks)
// dst surface always correct 32 sufraces (shared format for all)
int SDLgfxBlitRGBA(SDL_Surface *const src,
                   const SDL_Rect *const srcrect,
                   SDL_Surface *const dst,
                   const SDL_Rect *const dstrect);

#endif  // RESOURCE_SDLGFXBLITFUNC_H
