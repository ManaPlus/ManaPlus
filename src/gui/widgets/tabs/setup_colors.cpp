/*
 *  Configurable text colors
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
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

#include "gui/widgets/tabs/setup_colors.h"

#include "gui/font.h"
#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/textpreview.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <string>
#include <cmath>

#include "debug.h"

// TRANSLATORS: color selection preview message
const char *const Setup_Colors::rawmsg =
    N_("This is what the color looks like");

Setup_Colors::Setup_Colors(const Widget2 *const widget) :
    SetupTab(widget),
    SelectionListener(),
    mColorBox(new ListBox(this, userPalette, "")),
    mScroll(new ScrollArea(this, mColorBox,
        true, "setup_colors_background.xml")),
    mPreview(new BrowserBox(this, BrowserBox::AUTO_WRAP, true,
        "browserbox.xml")),
    mTextPreview(new TextPreview(this, gettext(rawmsg))),
    mPreviewBox(new ScrollArea(this, mPreview, true,
        "setup_colors_preview_background.xml")),
    mSelected(-1),
    // TRANSLATORS: colors tab. label.
    mGradTypeLabel(new Label(this, _("Type:"))),
    mGradTypeSlider(new Slider(this, 0, 3)),
    mGradTypeText(new Label(this)),
    // TRANSLATORS: colors tab. label.
    mGradDelayLabel(new Label(this, _("Delay:"))),
    mGradDelaySlider(new Slider(this, 20, 100)),
    mGradDelayText(new TextField(this)),
    // TRANSLATORS: colors tab. label.
    mRedLabel(new Label(this, _("Red:"))),
    mRedSlider(new Slider(this, 0, 255)),
    mRedText(new TextField(this)),
    // TRANSLATORS: colors tab. label.
    mGreenLabel(new Label(this, _("Green:"))),
    mGreenSlider(new Slider(this, 0, 255)),
    mGreenText(new TextField(this)),
    // TRANSLATORS: colors tab. label.
    mBlueLabel(new Label(this, _("Blue:"))),
    mBlueSlider(new Slider(this, 0, 255)),
    mBlueText(new TextField(this))
{
    mColorBox->postInit();

    // TRANSLATORS: settings colors tab name
    setName(_("Colors"));
    mColorBox->addSelectionListener(this);
    mScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mPreview->setOpaque(false);

    // don't do anything with links
    mPreview->setLinkHandler(nullptr);

    mPreviewBox->setHeight(20);
    mPreviewBox->setScrollPolicy(gcn::ScrollArea::SHOW_NEVER,
                                 gcn::ScrollArea::SHOW_NEVER);

    mGradTypeSlider->setWidth(180);
    mGradTypeSlider->setActionEventId("slider_grad");
    mGradTypeSlider->setValue2(0);
    mGradTypeSlider->addActionListener(this);
    mGradTypeSlider->setEnabled(false);

    // TRANSLATORS: color type
    std::string longText = _("Static");

    const Font *const font = getFont();
    if (getFont()->getWidth(_("Pulse")) > font->getWidth(longText))
    {
        // TRANSLATORS: color type
        longText = _("Pulse");
    }
    if (getFont()->getWidth(_("Rainbow")) > font->getWidth(longText))
    {
        // TRANSLATORS: color type
        longText = _("Rainbow");
    }
    if (getFont()->getWidth(_("Spectrum")) > font->getWidth(longText))
    {
        // TRANSLATORS: color type
        longText = _("Spectrum");
    }

    mGradTypeText->setCaption(longText);

    mGradDelayText->setWidth(40);
    mGradDelayText->setRange(20, 100);
    mGradDelayText->setNumeric(true);
    mGradDelayText->setEnabled(false);

    mGradDelaySlider->setWidth(180);
    mGradDelaySlider->setValue2(mGradDelayText->getValue());
    mGradDelaySlider->setActionEventId("slider_graddelay");
    mGradDelaySlider->addActionListener(this);
    mGradDelaySlider->setEnabled(false);

    mRedText->setWidth(40);
    mRedText->setRange(0, 255);
    mRedText->setNumeric(true);
    mRedText->setEnabled(false);

    mRedSlider->setWidth(180);
    mRedSlider->setValue2(mRedText->getValue());
    mRedSlider->setActionEventId("slider_red");
    mRedSlider->addActionListener(this);
    mRedSlider->setEnabled(false);

    mGreenText->setWidth(40);
    mGreenText->setRange(0, 255);
    mGreenText->setNumeric(true);
    mGreenText->setEnabled(false);

    mGreenSlider->setWidth(180);
    mGreenSlider->setValue2(mGreenText->getValue());
    mGreenSlider->setActionEventId("slider_green");
    mGreenSlider->addActionListener(this);
    mGreenSlider->setEnabled(false);

    mBlueText->setWidth(40);
    mBlueText->setRange(0, 255);
    mBlueText->setNumeric(true);
    mBlueText->setEnabled(false);

    mBlueSlider->setWidth(180);
    mBlueSlider->setValue2(mBlueText->getValue());
    mBlueSlider->setActionEventId("slider_blue");
    mBlueSlider->addActionListener(this);
    mBlueSlider->setEnabled(false);

    setOpaque(false);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mScroll, 6, 6).setPadding(2);
    place(0, 6, mPreviewBox, 6).setPadding(2);
    place(0, 7, mGradTypeLabel, 3);
    place(3, 7, mGradTypeSlider);
    place(4, 7, mGradTypeText, 2).setPadding(1);
    place(0, 8, mRedLabel, 3);
    place(3, 8, mRedSlider);
    place(5, 8, mRedText).setPadding(1);
    place(0, 9, mGreenLabel, 3);
    place(3, 9, mGreenSlider);
    place(5, 9, mGreenText).setPadding(1);
    place(0, 10, mBlueLabel, 3);
    place(3, 10, mBlueSlider);
    place(5, 10, mBlueText).setPadding(1);
    place(0, 11, mGradDelayLabel, 3);
    place(3, 11, mGradDelaySlider);
    place(5, 11, mGradDelayText).setPadding(1);

    mGradTypeText->setCaption("");

    setDimension(Rect(0, 0, 365, 350));
}

Setup_Colors::~Setup_Colors()
{
    if (mPreviewBox && mPreviewBox->getContent() == mPreview)
    {
        delete mTextPreview;
        mTextPreview = nullptr;
    }
    else
    {
        delete mPreview;
        mPreview = nullptr;
    }
}

void Setup_Colors::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "slider_grad")
    {
        updateGradType();
        updateColor();
        return;
    }

    if (eventId == "slider_graddelay")
    {
        mGradDelayText->setText(toString(
            std::floor(mGradDelaySlider->getValue())));
        updateColor();
        return;
    }
    if (eventId == "slider_red")
    {
        mRedText->setText(toString(std::floor(mRedSlider->getValue())));
        updateColor();
        return;
    }
    if (eventId == "slider_green")
    {
        mGreenText->setText(toString(std::floor(mGreenSlider->getValue())));
        updateColor();
        return;
    }
    if (eventId == "slider_blue")
    {
        mBlueText->setText(toString(std::floor(mBlueSlider->getValue())));
        updateColor();
        return;
    }
}

void Setup_Colors::valueChanged(const SelectionEvent &event A_UNUSED)
{
    if (!userPalette)
        return;

    mSelected = mColorBox->getSelected();
    const int type = userPalette->getColorTypeAt(mSelected);
    const Color *col = &userPalette->getColor(type);
    const Palette::GradientType grad = userPalette->getGradientType(type);
    const int delay = userPalette->getGradientDelay(type);

    mPreview->clearRows();
    mPreviewBox->setContent(mTextPreview);
    mTextPreview->setFont(boldFont);
    mTextPreview->setTextColor(col);
    mTextPreview->setTextBGColor(nullptr);
    mTextPreview->setOpaque(false);
    mTextPreview->setShadow(true);
    mTextPreview->setOutline(true);
    mTextPreview->useTextAlpha(false);

    switch (type)
    {
        case UserPalette::AIR_COLLISION_HIGHLIGHT:
        case UserPalette::WATER_COLLISION_HIGHLIGHT:
        case UserPalette::GROUNDTOP_COLLISION_HIGHLIGHT:
        case UserPalette::COLLISION_HIGHLIGHT:
        case UserPalette::PORTAL_HIGHLIGHT:
        case UserPalette::HOME_PLACE:
        case UserPalette::ROAD_POINT:
        case UserPalette::NET:
            mTextPreview->setBGColor(col);
            mTextPreview->setOpaque(true);
            mTextPreview->setOutline(false);
            mTextPreview->setShadow(false);
            break;
        case UserPalette::ATTACK_RANGE_BORDER:
        case UserPalette::HOME_PLACE_BORDER:
            if (gui)
                mTextPreview->setFont(gui->getFont());
            mTextPreview->setTextColor(col);
            mTextPreview->setOutline(false);
            mTextPreview->setShadow(false);
            break;
        case UserPalette::PARTICLE:
        case UserPalette::EXP_INFO:
        case UserPalette::PICKUP_INFO:
        case UserPalette::HIT_PLAYER_MONSTER:
        case UserPalette::HIT_MONSTER_PLAYER:
        case UserPalette::HIT_CRITICAL:
        case UserPalette::MISS:
        case UserPalette::HIT_LOCAL_PLAYER_MONSTER:
        case UserPalette::HIT_LOCAL_PLAYER_CRITICAL:
        case UserPalette::HIT_LOCAL_PLAYER_MISS:
        case UserPalette::ATTACK_RANGE:
        case UserPalette::MONSTER_ATTACK_RANGE:
        case UserPalette::FLOOR_ITEM_TEXT:
            mTextPreview->setShadow(false);
            break;
        default:
            break;
    }

    switch (type)
    {
        case UserPalette::PORTAL_HIGHLIGHT:
        case UserPalette::ATTACK_RANGE:
        case UserPalette::ATTACK_RANGE_BORDER:
        case UserPalette::MONSTER_ATTACK_RANGE:
        case UserPalette::HOME_PLACE:
        case UserPalette::HOME_PLACE_BORDER:
        case UserPalette::AIR_COLLISION_HIGHLIGHT:
        case UserPalette::WATER_COLLISION_HIGHLIGHT:
        case UserPalette::GROUNDTOP_COLLISION_HIGHLIGHT:
        case UserPalette::COLLISION_HIGHLIGHT:
        case UserPalette::WALKABLE_HIGHLIGHT:
        case UserPalette::ROAD_POINT:
        case UserPalette::MONSTER_HP:
        case UserPalette::MONSTER_HP2:
        case UserPalette::PLAYER_HP:
        case UserPalette::PLAYER_HP2:
        case UserPalette::FLOOR_ITEM_TEXT:
        case UserPalette::NET:
            // TRANSLATORS: colors tab. label.
            mGradDelayLabel->setCaption(_("Alpha:"));
            mGradDelayText->setRange(0, 255);
            mGradDelaySlider->setScale(0, 255);
            break;
        default:
            // TRANSLATORS: colors tab. label.
            mGradDelayLabel->setCaption(_("Delay:"));
            mGradDelayText->setRange(20, 100);
            mGradDelaySlider->setScale(20, 100);
            break;
    }
    if (grad != Palette::STATIC && grad != Palette::PULSE)
    { // If nonstatic color, don't display the current, but the committed
      // color at the sliders
        col = &userPalette->getCommittedColor(type);
    }
    else if (grad == Palette::PULSE)
    {
        col = &userPalette->getTestColor(type);
    }

    setEntry(mGradDelaySlider, mGradDelayText, delay);
    setEntry(mRedSlider, mRedText, col->r);
    setEntry(mGreenSlider, mGreenText, col->g);
    setEntry(mBlueSlider, mBlueText, col->b);

    mGradTypeSlider->setValue2(grad);
    updateGradType();
    mGradTypeSlider->setEnabled(true);
}

void Setup_Colors::setEntry(Slider *const s, TextField *const t,
                            const int value)
{
    if (s)
        s->setValue2(value);
    if (t)
        t->setText(toString(value));
}

void Setup_Colors::apply()
{
    if (userPalette)
        userPalette->commit();
}

void Setup_Colors::cancel()
{
    if (!userPalette)
        return;

    userPalette->rollback();
    const int type = userPalette->getColorTypeAt(mSelected);
    const Color *const col = &userPalette->getColor(type);
    mGradTypeSlider->setValue2(userPalette->getGradientType(type));
    const int delay = userPalette->getGradientDelay(type);
    setEntry(mGradDelaySlider, mGradDelayText, delay);
    setEntry(mRedSlider, mRedText, col->r);
    setEntry(mGreenSlider, mGreenText, col->g);
    setEntry(mBlueSlider, mBlueText, col->b);
}

void Setup_Colors::updateGradType()
{
    if (mSelected == -1 || !userPalette)
        return;

    mSelected = mColorBox->getSelected();
    const int type = userPalette->getColorTypeAt(mSelected);
    const Palette::GradientType grad = userPalette->getGradientType(type);

    mGradTypeText->setCaption(
        // TRANSLATORS: color type
        (grad == Palette::STATIC) ? _("Static") :
        // TRANSLATORS: color type
        (grad == Palette::PULSE) ? _("Pulse") :
        // TRANSLATORS: color type
        (grad == Palette::RAINBOW) ? _("Rainbow") : _("Spectrum"));

    const bool enable = (grad == Palette::STATIC || grad == Palette::PULSE);
    const bool delayEnable = true;

    mGradDelayText->setEnabled(delayEnable);
    mGradDelaySlider->setEnabled(delayEnable);

    mRedText->setEnabled(enable);
    mRedSlider->setEnabled(enable);
    mGreenText->setEnabled(enable);
    mGreenSlider->setEnabled(enable);
    mBlueText->setEnabled(enable);
    mBlueSlider->setEnabled(enable);
}

void Setup_Colors::updateColor()
{
    if (mSelected == -1 || !userPalette)
        return;

    const int type = userPalette->getColorTypeAt(mSelected);
    const Palette::GradientType grad = static_cast<Palette::GradientType>(
            static_cast<int>(mGradTypeSlider->getValue()));
    const int delay = static_cast<int>(mGradDelaySlider->getValue());
    userPalette->setGradient(type, grad);
    userPalette->setGradientDelay(type, delay);

    if (grad == Palette::STATIC)
    {
        userPalette->setColor(type,
                static_cast<int>(mRedSlider->getValue()),
                static_cast<int>(mGreenSlider->getValue()),
                static_cast<int>(mBlueSlider->getValue()));
    }
    else if (grad == Palette::PULSE)
    {
        userPalette->setTestColor(type, Color(
                static_cast<int>(mRedSlider->getValue()),
                static_cast<int>(mGreenSlider->getValue()),
                static_cast<int>(mBlueSlider->getValue())));
    }
}
