/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "resources/imageloader.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include <guichan/color.hpp>

#include <guichan/sdl/sdlpixel.hpp>

#include "debug.h"

#ifdef free
#undef free
#endif

ProxyImage::ProxyImage(SDL_Surface *s):
    mImage(nullptr),
    mSDLImage(s)
{
}

ProxyImage::~ProxyImage()
{
    free();
}

void ProxyImage::free()
{
    if (mSDLImage)
    {
        SDL_FreeSurface(mSDLImage);
        mSDLImage = nullptr;
    }
    else
    {
        delete mImage;
        mImage = nullptr;
    }
}

int ProxyImage::getWidth() const
{
    if (mSDLImage)
        return mSDLImage->w;
    else if (mImage)
        return mImage->mBounds.w;
    else
        return 0;
}

int ProxyImage::getHeight() const
{
    if (mSDLImage)
        return mSDLImage->h;
    else if (mImage)
        return mImage->mBounds.h;
    else
        return 0;
}

gcn::Color ProxyImage::getPixel(int x, int y)
{
    return gcn::SDLgetPixel(mSDLImage, x, y);
}

void ProxyImage::putPixel(int x, int y, gcn::Color const &color)
{
    if (!mSDLImage)
        return;
    gcn::SDLputPixel(mSDLImage, x, y, color);
}

void ProxyImage::convertToDisplayFormat()
{
    if (!mSDLImage)
        return;

    /* The picture is most probably full of the pink pixels Guichan uses for
       transparency, as this function will only be called when setting an image
       font. Get rid of them. */
    SDL_SetColorKey(mSDLImage, SDL_SRCCOLORKEY,
                    SDL_MapRGB(mSDLImage->format, 255, 0, 255));
    mImage = ::Image::load(mSDLImage);
    SDL_FreeSurface(mSDLImage);
    mSDLImage = nullptr;
}

gcn::Image *ImageLoader::load(const std::string &filename, bool convert)
{
    ResourceManager *resman = ResourceManager::getInstance();
    ProxyImage *i = new ProxyImage(resman->loadSDLSurface(filename));
    if (convert)
        i->convertToDisplayFormat();
    return i;
}
