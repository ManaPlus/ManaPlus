/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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

#include "gui/widgets/tabs/setup_theme.h"

#include "gui/gui.h"
#include "gui/themeinfo.h"

#include "gui/windows/okdialog.h"

#include "gui/models/fontsizechoicelistmodel.h"
#include "gui/models/fontsmodel.h"
#include "gui/models/langlistmodel.h"
#include "gui/models/themesmodel.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "configuration.h"

#include "utils/delete2.h"

#include "debug.h"

const char* ACTION_THEME = "theme";
const char* ACTION_FONT = "font";
const char* ACTION_LANG = "lang";
const char* ACTION_BOLD_FONT = "bold font";
const char* ACTION_PARTICLE_FONT = "particle font";
const char* ACTION_HELP_FONT = "help font";
const char* ACTION_SECURE_FONT = "secure font";
const char* ACTION_NPC_FONT = "npc font";
const char* ACTION_JAPAN_FONT = "japanese font";
const char* ACTION_CHINA_FONT = "chinese font";
const char* ACTION_INFO = "info";

Setup_Theme::Setup_Theme(const Widget2 *const widget) :
    SetupTab(widget),
    // TRANSLATORS: theme settings label
    mThemeLabel(new Label(this, _("Gui theme"))),
    mThemesModel(new ThemesModel),
    mThemeDropDown(new DropDown(this, mThemesModel,
        false, Modal_false, nullptr, std::string())),
    mTheme(config.getStringValue("theme")),
    mInfo(Theme::loadInfo(mTheme)),
    mFontsModel(new FontsModel),
    // TRANSLATORS: theme settings label
    mFontLabel(new Label(this, _("Main Font"))),
    mFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mFont(config.getStringValue("font")),
    mLangListModel(new LangListModel),
    // TRANSLATORS: theme settings label
    mLangLabel(new Label(this, _("Language"))),
    mLangDropDown(new DropDown(this, mLangListModel,
        true, Modal_false, nullptr, std::string())),
    mLang(config.getStringValue("lang")),
    // TRANSLATORS: theme settings label
    mBoldFontLabel(new Label(this, _("Bold font"))),
    mBoldFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mBoldFont(config.getStringValue("boldFont")),
    // TRANSLATORS: theme settings label
    mParticleFontLabel(new Label(this, _("Particle font"))),
    mParticleFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mParticleFont(config.getStringValue("particleFont")),
    // TRANSLATORS: theme settings label
    mHelpFontLabel(new Label(this, _("Help font"))),
    mHelpFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mHelpFont(config.getStringValue("helpFont")),
    // TRANSLATORS: theme settings label
    mSecureFontLabel(new Label(this, _("Secure font"))),
    mSecureFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mSecureFont(config.getStringValue("secureFont")),
    // TRANSLATORS: theme settings label
    mNpcFontLabel(new Label(this, _("Npc font"))),
    mNpcFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mNpcFont(config.getStringValue("npcFont")),
    // TRANSLATORS: theme settings label
    mJapanFontLabel(new Label(this, _("Japanese font"))),
    mJapanFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mJapanFont(config.getStringValue("japanFont")),
    // TRANSLATORS: theme settings label
    mChinaFontLabel(new Label(this, _("Chinese font"))),
    mChinaFontDropDown(new DropDown(this, mFontsModel,
        false, Modal_false, nullptr, std::string())),
    mChinaFont(config.getStringValue("chinaFont")),
    mFontSizeListModel(new FontSizeChoiceListModel),
    // TRANSLATORS: theme settings label
    mFontSizeLabel(new Label(this, _("Font size"))),
    mFontSize(config.getIntValue("fontSize")),
    mFontSizeDropDown(new DropDown(this, mFontSizeListModel,
        false, Modal_false, nullptr, std::string())),
    mNpcFontSizeListModel(new FontSizeChoiceListModel),
    // TRANSLATORS: theme settings label
    mNpcFontSizeLabel(new Label(this, _("Npc font size"))),
    mNpcFontSize(config.getIntValue("npcfontSize")),
    mNpcFontSizeDropDown(new DropDown(this, mNpcFontSizeListModel,
        false, Modal_false, nullptr, std::string())),
    // TRANSLATORS: button name with information about selected theme
    mInfoButton(new Button(this, _("i"), ACTION_INFO, BUTTON_SKIN, this)),
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
    mNpcFontDropDown->setActionEventId(ACTION_NPC_FONT);
    mNpcFontDropDown->addActionListener(this);
    mJapanFontDropDown->setActionEventId(ACTION_JAPAN_FONT);
    mJapanFontDropDown->addActionListener(this);
    mChinaFontDropDown->setActionEventId(ACTION_CHINA_FONT);
    mChinaFontDropDown->addActionListener(this);
    mFontSizeDropDown->setSelected(mFontSize - 9);
    mFontSizeDropDown->adjustHeight();
    mNpcFontSizeDropDown->setSelected(mNpcFontSize - 9);
    mNpcFontSizeDropDown->adjustHeight();

    const std::string skin = Theme::getThemeName();
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
    mNpcFontDropDown->setSelectedString(getFileName(
        config.getStringValue("npcFont")));
    mJapanFontDropDown->setSelectedString(getFileName(
        config.getStringValue("japanFont")));
    mChinaFontDropDown->setSelectedString(getFileName(
        config.getStringValue("chinaFont")));

    updateInfo();

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mThemeLabel, 5, 1);
    place(0, 1, mLangLabel, 5, 1);
    place(0, 2, mFontSizeLabel, 5, 1);
    place(0, 3, mNpcFontSizeLabel, 5, 1);
    place(0, 4, mFontLabel, 5, 1);
    place(0, 5, mBoldFontLabel, 5, 1);
    place(0, 6, mParticleFontLabel, 5, 1);
    place(0, 7, mHelpFontLabel, 5, 1);
    place(0, 8, mSecureFontLabel, 5, 1);
    place(0, 9, mNpcFontLabel, 5, 1);
    place(0, 10, mJapanFontLabel, 5, 1);
    place(0, 11, mChinaFontLabel, 5, 1);

    place(6, 0, mThemeDropDown, 10, 1);
    place(6, 1, mLangDropDown, 10, 1);
    place(6, 2, mFontSizeDropDown, 10, 1);
    place(6, 3, mNpcFontSizeDropDown, 10, 1);
    place(6, 4, mFontDropDown, 10, 1);
    place(6, 5, mBoldFontDropDown, 10, 1);
    place(6, 6, mParticleFontDropDown, 10, 1);
    place(6, 7, mHelpFontDropDown, 10, 1);
    place(6, 8, mSecureFontDropDown, 10, 1);
    place(6, 9, mNpcFontDropDown, 10, 1);
    place(6, 10, mJapanFontDropDown, 10, 1);
    place(6, 11, mChinaFontDropDown, 10, 1);

    place(17, 0, mInfoButton, 1, 1);

    int size = mainGraphics->mWidth - 10;
    const int maxWidth = mFontSize * 30 + 290;
    if (size < 465)
        size = 465;
    else if (size > maxWidth)
        size = maxWidth;

    setDimension(Rect(0, 0, size, 500));
}

