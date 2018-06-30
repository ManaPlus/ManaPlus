/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/windows/charcreatedialog.h"

#include "configuration.h"

#include "enums/being/beingdirection.h"

#include "being/being.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/windows/charselectdialog.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/tabstrip.h"
#include "gui/widgets/textfield.h"

#include "net/charserverhandler.h"
#include "net/serverfeatures.h"

#include "resources/iteminfo.h"

#include "resources/db/chardb.h"
#include "resources/db/colordb.h"
#include "resources/db/itemdb.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

#undef ERROR

static const BeingActionT actions[] =
{
    BeingAction::STAND,
    BeingAction::SIT,
    BeingAction::MOVE,
    BeingAction::ATTACK,
    BeingAction::DEAD
};

static const uint8_t directions[] =
{
    BeingDirection::DOWN,
    BeingDirection::RIGHT,
    BeingDirection::UP,
    BeingDirection::LEFT
};

CharCreateDialog::CharCreateDialog(CharSelectDialog *const parent,
                                   const int slot) :
    // TRANSLATORS: char create dialog name
    Window(_("New Character"), Modal_true, parent, "charcreate.xml"),
    ActionListener(),
    KeyListener(),
    mCharSelectDialog(parent),
    mNameField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    // TRANSLATORS: char create dialog label
    mNameLabel(new Label(this, _("Name:"))),
    mNextHairColorButton(nullptr),
    mPrevHairColorButton(nullptr),
    mHairColorLabel(nullptr),
    mHairColorNameLabel(nullptr),
    mNextHairStyleButton(nullptr),
    mPrevHairStyleButton(nullptr),
    mHairStyleLabel(nullptr),
    mHairStyleNameLabel(nullptr),
    mNextRaceButton(nullptr),
    mPrevRaceButton(nullptr),
    mRaceLabel(nullptr),
    mRaceNameLabel(nullptr),
    mNextLookButton(nullptr),
    mPrevLookButton(nullptr),
    mLookLabel(nullptr),
    mLookNameLabel(nullptr),
    // TRANSLATORS: char create dialog button
    mActionButton(new Button(this, _("^"), "action", BUTTON_SKIN, this)),
    // TRANSLATORS: char create dialog button
    mRotateButton(new Button(this, _(">"), "rotate", BUTTON_SKIN, this)),
    mAttributeSlider(),
    mAttributeLabel(),
    mAttributeValue(),
    mAttributesLeft(new Label(this,
        // TRANSLATORS: char create dialog label
        strprintf(_("Please distribute %d points"), 99))),
    // TRANSLATORS: char create dialog button
    mCreateButton(new Button(this, _("Create"), "create", BUTTON_SKIN, this)),
    // TRANSLATORS: char create dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", BUTTON_SKIN, this)),
    mPlayer(Being::createBeing(BeingId_zero,
            ActorType::Player,
            BeingTypeId_zero,
            nullptr)),
    mPlayerBox(new PlayerBox(this, mPlayer, "charcreate_playerbox.xml",
        "charcreate_selectedplayerbox.xml")),
    mGenderStrip(nullptr),
    mMaxPoints(0),
    mUsedPoints(0),
    mRace(CharDB::getMinRace()),
    mLook(0),
    mMinLook(CharDB::getMinLook()),
    mMaxLook(CharDB::getMaxLook()),
    mMinRace(CharDB::getMinRace()),
    mMaxRace(CharDB::getMaxRace()),
    mHairStyle(0),
    mHairColor(0),
    mMaxY(0U),
    mSlot(slot),
    mDefaultGender(Gender::FEMALE),
    mGender(Gender::UNSPECIFIED),
    maxHairColor(CharDB::getMaxHairColor()),
    minHairColor(CharDB::getMinHairColor()),
    maxHairStyle(CharDB::getMaxHairStyle()),
    minHairStyle(CharDB::getMinHairStyle()),
    mAction(0),
    mDirection(0)
{
    setStickyButtonLock(true);
    setSticky(true);
    setWindowName("NewCharacter");

    const int w = 480;
    const int h = 350;
    setContentSize(w, h);

    mPlayer->setGender(Gender::MALE);
    const STD_VECTOR<BeingSlot> &items = CharDB::getDefaultItems();
    int i = 1;
    for (STD_VECTOR<BeingSlot>::const_iterator it = items.begin(),
         it_fend = items.end();
         it != it_fend;
         ++ it, i ++)
    {
        const BeingSlot &beingSlot = *it;
        mPlayer->setSpriteCards(i,
            beingSlot.spriteId,
            beingSlot.cardsId);
    }

    if (maxHairColor == 0u)
        maxHairColor = ColorDB::getHairSize();
    if (maxHairStyle == 0u)
        maxHairStyle = ItemDB::getNumOfHairstyles();

    if (maxHairStyle != 0u)
    {
        mHairStyle = (CAST_U32(rand())
            % maxHairStyle) + minHairStyle;
    }
    else
    {
        mHairStyle = 0;
    }
    if (maxHairColor != 0u)
    {
        mHairColor = (CAST_U32(rand())
            % maxHairColor) + minHairColor;
    }
    else
    {
        mHairColor = 0;
    }

    mNameField->setMaximum(24);

    if (maxHairColor > minHairColor)
    {
        mNextHairColorButton = new Button(this,
            // TRANSLATORS: This is a narrow symbol used to denote 'next'.
            // You may change this symbol if your language uses another.
            // TRANSLATORS: char create dialog button
            _(">"),
            "nextcolor",
            BUTTON_SKIN,
            this);
        mPrevHairColorButton = new Button(this,
            // TRANSLATORS: This is a narrow symbol used to denote 'previous'.
            // You may change this symbol if your language uses another.
            // TRANSLATORS: char create dialog button
            _("<"),
            "prevcolor",
             BUTTON_SKIN,
            this);
        // TRANSLATORS: char create dialog label
        mHairColorLabel = new Label(this, _("Hair color:"));
        mHairColorNameLabel = new Label(this, "");
    }

    if (maxHairStyle > minHairStyle)
    {
        mNextHairStyleButton = new Button(this,
            // TRANSLATORS: char create dialog button
            _(">"),
            "nextstyle",
            BUTTON_SKIN,
            this);
        mPrevHairStyleButton = new Button(this,
            // TRANSLATORS: char create dialog button
            _("<"),
            "prevstyle",
            BUTTON_SKIN,
            this);
        // TRANSLATORS: char create dialog label
        mHairStyleLabel = new Label(this, _("Hair style:"));
        mHairStyleNameLabel = new Label(this, "");
    }

    if (serverFeatures->haveRaceSelection() && mMinRace < mMaxRace)
    {
        mNextRaceButton = new Button(this,
            // TRANSLATORS: char create dialog button
            _(">"),
            "nextrace",
            BUTTON_SKIN,
            this);
        mPrevRaceButton = new Button(this,
            // TRANSLATORS: char create dialog button
            _("<"),
            "prevrace",
            BUTTON_SKIN,
            this);
        // TRANSLATORS: char create dialog label
        mRaceLabel = new Label(this, _("Race:"));
        mRaceNameLabel = new Label(this, "");
    }
    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
    {
        mNextLookButton = new Button(this,
            // TRANSLATORS: char create dialog button
            _(">"),
            "nextlook",
            BUTTON_SKIN,
            this);
        mPrevLookButton = new Button(this,
            // TRANSLATORS: char create dialog button
            _("<"),
            "prevlook",
            BUTTON_SKIN,
            this);
        // TRANSLATORS: char create dialog label
        mLookLabel = new Label(this, _("Look:"));
        mLookNameLabel = new Label(this, "");
    }

    if (serverFeatures->haveCreateCharGender())
    {
        const int forceGender = features.getIntValue("forceCharGender");
        if (forceGender == -1)
        {
            const int size = config.getIntValue("fontSize");
            mGenderStrip = new TabStrip(this,
                "gender_" + getWindowName(),
                size + 16,
                0);
            mGenderStrip->setPressFirst(false);
            mGenderStrip->addActionListener(this);
            mGenderStrip->setActionEventId("gender_");
            // TRANSLATORS: one char size female character gender
            mGenderStrip->addButton(_("F"), "f", false);
            if (features.getIntValue("forceAccountGender") == -1)
            {
                if (serverFeatures->haveCharHiddenGender())
                {
                    // TRANSLATORS: one char size male character gender
                    mGenderStrip->addButton(_("M"), "m", true);
                    // TRANSLATORS: one char size hidden character gender
                    mGenderStrip->addButton(_("H"), "h", false);
                }
                else
                {
                    // TRANSLATORS: one char size male character gender
                    mGenderStrip->addButton(_("M"), "m", false);
                    // TRANSLATORS: one char size unknown character gender
                    mGenderStrip->addButton(_("U"), "u", true);
                }
            }
            else
            {
                // TRANSLATORS: one char size male character gender
                mGenderStrip->addButton(_("M"), "m", true);
                if (serverFeatures->haveCharHiddenGender())
                {
                    // TRANSLATORS: one char size hidden character gender
                    mGenderStrip->addButton(_("H"), "h", false);
                }
            }

            mGenderStrip->setVisible(Visible_true);
            add(mGenderStrip);

            mGenderStrip->setPosition(385, 130);
            mGenderStrip->setWidth(500);
            mGenderStrip->setHeight(50);
        }
        else
        {
            mGender = Being::intToGender(CAST_U8(forceGender));
            mPlayer->setGender(mGender);
        }
    }

    mPlayerBox->setWidth(74);

    mNameField->setActionEventId("create");
    mNameField->addActionListener(this);

    mPlayerBox->setDimension(Rect(360, 0, 110, 90));
    mActionButton->setPosition(385, 100);
    mRotateButton->setPosition(415, 100);

    mNameLabel->setPosition(mPadding, 2);
    mNameField->setDimension(Rect(60, 2,
        300, mNameField->getHeight()));

    const uint32_t labelPadding = getOption("labelPadding", 2);
    const uint32_t leftX = 120 + mPadding;
    const uint32_t rightX = 300 + mPadding;
    const uint32_t labelX = mPadding;
    uint32_t nameX = leftX + labelPadding;
    uint32_t y = 30;
    if (mPrevHairColorButton != nullptr)
        nameX += mPrevHairColorButton->getWidth();
    else if (mPrevHairStyleButton != nullptr)
        nameX += mPrevHairStyleButton->getWidth();
    else if (mPrevLookButton != nullptr)
        nameX += mPrevLookButton->getWidth();
    else if (mPrevRaceButton != nullptr)
        nameX += mPrevRaceButton->getWidth();

    if (maxHairColor > minHairColor)
    {
        mPrevHairColorButton->setPosition(leftX, y);
        mNextHairColorButton->setPosition(rightX, y);
        y += 5;
        mHairColorLabel->setPosition(labelX, y);
        mHairColorNameLabel->setPosition(nameX, y);
        y += 24;
    }
    if (maxHairStyle > minHairStyle)
    {
        mPrevHairStyleButton->setPosition(leftX, y);
        mNextHairStyleButton->setPosition(rightX, y);
        y += 5;
        mHairStyleLabel->setPosition(labelX, y);
        mHairStyleNameLabel->setPosition(nameX, y);
        y += 24;
    }

    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
    {
        if (mPrevLookButton != nullptr)
            mPrevLookButton->setPosition(leftX, y);
        if (mNextLookButton != nullptr)
            mNextLookButton->setPosition(rightX, y);
        y += 5;
        if (mLookLabel != nullptr)
            mLookLabel->setPosition(labelX, y);
        if (mLookNameLabel != nullptr)
            mLookNameLabel->setPosition(nameX, y);  // 93
        y += 24;
    }
    if (serverFeatures->haveRaceSelection() && mMinRace < mMaxRace)
    {
        if (mPrevRaceButton != nullptr)
            mPrevRaceButton->setPosition(leftX, y);
        if (mNextRaceButton != nullptr)
            mNextRaceButton->setPosition(rightX, y);
        y += 5;
        if (mRaceLabel != nullptr)
            mRaceLabel->setPosition(labelX, y);
        if (mRaceNameLabel != nullptr)
            mRaceNameLabel->setPosition(nameX, y);
    }
    mMaxY = y + 29 + getTitlePadding();

    updateSliders();
    setButtonsPosition(w, h);

    add(mPlayerBox);
    add(mNameField);
    add(mNameLabel);

    if (maxHairColor > minHairColor)
    {
        add(mNextHairColorButton);
        add(mPrevHairColorButton);
        add(mHairColorLabel);
        add(mHairColorNameLabel);
    }

    if (maxHairStyle > minHairStyle)
    {
        add(mNextHairStyleButton);
        add(mPrevHairStyleButton);
        add(mHairStyleLabel);
        add(mHairStyleNameLabel);
    }
    add(mActionButton);
    add(mRotateButton);

    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
    {
        add(mNextLookButton);
        add(mPrevLookButton);
        add(mLookLabel);
        add(mLookNameLabel);
    }

    if (serverFeatures->haveRaceSelection() && mMinRace < mMaxRace)
    {
        add(mNextRaceButton);
        add(mPrevRaceButton);
        add(mRaceLabel);
        add(mRaceNameLabel);
    }

    add(mAttributesLeft);
    add(mCreateButton);
    add(mCancelButton);

    center();
    setVisible(Visible_true);
    mNameField->requestFocus();

    updateHair();
    if (serverFeatures->haveRaceSelection() && mMinRace < mMaxRace)
        updateRace();
    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
        updateLook();
    updatePlayer();

    addKeyListener(this);
}

