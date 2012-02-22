/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
#include "logger.h"

#include "utils/gettext.h"

#include "resources/resourcemanager.h"

#include "debug.h"

const char* ACTION_THEME = "theme";
const char* ACTION_FONT = "font";
const char* ACTION_LANG = "lang";
const char* ACTION_BOLD_FONT = "bold font";
const char* ACTION_PARTICLE_FONT = "particle font";
const char* ACTION_HELP_FONT = "help font";
const char* ACTION_SECURE_FONT = "secure font";
const char* ACTION_JAPAN_FONT = "japan font";

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
        mNames.push_back(gettext("(default)"));
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

const char *SIZE_NAME[6] =
{
    N_("Tiny (10)"),
    N_("Small (11)"),
    N_("Medium (12)"),
    N_("Large (13)"),
    N_("Big (14)"),
    N_("Huge (15)"),
};

class FontSizeChoiceListModel : public gcn::ListModel
{
public:
    virtual ~FontSizeChoiceListModel()
    { }

    virtual int getNumberOfElements()
    { return 6; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(SIZE_NAME[i]);
    }
};

struct Language
{
    std::string name;

    std::string value;
};

const int langs_count = 15;

const Language LANG_NAME[langs_count] =
{
    {N_("(default)"), ""},
    {N_("Chinese (China)"), "zh_CN"},
    {N_("Czech"), "cs_CZ"},
    {N_("English"), "C"},
    {N_("Finnish"), "fi_FI"},
    {N_("French"), "fr_FR"},
    {N_("German"), "de_DE"},
    {N_("Indonesian"), "id_ID"},
    {N_("Polish"), "pl_PL"},
    {N_("Japanese"), "ja_JP.utf8"},
    {N_("Dutch (Belgium/Flemish)"), "nl_BE"},
    {N_("Portuguese"), "pt_PT"},
    {N_("Portuguese (Brazilian)"), "pt_BR"},
    {N_("Russian"), "ru_RU"},
    {N_("Spanish (Castilian)"), "es_ES"}
};

class LangListModel : public gcn::ListModel
{
public:
    virtual ~LangListModel()
    { }

    virtual int getNumberOfElements()
    { return langs_count; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(LANG_NAME[i].name.c_str());
    }
};

Setup_Theme::Setup_Theme():
    mTheme(config.getValue("theme", config.getValue("selectedSkin", ""))),
    mFont(config.getStringValue("font")),
    mLang(config.getStringValue("lang")),
    mBoldFont(config.getStringValue("boldFont")),
    mParticleFont(config.getStringValue("particleFont")),
    mHelpFont(config.getStringValue("helpFont")),
    mSecureFont(config.getStringValue("secureFont")),
    mJapanFont(config.getStringValue("japanFont")),
    mFontSize(config.getIntValue("fontSize"))
{
    setName(_("Theme"));

    mThemeLabel = new Label(_("Gui theme"));
    mLangLabel = new Label(_("Language"));
    mFontLabel = new Label(_("Main Font"));
    mBoldFontLabel = new Label(_("Bold font"));
    mParticleFontLabel = new Label(_("Particle font"));
    mHelpFontLabel = new Label(_("Help font"));
    mSecureFontLabel = new Label(_("Secure font"));
    mJapanFontLabel = new Label(_("Japanese font"));
    mThemesModel  = new ThemesModel();
    mFontsModel  = new FontsModel();
    mLangListModel = new LangListModel();

    mThemeDropDown = new DropDown(mThemesModel);
    mThemeDropDown->setActionEventId(ACTION_THEME);
    mThemeDropDown->addActionListener(this);

    mFontDropDown = new DropDown(mFontsModel);
    mFontDropDown->setActionEventId(ACTION_FONT);
    mFontDropDown->addActionListener(this);

    mLangDropDown = new DropDown(mLangListModel);
    mLangDropDown->setActionEventId(ACTION_LANG);
    mLangDropDown->addActionListener(this);

    mBoldFontDropDown = new DropDown(mFontsModel);
    mBoldFontDropDown->setActionEventId(ACTION_BOLD_FONT);
    mBoldFontDropDown->addActionListener(this);

    mParticleFontDropDown = new DropDown(mFontsModel);
    mParticleFontDropDown->setActionEventId(ACTION_PARTICLE_FONT);
    mParticleFontDropDown->addActionListener(this);

    mHelpFontDropDown = new DropDown(mFontsModel);
    mHelpFontDropDown->setActionEventId(ACTION_HELP_FONT);
    mHelpFontDropDown->addActionListener(this);

    mSecureFontDropDown = new DropDown(mFontsModel);
    mSecureFontDropDown->setActionEventId(ACTION_SECURE_FONT);
    mSecureFontDropDown->addActionListener(this);

    mJapanFontDropDown = new DropDown(mFontsModel);
    mJapanFontDropDown->setActionEventId(ACTION_JAPAN_FONT);
    mJapanFontDropDown->addActionListener(this);

    fontSizeLabel = new Label(_("Font size"));
    mFontSizeListModel = new FontSizeChoiceListModel;
    mFontSizeDropDown = new DropDown(mFontSizeListModel);
    mFontSizeDropDown->setSelected(mFontSize - 10);
    mFontSizeDropDown->adjustHeight();

    std::string skin = Theme::getThemeName();
    if (!skin.empty())
        mThemeDropDown->setSelectedString(skin);
    else
        mThemeDropDown->setSelected(0);

    const std::string str = config.getStringValue("lang");

    for (int f = 0; f < langs_count; f ++)
    {
        if (LANG_NAME[f].value == str)
        {
            mLangDropDown->setSelected(f);
            break;
        }
    }

    mFontDropDown->setSelectedString(getFileName(
        config.getStringValue("font")));
    mBoldFontDropDown->setSelectedString(getFileName(
        config.getStringValue("boldFont")));
    mParticleFontDropDown->setSelectedString(getFileName(
        config.getStringValue("particleFont")));
    mHelpFontDropDown->setSelectedString(getFileName(
        config.getStringValue("helpFont")));
    mSecureFontDropDown->setSelectedString(getFileName(
        config.getStringValue("secureFont")));
    mJapanFontDropDown->setSelectedString(getFileName(
        config.getStringValue("japanFont")));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mThemeLabel, 5);
    place(0, 1, mLangLabel, 5);
    place(0, 2, fontSizeLabel, 5);
    place(0, 3, mFontLabel, 5);
    place(0, 4, mBoldFontLabel, 5);
    place(0, 5, mParticleFontLabel, 5);
    place(0, 6, mHelpFontLabel, 5);
    place(0, 7, mSecureFontLabel, 5);
    place(0, 8, mJapanFontLabel, 5);

    place(6, 0, mThemeDropDown, 10);
    place(6, 1, mLangDropDown, 10);
    place(6, 2, mFontSizeDropDown, 10);
    place(6, 3, mFontDropDown, 10);
    place(6, 4, mBoldFontDropDown, 10);
    place(6, 5, mParticleFontDropDown, 10);
    place(6, 6, mHelpFontDropDown, 10);
    place(6, 7, mSecureFontDropDown, 10);
    place(6, 8, mJapanFontDropDown, 10);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);

    setDimension(gcn::Rectangle(0, 0, 365, 500));
}

