/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/setup_theme.h"

#include "gui/gui.h"
#include "gui/editdialog.h"
#include "gui/okdialog.h"
#include "gui/theme.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/dropdown.h"

#include "configuration.h"
#include "localplayer.h"
#include "log.h"

#include "utils/gettext.h"

#include "resources/resourcemanager.h"

#include "debug.h"

const char* ACTION_THEME = "theme";
const char* ACTION_FONT = "font";
const char* ACTION_BOLD_FONT = "bold font";
const char* ACTION_PARTICLE_FONT = "particle font";
const char* ACTION_HELP_FONT = "help font";

class NamesModel : public gcn::ListModel
{
public:
    NamesModel()
    {
    }

    virtual ~NamesModel() { }

    virtual int getNumberOfElements()
    {
        return static_cast<int>(mNames.size());
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return mNames[i];
    }

protected:
    std::vector<std::string> mNames;
};

class ThemesModel : public NamesModel
{
public:
    ThemesModel()
    {
        mNames.push_back("(default)");
        Theme::fillSkinsList(mNames);
    }

    virtual ~ThemesModel()
    { }
};

class FontsModel : public NamesModel
{
public:
    FontsModel()
    { Theme::fillFontsList(mNames); }

    virtual ~FontsModel()
    { }
};

Setup_Theme::Setup_Theme():
    mTheme(config.getValue("theme", config.getValue("selectedSkin", ""))),
    mFont(config.getStringValue("font")),
    mBoldFont(config.getStringValue("boldFont")),
    mParticleFont(config.getStringValue("particleFont")),
    mHelpFont(config.getStringValue("helpFont"))
{
    setName(_("Theme"));

    mThemeLabel = new Label(_("Gui theme"));
    mFontLabel = new Label(_("Main Font"));
    mBoldFontLabel = new Label(_("Bold font"));
    mParticleFontLabel = new Label(_("Particle font"));
    mHelpFontLabel = new Label(_("Help font"));
    mThemesModel  = new ThemesModel();
    mFontsModel  = new FontsModel();

    mThemeDropDown = new DropDown(mThemesModel);
    mThemeDropDown->setActionEventId(ACTION_THEME);
    mThemeDropDown->addActionListener(this);

    mFontDropDown = new DropDown(mFontsModel);
    mFontDropDown->setActionEventId(ACTION_FONT);
    mFontDropDown->addActionListener(this);

    mBoldFontDropDown = new DropDown(mFontsModel);
    mBoldFontDropDown->setActionEventId(ACTION_BOLD_FONT);
    mBoldFontDropDown->addActionListener(this);

    mParticleFontDropDown = new DropDown(mFontsModel);
    mParticleFontDropDown->setActionEventId(ACTION_PARTICLE_FONT);
    mParticleFontDropDown->addActionListener(this);

    mHelpFontDropDown = new DropDown(mFontsModel);
    mHelpFontDropDown->setActionEventId(ACTION_HELP_FONT);
    mHelpFontDropDown->addActionListener(this);

    std::string skin = Theme::getThemeName();
    if (!skin.empty())
        mThemeDropDown->setSelectedString(skin);
    else
        mThemeDropDown->setSelected(0);

    mFontDropDown->setSelectedString(getFileName(
        config.getStringValue("font")));
    mBoldFontDropDown->setSelectedString(getFileName(
        config.getStringValue("boldFont")));
    mParticleFontDropDown->setSelectedString(getFileName(
        config.getStringValue("particleFont")));
    mHelpFontDropDown->setSelectedString(getFileName(
        config.getStringValue("helpFont")));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mThemeLabel, 10);
    place(0, 1, mThemeDropDown, 6);
    place(0, 2, mFontLabel, 10);
    place(0, 3, mFontDropDown, 6);
    place(0, 4, mBoldFontLabel, 10);
    place(0, 5, mBoldFontDropDown, 6);
    place(0, 6, mParticleFontLabel, 10);
    place(0, 7, mParticleFontDropDown, 6);
    place(0, 8, mHelpFontLabel, 10);
    place(0, 9, mHelpFontDropDown, 6);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);

    setDimension(gcn::Rectangle(0, 0, 365, 500));
}

Setup_Theme::~Setup_Theme()
{
    delete mThemesModel;
    mThemesModel = 0;

    delete mFontsModel;
    mFontsModel = 0;
}

void Setup_Theme::action(const gcn::ActionEvent &event)
{
    if (event.getId() == ACTION_THEME)
    {
        if (mThemeDropDown->getSelected() == 0)
            mTheme = "";
        else
            mTheme = mThemeDropDown->getSelectedString();
    }
    else if (event.getId() == ACTION_FONT)
    {
        mFont = mFontDropDown->getSelectedString();
    }
    else if (event.getId() == ACTION_BOLD_FONT)
    {
        mBoldFont = mBoldFontDropDown->getSelectedString();
    }
    else if (event.getId() == ACTION_PARTICLE_FONT)
    {
        mParticleFont = mParticleFontDropDown->getSelectedString();
    }
    else if (event.getId() == ACTION_HELP_FONT)
    {
        mHelpFont = mHelpFontDropDown->getSelectedString();
    }
}

void Setup_Theme::cancel()
{
    mTheme = config.getValue("theme", config.getValue("selectedSkin", ""));
    mFont = getFileName(config.getStringValue("font"));
    mBoldFont = getFileName(config.getStringValue("boldFont"));
    mParticleFont = getFileName(config.getStringValue("particleFont"));
    mHelpFont = getFileName(config.getStringValue("helpFont"));
}

void Setup_Theme::apply()
{
    if (config.getValue("theme",
        config.getValue("selectedSkin", "")) != mTheme)
    {
        new OkDialog(_("Theme Changed"),
                     _("Restart your client for the change to take effect."));
    }
    config.setValue("selectedSkin", "");
    config.setValue("theme", mTheme);
    if (config.getValue("font", "dejavusans.ttf") != mFont
        || config.getValue("boldFont", "dejavusans-bold.ttf") != mBoldFont
        || config.getValue("particleFont", "dejavusans.ttf") != mParticleFont
        || config.getValue("helpFont", "dejavusansmono.ttf") != mHelpFont)
    {
        config.setValue("font", "fonts/" + getFileName(mFont));
        config.setValue("boldFont", "fonts/" + getFileName(mBoldFont));
        config.setValue("particleFont", "fonts/" + getFileName(mParticleFont));
        config.setValue("helpFont", "fonts/" + getFileName(mHelpFont));
        gui->updateFonts();
    }
}
