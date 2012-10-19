/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/charcreatedialog.h"

#include "keydata.h"
#include "keyevent.h"
#include "main.h"
#include "units.h"

#include "gui/confirmdialog.h"
#include "gui/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"

#include "net/messageout.h"
#include "net/net.h"

#include "resources/chardb.h"
#include "resources/colordb.h"
#include "resources/itemdb.h"
#include "resources/iteminfo.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

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
    Window(_("New Character"), true, parent, "charcreate.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    mCharSelectDialog(parent),
    mNameField(new TextField(this, "")),
    mNameLabel(new Label(_("Name:"))),
    // TRANSLATORS: This is a narrow symbol used to denote 'next'.
    // You may change this symbol if your language uses another.
    mNextHairColorButton(new Button(this, _(">"), "nextcolor", this)),
    // TRANSLATORS: This is a narrow symbol used to denote 'previous'.
    // You may change this symbol if your language uses another.
    mPrevHairColorButton(new Button(this, _("<"), "prevcolor", this)),
    mHairColorLabel(new Label(_("Hair color:"))),
    mHairColorNameLabel(new Label("")),
    mNextHairStyleButton(new Button(this, _(">"), "nextstyle", this)),
    mPrevHairStyleButton(new Button(this, _("<"), "prevstyle", this)),
    mHairStyleLabel(new Label(_("Hair style:"))),
    mHairStyleNameLabel(new Label("")),
    mActionButton(new Button(this, _("^"), "action", this)),
    mRotateButton(new Button(this, _(">"), "rotate", this)),
    mMale(new RadioButton(_("Male"), "gender")),
    mFemale(new RadioButton(_("Female"), "gender")),
    mOther(new RadioButton(_("Other"), "gender")),
    mAttributesLeft(new Label(
        strprintf(_("Please distribute %d points"), 99))),
    mMaxPoints(0),
    mUsedPoints(0),
    mCreateButton(new Button(this, _("Create"), "create", this)),
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mRace(0),
    mPlayer(new Being(0, ActorSprite::PLAYER, static_cast<uint16_t>(mRace),
            nullptr)),
    mPlayerBox(new PlayerBox(mPlayer, "charcreate_playerbox.xml")),
    mSlot(slot),
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

    maxHairColor = CharDB::getMaxHairColor();
    minHairColor = CharDB::getMinHairColor();
    if (!maxHairColor)
        maxHairColor = ColorDB::getHairSize();

    maxHairStyle = CharDB::getMaxHairStyle();
    minHairStyle = CharDB::getMinHairStyle();
    if (!maxHairStyle)
        maxHairStyle = mPlayer->getNumOfHairstyles();

    mHairStyle = (rand() % maxHairStyle) + minHairStyle;
    mHairColor = (rand() % maxHairColor) + minHairColor;

    mNameField->setMaximum(24);

    if (serverVersion >= 2)
    {
        mNextRaceButton = new Button(this, _(">"), "nextrace", this);
        mPrevRaceButton = new Button(this, _("<"), "prevrace", this);
        mRaceLabel = new Label(_("Race:"));
        mRaceNameLabel = new Label("");
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
    mPlayerBox->setDimension(gcn::Rectangle(350, 40, 110, 90));
    mActionButton->setPosition(375, 140);
    mRotateButton->setPosition(405, 140);

    mNameLabel->setPosition(5, 10);
    mNameField->setDimension(
            gcn::Rectangle(60, 10, 300, mNameField->getHeight()));

    const int leftX = 120;
    const int rightX = 300;
    const int labelX = 5;
    const int nameX = 145;
    mPrevHairColorButton->setPosition(leftX, 40);
    mNextHairColorButton->setPosition(rightX, 40);
    mHairColorLabel->setPosition(labelX, 45);
    mHairColorNameLabel->setPosition(nameX, 45);
    mPrevHairStyleButton->setPosition(leftX, 69);
    mNextHairStyleButton->setPosition(rightX, 69);
    mHairStyleLabel->setPosition(labelX, 74);
    mHairStyleNameLabel->setPosition(nameX, 74);

    if (serverVersion >= 2)
    {
        mPrevRaceButton->setPosition(leftX, 103);
        mNextRaceButton->setPosition(rightX, 103);
        mRaceLabel->setPosition(labelX, 108);
        mRaceNameLabel->setPosition(nameX, 108);
    }

    mAttributesLeft->setPosition(15, 280);
    updateSliders();
    mCancelButton->setPosition(
            w / 2,
            h - 5 - mCancelButton->getHeight());
    mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            h - 5 - mCancelButton->getHeight());

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

    updatePlayer();

    addKeyListener(this);
}