Setup_Theme::~Setup_Theme()
{
    delete mThemesModel;
    mThemesModel = nullptr;

    delete mFontsModel;
    mFontsModel = nullptr;

    delete mFontSizeListModel;
    mFontSizeListModel = nullptr;

    delete mLangListModel;
    mLangListModel = nullptr;
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
    else if (event.getId() == ACTION_LANG)
    {
        int id = mLangDropDown->getSelected();
        if (id < 0 || id >= langs_count)
            mLang = "";
        else
            mLang = LANG_NAME[id].value;
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
    else if (event.getId() == ACTION_SECURE_FONT)
    {
        mSecureFont = mSecureFontDropDown->getSelectedString();
    }
    else if (event.getId() == ACTION_JAPAN_FONT)
    {
        mJapanFont = mJapanFontDropDown->getSelectedString();
    }
}

void Setup_Theme::cancel()
{
    mTheme = config.getValue("theme", config.getValue("selectedSkin", ""));
    mLang = config.getStringValue("lang");
    mFont = getFileName(config.getStringValue("font"));
    mBoldFont = getFileName(config.getStringValue("boldFont"));
    mParticleFont = getFileName(config.getStringValue("particleFont"));
    mHelpFont = getFileName(config.getStringValue("helpFont"));
    mSecureFont = getFileName(config.getStringValue("secureFont"));
    mJapanFont = getFileName(config.getStringValue("japanFont"));
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
    config.setValue("lang", mLang);
    if (config.getValue("font", "dejavusans.ttf") != mFont
        || config.getValue("boldFont", "dejavusans-bold.ttf") != mBoldFont
        || config.getValue("particleFont", "dejavusans.ttf") != mParticleFont
        || config.getValue("helpFont", "dejavusansmono.ttf") != mHelpFont
        || config.getValue("secureFont", "dejavusansmono.ttf") != mSecureFont
        || config.getValue("japanFont", "mplus-1p-regular.ttf") != mJapanFont
        || config.getIntValue("fontSize")
        != static_cast<int>(mFontSizeDropDown->getSelected()) + 10)
    {
        config.setValue("font", "fonts/" + getFileName(mFont));
        config.setValue("boldFont", "fonts/" + getFileName(mBoldFont));
        config.setValue("particleFont", "fonts/" + getFileName(mParticleFont));
        config.setValue("helpFont", "fonts/" + getFileName(mHelpFont));
        config.setValue("secureFont", "fonts/" + getFileName(mSecureFont));
        config.setValue("japanFont", "fonts/" + getFileName(mJapanFont));
        config.setValue("fontSize", mFontSizeDropDown->getSelected() + 10);
        gui->updateFonts();
    }
}
