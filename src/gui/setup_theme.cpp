/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/extendedlistmodel.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/namesmodel.h"

#include "configuration.h"
#include "localplayer.h"

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
const char* ACTION_INFO = "info";

class ThemesModel final : public NamesModel
{
public:
    ThemesModel() :
        NamesModel()
    {
        mNames.push_back(gettext("(default)"));
        Theme::fillSkinsList(mNames);
    }

    virtual ~ThemesModel()
    { }
};

class FontsModel final : public NamesModel
{
public:
    FontsModel() :
        NamesModel()
    { Theme::fillFontsList(mNames); }

    virtual ~FontsModel()
    { }
};

const int maxFontSizes = 16;

const char *SIZE_NAME[maxFontSizes] =
{
    // TRANSLATORS: font size
    N_("Very small (8)"),
    // TRANSLATORS: font size
    N_("Very small (9)"),
    // TRANSLATORS: font size
    N_("Tiny (10)"),
    // TRANSLATORS: font size
    N_("Small (11)"),
    // TRANSLATORS: font size
    N_("Medium (12)"),
    // TRANSLATORS: font size
    N_("Normal (13)"),
    // TRANSLATORS: font size
    N_("Large (14)"),
    // TRANSLATORS: font size
    N_("Large (15)"),
    // TRANSLATORS: font size
    N_("Large (16)"),
    // TRANSLATORS: font size
    N_("Big (17)"),
    // TRANSLATORS: font size
    N_("Big (18)"),
    // TRANSLATORS: font size
    N_("Big (19)"),
    // TRANSLATORS: font size
    N_("Very big (20)"),
    // TRANSLATORS: font size
    N_("Very big (21)"),
    // TRANSLATORS: font size
    N_("Very big (22)"),
    // TRANSLATORS: font size
    N_("Huge (23)"),
};

class FontSizeChoiceListModel final : public gcn::ListModel
{
public:
    virtual ~FontSizeChoiceListModel()
    { }

    virtual int getNumberOfElements()
    { return maxFontSizes; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return "???";

        return gettext(SIZE_NAME[i]);
    }
};

struct Language final
{
    std::string name;
    std::string value;
    std::string icon;
};

const int langs_count = 17;

const Language LANG_NAME[langs_count] =
{
    // TRANSLATORS: language
    {N_("(default)"), "", ""},
    // TRANSLATORS: language
    {N_("Chinese (China)"), "zh_CN", "cn.png"},
    // TRANSLATORS: language
    {N_("Czech"), "cs_CZ", "cz.png"},
    // TRANSLATORS: language
    {N_("English"), "C", "en.png"},
    // TRANSLATORS: language
    {N_("Finnish"), "fi_FI", "fi.png"},
    // TRANSLATORS: language
    {N_("French"), "fr_FR", "fr.png"},
    // TRANSLATORS: language
    {N_("German"), "de_DE", "de.png"},
    // TRANSLATORS: language
    {N_("Indonesian"), "id_ID", "id.png"},
    // TRANSLATORS: language
    {N_("Italian"), "it_IT", "it.png"},
    // TRANSLATORS: language
    {N_("Polish"), "pl_PL", "pl.png"},
    // TRANSLATORS: language
    {N_("Japanese"), "ja_JP", "jp.png"},
    // TRANSLATORS: language
    {N_("Dutch (Belgium/Flemish)"), "nl_BE", "nl_BE.png"},
    // TRANSLATORS: language
    {N_("Portuguese"), "pt_PT", "pt.png"},
    // TRANSLATORS: language
    {N_("Portuguese (Brazilian)"), "pt_BR", "pt_BR.png"},
    // TRANSLATORS: language
    {N_("Russian"), "ru_RU", "ru.png"},
    // TRANSLATORS: language
    {N_("Spanish (Castilian)"), "es_ES", "es.png"},
    // TRANSLATORS: language
    {N_("Turkish"), "tr_TR", "tr.png"}
};