CharCreateDialog::~CharCreateDialog()
{
    delete2(mPlayer);

    if (charServerHandler != nullptr)
        charServerHandler->setCharCreateDialog(nullptr);
}

void CharCreateDialog::action(const ActionEvent &event)
{
    const std::string &id = event.getId();
    if (id == "create")
    {
        if (getName().length() >= 4)
        {
            // Attempt to create the character
            mCreateButton->setEnabled(false);

            STD_VECTOR<int> atts;
            const size_t sz = mAttributeSlider.size();
            for (size_t i = 0; i < sz; i++)
            {
                atts.push_back(CAST_S32(
                    mAttributeSlider[i]->getValue()));
            }
            for (size_t i = sz; i < 6; i ++)
                atts.push_back(1);

            const int characterSlot = mSlot;

            charServerHandler->newCharacter(getName(),
                characterSlot,
                mGender,
                mHairStyle,
                mHairColor,
                CAST_U8(mRace),
                CAST_U8(mLook),
                atts);
        }
        else
        {
            CREATEWIDGET(OkDialog,
                // TRANSLATORS: char creation error
                _("Error"),
                // TRANSLATORS: char creation error
                _("Your name needs to be at least 4 characters."),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
        }
    }
    else if (id == "cancel")
    {
        scheduleDelete();
    }
    else if (id == "nextcolor")
    {
        mHairColor ++;
        updateHair();
    }
    else if (id == "prevcolor")
    {
        mHairColor --;
        updateHair();
    }
    else if (id == "nextstyle")
    {
        mHairStyle ++;
        updateHair();
    }
    else if (id == "prevstyle")
    {
        mHairStyle --;
        updateHair();
    }
    else if (id == "nextrace")
    {
        mRace ++;
        updateRace();
    }
    else if (id == "prevrace")
    {
        mRace --;
        updateRace();
    }
    else if (id == "nextlook")
    {
        mLook ++;
        updateLook();
    }
    else if (id == "prevlook")
    {
        mLook --;
        updateLook();
    }
    else if (id == "statslider")
    {
        updateSliders();
    }
    else if (id == "action")
    {
        mAction ++;
        if (mAction >= 5)
            mAction = 0;
        updatePlayer();
    }
    else if (id == "rotate")
    {
        mDirection ++;
        if (mDirection >= 4)
            mDirection = 0;
        updatePlayer();
    }
    else if (id == "gender_m")
    {
        mGender = Gender::MALE;
        mPlayer->setGender(Gender::MALE);
    }
    else if (id == "gender_f")
    {
        mGender = Gender::FEMALE;
        mPlayer->setGender(Gender::FEMALE);
    }
    else if (id == "gender_u")
    {
        mGender = Gender::UNSPECIFIED;
        mPlayer->setGender(mDefaultGender);
    }
    else if (id == "gender_h")
    {
        mGender = Gender::HIDDEN;
        mPlayer->setGender(Gender::HIDDEN);
    }
}

std::string CharCreateDialog::getName() const
{
    std::string name = mNameField->getText();
    trim(name);
    return name;
}

void CharCreateDialog::updateSliders()
{
    for (size_t i = 0, sz = mAttributeSlider.size(); i < sz; i++)
    {
        // Update captions
        mAttributeValue[i]->setCaption(
                toString(CAST_S32(mAttributeSlider[i]->getValue())));
        mAttributeValue[i]->adjustSize();
    }

    // Update distributed points
    const int pointsLeft = mMaxPoints - getDistributedPoints();
    if (pointsLeft == 0)
    {
        // TRANSLATORS: char create dialog label
        mAttributesLeft->setCaption(_("Character stats OK"));
        mCreateButton->setEnabled(true);
    }
    else
    {
        mCreateButton->setEnabled(false);
        if (pointsLeft > 0)
        {
            mAttributesLeft->setCaption(
                // TRANSLATORS: char create dialog label
                strprintf(_("Please distribute %d points"), pointsLeft));
        }
        else
        {
            mAttributesLeft->setCaption(
                // TRANSLATORS: char create dialog label
                strprintf(_("Please remove %d points"), -pointsLeft));
        }
    }

    mAttributesLeft->adjustSize();
}

void CharCreateDialog::unlock()
{
    mCreateButton->setEnabled(true);
}

int CharCreateDialog::getDistributedPoints() const
{
    int points = 0;

    for (size_t i = 0, sz = mAttributeSlider.size(); i < sz; i++)
        points += CAST_S32(mAttributeSlider[i]->getValue());
    return points;
}

void CharCreateDialog::setAttributes(const StringVect &labels,
                                     int available,
                                     const int min, const int max)
{
    size_t sz;

    if (min == max || available == 0)
    {
        sz = 0U;
        available = 0;
    }
    else
    {
        sz = labels.size();
    }

    mMaxPoints = available;

    for (size_t i = 0; i < mAttributeLabel.size(); i++)
    {
        remove(mAttributeLabel[i]);
        delete2(mAttributeLabel[i])
        remove(mAttributeSlider[i]);
        delete2(mAttributeSlider[i])
        remove(mAttributeValue[i]);
        delete2(mAttributeValue[i])
    }

    mAttributeLabel.resize(sz);
    mAttributeSlider.resize(sz);
    mAttributeValue.resize(sz);

    const uint32_t w = 480;
    uint32_t h = 350;
    uint32_t y = 89;
    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
        y += 29;
    if (serverFeatures->haveRaceSelection() && mMinRace < mMaxRace)
        y += 29;

    for (size_t i = 0; i < sz; i++)
    {
        mAttributeLabel[i] = new Label(this, labels[i]);
        mAttributeLabel[i]->setWidth(70);
        mAttributeLabel[i]->setPosition(mPadding,
            y + CAST_S32(i * 24));
        mAttributeLabel[i]->adjustSize();
        add(mAttributeLabel[i]);

        mAttributeSlider[i] = new Slider(this, min, max, 1.0);
        mAttributeSlider[i]->setDimension(Rect(140 + mPadding,
            y + CAST_S32(i * 24),
            150,
            12));
        mAttributeSlider[i]->setActionEventId("statslider");
        mAttributeSlider[i]->addActionListener(this);
        add(mAttributeSlider[i]);

        mAttributeValue[i] = new Label(this, toString(min));
        mAttributeValue[i]->setPosition(295 + mPadding,
            y + CAST_S32(i * 24));
        add(mAttributeValue[i]);
    }

    updateSliders();
    if (available == 0)
    {
        mAttributesLeft->setVisible(Visible_false);
        h = y;
    }
    else
    {
        h = y +
            CAST_S32(sz) * 24 +
            mAttributesLeft->getHeight() +
            getPadding();
    }
    if (serverFeatures->haveCreateCharGender() &&
        features.getIntValue("forceCharGender") == -1 &&
        y < 160)
    {
        if (h < 160)
            h = 160;
    }
    if (h < mMaxY)
        h = mMaxY;
    if (serverFeatures->haveCreateCharGender())
    {
        const int forceGender = features.getIntValue("forceCharGender");
        if (forceGender == -1 && h < 180)
            h = 180;
        else if (h < 120)
            h = 120;
    }
    h += mCreateButton->getHeight();

    setContentSize(w, h);
    setButtonsPosition(w, h);
}

void CharCreateDialog::setDefaultGender(const GenderT gender)
{
    if (features.getIntValue("forceCharGender") != -1)
        return;

    mDefaultGender = gender;
    mPlayer->setGender(gender);
}

void CharCreateDialog::updateHair()
{
    if (mHairStyle <= 0)
        mHairStyle = ItemDB::getNumOfHairstyles() - 1;
    else
        mHairStyle %= ItemDB::getNumOfHairstyles();
    if (mHairStyle < CAST_S32(minHairStyle)
        || mHairStyle > CAST_S32(maxHairStyle))
    {
        mHairStyle = minHairStyle;
    }
    const ItemInfo &item = ItemDB::get(-mHairStyle);
    if (mHairStyleNameLabel != nullptr)
    {
        mHairStyleNameLabel->setCaption(item.getName());
        mHairStyleNameLabel->resizeTo(150, 150);
    }

    if (ColorDB::getHairSize() != 0)
        mHairColor %= ColorDB::getHairSize();
    else
        mHairColor = 0;
    if (mHairColor < 0)
        mHairColor += ColorDB::getHairSize();
    if (mHairColor < CAST_S32(minHairColor)
        || mHairColor > CAST_S32(maxHairColor))
    {
        mHairColor = minHairColor;
    }
    if (mHairColorNameLabel != nullptr)
    {
        mHairColorNameLabel->setCaption(ColorDB::getHairColorName(
            fromInt(mHairColor, ItemColor)));
        mHairColorNameLabel->resizeTo(150, 150);
    }

    mPlayer->setSpriteColor(charServerHandler->hairSprite(),
        mHairStyle * -1,
        item.getDyeColorsString(fromInt(mHairColor, ItemColor)));
    updatePlayer();
}

void CharCreateDialog::updateRace()
{
    if (mRace < mMinRace)
        mRace = mMaxRace;
    else if (mRace > mMaxRace)
        mRace = mMinRace;

    updateLook();
}

void CharCreateDialog::updateLook()
{
    const ItemInfo &item = ItemDB::get(-100 - mRace);
    const int sz = item.getColorsSize();
    if (sz > 0 && serverFeatures->haveLookSelection())
    {
        if (mLook < 0)
            mLook = sz - 1;
        if (mLook > mMaxLook)
            mLook = mMinLook;
        if (mLook >= sz)
            mLook = mMinLook;
    }
    else
    {
        mLook = 0;
    }
    mPlayer->setSubtype(fromInt(mRace, BeingTypeId),
        CAST_U8(mLook));
    if (mRaceNameLabel != nullptr)
    {
        mRaceNameLabel->setCaption(item.getName());
        mRaceNameLabel->resizeTo(150, 150);
    }
    if (mLookNameLabel != nullptr)
    {
        mLookNameLabel->setCaption(item.getColorName(
            fromInt(mLook, ItemColor)));
        mLookNameLabel->resizeTo(150, 150);
    }
}

void CharCreateDialog::logic()
{
    BLOCK_START("CharCreateDialog::logic")
    if (mPlayer != nullptr)
        mPlayer->logic();
    BLOCK_END("CharCreateDialog::logic")
}

void CharCreateDialog::updatePlayer()
{
    if (mPlayer != nullptr)
    {
        mPlayer->setDirection(directions[mDirection]);
        mPlayer->setAction(actions[mAction], 0);
    }
}

void CharCreateDialog::keyPressed(KeyEvent &event)
{
    const InputActionT actionId = event.getActionId();
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (actionId)
    {
        case InputAction::GUI_CANCEL:
            event.consume();
            action(ActionEvent(mCancelButton,
                mCancelButton->getActionEventId()));
            break;

        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)
}

void CharCreateDialog::setButtonsPosition(const int w, const int h)
{
    const int h2 = h - 5 - mCancelButton->getHeight();
    if (mainGraphics->getHeight() < 480)
    {
        if (mMaxPoints != 0)
        {
            mCreateButton->setPosition(337, 160);
            mCancelButton->setPosition(337 + mCreateButton->getWidth(), 160);
        }
        else
        {
            mCancelButton->setPosition(
                w / 2,
                160);
            mCreateButton->setPosition(
                mCancelButton->getX() - 5 - mCreateButton->getWidth(),
                160);
        }
    }
    else
    {
        mCancelButton->setPosition(
            w / 2,
            h2);
        mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            h2);
    }
    mAttributesLeft->setPosition(15, h2 - mAttributesLeft->getHeight());
}
