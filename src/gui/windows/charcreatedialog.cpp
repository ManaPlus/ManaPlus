/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "input/inputaction.h"

#include "enums/being/beingdirection.h"

#include "enums/gui/dialogtype.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/windows/charselectdialog.h"
#include "gui/widgets/label.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/tabstrip.h"
#include "gui/widgets/textfield.h"

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
    mNameField(new TextField(this, "")),
    // TRANSLATORS: char create dialog label
    mNameLabel(new Label(this, _("Name:"))),
    // TRANSLATORS: This is a narrow symbol used to denote 'next'.
    // You may change this symbol if your language uses another.
    // TRANSLATORS: char create dialog button
    mNextHairColorButton(new Button(this, _(">"), "nextcolor", this)),
    // TRANSLATORS: This is a narrow symbol used to denote 'previous'.
    // You may change this symbol if your language uses another.
    // TRANSLATORS: char create dialog button
    mPrevHairColorButton(new Button(this, _("<"), "prevcolor", this)),
    // TRANSLATORS: char create dialog label
    mHairColorLabel(new Label(this, _("Hair color:"))),
    mHairColorNameLabel(new Label(this, "")),
    // TRANSLATORS: char create dialog button
    mNextHairStyleButton(new Button(this, _(">"), "nextstyle", this)),
    // TRANSLATORS: char create dialog button
    mPrevHairStyleButton(new Button(this, _("<"), "prevstyle", this)),
    // TRANSLATORS: char create dialog label
    mHairStyleLabel(new Label(this, _("Hair style:"))),
    mHairStyleNameLabel(new Label(this, "")),
    mNextRaceButton(nullptr),
    mPrevRaceButton(nullptr),
    mRaceLabel(nullptr),
    mRaceNameLabel(nullptr),
    mNextLookButton(nullptr),
    mPrevLookButton(nullptr),
    mLookLabel(nullptr),
    mLookNameLabel(nullptr),
    // TRANSLATORS: char create dialog button
    mActionButton(new Button(this, _("^"), "action", this)),
    // TRANSLATORS: char create dialog button
    mRotateButton(new Button(this, _(">"), "rotate", this)),
    mAttributeSlider(),
    mAttributeLabel(),
    mAttributeValue(),
    mAttributesLeft(new Label(this,
        // TRANSLATORS: char create dialog label
        strprintf(_("Please distribute %d points"), 99))),
    // TRANSLATORS: char create dialog button
    mCreateButton(new Button(this, _("Create"), "create", this)),
    // TRANSLATORS: char create dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mPlayer(new Being(BeingId_zero,
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
    const std::vector<int> &items = CharDB::getDefaultItems();
    int i = 1;
    for (std::vector<int>::const_iterator it = items.begin(),
         it_end = items.end();
         it != it_end; ++ it, i ++)
    {
        mPlayer->setSprite(i, *it);
    }

    if (!maxHairColor)
        maxHairColor = ColorDB::getHairSize();
    if (!maxHairStyle)
        maxHairStyle = mPlayer->getNumOfHairstyles();

    if (maxHairStyle)
    {
        mHairStyle = (static_cast<unsigned int>(rand())
            % maxHairStyle) + minHairStyle;
    }
    else
    {
        mHairStyle = 0;
    }
    if (maxHairColor)
    {
        mHairColor = (static_cast<unsigned int>(rand())
            % maxHairColor) + minHairColor;
    }
    else
    {
        mHairColor = 0;
    }

    mNameField->setMaximum(24);

    if (serverFeatures->haveRaceSelection())
    {
        // TRANSLATORS: char create dialog button
        mNextRaceButton = new Button(this, _(">"), "nextrace", this);
        // TRANSLATORS: char create dialog button
        mPrevRaceButton = new Button(this, _("<"), "prevrace", this);
        // TRANSLATORS: char create dialog label
        mRaceLabel = new Label(this, _("Race:"));
        mRaceNameLabel = new Label(this, "");
    }
    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
    {
        // TRANSLATORS: char create dialog button
        mNextLookButton = new Button(this, _(">"), "nextlook", this);
        // TRANSLATORS: char create dialog button
        mPrevLookButton = new Button(this, _("<"), "prevlook", this);
        // TRANSLATORS: char create dialog label
        mLookLabel = new Label(this, _("Look:"));
        mLookNameLabel = new Label(this, "");
    }

    if (serverFeatures->haveCreateCharGender())
    {
        const int size = config.getIntValue("fontSize");
        mGenderStrip = new TabStrip(this,
            "gender_" + getWindowName(),
            size + 16);
        mGenderStrip->setPressFirst(false);
        mGenderStrip->addActionListener(this);
        mGenderStrip->setActionEventId("gender_");
        // TRANSLATORS: one char size female character gender
        mGenderStrip->addButton(_("F"), "f", false);
        // TRANSLATORS: one char size male character gender
        mGenderStrip->addButton(_("M"), "m", false);
        // TRANSLATORS: one char size unknown character gender
        mGenderStrip->addButton(_("U"), "u", true);
        mGenderStrip->setVisible(Visible_true);
        add(mGenderStrip);

        mGenderStrip->setPosition(385, 130);
        mGenderStrip->setWidth(500);
        mGenderStrip->setHeight(50);
    }

    mPlayerBox->setWidth(74);

    mNameField->setActionEventId("create");
    mNameField->addActionListener(this);

    mPlayerBox->setDimension(Rect(360, 0, 110, 90));
    mActionButton->setPosition(385, 100);
    mRotateButton->setPosition(415, 100);

    mNameLabel->setPosition(5, 2);
    mNameField->setDimension(
            Rect(60, 2, 300, mNameField->getHeight()));

    const int leftX = 120;
    const int rightX = 300;
    const int labelX = 5;
    const int nameX = 145;
    int y = 30;

    mPrevHairColorButton->setPosition(leftX, y);
    mNextHairColorButton->setPosition(rightX, y);
    y += 5;
    mHairColorLabel->setPosition(labelX, y);
    mHairColorNameLabel->setPosition(nameX, y);
    y += 24;
    mPrevHairStyleButton->setPosition(leftX, y);
    mNextHairStyleButton->setPosition(rightX, y);
    y += 5;
    mHairStyleLabel->setPosition(labelX, y);
    mHairStyleNameLabel->setPosition(nameX, y);

    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
    {
        y += 24;
        mPrevLookButton->setPosition(leftX, y);
        mNextLookButton->setPosition(rightX, y);
        y += 5;
        mLookLabel->setPosition(labelX, y);
        mLookNameLabel->setPosition(nameX, y);  // 93
    }
    if (serverFeatures->haveRaceSelection())
    {
        y += 24;
        mPrevRaceButton->setPosition(leftX, y);
        mNextRaceButton->setPosition(rightX, y);
        y += 5;
        mRaceLabel->setPosition(labelX, y);
        mRaceNameLabel->setPosition(nameX, y);
    }

    updateSliders();
    setButtonsPosition(w, h);

    add(mPlayerBox);
    add(mNameField);
    add(mNameLabel);
    add(mNextHairColorButton);
    add(mPrevHairColorButton);
    add(mHairColorLabel);
    add(mHairColorNameLabel);
    add(mNextHairStyleButton);
    add(mPrevHairStyleButton);
    add(mHairStyleLabel);
    add(mHairStyleNameLabel);
    add(mActionButton);
    add(mRotateButton);

    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
    {
        add(mNextLookButton);
        add(mPrevLookButton);
        add(mLookLabel);
        add(mLookNameLabel);
    }

    if (serverFeatures->haveRaceSelection())
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
    if (serverFeatures->haveRaceSelection())
        updateRace();
    if (serverFeatures->haveLookSelection() && mMinLook < mMaxLook)
        updateLook();
    updatePlayer();

    addKeyListener(this);
}

