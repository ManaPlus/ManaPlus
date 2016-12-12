/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_SKIN_H
#define GUI_SKIN_H

#include "utils/stringmap.h"

#include "localconsts.h"

class Image;
class ImageRect;

class Skin final
{
    public:
        Skin(ImageRect *const restrict skin,
             const ImageRect *const restrict images,
             const std::string &filePath,
             const std::string &name,
             const int padding,
             const int titlePadding,
             StringIntMap *restrict const options) A_NONNULL(8);

        A_DELETE_COPY(Skin)

        ~Skin();

        /**
         * Returns the skin's name. Useful for giving a human friendly skin
         * name if a dialog for skin selection for a specific window type is
         * done.
         */
        const std::string &getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        /**
         * Returns the skin's xml file path.
         */
        const std::string &getFilePath() const noexcept2 A_WARN_UNUSED
        { return mFilePath; }

        /**
         * Returns the background skin.
         */
        const ImageRect &getBorder() const noexcept2 A_WARN_UNUSED
        { return *mBorder; }

        /**
         * Returns the image used by a close button for this skin.
         */
        const Image *getCloseImage(const bool state) const noexcept2 A_WARN_UNUSED
        { return state ? mCloseImageHighlighted : mCloseImage; }

        /**
         * Returns the image used by a sticky button for this skin.
         */
        const Image *getStickyImage(const bool state) const noexcept2 A_WARN_UNUSED
        { return state ? mStickyImageDown : mStickyImageUp; }

        /**
         * Returns the minimum width which can be used with this skin.
         */
        int getMinWidth() const A_WARN_UNUSED;

        /**
         * Returns the minimum height which can be used with this skin.
         */
        int getMinHeight() const A_WARN_UNUSED;

        /**
         * Updates the alpha value of the skin
         */
        void updateAlpha(const float minimumOpacityAllowed = 0.0F);

        int getPadding() const noexcept2 A_WARN_UNUSED
        { return mPadding; }

        int getTitlePadding() const noexcept2 A_WARN_UNUSED
        { return mTitlePadding; }

        int getOption(const std::string &name) const A_WARN_UNUSED
        {
            if (mOptions->find(name) != mOptions->end())
                return (*mOptions)[name];
            else
                return 0;
        }

        int getOption(const std::string &name,
                      const int def) const A_WARN_UNUSED
        {
            if (mOptions->find(name) != mOptions->end())
                return (*mOptions)[name];
            else
                return def;
        }

        int instances;

    private:
        std::string mFilePath;     /**< File name path for the skin */
        std::string mName;         /**< Name of the skin to use */
        ImageRect *mBorder;        /**< The window border and background */
        Image *mCloseImage;        /**< Close Button Image */
        Image *mCloseImageHighlighted; /**< Highlighted close Button Image */
        Image *mStickyImageUp;     /**< Sticky Button Image */
        Image *mStickyImageDown;   /**< Sticky Button Image */
        int mPadding;
        int mTitlePadding;
        StringIntMap *mOptions A_NONNULLPOINTER;
};

#endif  // GUI_SKIN_H
