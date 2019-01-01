/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TABS_SETUP_THEME_H
#define GUI_WIDGETS_TABS_SETUP_THEME_H

#include "gui/widgets/tabs/setuptab.h"

class Button;
class DropDown;
class FontsModel;
class FontSizeChoiceListModel;
class Label;
class LangListModel;
class ThemesModel;

class Setup_Theme final : public SetupTab
{
    public:
        explicit Setup_Theme(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Theme)

        ~Setup_Theme() override final;

        void apply() override final;

        void cancel() override final;

        void action(const ActionEvent &event) override final;

        void updateInfo();

    private:
        Label *mThemeLabel;
        ThemesModel *mThemesModel A_NONNULLPOINTER;
        DropDown *mThemeDropDown;
        std::string mTheme;
        ThemeInfo *mInfo;

        FontsModel *mFontsModel A_NONNULLPOINTER;
        Label *mFontLabel A_NONNULLPOINTER;
        DropDown *mFontDropDown A_NONNULLPOINTER;
        std::string mFont;

        LangListModel *mLangListModel A_NONNULLPOINTER;

        Label *mLangLabel A_NONNULLPOINTER;
        DropDown *mLangDropDown A_NONNULLPOINTER;
        std::string mLang;

        Label *mBoldFontLabel A_NONNULLPOINTER;
        DropDown *mBoldFontDropDown A_NONNULLPOINTER;
        std::string mBoldFont;

        Label *mParticleFontLabel A_NONNULLPOINTER;
        DropDown *mParticleFontDropDown A_NONNULLPOINTER;
        std::string mParticleFont;

        Label *mHelpFontLabel A_NONNULLPOINTER;
        DropDown *mHelpFontDropDown A_NONNULLPOINTER;
        std::string mHelpFont;

        Label *mSecureFontLabel A_NONNULLPOINTER;
        DropDown *mSecureFontDropDown A_NONNULLPOINTER;
        std::string mSecureFont;

        Label *mNpcFontLabel A_NONNULLPOINTER;
        DropDown *mNpcFontDropDown A_NONNULLPOINTER;
        std::string mNpcFont;

        Label *mJapanFontLabel A_NONNULLPOINTER;
        DropDown *mJapanFontDropDown A_NONNULLPOINTER;
        std::string mJapanFont;

        Label *mChinaFontLabel A_NONNULLPOINTER;
        DropDown *mChinaFontDropDown A_NONNULLPOINTER;
        std::string mChinaFont;

        FontSizeChoiceListModel *mFontSizeListModel A_NONNULLPOINTER;
        Label *mFontSizeLabel;
        int mFontSize;
        DropDown *mFontSizeDropDown;

        FontSizeChoiceListModel *mNpcFontSizeListModel A_NONNULLPOINTER;
        Label *mNpcFontSizeLabel;
        int mNpcFontSize;
        DropDown *mNpcFontSizeDropDown A_NONNULLPOINTER;

        Button *mInfoButton A_NONNULLPOINTER;
        std::string mThemeInfo;
};

#endif  // GUI_WIDGETS_TABS_SETUP_THEME_H
