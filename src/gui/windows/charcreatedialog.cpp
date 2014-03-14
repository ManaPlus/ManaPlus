/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/windows/charcreatedialog.h"

#include "main.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/windows/charselectdialog.h"
#include "gui/widgets/label.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"

#include "net/net.h"

#include "resources/iteminfo.h"

#include "resources/db/chardb.h"
#include "resources/db/itemdb.h"

#include "utils/gettext.h"

#include "debug.h"

extern int serverVersion;

static const Being::Action actions[] =
{
    Being::STAND, Being::SIT, Being::MOVE, Being::ATTACK, Being::DEAD
};

static const uint8_t directions[] =
{
    Being::DOWN, Being::RIGHT, Being::UP, Being::LEFT
};

CharCreateDialog::CharCreateDialog(CharSelectDialog *const parent,
                                   const int slot) :
    // TRANSLATORS: char create dialog name
    Window(_("New Character"), true, parent, "charcreate.xml"),
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
    // TRANSLATORS: char create dialog button
    mMale(new RadioButton(this, _("Male"), "gender")),
    // TRANSLATORS: char create dialog button
    mFemale(new RadioButton(this, _("Female"), "gender")),
    // TRANSLATORS: char create dialog button
    mOther(new RadioButton(this, _("Other"), "gender")),
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
    mPlayer(new Being(0, ActorSprite::PLAYER, static_cast<uint16_t>(0U),
            nullptr)),
    mPlayerBox(new PlayerBox(this, mPlayer, "charcreate_playerbox.xml",
        "charcreate_selectedplayerbox.xml")),
    mMaxPoints(0),
    mUsedPoints(0),
    mRace(0),
    mLook(0),
    mMinLook(CharDB::getMinLook()),
    mMaxLook(CharDB::getMaxLook()),
    mHairStyle(0),
    mHairColor(0),
    mSlot(slot),
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

    mPlayer->setGender(GENDER_MALE);
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
        mHairStyle = (rand() % maxHairStyle) + minHairStyle;
    else
        mHairStyle = 0;
    if (maxHairColor)
        mHairColor = (rand() % maxHairColor) + minHairColor;
    else
        mHairColor = 0;

    mNameField->setMaximum(24);

    if (serverVersion >= 2)
    {
        // TRANSLATORS: char create dialog button
        mNextRaceButton = new Button(this, _(">"), "nextrace", this);
        // TRANSLATORS: char create dialog button
        mPrevRaceButton = new Button(this, _("<"), "prevrace", this);
        // TRANSLATORS: char create dialog label
        mRaceLabel = new Label(this, _("Race:"));
        mRaceNameLabel = new Label(this, "");
    }
    if (serverVersion >= 9 && mMinLook < mMaxLook)
    {
        // TRANSLATORS: char create dialog button
        mNextLookButton = new Button(this, _(">"), "nextlook", this);
        // TRANSLATORS: char create dialog button
        mPrevLookButton = new Button(this, _("<"), "prevlook", this);
        // TRANSLATORS: char create dialog label
        mLookLabel = new Label(this, _("Look:"));
        mLookNameLabel = new Label(this, "");
    }

    // Default to a Male character
    mMale->setSelected(true);

    mMale->setActionEventId("gender");
    mFemale->setActionEventId("gender");
    mOther->setActionEventId("gender");

    mMale->addActionListener(this);
    mFemale->addActionListener(this);
    mOther->addActionListener(this);

    mPlayerBox->setWidth(74);

    mNameField->setActionEventId("create");
    mNameField->addActionListener(this);

    const int w = 480;
    const int h = 350;

    setContentSize(w, h);
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

    if (serverVersion >= 9 && mMinLook < mMaxLook)
    {
        y += 24;
        mPrevLookButton->setPosition(leftX, y);
        mNextLookButton->setPosition(rightX, y);
        y += 5;
        mLookLabel->setPosition(labelX, y);
        mLookNameLabel->setPosition(nameX, y);  // 93
    }
    if (serverVersion >= 2)
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

    mMale->setPosition(30, 120);
    mFemale->setPosition(100, 120);
    mOther->setPosition(170, 120);

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

    if (serverVersion >= 9 && mMinLook < mMaxLook)
    {
        add(mNextLookButton);
        add(mPrevLookButton);
        add(mLookLabel);
        add(mLookNameLabel);
    }

    if (serverVersion >= 2)
    {
        add(mNextRaceButton);
        add(mPrevRaceButton);
        add(mRaceLabel);
        add(mRaceNameLabel);
    }

    add(mAttributesLeft);
    add(mCreateButton);
    add(mCancelButton);

    add(mMale);
    add(mFemale);
    add(mOther);

    center();
    setVisible(true);
    mNameField->requestFocus();

    updateHair();
    if (serverVersion >= 2)
        updateRace();
    if (serverVersion >= 9 && mMinLook < mMaxLook)
        updateLook();
    updatePlayer();

    addKeyListener(this);
}

