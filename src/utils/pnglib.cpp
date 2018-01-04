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

#include "utils/pnglib.h"

#include "utils/cast.h"
#include "utils/checkutils.h"

#include <png.h>
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_video.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

bool PngLib::writePNG(SDL_Surface *const surface,
                      const std::string &filename)
{
    if (surface == nullptr)
        return false;


    if (SDL_MUSTLOCK(surface))
        SDL_LockSurface(surface);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
        nullptr, nullptr, nullptr);
    if (png_ptr == nullptr)
    {
        reportAlways("Had trouble creating png_structp");
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr)
    {
        png_destroy_write_struct(&png_ptr, static_cast<png_infopp>(nullptr));
        reportAlways("Could not create png_info");
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        reportAlways("problem writing to %s", filename.c_str());
        return false;
    }

    FILE *const fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr)
    {
        reportAlways("could not open file %s for writing",
            filename.c_str());
        return false;
    }

    png_init_io(png_ptr, fp);

    const int colortype = (surface->format->BitsPerPixel == 24) ?
        PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGB_ALPHA;

    png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8, colortype,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_set_packing(png_ptr);

    png_bytep *const row_pointers
        = new png_bytep[CAST_SIZE(surface->h)];

    for (int i = 0; i < surface->h; i++)
    {
        row_pointers[i] = static_cast<png_bytep>(static_cast<uint8_t *>(
            surface->pixels) + CAST_SIZE(i * surface->pitch));
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);

    fclose(fp);

    delete [] row_pointers;

    png_destroy_write_struct(&png_ptr,
        &info_ptr);

    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);

    return true;
}
