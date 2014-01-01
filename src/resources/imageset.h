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

#ifndef RESOURCES_IMAGESET_H
#define RESOURCES_IMAGESET_H

#include "resources/resource.h"

#include <vector>

#include "localconsts.h"

class Image;

/**
 * Stores a set of subimages originating from a single image.
 */
class ImageSet : public Resource
{
    public:
        /**
         * Cuts the passed image in a grid of sub images.
         */
        ImageSet(Image *const img, const int w, const int h,
                 const int margin = 0, const int spacing = 0);

        A_DELETE_COPY(ImageSet)

        /**
         * Destructor.
         */
        virtual ~ImageSet();

        /**
         * Returns the width of the images in the image set.
         */
        int getWidth() const A_WARN_UNUSED
        { return mWidth; }

        /**
         * Returns the height of the images in the image set.
         */
        int getHeight() const A_WARN_UNUSED
        { return mHeight; }

        typedef std::vector<Image*>::size_type size_type;

        Image* get(const size_type i) const A_WARN_UNUSED;

        size_type size() const A_WARN_UNUSED
        { return mImages.size(); }

        int getOffsetX() const A_WARN_UNUSED
        { return mOffsetX; }

        void setOffsetX(const int n)
        { mOffsetX = n; }

        int getOffsetY() const A_WARN_UNUSED
        { return mOffsetY; }

        void setOffsetY(const int n)
        { mOffsetY = n; }

        const std::vector<Image*> &getImages() const
        { return mImages; }

    private:
        std::vector<Image*> mImages;

        int mWidth;  /**< Width of the images in the image set. */
        int mHeight; /**< Height of the images in the image set. */
        int mOffsetX;
        int mOffsetY;
};

#endif  // RESOURCES_IMAGESET_H