Setup_Theme::~Setup_Theme()
{
    delete2(mInfo)
    delete2(mThemesModel)
    delete2(mFontsModel)
    delete2(mFontSizeListModel)
    delete2(mNpcFontSizeListModel)
    delete2(mLangListModel)
}

void Setup_Theme::updateInfo()
{
    delete mInfo;
    mInfo = Theme::loadInfo(mTheme);
    if (mInfo != nullptr)
    {
        // TRANSLATORS: theme name
        mThemeInfo = std::string(_("Name: ")).append(mInfo->name)
            // TRANSLATORS: theme copyright
            .append("\n").append(_("Copyright:")).append("\n")
            .append(mInfo->copyright);
    }
    else
    {
        mThemeInfo.clear();
    }
    replaceAll(mThemeInfo, "\\n", "\n");
    mInfoButton->setEnabled(!mThemeInfo.empty());
}

void Setup_Theme::action(const ActionEvent &event)
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
    else if (eventId == ACTION_NPC_FONT)
    {
        mNpcFont = mNpcFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_JAPAN_FONT)
    {
        mJapanFont = mJapanFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_CHINA_FONT)
    {
        mChinaFont = mChinaFontDropDown->getSelectedString();
    }
    else if (eventId == ACTION_INFO)
    {
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: theme info dialog header
            _("Theme info"),
            mThemeInfo,
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            600);
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
    mNpcFont = getFileName(config.getStringValue("npcFont"));
    mJapanFont = getFileName(config.getStringValue("japanFont"));
    mChinaFont = getFileName(config.getStringValue("chinaFont"));
}

