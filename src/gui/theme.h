/*
 *  Gui Skinning
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef SKIN_H
#define SKIN_H

#include "configlistener.h"
#include "graphics.h"

#include "gui/palette.h"

#include <map>

#include "localconsts.h"

class DyePalette;
class Image;
class ImageSet;
class ProgressBar;

const int THEME_PALETTES = 5;

struct ThemeInfo final
{
    std::string name;
    std::string copyright;
};

class Skin final
{
    public:
        Skin(ImageRect *skin, const ImageRect *images,
             const std::string &filePath, const std::string &name = "",
             const int padding = 3, const int titlePadding = 4,
             std::map<std::string, int> *const options = nullptr);

        A_DELETE_COPY(Skin)

        ~Skin();

        /**
         * Returns the skin's name. Useful for giving a human friendly skin
         * name if a dialog for skin selection for a specific window type is
         * done.
         */
        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        /**
         * Returns the skin's xml file path.
         */
        const std::string &getFilePath() const A_WARN_UNUSED
        { return mFilePath; }

        /**
         * Returns the background skin.
         */
        ImageRect &getBorder() const A_WARN_UNUSED
        { return *mBorder; }

        /**
         * Returns the image used by a close button for this skin.
         */
        Image *getCloseImage(const bool state) const A_WARN_UNUSED
        { return state ? mCloseImageHighlighted : mCloseImage; }

        /**
         * Returns the image used by a sticky button for this skin.
         */
        Image *getStickyImage(const bool state) const A_WARN_UNUSED
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
        void updateAlpha(const float minimumOpacityAllowed = 0.0f);

        int getPadding() const A_WARN_UNUSED
        { return mPadding; }

        int getTitlePadding() const A_WARN_UNUSED
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
        std::map<std::string, int> *mOptions;
};

class Theme final : public Palette, public ConfigListener
{
    public:
        A_DELETE_COPY(Theme)

        static Theme *instance() A_WARN_UNUSED;

        static void deleteInstance();

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

        ImageSet *getImageSetFromThemeXml(const std::string &name,
                                          const std::string &name2,
                                          const int w,
                                          const int h) const A_WARN_UNUSED;
        enum ThemePalette
        {
            BROWSERBOX = 0,
            BROWSERBOX_OUTLINE,
            SELFNICK,
            SELFNICK_OUTLINE,
            TEXT,
            CARET,
            SHADOW,
            OUTLINE,
            BORDER,
            PROGRESS_BAR,
            BUTTON,
            BUTTON_DISABLED,
            BUTTON_HIGHLIGHTED,
            BUTTON_PRESSED,
            CHECKBOX,
            DROPDOWN,
            LABEL,
            LISTBOX,
            LISTBOX_SELECTED,
            RADIOBUTTON,
            POPUP,
            POPUP_OUTLINE,
            TAB,
            TAB_OUTLINE,
            TAB_HIGHLIGHTED,
            TAB_HIGHLIGHTED_OUTLINE,
            TAB_SELECTED,
            TAB_SELECTED_OUTLINE,
            TEXTBOX,
            TEXTFIELD,
            WINDOW,
            WINDOW_OUTLINE,
            PARTY_CHAT_TAB,
            PARTY_CHAT_TAB_OUTLINE,
            PARTY_SOCIAL_TAB,
            PARTY_SOCIAL_TAB_OUTLINE,
            GUILD_CHAT_TAB,
            GUILD_CHAT_TAB_OUTLINE,
            GUILD_SOCIAL_TAB,
            GUILD_SOCIAL_TAB_OUTLINE,
            GM_CHAT_TAB,
            GM_CHAT_TAB_OUTLINE,
            PARTY_CHAT_TAB_HIGHLIGHTED,
            PARTY_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            PARTY_SOCIAL_TAB_HIGHLIGHTED,
            PARTY_SOCIAL_TAB_HIGHLIGHTED_OUTLINE,
            GUILD_CHAT_TAB_HIGHLIGHTED,
            GUILD_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            GUILD_SOCIAL_TAB_HIGHLIGHTED,
            GUILD_SOCIAL_TAB_HIGHLIGHTED_OUTLINE,
            GM_CHAT_TAB_HIGHLIGHTED,
            GM_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            PARTY_CHAT_TAB_SELECTED,
            PARTY_CHAT_TAB_SELECTED_OUTLINE,
            PARTY_SOCIAL_TAB_SELECTED,
            PARTY_SOCIAL_TAB_SELECTED_OUTLINE,
            GUILD_CHAT_TAB_SELECTED,
            GUILD_CHAT_TAB_SELECTED_OUTLINE,
            GUILD_SOCIAL_TAB_SELECTED,
            GUILD_SOCIAL_TAB_SELECTED_OUTLINE,
            GM_CHAT_TAB_SELECTED,
            GM_CHAT_TAB_SELECTED_OUTLINE,
            BACKGROUND,
            BACKGROUND_GRAY,
            SCROLLBAR_GRAY,
            DROPDOWN_SHADOW,
            HIGHLIGHT,
            HIGHLIGHT_OUTLINE,
            TAB_FLASH,
            TAB_FLASH_OUTLINE,
            TAB_PLAYER_FLASH,
            TAB_PLAYER_FLASH_OUTLINE,
            SHOP_WARNING,
            ITEM_EQUIPPED,
            ITEM_NOT_EQUIPPED,
            CHAT,
            CHAT_OUTLINE,
            GM,
            GM_OUTLINE,
            PLAYER,
            PLAYER_OUTLINE,
            WHISPER_TAB,
            WHISPER_TAB_OUTLINE,
            WHISPER_TAB_OFFLINE,
            WHISPER_TAB_OFFLINE_OUTLINE,
            WHISPER_TAB_HIGHLIGHTED,
            WHISPER_TAB_HIGHLIGHTED_OUTLINE,
            WHISPER_TAB_OFFLINE_HIGHLIGHTED,
            WHISPER_TAB_OFFLINE_HIGHLIGHTED_OUTLINE,
            WHISPER_TAB_SELECTED,
            WHISPER_TAB_SELECTED_OUTLINE,
            WHISPER_TAB_OFFLINE_SELECTED,
            WHISPER_TAB_OFFLINE_SELECTED_OUTLINE,
            IS,
            IS_OUTLINE,
            SERVER,
            SERVER_OUTLINE,
            LOGGER,
            LOGGER_OUTLINE,
            HYPERLINK,
            HYPERLINK_OUTLINE,
            UNKNOWN_ITEM,
            GENERIC,
            HEAD,
            USABLE,
            TORSO,
            ONEHAND,
            LEGS,
            FEET,
            TWOHAND,
            SHIELD,
            RING,
            NECKLACE,
            ARMS,
            AMMO,
            SERVER_VERSION_NOT_SUPPORTED,
            WARNING,
            CHARM,
            PLAYER_ADVANCED,
            BUBBLE_NAME,
            BUBBLE_TEXT,
            RED,
            RED_OUTLINE,
            GREEN,
            GREEN_OUTLINE,
            BLUE,
            BLUE_OUTLINE,
            ORANGE,
            ORANGE_OUTLINE,
            YELLOW,
            YELLOW_OUTLINE,
            PINK,
            PINK_OUTLINE,
            PURPLE,
            PURPLE_OUTLINE,
            GRAY,
            GRAY_OUTLINE,
            BROWN,
            BROWN_OUTLINE,
            THEME_COLORS_END
        };