class LangListModel final : public ExtendedListModel
{
public:
    LangListModel()
    {
        ResourceManager *const resman = ResourceManager::getInstance();
        for (int f = 0; f < langs_count; f ++)
        {
            mIcons[f] = resman->getImage("graphics/flags/"
                + LANG_NAME[f].icon);
        }
    }

    A_DELETE_COPY(LangListModel)

    virtual ~LangListModel()
    {
        for (int f = 0; f < langs_count; f ++)
        {
            Image *const img = mIcons[f];
            if (img)
                img->decRef();
        }
    }

    int getNumberOfElements() override A_WARN_UNUSED
    { return langs_count; }

    std::string getElementAt(int i) override A_WARN_UNUSED
    {
        if (i >= getNumberOfElements() || i < 0)
            return "???";

        return gettext(LANG_NAME[i].name.c_str());
    }

    const Image *getImageAt(int i) override A_WARN_UNUSED
    {
        if (i >= getNumberOfElements() || i < 0)
            return nullptr;
        return mIcons[i];
    }

    Image *mIcons[langs_count];
};

Setup_Theme::Setup_Theme(const Widget2 *const widget) :
    SetupTab(widget),
    // TRANSLATORS: theme settings label
    mThemeLabel(new Label(this, _("Gui theme"))),
    mThemesModel(new ThemesModel),
    mThemeDropDown(new DropDown(this, mThemesModel)),
    mTheme(config.getStringValue("theme")),
    mInfo(Theme::loadInfo(mTheme)),
    mFontsModel(new FontsModel),
    // TRANSLATORS: theme settings label
    mFontLabel(new Label(this, _("Main Font"))),
    mFontDropDown(new DropDown(this, mFontsModel)),
    mFont(config.getStringValue("font")),
    mLangListModel(new LangListModel),
    // TRANSLATORS: theme settings label
    mLangLabel(new Label(this, _("Language"))),
    mLangDropDown(new DropDown(this, mLangListModel, true)),
    mLang(config.getStringValue("lang")),
    // TRANSLATORS: theme settings label
    mBoldFontLabel(new Label(this, _("Bold font"))),
    mBoldFontDropDown(new DropDown(this, mFontsModel)),
    mBoldFont(config.getStringValue("boldFont")),
    // TRANSLATORS: theme settings label
    mParticleFontLabel(new Label(this, _("Particle font"))),
    mParticleFontDropDown(new DropDown(this, mFontsModel)),
    mParticleFont(config.getStringValue("particleFont")),
    // TRANSLATORS: theme settings label
    mHelpFontLabel(new Label(this, _("Help font"))),
    mHelpFontDropDown(new DropDown(this, mFontsModel)),
    mHelpFont(config.getStringValue("helpFont")),
    // TRANSLATORS: theme settings label
    mSecureFontLabel(new Label(this, _("Secure font"))),
    mSecureFontDropDown(new DropDown(this, mFontsModel)),
    mSecureFont(config.getStringValue("secureFont")),
    // TRANSLATORS: theme settings label
    mJapanFontLabel(new Label(this, _("Japanese font"))),
    mJapanFontDropDown(new DropDown(this, mFontsModel)),
    mJapanFont(config.getStringValue("japanFont")),
    mFontSizeListModel(new FontSizeChoiceListModel),
    // TRANSLATORS: theme settings label
    mFontSizeLabel(new Label(this, _("Font size"))),
    mFontSize(config.getIntValue("fontSize")),
    mFontSizeDropDown(new DropDown(this, mFontSizeListModel)),
    mNpcFontSizeListModel(new FontSizeChoiceListModel),
    // TRANSLATORS: theme settings label
    mNpcFontSizeLabel(new Label(this, _("Npc font size"))),
    mNpcFontSize(config.getIntValue("npcfontSize")),
    mNpcFontSizeDropDown(new DropDown(this, mNpcFontSizeListModel)),
    // TRANSLATORS: button name with information about selected theme
    mInfoButton(new Button(this, _("i"), ACTION_INFO, this)),
    mThemeInfo()
{
    // TRANSLATORS: theme settings tab name
    setName(_("Theme"));

    mThemeDropDown->setActionEventId(ACTION_THEME);
    mThemeDropDown->addActionListener(this);
    mFontDropDown->setActionEventId(ACTION_FONT);
    mFontDropDown->addActionListener(this);
    mLangDropDown->setActionEventId(ACTION_LANG);
    mLangDropDown->addActionListener(this);
    mBoldFontDropDown->setActionEventId(ACTION_BOLD_FONT);
    mBoldFontDropDown->addActionListener(this);
    mParticleFontDropDown->setActionEventId(ACTION_PARTICLE_FONT);
    mParticleFontDropDown->addActionListener(this);
    mHelpFontDropDown->setActionEventId(ACTION_HELP_FONT);
    mHelpFontDropDown->addActionListener(this);
    mSecureFontDropDown->setActionEventId(ACTION_SECURE_FONT);
    mSecureFontDropDown->addActionListener(this);
    mJapanFontDropDown->setActionEventId(ACTION_JAPAN_FONT);
    mJapanFontDropDown->addActionListener(this);
    mFontSizeDropDown->setSelected(mFontSize - 9);
    mFontSizeDropDown->adjustHeight();
    mNpcFontSizeDropDown->setSelected(mNpcFontSize - 9);
    mNpcFontSizeDropDown->adjustHeight();

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

    updateInfo();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mThemeLabel, 5);
    place(0, 1, mLangLabel, 5);
    place(0, 2, mFontSizeLabel, 5);
    place(0, 3, mNpcFontSizeLabel, 5);
    place(0, 4, mFontLabel, 5);
    place(0, 5, mBoldFontLabel, 5);
    place(0, 6, mParticleFontLabel, 5);
    place(0, 7, mHelpFontLabel, 5);
    place(0, 8, mSecureFontLabel, 5);
    place(0, 9, mJapanFontLabel, 5);

    place(6, 0, mThemeDropDown, 10);
    place(6, 1, mLangDropDown, 10);
    place(6, 2, mFontSizeDropDown, 10);
    place(6, 3, mNpcFontSizeDropDown, 10);
    place(6, 4, mFontDropDown, 10);
    place(6, 5, mBoldFontDropDown, 10);
    place(6, 6, mParticleFontDropDown, 10);
    place(6, 7, mHelpFontDropDown, 10);
    place(6, 8, mSecureFontDropDown, 10);
    place(6, 9, mJapanFontDropDown, 10);

    place(17, 0, mInfoButton, 1);

    setDimension(gcn::Rectangle(0, 0, 365, 500));
}