#define updateField(name1, name2) if (!mInfo->name1.empty()) \
    name2 = mInfo->name1

void Setup_Theme::apply()
{
    if (config.getStringValue("theme") != mTheme)
    {
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: theme message dialog
            _("Theme Changed"),
            // TRANSLATORS: ok dialog message
            _("Restart your client for the change to take effect."),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
    }

    config.setValue("selectedSkin", "");
    if (config.getStringValue("theme") != mTheme && (mInfo != nullptr))
    {
        updateField(font, mFont);
        updateField(boldFont, mBoldFont);
        updateField(particleFont, mParticleFont);
        updateField(helpFont, mHelpFont);
        updateField(secureFont, mSecureFont);
        updateField(npcFont, mNpcFont);
        updateField(japanFont, mJapanFont);
        updateField(chinaFont, mChinaFont);
        if (mInfo->fontSize != 0)
        {
            const int size = mInfo->fontSize - 9;
            if (size >= 0)
                mFontSizeDropDown->setSelected(size);
        }
        if (mInfo->npcfontSize != 0)
        {
            const int size = mInfo->npcfontSize - 9;
            if (size >= 0)
                mNpcFontSizeDropDown->setSelected(size);
        }
        if (mInfo->guiAlpha > 0.01F)
            config.setValue("guialpha", mInfo->guiAlpha);
    }
    config.setValue("theme", mTheme);
    config.setValue("lang", mLang);
    if (config.getValue("font", "dejavusans.ttf") != mFont
        || config.getValue("boldFont", "dejavusans-bold.ttf") != mBoldFont
        || config.getValue("particleFont", "dejavusans.ttf") != mParticleFont
        || config.getValue("helpFont", "dejavusansmono.ttf") != mHelpFont
        || config.getValue("secureFont", "dejavusansmono.ttf") != mSecureFont
        || config.getValue("npcFont", "dejavusans.ttf") != mNpcFont
        || config.getValue("japanFont", "mplus-1p-regular.ttf") != mJapanFont
        || config.getValue("chinaFont", "fonts/wqy-microhei.ttf")
        != mChinaFont
        || config.getIntValue("fontSize")
        != CAST_S32(mFontSizeDropDown->getSelected()) + 9
        || config.getIntValue("npcfontSize")
        != CAST_S32(mNpcFontSizeDropDown->getSelected()) + 9)
    {
        config.setValue("font", "fonts/" + getFileName(mFont));
        config.setValue("boldFont", "fonts/" + getFileName(mBoldFont));
        config.setValue("particleFont", "fonts/" + getFileName(mParticleFont));
        config.setValue("helpFont", "fonts/" + getFileName(mHelpFont));
        config.setValue("secureFont", "fonts/" + getFileName(mSecureFont));
        config.setValue("npcFont", "fonts/" + getFileName(mNpcFont));
        config.setValue("japanFont", "fonts/" + getFileName(mJapanFont));
        config.setValue("chinaFont", "fonts/" + getFileName(mChinaFont));
        config.setValue("fontSize", mFontSizeDropDown->getSelected() + 9);
        config.setValue("npcfontSize",
            mNpcFontSizeDropDown->getSelected() + 9);
        gui->updateFonts();
    }
}

#undef updateField
