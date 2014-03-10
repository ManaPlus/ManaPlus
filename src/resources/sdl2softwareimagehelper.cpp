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

#ifdef USE_SDL2

#include "resources/sdl2softwareimagehelper.h"

#include "resources/dye.h"
#include "resources/resourcemanager.h"

#include "logger.h"
#include "main.h"

#include "resources/image.h"

#include "utils/sdlcheckutils.h"

#include <SDL_image.h>

#include "debug.h"

bool SDL2SoftwareImageHelper::mEnableAlphaCache = false;
SDL_PixelFormat *SDL2SoftwareImageHelper::mFormat = nullptr;

Image *SDL2SoftwareImageHelper::load(SDL_Surface *const tmpImage) const
{
    return _SDLload(tmpImage);
}

Image *SDL2SoftwareImageHelper::createTextSurface(SDL_Surface *const tmpImage,
                                                  const int width A_UNUSED,
                                                  const int height A_UNUSED,
                                                  const float alpha) const
{
    if (!tmpImage)
        return nullptr;

    Image *const img = _SDLload(tmpImage);
    if (img)
        img->setAlpha(alpha);
    return img;
}

SDL_Surface* SDL2SoftwareImageHelper::SDLDuplicateSurface(SDL_Surface *const
                                                          tmpImage)
{
    if (!tmpImage || !tmpImage->format)
        return nullptr;

    return MSDL_ConvertSurface(tmpImage, tmpImage->format, SDL_SWSURFACE);
}

Image *SDL2SoftwareImageHelper::_SDLload(SDL_Surface *tmpImage) const
{
    if (!tmpImage)
        return nullptr;

    SDL_Surface *image = SDL_ConvertSurface(tmpImage, mFormat, 0);
    return new Image(image, false, nullptr);
}

int SDL2SoftwareImageHelper::combineSurface(SDL_Surface *restrict const src,
                                            SDL_Rect *restrict const srcrect,
                                            SDL_Surface *restrict const dst,
                                            SDL_Rect *restrict const dstrect)
{
    SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(src, srcrect, dst, dstrect);
    return 1;
}

#endif  // USE_SDL2
