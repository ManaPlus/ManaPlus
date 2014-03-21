/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_CHARCREATEDIALOG_H
#define GUI_WINDOWS_CHARCREATEDIALOG_H

#include "being/being.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class Button;
class CharSelectDialog;
class Label;
class PlayerBox;
class RadioButton;
class Slider;
class TextField;

/**
 * Character creation dialog.
 *
 * \ingroup Interface
 */
class CharCreateDialog final : public Window,
                               public ActionListener,
                               public KeyListener
{
    public:
        /**
         * Constructor.
         */
        CharCreateDialog(CharSelectDialog *const parent, const int slot);

        A_DELETE_COPY(CharCreateDialog)

        /**
         * Destructor.
         */
        ~CharCreateDialog();

        void action(const ActionEvent &event) override final;

        /**
         * Unlocks the dialog, enabling the create character button again.
         */
        void unlock();

        void setAttributes(const StringVect &labels,
                           const int available,
                           const int min, const int max);

        void setFixedGender(const bool fixed,
                            const Gender gender = GENDER_FEMALE);

        void logic() override final;

        void updatePlayer();

        void keyPressed(KeyEvent &event) override final;

    private:
        int getDistributedPoints() const A_WARN_UNUSED;

        void updateSliders();

        void setButtonsPosition(const int w, const int h);

        /**
         * Returns the name of the character to create.
         */
        std::string getName() const A_WARN_UNUSED;

        /**
         * Communicate character creation to the server.
         */
        void attemptCharCreate();

        void updateHair();

        void updateRace();

        void updateLook();

        CharSelectDialog *mCharSelectDialog;

        TextField *mNameField;
        Label *mNameLabel;
        Button *mNextHairColorButton;
        Button *mPrevHairColorButton;
        Label *mHairColorLabel;
        Label *mHairColorNameLabel;
        Button *mNextHairStyleButton;
        Button *mPrevHairStyleButton;
        Label *mHairStyleLabel;
        Label *mHairStyleNameLabel;
        Button *mNextRaceButton;
        Button *mPrevRaceButton;
        Label *mRaceLabel;
        Label *mRaceNameLabel;
        Button *mNextLookButton;
        Button *mPrevLookButton;
        Label *mLookLabel;
        Label *mLookNameLabel;

        Button *mActionButton;
        Button *mRotateButton;

        RadioButton *mMale;
        RadioButton *mFemale;
        RadioButton *mOther;

        std::vector<Slider*> mAttributeSlider;
        std::vector<Label*> mAttributeLabel;
        std::vector<Label*> mAttributeValue;
        Label *mAttributesLeft;

        Button *mCreateButton;
        Button *mCancelButton;

        Being *mPlayer;
        PlayerBox *mPlayerBox;

        int mMaxPoints;
        int mUsedPoints;

        int mRace;
        int mLook;
        int mMinLook;
        int mMaxLook;

        int mHairStyle;
        int mHairColor;

        int mSlot;

        unsigned int maxHairColor;
        unsigned int minHairColor;
        unsigned int maxHairStyle;
        unsigned int minHairStyle;

        unsigned int mAction;
        unsigned int mDirection;
};

#endif  // GUI_WINDOWS_CHARCREATEDIALOG_H