CharCreateDialog::~CharCreateDialog()
{
    delete mPlayer;
    mPlayer = nullptr;

    if (Net::getCharServerHandler())
        Net::getCharServerHandler()->setCharCreateDialog(nullptr);
}

void CharCreateDialog::action(const ActionEvent &event)
{
    const std::string id = event.getId();
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

            Net::getCharServerHandler()->newCharacter(getName(), characterSlot,
                mFemale->isSelected(), mHairStyle, mHairColor,
                static_cast<unsigned char>(mRace), mLook, atts);
        }
        else
        {
            // TRANSLATORS: char creation error
            new OkDialog(_("Error"),
                // TRANSLATORS: char creation error
                _("Your name needs to be at least 4 characters."),
                DIALOG_ERROR, true,  this);
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
    else if (id == "gender")
    {
        if (mMale->isSelected())
            mPlayer->setGender(GENDER_MALE);
        else
            mPlayer->setGender(GENDER_FEMALE);
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

    for (unsigned i = 0; i < mAttributeLabel.size(); i++)
    {
        remove(mAttributeLabel[i]);
        delete mAttributeLabel[i];
        mAttributeLabel[i] = nullptr;
        remove(mAttributeSlider[i]);
        delete mAttributeSlider[i];
        mAttributeSlider[i] = nullptr;
        remove(mAttributeValue[i]);
        delete mAttributeValue[i];
        mAttributeValue[i] = nullptr;
    }

    mAttributeLabel.resize(labels.size());
    mAttributeSlider.resize(labels.size());
    mAttributeValue.resize(labels.size());

    const int w = 480;
    const int h = 350;
    const int y = 118 + 29;

    for (unsigned i = 0, sz = static_cast<unsigned>(labels.size());
         i < sz; i++)
    {
        mAttributeLabel[i] = new Label(this, labels[i]);
        mAttributeLabel[i]->setWidth(70);
        mAttributeLabel[i]->setPosition(5, y + i * 24);
        mAttributeLabel[i]->adjustSize();
        add(mAttributeLabel[i]);

        mAttributeSlider[i] = new Slider(this, min, max);
        mAttributeSlider[i]->setDimension(Rect(140, y + i * 24,
                                                         150, 12));
        mAttributeSlider[i]->setActionEventId("statslider");
        mAttributeSlider[i]->addActionListener(this);
        add(mAttributeSlider[i]);

        mAttributeValue[i] = new Label(this, toString(min));
        mAttributeValue[i]->setPosition(295, y + i * 24);
        add(mAttributeValue[i]);
    }

    updateSliders();
    setButtonsPosition(w, h);
}

void CharCreateDialog::setFixedGender(const bool fixed, const Gender gender)
{
    if (gender == GENDER_FEMALE)
    {
        mFemale->setSelected(true);
        mMale->setSelected(false);
        mOther->setSelected(false);
    }
    else if (gender == GENDER_MALE)
    {
        mFemale->setSelected(false);
        mMale->setSelected(true);
        mOther->setSelected(false);
    }
    else
    {
        mFemale->setSelected(false);
        mMale->setSelected(false);
        mOther->setSelected(true);
    }

    mPlayer->setGender(gender);

    if (fixed)
    {
        mMale->setVisible(false);
        mFemale->setVisible(false);
        mOther->setVisible(false);
    }
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

    mPlayer->setSprite(Net::getCharServerHandler()->hairSprite(),
        mHairStyle * -1, item.getDyeColorsString(mHairColor));
}

void CharCreateDialog::updateRace()
{
    if (mRace < 0)
        mRace = Being::getNumOfRaces() - 1;
    else if (mRace >= Being::getNumOfRaces())
        mRace = 0;

    updateLook();
}

void CharCreateDialog::updateLook()
{
    const ItemInfo &item = ItemDB::get(-100 - mRace);
    const int sz = item.getColorsSize();
    if (sz > 0 && serverVersion >= 9)
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
    mPlayer->setSubtype(static_cast<uint16_t>(mRace), mLook);
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

void CharCreateDialog::keyPressed(KeyEvent &keyEvent)
{
    const int actionId = keyEvent.getActionId();
    switch (actionId)
    {
        case Input::KEY_GUI_CANCEL:
            keyEvent.consume();
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