        enum ProgressPalette
        {
            PROG_DEFAULT = 0,
            PROG_HP,
            PROG_MP,
            PROG_NO_MP,
            PROG_EXP,
            PROG_INVY_SLOTS,
            PROG_WEIGHT,
            PROG_JOB,
            THEME_PROG_END
        };

        /**
         * Gets the color associated with the type. Sets the alpha channel
         * before returning.
         *
         * @param type the color type requested
         * @param alpha alpha channel to use
         *
         * @return the requested color
         */
        inline static const gcn::Color &getThemeColor(const int type,
                                                      const int alpha = 255)
                                                      A_WARN_UNUSED
        { return mInstance->getColor(type, alpha); }

        static const gcn::Color &getThemeCharColor(const signed char c,
                                                   bool &valid) A_WARN_UNUSED
        { return mInstance->getCharColor(c, valid); }

        static int getThemeIdByChar(const signed char c,
                                    bool &valid) A_WARN_UNUSED
        { return mInstance->getIdByChar(c, valid); }

        static gcn::Color getProgressColor(const int type,
                                           const float progress) A_WARN_UNUSED;

        /**
         * Loads a skin.
         */
        Skin *load(const std::string &filename, const std::string &filename2,
                   const bool full = true, const std::string
                   &defaultPath = getThemePath()) A_WARN_UNUSED;

        Skin *loadSkinRect(ImageRect &image, const std::string &name,
                           const std::string &name2, const int start = 0,
                           const int end = 8) A_WARN_UNUSED;

        void unload(Skin *const skin);

        /**
         * Updates the alpha values of all of the skins.
         */
        void updateAlpha();

        /**
         * Get the minimum opacity allowed to skins.
         */
        float getMinimumOpacity() const A_WARN_UNUSED
        { return mMinimumOpacity; }

        /**
         * Set the minimum opacity allowed to skins.
         * Set a negative value to free the minimum allowed.
         */
        void setMinimumOpacity(const float minimumOpacity);

        void optionChanged(const std::string &) override;

        void loadRect(ImageRect &image, const std::string &name,
                      const std::string &name2, const int start = 0,
                      const int end = 8);

        void unloadRect(const ImageRect &rect, const int start = 0,
                        const int end = 8) const;

        static Image *getImageFromThemeXml(const std::string &name,
                                           const std::string &name2)
                                           A_WARN_UNUSED;

        static ThemeInfo *loadInfo(const std::string &themeName) A_WARN_UNUSED;

    private:
        Theme();

        ~Theme();

        Skin *readSkin(const std::string &filename0,
                       const bool full) A_WARN_UNUSED;

        // Map containing all window skins
        typedef std::map<std::string, Skin*> Skins;
        typedef Skins::iterator SkinIterator;

        Skins mSkins;

        static std::string mThemePath;
        static std::string mThemeName;
        static Theme *mInstance;

        static bool tryThemePath(std::string themePath) A_WARN_UNUSED;

        void loadColors(std::string file = "");

        /**
         * Tells if the current skins opacity
         * should not get less than the given value
         */
        float mMinimumOpacity;

        typedef std::vector<DyePalette*> ProgressColors;
        ProgressColors mProgressColors;
};

#endif
