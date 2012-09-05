/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef CHAR_CREATE_DIALOG_H
#define CHAR_CREATE_DIALOG_H

#include "being.h"

#include "gui/charselectdialog.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>

class LocalPlayer;
class PlayerBox;
class TextField;

namespace gcn
{
    class Label;
    class RadioButton;
    class Slider;
}

/**
 * Character creation dialog.
 *
 * \ingroup Interface
 */
class CharCreateDialog : public Window,
                         public gcn::ActionListener,
                         public gcn::KeyListener
{
    public:
        /**
         * Constructor.
         */
        CharCreateDialog(CharSelectDialog *const parent, const int slot);

        /**
         * Destructor.
         */
        ~CharCreateDialog();

        void action(const gcn::ActionEvent &event);

        /**
         * Unlocks the dialog, enabling the create character button again.
         */
        void unlock();

        void setAttributes(const StringVect &labels,
                           const int available,
                           const int min, const int max);

        void setFixedGender(const bool fixed,
                            const Gender gender = GENDER_FEMALE);

        void logic();

        void updatePlayer();

        void keyPressed(gcn::KeyEvent &keyEvent);

    private:
        int getDistributedPoints() const;

        void updateSliders();

        /**
         * Returns the name of the character to create.
         */
        std::string getName() const;

        /**
         * Communicate character creation to the server.
         */
        void attemptCharCreate();

        void updateHair();

        void updateRace();

        CharSelectDialog *mCharSelectDialog;

        TextField *mNameField;
        gcn::Label *mNameLabel;
        gcn::Button *mNextHairColorButton;
        gcn::Button *mPrevHairColorButton;
        gcn::Label *mHairColorLabel;
        gcn::Label *mHairColorNameLabel;
        gcn::Button *mNextHairStyleButton;
        gcn::Button *mPrevHairStyleButton;
        gcn::Label *mHairStyleLabel;
        gcn::Label *mHairStyleNameLabel;
        gcn::Button *mNextRaceButton;
        gcn::Button *mPrevRaceButton;
        gcn::Label *mRaceLabel;
        gcn::Label *mRaceNameLabel;

        gcn::Button *mActionButton;
        gcn::Button *mRotateButton;

        gcn::RadioButton *mMale;
        gcn::RadioButton *mFemale;
        gcn::RadioButton *mOther;

        std::vector<gcn::Slider*> mAttributeSlider;
        std::vector<gcn::Label*> mAttributeLabel;
        std::vector<gcn::Label*> mAttributeValue;
        gcn::Label *mAttributesLeft;

        int mMaxPoints;
        int mUsedPoints;

        gcn::Button *mCreateButton;
        gcn::Button *mCancelButton;

        int mRace;

        Being *mPlayer;
        PlayerBox *mPlayerBox;

        int mHairStyle;
        int mHairColor;

        int mSlot;

        unsigned maxHairColor;
        unsigned minHairColor;
        unsigned maxHairStyle;
        unsigned minHairStyle;

        unsigned mAction;
        unsigned mDirection;
};

#endif // CHAR_CREATE_DIALOG_H
