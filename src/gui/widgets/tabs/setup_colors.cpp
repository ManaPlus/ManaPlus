/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/gui.h"
#include "gui/userpalette.h"

#include "gui/fonts/font.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/textpreview.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <cmath>

#include "debug.h"

const char *const Setup_Colors::rawmsg =
    // TRANSLATORS: color selection preview message
    N_("This is what the color looks like");

Setup_Colors::Setup_Colors(const Widget2 *const widget) :
    SetupTab(widget),
    SelectionListener(),
    mColorBox(CREATEWIDGETR(ListBox, this, userPalette, "")),
    mScroll(new ScrollArea(this, mColorBox,
        Opaque_true, "setup_colors_background.xml")),
    mPreview(new BrowserBox(this, Opaque_true,
        "browserbox.xml")),
    mTextPreview(new TextPreview(this, gettext(rawmsg))),
    mPreviewBox(new ScrollArea(this, mPreview, Opaque_true,
        "setup_colors_preview_background.xml")),
    mSelected(-1),
    // TRANSLATORS: colors tab. label.
    mGradTypeLabel(new Label(this, _("Type:"))),
    mGradTypeSlider(new Slider(this, 0.0, 3.0, 1.0)),
    mGradTypeText(new Label(this)),
    // TRANSLATORS: colors tab. label.
    mGradDelayLabel(new Label(this, _("Delay:"))),
    mGradDelaySlider(new Slider(this, 20.0, 100.0, 1.0)),
    mGradDelayText(new TextField(this)),
    // TRANSLATORS: colors tab. label.
    mRedLabel(new Label(this, _("Red:"))),
    mRedSlider(new Slider(this, 0.0, 255.0, 1.0)),
    mRedText(new TextField(this)),
    // TRANSLATORS: colors tab. label.
    mGreenLabel(new Label(this, _("Green:"))),
    mGreenSlider(new Slider(this, 0.0, 255.0, 1.0)),
    mGreenText(new TextField(this)),
    // TRANSLATORS: colors tab. label.
    mBlueLabel(new Label(this, _("Blue:"))),
    mBlueSlider(new Slider(this, 0.0, 255.0, 1.0)),
    mBlueText(new TextField(this))
{
    // TRANSLATORS: settings colors tab name
    setName(_("Colors"));
    mColorBox->addSelectionListener(this);
    mScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mPreview->setOpaque(Opaque_false);

    // don't do anything with links
    mPreview->setLinkHandler(nullptr);

    mPreviewBox->setHeight(20);
    mPreviewBox->setScrollPolicy(ScrollArea::SHOW_NEVER,
        ScrollArea::SHOW_NEVER);

    mGradTypeSlider->setWidth(180);
    mGradTypeSlider->setActionEventId("slider_grad");
    mGradTypeSlider->setValue(0);
    mGradTypeSlider->addActionListener(this);
    mGradTypeSlider->setEnabled(false);

    // TRANSLATORS: color type
    std::string longText = _("Static");

    const Font *const font = getFont();
    // TRANSLATORS: color type
    if (getFont()->getWidth(_("Pulse")) > font->getWidth(longText))
    {
        // TRANSLATORS: color type
        longText = _("Pulse");
    }
    // TRANSLATORS: color type
    if (getFont()->getWidth(_("Rainbow")) > font->getWidth(longText))
    {
        // TRANSLATORS: color type
        longText = _("Rainbow");
    }
    // TRANSLATORS: color type
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
    mGradDelaySlider->setValue(mGradDelayText->getValue());
    mGradDelaySlider->setActionEventId("slider_graddelay");
    mGradDelaySlider->addActionListener(this);
    mGradDelaySlider->setEnabled(false);

    mRedText->setWidth(40);
    mRedText->setRange(0, 255);
    mRedText->setNumeric(true);
    mRedText->setEnabled(false);

    mRedSlider->setWidth(180);
    mRedSlider->setValue(mRedText->getValue());
    mRedSlider->setActionEventId("slider_red");
    mRedSlider->addActionListener(this);
    mRedSlider->setEnabled(false);

    mGreenText->setWidth(40);
    mGreenText->setRange(0, 255);
    mGreenText->setNumeric(true);
    mGreenText->setEnabled(false);

    mGreenSlider->setWidth(180);
    mGreenSlider->setValue(mGreenText->getValue());
    mGreenSlider->setActionEventId("slider_green");
    mGreenSlider->addActionListener(this);
    mGreenSlider->setEnabled(false);

    mBlueText->setWidth(40);
    mBlueText->setRange(0, 255);
    mBlueText->setNumeric(true);
    mBlueText->setEnabled(false);

    mBlueSlider->setWidth(180);
    mBlueSlider->setValue(mBlueText->getValue());
    mBlueSlider->setActionEventId("slider_blue");
    mBlueSlider->addActionListener(this);
    mBlueSlider->setEnabled(false);

    setOpaque(Opaque_false);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mScroll, 6, 6).setPadding(2);
    place(0, 6, mPreviewBox, 6, 1).setPadding(2);
    place(0, 7, mGradTypeLabel, 3, 1);
    place(3, 7, mGradTypeSlider, 1, 1);
    place(4, 7, mGradTypeText, 2, 1).setPadding(1);
    place(0, 8, mRedLabel, 3, 1);
    place(3, 8, mRedSlider, 1, 1);
    place(5, 8, mRedText, 1, 1).setPadding(1);
    place(0, 9, mGreenLabel, 3, 1);
    place(3, 9, mGreenSlider, 1, 1);
    place(5, 9, mGreenText, 1, 1).setPadding(1);
    place(0, 10, mBlueLabel, 3, 1);
    place(3, 10, mBlueSlider, 1, 1);
    place(5, 10, mBlueText, 1, 1).setPadding(1);
    place(0, 11, mGradDelayLabel, 3, 1);
    place(3, 11, mGradDelaySlider, 1, 1);
    place(5, 11, mGradDelayText, 1, 1).setPadding(1);

    mGradTypeText->setCaption("");

    setDimension(Rect(0, 0, 365, 350));
}

