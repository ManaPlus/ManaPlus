/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef UTILS_SDLCHECKUTILS_H
#define UTILS_SDLCHECKUTILS_H

#include "localconsts.h"
#ifdef DEBUG_SDL_SURFACES

#include <string>

struct _TTF_Font;
struct SDL_Color;
struct SDL_PixelFormat;
struct SDL_RWops;
struct SDL_Surface;

SDL_Surface *FakeIMG_LoadPNG_RW(SDL_RWops *const src, const char *const file,
                                const unsigned line);

SDL_Surface *FakeIMG_LoadJPG_RW(SDL_RWops *const src, const char *const file,
                                const unsigned line);

void FakeSDL_FreeSurface(SDL_Surface *const surface, const char *const file,
                         const unsigned line);

SDL_Surface *FakeSDL_CreateRGBSurface(const uint32_t flags,
                                      const int width, const int height,
                                      const int depth,
                                      const uint32_t rMask,
                                      const uint32_t gMask,
                                      const uint32_t bMask,
                                      const uint32_t aMask,
                                      const char *const file,
                                      const unsigned line);

SDL_Surface *FakeSDL_ConvertSurface(SDL_Surface *const src,
                                    SDL_PixelFormat *const fmt,
                                    const uint32_t flags,
                                    const char *const file,
                                    const unsigned line);

SDL_Surface *FakeTTF_RenderUTF8_Blended(_TTF_Font *const font,
                                        const char *const text,
                                        const SDL_Color &fg,
                                        const char *const file,
                                        const unsigned line);

SDL_Surface *FakeIMG_Load(const char *name, const char *const file,
                          const unsigned line);

SDL_Surface *FakeSDL_DisplayFormat(SDL_Surface *const surface,
                                   const char *const file,
                                   const unsigned line);

SDL_Surface *FakeSDL_DisplayFormatAlpha(SDL_Surface *const surface,
                                        const char *const file,
                                        const unsigned line);

#endif  // DEBUG_SDL_SURFACES
#endif  // UTILS_SDLCHECKUTILS_H