CharCreateDialog::~CharCreateDialog()
{
    delete2(mPlayer);

    if (charServerHandler)
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

            std::vector<int> atts;
            for (size_t i = 0, sz = mAttributeSlider.size(); i < sz; i++)
            {
                atts.push_back(static_cast<int>(
                    mAttributeSlider[i]->getValue()));
            }

            const int characterSlot = mSlot;

            charServerHandler->newCharacter(getName(),
                characterSlot,
                mGender,
                mHairStyle,
                mHairColor,
                static_cast<unsigned char>(mRace),
                static_cast<unsigned char>(mLook),
                atts);
        }
        else
        {
            // TRANSLATORS: char creation error
            (new OkDialog(_("Error"),
                // TRANSLATORS: char creation error
                _("Your name needs to be at least 4 characters."),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260))->postInit();
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
                toString(static_cast<int>(mAttributeSlider[i]->getValue())));
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
        points += static_cast<int>(mAttributeSlider[i]->getValue());
    return points;
}

void CharCreateDialog::setAttributes(const StringVect &labels,
                                     const int available,
                                     const int min, const int max)
{
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

    mAttributeLabel.resize(labels.size());
    mAttributeSlider.resize(labels.size());
    mAttributeValue.resize(labels.size());

    const int w = 480;
    int h = 350;
    const int y = 118 + 29;

    for (unsigned i = 0, sz = static_cast<unsigned>(labels.size());
         i < sz; i++)
    {
        mAttributeLabel[i] = new Label(this, labels[i]);
        mAttributeLabel[i]->setWidth(70);
        mAttributeLabel[i]->setPosition(5, y + i * 24);
        mAttributeLabel[i]->adjustSize();
        add(mAttributeLabel[i]);

        mAttributeSlider[i] = new Slider(this, min, max, 1.0);
        mAttributeSlider[i]->setDimension(Rect(140, y + i * 24, 150, 12));
        mAttributeSlider[i]->setActionEventId("statslider");
        mAttributeSlider[i]->addActionListener(this);
        add(mAttributeSlider[i]);

        mAttributeValue[i] = new Label(this, toString(min));
        mAttributeValue[i]->setPosition(295, y + i * 24);
        add(mAttributeValue[i]);
    }

    updateSliders();
    if (!available)
    {
        mAttributesLeft->setVisible(Visible_false);
        h = y;
        setContentSize(w, h);
    }
    if (serverFeatures->haveCreateCharGender() && y < 160)
    {
        h = 160;
        setContentSize(w, h);
    }
    setButtonsPosition(w, h);
}

