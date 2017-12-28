/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WINDOWS_CHARCREATEDIALOG_H
#define GUI_WINDOWS_CHARCREATEDIALOG_H

#include "enums/being/gender.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class Being;
class Button;
class CharSelectDialog;
class Label;
class PlayerBox;
class Slider;
class TabStrip;
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
        ~CharCreateDialog() override final;

        void action(const ActionEvent &event) override final;

        /**
         * Unlocks the dialog, enabling the create character button again.
         */
        void unlock();

        void setAttributes(const StringVect &labels,
                           int available,
                           const int min, const int max);

        void setDefaultGender(const GenderT gender);

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

        TextField *mNameField A_NONNULLPOINTER;
        Label *mNameLabel A_NONNULLPOINTER;
        Button *mNextHairColorButton A_NONNULLPOINTER;
        Button *mPrevHairColorButton A_NONNULLPOINTER;
        Label *mHairColorLabel A_NONNULLPOINTER;
        Label *mHairColorNameLabel A_NONNULLPOINTER;
        Button *mNextHairStyleButton A_NONNULLPOINTER;
        Button *mPrevHairStyleButton A_NONNULLPOINTER;
        Label *mHairStyleLabel A_NONNULLPOINTER;
        Label *mHairStyleNameLabel A_NONNULLPOINTER;
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

        STD_VECTOR<Slider*> mAttributeSlider;
        STD_VECTOR<Label*> mAttributeLabel;
        STD_VECTOR<Label*> mAttributeValue;
        Label *mAttributesLeft;

        Button *mCreateButton;
        Button *mCancelButton;

        Being *mPlayer;
        PlayerBox *mPlayerBox;

        TabStrip *mGenderStrip;

        int mMaxPoints;
        int mUsedPoints;

        int mRace;
        int mLook;
        int mMinLook;
        int mMaxLook;
        int mMinRace;
        int mMaxRace;

        int mHairStyle;
        int mHairColor;
        uint32_t mMaxY;

        int mSlot;

        GenderT mDefaultGender;
        GenderT mGender;

        unsigned int maxHairColor;
        unsigned int minHairColor;
        unsigned int maxHairStyle;
        unsigned int minHairStyle;

        unsigned int mAction;
        unsigned int mDirection;
};

#endif  // GUI_WINDOWS_CHARCREATEDIALOG_H