Setup_Theme::~Setup_Theme()
{
    delete mInfo;
    mInfo = nullptr;

    delete mThemesModel;
    mThemesModel = nullptr;

    delete mFontsModel;
    mFontsModel = nullptr;

    delete mFontSizeListModel;
    mFontSizeListModel = nullptr;

    delete mNpcFontSizeListModel;
    mNpcFontSizeListModel = nullptr;

    delete mLangListModel;
    mLangListModel = nullptr;
}

void Setup_Theme::updateInfo()
{
    mInfo = Theme::loadInfo(mTheme);
    if (mInfo)
    {
        mThemeInfo = std::string("Name: ").append(mInfo->name)
            .append("\nCopyright:\n").append(mInfo->copyright);
    }
    else
    {
        mThemeInfo.clear();
    }
    replaceAll(mThemeInfo, "\\n", "\n");
    mInfoButton->setEnabled(!mThemeInfo.empty());
}

void Setup_Theme::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == ACTION_THEME)
    {
        if (mThemeDropDown->getSelected() == 0)
            mTheme.clear();
        else
            mTheme = mThemeDropDown->getSelectedString();
        updateInfo();
    }
    else if (eventId == ACTION_FONT)
    {
        mFont = mFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_LANG)
    {
        const int id = mLangDropDown->getSelected();
        if (id < 0 || id >= langs_count)
            mLang.clear();
        else
            mLang = LANG_NAME[id].value;
    }
    else if (eventId == ACTION_BOLD_FONT)
    {
        mBoldFont = mBoldFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_PARTICLE_FONT)
    {
        mParticleFont = mParticleFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_HELP_FONT)
    {
        mHelpFont = mHelpFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_SECURE_FONT)
    {
        mSecureFont = mSecureFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_JAPAN_FONT)
    {
        mJapanFont = mJapanFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_INFO)
    {
        // TRANSLATORS: theme info dialog header
        new OkDialog(_("Theme info"), mThemeInfo, DIALOG_OK,
            false, true, nullptr, 600);
    }
}