void CharCreateDialog::setDefaultGender(const Gender::Type gender)
{
    mDefaultGender = gender;
    mPlayer->setGender(gender);
}

void CharCreateDialog::updateHair()
{
    if (mHairStyle <= 0)
        mHairStyle = Being::getNumOfHairstyles() - 1;
    else
        mHairStyle %= Being::getNumOfHairstyles();
    if (mHairStyle < static_cast<signed>(minHairStyle)
        || mHairStyle > static_cast<signed>(maxHairStyle))
    {
        mHairStyle = minHairStyle;
    }
    const ItemInfo &item = ItemDB::get(-mHairStyle);
    mHairStyleNameLabel->setCaption(item.getName());
    mHairStyleNameLabel->resizeTo(150, 150);

    if (ColorDB::getHairSize())
        mHairColor %= ColorDB::getHairSize();
    else
        mHairColor = 0;
    if (mHairColor < 0)
        mHairColor += ColorDB::getHairSize();
    if (mHairColor < static_cast<signed>(minHairColor)
        || mHairColor > static_cast<signed>(maxHairColor))
    {
        mHairColor = minHairColor;
    }
    mHairColorNameLabel->setCaption(ColorDB::getHairColorName(mHairColor));
    mHairColorNameLabel->resizeTo(150, 150);

    mPlayer->setSprite(charServerHandler->hairSprite(),
        mHairStyle * -1, item.getDyeColorsString(mHairColor));
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
        static_cast<uint8_t>(mLook));
    if (mRaceNameLabel)
    {
        mRaceNameLabel->setCaption(item.getName());
        mRaceNameLabel->resizeTo(150, 150);
    }
    if (mLookNameLabel)
    {
        mLookNameLabel->setCaption(item.getColorName(mLook));
        mLookNameLabel->resizeTo(150, 150);
    }
}

void CharCreateDialog::logic()
{
    BLOCK_START("CharCreateDialog::logic")
    if (mPlayer)
        mPlayer->logic();
    BLOCK_END("CharCreateDialog::logic")
}

void CharCreateDialog::updatePlayer()
{
    if (mPlayer)
    {
        mPlayer->setDirection(directions[mDirection]);
        mPlayer->setAction(actions[mAction], 0);
    }
}

void CharCreateDialog::keyPressed(KeyEvent &event)
{
    const int actionId = event.getActionId();
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
}

void CharCreateDialog::setButtonsPosition(const int w, const int h)
{
    if (mainGraphics->getHeight() < 480)
    {
        mCreateButton->setPosition(340, 150);
        mCancelButton->setPosition(340, 160 + mCreateButton->getHeight());
    }
    else
    {
        mCancelButton->setPosition(
            w / 2,
            h - 5 - mCancelButton->getHeight());
        mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            h - 5 - mCancelButton->getHeight());
    }
    mAttributesLeft->setPosition(15, 260 + 29);
}
