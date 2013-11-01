/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "localconsts.h"

// #define DEBUG_JOYSTICK 1

#ifdef ENABLE_MEM_DEBUG

// #define _DEBUG_NEW_EMULATE_MALLOC 1
#include "debug/debug_new.h"

#define reportFalse(val) reportFalse1(val, __FILE__, __LINE__)
#define reportFalse1(val, file, line) reportFalseReal(val, file, line)

#define reportTrue(val) reportTrue1(val, __FILE__, __LINE__)
#define reportTrue1(val, file, line) reportTrueReal(val, file, line)

#else  // ENABLE_MEM_DEBUG

#define reportFalse(val) (val)
#define reportTrue(val) (val)

#endif  // ENABLE_MEM_DEBUG


#ifdef DEBUG_SDL_SURFACES

#define MIMG_LoadPNG_RW(src)  FakeIMG_LoadPNG_RW(src, __FILE__, __LINE__)
#define MIMG_LoadJPG_RW(src)  FakeIMG_LoadJPG_RW(src, __FILE__, __LINE__)
#define MSDL_FreeSurface(s) FakeSDL_FreeSurface(s, __FILE__, __LINE__)
#define MSDL_CreateRGBSurface(flags, w, h, d, r, g, b, a) \
    FakeSDL_CreateRGBSurface(flags, w, h, d, r, g, b, a, __FILE__, __LINE__)
#define MSDL_ConvertSurface(src, fmt, flags) \
    FakeSDL_ConvertSurface(src, fmt, flags, __FILE__, __LINE__)
#define MTTF_RenderUTF8_Blended(font, text, fg) \
    FakeTTF_RenderUTF8_Blended(font, text, fg, __FILE__, __LINE__)
#define MIMG_Load(file) FakeIMG_Load(file, __FILE__, __LINE__)
#define MSDL_DisplayFormatAlpha(surface) \
    FakeSDL_DisplayFormatAlpha(surface, __FILE__, __LINE__)
#define MSDL_DisplayFormat(surface) \
    FakeSDL_DisplayFormat(surface, __FILE__, __LINE__)

#else  // ENABLE_SDL_DEBUG

#define MIMG_LoadPNG_RW(src)  IMG_LoadPNG_RW(src)
#define MIMG_LoadJPG_RW(src)  IMG_LoadJPG_RW(src)
#define MSDL_FreeSurface(surface) SDL_FreeSurface(surface)
#define MSDL_CreateRGBSurface(flags, w, h, d, r, g, b, a) \
    SDL_CreateRGBSurface(flags, w, h, d, r, g, b, a)
#define MSDL_ConvertSurface(src, fmt, flags) \
    SDL_ConvertSurface(src, fmt, flags)
#define MTTF_RenderUTF8_Blended(font, text, fg) \
    TTF_RenderUTF8_Blended(font, text, fg)
#define MIMG_Load(file) IMG_Load(file)
#define MSDL_DisplayFormatAlpha(surface) SDL_DisplayFormatAlpha(surface)
#define MSDL_DisplayFormat(surface) SDL_DisplayFormat(surface)

#endif  // ENABLE_SDL_DEBUG


#ifdef DEBUG_PHYSFS

#define MPHYSFSRWOPS_openRead(name) \
    FakePHYSFSRWOPS_openRead(name, __FILE__, __LINE__)

#else  // DEBUG_PHYSFS

#define MPHYSFSRWOPS_openRead(name) PHYSFSRWOPS_openRead(name)

#endif  // DEBUG_PHYSFS


#ifdef ENABLE_CHECKS

#define CHECKLISTENERS \
    config.checkListeners(this, __FILE__, __LINE__); \
    serverConfig.checkListeners(this, __FILE__, __LINE__);

#else  // ENABLE_CHECKS

#define CHECKLISTENERS

#endif  // ENABLE_CHECKS