void Setup_Theme::cancel()
{
    mTheme = config.getStringValue("theme");
    mLang = config.getStringValue("lang");
    mFont = getFileName(config.getStringValue("font"));
    mBoldFont = getFileName(config.getStringValue("boldFont"));
    mParticleFont = getFileName(config.getStringValue("particleFont"));
    mHelpFont = getFileName(config.getStringValue("helpFont"));
    mSecureFont = getFileName(config.getStringValue("secureFont"));
    mJapanFont = getFileName(config.getStringValue("japanFont"));
}

#define updateField(name1, name2) if (!mInfo->name1.empty()) \
    name2 = mInfo->name1;

void Setup_Theme::apply()
{
    if (config.getStringValue("theme") != mTheme)
    {
        // TRANSLATORS: theme message dialog
        new OkDialog(_("Theme Changed"), _("Restart your client for "
            "the change to take effect."));
    }

    config.setValue("selectedSkin", "");
    if (config.getStringValue("theme") != mTheme && mInfo)
    {
        updateField(font, mFont);
        updateField(boldFont, mBoldFont);
        updateField(particleFont, mParticleFont);
        updateField(helpFont, mHelpFont);
        updateField(secureFont, mSecureFont);
        updateField(japanFont, mJapanFont);
        if (mInfo->fontSize)
        {
            const int size = mInfo->fontSize - 9;
            if (size >= 0)
                mFontSizeDropDown->setSelected(size);
        }
        if (mInfo->npcfontSize)
        {
            const int size = mInfo->npcfontSize - 9;
            if (size >= 0)
                mNpcFontSizeDropDown->setSelected(size);
        }
    }
    config.setValue("theme", mTheme);
    config.setValue("lang", mLang);
    if (config.getValue("font", "dejavusans.ttf") != mFont
        || config.getValue("boldFont", "dejavusans-bold.ttf") != mBoldFont
        || config.getValue("particleFont", "dejavusans.ttf") != mParticleFont
        || config.getValue("helpFont", "dejavusansmono.ttf") != mHelpFont
        || config.getValue("secureFont", "dejavusansmono.ttf") != mSecureFont
        || config.getValue("japanFont", "mplus-1p-regular.ttf") != mJapanFont
        || config.getIntValue("fontSize")
        != static_cast<int>(mFontSizeDropDown->getSelected()) + 9
        || config.getIntValue("npcfontSize")
        != static_cast<int>(mNpcFontSizeDropDown->getSelected()) + 9)
    {
        config.setValue("font", "fonts/" + getFileName(mFont));
        config.setValue("boldFont", "fonts/" + getFileName(mBoldFont));
        config.setValue("particleFont", "fonts/" + getFileName(mParticleFont));
        config.setValue("helpFont", "fonts/" + getFileName(mHelpFont));
        config.setValue("secureFont", "fonts/" + getFileName(mSecureFont));
        config.setValue("japanFont", "fonts/" + getFileName(mJapanFont));
        config.setValue("fontSize", mFontSizeDropDown->getSelected() + 9);
        config.setValue("npcfontSize",
            mNpcFontSizeDropDown->getSelected() + 9);
        gui->updateFonts();
    }
}

#undef updateField