Setup_Colors::~Setup_Colors()
{
    if ((mPreviewBox != nullptr) && mPreviewBox->getContent() == mPreview)
        delete2(mTextPreview)
    else
        delete2(mPreview)
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
    if (userPalette == nullptr)
        return;

    mSelected = mColorBox->getSelected();
    const UserColorIdT type = static_cast<UserColorIdT>(
        userPalette->getColorTypeAt(mSelected));
    const Color *col = &userPalette->getColor(type, 255U);
    const GradientTypeT grad = userPalette->getGradientType(type);
    const int delay = userPalette->getGradientDelay(type);
    const Visible showControls = fromBool(grad != GradientType::LABEL,
        Visible);
    mPreview->setVisible(showControls);
    mPreviewBox->setVisible(showControls);
    mTextPreview->setVisible(showControls);
    mGradTypeLabel->setVisible(showControls);
    mGradTypeSlider->setVisible(showControls);
    mGradTypeText->setVisible(showControls);
    mGradDelayLabel->setVisible(showControls);
    mGradDelaySlider->setVisible(showControls);
    mGradDelayText->setVisible(showControls);
    mRedLabel->setVisible(showControls);
    mRedSlider->setVisible(showControls);
    mRedText->setVisible(showControls);
    mGreenLabel->setVisible(showControls);
    mGreenSlider->setVisible(showControls);
    mGreenText->setVisible(showControls);
    mBlueLabel->setVisible(showControls);
    mBlueSlider->setVisible(showControls);
    mBlueText->setVisible(showControls);

    mPreview->clearRows();
    mPreviewBox->setContent(mTextPreview);
    mTextPreview->setFont(boldFont);
    mTextPreview->setTextColor(col);
    mTextPreview->setTextBGColor(nullptr);
    mTextPreview->setOpaque(Opaque_false);
    mTextPreview->setShadow(true);
    mTextPreview->setOutline(true);
    mTextPreview->useTextAlpha(false);

// probably need combite both switches and add all mssing color ids.

    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (type)
    {
        case UserColorId::AIR_COLLISION_HIGHLIGHT:
        case UserColorId::WATER_COLLISION_HIGHLIGHT:
        case UserColorId::MONSTER_COLLISION_HIGHLIGHT:
        case UserColorId::GROUNDTOP_COLLISION_HIGHLIGHT:
        case UserColorId::COLLISION_HIGHLIGHT:
        case UserColorId::PORTAL_HIGHLIGHT:
        case UserColorId::HOME_PLACE:
        case UserColorId::ROAD_POINT:
        case UserColorId::NET:
            mTextPreview->setBGColor(col);
            mTextPreview->setOpaque(Opaque_true);
            mTextPreview->setOutline(false);
            mTextPreview->setShadow(false);
            break;
        case UserColorId::ATTACK_RANGE_BORDER:
        case UserColorId::HOME_PLACE_BORDER:
            if (gui != nullptr)
                mTextPreview->setFont(gui->getFont());
            mTextPreview->setTextColor(col);
            mTextPreview->setOutline(false);
            mTextPreview->setShadow(false);
            break;
        case UserColorId::PARTICLE:
        case UserColorId::EXP_INFO:
        case UserColorId::PICKUP_INFO:
        case UserColorId::HIT_PLAYER_MONSTER:
        case UserColorId::HIT_MONSTER_PLAYER:
        case UserColorId::HIT_CRITICAL:
        case UserColorId::MISS:
        case UserColorId::HIT_LOCAL_PLAYER_MONSTER:
        case UserColorId::HIT_LOCAL_PLAYER_CRITICAL:
        case UserColorId::HIT_LOCAL_PLAYER_MISS:
        case UserColorId::ATTACK_RANGE:
        case UserColorId::MONSTER_ATTACK_RANGE:
        case UserColorId::FLOOR_ITEM_TEXT:
        case UserColorId::SKILL_ATTACK_RANGE:
            mTextPreview->setShadow(false);
            break;
        default:
            break;
    }

    switch (type)
    {
        case UserColorId::PORTAL_HIGHLIGHT:
        case UserColorId::ATTACK_RANGE:
        case UserColorId::ATTACK_RANGE_BORDER:
        case UserColorId::MONSTER_ATTACK_RANGE:
        case UserColorId::SKILL_ATTACK_RANGE:
        case UserColorId::HOME_PLACE:
        case UserColorId::HOME_PLACE_BORDER:
        case UserColorId::AIR_COLLISION_HIGHLIGHT:
        case UserColorId::WATER_COLLISION_HIGHLIGHT:
        case UserColorId::MONSTER_COLLISION_HIGHLIGHT:
        case UserColorId::GROUNDTOP_COLLISION_HIGHLIGHT:
        case UserColorId::COLLISION_HIGHLIGHT:
        case UserColorId::WALKABLE_HIGHLIGHT:
        case UserColorId::ROAD_POINT:
        case UserColorId::MONSTER_HP:
        case UserColorId::MONSTER_HP2:
        case UserColorId::HOMUN_HP:
        case UserColorId::HOMUN_HP2:
        case UserColorId::MERC_HP:
        case UserColorId::MERC_HP2:
        case UserColorId::ELEMENTAL_HP:
        case UserColorId::ELEMENTAL_HP2:
        case UserColorId::PLAYER_HP:
        case UserColorId::PLAYER_HP2:
        case UserColorId::FLOOR_ITEM_TEXT:
        case UserColorId::NET:
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
    PRAGMA45(GCC diagnostic pop)
    if (grad != GradientType::STATIC && grad != GradientType::PULSE)
    { // If nonstatic color, don't display the current, but the committed
      // color at the sliders
        col = &userPalette->getCommittedColor(type);
    }
    else if (grad == GradientType::PULSE)
    {
        col = &userPalette->getTestColor(type);
    }

    setEntry(mGradDelaySlider, mGradDelayText, delay);
    setEntry(mRedSlider, mRedText, col->r);
    setEntry(mGreenSlider, mGreenText, col->g);
    setEntry(mBlueSlider, mBlueText, col->b);

    mGradTypeSlider->setValue(CAST_S32(grad));
    updateGradType();
    mGradTypeSlider->setEnabled(true);
}

void Setup_Colors::setEntry(Slider *const s, TextField *const t,
                            const int value)
{
    if (s != nullptr)
        s->setValue(value);
    if (t != nullptr)
        t->setText(toString(value));
}

void Setup_Colors::apply()
{
    if (userPalette != nullptr)
        userPalette->commit();
}

void Setup_Colors::cancel()
{
    if (userPalette == nullptr)
        return;

    userPalette->rollback();
    const UserColorIdT type = static_cast<UserColorIdT>(
        userPalette->getColorTypeAt(mSelected));
    const Color *const col = &userPalette->getColor(type, 255U);
    mGradTypeSlider->setValue(CAST_S32(
        userPalette->getGradientType(type)));
    const int delay = userPalette->getGradientDelay(type);
    setEntry(mGradDelaySlider, mGradDelayText, delay);
    setEntry(mRedSlider, mRedText, col->r);
    setEntry(mGreenSlider, mGreenText, col->g);
    setEntry(mBlueSlider, mBlueText, col->b);
}

void Setup_Colors::updateGradType()
{
    if (mSelected == -1 || (userPalette == nullptr))
        return;

    mSelected = mColorBox->getSelected();
    const UserColorIdT type = static_cast<UserColorIdT>(
        userPalette->getColorTypeAt(mSelected));
    const GradientTypeT grad = userPalette->getGradientType(type);

    mGradTypeText->setCaption(
        // TRANSLATORS: color type
        (grad == GradientType::STATIC) ? _("Static") :
        // TRANSLATORS: color type
        (grad == GradientType::PULSE) ? _("Pulse") :
        // TRANSLATORS: color type
        (grad == GradientType::RAINBOW) ? _("Rainbow") : _("Spectrum"));

    const bool enable = (grad == GradientType::STATIC ||
        grad == GradientType::PULSE);
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

void Setup_Colors::updateColor() const
{
    if (mSelected == -1 || (userPalette == nullptr))
        return;

    const UserColorIdT type = static_cast<UserColorIdT>(
        userPalette->getColorTypeAt(mSelected));
    const GradientTypeT grad = static_cast<GradientTypeT>(
        CAST_S32(mGradTypeSlider->getValue()));
    const int delay = CAST_S32(mGradDelaySlider->getValue());
    userPalette->setGradient(type, grad);
    userPalette->setGradientDelay(type, delay);

    if (grad == GradientType::STATIC)
    {
        userPalette->setColor(type,
            CAST_S32(mRedSlider->getValue()),
            CAST_S32(mGreenSlider->getValue()),
            CAST_S32(mBlueSlider->getValue()));
    }
    else if (grad == GradientType::PULSE)
    {
        userPalette->setTestColor(type, Color(
            CAST_S32(mRedSlider->getValue()),
            CAST_S32(mGreenSlider->getValue()),
            CAST_S32(mBlueSlider->getValue()),
            255U));
    }
}