CharCreateDialog::~CharCreateDialog()
{
    delete mPlayer;
    mPlayer = nullptr;

    // Make sure the char server handler knows that we're gone
    Net::getCharHandler()->setCharCreateDialog(nullptr);
}

void CharCreateDialog::action(const gcn::ActionEvent &event)
{
    const std::string id = event.getId();
    if (id == "create")
    {
        if (
#ifdef MANASERV_SUPPORT
            Net::getNetworkType() == ServerInfo::MANASERV ||
#endif
            getName().length() >= 4)
        {
            // Attempt to create the character
            mCreateButton->setEnabled(false);

            std::vector<int> atts;
            for (size_t i = 0, sz = mAttributeSlider.size(); i < sz; i++)
            {
                atts.push_back(static_cast<int>(
                    mAttributeSlider[i]->getValue()));
            }

#ifdef MANASERV_SUPPORT
            int characterSlot = mSlot;
            // On Manaserv, the slots start at 1, so we offset them.
            if (Net::getNetworkType() == ServerInfo::MANASERV)
                ++characterSlot;
#else
            const int characterSlot = mSlot;
#endif

            Net::getCharHandler()->newCharacter(getName(), characterSlot,
                mFemale->isSelected(), mHairStyle, mHairColor,
                static_cast<unsigned char>(mRace), atts);
        }
        else
        {
            new OkDialog(_("Error"),
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
        mAttributesLeft->setCaption(_("Character stats OK"));
        mCreateButton->setEnabled(true);
    }
    else
    {
        mCreateButton->setEnabled(false);
        if (pointsLeft > 0)
        {
            mAttributesLeft->setCaption(
                    strprintf(_("Please distribute %d points"), pointsLeft));
        }
        else
        {
            mAttributesLeft->setCaption(
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

    for (unsigned i = 0, sz = static_cast<unsigned>(labels.size());
         i < sz; i++)
    {
        mAttributeLabel[i] = new Label(labels[i]);
        mAttributeLabel[i]->setWidth(70);
        mAttributeLabel[i]->setPosition(5, 145 + i * 24);
        mAttributeLabel[i]->adjustSize();
        add(mAttributeLabel[i]);

        mAttributeSlider[i] = new Slider(min, max);
        mAttributeSlider[i]->setDimension(gcn::Rectangle(140, 145 + i * 24,
                                                         150, 12));
        mAttributeSlider[i]->setActionEventId("statslider");
        mAttributeSlider[i]->addActionListener(this);
        add(mAttributeSlider[i]);

        mAttributeValue[i] = new Label(toString(min));
        mAttributeValue[i]->setPosition(295, 145 + i * 24);
        add(mAttributeValue[i]);
    }

    mAttributesLeft->setPosition(15, 300);
    updateSliders();

    mCancelButton->setPosition(
            w / 2,
            h - 5 - mCancelButton->getHeight());
    mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            h - 5 - mCancelButton->getHeight());
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
    mHairStyle %= Being::getNumOfHairstyles();
    if (mHairStyle < 0)
        mHairStyle += Being::getNumOfHairstyles();
    if (mHairStyle < static_cast<signed>(minHairStyle)
        || mHairStyle > static_cast<signed>(maxHairStyle))
    {
        mHairStyle = minHairStyle;
    }
    const ItemInfo &item = ItemDB::get(-mHairStyle);
    mHairStyleNameLabel->setCaption(item.getName());
    mHairStyleNameLabel->adjustSize();

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
    mHairColorNameLabel->adjustSize();

    mPlayer->setSprite(Net::getCharHandler()->hairSprite(),
        mHairStyle * -1, item.getDyeColorsString(mHairColor));
}

void CharCreateDialog::updateRace()
{
    int id;
    if (mRace < 0)
    {
        mRace = 0;
        id = -100;
    }
    else
    {
        id = -100 - mRace;
        while (id < -100 && !ItemDB::exists(id))
            id ++;
        mRace = -100 - id;
    }

    mPlayer->setSubtype(static_cast<uint16_t>(mRace));
    const ItemInfo &item = ItemDB::get(id);
    mRaceNameLabel->setCaption(item.getName());
    mRaceNameLabel->adjustSize();
}

void CharCreateDialog::logic()
{
    if (mPlayer)
        mPlayer->logic();
}

void CharCreateDialog::updatePlayer()
{
    if (mPlayer)
    {
        mPlayer->setDirection(directions[mDirection]);
        mPlayer->setAction(actions[mAction]);
    }
}

void CharCreateDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    const int actionId = static_cast<KeyEvent*>(&keyEvent)->getActionId();
    switch (actionId)
    {
        case Input::KEY_GUI_CANCEL:
            keyEvent.consume();
            action(gcn::ActionEvent(mCancelButton,
                mCancelButton->getActionEventId()));
            break;

        default:
            break;
    }
}
