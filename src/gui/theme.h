/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_THEME_H
#define GUI_THEME_H

#include "logger.h"

#include "enums/gui/progresscolorid.h"
#include "enums/gui/themecolorid.h"

#include "listeners/configlistener.h"

#include "gui/palette.h"

#include "utils/stringvector.h"

#include "localconsts.h"

class DyePalette;
class Image;
class ImageRect;
class ImageSet;
class Skin;
class Theme;

struct ThemeInfo;

extern Theme *theme;

class Theme final : public Palette,
                    public ConfigListener
{
    public:
        Theme();

        ~Theme() override final;

        A_DELETE_COPY(Theme)

        static void prepareThemePath();

        static void selectSkin();

        static std::string getThemePath() A_WARN_UNUSED
        { return mThemePath; }

        static std::string getThemeName() A_WARN_UNUSED
        { return mThemeName; }

        static void fillSkinsList(StringVect &list);

        static void fillFontsList(StringVect &list);

        static void fillSoundsList(StringVect &list);

        /**
         * Returns the patch to the given gui resource relative to the theme
         * or, if it isn't in the theme, relative to 'graphics/gui'.
         */
        static std::string resolveThemePath(const std::string &path)
                                            A_WARN_UNUSED;

        static Image *getImageFromTheme(const std::string &path) A_WARN_UNUSED;

        static ImageSet *getImageSetFromTheme(const std::string &path,
                                              const int w,
                                              const int h) A_WARN_UNUSED;

        static ImageSet *getImageSetFromThemeXml(const std::string &name,
                                          const std::string &name2,
                                          const int w,
                                          const int h)A_WARN_UNUSED;

        static Color getProgressColor(const ProgressColorIdT type,
                                      const float progress) A_WARN_UNUSED;

        /**
         * Loads a skin.
         */
        Skin *load(const std::string &filename,
                   const std::string &filename2,
                   const bool full,
                   const std::string &restrict defaultPath) A_WARN_UNUSED;

        Skin *loadSkinRect(ImageRect &image,
                           const std::string &name,
                           const std::string &name2,
                           const int start,
                           const int end) A_WARN_UNUSED;

        void unload(Skin *const skin);

        /**
         * Updates the alpha values of all of the skins.
         */
        void updateAlpha();

        /**
         * Get the minimum opacity allowed to skins.
         */
        float getMinimumOpacity() const noexcept2 A_WARN_UNUSED
        { return mMinimumOpacity; }

        /**
         * Gets the color associated with the type. Sets the alpha channel
         * before returning.
         *
         * @param type the color type requested
         * @param alpha alpha channel to use
         *
         * @return the requested color
         */
        inline const Color &getColor(const ThemeColorIdT type,
                                     const unsigned int alpha) A_WARN_UNUSED
        {
            if (CAST_SIZE(type) >= mColors.size())
            {
                logger->log("incorrect color request type: %d from %u",
                    CAST_S32(type),
                    CAST_U32(mColors.size()));
                Color *const col = &mColors[CAST_SIZE(
                    ThemeColorId::BROWSERBOX)].color;
                col->a = alpha;
                return *col;
            }
            else
            {
                Color *const col = &mColors[CAST_SIZE(type)].color;
                col->a = alpha;
                return *col;
            }
        }

        ThemeColorIdT getIdByChar(const signed char c,
                                  bool &valid) const A_WARN_UNUSED;

        /**
         * Set the minimum opacity allowed to skins.
         * Set a negative value to free the minimum allowed.
         */
        void setMinimumOpacity(const float minimumOpacity);

        void optionChanged(const std::string &name) override final;

        void loadRect(ImageRect &image,
                      const std::string &name,
                      const std::string &name2,
                      const int start,
                      const int end);

        static void unloadRect(const ImageRect &rect,
                               const int start,
                               const int end);

        static Image *getImageFromThemeXml(const std::string &name,
                                           const std::string &name2)
                                           A_WARN_UNUSED;

        static ThemeInfo *loadInfo(const std::string &themeName) A_WARN_UNUSED;

    private:
        Skin *readSkin(const std::string &filename0,
                       const bool full) A_WARN_UNUSED;

        // Map containing all window skins
        typedef std::map<std::string, Skin*> Skins;
        typedef Skins::iterator SkinIterator;

        Skins mSkins;

        static std::string mThemePath;
        static std::string mThemeName;
        static std::string mScreenDensity;

        static bool tryThemePath(const std::string &themePath) A_WARN_UNUSED;

        void loadColors(std::string file);

        /**
         * Tells if the current skins opacity
         * should not get less than the given value
         */
        float mMinimumOpacity;

        typedef STD_VECTOR<DyePalette*> ProgressColors;
        ProgressColors mProgressColors;
};

#endif  // GUI_THEME_H
