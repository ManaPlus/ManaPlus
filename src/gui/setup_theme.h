/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#ifndef GUI_Setup_Theme_H
#define GUI_Setup_Theme_H

#include "guichanfwd.h"

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

class DropDown;
class EditDialog;
class FontsModel;
class FontSizeChoiceListModel;
class LangListModel;
class ThemesModel;

class Setup_Theme : public SetupTab
{
    public:
        Setup_Theme();
        ~Setup_Theme();

        void apply();
        void cancel();

        void action(const gcn::ActionEvent &event);

    private:
        gcn::Label *mThemeLabel;
        DropDown *mThemeDropDown;
        std::string mTheme;
        ThemesModel *mThemesModel;
        FontsModel *mFontsModel;

        gcn::Label *mFontLabel;
        DropDown *mFontDropDown;
        std::string mFont;

        LangListModel *mLangListModel;

        gcn::Label *mLangLabel;
        DropDown *mLangDropDown;
        std::string mLang;

        gcn::Label *mBoldFontLabel;
        DropDown *mBoldFontDropDown;
        std::string mBoldFont;

        gcn::Label *mParticleFontLabel;
        DropDown *mParticleFontDropDown;
        std::string mParticleFont;

        gcn::Label *mHelpFontLabel;
        DropDown *mHelpFontDropDown;
        std::string mHelpFont;

        gcn::Label *mSecureFontLabel;
        DropDown *mSecureFontDropDown;
        std::string mSecureFont;

        gcn::Label *mJapanFontLabel;
        DropDown *mJapanFontDropDown;
        std::string mJapanFont;

        FontSizeChoiceListModel *mFontSizeListModel;
        gcn::Label *fontSizeLabel;
        int mFontSize;
        gcn::DropDown *mFontSizeDropDown;

        EditDialog *mEditDialog;
};

#endif
