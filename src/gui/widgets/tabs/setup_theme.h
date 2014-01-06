/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

        ~Setup_Theme();

        void apply() override final;

        void cancel() override final;

        void action(const gcn::ActionEvent &event) override final;

        void updateInfo();

    private:
        Label *mThemeLabel;
        ThemesModel *mThemesModel;
        DropDown *mThemeDropDown;
        std::string mTheme;
        ThemeInfo *mInfo;

        FontsModel *mFontsModel;
        Label *mFontLabel;
        DropDown *mFontDropDown;
        std::string mFont;

        LangListModel *mLangListModel;

        Label *mLangLabel;
        DropDown *mLangDropDown;
        std::string mLang;

        Label *mBoldFontLabel;
        DropDown *mBoldFontDropDown;
        std::string mBoldFont;

        Label *mParticleFontLabel;
        DropDown *mParticleFontDropDown;
        std::string mParticleFont;

        Label *mHelpFontLabel;
        DropDown *mHelpFontDropDown;
        std::string mHelpFont;

        Label *mSecureFontLabel;
        DropDown *mSecureFontDropDown;
        std::string mSecureFont;

        Label *mNpcFontLabel;
        DropDown *mNpcFontDropDown;
        std::string mNpcFont;

        Label *mJapanFontLabel;
        DropDown *mJapanFontDropDown;
        std::string mJapanFont;

        Label *mChinaFontLabel;
        DropDown *mChinaFontDropDown;
        std::string mChinaFont;

        FontSizeChoiceListModel *mFontSizeListModel;
        Label *mFontSizeLabel;
        int mFontSize;
        DropDown *mFontSizeDropDown;

        FontSizeChoiceListModel *mNpcFontSizeListModel;
        Label *mNpcFontSizeLabel;
        int mNpcFontSize;
        DropDown *mNpcFontSizeDropDown;

        Button *mInfoButton;
        std::string mThemeInfo;
};

#endif  // GUI_WIDGETS_TABS_SETUP_THEME_H
